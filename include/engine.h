#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "player.h"
#include "level.h"

static uint32_t WINDOW_WIDTH = 800;
static uint32_t WINDOW_HEIGHT = 600;

static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;

static float GAME_TICK = 0.01;

class Engine
{
    public:
        void initialise();

    private:
        void listenForEvents(sf::RenderWindow& window);
        void update(sf::Clock& clock, Player& player, Level& level);
        void render(sf::RenderWindow& window, Player& player, Level& level);

};
