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

    position = glm::vec3(0.0f, 0.0f, 1.5f);
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

void Ray_Tracer::draw(Camera &cam){
    // Lock the surface to access its pixel data
    if (SDL_LockSurface(framebuffer) != 0) {
        // Handle error
        return;
    }

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

            // Access the pixel value
            Uint32 pixelValue = pixels[pixelIndex];

            // // Extract individual color components (if needed)
            // Uint8 red, green, blue, alpha;
            // SDL_GetRGBA(pixelValue, pixelFormat, &red, &green, &blue, &alpha);

            // Calculate the center coordinates of the pixel
            float center_x = x + 0.5f;
            float center_y = y + 0.5f;

            // Print the center coordinates
            // printf("Center coordinates of pixel (%d, %d): (%.2f, %.2f)\n", x, y, center_x, center_y);

            Ray ray;
            ray.o = vec4(0.0, 0.0, 0.0, 1.0);  // needs to be corrected
            ray.d = vec4(center_x, center_y, ray.t_near, 0.0f);    // here also t_near needs to be seen

            // Sphere.hit()

        }
    }

    // Unlock the surface when done
    SDL_UnlockSurface(framebuffer);
}