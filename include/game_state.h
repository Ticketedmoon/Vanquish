#pragma once

#ifndef VANQUISH_GAME_STATE_H
#define VANQUISH_GAME_STATE_H

class GameState
{
    public:
        enum class State
        {
            PLAYING,
            GAME_OVER,
            DEBUG
        };

        [[nodiscard]] State getState() const
        {
            return m_state;
        }
        void updateState(const State state)
        {
            this->m_state = state;
        };

        GameClock& getClock()
        {
            return gameClock;
        }

    private:
        State m_state = State::PLAYING;
        GameClock gameClock;
};

#endif //VANQUISH_GAME_STATE_H
