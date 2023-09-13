#pragma once

#ifndef VANQUISH_GAME_COMPONENT_H
#define VANQUISH_GAME_COMPONENT_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "common_constants.h"
#include "game_clock.h"

class GameComponent : public sf::Drawable, public sf::Transformable
{
    public:
        GameComponent() = default;
        ~GameComponent() override = default;

        // TODO SINCE THIS IS NOT VIRTUAL, ARE WE REQUIRED TO OVERRIDE?
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        virtual void update(GameClock& gameClock) = 0;

};

#endif //VANQUISH_GAME_COMPONENT_H