#pragma once
#ifndef VANQUISH_TEXT_MANAGER_H
#define VANQUISH_TEXT_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Text.hpp>

#include "common_constants.h"

static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

class TextManager
{
    public:
        explicit TextManager(sf::RenderWindow& window);
        void drawText(sf::String text, sf::Color fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        void configureTextRendering();

    private:
        sf::RenderWindow& m_window;
        sf::Font m_font;
};


#endif //VANQUISH_TEXT_MANAGER_H
