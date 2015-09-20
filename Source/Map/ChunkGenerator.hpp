#ifndef MAP_CHUNKGENERATOR_HPP
#define MAP_CHUNKGENERATOR_HPP

#include <memory>
#include <random>

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

        ChunkGenerator();
        virtual ~ChunkGenerator();

        virtual void generate(Chunk& chunk);

        float randomFloat(float a, float b);
        int randomInt(int a, int b);

    private:
        Map* mMap;
        std::mt19937 mRandomGenerator;
};

} // namespace map

#endif // MAP_CHUNKGENERATOR_HPP
