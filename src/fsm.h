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

struct AnimationLayer {
    std::vector<SpriteData> frames;
    int frame_duration_ms;
};

struct StateAnimation {
    AnimationLayer body;
    // Más adelante: AnimationLayer hat, eyes, etc.
};

inline std::map<State, StateAnimation> stateAnimations = {
    {State::IDLE_FRONT, {
        .body = {
            {
                {sprite_idle_front_0_png, sprite_idle_front_0_png_len},
                {sprite_idle_front_1_png, sprite_idle_front_1_png_len},
                {sprite_idle_front_2_png, sprite_idle_front_2_png_len}
            },
            400
        }
    }},
    {State::IDLE_LEFT, {
        .body = {
            {
                {sprite_idle_left_0_png, sprite_idle_left_0_png_len},
                {sprite_idle_left_1_png, sprite_idle_left_1_png_len},
                {sprite_idle_left_2_png, sprite_idle_left_2_png_len}
            },
            380
        }
    }},
    {State::IDLE_RIGHT, {
        .body = {
            {
                {sprite_idle_right_0_png, sprite_idle_right_0_png_len},
                {sprite_idle_right_1_png, sprite_idle_right_1_png_len},
                {sprite_idle_right_2_png, sprite_idle_right_2_png_len}
            },
            380
        }
    }},
    {State::IDLE_UP, {
        .body = {
            {
                {sprite_idle_up_0_png, sprite_idle_up_0_png_len},
                {sprite_idle_up_1_png, sprite_idle_up_1_png_len},
                {sprite_idle_up_2_png, sprite_idle_up_2_png_len}
            },
            420
        }
    }},
    {State::IDLE_DOWN, {
        .body = {
            {
                {sprite_idle_down_0_png, sprite_idle_down_0_png_len},
                {sprite_idle_down_1_png, sprite_idle_down_1_png_len},
                {sprite_idle_down_2_png, sprite_idle_down_2_png_len}
            },
            350
        }
    }}
};
