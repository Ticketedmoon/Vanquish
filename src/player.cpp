#include "../include/player.h"

Player::Player(TextureManager& textureManager)
    : GameEntity(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, sf::Vector2f(300, 150),
                 sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                 sf::Vector2f(0, 0)),
      playerSpritePositionOffsetX((std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.5f)),
      playerSpritePositionOffsetY((std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT)))
{
    sf::Texture& texture = *textureManager.getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY);
    entitySprite = sf::Sprite(texture, entityDimRect);
    entitySprite.scale(PLAYER_SCALE_FACTOR, PLAYER_SCALE_FACTOR);
}

void Player::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    uint32_t tileUnderPlayerX = floor((position.x+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t tileUnderPlayerY = floor((position.y+playerSpritePositionOffsetY)/PLAYER_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);

    entitySprite.setPosition(position);
    entitySprite.setTextureRect(entityDimRect);
}

void Player::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

sf::Vector2<uint32_t> Player::findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection direction)
{
    float nextPlayerX = position.x;
    float nextPlayerY = position.y;

    if (direction == EntityDirection::UP)
    {
        nextPlayerY += (velocity.y - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::DOWN)
    {
        nextPlayerY += (velocity.y + PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::LEFT)
    {
        nextPlayerX += (velocity.x - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::RIGHT) 
    {
        nextPlayerX += (velocity.x + PLAYER_SPEED) * deltaTime.asSeconds();
    }
    
    uint32_t nextTileX = std::floor((nextPlayerX+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t nextTileY = std::floor((nextPlayerY+playerSpritePositionOffsetY)/PLAYER_HEIGHT);
    return {nextTileX, nextTileY};
}

void Player::updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTop, uint32_t spriteSheetLeft)
{
    sf::Time currentWorldTime = worldClock.getElapsedTime();
    if (currentWorldTime - animationFrameStartTime >= animationFrameDuration)
    {
        entityDimRect.top = spriteSheetTop;
        entityDimRect.left = spriteSheetLeft;
        animationFrameStartTime = currentWorldTime;
    }
}

uint16_t Player::getHealth() const
{
    return this->health;
}

void Player::setHealth(uint16_t newHealth)
{
    this->health = newHealth;
}

bool Player::isDead() const
{
    return this->health == 0;
}

void Player::reset()
{
    GameEntity::reset();
    entityDimRect = sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    this->health = STARTING_PLAYER_HEALTH;
}
