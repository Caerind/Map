#include "Isometric.hpp"

void Isometric::init()
{
    mTileSize = {64,32};

    mChunkSize = {16,64};

    mChunkSizePx = {getTileSize().x * getChunkSize().x + getTileSize().x * 0.5f, getTileSize().y * getChunkSize().y * 0.5f + getTileSize().y * 0.5f};

    mPath = "Assets/World/Isometric/";

    mShape.setPointCount(4);
    mShape.setPoint(0,sf::Vector2f(0.f, -getTileSize().y * 0.5f));
    mShape.setPoint(1,sf::Vector2f(getTileSize().x * 0.5f, 0.f));
    mShape.setPoint(2,sf::Vector2f(0.f,getTileSize().y * 0.5f));
    mShape.setPoint(3,sf::Vector2f(-getTileSize().x * 0.5f,0.f));

    mNeighboor = [&](sf::Vector2i coords) -> std::vector<sf::Vector2i>
    {
        std::vector<sf::Vector2i> n;
        if (coords.y %2 == 0)
        {
            n.push_back(sf::Vector2i(coords.x-1,coords.y-1));
            n.push_back(sf::Vector2i(coords.x,coords.y-1));
            n.push_back(sf::Vector2i(coords.x-1,coords.y+1));
            n.push_back(sf::Vector2i(coords.x,coords.y+1));
        }
        else
        {
            n.push_back(sf::Vector2i(coords.x,coords.y-1));
            n.push_back(sf::Vector2i(coords.x+1,coords.y-1));
            n.push_back(sf::Vector2i(coords.x,coords.y+1));
            n.push_back(sf::Vector2i(coords.x+1,coords.y+1));
        }
        return n;
    };

    mWorldToChunk = [&](sf::Vector2f world) -> sf::Vector2i
    {
        sf::Vector2i coords = worldToGlobalCoords(world);
        sf::Vector2i c = coords;
        coords.x /= getChunkSize().x;
        coords.y /= getChunkSize().y;
        if (c.x < 0)
        {
            coords.x--;
        }
        if (c.y < 0)
        {
            coords.y--;
        }
        return coords;
    };

    // TODO : Iso : Handle negative value
    mWorldToGlobal = [&](sf::Vector2f world) -> sf::Vector2i
    {
        sf::Vector2f ts = sf::Vector2f(getTileSize()) * 0.5f;
        sf::Vector2f coords = sf::Vector2f(floor(world.x / ts.x), floor(world.y / ts.y));
        sf::Vector2f pos = world - sf::Vector2f(coords.x * ts.x, coords.y * ts.y);
        if ((int)coords.x%2 == (int)coords.y%2)
        {
            // THERE IS A PROBLEM HERE
            if (coords.x < 0 || coords.y < 0)
            {
                if (std::atan2(ts.y - pos.y, pos.x) > 3.14152f/6.f)
                {
                    coords.x--;
                    coords.y--;
                }
            }
            else if (std::atan2(ts.y - pos.y, pos.x) > 3.14152f/6.f)
            {
                coords.x--;
                coords.y--;
            }
        }
        else
        {
            if (std::atan2(-pos.y, pos.x) > -3.14152f/6.f)
            {
                coords.y--;
            }
            else
            {
                coords.x--;
            }
        }
        return sf::Vector2i(floor((float)coords.x * 0.5f),coords.y);
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
        world.x = chunk.x * getChunkSize().x * getTileSize().x;
        world.y = chunk.y * getChunkSize().y * getTileSize().y * 0.5f;
        return world;
    };

    mGlobalToWorld = [&](sf::Vector2i globalCoords) -> sf::Vector2f
    {
        sf::Vector2f ret;
        ret.y = globalCoords.y * getTileSize().y * 0.5f + getTileSize().y * 0.5f;
        if (globalCoords.y%2 == 0)
        {
            ret.x = globalCoords.x * getTileSize().x + getTileSize().x * 0.5f;
        }
        else
        {
            ret.x = globalCoords.x * getTileSize().x + getTileSize().x;
        }
        return ret;
    };

    mLocalToWorld = [&](sf::Vector2i localCoords) -> sf::Vector2f
    {
        return globalCoordsToWorld(localCoords);
    };

    map::TilesetManager::addTileset("tileset","Assets/Textures/tilesetIso.png");

    map::TilesetManager::bind(1,"tileset",sf::IntRect(0,0,64,32),0); // yellow
    map::TilesetManager::bind(2,"tileset",sf::IntRect(64,0,64,32),0); // red
}
