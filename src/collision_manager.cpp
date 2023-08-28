#include "../include/collision_manager.h"

CollisionManager::CollisionManager() = default;

std::shared_ptr<GameEntity> CollisionManager::compareCollisionWithEnemies(const std::shared_ptr<GameEntity>& currGameEntity,
                                                                          const std::vector<std::shared_ptr<GameEntity>>& gameEntities)
{
    sf::Vector2f currGameEntityPos = currGameEntity->getPosition();
    for (const std::shared_ptr<GameEntity>& entityToCompare: gameEntities) {
        sf::Vector2f entityToComparePos = entityToCompare->getPosition();
        // AA-BB

        if ((currGameEntityPos.x + currGameEntity->getWidth()) > entityToComparePos.x
            && currGameEntityPos.x < (entityToComparePos.x + entityToCompare->getWidth())
            && (currGameEntityPos.y + currGameEntity->getHeight()) > entityToComparePos.y
            && currGameEntityPos.y < (entityToComparePos.y + entityToCompare->getHeight()))
        {
            return entityToCompare;
        }
    }
    return nullptr;
}