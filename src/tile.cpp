#include "tile.h"

Tile::Tile(sf::Vector2u coordinates, uint32_t position, uint32_t value)
    : coordinates(coordinates), position(position), value(value)
{
}

uint32_t Tile::getValue() const
{
    return value;
}

void Tile::setValue(uint32_t newValue)
{
    this->value = newValue;
}

uint32_t Tile::getPosition() const
{
    return position;
}

uint32_t Tile::getCoordinateX() const
{
    return coordinates.x;
}

uint32_t Tile::getCoordinateY() const
{
    return coordinates.y;
}
