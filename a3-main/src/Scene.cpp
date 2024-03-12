#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>


void Camera::initialize(float aspect) {
    firstMouse = true;
    yaw   = -90.0f;    
    pitch =  0.0f;
    lastX =  800.0f / 2.0;
    lastY =  600.0 / 2.0;
    fov   =  60.0f;

    this->aspect = aspect;

    position = glm::vec3(0.0f, 0.0f,  1.5f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);

    updateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() {
    return viewMatrix;
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, lookAt, up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}
    glm::vec3 getRightVector();

glm::vec3 Camera:: getViewDir() {
    return -glm::transpose(viewMatrix)[2];
}

glm::vec3 Camera::getRightVector() {
    return glm::transpose(viewMatrix)[0];
}

void Camera::setCameraView(glm::vec3 position_vector, glm::vec3 lookat_vector, glm::vec3 up_vector) {
    position = std::move(position_vector);
    lookAt = std::move(lookat_vector);
    up = std::move(up_vector);

    viewMatrix = glm::lookAt(position, lookAt, up);
}



bool Ray_Tracer::initialize(const std::string &title, int width, int height, int spp){
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