#ifndef MAP_DEFAULTGEN_HPP
#define MAP_DEFAULTGEN_HPP

#include "TilesetManager.hpp"
#include "Properties.hpp"
#include "ChunkGenerator.hpp"

namespace map
{

class Chunk;

class DefaultGen : public ChunkGenerator
{
    public:
        void generate(Chunk& chunk);
};

} // namespace map

#endif // MAP_DEFAULTGEN_HPP
