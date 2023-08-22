#include "user_interface_manager.h"

#include <utility>

UserInterfaceManager::UserInterfaceManager(std::shared_ptr<Player> player) : player(std::move(player))
{
    healthBar = sf::RectangleShape(sf::Vector2f(150, 15));
    healthBar.setPosition(sf::Vector2f(20, 20));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setOutlineColor(sf::Color::Black);
    healthBar.setOutlineThickness(2.0);
}

void UserInterfaceManager::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{

}

void UserInterfaceManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBar);
}

void UserInterfaceManager::updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) {};
void UserInterfaceManager::updateAnimation(sf::Clock &worldClock, uint32_t spriteSheetTopOffset) {};