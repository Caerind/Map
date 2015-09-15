#include "WorldState.hpp"

WorldState::WorldState(ah::StateManager& manager) : ah::State(manager)
{
    mType = WorldState::getID();
    mActualId = 1;
    mActualLayer = 0;

    mWorld.load();

    // Setting of World Renderer
    rd::Renderer::getView().setSize(800,600);
    rd::Renderer::getView().setCenter(400,300);
    rd::Renderer::getView().setViewport(sf::FloatRect(0.f,0.f,800.f/1200.f,600.f/700.f));

    // Fill the Tileset Renderer
    mView.setSize(400,600);
    mView.setCenter(200,300);
    mView.setViewport(sf::FloatRect(800.f/1200.f,0.f,400.f/1200.f,600.f/700.f));

    int tL = 400 / Orthogonal::getTextureSize().x;
    float dX = 400.f - static_cast<float>(tL * Orthogonal::getTextureSize().x);

    auto map = TilesetManager::getMap();
    for (auto itr = map.begin(); itr != map.end(); itr++)
    {
        int id = itr->first;
        auto tPair = TilesetManager::getPair(id);
        std::pair<sf::Sprite,int> p;
        mTiles.push_back(p);
        mTiles.back().second = id;
        mTiles.back().first.setTexture(*tPair.first);
        mTiles.back().first.setTextureRect(tPair.second);
        mTiles.back().first.setPosition(dX + ((mTiles.size() - 1) % tL) * Orthogonal::getTextureSize().x, ((mTiles.size() - 1) / tL) * Orthogonal::getTextureSize().y);
    }

    updateSprite();

    // GUI
    auto idG = mGui.create<sg::Box>();
    idG->setFont(ah::Application::instance().getFont("Coolveticca"));
    idG->setPosition(1100,600);
    idG->setSize(sf::Vector2f(100,100));
    idG->setString("Id :");
    idG->setFillColor(sf::Color::Blue);
    idG->setTextAlign(sg::Box::TextAlign::Left);
    idG->setPadding(15.f);

    mLayerG = mGui.create<sg::Box>();
    mLayerG->setFont(ah::Application::instance().getFont("Coolveticca"));
    mLayerG->setPosition(895,600);
    mLayerG->setSize(sf::Vector2f(150,100));
    mLayerG->setString("Layer : " + lp::to_string(mActualLayer));
    mLayerG->setFillColor(sf::Color::Blue);
    mLayerG->setTextAlign(sg::Box::TextAlign::Left);
    mLayerG->setPadding(15.f);

    auto layerGM = mGui.create<sg::Box>();
    layerGM->setFont(ah::Application::instance().getFont("Coolveticca"));
    layerGM->setPosition(1046,600);
    layerGM->setSize(sf::Vector2f(48,49));
    layerGM->setString("-");
    layerGM->setFillColor(sf::Color::Green);
    layerGM->getText().move(0,-10.f);
    layerGM->setCallback([&](){ mActualLayer = std::max(0,mActualLayer - 1); mLayerG->setString("Layer : " + lp::to_string(mActualLayer)); updateSprite(); });

    auto layerGP = mGui.create<sg::Box>();
    layerGP->setFont(ah::Application::instance().getFont("Coolveticca"));
    layerGP->setPosition(1046,650);
    layerGP->setSize(sf::Vector2f(48,50));
    layerGP->setString("+");
    layerGP->setFillColor(sf::Color::Green);
    layerGP->getText().move(0,-5.f);
    layerGP->setCallback([&](){ mActualLayer++; mLayerG->setString("Layer : " + lp::to_string(mActualLayer)); updateSprite(); });

    auto quit = mGui.create<sg::Box>();
    quit->setFont(ah::Application::instance().getFont("Coolveticca"));
    quit->setPosition(0,600);
    quit->setSize(sf::Vector2f(100,100));
    quit->setString("Quit");
    quit->setFillColor(sf::Color::Blue);
    quit->setCallback([&](){ requestPop(); requestPush(MainState::getID()); });
}

