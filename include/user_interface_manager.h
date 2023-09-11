#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

#include "game_entity.h"
#include "user_interface/health_bar.h"

static constexpr size_t TOTAL_UI_COMPONENTS = 1;

class UserInterfaceManager : public GameComponent
{
    public:
        explicit UserInterfaceManager(std::shared_ptr<GameEntity> entity);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override {};
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void render(sf::RenderTarget& renderTarget, sf::RenderStates states, GameState gameState) override;

    private:
        std::vector<std::shared_ptr<GameComponent>> uiComponents;
};