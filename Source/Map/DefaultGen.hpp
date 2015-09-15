#ifndef MAP_DEFAULTGEN_HPP
#define MAP_DEFAULTGEN_HPP

#include "Map.hpp"
#include "Chunk.hpp"
#include "TilesetManager.hpp"
#include "../../Aharos/Helper/Math.hpp"

namespace map
{

template <typename T>
class DefaultGen
{
    public:
        static void generate(Chunk<T,DefaultGen<T>>& chunk);

        static void setMap(Map<T,DefaultGen<T>>* map);

    private:
        static Map<T,DefaultGen<T>>* mMap;
};

template<typename T>
Map<T,DefaultGen<T>>* DefaultGen<T>::mMap = nullptr;

template <typename T>
void DefaultGen<T>::generate(Chunk<T,DefaultGen<T>>& chunk)
{
    chunk.addLayer();
    sf::Vector2i coords, size = T::getChunkSize();
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            chunk.setTileId(coords,0,1);
        }
    }
    int idDeco = lp::random(2,4);
    if (idDeco == 4) idDeco = 6;
    sf::Vector2i pos = sf::Vector2i(lp::random(8,24),lp::random(8,24));
    std::vector<sf::Vector2i> colored;
    colored.push_back(pos);
    std::size_t sized = lp::random(50,100);
    while (colored.size() < sized)
    {
        for (std::size_t i = 0; i < colored.size(); i++)
        {
            std::vector<sf::Vector2i> n = T::getNeighboor(colored[i]);
            for (std::size_t j = 0; j < n.size(); j++)
            {
                bool find = false;
                for (std::size_t k = 0; k < colored.size(); k++)
                {
                    if (n[j] == colored[k])
                    {
                        find = true;
                    }
                }
                if (!find && lp::random(0,100) > 75)
                {
                    colored.push_back(n[j]);
                }
                if (colored.size() > sized)
                {
                    break;
                }
            }
            if (colored.size() > sized)
            {
                break;
            }
        }
    }
    for (std::size_t i = 0; i < colored.size(); i++)
    {
        chunk.setTileId(colored[i],0,idDeco);
    }
}

template <typename T>
void DefaultGen<T>::setMap(Map<T,DefaultGen<T>>* map)
{
    mMap = map;
}

} // namespace map

#endif // MAP_DEFAULTGEN_HPP
