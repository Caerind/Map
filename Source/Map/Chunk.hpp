#ifndef MAP_CHUNK_HPP
#define MAP_CHUNK_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/Packet.hpp>

#include "../../Aharos/Helper/String.hpp"
#include "../../Aharos/Renderer/Renderer.hpp"

#include "Layer.hpp"

namespace map
{

template <typename T, typename G>
class Chunk
{
    public:
        Chunk(sf::Vector2i const& coords);
        virtual ~Chunk();

        void addLayer();
        void removeLayers();
        std::size_t getLayerCount() const;

        sf::Vector2i getCoords() const;
        sf::FloatRect getBounds() const;

        void setTileId(sf::Vector2i const& coords, int z, int id);
        int getTileId(sf::Vector2i const& coords, int z) const;

        bool isCollide(sf::Vector2i const& coords, int z) const;
        const sf::ConvexShape& getShape(sf::Vector2i const& coords, int z) const;

        void resetTimer();
        sf::Time getElapsedTime() const;

        bool load();
        void generate();
        void save();

        void loadFromPacket(sf::Packet& packet);
        void writeToPacket(sf::Packet& packet);

    private:
        sf::Vector2i mCoords;
        std::vector<Layer<T>*> mLayers;
        sf::Clock mClock;
        bool mSaveNeeded;
};

template <typename T, typename G>
Chunk<T,G>::Chunk(sf::Vector2i const& coords)
: mCoords(coords)
{
    mSaveNeeded = false;

    if (!load())
    {
        G::generate(*this);
        mSaveNeeded = true;
    }
}

template <typename T, typename G>
Chunk<T,G>::~Chunk()
{
    if (mSaveNeeded)
    {
        save();
    }
}

template <typename T, typename G>
void Chunk<T,G>::addLayer()
{
    mLayers.push_back(new Layer<T>(mCoords,mLayers.size()));
    rd::Renderer::add(mLayers.back());
}

template <typename T, typename G>
void Chunk<T,G>::removeLayers()
{
    for (std::size_t i = 0; i < getLayerCount(); i++)
    {
        rd::Renderer::remove(mLayers[i]);
        mLayers[i] = nullptr;
        delete mLayers[i];
    }
    mLayers.clear();
}

template <typename T, typename G>
std::size_t Chunk<T,G>::getLayerCount() const
{
    return mLayers.size();
}

template <typename T, typename G>
sf::Vector2i Chunk<T,G>::getCoords() const
{
    return mCoords;
}

template <typename T, typename G>
sf::FloatRect Chunk<T,G>::getBounds() const
{
    sf::Vector2f size = T::getChunkSizePx();
    return sf::FloatRect(T::chunkToWorld(mCoords), size);
}

template <typename T, typename G>
void Chunk<T,G>::setTileId(sf::Vector2i const& coords, int z, int id)
{
    if (z >= 0 && z < (int)mLayers.size())
    {
        mLayers[z]->setTileId(coords,id);
        mSaveNeeded = true;
    }
}

template <typename T, typename G>
int Chunk<T,G>::getTileId(sf::Vector2i const& coords, int z) const
{
    if (z >= 0 && z < (int)mLayers.size())
    {
        return mLayers[z]->getTileId(coords);
    }
    else
    {
        return 0;
    }
}

template <typename T, typename G>
bool Chunk<T,G>::isCollide(sf::Vector2i const& coords, int z) const
{
    if (z >= 0 && z < (int)mLayers.size())
    {
        return mLayers[z]->isCollide(coords);
    }
    else
    {
        return false;
    }
}

template <typename T, typename G>
const sf::ConvexShape& Chunk<T,G>::getShape(sf::Vector2i const& coords, int z) const
{
    if (z >= 0 && z < (int)mLayers.size())
    {
        return mLayers[z]->getShape(coords);
    }
    else
    {
        return sf::ConvexShape();
    }
}

template <typename T, typename G>
void Chunk<T,G>::resetTimer()
{
    mClock.restart();
}

template <typename T, typename G>
sf::Time Chunk<T,G>::getElapsedTime() const
{
    return mClock.getElapsedTime();
}

template <typename T, typename G>
bool Chunk<T,G>::load()
{
    std::ifstream file(T::getPath() + lp::to_string(mCoords.x) + "_" + lp::to_string(mCoords.y) + ".chunk");
    if (!file)
    {
        mSaveNeeded = true;
        return false;
    }

    std::string line;
    std::getline(file,line);
    std::size_t layerCount = lp::from_string<std::size_t>(line);
    for (std::size_t i = 0; i < layerCount; i++)
    {
        addLayer();
    }

    sf::Vector2i coords = sf::Vector2i();
    int z = 0;
    while (std::getline(file,line))
    {
        std::stringstream ss1(line);
        std::string temp1;
        coords.x = 0;
        while (std::getline(ss1,temp1,';'))
        {
            std::stringstream ss2(temp1);
            std::string temp2;
            z = 0;
            while (std::getline(ss2,temp2,'-'))
            {
                setTileId(coords,z,lp::from_string<int>(temp2));
                z++;
            }
            coords.x++;
        }
        coords.y++;
    }
    file.close();
    mSaveNeeded = false;
    return true;
}

template <typename T, typename G>
void Chunk<T,G>::save()
{
    std::ofstream file(T::getPath() + lp::to_string(mCoords.x) + "_" + lp::to_string(mCoords.y) + ".chunk");
    if (!file)
    {
        return;
    }
    file << getLayerCount() << std::endl;
    sf::Vector2i coords;
    for (coords.y = 0; coords.y < T::getChunkSize().y; coords.y++)
    {
        for (coords.x = 0; coords.x < T::getChunkSize().x; coords.x++)
        {
            for (std::size_t z = 0; z < getLayerCount(); z++)
            {
                file << getTileId(coords,z);
                if (z == getLayerCount() - 1)
                {
                    file << ";";
                }
                else
                {
                    file << "-";
                }
            }
        }
        file << std::endl;
    }
    file.close();
    mSaveNeeded = false;
}

template <typename T, typename G>
void Chunk<T,G>::loadFromPacket(sf::Packet& packet)
{
    // packet >> mCoords.x >> mCoords.y;   has already been read by the manager

    std::size_t layerCount;
    packet >> layerCount;
    for (std::size_t i = 0; i < layerCount; i++)
    {
        addLayer();
    }

    sf::Vector2i p;
    for (p.x = 0; p.x < T::getChunkSize().x; p.x++)
    {
        for (p.y = 0; p.y < T::getChunkSize().y; p.y++)
        {
            for (int k = 0; k < getLayerCount(); k++)
            {
                int id;
                packet >> id;
                setTileId(p,k,id);
            }
        }
    }
}

template <typename T, typename G>
void Chunk<T,G>::writeToPacket(sf::Packet& packet)
{
    packet << mCoords.x << mCoords.y;
    packet << getLayerCount();

    sf::Vector2i p;
    for (p.x = 0; p.x < T::getChunkSize().x; p.x++)
    {
        for (p.y = 0; p.y < T::getChunkSize().y; p.y++)
        {
            for (int k = 0; k < getLayerCount(); k++)
            {
                packet << getTileId(p,k);
            }
        }
    }
}

} // namespace map

#endif // MAP_CHUNK_HPP
