#include "Tileset.hpp"

namespace map
{

Tileset::Tileset(std::string const& name, std::string const& filename)
{
    mName = name;
    mFilename = filename;
    if (!ah::Application::instance().isTextureLoaded(mName))
    {
        ah::Application::instance().loadTexture(mName,mFilename);
    }
    mTexture = &ah::Application::instance().getTexture(mName);
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
