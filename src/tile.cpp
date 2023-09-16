#include "tile.h"

Tile::Tile(sf::Vector2u coordinates, uint32_t position, Type tileType)
    : coordinates(coordinates), position(position), type(tileType)
{
}

Tile::Type Tile::getType() const
{
    return type;
}

void Tile::setType(Tile::Type newTileType)
{
    this->type = newTileType;
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
