#pragma once

#ifndef VANQUISH_ENGINE_H
#define VANQUISH_ENGINE_H

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
#include "game_state.h"
#include "debug/debug_manager.h"

static constexpr std::string_view WINDOW_TITLE = "vanquish";
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;

class Engine {

    public:
        Engine();
        static void startGameLoop();

    private:
        // Game loop logic
        static void listenForEvents();
        static void update();
        static void render();

        static void createGameWindow();
        static void buildGameEngineComponents();
        static void renderCoreGameComponents();
        static void drawRenderTextureToWindow();

    private:
        // TODO Understand 'inline' and why it worked here
        static inline sf::RenderWindow window;
        static inline std::shared_ptr<TextureManager> textureManager;
        static inline std::unique_ptr<ViewManager> viewManager;
        static inline std::shared_ptr<TextManager> textManager; // TODO RENAME, TOO SIMILAR TO textureManager
        static inline std::unique_ptr<DebugManager> debugManager;
        static inline std::shared_ptr<UserInterfaceManager> userInterfaceManager;

        static inline GameState gameState;
        static inline std::shared_ptr<Player> player;
        static inline Level level;

        static inline sf::RenderTexture renderTexture;
        static inline sf::Sprite renderSprite;
};

#endif //VANQUISH_ENGINE_H