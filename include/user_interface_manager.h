#pragma once

#include "player.h"
#include "user_interface/health_bar.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

class UserInterfaceManager : public GameEntity
{
    public:
        explicit UserInterfaceManager(std::shared_ptr<Player>& player);

        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock &worldClock, uint32_t spriteSheetTopOffset) override;
        void reset() override;

    private:
        std::shared_ptr<Player> player;
        HealthBar healthBar;
};