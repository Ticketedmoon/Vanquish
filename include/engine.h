#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>

#include "player.h"
#include "enemy.h"
#include "level.h"
#include "tilemap.h"

static uint32_t WINDOW_WIDTH = 1280;
static uint32_t WINDOW_HEIGHT = 720;

const std::string WINDOW_TITLE = "vanquish";
static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;

static const size_t TOTAL_PLAYERS = 1;
static const size_t TOTAL_ENEMIES = 8;

static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

class Engine
{
    public:
        void initialise();

    private:
        void listenForEvents(sf::RenderWindow& window, Level& level, sf::Time& deltaTime);
        void render(sf::RenderWindow& window, sf::Clock& clock, std::shared_ptr<Player>& player, Level& level, std::vector<std::shared_ptr<GameEntity>>& gameEntities);
        static void update(sf::Time& deltaTime, sf::Clock& worldClock, Level& level, std::vector<std::shared_ptr<GameEntity>>& gameEntities);
        static void centerViewOnPlayer(sf::RenderWindow& window, std::shared_ptr<Player>& player, uint32_t levelWidth, uint32_t levelHeight);
        static void initialiseGameEntities(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>>& gameEntities);
        static void configureGameWindow(sf::RenderWindow& window);

        void startDebugMode(sf::RenderWindow& window, sf::Clock& clock, std::shared_ptr<Player>& player, Level& level);
        void configureTextRendering();

        sf::Font font;
        sf::Text debugText;
        bool debugMode = false;
};
