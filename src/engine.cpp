#include "../include/engine.h"

void Engine::initialise()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());
    configureGameWindow(window);

    sf::Clock worldClock;
    sf::Clock debugClock;
    sf::Clock deltaClock;

    sf::Time deltaTime = deltaClock.getElapsedTime();

    // TODO BETTER KEY STRUCTURE
    TextureManager textureManager("test", "resources/assets/character_sprite_sheet_v2.png");
    std::shared_ptr<Player> player = std::make_shared<Player>(textureManager);
    Level level(player);

    std::vector<std::shared_ptr<GameEntity>> gameEntities;
    gameEntities.reserve(TOTAL_ENTITIES);

    gameEntities.emplace_back(std::make_unique<UserInterfaceManager>(player));

    initialiseGameEntities(textureManager, player, gameEntities);

    configureTextRendering();

    //std::vector<sf::Drawable> drawables = { player.getSprite(), level.getTileMap() };
    while (window.isOpen())
    {
        listenForEvents(window, level, deltaTime);
        update(deltaTime, worldClock, level, gameEntities);
        render(window, debugClock, player, level, gameEntities);
        centerViewOnPlayer(window, player, level.getLevelWidth(), level.getLevelHeight());

        deltaTime = deltaClock.restart();
    }

}

void Engine::configureGameWindow(sf::RenderWindow& window)
{
    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    // create a view with its center and size
    // TODO: We should have 2 views, one for UI elements and one for moving for game world .
    //  https://en.sfml-dev.org/forums/index.php?topic=21660.0
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
}

void Engine::initialiseGameEntities(TextureManager& textureManager, std::shared_ptr<Player>& player,
                                    std::vector<std::shared_ptr<GameEntity>>& gameEntities) {
    gameEntities.push_back(player);

    // Load all characters on sprite sheet into memory.
    for (uint32_t rows = 0; rows < (TOTAL_ENEMIES / 4); rows++) {
        for (uint32_t cols = 0; cols < (TOTAL_ENEMIES / 2); cols++) {
            uint32_t enemyX = (cols+1) * 64;
            uint32_t enemyY = (rows+1) * 64;

            uint32_t enemyRectLeft = Enemy::ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = Enemy::ENEMY_HEIGHT * (4 * rows);

            gameEntities.emplace_back(std::make_shared<Enemy>(textureManager, player, enemyX, enemyY, enemyRectLeft, enemyRectTop));
        }
    }
}

void Engine::listenForEvents(sf::RenderWindow& window, Level& level, sf::Time& deltaTime)
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                level.interactWithNode(deltaTime);
            }

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

// TODO Combine deltaTime and worldClock and debugCLock into single class
void Engine::update(sf::Time& deltaTime, sf::Clock& worldClock, Level& level, std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    for (auto& entity : gameEntities)
    {
        entity->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());
    }

    // Update level/map
    level.update(deltaTime, worldClock);
}

void Engine::render(sf::RenderWindow& window, sf::Clock& clock, std::shared_ptr<Player>& player, Level& level,
                    std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    window.clear();

    window.draw(level.map);

    for (auto& entity : gameEntities)
    {
        entity->draw(window, sf::RenderStates::Default);
    }

    if (debugMode)
    {
        startDebugMode(window, clock, player, level);
    }

    window.display();
}

void Engine::centerViewOnPlayer(sf::RenderWindow& window, std::shared_ptr<Player>& player, uint32_t levelWidth, uint32_t levelHeight)
{
    // kee view centred/centered on player
    sf::Vector2f playerPos = player->getPosition();
    float centreX;
    float centreY;

    if (playerPos.x > WINDOW_WIDTH/4)
    {
        if (playerPos.x < (player->PLAYER_WIDTH * levelWidth))
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
        if (playerPos.y < (player->PLAYER_HEIGHT * levelHeight))
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

void Engine::startDebugMode(sf::RenderWindow& window, sf::Clock& clock, std::shared_ptr<Player>& player, Level& level)
{
    float fps = 1.0f / clock.restart().asSeconds();
    debugText.setString(
            "fps: " + std::to_string(fps) + "\n" +
            "window position (x, y): " + "(" + std::to_string(player->getPosition().x) + ", " + std::to_string(player->getPosition().y) + ")" + "\n" +
            "tile position (x, y): " + "(" + std::to_string(player->tilePosition.x) + ", " + std::to_string(player->tilePosition.y) + ")" + "\n"
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
