#pragma once

#include <SDL2/SDL.h>
#include "sprites.h"
#include <map>
#include <string>

enum class State {
    IDLE_FRONT,
    IDLE_LEFT,
    IDLE_RIGHT,
    IDLE_UP,
    IDLE_DOWN,
    // Podés extender con:
    // WALK_LEFT, SLEEP, REACT_HAPPY, etc.
};

inline std::map<State, SpriteData> spriteStateMap = {
    {State::IDLE_FRONT, {sprite_idle_front_png, sprite_idle_front_png_len}},
    {State::IDLE_LEFT,  {sprite_idle_left_png,  sprite_idle_left_png_len}},
    {State::IDLE_RIGHT, {sprite_idle_right_png, sprite_idle_right_png_len}},
    {State::IDLE_UP,    {sprite_idle_up_png,    sprite_idle_up_png_len}},
    {State::IDLE_DOWN,  {sprite_idle_down_png,  sprite_idle_down_png_len}},
};
