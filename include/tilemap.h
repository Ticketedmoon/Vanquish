#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "game_entity.h"

// Two triangles
static constexpr uint8_t TOTAL_VERTICES_IN_TILE = 6;

class TileMap : GameComponent
{
    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(GameClock& gameClock) override;

        bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<std::vector<uint32_t>>& tiles);
        void highlightTileForDebug(const std::shared_ptr<GameEntity>& entity, uint32_t levelWidth, sf::Color tileColour);

    private:

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

        std::unordered_map<std::shared_ptr<GameEntity>, uint32_t> previousEntityTilePosition;
};
