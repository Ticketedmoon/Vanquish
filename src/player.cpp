#include "../include/player.h"

Player::Player(std::shared_ptr<TextureManager>& textureManager)
    : GameEntity(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, sf::Vector2f(300, 150),
                 sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                 sf::Vector2u(0, 0), STARTING_PLAYER_HEALTH,
                 sf::Vector2u(
                         std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.5f,
                         std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT)))
{
    sf::Texture& textureUp = *textureManager->getTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY);

    entitySprite = sf::Sprite(textureUp, entitySpriteSheetDimRect);
    entitySprite.scale(PLAYER_SCALE_FACTOR, PLAYER_SCALE_FACTOR);
    entitySprite.setPosition(getPosition());
}

void Player::update(GameState& gameState)
{
    if (isDead())
    {
        gameState.updateState(GameState::State::GAME_OVER);
        return;
    }

    uint32_t tileUnderPlayerX = floor((getPosition().x + spritePositionOffset.x) / TILE_SIZE);
    uint32_t tileUnderPlayerY = floor((getPosition().y + spritePositionOffset.y) / TILE_SIZE);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);
    entitySprite.setPosition(getPosition());
    entitySprite.setTextureRect(entitySpriteSheetDimRect);

    GameClock& gameClock = gameState.getClock();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        startMovement(gameClock, EntityDirection::UP, 3);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        startMovement(gameClock, EntityDirection::DOWN, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        startMovement(gameClock, EntityDirection::LEFT, 1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        startMovement(gameClock, EntityDirection::RIGHT, 2);
    }
}

void Player::startMovement(GameClock& gameClock, EntityDirection direction, size_t spriteSheetTopOffset)
{
    setDirection(direction);
    updatePosition(gameClock);

    uint32_t spriteSheetTop = PLAYER_HEIGHT * spriteSheetTopOffset;
    uint32_t spriteSheetLeft = entitySpriteSheetDimRect.left == (PLAYER_WIDTH * MAX_SPRITE_SHEET_FRAMES)
            ? 0
            : (entitySpriteSheetDimRect.left + PLAYER_WIDTH);

    updateAnimation(gameClock.getDeltaTime(), spriteSheetTop, spriteSheetLeft);
}

void Player::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

EntityType Player::getType()
{
    return EntityType::PLAYER;
}

void Player::updateAnimation(sf::Time deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft)
{
    // Note: I don't think it's smart to use deltaTime in this calculation as it will vary across machines.
    animationFrameStartTime += deltaTime;
    if (animationFrameStartTime >= animationFrameDuration)
    {
        entitySpriteSheetDimRect.top = spriteSheetTop;
        entitySpriteSheetDimRect.left = spriteSheetLeft;
        animationFrameStartTime = sf::Time::Zero;
    }
}

sf::Time Player::getAnimationFrameDuration()
{
    return animationFrameDuration;
}
