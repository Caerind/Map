#include "Layer.hpp"

namespace map
{

Layer::Layer(sf::Vector2i const& chunkCoords)
: mChunkCoords(chunkCoords)
{
    sf::Vector2i coords, size = Properties::getChunkSize();
    mTiles.resize(size.x);
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            mTiles[coords.x].push_back(Tile(coords));
        }
    }
}

void Layer::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform.translate(getPosition());

    sf::Vector2i coords, size = Properties::getChunkSize();
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            target.draw(mTiles[coords.x][coords.y],states);
        }
    }
}

sf::Vector2f Layer::getPosition() const
{
    return Properties::chunkToWorld(mChunkCoords);
}

sf::FloatRect Layer::getBounds() const
{
    sf::Vector2f size = Properties::getChunkSizePx();
    return sf::FloatRect(getPosition(), size);
}

void Layer::setTileId(sf::Vector2i const& coords, int id)
{
    if (coords.x >= 0 && coords.x < Properties::getChunkSize().x && coords.y >= 0 && coords.y < Properties::getChunkSize().y)
    {
        mTiles[coords.x][coords.y].setId(id);
    }
}

int Layer::getTileId(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < Properties::getChunkSize().x && coords.y >= 0 && coords.y < Properties::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].getId();
    }
    else
    {
        return 0;
    }
}

bool Layer::isCollide(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < Properties::getChunkSize().x && coords.y >= 0 && coords.y < Properties::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].isCollide();
    }
    else
    {
        return false;
    }
}

const sf::ConvexShape& Layer::getShape(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < Properties::getChunkSize().x && coords.y >= 0 && coords.y < Properties::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].getShape();
    }
    else
    {
        return sf::ConvexShape();
    }
}

} // namespace map
