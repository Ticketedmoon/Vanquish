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

        Tile(sf::Vector2u coordinates, uint32_t position, uint32_t value);

    public:
        [[nodiscard]] uint32_t getCoordinateX() const;
        [[nodiscard]] uint32_t getCoordinateY() const;
        [[nodiscard]] uint32_t getPosition() const;
        [[nodiscard]] uint32_t getValue() const;
        void setValue(uint32_t newValue);

    private:
        sf::Vector2u coordinates;
        uint32_t position;
        uint32_t value;
};

#endif //VANQUISH_TILE_H
