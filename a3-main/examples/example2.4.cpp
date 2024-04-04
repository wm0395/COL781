#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

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

    // vec4 center1 = vec4(-0.5f, -0.7f, -2.0f, 1.0f);
    // float r1 = 0.3f;
    // Sphere *sphere1 = new Sphere(r1, center1);
    // // sphere1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // // sphere1->material->diffuse = iso_blue;
    // sphere1->material = new Metal(vec4(0.0f, 0.4f, 0.0f, 1.0f), 1.0f);
    // objects.push_back(sphere1);

    vec4 normal1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    vec4 point1 = vec4(0.0f, -1.0f, 0.0f, 1.0f);
    Plane *plane1 = new Plane(normal1, point1);
    // plane1->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane1->material->diffuse = iso_white;
    plane1->material = new Diffuse(vec4(1.0f), 1.0f);
    objects.push_back(plane1);

    vec4 normal2 = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    vec4 point2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    Plane *plane2 = new Plane(normal2, point2);
    // plane2->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane2->material->diffuse = iso_white;
    plane2->material = new Diffuse(vec4(1.0f), 1.0f);
    objects.push_back(plane2);

    vec4 normal3 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    vec4 point3 = vec4(-1.2f, 0.0f, 0.0f, 1.0f);
    Plane *plane3 = new Plane(normal3, point3);
    // plane3->material->albedo = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    // plane3->material->diffuse = iso_white;
    plane3->material = new Diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
    objects.push_back(plane3);

    vec4 normal4 = vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    vec4 point4 = vec4(1.2f, 0.0f, 0.0f, 1.0f);
    Plane *plane4 = new Plane(normal4, point4);
    // plane4->material->albedo = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    // plane4->material->diffuse = iso_white;
    plane4->material = new Diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f);
    objects.push_back(plane4);

    vec4 normal5 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    vec4 point5 = vec4(0.0f, 0.0f, -3.5f, 1.0f);
    Plane *plane5 = new Plane(normal5, point5);
    // plane5->material->albedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // plane5->material->diffuse = iso_white;
    plane5->material = new Diffuse(vec4(1.0f), 1.0f);
    objects.push_back(plane5);

    vec4 min1 = vec4(-0.3f, 0.95f, -3.0f, 1.0f);
    vec4 max1 = vec4(0.3f, 1.5f, -2.0f, 1.0f);
    Axis_Aligned_Box* bb1 = new Axis_Aligned_Box(min1, max1);
    bb1->material = new Emmissive(vec4(1.0f), 1000.0f);
    objects.push_back(bb1);
    
    scene->objects = objects;

    // vector<Light*> lights = {};

    // vec4 position1 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity1 = vec4(15.0f, 0.0f, 0.0f, 1.0f);
    // Light *light1 = new Light();
    // light1->Intensity = intensity1;
    // light1->position = position1;
    // lights.push_back(light1);

    // vec4 position2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity2 = vec4(0.0f, 0.0f, 15.0f, 1.0f);
    // Light *light2 = new Light();
    // light2->Intensity = intensity2;
    // light2->position = position2;
    // lights.push_back(light2);

    // vec4 position3 = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity3 = vec4(0.0f, 15.0f, 0.0f, 1.0f);
    // Light *light3 = new Light();
    // light3->Intensity = intensity3;
    // light3->position = position3;
    // lights.push_back(light3);

    // vec4 position4 = vec4(0.0f, 0.8f, -0.5f, 1.0f);
    // vec4 intensity4 = vec4(10.0f, 10.0f, 10.0f, 10.0f);
    // Light *light4 = new Light();
    // light4->Intensity = intensity4;
    // light4->position = position4;
    // lights.push_back(light4);
    
    // scene->lights = lights;

    scene->sky = vec4(0.5f, 0.5f, 0.6f, 1.0f);
    scene->mu = 1.0f;

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480, 2, 1, 10, "ray_trace")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }

    r.clear(vec4(1.0, 1.0, 1.0, 1.0));
    r.draw(scene);
    const char* filename = "test.png"; 
    r.saveImage(filename);
    return EXIT_SUCCESS;
}