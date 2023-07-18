#include <SFML/Graphics.hpp>

#include "../include/player.h"
#include "../include/level.h"

static uint32_t WINDOW_WIDTH = 800;
static uint32_t WINDOW_HEIGHT = 600;

static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;

static float GAME_TICK = 0.05;

void listenForEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void update(sf::Clock& clock, Player& player, Level& level)
{
    if (clock.getElapsedTime().asSeconds() > GAME_TICK) 
    {
        level.update();
        player.update();
        clock.restart();
    }
}

void render(sf::RenderWindow& window, Player& player, Level& level)
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
}

void initialise()
{
    float screenWidth = sf::VideoMode::getDesktopMode().width;
    float screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "vanquish");

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    // create a view with its center and size
    sf::View view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.75f, 1.75f));

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

}

int main()
{
    std::cout << "Application Setup!" << std::endl;
    initialise();
    return 0;
}
