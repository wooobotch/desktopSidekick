#pragma once

#include <SDL2/SDL.h>
#include "sprites.h"
#include <map>
#include <string>
#include <vector>

enum class State {
    IDLE_FRONT,
    IDLE_LEFT,
    IDLE_RIGHT,
    IDLE_UP,
    IDLE_DOWN,
    // Podés extender con:
    // WALK_LEFT, SLEEP, REACT_HAPPY, etc.
};

struct Animation {
    std::vector<SpriteData> frames;
    int frame_duration_ms;
};

inline std::map<State, Animation> spriteAnimationMap = {
    {State::IDLE_FRONT, {
        std::vector<SpriteData>{
            {sprite_idle_front_0_png, sprite_idle_front_0_png_len},
            {sprite_idle_front_1_png, sprite_idle_front_1_png_len},
            {sprite_idle_front_2_png, sprite_idle_front_2_png_len}
        },
        200
    }},
    {State::IDLE_LEFT, {
        std::vector<SpriteData>{
            {sprite_idle_left_0_png, sprite_idle_left_0_png_len},
            {sprite_idle_left_1_png, sprite_idle_left_1_png_len},
            {sprite_idle_left_2_png, sprite_idle_left_2_png_len}
        },
        200
    }},
    {State::IDLE_RIGHT, {
        std::vector<SpriteData>{
            {sprite_idle_right_0_png, sprite_idle_right_0_png_len},
            {sprite_idle_right_1_png, sprite_idle_right_1_png_len},
            {sprite_idle_right_2_png, sprite_idle_right_2_png_len}
        },
        200
    }},
    {State::IDLE_UP, {
        std::vector<SpriteData>{
            {sprite_idle_up_0_png, sprite_idle_up_0_png_len},
            {sprite_idle_up_1_png, sprite_idle_up_1_png_len},
            {sprite_idle_up_2_png, sprite_idle_up_2_png_len}
        },
        200
    }},
    {State::IDLE_DOWN, {
        std::vector<SpriteData>{
            {sprite_idle_down_0_png, sprite_idle_down_0_png_len},
            {sprite_idle_down_1_png, sprite_idle_down_1_png_len},
            {sprite_idle_down_2_png, sprite_idle_down_2_png_len}
        },
        200
    }}
};
