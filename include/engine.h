#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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
#include <experimental/random>

#include "texture_manager.h"
#include "user_interface_manager.h"
#include "player.h"
#include "enemy.h"
#include "level.h"
#include "tilemap.h"
#include "view_manager.h"
#include "common_constants.h"
#include "debug/debug_manager.h"
#include "game_clock.h"

static constexpr std::string_view WINDOW_TITLE = "vanquish";
static const bool USE_VERTICAL_SYNC = false;
static const uint32_t APP_FRAME_RATE = 60;

class Engine {

    public:
        Engine();
        void startGameLoop();

    private:
        // Game loop logic
        void update();
        void render();
        void listenForEvents();

        static void createGameWindow();
        static void renderCoreGameComponents();

        // TODO Move to ViewManager?
        static void showGameOverScreen();

    private:
        // TODO Understand 'inline' and why it worked here
        inline static sf::RenderWindow window;
        inline static std::shared_ptr<TextureManager> textureManager;
        inline static std::unique_ptr<ViewManager> viewManager;
        inline static std::shared_ptr<TextManager> textManager; // TODO RENAME, TOO SIMILAR TO textureManager
        inline static std::unique_ptr<DebugManager> debugManager;
        inline static std::shared_ptr<UserInterfaceManager> userInterfaceManager;
        inline static std::shared_ptr<Player> player;
        inline static Level level;
        inline static GameClock gameClock;

        GameState gameState = GameState::PLAYING;
};