WorldState::~WorldState()
{
    mWorld.save();
}

std::string WorldState::getID()
{
    return "WorldState";
}

bool WorldState::handleEvent(sf::Event const& event)
{
    mGui.update(event,ah::Application::instance().getMousePosition());

    // Zoom
    sf::View& view = rd::Renderer::getView();
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        if (event.mouseWheelScroll.delta < 1)
        {
            view.zoom(1.2f);
        }
        else
        {
            view.zoom(0.8f);
        }
    }

    return true;
}

bool WorldState::update(sf::Time dt)
{
    ah::Application::instance().removeDebugInfo("chunk");
    ah::Application::instance().removeDebugInfo("global");
    ah::Application::instance().removeDebugInfo("local");
    ah::Application::instance().removeDebugInfo("tile-id");
    ah::Application::instance().removeDebugInfo("tile-collide");

    mGui.update();

    sf::Vector2f mvtMap;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mvtMap.y--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mvtMap.x--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mvtMap.y++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mvtMap.x++;
    mvtMap *= dt.asSeconds() * 300.f;
    rd::Renderer::getView().move(mvtMap);

    sf::Vector2f mvtView;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && mView.getCenter().y >= 300.f) mvtView.y--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) mvtView.y++;
    mvtView *= dt.asSeconds() * 300.f;
    mView.move(mvtView);
    if (mView.getCenter().y < 300.f) mView.setCenter(mView.getCenter().x,300.f);

    mWorld.update(rd::Renderer::getView());

    sf::Vector2i m = ah::Application::instance().getMousePosition2i();
    sf::Vector2f mPos = ah::Application::instance().getMousePositionView(rd::Renderer::getView());

    if (sf::IntRect(0,0,800,600).contains(m))
    {
        sf::Vector2i gC = Orthogonal::worldToGlobalCoords(mPos);
        ah::Application::instance().setDebugInfo("chunk",lp::to_string(Orthogonal::worldToChunk(mPos).x) + " " + lp::to_string(Orthogonal::worldToChunk(mPos).y));
        ah::Application::instance().setDebugInfo("global",lp::to_string(gC.x) + " " + lp::to_string(gC.y));
        ah::Application::instance().setDebugInfo("local",lp::to_string(Orthogonal::worldToLocalCoords(mPos).x) + " " + lp::to_string(Orthogonal::worldToLocalCoords(mPos).y));
        ah::Application::instance().setDebugInfo("tile-id",lp::to_string(mWorld.getTileId(gC,mActualLayer)));
        ah::Application::instance().setDebugInfo("tile-collide",lp::to_string(mWorld.isCollide(gC,mActualLayer)));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mWorld.setTileId(gC,mActualLayer,mActualId);
        }
    }
    if (sf::IntRect(800,0,400,600).contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        int tL = 400 / Orthogonal::getTextureSize().x;
        float dX = 400.f - static_cast<float>(tL * Orthogonal::getTextureSize().x);
        sf::Vector2f pos = ah::Application::instance().getMousePositionView(mView);
        pos.x -= dX;
        int x = pos.x / Orthogonal::getTextureSize().x;
        int y = pos.y / Orthogonal::getTextureSize().y;
        int id = y * tL + x;
        mActualId = mTiles[id].second;
        updateSprite();
    }

    return true;
}

void WorldState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    rd::Renderer::render(target);

    sf::View old = target.getView();
    target.setView(mView);
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        target.draw(mTiles[i].first);
    }
    target.setView(old);

    target.draw(mGui);
    target.draw(mActualSprite);
}

void WorldState::onActivate()
{
    mGui.setVisible(true);
}

void WorldState::onDeactivate()
{
    mWorld.save();
    rd::Renderer::clear();
    mGui.setVisible(false);
}

void WorldState::updateSprite()
{
    auto p = TilesetManager::getPair(mActualId);
    mActualSprite.setPosition(1155,642);
    mActualSprite.setTexture(*p.first);
    mActualSprite.setTextureRect(p.second);
}
