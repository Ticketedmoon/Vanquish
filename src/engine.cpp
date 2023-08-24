#include "../include/engine.h"

void Engine::initialise()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());
    configureGameWindow(window);

    // TODO BETTER KEY STRUCTURE
    TextureManager textureManager("test", "resources/assets/character_sprite_sheet_v2.png");
    std::shared_ptr<Player> player = std::make_shared<Player>(textureManager);
    Level level(player);

    std::vector<std::shared_ptr<GameEntity>> gameEntities;
    gameEntities.reserve(TOTAL_GAME_ENTITIES);
    initialiseGameEntities(textureManager, player, gameEntities);

    std::vector<std::shared_ptr<GameEntity>> uiComponents;
    gameEntities.reserve(TOTAL_UI_COMPONENTS);
    uiComponents.emplace_back(std::make_unique<UserInterfaceManager>(player));

    configureTextRendering();

    startGameLoop(window, level, player, gameEntities, uiComponents);
}

void Engine::configureGameWindow(sf::RenderWindow& window)
{
    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }
}

void Engine::startGameLoop(sf::RenderWindow& window, Level& level, std::shared_ptr<Player>& player,
                           std::vector<std::shared_ptr<GameEntity>>& gameEntities,
                           std::vector<std::shared_ptr<GameEntity>>& uiComponents)
{
    sf::Clock worldClock;
    sf::Clock debugClock;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        listenForEvents(window, level, deltaTime);
        update(deltaTime, worldClock, level, gameEntities);

        window.clear();
        centerViewOnPlayer(window, player, level.getLevelWidth(), level.getLevelHeight());

        // map/entities
        window.draw(level.map);
        render(window, gameEntities);

        // Update view to default and render UI
        window.setView(window.getDefaultView());
        render(window, uiComponents);

        // Debug
        renderDebugInfo(window, debugClock, player, level);
        window.display();
    }
}

void Engine::initialiseGameEntities(TextureManager& textureManager, std::shared_ptr<Player>& player,
                                    std::vector<std::shared_ptr<GameEntity>>& gameEntities) {
    gameEntities.push_back(player);

    // Load all characters on sprite sheet into memory.
    for (uint32_t rows = 0; rows < (TOTAL_ENEMIES / 4); rows++) {
        for (uint32_t cols = 0; cols < (TOTAL_ENEMIES / 2); cols++) {
            // TODO These positions are temporary, so magic numbers aren't concerning for now.
            uint32_t enemyX = (cols+1) * 64;
            uint32_t enemyY = (rows+1) * 64;

            uint32_t enemyRectLeft = Enemy::ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = Enemy::ENEMY_HEIGHT * (4 * rows);

            gameEntities.emplace_back(std::make_shared<Enemy>(textureManager, player, enemyX, enemyY, enemyRectLeft, enemyRectTop));
        }
    }
}

// TODO We could make a nice improvement here where we take a map of {Keyboard::Key -> function ptr} and we simply
//      iterate over each during inner loop.
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

    level.update(deltaTime, worldClock);
}

void Engine::render(sf::RenderWindow& window, std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    for (auto& entity : gameEntities)
    {
        entity->draw(window, sf::RenderStates::Default);
    }
}

void Engine::renderDebugInfo(sf::RenderWindow& window, sf::Clock& debugClock, std::shared_ptr<Player>& player, Level& level)
{
    if (debugMode)
    {
        startDebugMode(window, debugClock, player, level);
    }
}

void Engine::centerViewOnPlayer(sf::RenderWindow& window, std::shared_ptr<Player>& player, uint32_t levelWidth, uint32_t levelHeight)
{
    // kee view centred/centered on player
    sf::Vector2f playerPos = player->getPosition();
    float centreX = getViewCentreForCoordinate(playerPos.x, levelWidth, WINDOW_WIDTH, player->PLAYER_WIDTH);
    float centreY = getViewCentreForCoordinate(playerPos.y, levelHeight, WINDOW_HEIGHT, player->PLAYER_HEIGHT);

    sf::View newView = window.getView();
    newView.zoom(VIEW_ZOOM_FACTOR);
    newView.setCenter(centreX, centreY);

    window.setView(newView);
}

float Engine::getViewCentreForCoordinate(const float playerCoordinatePosition, const float levelDimension,
                                         const float windowDimensionValue, const float playerDimensionValue) {
    float coordinateValue;
    if (playerCoordinatePosition > windowDimensionValue / 4)
    {
        if (playerCoordinatePosition < (playerDimensionValue * levelDimension))
        {
            coordinateValue = playerCoordinatePosition;
        }
        else
        {
            coordinateValue = windowDimensionValue - (windowDimensionValue / 3);
        }
    }
    else
    {
        coordinateValue = (windowDimensionValue / 4);
    }
    return coordinateValue;
}

// TODO ADD IMPROVEMENT TO ENABLE DEBUG CODE ONLY IF CODE IS RUN IN DEBUG CONFIGURATION>
//      WE DO NOT WANT TO HAVE DEBUG CODE IN DISTRIBUTION BUILDS.
void Engine::startDebugMode(sf::RenderWindow& window, sf::Clock& debugClock, std::shared_ptr<Player>& player, Level& level)
{
    float fps = 1.0f / debugClock.restart().asSeconds();
    debugText.setString(
            "fps: " + std::to_string(fps) + "\n" +
            "window position (x, y): " + "(" + std::to_string(player->getPosition().x) + ", " + std::to_string(player->getPosition().y) + ")" + "\n" +
            "tile position (x, y): " + "(" + std::to_string(player->tilePosition.x) + ", " + std::to_string(player->tilePosition.y) + ")" + "\n"
    );

    sf::Vector2f debugTextLocation = sf::Vector2f(25, 50);
    debugText.setPosition(debugTextLocation);
    debugText.setCharacterSize(12);

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
