#include "utils.h"
#include "fsm.h"
#include <iostream>

std::map<State, std::vector<SDL_Texture*>> loadedAnimations;

void loadAllAnimations(SDL_Renderer* renderer, SDL_Window* window) {
    for (const auto& [state, animation] : spriteAnimationMap) {
        std::vector<SDL_Texture*> textures;
        for (const SpriteData& frame : animation.frames) {
            SDL_Texture* tex;
            if (&frame == &animation.frames[0]) {
                tex = loadTextureAndApplyMask(renderer, window, frame);
            } else {
                tex = loadTexture(renderer, frame);
            }
            if (!tex) {
                std::cerr << "Error al cargar frame para estado " << static_cast<int>(state) << "\n";
                continue;
            }
            textures.push_back(tex);
        }
        loadedAnimations[state] = textures;
    }
}

void unloadAllAnimations() {
    for (auto& [state, textures] : loadedAnimations) {
        for (SDL_Texture* tex : textures) {
            SDL_DestroyTexture(tex);
        }
    }
    loadedAnimations.clear();
}
