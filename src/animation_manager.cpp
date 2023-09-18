#include "animation_manager.h"

void AnimationManager::updateAnimationGroup(uint32_t entityWidth, uint32_t entityHeight, int directionIndex,
        std::shared_ptr<AnimationGroup>& animationGroup)
{
    bool isAnimationCompleted = animationGroup->entitySpriteSheetDimRect.left
                                == animationGroup->startingAnimationPosition.x + (entityWidth * (animationGroup->framesPerRow - 1));
    uint32_t spriteSheetTop = animationGroup->startingAnimationPosition.y + (entityHeight * directionIndex);

    animationGroup->entitySpriteSheetDimRect.top = spriteSheetTop;
    animationGroup->currentAnimationTime = sf::Time::Zero;

    if (!isAnimationCompleted)
    {
        animationGroup->entitySpriteSheetDimRect.left = animationGroup->entitySpriteSheetDimRect.left + entityWidth;
        return;
    }

    if (animationGroup->animationCompletionType == AnimationGroup::AnimationCompletionType::COMPLETE_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
    {
        animationGroup->entitySpriteSheetDimRect.left = animationGroup->entitySpriteSheetDimRect.left;
        return;
    }
    else if (animationGroup->animationCompletionType == AnimationGroup::AnimationCompletionType::RESET_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
    {
        return;
    }
    else if (animationGroup->animationCompletionType == AnimationGroup::AnimationCompletionType::REPEAT_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
    {
        animationGroup->entitySpriteSheetDimRect.left = animationGroup->startingAnimationPosition.x;
        return;
    }

    throw new std::runtime_error("Animation Completion type not implemented");
}

void AnimationManager::resetAnimation(std::shared_ptr<AnimationGroup>& animationGroup)
{
    animationGroup->entitySpriteSheetDimRect.left = animationGroup->startingAnimationPosition.x;
}
