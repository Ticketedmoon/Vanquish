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
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                                        AnimationGroup::AnimationCompletionType::REPEAT_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_IDLE_KEY,
                                std::make_shared<AnimationGroup>(3, sf::Vector2u(0, 0), sf::seconds(1.f / 6.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                                        AnimationGroup::AnimationCompletionType::REPEAT_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_HURT_KEY,
                                std::make_shared<AnimationGroup>(3, sf::Vector2u(0, 0), sf::seconds(1.f / 4.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                                        AnimationGroup::AnimationCompletionType::RESET_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_DEATH_KEY,
                                std::make_shared<AnimationGroup>(3, sf::Vector2u(0, 0), sf::seconds(1.f / 6.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                                        AnimationGroup::AnimationCompletionType::COMPLETE_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
                        },
                        {
                                PLAYER_SPRITE_SHEET_A_ATTACK_KEY,
                                std::make_shared<AnimationGroup>(5, sf::Vector2u(0, 0), sf::seconds(1.f / 6.f),
                                        sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                                        AnimationGroup::AnimationCompletionType::COMPLETE_ANIMATION_AFTER_EXECUTING_ALL_FRAMES)
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

/* FIXME - Bug Note:
 *  - First damage to player doesn't trigger hurt animation, but subsequent damage does.
 *  - Idle animation needs to be aware which movement animation is happening, so there isn't a flicker when the player
 *    stops moving.
 */
void Player::update(GameState& gameState)
{
    GameClock& gameClock = gameState.getClock();

    // TODO: Refactor this block
    if (isDead())
    {
        if (deathTimeSeconds == 0)
        {
            deathTimeSeconds = gameClock.getWorldTimeSeconds();
        }

        if (gameClock.getWorldTimeSeconds() > (deathTimeSeconds + 2))
        {
            gameState.updateGameState(GameState::State::GAME_OVER);
        }

        startAnimationFromAnimationGroup(gameClock, PLAYER_SPRITE_SHEET_A_DEATH_KEY);
        return;
    }

    uint32_t tileUnderPlayerX = floor((getPosition().x + spritePositionOffset.x) / TILE_SIZE);
    uint32_t tileUnderPlayerY = floor((getPosition().y + spritePositionOffset.y) / TILE_SIZE);
    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);
    entitySprite.setPosition(getPosition());


    // TODO: Refactor this block
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        // reset animation
        std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(PLAYER_SPRITE_SHEET_A_ATTACK_KEY);
        AnimationManager::resetAnimation(animationGroup);
        swingTimeSeconds = gameClock.getWorldTimeSeconds() + 1;
    }

    // TODO: Refactor this block
    if (swingTimeSeconds > gameClock.getWorldTimeSeconds())
    {
        startAnimationFromAnimationGroup(gameClock, PLAYER_SPRITE_SHEET_A_ATTACK_KEY);

        // 3/4 through animation
        if (swingTimeSeconds - gameClock.getWorldTimeSeconds() < 0.7f)
        {
            gameState.updatePlayerState(GameState::PlayerState::ATTACK);
        }
        return;
    }

    startMovement(gameState);
}

void Player::startAnimation(GameClock& gameClock, const std::string& animationKeyA, const std::string& animationKeyB)
{
    if (painTimeSeconds > gameClock.getWorldTimeSeconds())
    {
        startAnimationFromAnimationGroup(gameClock, animationKeyA);
    }
    else
    {
        startAnimationFromAnimationGroup(gameClock, animationKeyB);
    }
}

// TODO small bug here, we need to update the animation to the idle animation with the current direction.
//      this is due to the walk animation being performed but the old animation remaining static.
//      ideally at the before this conditional logic, we keep all animations aligned with which 'direction'
//      they are facing.
void Player::startMovement(GameState& gameState)
{
    GameClock& gameClock = gameState.getClock();
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
        startAnimation(gameClock, PLAYER_SPRITE_SHEET_A_HURT_KEY, PLAYER_SPRITE_SHEET_A_WALK_KEY);
        gameState.updatePlayerState(GameState::PlayerState::MOVING);
        return;
    }

    startAnimation(gameClock, PLAYER_SPRITE_SHEET_A_HURT_KEY, PLAYER_SPRITE_SHEET_A_IDLE_KEY);
    gameState.updatePlayerState(GameState::PlayerState::IDLE);
}

void Player::startAnimationFromAnimationGroup(GameClock& gameClock, const std::string& animationKey)
{
    std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(animationKey);
    sf::Texture& texture = *m_textureManager->getTexture(animationKey);
    entitySprite.setTexture(texture);
    entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);
    performAnimationByKey(gameClock, animationKey);
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

void Player::applyDamage(GameClock& gameClock, uint16_t damage)
{
    if (health > 0)
    {
        GameEntity::applyDamage(gameClock, damage);
        painTimeSeconds = gameClock.getWorldTimeSeconds() + HURT_ANIMATION_TIME_OFFSET;
    }
}