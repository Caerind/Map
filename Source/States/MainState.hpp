#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include "../../Aharos/Application/State.hpp"

#include "../../Aharos/SimpleGui/SimpleGui.hpp"

#include "WorldState.hpp"
#include "TilesetState.hpp"
#include "AnimationState.hpp"

class MainState : public ah::State
{
    public:
        MainState(ah::StateManager& manager);

        static std::string getID();

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void onActivate();
        void onDeactivate();

    private:
        sg::Container mGui;
};

#endif // MAINSTATE_HPP
