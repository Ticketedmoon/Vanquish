#ifndef VANQUISH_DEBUG_MANAGER_H
#define VANQUISH_DEBUG_MANAGER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>

#include "common_constants.h"
#include "game_component.h"
#include "player.h"
#include "level.h"
#include "text_manager.h"

class DebugManager : public GameComponent
{
    public:
        DebugManager(std::shared_ptr<Player>& player, Level& level, std::shared_ptr<TextManager>& textManager);
        // TODO FIX THIS INCLUSION OF DRAW EVERYWHERE
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override {};
        void render(sf::RenderTarget& renderTarget, sf::RenderStates states, GameState gameState) override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;

    private:
        void startDebugView();

    private:
        sf::Clock debugClock;

        std::shared_ptr<Player>& player;
        Level& level;
        std::shared_ptr<TextManager>& textManager;
};


#endif //VANQUISH_DEBUG_MANAGER_H
