#include "Tileset.hpp"

namespace map
{

Tileset::Tileset(std::string const& name, std::string const& filename)
{
    mName = name;
    mFilename = filename;
    mTexture = new sf::Texture();
    assert(mTexture->loadFromFile(mFilename));
}

Tileset::~Tileset()
{
    delete mTexture;
}

std::string Tileset::getName() const
{
    return mName;
}

std::string Tileset::getFilename() const
{
    return mFilename;
}

sf::Texture* Tileset::getTexture() const
{
    return mTexture;
}

} // namespace map
