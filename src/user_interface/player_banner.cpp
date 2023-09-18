#include "player_banner.h"

PlayerBanner::PlayerBanner(std::shared_ptr<GameEntity>& gameEntity, sf::Vector2f fillDimensions,
        sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
        sf::Color backgroundFillColour, float outlineThickness, std::shared_ptr<TextManager> textManager)
        : entity(gameEntity), textManager(textManager)
{
    bannerBackground = sf::RectangleShape(backgroundDimensions);
    bannerBackground.setPosition(position);
    bannerBackground.setFillColor(backgroundFillColour);
    bannerBackground.setOutlineColor(outlineColour);
    bannerBackground.setOutlineThickness(outlineThickness);

    banner = sf::RectangleShape(fillDimensions);
    banner.setPosition(position);
    banner.setFillColor(fillColor);
    banner.setOutlineColor(outlineColour);
    banner.setOutlineThickness(outlineThickness);
}

void PlayerBanner::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(bannerBackground, states);
    renderTarget.draw(banner, states);

    std::string playerLevel = "Level: " + std::to_string(entity->getLevel());
    textManager->drawText(playerLevel, sf::Color::Green, 20, sf::Vector2f(108, 66));
}

void PlayerBanner::update(GameState& gameState)
{

}
