#include "Isometric.hpp"

void Isometric::init()
{
    mTileSize = {64,32};

    mChunkSize = {16,64};

    mChunkSizePx = {getTileSize().x * getChunkSize().x + getTileSize().x * 0.5f, getTileSize().y * getChunkSize().y * 0.5f + getTileSize().y * 0.5f};

    mPath = "Assets/World/Isometric/";

    mShape.setPointCount(4);
    mShape.setPoint(0,sf::Vector2f(-getTileSize().x * 0.5f,0.f));
    mShape.setPoint(1,sf::Vector2f(0.f,-getTileSize().y * 0.5f));
    mShape.setPoint(2,sf::Vector2f(getTileSize().x * 0.5f,0.f));
    mShape.setPoint(3,sf::Vector2f(0.f,getTileSize().y * 0.5f));

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
        sf::Vector2i coords;
        sf::Vector2f chunkSize;
        chunkSize.x = getTileSize().x * getChunkSize().x;
        chunkSize.y = getTileSize().y * getChunkSize().y * 0.5f;
        coords.x = ((int)world.x / (int)chunkSize.x);
        if (world.x < 0 && (int)world.x % (int)chunkSize.x != 0)
        {
            coords.x--;
        }
        coords.y = ((int)world.y / (int)chunkSize.y);
        if (world.y < 0 && (int)world.x % (int)chunkSize.y != 0)
        {
            coords.y--;
        }
        return coords;
    };

    mWorldToGlobal = [&](sf::Vector2f world) -> sf::Vector2i
    {
        sf::Vector2f pos = world;
        int i = pos.x / (getTileSize().x / 2);
        int j = pos.y / (getTileSize().y / 2);
        pos.x = pos.x - i * getTileSize().x * 0.5f;
        pos.y = pos.y - j * getTileSize().y * 0.5f;
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
        ret.y = globalCoords.y * getTileSize().y * 0.5f;
        if (globalCoords.y%2 == 0)
        {
            ret.x = globalCoords.x * getTileSize().x;
        }
        else
        {
            ret.x = (globalCoords.x + 0.5) * getTileSize().x;
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
