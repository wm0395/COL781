// window.hpp

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace util {
    class Window {
    private:
        int frameWidth = 20;
        int frameHeight = 20;
        int displayScale = 1;
        int spp = 1;
        SDL_Surface* framebuffer = nullptr;
        SDL_Window* window = nullptr;
        SDL_Surface* windowSurface = nullptr;
        bool quit = false;
        const char* outputFile = "out.png";

    public:
        bool initialize(const std::string &title, int width, int height, int spp = 1);
        void handleEvents();
        void saveFramebuffer();
        void cleanup();
        void draw(std::vector<std::vector<float>> &screen);
    };
}

#endif // WINDOW_HPP
