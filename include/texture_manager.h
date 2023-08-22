#pragma once

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <filesystem>

class TextureManager
{
    public:
        TextureManager();
        TextureManager(const std::string &id, const std::string& pathToTextureFile);

        void addTexture(const std::string& id, const std::string& texturePath);
        std::shared_ptr<sf::Texture> getTexture(const std::string &id);

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureMap_;
};