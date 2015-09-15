#ifndef MAP_TILESET_HPP
#define MAP_TILESET_HPP

#include <string>

#include <SFML/Graphics/Texture.hpp>

#include "../../Aharos/Application/Application.hpp"

namespace map
{

class Tileset
{
    public:
        Tileset(std::string const& name, std::string const& filename);

        std::string getName() const;
        std::string getFilename() const;
        sf::Texture* getTexture() const;

    private:
        std::string mName;
        std::string mFilename;
        sf::Texture* mTexture;
};

} // namespace map

#endif // MAP_TILESET_HPP
