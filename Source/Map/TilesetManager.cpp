#include "TilesetManager.hpp"

namespace map
{

std::vector<Tileset> TilesetManager::mTilesets;
std::map<int,std::pair<std::string,sf::IntRect>> TilesetManager::mPairs;
std::map<int,bool> TilesetManager::mCollisions;
std::vector<Animation> TilesetManager::mAnimations;

void TilesetManager::addTileset(std::string const& name, std::string const& filename, sf::Texture* texture)
{
    mTilesets.push_back(Tileset(name,filename,texture));
}

void TilesetManager::bind(int id, std::string const& name, sf::IntRect const& texRect, bool collide)
{
    for (std::size_t i = 0; i < mTilesets.size(); i++)
    {
        if (mTilesets[i].getName() == name)
        {
            std::pair<std::string,sf::IntRect> pair;
            pair.first = name;
            pair.second = texRect;
            mPairs[id] = pair;
            mCollisions[id] = collide;
            return;
        }
    }
}

void TilesetManager::addAnimation(Animation const& animation)
{
    mAnimations.push_back(animation);
}

std::pair<sf::Texture*,sf::IntRect> TilesetManager::getPair(int id)
{
    std::pair<sf::Texture*,sf::IntRect> pair;
    if (mPairs.find(id) != mPairs.end())
    {
        std::pair<std::string,sf::IntRect> spair = mPairs[id];
        for (std::size_t i = 0; i < mTilesets.size(); i++)
        {
            if (mTilesets[i].getName() == spair.first)
            {
                pair.first = mTilesets[i].getTexture();
                pair.second = spair.second;
                return pair;
            }
        }
    }
    pair.first = nullptr;
    pair.second = sf::IntRect();
    return pair;
}

bool TilesetManager::isCollide(int id)
{
    if (mCollisions.find(id) != mCollisions.end())
    {
        return mCollisions[id];
    }
    else
    {
        return false;
    }
}

std::vector<Animation> TilesetManager::getAnimations()
{
    return mAnimations;
}

} // namespace map
