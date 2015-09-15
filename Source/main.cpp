#include "../Aharos/Application/Application.hpp"

#include "States/MainState.hpp"
#include "States/WorldState.hpp"
#include "States/TilesetState.hpp"
#include "States/AnimationState.hpp"

int main()
{
    // Load Resources
    ah::Application::instance().loadFont("Coolveticca","Assets/Fonts/Coolveticca.ttf");

    // Load Application
    ah::Application::instance().showDebugScreen(true);
    ah::Application::instance().setFont(ah::Application::instance().getFont("Coolveticca"));
    ah::Application::instance().useConsole(true);

    // Window Config
    ah::Application::instance().create(sf::VideoMode(1200,700),"Map");
    ah::Application::instance().setIcon("Assets/Textures/icon.png");
    ah::Application::instance().setMouseCursorTexture("Assets/Textures/cursor.png");
    ah::Application::instance().useCustomMouseCursor();

    // Register States
    ah::Application::instance().registerState<MainState>(MainState::getID());
    ah::Application::instance().registerState<WorldState>(WorldState::getID());
    ah::Application::instance().registerState<TilesetState>(TilesetState::getID());
    ah::Application::instance().registerState<AnimationState>(AnimationState::getID());

    // Run
    ah::Application::instance().pushState(MainState::getID());
    ah::Application::instance().run();

    return EXIT_SUCCESS;
}
