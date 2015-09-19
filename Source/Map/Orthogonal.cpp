#include "Orthogonal.hpp"

namespace map
{

void Orthogonal::init()
{
    mTileSize = {32,32};

    mChunkSize = {32,32};

    mChunkSizePx = {1024.f,1024.f};

    mPath = "Assets/World/Orthogonal/";

    mShape.setPointCount(4);
    mShape.setPoint(0,sf::Vector2f());
    mShape.setPoint(1,sf::Vector2f(getTileSize().x,0.f));
    mShape.setPoint(2,sf::Vector2f(getTileSize().x,getTileSize().y));
    mShape.setPoint(3,sf::Vector2f(0.f,getTileSize().y));

    mNeighboor = [&](sf::Vector2i coords) -> std::vector<sf::Vector2i>
    {
        std::vector<sf::Vector2i> n;
        n.push_back(sf::Vector2i(coords.x,coords.y--));
        n.push_back(sf::Vector2i(coords.x,coords.y++));
        n.push_back(sf::Vector2i(coords.x--,coords.y));
        n.push_back(sf::Vector2i(coords.x++,coords.y));
        return n;
    };

    mWorldToChunk = [&](sf::Vector2f world) -> sf::Vector2i
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
    };

    mWorldToGlobal = [&](sf::Vector2f world) -> sf::Vector2i
    {
        sf::Vector2i coords;
        coords.x = ((int)world.x / getTileSize().x);
        coords.y = ((int)world.y / getTileSize().y);
        if (world.x < 0 && (int)world.x % getTileSize().x != 0)
        {
            coords.x--;
        }
        if (world.y < 0 && (int)world.y % getTileSize().y != 0)
        {
            coords.y--;
        }
        return coords;
    };

    mWorldToLocal = [&](sf::Vector2f world) -> sf::Vector2i
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
    };

    mChunkToWorld = [&](sf::Vector2i chunk) -> sf::Vector2f
    {
        sf::Vector2f world;
        world.x = chunk.x * getChunkSizePx().x;
        world.y = chunk.y * getChunkSizePx().y;
        return world;
    };

    mGlobalToWorld = [&](sf::Vector2i globalCoords) -> sf::Vector2f
    {
        sf::Vector2f world;
        world.x = globalCoords.x * getTileSize().x;
        world.y = globalCoords.y * getTileSize().y;
        return world;
    };

    mLocalToWorld = [&](sf::Vector2i localCoords) -> sf::Vector2f
    {
        return globalCoordsToWorld(localCoords);
    };
}

} // namespace map
