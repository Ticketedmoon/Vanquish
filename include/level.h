#pragma once

#ifndef VANQUISH_LEVEL_H
#define VANQUISH_LEVEL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>

#include "player.h"
#include "tilemap.h"
#include "enemy.h"
#include "tile.h"

static constexpr uint8_t TOTAL_PLAYERS = 1;
static constexpr uint8_t TOTAL_ENEMIES = 8;
static constexpr uint8_t TOTAL_ENTITIES_PER_SPRITE_SHEET_ROW = 4;
static constexpr uint8_t TOTAL_GAME_ENTITIES = TOTAL_PLAYERS + TOTAL_ENEMIES;

// TODO Rename to world.h
class Level : public GameComponent
{
    public:
        Level() = default;
        Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager);

        void update(GameState& gameState) override;
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

        void interactWithTile(sf::Time deltaTime, std::shared_ptr<Player>& player);
        void enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour);

        void initialiseGameEntities();

        static TileMap& getTileMap();

    private:
        static inline TileMap tileMap;

        std::shared_ptr<Player> m_player;
        std::vector<std::shared_ptr<GameEntity>> gameEntities;
        std::shared_ptr<TextureManager> m_textureManager;
};

#endif //VANQUISH_LEVEL_H