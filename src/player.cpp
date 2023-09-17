#include "../include/player.h"

Player::Player(std::shared_ptr<TextureManager>& textureManager)
        :
        GameEntity(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, sf::Vector2f(300, 150),
                STARTING_PLAYER_HEALTH,
                sf::Vector2u(
                        std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.5f,
                        std::floor(PLAYER_SCALE_FACTOR * PLAYER_HEIGHT) * 0.75f
                ),
                {
                        {
                                PLAYER_SPRITE_SHEET_A_WALK_KEY,
                                std::make_shared<AnimationGroup>(7, sf::Vector2u(0, 0), sf::seconds(1.f / 12.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT))
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_IDLE_KEY,
                                std::make_shared<AnimationGroup>(3, sf::Vector2u(0, 0), sf::seconds(1.f / 6.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT))
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_HURT_KEY,
                                std::make_shared<AnimationGroup>(3, sf::Vector2u(0, 0), sf::seconds(1.f / 32.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT))
                        }
                }
        ),
        m_textureManager(textureManager)
{
    sf::Texture& texture = *m_textureManager->getTexture(PLAYER_SPRITE_SHEET_A_WALK_KEY);
    std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(PLAYER_SPRITE_SHEET_A_IDLE_KEY);

    entitySprite = sf::Sprite(texture, animationGroup->entitySpriteSheetDimRect);
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

    startMovement(gameState.getClock());
}

void Player::startMovement(GameClock& gameClock)
{
    bool isMovingDown = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Down, sf::Keyboard::S),
            EntityDirection::DOWN);
    bool isMovingUp = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Up, sf::Keyboard::W),
            EntityDirection::UP);
    bool isMovingLeft = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Left, sf::Keyboard::A),
            EntityDirection::LEFT);
    bool isMovingRight = tryMoveDirection(gameClock, std::make_pair(sf::Keyboard::Right, sf::Keyboard::D),
            EntityDirection::RIGHT);

    if (isMovingDown || isMovingUp || isMovingLeft || isMovingRight)
    {
        const std::string& animationKey = PLAYER_SPRITE_SHEET_A_WALK_KEY;
        std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(animationKey);
        sf::Texture& texture = *m_textureManager->getTexture(animationKey);
        entitySprite.setTexture(texture);
        entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);
        performAnimation(gameClock, animationKey);
        return;
    }
    else
    {
        // TODO small bug here, we need to update the animation to the idle animation with the current direction.
        //      this is due to the walk animation being performed but the old animation remaining static.
        //      ideally at the before this conditional logic, we keep all animations aligned with which 'direction'
        //      they are facing.

        // Idle animation
        const std::string& animationKey = PLAYER_SPRITE_SHEET_A_IDLE_KEY;
        std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(animationKey);
        sf::Texture& texture = *m_textureManager->getTexture(animationKey);
        entitySprite.setTexture(texture);
        entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);
        performAnimation(gameClock, animationKey);
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

void Player::takeDamage(GameClock& gameClock, uint16_t damage)
{
    int timeNowSeconds = static_cast<int>(gameClock.getWorldTimeSeconds());
    bool hasEnemyAttackedAfterTimeWindow = timeNowSeconds - lastPlayerWasAttackedSeconds >= 3;
    if (health > 0 && hasEnemyAttackedAfterTimeWindow)
    {
        const std::string animationId = PLAYER_SPRITE_SHEET_A_HURT_KEY;
        std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(PLAYER_SPRITE_SHEET_A_HURT_KEY);
        sf::Texture& texture = *m_textureManager->getTexture(animationId);
        entitySprite.setTexture(texture);
        entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);

        // update attributes
        GameEntity::takeDamage(gameClock, damage);
        lastPlayerWasAttackedSeconds = timeNowSeconds;
    }
}