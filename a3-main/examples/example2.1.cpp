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
    // sphere1->material->albedo = vec4(1.0f, 0.75f, 0.3f, 1.0f);
    // sphere1->material->diffuse = schlik_reflectance;
    // // sphere1->material->diffuse = iso_reflectance;
    // // sphere1->material->emmission = iso_white;
    sphere1->material = new Metal(vec4(1.0f, 0.9f, 0.75f, 1.0f), 1.0f);
    objects.push_back(sphere1);

    vec4 normal1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    vec4 point1 = vec4(0.0f, -1.0f, 0.0f, 1.0f);
    Plane *plane1 = new Plane(normal1, point1);
    // plane1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane1->material->diffuse = iso_white;
    // plane1->material->reflectance = iso_reflectance;
    plane1->material = new Diffuse(vec4(1.0f), 1.0f);
    objects.push_back(plane1);

    // vec4 normal1_1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    // vec4 point1_1 = vec4(0.0f, 4.0f, 0.0f, 1.0f);
    // Plane *plane1_1 = new Plane(normal1_1, point1_1);
    // plane1_1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane1_1->material->diffuse = iso_white;
    // plane1_1->material->reflectance = iso_reflectance;
    // objects.push_back(plane1_1);

    vec4 normal2 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    vec4 point2 = vec4(0.0f, 0.0f, -3.5f, 1.0f);
    Plane *plane2 = new Plane(normal2, point2);
    // plane2->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane2->material->diffuse = iso_white;
    // plane2->material->reflectance = iso_reflectance;
    plane2->material = new Diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f);
    objects.push_back(plane2);
    
    vec4 normal3 = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    vec4 point3 = vec4(0.0f, 2.0f, 0.0f, 1.0f);
    Plane *plane3 = new Plane(normal3, point3);
    // plane3->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane3->material->diffuse = iso_white;
    // plane3->material->emmission = iso_gray;
    // plane3->material->reflectance = iso_reflectance;
    plane3->material = new Emmissive(vec4(1.0f), 100.0f);
    objects.push_back(plane3);

    // vec4 normal4 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    // vec4 point4 = vec4(0.0f, -2.0f, -0.0f, 1.0f);
    // Plane *plane4 = new Plane(normal4, point4);
    // plane4->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane4->material->diffuse = iso_white;
    // plane4->material->reflectance = iso_reflectance;
    // objects.push_back(plane4);

    // vec4 normal5 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    // vec4 point5 = vec4(0.0f, 2.0f, -0.0f, 1.0f);
    // Plane *plane5 = new Plane(normal5, point5);
    // plane5->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane5->material->diffuse = iso_white;
    // plane5->material->reflectance = iso_reflectance;
    // objects.push_back(plane5);

    vec4 center2 = vec4(0.75f, 0.3f, -1.5f, 1.0f);
    float r2 = 0.1f;
    Sphere *sphere2 = new Sphere(r2, center2);
    // sphere2->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // sphere2->material->diffuse = iso_white;
    // sphere2->material->reflectance = iso_reflectance;
    // sphere2->material->emmission = iso_blue;
    sphere2->material = new Emmissive(vec4(1.0f), 100.0f);
    objects.push_back(sphere2);

    // vec4 center3 = vec4(-0.3f, 0.3f, -1.0f, 1.0f);
    // float r3 = 0.075f;
    // Sphere *sphere3 = new Sphere(r3, center3);
    // sphere3->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // sphere3->material->diffuse = iso_white;
    // sphere3->material->reflectance = iso_reflectance;
    // sphere3->material->emmission = iso_blue;
    // objects.push_back(sphere3);

    // vec4 center4 = vec4(0.0f, 1.0f, -2.5f, 1.0f);
    // float r4 = 0.5f;
    // Sphere *sphere4 = new Sphere(r4, center4);
    // // sphere4->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // // sphere4->material->diffuse = iso_white;
    // // sphere4->material->reflectance = iso_reflectance;
    // // sphere4->material->emmission = iso_blue;
    // sphere4->material = new Emmissive(vec4(1.0f, 1.0f, 1.0f, 1.0f), 100000000.0f);
    // objects.push_back(sphere4);


    scene->objects = objects;

    scene->sky = vec4(0.5f, 0.5f, 0.6f, 1.0f);

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480, 5, 1, 5, "ray_trace")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }


    // while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.draw(scene);
        const char* filename = "metal_ball.png"; 
        r.saveImage(filename);
        // r.show();
    // }
    return EXIT_SUCCESS;
}