#ifndef WORLDSTATE_HPP
#define WORLDSTATE_HPP

#include "../../Aharos/Application/State.hpp"

#include "../../Aharos/Renderer/Renderer.hpp"
#include "../../Aharos/SimpleGui/SimpleGui.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "MainState.hpp"

#include "../Map/Map.hpp"
#include "../Map/Orthogonal.hpp"
#include "../Map/DefaultGen.hpp"

using namespace map;

class WorldState : public ah::State
{
    public:
        WorldState(ah::StateManager& manager);
        ~WorldState();

        static std::string getID();

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void onActivate();
        void onDeactivate();

        void updateSprite();

    private:
        Map<Orthogonal,DefaultGen<Orthogonal>> mWorld;

        int mActualId;
        int mActualLayer;

        sf::View mView;
        std::vector<std::pair<sf::Sprite,int>> mTiles;

        sg::Container mGui;
        sg::Box::Ptr mLayerG;

        sf::Sprite mActualSprite;
};

#endif // WORLDSTATE_HPP
