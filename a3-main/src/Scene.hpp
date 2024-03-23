#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

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

struct Ray{
    vec4 o;
    vec4 d;
    float t;
    float t_near;
    float t_far;
};

struct Material{
    vec3 albedo;
    vec4 (*emmission)(vec4 position, vec4 omega);
    vec4 (*diffuse)(vec4 position, vec4 omega);
};

class Shape {
    public:
    Shape();
    virtual std::pair<Ray*, vec4> hit(Ray *ray) = 0;
    Material* material;
};


class Sphere : public Shape {
    public:
    Sphere(const float &r, const vec4 &c);
    std::pair<Ray*, vec4> hit(Ray *ray) override;

    private:
    float radius;
    vec4 centre;

    pair<Ray*, vec4> reflected_ray(Ray* ray, float t);
};

class Plane : public Shape {
    public:
    Plane(const vec4 &normal, const vec4 &point_on_plane);
    std::pair<Ray*, vec4> hit(Ray *ray) override;

    private:
    vec4 normal;
    vec4 point_on_plane;

    pair<Ray*, vec4> reflected_ray(Ray* ray, float t);
};

class Light{
    public:
    vec4 Intensity;
    vec4 position;
};

struct Scene{
    Camera* camera;
    vector<Shape*> objects;
    vector<Light*> lights;
    float near_plane;
    float far_plane;
};

class Renderer{
    public:
    const int MAX_BOUNCES;
    const int PATHS;
    const int SAMPLES;
    const string SAMPLING;
    Scene* scene;
    Renderer(int bounces, int paths, int samples, string sampling);

    vec4 MC_Sampling(int obj_id, vec4 position, vec4 out_dir, int depth);
    private:
    vec4 path_trace(int obj_id, vec4 position, vec4 out_dir, int depth);
    vec4 rand_hemisphere();
    pair<int, vec4> incident_ray(vec4 position, vec4 direction);
};

class Ray_Tracer{
    public:
    bool initialize(const std::string &title, int width, int height, int spp = 1);
    bool shouldQuit();
    void clear(vec4 color);
    void show();
    void draw(Scene *scene);

    private:
    SDL_Window *window;
    bool quit;
    SDL_Surface* framebuffer = NULL;
    SDL_Surface *windowSurface = NULL;
    vec4 sample(Scene *scene, float x, float y);

    int frameWidth;
    int frameHeight;
    int displayScale;
    int spp; // Samples-per-pixel
    Renderer* renderer;
};