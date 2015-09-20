#include "Map.hpp"

namespace map
{

Map::Map(ChunkGenerator* generator)
: mType(Map::Type::ClientSolo)
, mGenerator(generator)
, mSocket(nullptr)
{
    if (mGenerator == nullptr)
    {
        mGenerator = new ChunkGenerator();
    }
    mGenerator->mMap = this;
    mAnimations = TilesetManager::getAnimations();
}

Map::Map(sf::TcpSocket* socket)
: mType(Map::Type::ClientOnline)
, mGenerator(nullptr)
, mSocket(socket)
{
    if (mSocket == nullptr)
    {
        mSocket = new sf::TcpSocket();
    }
    mAnimations = TilesetManager::getAnimations();
}

Map::Map(ChunkGenerator* generator, sf::TcpSocket* socket)
: mType(Map::Type::Server)
, mGenerator(generator)
, mSocket(socket)
{
    if (mGenerator == nullptr)
    {
        mGenerator = new ChunkGenerator();
    }
    mGenerator->mMap = this;
    if (mSocket == nullptr)
    {
        mSocket = new sf::TcpSocket();
    }
    mAnimations = TilesetManager::getAnimations();
}

Map::~Map()
{
    if (mType == Map::Type::ClientSolo || mType == Map::Type::Server)
    {
        mGenerator->mMap = nullptr;
        mGenerator = nullptr;
        delete mGenerator;
    }
    if (mType == Map::Type::ClientOnline || mType == Map::Type::Server)
    {
        mSocket = nullptr;
        delete mSocket;
    }
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
    if (mType == Map::Type::ClientOnline || mType == Map::Type::ClientSolo)
    {
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
    }

    if (mType == Map::Type::ClientSolo || mType == Map::Type::ClientOnline)
    {
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
                    if (mType == Map::Type::ClientSolo)
                    {
                        mChunks.emplace_back(coords, *mGenerator);
                    }
                    if (mType == Map::Type::ClientOnline)
                    {
                        sendChunkQuery(coords);
                    }
                }
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

void Map::receiveChunk(sf::Packet& packet)
{
    sf::Vector2i coords;
    packet >> coords.x >> coords.y;
    mChunks.emplace_back(coords);
    getChunk(coords).loadFromPacket(packet);
}

void Map::receiveTileModification(sf::Packet& packet)
{
    sf::Vector2i coords;
    int layer, id;
    packet >> coords.x >> coords.y >> layer >> id;
    setTileId(coords,layer,id);
}

void Map::setDefaultPacketChunk(sf::Packet packet)
{
    mDefaultPacketChunk = packet;
}

void Map::setDefaultPacketModification(sf::Packet packet)
{
    mDefaultPacketModification = packet;
}

void Map::sendChunkQuery(sf::Vector2i const& coords)
{
    sf::Packet packet = mDefaultPacketChunk;
    packet << coords.x << coords.y;
    if (mSocket != nullptr)
    {
        mSocket->send(packet);
    }
}

void Map::sendTileModificationQuery(sf::Vector2i const& coords, int z, int id)
{
    sf::Packet packet = mDefaultPacketModification;
    packet /*>> ..*/<< coords.x << coords.y << z << id;
    if (mSocket != nullptr)
    {
        mSocket->send(packet);
    }
}

void Map::receiveChunkQuery(sf::Packet& incoming, sf::Packet& out)
{
    sf::Vector2i coords;
    incoming >> coords.x >> coords.y;
    if (!isChunkLoaded(coords))
    {
        mChunks.emplace_back(coords,*mGenerator);
    }
    getChunk(coords).writeToPacket(out);
}

void Map::receiveModificationQuery(sf::Packet& incoming, sf::Packet& out, bool& queryApplied)
{
    sf::Vector2i coords;
    int layer, id;
    incoming >> coords.x >> coords.y >> layer >> id;
    int oldId = getTileId(coords,layer);
    setTileId(coords,layer,id);
    queryApplied = (oldId == getTileId(coords,layer));
    out << coords.x << coords.y << layer << id;
}


} // namespace map
