#include "../include/tilemap.h"

TileMap::TileMap()
{
    std::vector<std::vector<uint8_t>> worldData = createTilesForWorld();
    TileMap::worldWidthInTiles = worldData.at(0).size();
    TileMap::worldHeightInTiles = worldData.size();

    if (!load("./resources/assets/basic_tilemap.png", sf::Vector2u(TILE_SIZE, TILE_SIZE)))
    {
        std::cout << "Failed to load tileset" << '\n';
        return;
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tile set texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void TileMap::update(GameState& gameState)
{
    // NOT IMPLEMENTED
}

std::vector<std::vector<uint8_t>> TileMap::createTilesForWorld()
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    std::vector<std::vector<uint8_t>> worldData = data["grid"];
    for (size_t i = 0; i < worldData.size(); i++)
    {
        size_t rowSize = worldData.at(i).size();
        for (size_t j = 0; j < rowSize; j++)
        {
            uint32_t tileValue = worldData.at(i).at(j);
            world.emplace_back(i, j, tileValue);
        }
    }
    return worldData;
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
    {
        return false;
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);

    // resize vertex array based on level size
    m_vertices.resize(worldWidthInTiles * worldHeightInTiles * TOTAL_VERTICES_IN_TILE);

    std::cout << "Vertex count: " << m_vertices.getVertexCount() << '\n';
    std::cout << "Tileset match tile size: " << ((m_tileset.getSize().x % tileSize.x == 0) && (m_tileset.getSize().y % tileSize.y == 0)) << '\n';
    std::cout << "Tile Map Width: " << worldWidthInTiles << '\n';
    std::cout << "Tile Map Height: " << worldHeightInTiles << '\n';

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < worldHeightInTiles; i++)
    {
        for (unsigned int j = 0; j < worldWidthInTiles; j++)
        {
            // get the current tile number
            int tilePos = j + (i*worldWidthInTiles);
            Tile tile = world.at(tilePos);

            // find its position in the tileset texture
            int tu = tile.getValue() % (m_tileset.getSize().x / tileSize.x);
            int tv = tile.getValue() / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[tilePos * TOTAL_VERTICES_IN_TILE];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(j * tileSize.x, i * tileSize.y);
            triangles[1].position = sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);
            triangles[2].position = sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);

            triangles[3].position = sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);
            triangles[4].position = sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);
            triangles[5].position = sf::Vector2f((j + 1) * tileSize.x, (i + 1) * tileSize.y);

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

            triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        }
    }
    
    return true;
}

Tile& TileMap::getTile(uint32_t x, uint32_t y)
{
    uint32_t row = getWorldWidthInTiles() * y;
    uint32_t positionForTile = row + x;
    return world.at(positionForTile);
}

void TileMap::updateTile(uint32_t x, uint32_t y, uint32_t value)
{
    uint32_t row = getWorldWidthInTiles() * y;
    uint32_t positionForTile = row + x;
    world.at(positionForTile).setValue(value);
}

void TileMap::interactWithTile(sf::Time deltaTime, std::shared_ptr<Player>& player)
{
    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = player->findNextTileDirection(deltaTime);
    Tile& tileFacingPlayer = getTile(nextPlayerFacingTile.x, nextPlayerFacingTile.y);

    if (tileFacingPlayer.getValue() == 2 || tileFacingPlayer.getValue() == 3)
    {
        int tilePos = nextPlayerFacingTile.y + (nextPlayerFacingTile.x * worldWidthInTiles);
        std::cout << "tile interact: " << nextPlayerFacingTile.x << ", " << nextPlayerFacingTile.y << '\n';

        Tile tile = world.at(tilePos);
        tileFacingPlayer.setValue(0);

        // Note: This is working, in that the map value is being updated and listened to by the game world.
        //       The issue is that the texture remains in the world, and instead should be wiped.

        // find its position in the tileset texture
        int tu = tile.getValue() % (m_tileset.getSize().x / TILE_SIZE);
        int tv = tile.getValue() / (m_tileset.getSize().x / TILE_SIZE);

        // get a pointer to the triangles' vertices of the current tile
        sf::Vertex* triangles = &m_vertices[tilePos * TOTAL_VERTICES_IN_TILE];

        // define the 6 corners of the two triangles
        triangles[0].position = sf::Vector2f(nextPlayerFacingTile.y * TILE_SIZE, nextPlayerFacingTile.x * TILE_SIZE);
        triangles[1].position = sf::Vector2f((nextPlayerFacingTile.y + 1) * TILE_SIZE, nextPlayerFacingTile.x * TILE_SIZE);
        triangles[2].position = sf::Vector2f(nextPlayerFacingTile.y * TILE_SIZE, (nextPlayerFacingTile.x + 1) * TILE_SIZE);

        triangles[3].position = sf::Vector2f(nextPlayerFacingTile.y * TILE_SIZE, (nextPlayerFacingTile.x + 1) * TILE_SIZE);
        triangles[4].position = sf::Vector2f((nextPlayerFacingTile.y + 1) * TILE_SIZE, nextPlayerFacingTile.x * TILE_SIZE);
        triangles[5].position = sf::Vector2f((nextPlayerFacingTile.y + 1) * TILE_SIZE, (nextPlayerFacingTile.x + 1) * TILE_SIZE);

        // define the 6 matching texture coordinates
        triangles[0].texCoords = sf::Vector2f(tu * TILE_SIZE, tv * TILE_SIZE);
        triangles[1].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
        triangles[2].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);

        triangles[3].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);
        triangles[4].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
        triangles[5].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, (tv + 1) * TILE_SIZE);
    }
}

uint32_t TileMap::getWorldWidthInTiles()
{
    return worldWidthInTiles;
}

uint32_t TileMap::getWorldHeightInTiles()
{
    return worldHeightInTiles;
}

// Debug feature
// TODO put execution in separate thread?
void TileMap::highlightTileForDebug(const std::shared_ptr<GameEntity>& entity, uint32_t levelWidth, sf::Color tileColour)
{
    uint32_t currTilePos = entity->tilePosition.x + (entity->tilePosition.y * levelWidth);

    if (previousEntityTilePosition.contains(entity) && previousEntityTilePosition.at(entity) != currTilePos)
    {
        for (uint8_t i = 0; i < TOTAL_VERTICES_IN_TILE; i++)
        {
            m_vertices[previousEntityTilePosition.at(entity) * TOTAL_VERTICES_IN_TILE + i].color = sf::Color::White;
        }
    }

    for (uint8_t i = 0; i < TOTAL_VERTICES_IN_TILE; i++)
    {
        m_vertices[currTilePos * TOTAL_VERTICES_IN_TILE + i].color = tileColour;
    }

    previousEntityTilePosition[entity] = currTilePos;
}