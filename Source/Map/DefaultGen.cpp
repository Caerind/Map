#include "DefaultGen.hpp"
#include "Chunk.hpp"
#include "Map.hpp"

void DefaultGen::generate(map::Chunk& chunk)
{
    chunk.addLayer();
    sf::Vector2i coords, size = map::Properties::getChunkSize();
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            chunk.setTileId(coords,0,1);
        }
    }

    /*
    int idDeco = randomInt(2,3);
    sf::Vector2i pos = sf::Vector2i(randomInt(8,24),randomInt(8,24));
    std::vector<sf::Vector2i> colored;
    colored.push_back(pos);
    std::size_t sized = randomInt(50,100);
    while (colored.size() < sized)
    {
        for (std::size_t i = 0; i < colored.size(); i++)
        {
            std::vector<sf::Vector2i> n = map::Properties::getNeighboor(colored[i]);
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
                if (!find && randomInt(0,100) > 75)
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
    */
}
