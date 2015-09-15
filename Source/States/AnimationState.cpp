#include "AnimationState.hpp"

AnimationState::AnimationState(ah::StateManager& manager) : ah::State(manager)
{
    mType = AnimationState::getID();

    mWorld.load();

    mAnimId = mGui.create<sg::TextField>();
    mAnimDur = mGui.create<sg::TextField>();
    mAddFrame = mGui.create<sg::Box>();
    mAddAnimation = mGui.create<sg::Box>();

    hide();
    resetText();

    auto newAnim = mGui.create<sg::Box>();
    newAnim->setFont(ah::Application::instance().getFont("Coolveticca"));
    newAnim->setPosition(25,50);
    newAnim->setSize(sf::Vector2f(250,50));
    newAnim->setString("Create Animation");
    newAnim->setFillColor(sf::Color::Blue);
    newAnim->setCallback([&]()
    {
        mAnimId->setVisible(!mAnimId->isVisible());
        mAnimDur->setVisible(!mAnimDur->isVisible());
        mAddFrame->setVisible(!mAddFrame->isVisible());
        mAddAnimation->setVisible(!mAddAnimation->isVisible());
        resetText();
    });

    mAnimId->setFont(ah::Application::instance().getFont("Coolveticca"));
    mAnimId->setPosition(300,50);
    mAnimId->setFillColor(sf::Color::Green);
    mAnimId->setSize(sf::Vector2f(200,50));
    mAnimId->setCallback([&](){mAnimId->setString("");});

    mAnimDur->setFont(ah::Application::instance().getFont("Coolveticca"));
    mAnimDur->setPosition(525,50);
    mAnimDur->setFillColor(sf::Color::Green);
    mAnimDur->setSize(sf::Vector2f(200,50));
    mAnimDur->setCallback([&](){mAnimDur->setString("");});

    mAddFrame->setFont(ah::Application::instance().getFont("Coolveticca"));
    mAddFrame->setPosition(300,200);
    mAddFrame->setString("Add Frame");
    mAddFrame->setFillColor(sf::Color::Blue);
    mAddFrame->setSize(sf::Vector2f(200,50));
    mAddFrame->setCallback([&]()
    {
        mAnimation.addFrame(lp::from_string<int>(mAnimId->getString()),sf::seconds(lp::from_string<float>(mAnimDur->getString())));
        resetText();
    });

    mAddAnimation->setFont(ah::Application::instance().getFont("Coolveticca"));
    mAddAnimation->setPosition(525,200);
    mAddAnimation->setString("Add Animation");
    mAddAnimation->setFillColor(sf::Color::Blue);
    mAddAnimation->setSize(sf::Vector2f(200,50));
    mAddAnimation->setCallback([&]()
    {
        mWorld.addAnimation(mAnimation);
        mAnimation = Animation();
        hide();
        resetText();
    });

    auto quit = mGui.create<sg::Box>();
    quit->setFont(ah::Application::instance().getFont("Coolveticca"));
    quit->setPosition(0,600);
    quit->setString("Quit");
    quit->setFillColor(sf::Color::Blue);
    quit->setSize(sf::Vector2f(100,100));
    quit->setCallback([&](){ requestPop(); requestPush(MainState::getID()); });
}

AnimationState::~AnimationState()
{
    mWorld.save();
}

std::string AnimationState::getID()
{
    return "AnimationState";
}

bool AnimationState::handleEvent(sf::Event const& event)
{
    mGui.update(event,ah::Application::instance().getMousePosition());

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
    {
        if (mAnimId->hasFocus())
        {
            mAnimId->lostFocus();
            mAnimDur->gainFocus();
        }
    }
    return true;
}

bool AnimationState::update(sf::Time dt)
{
    mGui.update();
    return true;
}

void AnimationState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mGui);
}

void AnimationState::onActivate()
{
    mGui.setVisible(true);
}

void AnimationState::onDeactivate()
{
    mGui.setVisible(false);
    mWorld.save();
}

void AnimationState::hide()
{
    mAnimId->setVisible(false);
    mAnimDur->setVisible(false);
    mAddFrame->setVisible(false);
    mAddAnimation->setVisible(false);
}

void AnimationState::resetText()
{
    mAnimId->setString("Id");
    mAnimDur->setString("Duration");
}
