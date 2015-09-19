#include "TilesetManager.hpp"

namespace map
{

TilesetManager TilesetManager::mInstance;

TilesetManager::TilesetManager()
{
    mSaveNeeded = false;
}

void TilesetManager::load(std::string const& path)
{
    // TODO : This need to be FIXED

    std::ifstream file(path + "tilesets.dat");
    if (file)
    {
        std::string line;
        while (std::getline(file,line))
        {
            if (line.size() > 0)
            {
                if (line[0] == '-')
                {
                    line.erase(0,2);
                    std::size_t find = line.find_first_of(' ');
                    if (find != std::string::npos)
                    {
                        std::string name = line.substr(0, find);
                        std::string filename = line.substr(find+1,line.size()-find-1);
                        addTileset(name,filename);
                    }
                }
                else if (line[0] == '+')
                {
                    line.erase(0,2);
                    std::size_t find = line.find_first_of(' ');
                    if (find != std::string::npos)
                    {
                        int id;
                        {
                            std::istringstream iss(line.substr(0, find));
                            iss >> id;
                        }
                        line = line.substr(find+1,line.size()-find-1);
                        find = line.find_first_of(' ');
                        if (find != std::string::npos)
                        {
                            std::string name = line.substr(0, find);
                            line = line.substr(find+1,line.size()-find-1);
                            find = line.find_first_of(' ');
                            if (find != std::string::npos)
                            {
                                sf::IntRect tRect;
                                {
                                    std::istringstream iss(line.substr(0, find));
                                    iss >> tRect.left;
                                }
                                line = line.substr(find+1,line.size()-find-1);
                                find = line.find_first_of(' ');
                                if (find != std::string::npos)
                                {
                                    {
                                        std::istringstream iss(line.substr(0, find));
                                        iss >> tRect.top;
                                    }
                                    line = line.substr(find+1,line.size()-find-1);
                                    find = line.find_first_of(' ');
                                    if (find != std::string::npos)
                                    {
                                        {
                                            std::istringstream iss(line.substr(0, find));
                                            iss >> tRect.width;
                                        }
                                        line = line.substr(find+1,line.size()-find-1);
                                        find = line.find_first_of(' ');
                                        if (find != std::string::npos)
                                        {
                                            {
                                                std::istringstream iss(line.substr(0, find));
                                                iss >> tRect.height;
                                            }
                                            bool collide;
                                            {
                                                std::istringstream iss(line.substr(find+1,line.size()-find-1));
                                                iss >> collide;
                                            }
                                            bind(id,name,tRect,collide);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        mInstance.mSaveNeeded = false;
    }
    else
    {
        mInstance.mSaveNeeded = true;
    }
    file.close();
}

void TilesetManager::save(std::string const& path)
{
    if (mInstance.mSaveNeeded)
    {
        std::ofstream file(path + "tilesets.dat");
        if (file)
        {
            for (std::size_t i = 0; i < mInstance.mTilesets.size(); i++)
            {
                file << "- " << mInstance.mTilesets[i].getName() << " " << mInstance.mTilesets[i].getFilename() << std::endl;
            }
            for (auto itr = mInstance.mPairs.begin(); itr != mInstance.mPairs.end(); itr++)
            {
                file << "+ " << itr->first << " " << itr->second.first << " ";
                file << itr->second.second.left << " " << itr->second.second.top << " ";
                file << itr->second.second.width << " " << itr->second.second.height << " " << mInstance.mCollisions[itr->first] << std::endl;
            }
        }
        file.close();
        mInstance.mSaveNeeded = false;
    }
}

void TilesetManager::addTileset(std::string const& name, std::string const& filename)
{
    mInstance.mTilesets.push_back(Tileset(name,filename));
    mInstance.mSaveNeeded = true;
}

void TilesetManager::bind(int id, std::string const& name, sf::IntRect const& texRect, bool collide)
{
    for (std::size_t i = 0; i < mInstance.mTilesets.size(); i++)
    {
        if (mInstance.mTilesets[i].getName() == name)
        {
            std::pair<std::string,sf::IntRect> pair;
            pair.first = name;
            pair.second = texRect;
            mInstance.mPairs[id] = pair;
            mInstance.mSaveNeeded = true;
            mInstance.mCollisions[id] = collide;
            return;
        }
    }
}

std::pair<sf::Texture*,sf::IntRect> TilesetManager::getPair(int id)
{
    std::pair<sf::Texture*,sf::IntRect> pair;
    if (mInstance.mPairs.find(id) != mInstance.mPairs.end())
    {
        std::pair<std::string,sf::IntRect> spair = mInstance.mPairs[id];
        for (std::size_t i = 0; i < mInstance.mTilesets.size(); i++)
        {
            if (mInstance.mTilesets[i].getName() == spair.first)
            {
                pair.first = mInstance.mTilesets[i].getTexture();
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
    if (mInstance.mCollisions.find(id) != mInstance.mCollisions.end())
    {
        return mInstance.mCollisions[id];
    }
    else
    {
        return false;
    }
}

std::map<int,std::pair<std::string,sf::IntRect>>& TilesetManager::getMap()
{
    return mInstance.mPairs;
}

} // namespace map
