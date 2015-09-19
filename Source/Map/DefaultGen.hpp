#ifndef DEFAULTGEN_HPP
#define DEFAULTGEN_HPP

#include "TilesetManager.hpp"
#include "Properties.hpp"
#include "ChunkGenerator.hpp"

class Chunk;

class DefaultGen : public map::ChunkGenerator
{
    public:
        void generate(map::Chunk& chunk);
};

#endif // DEFAULTGEN_HPP
