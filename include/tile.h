#pragma once

#ifndef VANQUISH_TILE_H
#define VANQUISH_TILE_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

class Tile
{
    public:
        enum class Type
        {
                GRASS = 0,
                WATER = 1,
                TREE = 2,
                STONE = 3
        };

        Tile(sf::Vector2u coordinates, uint32_t position, Type tileType);

    public:
        [[nodiscard]] uint32_t getCoordinateX() const;
        [[nodiscard]] uint32_t getCoordinateY() const;
        [[nodiscard]] uint32_t getPosition() const;
        [[nodiscard]] Type getType() const;
        void setType(Type newTileType);

    private:
        sf::Vector2u coordinates;
        uint32_t position;
        Type type;
};

#endif //VANQUISH_TILE_H
