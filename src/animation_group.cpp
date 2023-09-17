#include "animation_group.h"

AnimationGroup::AnimationGroup(uint32_t framesPerRow, sf::Vector2u startingAnimationPosition,
        sf::Time animationCompletionTime, sf::IntRect entitySpriteSheetDimRect)
        : framesPerRow(framesPerRow), startingAnimationPosition(startingAnimationPosition),
          currentAnimationTime(sf::Time::Zero), animationCompletionTime(animationCompletionTime),
          entitySpriteSheetDimRect(entitySpriteSheetDimRect)
{

}
