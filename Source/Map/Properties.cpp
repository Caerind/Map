#include "Properties.hpp"

namespace map
{

sf::ConvexShape Properties::mShape = sf::ConvexShape();
sf::Vector2i Properties::mTileSize = {1,1};
Properties::NeighboorFunction Properties::mNeighboor = [&](sf::Vector2i) -> std::vector<sf::Vector2i> { return {}; };
sf::Vector2i Properties::mChunkSize = {1,1};
sf::Vector2f Properties::mChunkSizePx = {1.f,1.f};
std::string Properties::mPath = "";
Properties::FunctionF2I Properties::mWorldToChunk = [&](sf::Vector2f) -> sf::Vector2i { return {0,0}; };
Properties::FunctionF2I Properties::mWorldToGlobal = [&](sf::Vector2f) -> sf::Vector2i { return {0,0}; };
Properties::FunctionF2I Properties::mWorldToLocal = [&](sf::Vector2f) -> sf::Vector2i { return {0,0}; };
Properties::FunctionI2F Properties::mChunkToWorld = [&](sf::Vector2i) -> sf::Vector2f { return {0,0}; };
Properties::FunctionI2F Properties::mGlobalToWorld = [&](sf::Vector2i) -> sf::Vector2f { return {0,0}; };
Properties::FunctionI2F Properties::mLocalToWorld = [&](sf::Vector2i) -> sf::Vector2f { return {0,0}; };

sf::ConvexShape Properties::getShape()
{
    return mShape;
}

sf::Vector2i Properties::getTileSize()
{
    return mTileSize;
}

std::vector<sf::Vector2i> Properties::getNeighboor(sf::Vector2i coords)
{
    return mNeighboor(coords);
}

sf::Vector2i Properties::getChunkSize()
{
    return mChunkSize;
}

sf::Vector2f Properties::getChunkSizePx()
{
    return mChunkSizePx;
}

std::string Properties::getPath()
{
    return mPath;
}

sf::Vector2i Properties::worldToChunk(sf::Vector2f const& world)
{
    return mWorldToChunk(world);
}

sf::Vector2i Properties::worldToGlobalCoords(sf::Vector2f const& world)
{
    return mWorldToGlobal(world);
}

sf::Vector2i Properties::worldToLocalCoords(sf::Vector2f const& world)
{
    return mWorldToLocal(world);
}

sf::Vector2f Properties::chunkToWorld(sf::Vector2i const& chunk)
{
    return mChunkToWorld(chunk);
}

sf::Vector2f Properties::globalCoordsToWorld(sf::Vector2i const& globalCoords)
{
    return mGlobalToWorld(globalCoords);
}

sf::Vector2f Properties::localCoordsToWorld(sf::Vector2i const& localCoords)
{
    return mLocalToWorld(localCoords);
}

} // namespace map
