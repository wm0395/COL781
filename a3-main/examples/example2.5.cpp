#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.hpp"

using namespace std;
using namespace glm;

int main(){

    Scene *scene = new Scene();
    Camera *cam = new Camera();
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    cam->up = vec3(0.0f, 1.0f, 0.0f);
    cam->lookAt = vec3(0.0f, 0.0f, -1.0f);
    cam->fov = 60.0f;
    cam->near_plane = -1.0f;
    cam->far_plane = -1000.0f;
    cam->updateViewMatrix();
    scene->camera = cam;

    vector<Shape*> objects = {};

    vec4 center1 = vec4(0.0f, 0.0f, -2.5f, 1.0f);
    float r1 = 1.0f;
    Sphere *sphere1 = new Sphere(r1, center1);
    sphere1->material = new Diffuse(vec4(1.0f, 0.9f, 0.75f, 1.0f), 1.0f);
    objects.push_back(sphere1);

    vec4 normal1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    vec4 point1 = vec4(0.0f, -1.0f, 0.0f, 1.0f);
    Plane *plane1 = new Plane(normal1, point1);
    plane1->material = new Diffuse(vec4(1.0f), 1.0f);
    objects.push_back(plane1);

    vec4 normal2 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    vec4 point2 = vec4(0.0f, 0.0f, -3.5f, 1.0f);
    Plane *plane2 = new Plane(normal2, point2);
    plane2->material = new Diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f);
    objects.push_back(plane2);
    
    vec4 normal3 = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    vec4 point3 = vec4(0.0f, 2.0f, 0.0f, 1.0f);
    Plane *plane3 = new Plane(normal3, point3);
    plane3->material = new Emmissive(vec4(1.0f), 100.0f);
    objects.push_back(plane3);

    scene->objects = objects;

    scene->sky = vec4(0.5f, 0.5f, 0.6f, 1.0f);

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480, 10, 1, 10, "ray_trace")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }


    // while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.draw(scene);
        const char* filename = "diffuse_with_cosine.png"; 
        r.saveImage(filename);
        // r.show();
    // }
    return EXIT_SUCCESS;
}