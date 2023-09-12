#include "game_clock.h"

float GameClock::getWorldTimeSeconds()
{
    return m_worldClock.getElapsedTime().asSeconds();
}

int GameClock::getWorldTimeMs()
{
    return m_worldClock.getElapsedTime().asMilliseconds();
}

void GameClock::restartDeltaClock()
{
    this->m_deltaTime = m_deltaClock.restart();
}

sf::Time GameClock::getDeltaTime()
{
    return m_deltaTime;
}
