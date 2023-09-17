#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
    buildGameEngineComponents();
}

void Engine::startGameLoop()
{
    while (window.isOpen())
    {
        gameState.getClock().restartDeltaClock();

        window.clear();

        listenForEvents();
        update();
        render();

        window.display();
    }
}

// TODO We could make a nice improvement here where we take a map of {Keyboard::Key -> function ptr} and we simply
//      iterate over each during inner loop.
void Engine::listenForEvents()
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
                Level::interactWithTile(gameState.getClock().getDeltaTime(), player, Tile::Type::GRASS);
            }

            if (event.key.code == sf::Keyboard::SemiColon)
            {
                debugManager->updateDebugState(gameState);
            }

            if (event.key.code == sf::Keyboard::Space)
            {
                if (gameState.getState() == GameState::State::GAME_OVER)
                {
                    buildGameEngineComponents();
                }
            }

        }
    }
}

void Engine::update()
{
    if (gameState.getState() == GameState::State::GAME_OVER)
    {
        return;
    }

    userInterfaceManager->update(gameState);

    // TODO INVESTIGATE IF WE CAN MOVE THE PLAYER UPDATE LOGIC OUT OF LEVEL
    level.update(gameState);

    if (gameState.getState() == GameState::State::DEBUG)
    {
        debugManager->update(gameState);
    }
}

void Engine::render()
{
    renderTexture.clear();

    switch (gameState.getState())
    {
        case GameState::State::PLAYING:
            renderCoreGameComponents();
            break;

        case GameState::State::GAME_OVER:
            viewManager->showGameOverView();
            break;

        case GameState::State::DEBUG:
            renderCoreGameComponents();
            debugManager->draw(renderTexture, sf::RenderStates::Default);
            break;
    }

    drawRenderTextureToWindow();
}

void Engine::drawRenderTextureToWindow()
{
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite, sf::RenderStates(sf::BlendAdd));
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

void Engine::buildGameEngineComponents()
{
    renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderSprite.setTexture(renderTexture.getTexture());
    renderSprite.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    textureManager = std::make_shared<TextureManager>();

    // Player Animations
    textureManager->addTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY, PLAYER_SPRITE_SHEET_WALK_FILE_PATH);
    textureManager->addTexture(PLAYER_SPRITE_SHEET_A_HURT_KEY, PLAYER_SPRITE_SHEET_HURT_FILE_PATH);
    textureManager->addTexture(PLAYER_SPRITE_SHEET_A_IDLE_KEY, PLAYER_SPRITE_SHEET_IDLE_FILE_PATH);

    // Enemy Animations
    textureManager->addTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY, HUMAN_CHARACTER_SPRITE_SHEET_A_FILE_PATH);

    player = std::make_shared<Player>(textureManager);
    level = Level(player, textureManager);

    textManager = std::make_shared<TextManager>(renderTexture);
    userInterfaceManager = std::make_shared<UserInterfaceManager>(player);
    viewManager = std::make_unique<ViewManager>(renderTexture, textManager);
    debugManager = std::make_unique<DebugManager>(player, level, textManager);
    gameState = GameState();
}

void Engine::renderCoreGameComponents()
{
    viewManager->centerViewOnEntity(player);
    level.draw(renderTexture, sf::RenderStates::Default);
    userInterfaceManager->draw(renderTexture, sf::RenderStates::Default);
}