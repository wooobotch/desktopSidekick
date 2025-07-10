#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rwops.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <SDL2/SDL_syswm.h>
#include <iostream>

#include "build/embedded/sprite_idle_front_png.h"
#include "build/embedded/sprite_idle_up_png.h"
#include "build/embedded/sprite_idle_down_png.h"
#include "build/embedded/sprite_idle_left_png.h"
#include "build/embedded/sprite_idle_right_png.h"

const int WIDTH = 150;
const int HEIGHT = 150;

enum class Direction {
    FRONT,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

void apply_window_shape(SDL_Window* sdlWindow, SDL_Surface* surface) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
    Display* display = wmInfo.info.x11.display;
    Window xwin = wmInfo.info.x11.window;

    // Crear bitmap de recorte
    Pixmap mask = XCreatePixmap(display, xwin, WIDTH, HEIGHT, 1);
    GC gc = XCreateGC(display, mask, 0, NULL);
    XSetForeground(display, gc, 0);
    XFillRectangle(display, mask, gc, 0, 0, WIDTH, HEIGHT);
    XSetForeground(display, gc, 1);

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32* pixels = (Uint32*)surface->pixels;
            Uint32 pixel = pixels[y * surface->w + x];
            Uint8 alpha = pixel >> 24;
            if (alpha > 128)
                XDrawPoint(display, mask, gc, x, y);
        }
    }

    XShapeCombineMask(display, xwin, ShapeBounding, 0, 0, mask, ShapeSet);

    // Input passthrough excepto zonas visibles
    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32* pixels = (Uint32*)surface->pixels;
            Uint32 pixel = pixels[y * surface->w + x];
            Uint8 alpha = pixel >> 24;
            if (alpha > 128) {
                XRectangle rect = { (short)x, (short)y, 1, 1 };
                XFixesUnionRegion(display, region, region, XFixesCreateRegion(display, &rect, 1));
            }
        }
    }
    XFixesSetWindowShapeRegion(display, xwin, ShapeInput, 0, 0, region);

    XFreeGC(display, gc);
    XFreePixmap(display, mask);
    XFlush(display);
}

int main(int argc, char* argv[]) {
    Direction currentDirection = Direction::FRONT;
    Direction lastDirection = Direction::FRONT;
    const unsigned char* image_data = nullptr;
    unsigned int image_size = 0;

    switch (currentDirection) {
        case Direction::FRONT:
            image_data = sprite_idle_front_png;
            image_size = sprite_idle_front_png_len;
            break;
        case Direction::LEFT:
            image_data = sprite_idle_left_png;
            image_size = sprite_idle_left_png_len;
            break;
        case Direction::RIGHT:
            image_data = sprite_idle_right_png;
            image_size = sprite_idle_right_png_len;
            break;
        case Direction::UP:
            image_data = sprite_idle_up_png;
            image_size = sprite_idle_up_png_len;
            break;
        case Direction::DOWN:
            image_data = sprite_idle_down_png;
            image_size = sprite_idle_down_png_len;
            break;
    }

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mascota",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_RWops* rw = SDL_RWFromConstMem(image_data, image_size);
    SDL_Surface* image = IMG_Load_RW(rw, 1);
    if (!image) {
        std::cerr << "No se pudo cargar imagen desde memoria\n";
        return 1;
    }

    apply_window_shape(window, image);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

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
            lastDirection = currentDirection;

            SDL_DestroyTexture(texture);

            const unsigned char* image_data = nullptr;
            unsigned int image_size = 0;

            switch (currentDirection) {
                case Direction::FRONT:
                    image_data = sprite_idle_front_png;
                    image_size = sprite_idle_front_png_len;
                    break;
                case Direction::LEFT:
                    image_data = sprite_idle_left_png;
                    image_size = sprite_idle_left_png_len;
                    break;
                case Direction::RIGHT:
                    image_data = sprite_idle_right_png;
                    image_size = sprite_idle_right_png_len;
                    break;
                case Direction::UP:
                    image_data = sprite_idle_up_png;
                    image_size = sprite_idle_up_png_len;
                    break;
                case Direction::DOWN:
                    image_data = sprite_idle_down_png;
                    image_size = sprite_idle_down_png_len;
                    break;
            }

            SDL_RWops* rw = SDL_RWFromConstMem(image_data, image_size);
            SDL_Surface* newSurface = IMG_Load_RW(rw, 1);
            texture = SDL_CreateTextureFromSurface(renderer, newSurface);

            apply_window_shape(window, newSurface);
            SDL_FreeSurface(newSurface);
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
