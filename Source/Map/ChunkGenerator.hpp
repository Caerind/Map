#ifndef MAP_CHUNKGENERATOR_HPP
#define MAP_CHUNKGENERATOR_HPP

#include <memory>

#include "TilesetManager.hpp"
#include "Properties.hpp"

namespace map
{

class Map;
class Chunk;

class ChunkGenerator
{
    public:
        friend Map;

        typedef std::unique_ptr<ChunkGenerator> Ptr;

        ChunkGenerator();
        ~ChunkGenerator();

        void generate(Chunk& chunk);

        // TODO : Add random generator in it

    private:
        Map* mMap;
};

} // namespace map

#endif // MAP_CHUNKGENERATOR_HPP
