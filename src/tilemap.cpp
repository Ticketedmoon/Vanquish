#include "../include/tilemap.h"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const uint32_t* tiles, uint32_t levelWidth, uint32_t levelHeight)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
    {
        return false;
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);

    // resize vertex array based on level size
    m_vertices.resize(levelWidth * levelHeight * TOTAL_VERTICES_IN_TILE);
    
    std::cout << "Tileset match tilesize: " << ((m_tileset.getSize().x % tileSize.x == 0) && (m_tileset.getSize().y % tileSize.y == 0)) << std::endl;

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < levelWidth; i++)
    {
        for (unsigned int j = 0; j < levelHeight; j++)
        {
            // get the current tile number
            int tilePos = i + j * levelWidth;
            int tileNumber = tiles[i + j * levelWidth];

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[tilePos * TOTAL_VERTICES_IN_TILE];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
            triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

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

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
};
