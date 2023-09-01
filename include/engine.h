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

class Engine {
    public:
        void initialise();

    private:
        // Set up
        void listenForEvents(sf::RenderWindow& window, Level& level, sf::Time& deltaTime);
        static void configureGameWindow(sf::RenderWindow& window);
        static void initialiseGameEntities(TextureManager& textureManager, std::shared_ptr<Player>& player,
                                           std::vector<std::shared_ptr<GameEntity>>& gameEntities);
        void configureTextRendering();

        // Game loop logic
        void startGameLoop(sf::RenderWindow& window, Level& level, std::shared_ptr<Player>& player,
                                  std::vector<std::shared_ptr<GameEntity>>& gameEntities,
                                  std::vector<std::shared_ptr<GameComponent>>& uiComponents);
        static void update(sf::Time& deltaTime, sf::Clock& worldClock, Level& level,
                           std::vector<std::shared_ptr<GameEntity>>& enemies,
                           std::vector<std::shared_ptr<GameComponent>>& uiComponents);
        static void render(sf::RenderWindow &window, std::vector<std::shared_ptr<GameEntity>>& gameEntities,
                           std::vector<std::shared_ptr<GameComponent>>& uiComponents);
        static bool performEntityCollisionDetection(const std::shared_ptr<GameEntity>& entity,
                                                    const std::vector<std::shared_ptr<GameEntity>>& gameEntities);

        static void centerViewOnPlayer(sf::RenderWindow& window, std::shared_ptr<Player>& player,
                                       uint32_t levelWidth,uint32_t levelHeight);
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue,float playerDimensionValue);
        void showGameOverScreen(sf::RenderWindow &window) const;

        // Debug
        void startDebugMode(sf::RenderWindow& window, sf::Clock& debugClock, std::shared_ptr<Player>& player, Level& level);
        void renderDebugInfo(sf::RenderWindow& window, sf::Clock& debugClock, std::shared_ptr<Player>& player, Level& level);

        bool isGameOver = false;
        sf::Font font;

        // Debug
        sf::Text debugText;
        inline static bool debugMode = false;
};
