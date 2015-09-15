#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

#include <vector>
#include <fstream>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "../../Aharos/Helper/Utility.hpp"

#include "TilesetManager.hpp"
#include "Chunk.hpp"
#include "Animation.hpp"

namespace map
{

template <typename T, class G>
class Map
{
    public:
        Map();

        void load();
        void save();

        bool isChunkLoaded(sf::Vector2i const& chunkCoords);
        sf::Time getChunkLoadedTime() const;

        void update(sf::View const& view);
        void animate(int oldId, int newId);

        void setTileId(sf::Vector2i const& coords, int z, int id);
        int getTileId(sf::Vector2i const& coords, int z);
        bool isCollide(sf::Vector2i const& coords, int z);
        const sf::ConvexShape& getShape(sf::Vector2i const& coords, int z);

        Chunk<T,G>& getChunk(sf::Vector2i const& coords);

        void addAnimation(Animation a);


        // TODO : Online Map

    private:
        std::vector<Chunk<T,G>> mChunks;
        std::vector<Animation> mAnimations;
};

template <typename T, typename G>
Map<T,G>::Map()
{
    G::setMap(this);
}

template <typename T, typename G>
void Map<T,G>::load()
{
    TilesetManager::load(T::getPath());
    std::ifstream file(T::getPath() + "map.dat");
    if (file)
    {
        std::string line;
        while (std::getline(file,line))
        {
            if (line.size() > 0)
            {
                if (line[0] == 'a')
                {
                    line.erase(0,2);
                    Animation a;
                    std::stringstream ss(line);
                    std::string temp;
                    while (std::getline(ss,temp,';'))
                    {
                        std::size_t find = temp.find('-');
                        if (find != std::string::npos)
                        {
                            int id = lp::from_string<int>(temp.substr(0,find));
                            sf::Time t = sf::seconds(lp::from_string<float>(temp.substr(find+1,temp.size()-find-1)));
                            a.addFrame(id,t);
                        }
                    }
                    addAnimation(a);
                }
            }
        }
    }
    file.close();
}

template <typename T, typename G>
void Map<T,G>::save()
{
    TilesetManager::save(T::getPath());
    std::ofstream file(T::getPath() + "map.dat");
    if (file)
    {
        for (std::size_t i = 0; i < mAnimations.size(); i++)
        {
            file << "a ";
            for (std::size_t j = 0; j < mAnimations[i].getFrameCount(); j++)
            {
                auto pair = mAnimations[i].getFrame(j);
                file << pair.first << "-" << pair.second.asSeconds() << ";";
            }
            file << std::endl;
        }
    }
    file.close();
}

template <typename T, typename G>
bool Map<T,G>::isChunkLoaded(sf::Vector2i const& chunkCoords)
{
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i].getCoords() == chunkCoords)
        {
            return true;
        }
    }
    return false;
}

template <typename T, typename G>
sf::Time Map<T,G>::getChunkLoadedTime() const
{
    return sf::seconds(10);
}

template <typename T, typename G>
void Map<T,G>::update(sf::View const& view)
{
    sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - 0.5f * view.getSize(),view.getSize());

    // Update Chunks Timer
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i].getBounds().intersects(viewRect))
        {
            mChunks[i].resetTimer();
        }
        if (mChunks[i].getElapsedTime() > getChunkLoadedTime())
        {
            mChunks[i].removeLayers();
            mChunks.erase(mChunks.begin() + i);
        }
    }

    // Load New Chunks
    std::array<sf::Vector2f,4> arrayF = lp::getPoint<float>(viewRect);
    std::array<sf::Vector2i,4> arrayI;
    for (std::size_t i = 0; i < arrayF.size(); i++)
    {
        arrayI[i] = T::worldToChunk(arrayF[i]);
    }
    sf::Vector2i coords;
    for (coords.x = arrayI[0].x; coords.x <= arrayI[2].x; coords.x++)
    {
        for (coords.y = arrayI[0].y; coords.y <= arrayI[2].y; coords.y++)
        {
            if (!isChunkLoaded(coords))
            {
                mChunks.emplace_back(coords);
            }
        }
    }

    // Update Animations
    for (std::size_t i = 0; i < mAnimations.size(); i++)
    {
        sf::Vector2i pair = mAnimations[i].update();
        if (pair != sf::Vector2i())
        {
            animate(pair.x,pair.y);
        }
    }
}

template <typename T, typename G>
void Map<T,G>::animate(int oldId, int newId)
{
    std::size_t cSize = mChunks.size();
    sf::Vector2i size = T::getChunkSize();
    for (std::size_t c = 0; c < cSize; c++)
    {
        std::size_t lSize = mChunks[c].getLayerCount();
        for (std::size_t z = 0; z < lSize; z++)
        {
            sf::Vector2i coords;
            for (coords.x = 0; coords.x < size.x; coords.x++)
            {
                for (coords.y = 0; coords.y < size.y; coords.y++)
                {
                    if (mChunks[c].getTileId(coords,z) == oldId)
                    {
                        mChunks[c].setTileId(coords,z,newId);
                    }
                }
            }
        }
    }
}

template <typename T, typename G>
void Map<T,G>::setTileId(sf::Vector2i const& coords, int z, int id)
{
    sf::Vector2i cCoords = T::worldToChunk(T::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = T::worldToLocalCoords(T::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        getChunk(cCoords).setTileId(tCoords,z,id);
    }
}

template <typename T, typename G>
int Map<T,G>::getTileId(sf::Vector2i const& coords, int z)
{
    sf::Vector2f world = T::globalCoordsToWorld(coords);
    sf::Vector2i cCoords = T::worldToChunk(world);
    sf::Vector2i tCoords = T::worldToLocalCoords(world);
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).getTileId(tCoords,z);
    }
    return 0;
}

template <typename T, typename G>
bool Map<T,G>::isCollide(sf::Vector2i const& coords, int z)
{
    sf::Vector2i cCoords = T::worldToChunk(T::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = T::worldToLocalCoords(T::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).isCollide(tCoords,z);
    }
    return false;
}

template <typename T, typename G>
const sf::ConvexShape& Map<T,G>::getShape(sf::Vector2i const& coords, int z)
{
    sf::Vector2i cCoords = T::worldToChunk(T::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = T::worldToLocalCoords(T::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).getShape(tCoords,z);
    }
    return sf::ConvexShape();
}

template <typename T, typename G>
Chunk<T,G>& Map<T,G>::getChunk(sf::Vector2i const& coords)
{
    assert(isChunkLoaded(coords));
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i].getCoords() == coords)
        {
            return mChunks[i];
        }
    }
}

template <typename T, typename G>
void Map<T,G>::addAnimation(Animation a)
{
    mAnimations.push_back(a);
}

} // namespace map

#endif // MAP_MAP_HPP
