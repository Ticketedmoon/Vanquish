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
        const sf::Vector2<float>& healthBarSize = sf::Vector2f(enemyEntity->getHealth(), 2);
        const float healthBarPositionX = ((enemyEntity->getPosition().x + enemyEntity->getWidth()) / 2) - (healthBarSize.x / 2);
        const float healthBarPositionY = ((enemyEntity->getPosition().y + enemyEntity->getHeight()) / 2) + 10;

        std::shared_ptr<EnemyHealthBar> healthBarForEnemy = std::make_shared<EnemyHealthBar>(enemyEntity,
                healthBarSize,
                healthBarSize,
                sf::Vector2f(healthBarPositionX, healthBarPositionY),
                sf::Color::Red,
                sf::Color::Black,
                2.0f);
        enemyHealthBarComponents.insert({enemyEntity->getId(), healthBarForEnemy});
    }
}

// FIXME, this is not great, instead we should perhaps store the ui component on the enemy object.
//        or store game entities in GameState.
//        After thinking about it again, the first option seems very nice, as when the enemy is deleted, the health
//        bar will automatically be removed from the vector of enemies, when passing by reference.
void UserInterfaceManager::update(GameState& gameState)
{
    std::vector<uint32_t> enemyIdsToRemove;
    for (auto& enemyHealthBar: enemyHealthBarComponents)
    {
        std::shared_ptr<EnemyHealthBar> healthBar = enemyHealthBar.second;
        if (healthBar->getEntity()->isDead())
        {
            enemyIdsToRemove.emplace_back(healthBar->getEntity()->getId());
        }

        healthBar->update(gameState);
    }

    // TODO This is not clean at all, refactor all of this block using suggestions at top of method.
    if (!enemyIdsToRemove.empty())
    {
        for (uint32_t id : enemyIdsToRemove)
        {
            enemyHealthBarComponents.erase(id);
        }
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
        component.second->draw(renderTarget, sf::RenderStates::Default);
    }

    renderTarget.setView(renderTarget.getDefaultView());
    for (auto& component: playerUiComponents)
    {
        component->draw(renderTarget, sf::RenderStates::Default);
    }
}