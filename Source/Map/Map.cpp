#include "Map.hpp"

namespace map
{

Map::Map(ChunkGenerator::Ptr generator)
: mGenerator(std::move(generator))
{
    if (mGenerator != nullptr)
    {
        mGenerator->mMap = this;
    }
}

Map::~Map()
{
    if (mGenerator != nullptr)
    {
        mGenerator->mMap = nullptr;
        mGenerator = nullptr;
    }
}

void Map::load()
{
    TilesetManager::load(Properties::getPath());
    std::ifstream file(Properties::getPath() + "map.dat");
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
                            int id;
                            {
                                std::istringstream iss(temp.substr(0,find));
                                iss >> id;
                            }
                            float ts;
                            {
                                std::istringstream iss(temp.substr(find+1,temp.size()-find-1));
                                iss >> ts;
                            }
                            a.addFrame(id,sf::seconds(ts));
                        }
                    }
                    addAnimation(a);
                }
            }
        }
    }
    file.close();
}

void Map::save()
{
    TilesetManager::save(Properties::getPath());
    std::ofstream file(Properties::getPath() + "map.dat");
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

bool Map::isChunkLoaded(sf::Vector2i const& chunkCoords)
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

sf::Time Map::getChunkLoadedTime() const
{
    return sf::seconds(10);
}

void Map::update(sf::View const& view)
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
    std::array<sf::Vector2f,4> arrayF;
    arrayF[0] = sf::Vector2f(viewRect.left,viewRect.top);
    arrayF[1] = sf::Vector2f(viewRect.left+viewRect.width,viewRect.top);
    arrayF[2] = sf::Vector2f(viewRect.left+viewRect.width,viewRect.top+viewRect.height);
    arrayF[3] = sf::Vector2f(viewRect.left,viewRect.top+viewRect.height);
    std::array<sf::Vector2i,4> arrayI;
    for (std::size_t i = 0; i < arrayF.size(); i++)
    {
        arrayI[i] = Properties::worldToChunk(arrayF[i]);
    }
    sf::Vector2i coords;
    for (coords.x = arrayI[0].x; coords.x <= arrayI[2].x; coords.x++)
    {
        for (coords.y = arrayI[0].y; coords.y <= arrayI[2].y; coords.y++)
        {
            if (!isChunkLoaded(coords))
            {
                mChunks.emplace_back(*mGenerator, coords);
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

void Map::animate(int oldId, int newId)
{
    std::size_t cSize = mChunks.size();
    sf::Vector2i size = Properties::getChunkSize();
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

void Map::render(sf::RenderTarget& target, sf::View const& view, int layer)
{
    sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - 0.5f * view.getSize(),view.getSize());
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i].getBounds().intersects(viewRect))
        {
            mChunks[i].render(target,layer);
        }
    }
}

void Map::setTileId(sf::Vector2i const& coords, int z, int id)
{
    sf::Vector2i cCoords = Properties::worldToChunk(Properties::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = Properties::worldToLocalCoords(Properties::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        getChunk(cCoords).setTileId(tCoords,z,id);
    }
}

int Map::getTileId(sf::Vector2i const& coords, int z)
{
    sf::Vector2f world = Properties::globalCoordsToWorld(coords);
    sf::Vector2i cCoords = Properties::worldToChunk(world);
    sf::Vector2i tCoords = Properties::worldToLocalCoords(world);
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).getTileId(tCoords,z);
    }
    return 0;
}

bool Map::isCollide(sf::Vector2i const& coords, int z)
{
    sf::Vector2i cCoords = Properties::worldToChunk(Properties::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = Properties::worldToLocalCoords(Properties::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).isCollide(tCoords,z);
    }
    return false;
}

const sf::ConvexShape& Map::getShape(sf::Vector2i const& coords, int z)
{
    sf::Vector2i cCoords = Properties::worldToChunk(Properties::globalCoordsToWorld(coords));
    sf::Vector2i tCoords = Properties::worldToLocalCoords(Properties::globalCoordsToWorld(coords));
    if (isChunkLoaded(cCoords))
    {
        return getChunk(cCoords).getShape(tCoords,z);
    }
    return sf::ConvexShape();
}

Chunk& Map::getChunk(sf::Vector2i const& coords)
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

void Map::addAnimation(Animation a)
{
    mAnimations.push_back(a);
}

} // namespace map
