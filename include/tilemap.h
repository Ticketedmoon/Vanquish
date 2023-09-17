#pragma once

#ifndef VANQUISH_TILEMAP_H
#define VANQUISH_TILEMAP_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <fstream>
#include "json.hpp"
#include "game_component.h"
#include "game_entity.h"
#include "tile.h"
#include "player.h"

// Two triangles
static constexpr uint8_t TOTAL_VERTICES_IN_TILE = 6;

class TileMap : public GameComponent
{
    public:
        TileMap();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(GameState& gameState) override;
        bool load(const std::string& tileset, sf::Vector2u tileSize);

        static Tile& getTile(uint32_t x, uint32_t y);
        void updateTile(Tile& tileToUpdate, Tile::Type newTileType);

        void highlightTileForDebug(const std::shared_ptr<GameEntity>& entity, uint32_t levelWidth, sf::Color tileColour);

        static uint32_t getWorldWidthInTiles();
        static uint32_t getWorldHeightInTiles();

    private:
        static void createTilesForWorld(const std::vector<std::vector<uint8_t>>& worldData);
        static uint32_t getPositionForTile(uint32_t x, uint32_t y) ;

        void updateVertexPositionsForTile(const Tile& tile);
        void updateTileTexture(const Tile& tile);

    private:
        static inline std::vector<Tile> world;
        static inline uint32_t worldWidthInTiles;
        static inline uint32_t worldHeightInTiles;

        /* Note:
         * Rather than having a single vertex array for the world, we should break the world into chunks, lets say
         * 400x400 blocks.
         * Each chunk/block will get one vertex array.
         * Then you can simply draw only the chunks that are in a visible area of the current view.
         */
        sf::VertexArray m_vertices;

        sf::Texture m_tileset;

        std::unordered_map<std::shared_ptr<GameEntity>, uint32_t> previousEntityTilePosition;
        sf::VertexArray getTileVerticesInView(const sf::RenderTarget& target) const;
};

#endif //VANQUISH_TILEMAP_H