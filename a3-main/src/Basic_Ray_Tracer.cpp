#include "Basic_Ray_Tracer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool Ray_Tracer::initialize(const std::string &title, int width, int height, int spp){
    bool success = true;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            success = false;
        }
        // } else {
        //     this->frameHeight = height;
        //     this->frameWidth = width;
        //     this->displayScale = 1;
        //     this->spp = spp;
        //     int screenWidth = frameWidth * displayScale;
        //     int screenHeight = frameHeight * displayScale;
        //     window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        //     if (window == NULL) {
        //         printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        //         success = false;
        //     } else {
        //         this->windowSurface = SDL_GetWindowSurface(window);
        //         this->framebuffer = SDL_CreateRGBSurface(0, this->frameWidth, this->frameHeight, 32, 0, 0, 0, 0);
        //         this->pointBuffer = std::vector<std::vector<Attribs>>(this->frameWidth, std::vector<Attribs>(this->frameHeight, Attribs()));
        //     }
        // }
        return success;
}