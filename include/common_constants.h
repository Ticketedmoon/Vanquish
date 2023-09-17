#pragma once

#ifndef VANQUISH_COMMON_CONSTANTS_H
#define VANQUISH_COMMON_CONSTANTS_H

#include <iostream>

static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;

static const std::string PLAYER_SPRITE_SHEET_A_WALK_KEY = "player_sprite_sheet_walk_animation_a";
static const std::string PLAYER_SPRITE_SHEET_A_HURT_KEY = "player_sprite_sheet_hurt_animation_a";
static const std::string PLAYER_SPRITE_SHEET_A_IDLE_KEY = "player_sprite_sheet_idle_animation_a";
static const std::string PLAYER_SPRITE_SHEET_A_DEATH_KEY = "player_sprite_sheet_death_animation_a";
static const std::string HUMAN_CHARACTER_SPRITE_SHEET_A_KEY = "human_character_sprite_sheet_a";

static const std::string PLAYER_SPRITE_SHEET_WALK_FILE_PATH = "resources/assets/movement/knight_movement.png";
static const std::string PLAYER_SPRITE_SHEET_HURT_FILE_PATH = "resources/assets/movement/knight_hurt.png";
static const std::string PLAYER_SPRITE_SHEET_IDLE_FILE_PATH = "resources/assets/movement/knight_idle.png";
static const std::string PLAYER_SPRITE_SHEET_DEATH_FILE_PATH = "resources/assets/movement/knight_death.png";

static const std::string HUMAN_CHARACTER_SPRITE_SHEET_A_FILE_PATH = "resources/assets/character_sprite_sheet_v2.png";

static constexpr uint32_t TILE_SIZE = 32;

#endif //VANQUISH_COMMON_CONSTANTS_H