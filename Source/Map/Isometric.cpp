#include "Isometric.hpp"

namespace map
{

void Isometric::init()
{
    // TODO : Do Isometric Init
    mTileSize = {64,32};

    mChunkSize = {16,64};

    mChunkSizePx = {1024.f,1024.f};

    mPath = "Assets/World/Isometric/";

    mShape.setPointCount(4);
    mShape.setPoint(0,sf::Vector2f(-getTileSize().x * 0.5f,0.f));
    mShape.setPoint(1,sf::Vector2f(0.f,-getTileSize().y * 0.5f));
    mShape.setPoint(2,sf::Vector2f(getTileSize().x * 0.5f,0.f));
    mShape.setPoint(3,sf::Vector2f(0.f,getTileSize().y * 0.5f));

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

/*

sf::Vector2i Isometric::getChunkSize()
{
    return sf::Vector2i(32,64);
}

sf::Vector2f Isometric::getChunkSizePx()
{
    return sf::Vector2f(2048.f,1024.f);
}

float Isometric::getLayerHeight()
{
    return 32.f;
}

float Isometric::getLayerShift()
{
    return 0.f;
}

std::string Isometric::getPath()
{
    return "Assets/World/";
}

sf::Vector2i Isometric::worldToChunk(sf::Vector2f const& world)
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

sf::Vector2i Isometric::worldToGlobalCoords(sf::Vector2f const& world)
{
    sf::Vector2f pos = world;
    int i = pos.x / (getTextureSize().x/2);
    int j = pos.y / (getTextureSize().y/2);
    pos.x = pos.x - i * (getTextureSize().x/2);
    pos.y = pos.y - j * (getTextureSize().y/2);
    if (i%2 == j%2)
    {
        if (std::atan(pos.y/pos.x) > 3.14152/6)
        {
            i--;
            j--;
        }
    }
    else
    {
        if (-std::atan(pos.y/pos.x) > -3.14152/6)
        {
            j--;
        }
        else
        {
            i--;
        }
    }
    return sf::Vector2i(i/2,j);
}

sf::Vector2i Isometric::worldToLocalCoords(sf::Vector2f const& world)
{

}

sf::Vector2f Isometric::chunkToWorld(sf::Vector2i const& chunk)
{
    sf::Vector2f world;
    world.x = chunk.x * getChunkSizePx().x;
    world.y = chunk.y * getChunkSizePx().y;
    return world;
}

sf::Vector2f Isometric::globalCoordsToWorld(sf::Vector2i const& globalCoords)
{

}

sf::Vector2f Isometric::localCoordsToWorld(sf::Vector2i const& localCoords)
{
    return globalCoordsToWorld(localCoords);
}
*/
