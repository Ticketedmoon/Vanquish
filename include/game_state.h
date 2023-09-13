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

        void reset()
        {
            if (m_state == GameState::State::GAME_OVER)
            {
                m_state = GameState::State::PLAYING;
            }
        }

    private:
        State m_state = State::PLAYING;
};

#endif //VANQUISH_GAME_STATE_H
