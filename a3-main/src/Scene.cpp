#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>



bool Ray_Tracer::initialize(const std::string &title, int width, int height, int bounces, int paths, int samples, string sampling){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        success = false;
    }
    else {
        this->frameHeight = height;
        this->frameWidth = width;
        this->displayScale = 1;
        this->spp = spp;
        int screenWidth = frameWidth * displayScale;
        int screenHeight = frameHeight * displayScale;
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s", SDL_GetError());
            success = false;
        } else {
            this->windowSurface = SDL_GetWindowSurface(window);
            this->framebuffer = SDL_CreateRGBSurface(0, this->frameWidth, this->frameHeight, 32, 0, 0, 0, 0);
            this->renderer = new Renderer(bounces, paths, samples, sampling);
        }
    }
    return success;
}

bool Ray_Tracer::shouldQuit(){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
    return quit;
}

void Ray_Tracer::clear(vec4 color){
    SDL_Color sdlClearColor = {
        static_cast<Uint8>(color.r * 255),
        static_cast<Uint8>(color.g * 255),
        static_cast<Uint8>(color.b * 255),
        static_cast<Uint8>(color.a * 255),
    };

    // Fill the framebuffer surface with the clear color
    SDL_FillRect(framebuffer, nullptr, SDL_MapRGB(framebuffer->format, sdlClearColor.r, sdlClearColor.g, sdlClearColor.b));
}

void Ray_Tracer::show(){
    Uint32 *pixels = (Uint32*)framebuffer->pixels;
    SDL_PixelFormat *format = framebuffer->format;
    SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);
}	

void Ray_Tracer::draw(Scene *scene){
    // Lock the surface to access its pixel data
    if (SDL_LockSurface(framebuffer) != 0) {
        // Handle error
        return;
    }

    // Set Scene in renderer
    renderer->scene = scene;

    // Get the pixel format of the surface
    SDL_PixelFormat* pixelFormat = framebuffer->format;

    // Access the pixel data
    Uint32* pixels = static_cast<Uint32*>(framebuffer->pixels);

    // Calculate the pitch (bytes per row)
    int pitch = framebuffer->pitch / sizeof(Uint32);

    // Loop through each row of pixels
    for (int y = 0; y < framebuffer->h; ++y) {
        // Loop through each pixel in the row
        for (int x = 0; x < framebuffer->w; ++x) {
            // Calculate the index of the current pixel
            int pixelIndex = y * pitch + x;

            float center_x = (x + 0.5f);///framebuffer->w;
            float center_y = (y + 0.5f);///framebuffer->h;

            vec4 color = sample(center_x, center_y);
            pixels[pixelIndex] = SDL_MapRGBA(pixelFormat, 255*color.x, 255 * color.y, 255*color.z, 255 * color.w);

            // Access the pixel value
            Uint32 pixelValue = pixels[pixelIndex];

            // // Extract individual color components (if needed)
            // Uint8 red, green, blue, alpha;
            // SDL_GetRGBA(pixelValue, pixelFormat, &red, &green, &blue, &alpha);
            // return;

        }
    }
    
    SDL_UnlockSurface(framebuffer);
}

vec4 Ray_Tracer::sample(float x, float y){
    
    Ray* ray = new Ray();
    float h_prime = 1/sqrt(3);
    float b_prime = h_prime * framebuffer->w / float(framebuffer->h);
    float x_prime = 2 * b_prime * (x - (float(framebuffer->w) / 2)) / float(framebuffer -> w);
    float y_prime = 2 * h_prime * (-y + (float(framebuffer->h) / 2)) / float(framebuffer -> h);
    ray->o = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ray->d = vec4(x_prime, y_prime, -1.0f, 0.0f);
    // cout << ray->d.x << " " << ray->d.y << " " << ray->d.z << " " << ray->d.w << " \n";
    ray->t_near = 0.01f;
    ray->t_far = 1000.0f;
    float t = INT32_MAX;

    vec4 color = renderer->render(ray);
    return color;
}