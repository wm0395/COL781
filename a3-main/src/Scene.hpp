#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Ray_Tracer{
    public:
    bool initialize(const std::string &title, int width, int height, int spp);

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

struct Ray{
    vec4 o;
    vec4 d;
    float t_near;
    float t_far;
};

struct Material{

};

class Shape {
public:
    Shape();
    virtual std::pair<Ray*, vec4> hit(Ray *ray) = 0;
    Material* get_material();
private:
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