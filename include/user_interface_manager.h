#pragma once

#include "player.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

class UserInterfaceManager : public GameEntity
{
    public:
        UserInterfaceManager(std::shared_ptr<Player> player);

        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) ;
        void render(sf::RenderWindow& window);
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight);
        void updateAnimation(sf::Clock &worldClock, uint32_t spriteSheetTopOffset);

    private:
        std::shared_ptr<Player> player;
        sf::RectangleShape healthBar;
};