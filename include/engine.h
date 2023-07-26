#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

#include <stdexcept>
#include <iostream>
#include <string.h>

#include "player.h"
#include "level.h"

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
        void listenForEvents(sf::RenderWindow& window, Level& level, Player& player);
        void update(sf::Clock& clock, Player& player, Level& level);
        void render(sf::RenderWindow& window, sf::Clock& clock, Player& player, Level& level);
        void centerViewOnPlayer(sf::RenderWindow& window, Player& player);
        void displayDebugText(sf::RenderWindow& window, sf::Clock& clock, Player& player);
        void configureTextRendering();

        sf::Font font;
        sf::Text debugText;
        bool showDebugText = false;
};
