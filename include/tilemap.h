#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdint>
#include <stdlib.h>
#include <iostream>

// Two tringles
const uint32_t TOTAL_VERTICES_IN_TILE = 6;

class TileMap : public sf::Drawable, public sf::Transformable
{
    public:

        bool load(const std::string& tileset, sf::Vector2u tileSize, const uint32_t* tiles, uint32_t levelWidth, uint32_t levelHeight);

    private:
        
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
};
