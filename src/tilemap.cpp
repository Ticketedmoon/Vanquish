#include "../include/tilemap.h"

TileMap::TileMap()
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    std::vector<std::vector<uint8_t>> worldData = data["grid"];

    TileMap::worldWidthInTiles = worldData.at(0).size();
    TileMap::worldHeightInTiles = worldData.size();
    createTilesForWorld(worldData);

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

void TileMap::createTilesForWorld(const std::vector<std::vector<uint8_t>>& worldData)
{
    for (size_t y = 0; y < worldData.size(); y++)
    {
        size_t rowSize = worldData.at(y).size();
        for (size_t x = 0; x < rowSize; x++)
        {
            uint32_t tileValue = worldData.at(y).at(x);
            uint32_t tilePosition = getPositionForTile(x, y);
            world.emplace_back(sf::Vector2u(x, y), tilePosition, tileValue);
        }
    }
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

    std::cout << "Vertex count: " << m_vertices.getVertexCount() << '\n'
              << "Tileset match tile size: " << ((m_tileset.getSize().x % tileSize.x == 0) && (m_tileset.getSize().y % tileSize.y == 0)) << '\n'
              << "Tile Map Width: " << worldWidthInTiles << '\n'
              << "Tile Map Height: " << worldHeightInTiles << '\n';

    // populate the vertex array, with two triangles per tile
    for (unsigned int y = 0; y < worldHeightInTiles; y++)
    {
        for (unsigned int x = 0; x < worldWidthInTiles; x++)
        {
            Tile& tile = getTile(x, y);

            sf::Vertex* triangles = &m_vertices[tile.getPosition() * TOTAL_VERTICES_IN_TILE];

            triangles[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
            triangles[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
            triangles[2].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

            triangles[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
            triangles[4].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
            triangles[5].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);

            updateTileTexture(tile);
        }
    }
    
    return true;
}

Tile& TileMap::getTile(uint32_t x, uint32_t y)
{
    uint32_t positionForTile = getPositionForTile(x, y);
    return world.at(positionForTile);
}

uint32_t TileMap::getPositionForTile(uint32_t x, uint32_t y)
{
    uint32_t row = getWorldWidthInTiles() * y;
    uint32_t positionForTile = row + x;
    return positionForTile;
}

void TileMap::interactWithTile(sf::Time deltaTime, std::shared_ptr<Player>& player, Tile::Type tileType)
{
    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTilePosition = player->findNextTileDirection(deltaTime);
    Tile& nextPlayerFacingTile = getTile(nextPlayerFacingTilePosition.x, nextPlayerFacingTilePosition.y);

    if (nextPlayerFacingTile.getValue() == 2 || nextPlayerFacingTile.getValue() == 3)
    {
        std::cout << "tile interact: " << nextPlayerFacingTile.getCoordinateX() << ", " << nextPlayerFacingTile.getCoordinateY() << '\n';

        int tileTypeValue = static_cast<int>(tileType);
        nextPlayerFacingTile.setValue(tileTypeValue);

        updateTileTexture(nextPlayerFacingTile);
    }
}

void TileMap::updateTileTexture(const Tile& tile)
{
    float tu = tile.getValue() % (m_tileset.getSize().x / TILE_SIZE);
    float tv = tile.getValue() / (m_tileset.getSize().x / TILE_SIZE);

    sf::Vertex* triangles = &m_vertices[tile.getPosition() * TOTAL_VERTICES_IN_TILE];
    float tuPositionStart = tu * TILE_SIZE;
    float tuPositionEnd = (tu + 1) * TILE_SIZE;

    float tvPositionStart = tv * TILE_SIZE;
    float tvPositionEnd = (tv + 1) * TILE_SIZE;

    triangles[0].texCoords = sf::Vector2f(tuPositionStart, tvPositionStart);
    triangles[1].texCoords = sf::Vector2f(tuPositionEnd, tvPositionStart);
    triangles[2].texCoords = sf::Vector2f(tuPositionStart, tvPositionEnd);

    triangles[3].texCoords = sf::Vector2f(tuPositionStart, tvPositionEnd);
    triangles[4].texCoords = sf::Vector2f(tuPositionEnd, tvPositionStart);
    triangles[5].texCoords = sf::Vector2f(tuPositionEnd, tvPositionEnd);
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