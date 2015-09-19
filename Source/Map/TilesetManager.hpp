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
#include "Animation.hpp"

namespace map
{

class TilesetManager
{
    public:
        static void addTileset(std::string const& name, std::string const& filename, sf::Texture* texture = nullptr);
        static void bind(int id, std::string const& name, sf::IntRect const& texRect, bool collide);
        static void addAnimation(Animation const& animation);

        static std::pair<sf::Texture*,sf::IntRect> getPair(int id);
        static bool isCollide(int id);
        static std::vector<Animation> getAnimations();

    private:
        static std::vector<Tileset> mTilesets;
        static std::map<int,std::pair<std::string,sf::IntRect>> mPairs;
        static std::map<int,bool> mCollisions;
        static std::vector<Animation> mAnimations;
};

} // namespace map

#endif // MAP_TILESETMANAGER_HPP
