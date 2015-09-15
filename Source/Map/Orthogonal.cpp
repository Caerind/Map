#include "Orthogonal.hpp"

namespace map
{

sf::ConvexShape Orthogonal::getShape()
{
    sf::ConvexShape s(4);
    s.setPoint(0,sf::Vector2f());
    s.setPoint(1,sf::Vector2f(getTextureSize().x,0.f));
    s.setPoint(2,sf::Vector2f(getTextureSize().x,getTextureSize().y));
    s.setPoint(3,sf::Vector2f(0.f,getTextureSize().y));
    return s;
}

sf::Vector2i Orthogonal::getTextureSize()
{
    return sf::Vector2i(32,32);
}

std::vector<sf::Vector2i> Orthogonal::getNeighboor(sf::Vector2i coords)
{
    std::vector<sf::Vector2i> n;
    n.push_back(sf::Vector2i(coords.x,coords.y--));
    n.push_back(sf::Vector2i(coords.x,coords.y++));
    n.push_back(sf::Vector2i(coords.x--,coords.y));
    n.push_back(sf::Vector2i(coords.x++,coords.y));
    return n;
}

sf::Vector2i Orthogonal::getChunkSize()
{
    return sf::Vector2i(32,32);
}

sf::Vector2f Orthogonal::getChunkSizePx()
{
    return sf::Vector2f(1024.f,1024.f);
}

float Orthogonal::getLayerHeight()
{
    return 32.f;
}

float Orthogonal::getLayerShift()
{
    return 0.f;
}

std::string Orthogonal::getPath()
{
    return "Assets/World/";
}

sf::Vector2i Orthogonal::worldToChunk(sf::Vector2f const& world)
{
    sf::Vector2i coords;
    coords.x = ((int)world.x / (int)getChunkSizePx().x);
    if (world.x < 0 && (int)world.x % (int)getChunkSizePx().x != 0)
    {
        coords.x--;
    }
    coords.y = ((int)world.y / (int)getChunkSizePx().y);
    if (world.y < 0 && (int)world.x % (int)getChunkSizePx().y != 0)
    {
        coords.y--;
    }
    return coords;
}

sf::Vector2i Orthogonal::worldToGlobalCoords(sf::Vector2f const& world)
{
    sf::Vector2i coords;
    coords.x = ((int)world.x / getTextureSize().x);
    coords.y = ((int)world.y / getTextureSize().y);
    if (world.x < 0 && (int)world.x % getTextureSize().x != 0)
    {
        coords.x--;
    }
    if (world.y < 0 && (int)world.y % getTextureSize().y != 0)
    {
        coords.y--;
    }
    return coords;
}

sf::Vector2i Orthogonal::worldToLocalCoords(sf::Vector2f const& world)
{
    sf::Vector2i coords = worldToGlobalCoords(world);
    coords.x %= getChunkSize().x;
    if (coords.x < 0)
    {
        coords.x += getChunkSize().x;
    }
    coords.y %= getChunkSize().y;
    if (coords.y < 0)
    {
        coords.y += getChunkSize().y;
    }
    return coords;
}

sf::Vector2f Orthogonal::chunkToWorld(sf::Vector2i const& chunk)
{
    sf::Vector2f world;
    world.x = chunk.x * getChunkSizePx().x;
    world.y = chunk.y * getChunkSizePx().y;
    return world;
}

sf::Vector2f Orthogonal::globalCoordsToWorld(sf::Vector2i const& globalCoords)
{
    sf::Vector2f world;
    world.x = globalCoords.x * getTextureSize().x;
    world.y = globalCoords.y * getTextureSize().y;
    return world;
}

sf::Vector2f Orthogonal::localCoordsToWorld(sf::Vector2i const& localCoords)
{
    return globalCoordsToWorld(localCoords); // TODO : Useless ?
}

} // namespace map
