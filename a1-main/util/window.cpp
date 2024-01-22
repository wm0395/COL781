

#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <string>
// #define vector std::vector

#include "window.hpp"

namespace util{
    // class window{

        /* Framebuffer-related variables.
        We create a 20x20 image and display it at 20x size so you can clearly
        see the pixels! Set displayScale to 1 to see it at the correct size.
        Then you can increase frameWidth and frameHeight as desired. */

        // private:
        //     int frameWidth = 20;
        //     int frameHeight = 20;

        //     int displayScale = 1;

        //     int spp = 1;

        //     SDL_Surface* framebuffer = NULL;

        //     /* SDL parameters */

        //     SDL_Window* window = NULL;
        //     SDL_Surface *windowSurface = NULL;
        //     bool quit = false;

        //     /* Output file */

        //     const char* outputFile = "out.png";

        // /* Function prototypes */
        // public:
        //     bool initialize();
        //     void handleEvents();
        //     void saveFramebuffer();
        //     void cleanup();
        //     void draw();

        //     /* Main function. Pay attention to this. */

            void Window::draw(std::vector<std::vector<float>> &screen) {
                while (!quit) {
                    // Event handling
                    handleEvents();

                    /* Set pixel data.
                    CHANGE THIS TO YOUR OWN CODE! */

                    Uint32 *pixels = (Uint32*)framebuffer->pixels;
                    SDL_PixelFormat *format = framebuffer->format;
                    for (int i = 0; i < frameWidth; i++) {
                        for (int j = 0; j < frameHeight; j++) {
                            float l = 255*j/frameHeight;
                            pixels[i + frameWidth*j] = SDL_MapRGBA(format, l, l, l, 255*screen[j][i]);;
                        }
                    }

                    // Update screen to apply the changes
                    SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
                    SDL_UpdateWindowSurface(window);
                }
                // Save image
                saveFramebuffer();
                cleanup();
            }

            /* Everything below here is library-specific boilerplate code.
            You can ignore it for now. */

            // Initialising SDL2
            bool Window::initialize(const std::string &title, int width, int height, int spp=1) {
                bool success = true;
                if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                    printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
                    success = false;
                } else {
                    this->frameHeight = height;
                    this->frameWidth = width;
                    this->displayScale = 1;
                    int screenWidth = frameWidth * displayScale;
                    int screenHeight = frameHeight * displayScale;
                    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
                    if (window == NULL) {
                        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
                        success = false;
                    } else {
                        windowSurface = SDL_GetWindowSurface(window);
                        framebuffer = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);
                    }
                }
                return success;
            }

            // Handle window exit 
            void Window::handleEvents() {
                SDL_Event e;
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
            }
                    
            // Save the image
            void Window::saveFramebuffer() {
                IMG_SavePNG(framebuffer, outputFile);
            }

            // Free resources and close SDL
            void Window::cleanup() {
                SDL_FreeSurface(framebuffer);
                SDL_DestroyWindow(window);
                SDL_Quit();
            }
    }
// }