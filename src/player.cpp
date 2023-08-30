#include "../include/player.h"

Player::Player(TextureManager& textureManager)
    : GameEntity(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, sf::Vector2f(300, 150),
                 sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                 sf::Vector2f(0, 0)),
      playerSpritePositionOffsetX((std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.5f)),
      playerSpritePositionOffsetY((std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT)))
{
    sf::Texture& texture = *textureManager.getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY);
    entitySprite = sf::Sprite(texture, entitySpriteSheetDimRect);
    entitySprite.scale(PLAYER_SCALE_FACTOR, PLAYER_SCALE_FACTOR);
}

void Player::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    uint32_t tileUnderPlayerX = floor((getPosition().x+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t tileUnderPlayerY = floor((getPosition().y+playerSpritePositionOffsetY)/PLAYER_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);
    entitySprite.setPosition(getPosition());
    entitySprite.setTextureRect(entitySpriteSheetDimRect);
}

void Player::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

sf::Vector2<uint32_t> Player::findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection direction)
{
    sf::Vector2f nextPlayerPos = getPosition();

    if (direction == EntityDirection::UP)
    {
        nextPlayerPos.y += (velocity.y - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::DOWN)
    {
        nextPlayerPos.y += (velocity.y + PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::LEFT)
    {
        nextPlayerPos.x += (velocity.x - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::RIGHT) 
    {
        nextPlayerPos.x += (velocity.x + PLAYER_SPEED) * deltaTime.asSeconds();
    }

    uint32_t nextTileX = std::floor((nextPlayerPos.x + playerSpritePositionOffsetX) / PLAYER_WIDTH);
    uint32_t nextTileY = std::floor((nextPlayerPos.y + playerSpritePositionOffsetY) / PLAYER_HEIGHT);
    return {nextTileX, nextTileY};
}

void Player::updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTop, uint32_t spriteSheetLeft)
{
    sf::Time currentWorldTime = worldClock.getElapsedTime();
    if (currentWorldTime - animationFrameStartTime >= animationFrameDuration)
    {
        entitySpriteSheetDimRect.top = spriteSheetTop;
        entitySpriteSheetDimRect.left = spriteSheetLeft;
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
    entitySpriteSheetDimRect = sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    this->health = STARTING_PLAYER_HEALTH;
}

EntityType Player::getType()
{
    return EntityType::PLAYER;
}
