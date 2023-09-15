#pragma once

#ifndef VANQUISH_TILE_H
#define VANQUISH_TILE_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

class Tile
{
    public:
        Tile(uint32_t x, uint32_t y, uint32_t value);

    public:
        [[nodiscard]] uint32_t getValue() const;
        void setValue(uint32_t newValue);

    private:
        sf::Vector2u position;
        uint32_t value;
};

#endif //VANQUISH_TILE_H
