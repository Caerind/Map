#include "ChunkGenerator.hpp"
#include "Chunk.hpp"
#include "Map.hpp"

namespace map
{

ChunkGenerator::ChunkGenerator()
{
    mMap = nullptr;
}

ChunkGenerator::~ChunkGenerator()
{
    mMap = nullptr;
}

void ChunkGenerator::generate(Chunk& chunk)
{
    chunk.addLayer();
    sf::Vector2i coords, size = Properties::getChunkSize();
    for (coords.x = 0; coords.x < size.x; coords.x++)
    {
        for (coords.y = 0; coords.y < size.y; coords.y++)
        {
            chunk.setTileId(coords,0,1);
        }
    }
}

float ChunkGenerator::randomFloat(float a, float b)
{
    std::uniform_real_distribution<float> distribution(a,b);
    return distribution(mRandomGenerator);
}

int ChunkGenerator::randomInt(int a, int b)
{
    std::uniform_int_distribution<int> distribution(a,b);
    return distribution(mRandomGenerator);
}

} // namespace map
