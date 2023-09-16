#include "../include/player.h"

Player::Player(std::shared_ptr<TextureManager>& textureManager)
    : GameEntity(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, sf::Vector2f(300, 150),
                 sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                 sf::Vector2u(0, 0), STARTING_PLAYER_HEALTH,
                 sf::Vector2u(
                         std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.5f,
                         std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.75f
                         ))
{
    sf::Texture& textureUp = *textureManager->getTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY);

    entitySprite = sf::Sprite(textureUp, entitySpriteSheetDimRect);
    entitySprite.scale(PLAYER_SCALE_FACTOR, PLAYER_SCALE_FACTOR);
    entitySprite.setPosition(getPosition());
}

void Player::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    sf::BlendMode blendMode = sf::BlendMode();
    blendMode.alphaDstFactor = sf::BlendMode::Factor::One;
    blendMode.colorSrcFactor = sf::BlendMode::Factor::One;
    //blendMode.colorDstFactor = sf::BlendMode::Factor::DstColor;

    states.blendMode = blendMode;

    renderTarget.draw(entitySprite, states);
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

    startMovement(gameState.getClock());
}

void Player::startMovement(GameClock& gameClock)
{
    bool isMovingDown = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Down, sf::Keyboard::S), EntityDirection::DOWN);
    bool isMovingUp = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Up, sf::Keyboard::W), EntityDirection::UP);
    bool isMovingLeft = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Left, sf::Keyboard::A), EntityDirection::LEFT);
    bool isMovingRight = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Right, sf::Keyboard::D), EntityDirection::RIGHT);

    if (isMovingDown || isMovingUp || isMovingLeft || isMovingRight)
    {
        performAnimation(gameClock, MAX_SPRITE_SHEET_FRAMES);
    }
}

bool Player::tryMoveDirection(GameClock& gameClock, std::pair<sf::Keyboard::Key, sf::Keyboard::Key> keyboardInputGroup,
        EntityDirection direction)
{
    if (sf::Keyboard::isKeyPressed(keyboardInputGroup.first) || sf::Keyboard::isKeyPressed(keyboardInputGroup.second))
    {
        setDirection(direction);
        updatePosition(gameClock);
        return true;
    }

    return false;
}

EntityType Player::getType()
{
    return EntityType::PLAYER;
}

sf::Time Player::getAnimationFrameDuration()
{
    return animationFrameDuration;
}
