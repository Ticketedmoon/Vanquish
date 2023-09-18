#include "user_interface_manager.h"

UserInterfaceManager::UserInterfaceManager(std::shared_ptr<GameEntity> gameEntity, std::shared_ptr<TextManager> textManager)
    : textManager(textManager)
{
    uiComponents.clear();
    uiComponents.reserve(TOTAL_UI_COMPONENTS);

    std::shared_ptr<PlayerBanner> playerBanner = std::make_shared<PlayerBanner>(gameEntity,
            sf::Vector2f(60, 60),
            sf::Vector2f(60, 60),
            sf::Vector2f(20, 20),
            sf::Color::Transparent,
            sf::Color::Black,
            sf::Color{128, 128, 255, 128},
            2.4f,
            textManager);

    std::shared_ptr<HealthBar> healthBarForEntity = std::make_shared<HealthBar>(gameEntity,
            sf::Vector2f(gameEntity->getHealth(), 15),
            sf::Vector2f(gameEntity->getHealth(), 15),
            sf::Vector2f(108, 20),
            sf::Color::Green,
            sf::Color::Black,
            2.0f);

    std::shared_ptr<ExperiencePointsBar> experienceBarForEntity = std::make_shared<ExperiencePointsBar>(gameEntity,
            sf::Vector2f(gameEntity->getTotalExperiencePoints(), 8),
            sf::Vector2f(gameEntity->getTotalExperiencePointsRequiredForLevelUp(), 8),
            sf::Vector2f(108, 48),
            sf::Color{0, 196, 196, 255},
            sf::Color::Black,
            2.0f);

    uiComponents.emplace_back(playerBanner);
    uiComponents.emplace_back(healthBarForEntity);
    uiComponents.emplace_back(experienceBarForEntity);
}

void UserInterfaceManager::update(GameState& gameState)
{
    for (auto& component: uiComponents)
    {
        component->update(gameState);
    }
}

void UserInterfaceManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.setView(renderTarget.getDefaultView());
    for (auto& component: uiComponents)
    {
        component->draw(renderTarget, sf::RenderStates::Default);
    }
}