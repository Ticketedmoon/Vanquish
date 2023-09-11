#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>
#include <optional>

#include "player.h"
#include "tilemap.h"
#include "enemy.h"

class Level : public GameComponent
{
    public:
        Level() = default;
        Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager);

        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

        void interactWithNode(sf::Time& deltaTime);
        void enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour);

        void initialiseGameEntities();
        uint32_t getLevelWidth();
        uint32_t getLevelHeight();

    private:
        void loadLevel();
        void checkForPlayerMovement(sf::Time& deltaTime, EntityDirection dir);

        TileMap map;
        std::vector<std::vector<uint32_t>> world;
        std::shared_ptr<Player> m_player;
        std::vector<std::shared_ptr<GameEntity>> gameEntities;
        std::shared_ptr<TextureManager> m_textureManager;
};
