#ifndef MAP_CHUNK_HPP
#define MAP_CHUNK_HPP

#include <sstream>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/Packet.hpp>

#include "Layer.hpp"
#include "Properties.hpp"
#include "ChunkGenerator.hpp"

namespace map
{

class Chunk
{
    public:
        Chunk(sf::Vector2i const& coords, ChunkGenerator& generator);
        Chunk(sf::Vector2i const& coords);
        ~Chunk();

        void addLayer();
        void removeLayers();
        std::size_t getLayerCount() const;
        void render(sf::RenderTarget& target, int layer);

        sf::Vector2i getCoords() const;
        sf::FloatRect getBounds() const;

        void setTileId(sf::Vector2i const& coords, int layer, int id);
        int getTileId(sf::Vector2i const& coords, int layer) const;

        bool isCollide(sf::Vector2i const& coords, int layer) const;
        const sf::ConvexShape& getShape(sf::Vector2i const& coords, int layer) const;

        void resetTimer();
        sf::Time getElapsedTime() const;

        bool load();
        void save();

        void loadFromPacket(sf::Packet& packet);
        void writeToPacket(sf::Packet& packet);

        static std::string getPathFromCoords(sf::Vector2i const& coords);

    private:
        sf::Vector2i mCoords;
        bool mSave;
        bool mNeedSave;
        std::vector<Layer> mLayers;
        sf::Clock mClock;
};

} // namespace map

#endif // MAP_CHUNK_HPP
