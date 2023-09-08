#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();

    textureManager.addTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY, PLAYER_SPRITE_SHEET_WALK_FILE_PATH);
    textureManager.addTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY, HUMAN_CHARACTER_SPRITE_SHEET_A_FILE_PATH);

    initialiseGameEntities();
    initialiseUserInterface();

    level = Level(player, gameEntities);

    textManager = std::make_shared<TextManager>(window);
    viewManager = std::make_unique<ViewManager>(window, level, textManager);
}

void Engine::initialiseUserInterface()
{
    uiComponents.clear();
    uiComponents.reserve(TOTAL_UI_COMPONENTS);
    uiComponents.emplace_back(std::make_unique<UserInterfaceManager>(player));
}

void Engine::startGameLoop()
{
    sf::Clock worldClock;
    sf::Clock debugClock;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        window.clear();

        listenForEvents(deltaTime);
        update(deltaTime, worldClock, debugClock);
        render(debugClock);

        window.display();
    }
}

void Engine::createGameWindow()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());

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

void Engine::initialiseGameEntities()
{
    player = std::make_shared<Player>(textureManager);

    gameEntities.clear();
    gameEntities.reserve(TOTAL_GAME_ENTITIES);
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
void Engine::listenForEvents(sf::Time& deltaTime)
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
                gameState = gameState == GameState::PLAYING
                        ? GameState::DEBUG
                        : GameState::PLAYING;
                if (gameState == GameState::PLAYING)
                {
                    level.enableEntityTileHighlightsForDebug({
                        {EntityType::PLAYER, sf::Color::White},
                        {EntityType::ENEMY,  sf::Color::White}
                    });
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (gameState == GameState::GAME_OVER)
                {
                    gameState = GameState::PLAYING;
                }
            }
        
        }
    }
}

// TODO Combine deltaTime and worldClock and debugCLock into single class
void Engine::update(sf::Time& deltaTime, sf::Clock& worldClock, sf::Clock& debugClock)
{
    // Consider using A* for entity pathing, or is it that overkill? = research.
    for (const auto& entity : gameEntities)
    {
        entity->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());
    }

    for (auto& entity : uiComponents)
    {
        entity->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());
    }

    // TODO INVESTIGATE IF WE CAN MOVE THE PLAYER UPDATE LOGIC OUT OF LEVEL
    level.update(deltaTime);

    if (player->isDead())
    {
        gameState = GameState::GAME_OVER;
        worldClock.restart();
        debugClock.restart();

        initialiseGameEntities();
        initialiseUserInterface();

        level = Level(player, gameEntities);
    }
}

void Engine::render(sf::Clock& debugClock)
{
    if (gameState == GameState::GAME_OVER)
    {
        showGameOverScreen();
        return;
    }

    viewManager->centerViewOnEntity(player);

    window.draw(level.getMap());

    for (auto& entity : gameEntities)
    {
        entity->draw(window, sf::RenderStates::Default);
    }

    // Update view to default and render UI
    window.setView(window.getDefaultView());

    if (gameState == GameState::DEBUG)
    {
        viewManager->startDebugView(debugClock, player);
    }

    for (auto& entity : uiComponents)
    {
        entity->draw(window, sf::RenderStates::Default);
    }
}

void Engine::showGameOverScreen() {
    window.clear(sf::Color::Red);
    const std::string text = "You have died!\nPress [SPACE] to restart";

    // TODO - Make this calculation using text.getLocalBounds()
    float textPositionX = WINDOW_WIDTH / 2 - (text.size() * 0.5 * 20);
    textManager->drawText(text, sf::Color::White, 64, sf::Vector2f(textPositionX, 255));
}