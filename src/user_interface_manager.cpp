#include "user_interface_manager.h"

UserInterfaceManager::UserInterfaceManager(std::shared_ptr<Player>& player)
    : player(player),
      healthBar(HealthBar(sf::Vector2f(player->getHealth(), 15), sf::Vector2f(20, 20), sf::Color::Green, sf::Color::Black, 2.0f))
{
}

void UserInterfaceManager::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    healthBar.update(player->getHealth());
}

void UserInterfaceManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBar);
}