#include "utils.h"
#include "fsm.h"
#include "sprites.h"

int main(int argc, char* argv[]) {
    State currentState = State::IDLE_FRONT;
    State lastState = State::IDLE_FRONT;

    const Uint32 min_delta_time_ms = 150;
    Uint32 last_change_time = SDL_GetTicks();

    int frameIndex = 0;
    Uint32 last_frame_time = SDL_GetTicks();
    Uint32 frame_duration_ms = spriteAnimationMap[currentState].frame_duration_ms;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mascota",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    loadAllAnimations(renderer, window);

    SDL_Texture* texture = loadedAnimations[currentState][frameIndex];

    if (!texture) return 1;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentState = State::IDLE_LEFT;
                        break;
                    case SDLK_RIGHT:
                        currentState = State::IDLE_RIGHT;
                        break;
                   case SDLK_UP:
                        currentState = State::IDLE_UP;
                        break;
                    case SDLK_DOWN:
                        currentState = State::IDLE_DOWN;
                        break;
                    case SDLK_SPACE:
                        currentState = State::IDLE_FRONT;
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            }
        }

        if (currentState != lastState) {
            Uint32 now = SDL_GetTicks();
            frameIndex = 0;
            if (now - last_change_time >= min_delta_time_ms) {
                last_change_time = now;
                lastState = currentState;
                SDL_DestroyTexture(texture);
                frame_duration_ms = spriteAnimationMap[currentState].frame_duration_ms;
                texture = loadedAnimations[currentState][frameIndex];

            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last_frame_time >= frame_duration_ms) {
            last_frame_time = now;
            const std::vector<SpriteData>& frames = spriteAnimationMap[currentState].frames;
            frameIndex = (frameIndex + 1) % frames.size();
            texture = loadedAnimations[currentState][frameIndex];
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    unloadAllAnimations();
    SDL_Quit();
    return 0;
}
