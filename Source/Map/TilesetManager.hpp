#ifndef MAP_TILESETMANAGER_HPP
#define MAP_TILESETMANAGER_HPP

#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "Tileset.hpp"

namespace map
{

class TilesetManager
{
    public:
        static void load(std::string const& path);
        static void save(std::string const& path);

        static void addTileset(std::string const& name, std::string const& filename);
        static void bind(int id, std::string const& name, sf::IntRect const& texRect, bool collide);
        static std::pair<sf::Texture*,sf::IntRect> getPair(int id);
        static bool isCollide(int id);

        static std::map<int,std::pair<std::string,sf::IntRect>>& getMap();

    private:
        TilesetManager();
        static TilesetManager mInstance;

        std::vector<Tileset> mTilesets;
        std::map<int,std::pair<std::string,sf::IntRect>> mPairs;
        std::map<int,bool> mCollisions;
        bool mSaveNeeded;
};

} // namespace map

#endif // MAP_TILESETMANAGER_HPP
