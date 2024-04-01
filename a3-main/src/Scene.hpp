#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

    void translate_camera(vec3 v);
    void rotate_camera(float radian, vec3 axis);
};

struct Ray{
    vec4 o;
    vec4 d;
    float t;
    float t_near;
    float t_far;
};

class Material{
    public:
    vec4 albedo;
    vec4 (*emmission)(vec4 position, vec4 omega, vec4 normal);
    vec4 (*diffuse)(vec4 position, vec4 omega, vec4 normal);
    vec4 (*reflectance)(vec4 position, vec4 omega, vec4 normal);
};

class Shape {
    public:
    Shape();
    virtual std::pair<Ray*, vec4> hit(Ray *ray) = 0;
    virtual vec4 normal_ray(vec4 position) = 0;
    Material *material;
    mat4 transformation_mat;

    void scaling(vec3 s);
    void translation(vec3 t);
    void rotation(float radian, vec3 axis);
    void invert_transformation();
};


class Sphere : public Shape {
    public:
    Sphere(const float &r, const vec4 &c);
    std::pair<Ray*, vec4> hit(Ray *ray) override;
    vec4 normal_ray(vec4 position) override;

    private:
    float radius;
    vec4 centre;

    pair<Ray*, vec4> reflected_ray(Ray* ray, float t);
};

class Plane : public Shape {
    public:
    Plane(const vec4 &normal, const vec4 &point_on_plane);
    std::pair<Ray*, vec4> hit(Ray *ray) override;
    vec4 normal_ray(vec4 position) override;

    private:
    vec4 normal;
    vec4 point_on_plane;

    pair<Ray*, vec4> reflected_ray(Ray* ray, float t);
};

class Bounding_Box : public Shape {
    public:
    Bounding_Box(const vec4 &min, const vec4 &max);
    std::pair<Ray*, vec4> hit(Ray *ray) override;
    vec4 normal_ray(vec4 position) override;

    private:
    vec4 min;
    vec4 max;

    pair<Ray*, vec4> reflected_ray(Ray* ray, float t, int min_plane);
};

class Light{
    public:
    vec4 Intensity;
    vec4 position;
};

struct Scene{
    Camera* camera;
    vec4 sky = vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
    
    vec4 render(Ray *ray);

    private:
    vec4 path_trace(int obj_id, vec4 position, vec4 out_dir, int depth);
    vec4 rand_hemisphere();
    pair<int, vec4> incident_ray(vec4 position, vec4 direction);
    int shadow_ray(int light_id, vec4 position);
    
    vec4 point_lambert(Ray *ray);
    vec4 normal_map(Ray *ray);
    vec4 ray_trace(Ray *ray);
    vec4 MC_Sampling(int obj_id, vec4 position, vec4 out_dir, int depth);
};

class Ray_Tracer{
    public:
    bool initialize(const std::string &title, int width, int height, int bounces=1, int paths=1, int samples=1, string sampling="normal_map");
    bool shouldQuit();
    void clear(vec4 color);
    void show();
    void draw(Scene *scene);
    void saveImage(const char *filename);

    private:
    SDL_Window *window;
    bool quit;
    SDL_Surface* framebuffer = NULL;
    SDL_Surface *windowSurface = NULL;
    vec4 sample(float x, float y, Camera* camera);

    int frameWidth;
    int frameHeight;
    int displayScale;
    int spp; // Samples-per-pixel
    Renderer* renderer;
};