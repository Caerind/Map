#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "TilesetManager.hpp"

namespace map
{

template <typename T>
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

template <typename T>
Tile<T>::Tile(sf::Vector2i const& coords)
: mCoords(coords), mId(0)
{
    mShape = T::getShape();
    mShape.setPosition(T::localCoordsToWorld(coords));
}

template <typename T>
sf::Vector2i Tile<T>::getCoords() const
{
    return mCoords;
}

template <typename T>
void Tile<T>::setId(int id)
{
    mId = id;
    auto pair = TilesetManager::getPair(id);
    if (pair.first != nullptr)
    {
        mShape.setTexture(pair.first);
        mShape.setTextureRect(pair.second);
    }
}

template <typename T>
int Tile<T>::getId() const
{
    return mId;
}

template <typename T>
void Tile<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape,states);
}

template <typename T>
bool Tile<T>::isCollide() const
{
    return TilesetManager::isCollide(mId);
}

template <typename T>
const sf::ConvexShape& Tile<T>::getShape() const
{
    return mShape;
}

} // namespace map

#endif // MAP_TILE_HPP
