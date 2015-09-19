#include "Chunk.hpp"

namespace map
{

Chunk::Chunk(ChunkGenerator& generator, sf::Vector2i const& coords)
: mCoords(coords)
{
    mSaveNeeded = false;

    if (!load())
    {
        generator.generate(*this);
        save();
    }
}

Chunk::~Chunk()
{
    if (mSaveNeeded)
    {
        save();
    }
}

void Chunk::addLayer()
{
    mLayers.push_back(Layer(mCoords));
}

void Chunk::removeLayers()
{
    mLayers.clear();
}

std::size_t Chunk::getLayerCount() const
{
    return mLayers.size();
}

void Chunk::render(sf::RenderTarget& target, int layer)
{
    if (layer >= 0 && layer < (int)mLayers.size())
    {
        mLayers.at(layer).render(target);
    }
}

sf::Vector2i Chunk::getCoords() const
{
    return mCoords;
}

sf::FloatRect Chunk::getBounds() const
{
    sf::Vector2f size = Properties::getChunkSizePx();
    return sf::FloatRect(Properties::chunkToWorld(mCoords), size);
}

void Chunk::setTileId(sf::Vector2i const& coords, int layer, int id)
{
    if (layer >= 0 && layer < (int)mLayers.size())
    {
        mLayers.at(layer).setTileId(coords,id);
        mSaveNeeded = true;
    }
}

int Chunk::getTileId(sf::Vector2i const& coords, int layer) const
{
    if (layer >= 0 && layer < (int)mLayers.size())
    {
        return mLayers.at(layer).getTileId(coords);
    }
    else
    {
        return 0;
    }
}

bool Chunk::isCollide(sf::Vector2i const& coords, int layer) const
{
    if (layer >= 0 && layer < (int)mLayers.size())
    {
        return mLayers.at(layer).isCollide(coords);
    }
    else
    {
        return false;
    }
}

const sf::ConvexShape& Chunk::getShape(sf::Vector2i const& coords, int layer) const
{
    if (layer >= 0 && layer < (int)mLayers.size())
    {
        return mLayers.at(layer).getShape(coords);
    }
    else
    {
        return sf::ConvexShape();
    }
}

void Chunk::resetTimer()
{
    mClock.restart();
}

sf::Time Chunk::getElapsedTime() const
{
    return mClock.getElapsedTime();
}

bool Chunk::load()
{
    std::ifstream file(getPathFromCoords(mCoords));
    if (!file)
    {
        mSaveNeeded = true;
        return false;
    }

    std::string line;
    std::size_t layerCount;
    std::getline(file,line);
    {
        std::istringstream iss(line);
        iss >> layerCount;
    }
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
                int id;
                {
                    std::istringstream iss(temp2);
                    iss >> id;
                }
                setTileId(coords,z,id);
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

void Chunk::save()
{
    std::ofstream file(getPathFromCoords(mCoords));
    if (!file)
    {
        return;
    }

    file << getLayerCount() << std::endl;
    sf::Vector2i coords, size = Properties::getChunkSize();
    for (coords.y = 0; coords.y < size.y; coords.y++)
    {
        for (coords.x = 0; coords.x < size.x; coords.x++)
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

void Chunk::loadFromPacket(sf::Packet& packet)
{
    // packet >> mCoords.x >> mCoords.y;   has already been read by the manager

    std::size_t layerCount;
    packet >> layerCount;
    for (std::size_t i = 0; i < layerCount; i++)
    {
        addLayer();
    }

    sf::Vector2i p;
    for (p.x = 0; p.x < Properties::getChunkSize().x; p.x++)
    {
        for (p.y = 0; p.y < Properties::getChunkSize().y; p.y++)
        {
            for (int z = 0; z < (int)getLayerCount(); z++)
            {
                int id;
                packet >> id;
                setTileId(p,z,id);
            }
        }
    }
}

void Chunk::writeToPacket(sf::Packet& packet)
{
    packet << mCoords.x << mCoords.y;
    packet << getLayerCount();

    sf::Vector2i p, size = Properties::getChunkSize();
    for (p.x = 0; p.x < size.x; p.x++)
    {
        for (p.y = 0; p.y < size.y; p.y++)
        {
            for (int z = 0; z < (int)getLayerCount(); z++)
            {
                packet << getTileId(p,z);
            }
        }
    }
}

std::string Chunk::getPathFromCoords(sf::Vector2i const& coords)
{
    std::ostringstream oss;
    oss << Properties::getPath() << coords.x << "_" << coords.y << ".chunk";
    return oss.str();
}

} // namespace map
