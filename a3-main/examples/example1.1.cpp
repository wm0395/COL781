#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

using namespace std;
using namespace glm;

int main(){

    cout << "Hello World\n";

    Scene *scene = new Scene();
    Camera *cam = new Camera();
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    cam->up = vec3(0.0f, 1.0f, 0.0f);
    cam->lookAt = vec3(0.0f, 0.0f, -1.0f);
    cam->updateViewMatrix();
    // cam->translate_camera(vec3(2.0f, 1.5f, 1.0f));
    scene->camera = cam;

    vector<Shape*> objects = {};

    // vec4 center1 = vec4(0.0f, 0.0f, -2.0f, 1.0f);
    // float r1 = 1.0f;
    // Sphere *sphere1 = new Sphere(r1, center1);
    // objects.push_back(sphere1);

    // vec4 center2 = vec4(0.0f, -101.0f, -2.0f, 1.0f);
    // float r2 = 100.0f;
    // Sphere *sphere2 = new Sphere(r2, center2);
    // objects.push_back(sphere2);

    // vec4 min1 = vec4(-1.0f, -1.0f, -4.0f, 1.0f);
    // vec4 max1 = vec4(1.0f, 1.0f, -3.0f, 1.0f);
    // Bounding_Box* bb1 = new Bounding_Box(min1, max1);
    // objects.push_back(bb1);

    vec4 p0 = vec4(-2.0f, -2.0f, -3.0f, 1.0f);
    vec4 p1 = vec4(2.0f, -2.0f, -3.0f, 1.0f);
    vec4 p2 = vec4(0.0f, 2.0f, -3.0f, 1.0f);
    Triangle *triangle1 = new Triangle(p0, p1, p2);
    objects.push_back(triangle1);
    
    scene->objects = objects;

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480, 1, 1, 1, "normal_map")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.draw(scene);
        r.show();
        // break;
    }
    return EXIT_SUCCESS;
}