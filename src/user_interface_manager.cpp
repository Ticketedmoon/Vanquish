#include "user_interface_manager.h"

UserInterfaceManager::UserInterfaceManager(const std::shared_ptr<Player>& player,
        const std::vector<std::shared_ptr<GameEntity>> enemies,
        const std::shared_ptr<TextManager>& textManager)
    : textManager(textManager)
{
    playerUiComponents.clear();
    playerUiComponents.reserve(TOTAL_UI_COMPONENTS);

    std::shared_ptr<PlayerBanner> playerBanner = std::make_shared<PlayerBanner>(player,
            sf::Vector2f(60, 60),
            sf::Vector2f(60, 60),
            sf::Vector2f(20, 20),
            sf::Color::Transparent,
            sf::Color::Black,
            sf::Color{128, 128, 255, 128},
            2.4f,
            textManager);

    std::shared_ptr<ExperiencePointsBar> experienceBarForEntity = std::make_shared<ExperiencePointsBar>(player,
            sf::Vector2f(player->getTotalExperiencePoints(), 8),
            sf::Vector2f(player->getTotalExperiencePointsRequiredForLevelUp(), 8),
            sf::Vector2f(108, 48),
            sf::Color{0, 196, 196, 255},
            sf::Color::Black,
            2.0f);

    std::shared_ptr<PlayerHealthBar> healthBarForPlayer = std::make_shared<PlayerHealthBar>(player,
            sf::Vector2f(player->getHealth(), 15),
            sf::Vector2f(player->getHealth(), 15),
            sf::Vector2f(108, 20),
            sf::Color::Green,
            sf::Color::Black,
            2.0f);

    playerUiComponents.emplace_back(playerBanner);
    playerUiComponents.emplace_back(healthBarForPlayer);
    playerUiComponents.emplace_back(experienceBarForEntity);

    for (const std::shared_ptr<GameEntity>& enemyEntity : enemies)
    {
        std::shared_ptr<EnemyHealthBar> healthBarForEnemy = std::make_shared<EnemyHealthBar>(enemyEntity,
                sf::Vector2f(enemyEntity->getHealth(), 2),
                sf::Vector2f(enemyEntity->getHealth(), 2),
                sf::Vector2f(enemyEntity->getPosition().x - EnemyHealthBar::HEALTH_BAR_OFFSET_POSITION_X,
                        enemyEntity->getPosition().y + EnemyHealthBar::HEALTH_BAR_OFFSET_POSITION_Y),
                sf::Color::Red,
                sf::Color::Black,
                2.0f);
        enemyHealthBarComponents.emplace_back(healthBarForEnemy);
    }
}

void UserInterfaceManager::update(GameState& gameState)
{
    for (auto& enemyHealthBar: enemyHealthBarComponents)
    {
        enemyHealthBar->update(gameState);
    }

    for (auto& component: playerUiComponents)
    {
        component->update(gameState);
    }
}

void UserInterfaceManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    for (auto& component: enemyHealthBarComponents)
    {
        component->draw(renderTarget, sf::RenderStates::Default);
    }

    renderTarget.setView(renderTarget.getDefaultView());
    for (auto& component: playerUiComponents)
    {
        component->draw(renderTarget, sf::RenderStates::Default);
    }
}