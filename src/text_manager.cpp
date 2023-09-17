#include "text_manager.h"

TextManager::TextManager(sf::RenderTarget& renderTarget) : m_renderTarget(renderTarget)
{
    // This always loads, even when in dist mode - refactor at a later point.
    configureTextRendering();
}

void TextManager::drawText(sf::String text, sf::Color fillColour, uint8_t characterSize, sf::Vector2f position) {
    sf::Text sf_text = sf::Text(text, m_font);
    sf_text.setFillColor(fillColour);
    sf_text.setCharacterSize(characterSize); // in pixels, not points!
    sf_text.setPosition(position);

    // TODO parameterise us
    sf_text.setOutlineColor(sf::Color::Black);
    sf_text.setOutlineThickness(2.0f);
    sf_text.setLetterSpacing(3.0f);

    m_renderTarget.draw(sf_text);
}

void TextManager::configureTextRendering()
{
    if (!m_font.loadFromFile(FONT_PATH))
    {
        std::string msg = "Failed to load font from font path: " + FONT_PATH;
        throw new std::runtime_error(msg);
    }
}
