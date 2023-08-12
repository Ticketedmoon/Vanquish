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
#include <string.h>
#include <vector>

#include "player.h"
#include "enemy.h"
#include "level.h"
#include "tilemap.h"

static uint32_t WINDOW_WIDTH = 1280;
static uint32_t WINDOW_HEIGHT = 720;

static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;

static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

class Engine
{
    public:
        void initialise();

    private:
        void listenForEvents(sf::RenderWindow& window, Level& level);
        void update(sf::Clock& clock, Player& player, Level& level, std::vector<Enemy> enemies);
        void render(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level, std::vector<Enemy> enemies);
        void centerViewOnPlayer(sf::RenderWindow& window, Player& player, uint32_t levelWidth, uint32_t levelHeight);

        void startDebugMode(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level);
        void configureTextRendering();

        sf::Font font;
        sf::Text debugText;
        bool debugMode = false;
};
