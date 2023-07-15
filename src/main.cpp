#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "./tilemap.cpp"

static uint32_t WINDOW_WIDTH = 800;
static uint32_t WINDOW_HEIGHT = 600;

static bool USE_VERTICAL_SYNC = false;
static bool APP_FRAME_RATE = 60;

static uint32_t SPRITE_SHEET_X = 32;
static uint32_t SPRITE_SHEET_Y = 32;
static uint32_t LAST_SPRITE_LEFT_POS = 64;

void updatePlayerPosition(sf::IntRect& rectSourceSprite, sf::Vector2f& charPos)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        rectSourceSprite.top = SPRITE_SHEET_X * 0;
        if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
        {
            rectSourceSprite.left = 0;
        }
        else 
        {
            rectSourceSprite.left += SPRITE_SHEET_X;
        }

        charPos.y += 10;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        rectSourceSprite.top = SPRITE_SHEET_X * 3;
        if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
        {
            rectSourceSprite.left = 0;
        }
        else 
        {
            rectSourceSprite.left += SPRITE_SHEET_X;
        }

        charPos.y -= 10;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        rectSourceSprite.top = SPRITE_SHEET_X * 1;
        if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
        {
            rectSourceSprite.left = 0;
        }
        else 
        {
            rectSourceSprite.left += SPRITE_SHEET_X;
        }

        charPos.x -= 10;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        rectSourceSprite.top = SPRITE_SHEET_X * 2;
        if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
        {
            rectSourceSprite.left = 0;
        }
        else 
        {
            rectSourceSprite.left += SPRITE_SHEET_X;
        }

        charPos.x += 10;
    }
}

void createWindow()
{
    float screenWidth = sf::VideoMode::getDesktopMode().width;
    float screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "vanquish");

    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));


    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else {
        window.setFramerateLimit(APP_FRAME_RATE);
    }

    sf::Texture texture;
    if (!texture.loadFromFile("assets/character_sprite_sheet.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }

    sf::IntRect rectSourceSprite(0, 0, SPRITE_SHEET_X, SPRITE_SHEET_Y);
    sf::Sprite sprite(texture, rectSourceSprite);
    sf::Clock clock;
    sf::Vector2f charPos;

    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("./assets/basic_tilemap.png", sf::Vector2u(32, 32), level, 16, 8))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }

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

        std::cout << "update" << std::endl;

        if (clock.getElapsedTime().asSeconds() > 0.0f) 
        {
            updatePlayerPosition(rectSourceSprite, charPos);
            sprite.setPosition(charPos);
            sprite.setTextureRect(rectSourceSprite);

            clock.restart();
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
