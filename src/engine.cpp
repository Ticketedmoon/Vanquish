#include "../include/engine.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

void Engine::initialise()
{
    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "vanquish");

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    // create a view with its center and size
    sf::View view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    view.zoom(0.5);
    window.setView(view);

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else 
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }

    sf::Clock worldClock;
    sf::Clock debugClock;

    Player player;
    Level level(&player);
    std::vector<Enemy> enemies;
    for (int i = 0; i < 10; i++)
    {
        Enemy enemy(i * 10.0, i * 10.0);
        enemies.emplace_back(enemy);
    }

    configureTextRendering();

    //std::vector<sf::Drawable> drawables = { player.getSprite(), level.getTileMap() };
    while (window.isOpen())
    {
        listenForEvents(window, level);
        update(worldClock, player, level, enemies);
        render(window, debugClock, player, level, enemies);

        centerViewOnPlayer(window, player, level.getLevelWidth(), level.getLevelHeight());
    }

}

void Engine::listenForEvents(sf::RenderWindow& window, Level& level)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            // General player operations
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                level.interactWithNode();
            }

            // Debug
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon))
            {
                debugMode = !debugMode;
                if (!debugMode)
                {
                    level.debug(true);
                }
            }
        
        }
    }
}

void Engine::update(sf::Clock& clock, Player& player, Level& level, std::vector<Enemy> enemies)
{
    player.update();
    level.update(clock);
    for (int i = 0; i < enemies.size(); i++)
    {
        enemies.at(i).update();
    }
}

/* 
 * Consider polymorphic approach:
    for (auto& drawable : drawables)    
    {
        window.draw(drawable);
    }

    TODO Only draw tiles in level that are inside viewport.
    Example code:
         sf::View cam = target.getView();
         sf::FloatRect screenRect(sf::Vector2f(
            cam.getCenter().x - (cam.getSize().x/2.f),
            cam.getCenter().y - (cam.getSize().y/2.f)),
            cam.getSize()
         );
         ...

         if (screenRect.intersects(sf::FloatRect(m_map[i][j].x * 32, m_map[i][j].y * 32, 32, 32)))
*/
void Engine::render(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level, std::vector<Enemy> enemies)
{
    window.clear();

    window.draw(level.map);

    window.draw(player.getSprite());

    for (auto &enemy : enemies)
    {
        window.draw(enemy.getSprite());
    }

    if (debugMode)
    {
        startDebugMode(window, clock, player, level);
    }

    window.display();
}

void Engine::centerViewOnPlayer(sf::RenderWindow& window, Player& player, uint32_t levelWidth, uint32_t levelHeight)
{
    // kee view centred/centered on player
    sf::Vector2f playerPos = player.getPosition();
    float centreX;
    float centreY;

    if (playerPos.x > WINDOW_WIDTH/4)
    {
        if (playerPos.x < (player.PLAYER_WIDTH * levelWidth))
        {
            centreX = playerPos.x;
        }
        else
        {
            centreX = WINDOW_WIDTH - (WINDOW_WIDTH/3);
        }
    }
    else 
    {
        centreX = (WINDOW_WIDTH/4);
    }


    if (playerPos.y > WINDOW_HEIGHT/4)
    {
        if (playerPos.y < (player.PLAYER_HEIGHT * levelHeight))
        {
            centreY = playerPos.y;
        }
        else
        {
            centreY = WINDOW_HEIGHT - (WINDOW_HEIGHT/3);
        }
    }
    else 
    {
        centreY = (WINDOW_HEIGHT/4);
    }
    
    sf::View newView = window.getView();
    newView.setCenter(centreX, centreY);

    window.setView(newView);
}

/**
 * Display an FPS counter on the screen.
 *
 * This will be updated every second rather than every render call.
 * This will be done via sf::Clock.
 *
 * @param window sf::RenderWindow to update
 * @param clock sf::Clock& to track the time.
 */
void Engine::startDebugMode(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level)
{
    float fps = 1.0f / clock.restart().asSeconds();
    debugText.setString(
            "fps: " + std::to_string(fps) + "\n" +
            "window position (x, y): " + "(" + std::to_string(player.getPosition().x) + ", " + std::to_string(player.getPosition().y) + ")" + "\n" +
            "tile position (x, y): " + "(" + std::to_string(player.tilePosition.x) + ", " + std::to_string(player.tilePosition.y) + ")" + "\n"
    );

    sf::Vector2f centerView = window.getView().getCenter();

    float offset = 4.5f;
    debugText.setPosition(centerView.x - WINDOW_WIDTH / offset, centerView.y - WINDOW_HEIGHT / offset);

    window.draw(debugText);

    level.debug(false);
}

void Engine::configureTextRendering()
{
    if (!font.loadFromFile(FONT_PATH))
    {
        std::string msg = "Failed to load font from font path: " + FONT_PATH;
        throw new std::runtime_error(msg);
    }

    debugText.setFont(font); // font is a sf::Font
    debugText.setCharacterSize(9); // in pixels, not points!
    debugText.setFillColor(sf::Color::Green);
    debugText.setOutlineColor(sf::Color::Black);
    debugText.setOutlineThickness(2.0f);
    debugText.setLetterSpacing(3.0f);
}
