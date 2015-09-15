#include "TilesetState.hpp"

TilesetState::TilesetState(ah::StateManager& manager) : ah::State(manager)
{
    mType = TilesetState::getID();

    TilesetManager::load(Orthogonal::getPath());

    tilesetName = mGui.create<sg::TextField>();
    tilesetFilename = mGui.create<sg::TextField>();
    tilesetAdd = mGui.create<sg::Box>();

    tileId = mGui.create<sg::TextField>();
    tilesetId = mGui.create<sg::TextField>();
    tileCollide = mGui.create<sg::TextField>();
    tileX = mGui.create<sg::TextField>();
    tileY = mGui.create<sg::TextField>();
    tileAdd = mGui.create<sg::Box>();

    hideTileset();
    hideTile();

    resetTilesetText();
    resetTileText();


    // TILESET

    auto newTileset = mGui.create<sg::Box>();
    newTileset->setFont(ah::Application::instance().getFont("Coolveticca"));
    newTileset->setPosition(25,50);
    newTileset->setSize(sf::Vector2f(200,50));
    newTileset->setString("Create Tileset");
    newTileset->setFillColor(sf::Color::Blue);
    newTileset->setCallback([&]()
    {
        tilesetName->setVisible(!tilesetName->isVisible());
        tilesetFilename->setVisible(!tilesetFilename->isVisible());
        tilesetAdd->setVisible(!tilesetAdd->isVisible());
        resetTilesetText();
    });

    tilesetName->setFont(ah::Application::instance().getFont("Coolveticca"));
    tilesetName->setPosition(250,50);
    tilesetName->setFillColor(sf::Color::Green);
    tilesetName->setSize(sf::Vector2f(275,50));
    tilesetName->setCallback([&](){tilesetName->setString("");});

    tilesetFilename->setFont(ah::Application::instance().getFont("Coolveticca"));
    tilesetFilename->setPosition(550,50);
    tilesetFilename->setFillColor(sf::Color::Green);
    tilesetFilename->setSize(sf::Vector2f(400,50));
    tilesetFilename->setCallback([&](){tilesetFilename->setString("");});

    tilesetAdd->setFont(ah::Application::instance().getFont("Coolveticca"));
    tilesetAdd->setPosition(975,50);
    tilesetAdd->setString("Add Tileset");
    tilesetAdd->setFillColor(sf::Color::Blue);
    tilesetAdd->setSize(sf::Vector2f(200,50));
    tilesetAdd->setCallback([&]()
    {
        TilesetManager::addTileset(tilesetName->getString(),tilesetFilename->getString());
        resetTilesetText();
        hideTileset();
    });


    // TILE

    auto newTile = mGui.create<sg::Box>();
    newTile->setFont(ah::Application::instance().getFont("Coolveticca"));
    newTile->setPosition(25,300);
    newTile->setSize(sf::Vector2f(200,50));
    newTile->setString("Create Tile");
    newTile->setFillColor(sf::Color::Blue);
    newTile->setCallback([&]()
    {
        tileId->setVisible(!tileId->isVisible());
        tilesetId->setVisible(!tilesetId->isVisible());
        tileCollide->setVisible(!tileCollide->isVisible());
        tileX->setVisible(!tileX->isVisible());
        tileY->setVisible(!tileY->isVisible());
        tileAdd->setVisible(!tileAdd->isVisible());
    });

    tileId->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileId->setPosition(250,300);
    tileId->setFillColor(sf::Color::Green);
    tileId->setSize(sf::Vector2f(100,50));
    tileId->setCallback([&](){tileId->setString("");});

    tilesetId->setFont(ah::Application::instance().getFont("Coolveticca"));
    tilesetId->setPosition(375,300);
    tilesetId->setFillColor(sf::Color::Green);
    tilesetId->setSize(sf::Vector2f(275,50));
    tilesetId->setCallback([&](){tilesetId->setString("");});

    tileCollide->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileCollide->setPosition(675,300);
    tileCollide->setFillColor(sf::Color::Green);
    tileCollide->setSize(sf::Vector2f(150,50));
    tileCollide->setCallback([&](){tileCollide->setString("");});

    tileX->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileX->setPosition(250,400);
    tileX->setFillColor(sf::Color::Green);
    tileX->setSize(sf::Vector2f(100,50));
    tileX->setCallback([&](){tileX->setString("");});

    tileY->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileY->setPosition(375,400);
    tileY->setFillColor(sf::Color::Green);
    tileY->setSize(sf::Vector2f(100,50));
    tileY->setCallback([&](){tileY->setString("");});

    tileAdd->setFont(ah::Application::instance().getFont("Coolveticca"));
    tileAdd->setPosition(500,400);
    tileAdd->setString("Add Tile");
    tileAdd->setFillColor(sf::Color::Blue);
    tileAdd->setSize(sf::Vector2f(200,50));
    tileAdd->setCallback([&]()
    {
        int id = lp::from_string<int>(tileId->getString());
        std::string name = tilesetId->getString();
        sf::IntRect r;
        r.left = lp::from_string<int>(tileX->getString()) * Orthogonal::getTextureSize().x;
        r.top = lp::from_string<int>(tileY->getString()) * Orthogonal::getTextureSize().y;
        r.width = Orthogonal::getTextureSize().x;
        r.height = Orthogonal::getTextureSize().y;
        bool collide;
        if (tileCollide->getString() == "true")
        {
            collide = true;
        }
        else if (tileCollide->getString() == "false")
        {
            collide = false;
        }
        else
        {
            collide = lp::from_string<bool>(tileCollide->getString());
        }
        TilesetManager::bind(id,name,r,collide);
        resetTileText();
        hideTile();
    });


    // QUIT

    auto quit = mGui.create<sg::Box>();
    quit->setFont(ah::Application::instance().getFont("Coolveticca"));
    quit->setPosition(0,600);
    quit->setString("Quit");
    quit->setFillColor(sf::Color::Blue);
    quit->setSize(sf::Vector2f(100,100));
    quit->setCallback([&](){ requestPop(); requestPush(MainState::getID()); });
}

