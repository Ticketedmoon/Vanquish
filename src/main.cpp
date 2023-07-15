#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>

static bool USE_VERTICAL_SYNC = false;
static uint32_t WINDOW_WIDTH = 800;
static uint32_t WINDOW_HEIGHT = 600;

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
        window.setFramerateLimit(120);
    }

    sf::CircleShape shape(WINDOW_WIDTH);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

}

int main()
{
    std::cout << "Application Setup!" << std::endl;
    createWindow();
    return 0;
}
