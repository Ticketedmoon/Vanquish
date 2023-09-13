#include "user_interface_manager.h"

UserInterfaceManager::UserInterfaceManager(std::shared_ptr<GameEntity> gameEntity)
{
    uiComponents.clear();
    uiComponents.reserve(TOTAL_UI_COMPONENTS);

    std::shared_ptr<HealthBar> healthBarForEntity = std::make_shared<HealthBar>(
            gameEntity, sf::Vector2f(gameEntity->getHealth(), 15), sf::Vector2f(20, 20), sf::Color::Green, sf::Color::Black, 2.0f);

    uiComponents.emplace_back(healthBarForEntity);
}

void UserInterfaceManager::update(GameState& gameState)
{
    for (auto& component : uiComponents)
    {
        component->update(gameState);
    }
}

void UserInterfaceManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.setView(renderTarget.getDefaultView());
    for (auto& component : uiComponents)
    {
        component->draw(renderTarget, sf::RenderStates::Default);
    }
}