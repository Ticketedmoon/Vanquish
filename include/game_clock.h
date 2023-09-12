#ifndef VANQUISH_GAME_CLOCK_H
#define VANQUISH_GAME_CLOCK_H

#include <SFML/System/Clock.hpp>

class GameClock
{
    public:
        // World Clock
        float getWorldTimeSeconds();
        int getWorldTimeMs();

        // Delta Clock
        void restartDeltaClock();

        // Delta Time
        sf::Time getDeltaTime();

    private:
        sf::Clock m_worldClock;
        sf::Clock m_deltaClock;
        sf::Time m_deltaTime;
};


#endif //VANQUISH_GAME_CLOCK_H
