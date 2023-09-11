#include "game_component.h"

void GameComponent::render(sf::RenderTarget& renderTarget, sf::RenderStates states, GameState gameState)
{
    renderTarget.draw(*this, states);
}