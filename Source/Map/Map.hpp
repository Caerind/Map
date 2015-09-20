#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

#include <vector>
#include <fstream>
#include <sstream>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "TilesetManager.hpp"
#include "Chunk.hpp"
#include "Animation.hpp"
#include "Properties.hpp"

namespace map
{

class Map
{
    public:
        enum Type
        {
            ClientSolo,
            ClientOnline,
            Server,
        };

        Map(ChunkGenerator* generator); // Client Solo
        Map(sf::TcpSocket* socket); // Client Multi
        Map(ChunkGenerator* generator, sf::TcpSocket* socket); // Server
        ~Map();

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

        // TODO : Test Network
        // Client Reception
        void receiveChunk(sf::Packet& packet);
        void receiveTileModification(sf::Packet& packet);
        // Client Sending
        void setDefaultPacketChunk(sf::Packet packet);
        void setDefaultPacketModification(sf::Packet packet);
        void sendChunkQuery(sf::Vector2i const& coords);
        void sendTileModificationQuery(sf::Vector2i const& coords, int z, int id);
        // Server
        void receiveChunkQuery(sf::Packet& incoming, sf::Packet& out);
        void receiveModificationQuery(sf::Packet& incoming, sf::Packet& out, bool& queryApplied);

    private:
        Type mType;
        ChunkGenerator* mGenerator;
        sf::TcpSocket* mSocket;
        sf::Packet mDefaultPacketChunk;
        sf::Packet mDefaultPacketModification;
        std::vector<Chunk> mChunks;
        std::vector<Animation> mAnimations;
};

} // namespace map

#endif // MAP_MAP_HPP
