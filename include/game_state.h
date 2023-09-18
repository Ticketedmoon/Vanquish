#pragma once

#ifndef VANQUISH_GAME_STATE_H
#define VANQUISH_GAME_STATE_H

#include "game_clock.h"

class GameState
{
    public:

        enum class State
        {
            PLAYING,
            GAME_OVER,
            DEBUG
        };

        enum class PlayerState
        {
                IDLE,
                MOVING,
                ATTACKING
        };

        [[nodiscard]] State getState() const
        {
            return m_state;
        }

        [[nodiscard]] PlayerState getPlayerState() const
        {
            return m_playerState;
        }

        void updateGameState(const State state)
        {
            this->m_state = state;
        };

        void updatePlayerState(const PlayerState state)
        {
            this->m_playerState = state;
        };

        GameClock& getClock()
        {
            return gameClock;
        }

    private:
        State m_state = State::PLAYING;
        PlayerState m_playerState = PlayerState::IDLE;
        GameClock gameClock;
};

#endif //VANQUISH_GAME_STATE_H