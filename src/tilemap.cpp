#include "../include/tilemap.h"
#include <SFML/Graphics/Color.hpp>

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<std::vector<uint32_t>>& world)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
    {
        return false;
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);

    // extract levelWidth and levelHeight
    uint32_t levelWidth = world.at(0).size();
    uint32_t levelHeight = world.size();

    // resize vertex array based on level size
    m_vertices.resize(levelWidth * levelHeight * TOTAL_VERTICES_IN_TILE);

    std::cout << "vertice size: " << m_vertices.getVertexCount() << std::endl;
    std::cout << "Tileset match tile size: " << ((m_tileset.getSize().x % tileSize.x == 0) && (m_tileset.getSize().y % tileSize.y == 0)) << std::endl;
    std::cout << "Tile Map Height: " << world.size() << std::endl;
    std::cout << "Tile Map Width: " << world.at(0).size() << std::endl;

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < levelHeight; i++)
    {
        for (unsigned int j = 0; j < levelWidth; j++)
        {
            // get the current tile number
            int tilePos = j + (i*levelWidth);
            int tileNumber = world.at(i).at(j);

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

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
};

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
