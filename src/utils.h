#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <iostream>
#include "sprites.h"
#include <map>

const int WIDTH = 150;
const int HEIGHT = 150;

inline void apply_window_shape(SDL_Window* sdlWindow, SDL_Surface* surface) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
    Display* display = wmInfo.info.x11.display;
    Window xwin = wmInfo.info.x11.window;

    Pixmap mask = XCreatePixmap(display, xwin, surface->w, surface->h, 1);
    GC gc = XCreateGC(display, mask, 0, NULL);
    XSetForeground(display, gc, 0);
    XFillRectangle(display, mask, gc, 0, 0, surface->w, surface->h);
    XSetForeground(display, gc, 1);

    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32 pixel = pixels[y * surface->w + x];
            Uint8 alpha = pixel >> 24;
            if (alpha > 128) {
                XDrawPoint(display, mask, gc, x, y);
            }
        }
    }

    XShapeCombineMask(display, xwin, ShapeBounding, 0, 0, mask, ShapeSet);

    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32 pixel = pixels[y * surface->w + x];
            Uint8 alpha = pixel >> 24;
            if (alpha > 128) {
                XRectangle rect = { (short)x, (short)y, 1, 1 };
                XserverRegion r = XFixesCreateRegion(display, &rect, 1);
                XFixesUnionRegion(display, region, region, r);
                XFixesDestroyRegion(display, r);
            }
        }
    }
    XFixesSetWindowShapeRegion(display, xwin, ShapeInput, 0, 0, region);

    XFreeGC(display, gc);
    XFreePixmap(display, mask);
    XFixesDestroyRegion(display, region);
    XFlush(display);
}

inline SDL_Texture* loadTexture(SDL_Renderer* renderer, const SpriteData& sprite) {
    SDL_RWops* rw = SDL_RWFromConstMem(sprite.data, sprite.size);
    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (!surface) {
        std::cerr << "Error cargando superficie desde sprite embebido\n";
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

inline SDL_Texture* loadTextureAndApplyMask(SDL_Renderer* renderer, SDL_Window* window, const SpriteData& sprite) {
    SDL_RWops* rw = SDL_RWFromConstMem(sprite.data, sprite.size);
    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (!surface) {
        std::cerr << "Error cargando superficie desde sprite embebido\n";
        return nullptr;
    }

    apply_window_shape(window, surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
