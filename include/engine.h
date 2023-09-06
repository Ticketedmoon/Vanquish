#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <set>

#include "texture_manager.h"
#include "user_interface_manager.h"
#include "player.h"
#include "enemy.h"
#include "level.h"
#include "tilemap.h"
#include "common_constants.h"

static uint32_t WINDOW_WIDTH = 1280;
static uint32_t WINDOW_HEIGHT = 720;

static constexpr std::string_view WINDOW_TITLE = "vanquish";
static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;
static float VIEW_ZOOM_FACTOR = 0.5;

static constexpr size_t TOTAL_PLAYERS = 1;
static constexpr size_t TOTAL_ENEMIES = 8;
static constexpr size_t TOTAL_UI_COMPONENTS = 1;
static constexpr size_t TOTAL_GAME_ENTITIES = TOTAL_PLAYERS + TOTAL_ENEMIES;

static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

enum class GameState
{
    PLAYING,
    GAME_OVER,
    DEBUG
};

class Engine {

    public:
        Engine();
        void startGameLoop();

    private:
        // Game loop logic
        void update(sf::Time& deltaTime, sf::Clock& worldClock, sf::Clock& debugClock);
        void render(sf::Clock& debugClock);
        void listenForEvents(sf::Time& deltaTime);

        // Set up
        static void createGameWindow();
        static void initialiseGameEntities();
        void configureTextRendering();

        static void centerViewOnPlayer();
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue,float playerDimensionValue);
        void showGameOverScreen();

        // Debug
        void startDebugView(sf::Clock& debugClock);

    private:
        // TODO Understand 'inline' and why it worked here
        inline static sf::RenderWindow window;
        inline static TextureManager textureManager;
        inline static std::shared_ptr<Player> player;
        inline static Level level;
        inline static std::vector<std::shared_ptr<GameEntity>> gameEntities;
        inline static std::vector<std::shared_ptr<GameComponent>> uiComponents;

        GameState gameState = GameState::PLAYING;

        sf::Font font;

        // Debug
        sf::Text debugText;

    static void initialiseUserInterface();
};
