#include "../include/engine.h"

void Engine::initialise()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());
    configureGameWindow(window);

    TextureManager textureManager;
    textureManager.addTexture(PLAYER_SPRITE_SHEET_A_KEY, PLAYER_SPRITE_SHEET_A_FILE_PATH);
    textureManager.addTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY, HUMAN_CHARACTER_SPRITE_SHEET_A_FILE_PATH);

    configureTextRendering();

    std::shared_ptr<Player> player = std::make_shared<Player>(textureManager);

    std::vector<std::shared_ptr<GameEntity>> gameEntities;
    gameEntities.reserve(TOTAL_GAME_ENTITIES);
    initialiseGameEntities(textureManager, player, gameEntities);

    Level level(player, gameEntities);

    std::vector<std::shared_ptr<GameComponent>> uiComponents;
    uiComponents.clear();
    uiComponents.reserve(TOTAL_UI_COMPONENTS);
    uiComponents.emplace_back(std::make_unique<UserInterfaceManager>(player));

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
                           std::vector<std::shared_ptr<GameComponent>>& uiComponents)
{
    sf::Clock worldClock;
    sf::Clock debugClock;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();
        listenForEvents(window, level, deltaTime);
        window.clear();

        if (isGameOver || player->isDead())
        {
            // TODO INSTEAD OF ALL THESE RESTARTS AND RESETS CAN WE JUST RECREATE ALL OBJECTS
            worldClock.restart();
            debugClock.restart();
            deltaClock.restart();

            for (auto& entity : gameEntities)
            {
                entity->reset();
            }
            showGameOverScreen(window);
            isGameOver = true;
        }
        else
        {
            update(deltaTime, worldClock, level, gameEntities, uiComponents);

            centerViewOnPlayer(window, player, level.getLevelWidth(), level.getLevelHeight());

            // render map/entities/ui
            window.draw(level.map);
            render(window, gameEntities, uiComponents);

            // Debug
            renderDebugInfo(window, debugClock, player, level);
        }

        window.display();
    }
}

void Engine::initialiseGameEntities(TextureManager& textureManager, std::shared_ptr<Player>& player,
                                    std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    gameEntities.push_back(player);

    // Load all characters on sprite sheet into memory.
    double rowCountByTotalEnemies = ceil(TOTAL_ENEMIES / 4.0f);
    size_t totalRows = std::max(rowCountByTotalEnemies, static_cast<double>(1));
    size_t totalCols = totalRows == 1 ? TOTAL_ENEMIES : ceil(TOTAL_ENEMIES/2.0);
    std::cout << "Loading entities from sprite sheet [rows: " << totalRows << ", cols: " << totalCols << "]\n";

    for (uint32_t rows = 0; rows < totalRows; rows++) {
        for (uint32_t cols = 0; cols < totalCols; cols++) {
            if ((rows*4) + cols >= TOTAL_ENEMIES) return;

            // Note: These positions are temporary, so magic numbers aren't concerning for now.
            uint32_t enemyX = (cols + 1) * 64;
            uint32_t enemyY = (rows + 1) * 64;

            uint32_t enemyRectLeft = ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = ENEMY_HEIGHT * (4 * rows);

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(textureManager, player, enemyX, enemyY, enemyRectLeft, enemyRectTop));
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (isGameOver)
                {
                    isGameOver = false;
                }
            }
        
        }
    }
}

// TODO Combine deltaTime and worldClock and debugCLock into single class

// When enemy->enemy collision
// Have them both continue toward the player while minimising collisions.
// Therefore, the collision behaviour needs to be tied in with how entities move x or y.
// If the player is at 2,2 and the enemies 0,0 and 0,1 respectively,
//   enemyA can not go down / increase its y-axis, or a collision will occur, but it can still gain
//   territory by moving 1 or 2 across on the x, although not the most optimal route.
//   We can start with this as a basic form of movement for enemies and pathfinding behaviour.
//
// 1. If the enemy is colliding with another enemy, we need to effect one of them.
//   e.g., in the case of 2, 1 needs to take effect, while the other continues.
void Engine::update(sf::Time& deltaTime, sf::Clock& worldClock, Level& level,
                    std::vector<std::shared_ptr<GameEntity>>& gameEntities,
                    std::vector<std::shared_ptr<GameComponent>>& uiComponents)
{
    // Consider using A* for entity pathing, or is it that overkill? = research.
    for (const auto& entity : gameEntities)
    {
        bool isEntityColliding = performEntityCollisionDetection(entity, gameEntities);
        if (isEntityColliding)
        {
            if (entity->getType() == EntityType::ENEMY)
            {
                // TBD
            }
            else if (entity->getType() == EntityType::PLAYER)
            {
                // TBD
            }
        }
        entity->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());
    }

    for (auto& entity : uiComponents)
    {
        entity->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());
    }

    // TODO INVESTIGATE IF WE CAN MOVE THE PLAYER UPDATE LOGIC OUT OF LEVEL
    level.update(deltaTime);
}

/*
 * Note: This method is somewhat hacky, I don't like it.
 * Come back to this at a later point when enemy pathing becomes more of a focus.
 */
bool Engine::performEntityCollisionDetection(const std::shared_ptr<GameEntity>& entity,
                                             const std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    std::set<std::shared_ptr<GameEntity>> entitiesToCompare(gameEntities.begin(), gameEntities.end());
    entitiesToCompare.erase(entity);
    bool isColliding = false;

    // Can this be condensed into a single higher-order func?
    for (const auto& entityToCompare : entitiesToCompare)
    {
        if (entity->getSpriteGlobalBounds().intersects(entityToCompare->getSpriteGlobalBounds()))
        {
            isColliding = true;
        }
    }
    return isColliding;
}

void Engine::render(sf::RenderWindow& window, std::vector<std::shared_ptr<GameEntity>>& gameEntities,
                    std::vector<std::shared_ptr<GameComponent>>& uiComponents)
{
    for (auto& entity : gameEntities)
    {
        entity->draw(window, sf::RenderStates::Default);
    }

    // Update view to default and render UI
    window.setView(window.getDefaultView());

    for (auto& entity : uiComponents)
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
    float centreX = getViewCentreForCoordinate(playerPos.x, levelWidth, WINDOW_WIDTH, PLAYER_WIDTH);
    float centreY = getViewCentreForCoordinate(playerPos.y, levelHeight, WINDOW_HEIGHT, PLAYER_HEIGHT);

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

void Engine::showGameOverScreen(sf::RenderWindow &window) const {
    sf::Text sf_text = sf::Text("You have died!\nPress [SPACE] to restart", font);
    window.clear(sf::Color::Red);
    sf_text.setFillColor(sf::Color::White);
    sf_text.setCharacterSize(64); // in pixels, not points!
    sf_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect bounds = sf_text.getLocalBounds();
    sf_text.setPosition(WINDOW_WIDTH/2 - bounds.width/2, 225);
    window.draw(sf_text);
}
