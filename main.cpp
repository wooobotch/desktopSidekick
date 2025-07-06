#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <SDL2/SDL_syswm.h>
#include <iostream>

const int WIDTH = 50;
const int HEIGHT = 50;

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
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mascota",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* image = IMG_Load("../assets/sprite.png");
    if (!image) {
        std::cerr << "Error al cargar imagen\n";
        return 1;
    }

    apply_window_shape(window, image);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
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
