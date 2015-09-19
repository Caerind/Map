#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Tile.hpp"
#include "Properties.hpp"

namespace map
{

class Layer
{
    public:
        Layer(sf::Vector2i const& chunkCoords);

        void render(sf::RenderTarget& target);

        sf::Vector2f getPosition() const;
        sf::FloatRect getBounds() const;

        void setTileId(sf::Vector2i const& coords, int id);
        int getTileId(sf::Vector2i const& coords) const;
        bool isCollide(sf::Vector2i const& coords) const;
        const sf::ConvexShape& getShape(sf::Vector2i const& coords) const;

    private:
        sf::Vector2i mChunkCoords;
        std::vector<std::vector<Tile>> mTiles;
};

} // namespace map

#endif // MAP_LAYER_HPP
