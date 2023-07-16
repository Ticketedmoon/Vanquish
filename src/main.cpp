#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "./tilemap.cpp"
#include <cmath>

enum class PlayerDirection { 
    DOWN = 0,
    UP = 1,
    LEFT = 2,
    RIGHT = 3
};

static uint32_t WINDOW_WIDTH = 800;
static uint32_t WINDOW_HEIGHT = 600;

static bool USE_VERTICAL_SYNC = false;
static uint32_t APP_FRAME_RATE = 60;

static uint32_t SPRITE_SHEET_X = 32;
static uint32_t SPRITE_SHEET_Y = 32;
static uint32_t LAST_SPRITE_LEFT_POS = 64;

static uint32_t PLAYER_SPEED = 32;
static float GAME_TICK = 0.025;

static uint32_t level[] =
{
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
    0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
    2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 0, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
};

uint32_t LEVEL_ROW_SIZE = 16;
uint32_t TOTAL_LEVEL_COLUMNS = (sizeof(level) / sizeof(level[0])) / LEVEL_ROW_SIZE;

static PlayerDirection playerDir = PlayerDirection::DOWN;

// create the tilemap from the level definition
TileMap map;

void loadLevel()
{

    if (!map.load("./assets/basic_tilemap.png", sf::Vector2u(32, 32), level, LEVEL_ROW_SIZE, TOTAL_LEVEL_COLUMNS))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

void updatePlayerAnimation(sf::IntRect& rectSourceSprite, uint32_t spriteSheetTopOffset, PlayerDirection direction)
{
    rectSourceSprite.top = SPRITE_SHEET_X * spriteSheetTopOffset;
    if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
    {
        rectSourceSprite.left = 0;
    }
    else 
    {
        rectSourceSprite.left += SPRITE_SHEET_X;
    }
    playerDir = direction;
}

void checkForMoveVerticalDirection(sf::IntRect& rectSourceSprite, sf::Vector2f& charPos)
{
    int tileUnderPlayerX = floor(charPos.x/SPRITE_SHEET_X);
    int tileUnderPlayerY = floor(charPos.y/SPRITE_SHEET_X) * 16;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        updatePlayerAnimation(rectSourceSprite, 0, PlayerDirection::DOWN);
        if (tileUnderPlayerY >= ((TOTAL_LEVEL_COLUMNS-1) * LEVEL_ROW_SIZE) || level[tileUnderPlayerX + (tileUnderPlayerY + LEVEL_ROW_SIZE)] > 0)
        {
            return;
        }

        charPos.y += PLAYER_SPEED;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        updatePlayerAnimation(rectSourceSprite, 3, PlayerDirection::UP);
        if (tileUnderPlayerY <= 0 || (level[tileUnderPlayerX + (tileUnderPlayerY - LEVEL_ROW_SIZE)] > 0))
        {
            return;
        }

        charPos.y -= PLAYER_SPEED;
    }

}

void checkForMoveHorizontalDirection(sf::IntRect& rectSourceSprite, sf::Vector2f& charPos)
{
    int tileUnderPlayerX = floor(charPos.x/SPRITE_SHEET_X);
    int tileUnderPlayerY = floor(charPos.y/SPRITE_SHEET_X) * LEVEL_ROW_SIZE;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        updatePlayerAnimation(rectSourceSprite, 1, PlayerDirection::LEFT);
        if (tileUnderPlayerX == 0 || level[(tileUnderPlayerX - 1) + tileUnderPlayerY] > 0)
        {
            return;
        }

        charPos.x -= PLAYER_SPEED;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        updatePlayerAnimation(rectSourceSprite, 2, PlayerDirection::RIGHT);
        if (tileUnderPlayerX >= (LEVEL_ROW_SIZE-1) || level[(tileUnderPlayerX + 1) + tileUnderPlayerY] > 0)
        {
            return;
        }

        charPos.x += PLAYER_SPEED;
    }
}

void checkForChopTree(sf::Vector2f& charPos)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        int tileUnderPlayerX = floor(charPos.x/SPRITE_SHEET_X);
        int tileUnderPlayerY = floor(charPos.y/SPRITE_SHEET_X) * LEVEL_ROW_SIZE;

        if (playerDir == PlayerDirection::DOWN)
        {
            uint32_t tileBelowPlayerPos = tileUnderPlayerX + (tileUnderPlayerY + LEVEL_ROW_SIZE);
            if (level[tileBelowPlayerPos] == 2)
            {
                std::cout << "cut tree!" << std::endl;
                level[tileBelowPlayerPos] = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
            // TODO DO OTHER DIRS
        }
    }
}


// TODO A better solution might be to temporarily update player position in tmp vars.
//      If validation passes, commit update player pos.
//      Otherwise, reject and don't move player.
void updatePlayer(sf::IntRect& rectSourceSprite, sf::Vector2f& charPos)
{
    checkForMoveVerticalDirection(rectSourceSprite, charPos);
    checkForMoveHorizontalDirection(rectSourceSprite, charPos);
    checkForChopTree(charPos);
}

void createWindow()
{
    float screenWidth = sf::VideoMode::getDesktopMode().width;
    float screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "vanquish");

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    sf::IntRect rectSourceSprite(0, 0, SPRITE_SHEET_X, SPRITE_SHEET_Y);
    sf::Clock clock;
    sf::Vector2f charPos = sf::Vector2f(0.0f, 0.0f);

    // create a view with its center and size
    sf::View view(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.75f, 1.75f));

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else 
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }

    sf::Texture texture;
    if (!texture.loadFromFile("assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }
    texture.setSmooth(true);

    sf::Sprite sprite(texture, rectSourceSprite);

    loadLevel();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (clock.getElapsedTime().asSeconds() > GAME_TICK) 
        {
            updatePlayer(rectSourceSprite, charPos);
            sprite.setPosition(charPos);
            sprite.setTextureRect(rectSourceSprite);
            clock.restart();

            view.setCenter((WINDOW_WIDTH / 2) + charPos.x - (WINDOW_WIDTH / 4), (WINDOW_HEIGHT / 2) + charPos.y - (WINDOW_HEIGHT / 4));
            window.setView(view);
        }

        window.clear();
        window.draw(map);
        window.draw(sprite);
        window.display();
    }

}

int main()
{
    std::cout << "Application Setup!" << std::endl;
    createWindow();
    return 0;
}
