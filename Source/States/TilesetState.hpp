#ifndef TILESETSTATE_HPP
#define TILESETSTATE_HPP

#include "../../Aharos/Application/State.hpp"

#include "../../Aharos/SimpleGui/SimpleGui.hpp"

#include "MainState.hpp"

#include "../Map/TilesetManager.hpp"
#include "../Map/Orthogonal.hpp"

using namespace map;

class TilesetState : public ah::State
{
    public:
        TilesetState(ah::StateManager& manager);
        ~TilesetState();

        static std::string getID();

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void onActivate();
        void onDeactivate();

        void hideTileset();
        void hideTile();

        void resetTilesetText();
        void resetTileText();

    private:
        sg::Container mGui;

        sg::TextField::Ptr tilesetName;
        sg::TextField::Ptr tilesetFilename;
        sg::Box::Ptr tilesetAdd;

        sg::TextField::Ptr tileId;
        sg::TextField::Ptr tilesetId;
        sg::TextField::Ptr tileCollide;
        sg::TextField::Ptr tileX;
        sg::TextField::Ptr tileY;
        sg::Box::Ptr tileAdd;
};

#endif // TILESETSTATE_HPP
