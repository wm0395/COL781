// Rasterization starter code for COL781: Computer Graphics

/* Instructions for execution:
   1. Install SDL2 and SDL2_image libraries
   2. Compile using: g++ starter_code.cpp -I/path/to/SDL2 -lSDL2 -lSDL2_image
   (on Linux or MacOS, it should be sufficient to copy-paste the following:
   g++ starter_code.cpp `pkg-config --cflags --libs SDL2 SDL2_image`
   3. Run using: ./a.out
   4. The rendered frame will be shown in a window, and saved to a file on exit
*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Framebuffer-related variables.
   We create a 20x20 image and display it at 20x size so you can clearly
   see the pixels! Set displayScale to 1 to see it at the correct size.
   Then you can increase frameWidth and frameHeight as desired. */

const int frameWidth = 20;
const int frameHeight = 20;

const int displayScale = 20;

SDL_Surface* framebuffer = NULL;

/* SDL parameters */

SDL_Window* window = NULL;
SDL_Surface *windowSurface = NULL;
bool quit = false;

/* Output file */

const char* outputFile = "out.png";

/* Function prototypes */

bool initialize();
void handleEvents();
void saveFramebuffer();
void cleanup();

/* Main function. Pay attention to this. */

void draw(vector<vector<float>> &screen) {
    if (!initialize()) {
        printf("Failed to initialize!");
    } else {
        // Display and interaction
        while (!quit) {
            // Event handling
            handleEvents();

            /* Set pixel data.
               CHANGE THIS TO YOUR OWN CODE! */

            Uint32 *pixels = (Uint32*)framebuffer->pixels;
            SDL_PixelFormat *format = framebuffer->format;
            for (int i = 0; i < frameWidth; i++) {
                for (int j = 0; j < frameHeight; j++) {
                    // float x = i + 0.5;
                    // float y = j + 0.5;
                    // float cx = 10, cy = 10, r = 8;
                    // Uint32 color;
                    // if ((x-cx)*(x-cx) + (y-cy)*(y-cy) <= r*r) { // inside circle
                    //     color = SDL_MapRGBA(format, 0, 153, 0, 255); // green
                    // } else {
                    //     float l = 255*j/frameHeight;
                    //     color = SDL_MapRGBA(format, l, l, l, 255); // grey proportional to y
                    // }
                    // pixels[i + frameWidth*j] = color;
                    float l = 255*j/frameHeight;
                    pixels[i + frameWidth*j] = SDL_MapRGBA(format, l, l, l, 255*screen[j][i]);;
                }
            }

            // Update screen to apply the changes
            SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(window);
        }
    }
    // Save image
    saveFramebuffer();
    cleanup();
}

/* Everything below here is library-specific boilerplate code.
   You can ignore it for now. */

// Initialising SDL2
bool initialize() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        success = false;
    } else {
        int screenWidth = frameWidth * displayScale;
        int screenHeight = frameHeight * displayScale;
        window = SDL_CreateWindow("COL781", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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
void handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
}
		
// Save the image
void saveFramebuffer() {
    IMG_SavePNG(framebuffer, outputFile);
}

// Free resources and close SDL
void cleanup() {
    SDL_FreeSurface(framebuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}