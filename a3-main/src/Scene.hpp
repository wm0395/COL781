#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Ray_Tracer{
    public:
    bool initialize(const std::string &title, int width, int height, int spp = 1);
    bool shouldQuit();
    void clear(vec4 color);
    void show();

    private:
    SDL_Window *window;
    bool quit;
    SDL_Surface* framebuffer = NULL;
    SDL_Surface *windowSurface = NULL; 
    int frameWidth;
    int frameHeight;
    int displayScale;
    int spp; // Samples-per-pixel

};


class Camera {
    public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up; 
    glm::vec3 lookAt;
    glm::mat4 viewMatrix;

    float cameraSpeed, yaw, pitch, lastX, lastY, fov, aspect;
    bool firstMouse;
    void initialize(float aspect);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getViewDir();
    glm::vec3 getRightVector();

    void setCameraView(glm::vec3 position_vector, glm::vec3 lookat_vector, glm::vec3 up_vector);
    void updateViewMatrix();
};