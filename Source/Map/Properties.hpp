#ifndef MAP_PROPERTIES_HPP
#define MAP_PROPERTIES_HPP

#include <functional>
#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "TilesetManager.hpp"

namespace map
{

class Properties
{
    public:
        typedef std::function<std::vector<sf::Vector2i>(sf::Vector2i)> NeighboorFunction;
        typedef std::function<sf::Vector2i(const sf::Vector2f&)> FunctionF2I;
        typedef std::function<sf::Vector2f(const sf::Vector2i&)> FunctionI2F;

        // Tile Properties
        static sf::ConvexShape getShape();
        static sf::Vector2i getTileSize();
        static std::vector<sf::Vector2i> getNeighboor(sf::Vector2i coords);

        // Chunk Properties
        static sf::Vector2i getChunkSize();
        static sf::Vector2f getChunkSizePx();

        // Global Properties
        static std::string getPath();

        // Conversion
        static sf::Vector2i worldToChunk(sf::Vector2f const& world);
        static sf::Vector2i worldToGlobalCoords(sf::Vector2f const& world);
        static sf::Vector2i worldToLocalCoords(sf::Vector2f const& world);

        static sf::Vector2f chunkToWorld(sf::Vector2i const& chunk);
        static sf::Vector2f globalCoordsToWorld(sf::Vector2i const& globalCoords);
        static sf::Vector2f localCoordsToWorld(sf::Vector2i const& localCoords);

    protected:
        static sf::ConvexShape mShape;
        static sf::Vector2i mTileSize;
        static NeighboorFunction mNeighboor;

        static sf::Vector2i mChunkSize;
        static sf::Vector2f mChunkSizePx;

        static std::string mPath;

        static FunctionF2I mWorldToChunk;
        static FunctionF2I mWorldToGlobal;
        static FunctionF2I mWorldToLocal;

        static FunctionI2F mChunkToWorld;
        static FunctionI2F mGlobalToWorld;
        static FunctionI2F mLocalToWorld;
};

} // namespace map

#endif // MAP_PROPERTIES_HPP
