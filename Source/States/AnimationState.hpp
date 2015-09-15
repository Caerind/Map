#ifndef ANIMATIONSTATE_HPP
#define ANIMATIONSTATE_HPP

#include "../../Aharos/Application/State.hpp"
#include "../../Aharos/Application/Application.hpp"
#include "../../Aharos/SimpleGui/SimpleGui.hpp"

#include "MainState.hpp"

#include "../Map/Map.hpp"
#include "../Map/Orthogonal.hpp"
#include "../Map/DefaultGen.hpp"
#include "../Map/Animation.hpp"

using namespace map;

class AnimationState : public ah::State
{
    public:
        AnimationState(ah::StateManager& manager);
        ~AnimationState();

        static std::string getID();

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void onActivate();
        void onDeactivate();

        void hide();
        void resetText();

    private:
        Map<Orthogonal,DefaultGen<Orthogonal>> mWorld;

        sg::Container mGui;
        sg::TextField::Ptr mAnimId;
        sg::TextField::Ptr mAnimDur;
        sg::Box::Ptr mAddFrame;
        sg::Box::Ptr mAddAnimation;

        Animation mAnimation;
};

#endif // ANIMATIONSTATE_HPP
