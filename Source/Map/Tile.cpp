#include "Tile.hpp"

namespace map
{

Tile::Tile(sf::Vector2i const& coords)
: mCoords(coords), mId(0)
{
    mShape = Properties::getShape();
    mShape.setPosition(Properties::localCoordsToWorld(coords));
}

sf::Vector2i Tile::getCoords() const
{
    return mCoords;
}

void Tile::setId(int id)
{
    mId = id;
    auto pair = TilesetManager::getPair(id);
    if (pair.first != nullptr)
    {
        mShape.setTexture(pair.first);
        mShape.setTextureRect(pair.second);
    }
}

int Tile::getId() const
{
    return mId;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape,states);
}

bool Tile::isCollide() const
{
    return TilesetManager::isCollide(mId);
}

const sf::ConvexShape& Tile::getShape() const
{
    return mShape;
}

} // namespace map
