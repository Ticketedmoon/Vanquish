#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();

    textureManager = std::make_shared<TextureManager>();
    textureManager->addTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY, PLAYER_SPRITE_SHEET_WALK_FILE_PATH);
    textureManager->addTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY, HUMAN_CHARACTER_SPRITE_SHEET_A_FILE_PATH);

    player = std::make_shared<Player>(textureManager);
    level = Level(player, textureManager);

    textManager = std::make_shared<TextManager>(window);
    userInterfaceManager = std::make_shared<UserInterfaceManager>(player);
    viewManager = std::make_unique<ViewManager>(window, level, textManager);
    debugManager = std::make_unique<DebugManager>(player, level, textManager);
}

void Engine::startGameLoop()
{
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        window.clear();

        listenForEvents(deltaTime);
        update(deltaTime, worldClock);
        render();

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

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                level.interactWithNode(deltaTime);
            }

            if (event.key.code == sf::Keyboard::SemiColon)
            {
                gameState = gameState == GameState::PLAYING
                        ? GameState::DEBUG
                        : GameState::PLAYING;
                if (gameState == GameState::PLAYING)
                {
                    // Clear tiles of highlighting
                    level.enableEntityTileHighlightsForDebug({
                        {EntityType::PLAYER, sf::Color::White},
                        {EntityType::ENEMY,  sf::Color::White}
                    });
                }
            }

            if (event.key.code == sf::Keyboard::Space)
            {
                if (gameState == GameState::GAME_OVER)
                {
                    // Restart
                    gameState = GameState::PLAYING;
                }
            }

        }
    }
}

// TODO Combine deltaTime and worldClock and debugCLock into single class
void Engine::update(sf::Time& deltaTime, sf::Clock& worldClock)
{
    userInterfaceManager->update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());

    // TODO INVESTIGATE IF WE CAN MOVE THE PLAYER UPDATE LOGIC OUT OF LEVEL
    // TODO ALSO, WE'RE PASSING LEVEL info into level here, this is bad - refactor.
    level.update(worldClock, deltaTime, level.getLevelWidth(), level.getLevelHeight());

    if (player->isDead())
    {
        gameState = GameState::GAME_OVER;
        worldClock.restart();

        player = std::make_shared<Player>(textureManager);
        level = Level(player, textureManager);
        viewManager = std::make_unique<ViewManager>(window, level, textManager);
        userInterfaceManager = std::make_unique<UserInterfaceManager>(player);
        debugManager = std::make_unique<DebugManager>(player, level, textManager);
    }
}

void Engine::render()
{
    if (gameState == GameState::GAME_OVER)
    {
        showGameOverScreen();
        return;
    }

    viewManager->centerViewOnEntity(player);
    level.draw(window, sf::RenderStates::Default);
    userInterfaceManager->render(window, sf::RenderStates::Default, gameState);

    if (gameState == GameState::DEBUG)
    {
        debugManager->render(window, sf::RenderStates::Default, gameState);
    }
}

void Engine::showGameOverScreen() {
    window.clear(sf::Color::Red);
    const std::string text = "You have died!\nPress [SPACE] to restart";

    // TODO - Make this calculation using text.getLocalBounds()
    float textPositionX = WINDOW_WIDTH / 2 - (text.size() * 0.5 * 20);
    textManager->drawText(text, sf::Color::White, 64, sf::Vector2f(textPositionX, 255));
}