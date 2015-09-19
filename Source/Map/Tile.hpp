#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "TilesetManager.hpp"
#include "Properties.hpp"

namespace map
{

class Tile : public sf::Drawable
{
    public:
        Tile(sf::Vector2i const& coords);

        sf::Vector2i getCoords() const;

        void setId(int id);
        int getId() const;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool isCollide() const;
        const sf::ConvexShape& getShape() const;

    private:
        sf::Vector2i mCoords;
        int mId;
        sf::ConvexShape mShape;
};

} // namespace map

#endif // MAP_TILE_HPP
