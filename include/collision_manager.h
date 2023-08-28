#pragma once

#include <vector>

#include "game_entity.h"
#include "enemy.h"

/*
 * How this class works:
 *
 * Provide functionality to validate if an enemy touches another enemy from a list of enemies.
 *
 */
class CollisionManager
{
    public:
        CollisionManager();
        static std::shared_ptr<GameEntity> compareCollisionWithEnemies(const std::shared_ptr<GameEntity>& currGameEntity,
                                                                       const std::vector<std::shared_ptr<GameEntity>>& gameEntities);
};