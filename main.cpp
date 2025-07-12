#include "src/utils.h"

std::map<Direction, SpriteData> spriteAssets = {
    {Direction::FRONT, {sprite_idle_front_png, sprite_idle_front_png_len}},
    {Direction::LEFT,  {sprite_idle_left_png,  sprite_idle_left_png_len}},
    {Direction::RIGHT, {sprite_idle_right_png, sprite_idle_right_png_len}},
    {Direction::UP,    {sprite_idle_up_png,    sprite_idle_up_png_len}},
    {Direction::DOWN,  {sprite_idle_down_png,  sprite_idle_down_png_len}}
};

int main(int argc, char* argv[]) {
    Direction currentDirection = Direction::FRONT;
    Direction lastDirection = Direction::FRONT;

    const Uint32 min_delta_time_ms = 150;
    Uint32 last_change_time = SDL_GetTicks();


    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mascota",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* texture = loadTextureAndApplyMask(renderer, window, spriteAssets[currentDirection]);
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
                        currentDirection = Direction::LEFT;
                        break;
                    case SDLK_RIGHT:
                        currentDirection = Direction::RIGHT;
                        break;
                   case SDLK_UP:
                        currentDirection = Direction::UP;
                        break;
                    case SDLK_DOWN:
                        currentDirection = Direction::DOWN;
                        break;
                    case SDLK_SPACE:
                        currentDirection = Direction::FRONT;
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            }
        }

        if (currentDirection != lastDirection) {
            Uint32 now = SDL_GetTicks();
            if (now - last_change_time >= min_delta_time_ms) {
                last_change_time = now;
                lastDirection = currentDirection;
                SDL_DestroyTexture(texture);
                texture = loadTextureAndApplyMask(renderer, window, spriteAssets[currentDirection]);
            }
        }


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
