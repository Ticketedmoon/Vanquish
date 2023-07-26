#include "../include/engine.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

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

    sf::Clock worldClock;
    sf::Clock debugClock;

    Player player;
    Level level(&player);

    configureTextRendering();

    //std::vector<sf::Drawable> drawables = { player.getSprite(), level.getTileMap() };
    while (window.isOpen())
    {
        listenForEvents(window, level, player);
        update(worldClock, player, level);
        render(window, debugClock, player, level);
    }

};

void Engine::listenForEvents(sf::RenderWindow& window, Level& level, Player& player)
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
                level.chopTree();
            }

            // Debug
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon))
            {
                showDebugText = !showDebugText;
            }
        
        }
    }
};

void Engine::update(sf::Clock& clock, Player& player, Level& level)
{
    player.update();
    level.update(clock);
};

/* 
 * Consider polymorphic approach:
    for (auto& drawable : drawables)    
    {
        window.draw(drawable);
    }
*/
void Engine::render(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level)
{
    window.clear();
    centerViewOnPlayer(window, player);

    window.draw(level.getTileMap());
    window.draw(player.getSprite());

    displayDebugText(window, clock, player);
    window.display();
};

void Engine::centerViewOnPlayer(sf::RenderWindow& window, Player& player)
{
    // kee view centred/centered on player
    sf::Vector2f playerPos = player.getPlayerPos();
    float centreX = (WINDOW_WIDTH / 2) + playerPos.x - (WINDOW_WIDTH / 4);
    float centreY = (WINDOW_HEIGHT / 2) + playerPos.y - (WINDOW_HEIGHT / 4);

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
void Engine::displayDebugText(sf::RenderWindow& window, sf::Clock& clock, Player& player)
{
    if (!showDebugText)
    {
        return;
    }

    float fps = 1.0f / clock.restart().asSeconds();
    debugText.setString(
            "fps: " + std::to_string(fps) + "\n" +
            "position (x, y): " + "(" + std::to_string(player.tilePosition.x) + ", " + std::to_string(player.tilePosition.y) + ")" + "\n"
    );

    sf::Vector2f centerView = window.getView().getCenter();

    float offset = 2.15f;
    debugText.setPosition(centerView.x - WINDOW_WIDTH / offset, centerView.y - WINDOW_HEIGHT / offset);

    window.draw(debugText);
}

void Engine::configureTextRendering()
{
    if (!font.loadFromFile(FONT_PATH))
    {
        std::string msg = "Failed to load font from font path: " + FONT_PATH;
        throw new std::runtime_error(msg);
    }

    debugText.setFont(font); // font is a sf::Font
    debugText.setCharacterSize(12); // in pixels, not points!
    debugText.setFillColor(sf::Color::Green);
    debugText.setOutlineColor(sf::Color::Black);
    debugText.setOutlineThickness(3.0f);
    debugText.setLetterSpacing(3.0f);
}
