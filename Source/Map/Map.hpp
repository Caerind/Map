#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

#include <vector>
#include <fstream>
#include <sstream>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "TilesetManager.hpp"
#include "Chunk.hpp"
#include "Animation.hpp"
#include "Properties.hpp"

namespace map
{

class Map
{
    public:
        Map(ChunkGenerator::Ptr generator);
        ~Map();

        void load();
        void save();

        bool isChunkLoaded(sf::Vector2i const& chunkCoords);
        sf::Time getChunkLoadedTime() const;

        void update(sf::View const& view);
        void animate(int oldId, int newId);
        void render(sf::RenderTarget& target, sf::View const& view, int layer);

        void setTileId(sf::Vector2i const& coords, int z, int id);
        int getTileId(sf::Vector2i const& coords, int z);
        bool isCollide(sf::Vector2i const& coords, int z);
        const sf::ConvexShape& getShape(sf::Vector2i const& coords, int z);

        Chunk& getChunk(sf::Vector2i const& coords);

        void addAnimation(Animation a);


        // TODO : Online Map

    private:
        ChunkGenerator::Ptr mGenerator;
        std::vector<Chunk> mChunks;
        std::vector<Animation> mAnimations;
};

} // namespace map

#endif // MAP_MAP_HPP
