#pragma once

#ifndef VANQUISH_ANIMATION_GROUP_H
#define VANQUISH_ANIMATION_GROUP_H

#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

class AnimationGroup
{
    public:
        explicit AnimationGroup(uint32_t framesPerRow, sf::Vector2u startingAnimationPosition, sf::Time animationCompletionTime,
                sf::IntRect entitySpriteSheetDimRect);

    public:
        // TODO Getters or make this class a struct
        uint32_t framesPerRow;
        sf::Time currentAnimationTime;
        sf::Time animationCompletionTime;
        sf::IntRect entitySpriteSheetDimRect;
        sf::Vector2u startingAnimationPosition;
};

#endif //VANQUISH_ANIMATION_GROUP_H
