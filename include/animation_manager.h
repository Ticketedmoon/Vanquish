#pragma once

#ifndef VANQUISH_ANIMATION_MANAGER_H
#define VANQUISH_ANIMATION_MANAGER_H

#include <string>
#include <memory>
#include "game_clock.h"
#include "animation_group.h"
#include <iostream>

class AnimationManager
{
    public:
        AnimationManager() = default;

        static void updateAnimationGroup(uint32_t entityWidth, uint32_t entityHeight, int directionIndex,
                std::shared_ptr<AnimationGroup>& animationGroup);
        static void resetAnimation(std::shared_ptr<AnimationGroup>& animationGroup);

};


#endif //VANQUISH_ANIMATION_MANAGER_H
