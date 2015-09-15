#ifndef MAP_PROPERTIES_HPP
#define MAP_PROPERTIES_HPP

#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

namespace map
{

class Properties
{
    public:
        // Tile Properties
        static sf::ConvexShape getShape();
        static sf::Vector2i getTextureSize();
        static std::vector<sf::Vector2i> getNeighboor(sf::Vector2i coords);

        // Chunk Properties
        static sf::Vector2i getChunkSize();
        static sf::Vector2f getChunkSizePx();
        static float getLayerHeight();
        static float getLayerShift();

        // Global Properties
        static std::string getPath();

        // Conversion
        static sf::Vector2i worldToChunk(sf::Vector2f const& world);
        static sf::Vector2i worldToGlobalCoords(sf::Vector2f const& world);
        static sf::Vector2i worldToLocalCoords(sf::Vector2f const& world);

        static sf::Vector2f chunkToWorld(sf::Vector2i const& chunk);
        static sf::Vector2f globalCoordsToWorld(sf::Vector2i const& globalCoords);
        static sf::Vector2f localCoordsToWorld(sf::Vector2i const& localCoords);
};

} // namespace map

#endif // MAP_PROPERTIES_HPP
