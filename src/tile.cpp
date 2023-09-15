#include "tile.h"

Tile::Tile(uint32_t x, uint32_t y, uint32_t value)
{
    this->position = sf::Vector2u(x, y);
    this->value = value;
}

uint32_t Tile::getValue() const
{
    return value;
}

void Tile::setValue(uint32_t newValue)
{
    this->value = newValue;
}
