#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

using namespace std;
using namespace glm;

vec4 iso_blue(vec4 position, vec4 omega, vec4 normal){
    return vec4(0.0f, 0.0f, 1.0f, 1.0f);
}

vec4 iso_white(vec4 position, vec4 omega, vec4 normal){
    return vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(){

    Scene *scene = new Scene();
    Camera *cam = new Camera();
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    cam->up = vec3(0.0f, 1.0f, 0.0f);
    cam->lookAt = vec3(0.0f, 0.0f, -1.0f);
<<<<<<< HEAD
    cam->fov = 60.0f;
    cam->near_plane = -1.0f;
    cam->far_plane = 1000.0f;
=======
>>>>>>> main
    cam->updateViewMatrix();
    scene->camera = cam;

    vector<Shape*> objects = {};

    vec4 center1 = vec4(0.0f, 0.0f, -2.5f, 1.0f);
    float r1 = 1.0f;
    Sphere *sphere1 = new Sphere(r1, center1);
    sphere1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sphere1->material->diffuse = iso_blue;
    objects.push_back(sphere1);

    vec4 normal1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    vec4 point1 = vec4(0.0f, -1.0f, 0.0f, 1.0f);
    Plane *plane1 = new Plane(normal1, point1);
    plane1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    plane1->material->diffuse = iso_white;
    objects.push_back(plane1);

    vec4 normal2 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    vec4 point2 = vec4(0.0f, -1.0f, -5.5f, 1.0f);
    Plane *plane2 = new Plane(normal2, point2);
    plane2->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    plane2->material->diffuse = iso_white;
    // plane2->material->emmission = iso_white;
    objects.push_back(plane2);
    
    scene->objects = objects;

    vector<Light*> lights = {};

    vec4 position1 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    vec4 intensity1 = vec4(15.0f, 0.0f, 0.0f, 1.0f);
    Light *light1 = new Light();
    light1->Intensity = intensity1;
    light1->position = position1;
    lights.push_back(light1);

    vec4 position2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    vec4 intensity2 = vec4(0.0f, 0.0f, 15.0f, 1.0f);
    Light *light2 = new Light();
    light2->Intensity = intensity2;
    light2->position = position2;
    lights.push_back(light2);

    vec4 position3 = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    vec4 intensity3 = vec4(0.0f, 15.0f, 0.0f, 1.0f);
    Light *light3 = new Light();
    light3->Intensity = intensity3;
    light3->position = position3;
    lights.push_back(light3);

    vec4 position4 = vec4(0.0f, 3.0f, -2.5f, 1.0f);
    vec4 intensity4 = vec4(100.0f, 100.0f, 100.0f, 100.0f);
    Light *light4 = new Light();
    light4->Intensity = intensity4;
    light4->position = position4;
    lights.push_back(light4);
    
    scene->lights = lights;

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480, 1, 1, 1, "point_lambert")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }


    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.draw(scene);
        r.show();
    }
    return EXIT_SUCCESS;
}