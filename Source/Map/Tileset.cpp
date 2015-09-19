#include "Tileset.hpp"

namespace map
{

Tileset::Tileset(std::string const& name, std::string const& filename, sf::Texture* texture)
{
    mName = name;
    mFilename = filename;
    if (texture != nullptr)
    {
        mTexture = texture;
    }
    else
    {
        mTexture = new sf::Texture();
        assert(mTexture->loadFromFile(mFilename));
    }
}

Tileset::~Tileset()
{
    mTexture = nullptr;
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
