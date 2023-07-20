#include "../include/engine.h"

void Engine::initialise()
{
    float screenWidth = sf::VideoMode::getDesktopMode().width;
    float screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "vanquish");

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    // create a view with its center and size
    sf::View view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.25f, 1.25f));
    window.setView(view);

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else 
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }

    sf::Clock clock;
    Player player;
    Level level(&player, 32); // TODO CENTRALISE THIS MAGIC NUM

    //std::vector<sf::Drawable> drawables = { player.getSprite(), level.getTileMap() };
    while (window.isOpen())
    {
        listenForEvents(window);
        update(clock, player, level);
        render(window, player, level);
    }

};

void Engine::listenForEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
};

void Engine::update(sf::Clock& clock, Player& player, Level& level)
{
    level.update(clock);
    player.update();
};

void Engine::render(sf::RenderWindow& window, Player& player, Level& level)
{
    sf::Vector2f playerPos = player.getPlayerPos();
    float centreX = (WINDOW_WIDTH / 2) + playerPos.x - (WINDOW_WIDTH / 4);
    float centreY = (WINDOW_HEIGHT / 2) + playerPos.y - (WINDOW_HEIGHT / 4);

    sf::View newView = window.getView();
    newView.setCenter(centreX, centreY);
    window.setView(newView);

    window.clear();

    /*
    for (auto& drawable : drawables)    
    {
        window.draw(drawable);
    }
    */

    window.draw(level.getTileMap());
    window.draw(player.getSprite());
    window.display();
};
