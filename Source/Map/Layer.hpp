#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../../Aharos/Renderer/Renderable.hpp"
#include "../../Aharos/Renderer/Renderer.hpp"

#include "Tile.hpp"

namespace map
{

template <typename T>
class Layer : public rd::Renderable
{
    public:
        Layer(sf::Vector2i const& chunkCoords, int zPosition);
        virtual ~Layer();

        sf::FloatRect getBounds();
        void render(sf::RenderTarget& target);
        void setPosition(sf::Vector2f const& position);
        sf::Vector2f getPosition() const;

        void setTileId(sf::Vector2i const& coords, int id);
        int getTileId(sf::Vector2i const& coords) const;
        bool isCollide(sf::Vector2i const& coords) const;
        const sf::ConvexShape& getShape(sf::Vector2i const& coords) const;

    private:
        sf::Vector2i mChunkCoords;
        int mPositionZ; // Position in the chunk array of layer -> z-axis & decalage
        std::vector<std::vector<Tile<T>>> mTiles;
};


template <typename T>
Layer<T>::Layer(sf::Vector2i const& chunkCoords, int zPosition)
: mChunkCoords(chunkCoords), mPositionZ(zPosition)
{
    setZ(mPositionZ * T::getLayerHeight());

    sf::Vector2i coords, size = T::getChunkSize();
    mTiles.resize(size.x);
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            mTiles[coords.x].push_back(Tile<T>(coords));
        }
    }
}

template <typename T>
Layer<T>::~Layer()
{
}

template <typename T>
sf::FloatRect Layer<T>::getBounds()
{
    sf::Vector2f size = T::getChunkSizePx();
    return sf::FloatRect(getPosition(), size);
}

template <typename T>
void Layer<T>::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform.translate(getPosition());
    states.transform.translate(sf::Vector2f(0.f, mPositionZ * T::getLayerShift()));

    sf::Vector2i coords, size = T::getChunkSize();
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            target.draw(mTiles[coords.x][coords.y],states);
        }
    }
}

template <typename T>
void Layer<T>::setPosition(sf::Vector2f const& position)
{
}

template <typename T>
sf::Vector2f Layer<T>::getPosition() const
{
    return T::chunkToWorld(mChunkCoords);
}

template <typename T>
void Layer<T>::setTileId(sf::Vector2i const& coords, int id)
{
    if (coords.x >= 0 && coords.x < T::getChunkSize().x && coords.y >= 0 && coords.y < T::getChunkSize().y)
    {
        mTiles[coords.x][coords.y].setId(id);
    }
}

template <typename T>
int Layer<T>::getTileId(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < T::getChunkSize().x && coords.y >= 0 && coords.y < T::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].getId();
    }
    else
    {
        return 0;
    }
}

template <typename T>
bool Layer<T>::isCollide(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < T::getChunkSize().x && coords.y >= 0 && coords.y < T::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].isCollide();
    }
    else
    {
        return false;
    }
}

template <typename T>
const sf::ConvexShape& Layer<T>::getShape(sf::Vector2i const& coords) const
{
    if (coords.x >= 0 && coords.x < T::getChunkSize().x && coords.y >= 0 && coords.y < T::getChunkSize().y)
    {
        return mTiles[coords.x][coords.y].getShape();
    }
    else
    {
        return sf::ConvexShape();
    }
}

} // namespace map

#endif // MAP_LAYER_HPP
