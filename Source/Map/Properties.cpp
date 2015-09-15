#include "Properties.hpp"

namespace map
{

sf::ConvexShape Properties::getShape()
{
    return sf::ConvexShape();
}

sf::Vector2i Properties::getTextureSize()
{
    return sf::Vector2i();
}

std::vector<sf::Vector2i> Properties::getNeighboor(sf::Vector2i coords)
{
    return std::vector<sf::Vector2i>();
}

sf::Vector2i Properties::getChunkSize()
{
    return sf::Vector2i();
}

sf::Vector2f Properties::getChunkSizePx()
{
    return sf::Vector2f();
}

float Properties::getLayerHeight()
{
    return 0.f;
}

float Properties::getLayerShift()
{
    return 0.f;
}

std::string Properties::getPath()
{
    return std::string();
}

sf::Vector2i Properties::worldToChunk(sf::Vector2f const& world)
{
    return sf::Vector2i();
}

sf::Vector2i Properties::worldToGlobalCoords(sf::Vector2f const& world)
{
    return sf::Vector2i();
}

sf::Vector2i Properties::worldToLocalCoords(sf::Vector2f const& world)
{
    return sf::Vector2i();
}

sf::Vector2f Properties::chunkToWorld(sf::Vector2i const& chunk)
{
    return sf::Vector2f();
}

sf::Vector2f Properties::globalCoordsToWorld(sf::Vector2i const& globalCoords)
{
    return sf::Vector2f();
}

sf::Vector2f Properties::localCoordsToWorld(sf::Vector2i const& localCoords)
{
    return sf::Vector2f();
}

} // namespace map
