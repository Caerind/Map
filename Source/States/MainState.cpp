#include "MainState.hpp"
#include "../../Aharos/Application/Application.hpp"

MainState::MainState(ah::StateManager& manager) : ah::State(manager)
{
    mType = MainState::getID();

    auto world = mGui.create<sg::Box>();
    world->setFont(ah::Application::instance().getFont("Coolveticca"));
    world->setPosition(500,100);
    world->setString("World");
    world->setFillColor(sf::Color::Blue);
    world->setSize(sf::Vector2f(200,50));
    world->setCallback([&](){ requestPop(); requestPush(WorldState::getID()); });

    auto tileset = mGui.create<sg::Box>();
    tileset->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileset->setPosition(500,200);
    tileset->setString("Tileset");
    tileset->setFillColor(sf::Color::Blue);
    tileset->setSize(sf::Vector2f(200,50));
    tileset->setCallback([&](){ requestPop(); requestPush(TilesetState::getID()); });

    auto anim = mGui.create<sg::Box>();
    anim->setFont(ah::Application::instance().getFont("Coolveticca"));
    anim->setPosition(500,300);
    anim->setString("Animation");
    anim->setFillColor(sf::Color::Blue);
    anim->setSize(sf::Vector2f(200,50));
    anim->setCallback([&](){ requestPop(); requestPush(AnimationState::getID()); });

    auto quit = mGui.create<sg::Box>();
    quit->setFont(ah::Application::instance().getFont("Coolveticca"));
    quit->setPosition(500,400);
    quit->setString("Quit");
    quit->setFillColor(sf::Color::Blue);
    quit->setSize(sf::Vector2f(200,50));
    quit->setCallback([&](){ requestClear(); });
}

std::string MainState::getID()
{
    return "MainState";
}

bool MainState::handleEvent(sf::Event const& event)
{
    mGui.update(event,ah::Application::instance().getMousePosition());
    return true;
}

bool MainState::update(sf::Time dt)
{
    mGui.update();
    return true;
}

void MainState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mGui,states);
}

void MainState::onActivate()
{
    mGui.setVisible(true);
}

void MainState::onDeactivate()
{
    mGui.setVisible(false);
}
