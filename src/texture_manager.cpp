#include "texture_manager.h"

TextureManager::TextureManager() = default;

void TextureManager::addTexture(const std::string &id, const std::string &texturePath) {
    auto it = textureMap_.find(id);
    if (it != textureMap_.end()) {
        std::string msg = "Unable to add texture: '" + id + "' already exists";
        throw new std::runtime_error(msg);
    }

    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(texturePath)) {
        std::string msg = "Unable to open texture '" + texturePath + "'";
        throw new std::runtime_error(msg);
    }

    texture->setSmooth(true);

    textureMap_.emplace(id, texture);

    std::cout << "Texture with path: [" << texturePath << "] has been added\n";
}

std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string &id) {
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>>::iterator it = textureMap_.find(id);
    if (it == textureMap_.end()) {
        std::cout << "Unable to load texture: " << id << " doesn't exist";
        return nullptr;
    }

    // TODO REFACTOR
    return it->second;
}