TilesetState::~TilesetState()
{
    TilesetManager::save(Orthogonal::getPath());
}

std::string TilesetState::getID()
{
    return "TilesetState";
}

bool TilesetState::handleEvent(sf::Event const& event)
{
    mGui.update(event,ah::Application::instance().getMousePosition());

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
    {
        if (tilesetName->hasFocus())
        {
            tilesetName->lostFocus();
            tilesetFilename->gainFocus();
        }

        if (tileX->hasFocus())
        {
            tileX->lostFocus();
            tileY->gainFocus();
        }
        if (tileCollide->hasFocus())
        {
            tileCollide->lostFocus();
            tileX->gainFocus();
        }
        if (tilesetId->hasFocus())
        {
            tilesetId->lostFocus();
            tileCollide->gainFocus();
        }
        if (tileId->hasFocus())
        {
            tileId->lostFocus();
            tilesetId->gainFocus();
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
    {
        if (tilesetFilename->hasFocus())
        {
            TilesetManager::addTileset(tilesetName->getString(),tilesetFilename->getString());
            resetTilesetText();
            hideTileset();
        }
        if (tileY->hasFocus())
        {
            int id = lp::from_string<int>(tileId->getString());
            std::string name = tilesetId->getString();
            sf::IntRect r;
            r.left = lp::from_string<int>(tileX->getString()) * Orthogonal::getTextureSize().x;
            r.top = lp::from_string<int>(tileY->getString()) * Orthogonal::getTextureSize().y;
            r.width = Orthogonal::getTextureSize().x;
            r.height = Orthogonal::getTextureSize().y;
            bool collide;
            if (tileCollide->getString() == "true")
            {
                collide = true;
            }
            else if (tileCollide->getString() == "false")
            {
                collide = false;
            }
            else
            {
                collide = lp::from_string<bool>(tileCollide->getString());
            }
            TilesetManager::bind(id,name,r,collide);
            resetTileText();
            hideTile();
        }
    }

    return true;
}

bool TilesetState::update(sf::Time dt)
{
    mGui.update();
    return true;
}

void TilesetState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mGui);
}

void TilesetState::onActivate()
{
    mGui.setVisible(true);
}

void TilesetState::onDeactivate()
{
    mGui.setVisible(false);
    TilesetManager::save(Orthogonal::getPath());
}

void TilesetState::hideTileset()
{
    tilesetName->setVisible(false);
    tilesetFilename->setVisible(false);
    tilesetAdd->setVisible(false);
}

void TilesetState::hideTile()
{
    tileId->setVisible(false);
    tilesetId->setVisible(false);
    tileCollide->setVisible(false);
    tileX->setVisible(false);
    tileY->setVisible(false);
    tileAdd->setVisible(false);
}

void TilesetState::resetTilesetText()
{
    tilesetName->setString("Tileset Name");
    tilesetFilename->setString("Tileset Filename");
}

void TilesetState::resetTileText()
{
    tileId->setString("Tile Id");
    tilesetId->setString("Tileset Name");
    tileCollide->setString("Tile Collide");
    tileX->setString("Tile X");
    tileY->setString("Tile Y");
}
