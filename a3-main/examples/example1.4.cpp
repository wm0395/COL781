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
    cam->translate_camera(vec3(0.0f, 0.0f, 4.0f));
    // cam->rotate_camera(-40.0f, vec3(0.0f, 1.0f, 0.0f));
    scene->camera = cam;

    vector<Shape*> objects = {};

    vec4 center1 = vec4(0.0f, 0.0f, -2.0f, 1.0f);
    float r1 = 1.0f;
    Sphere *sphere1 = new Sphere(r1, center1);
    // sphere1->translation(vec3(1.0f, -1.0f, -2.0f));
    sphere1->scaling(vec3(2.0f, 2.0f, 2.0f));
    // sphere1->rotation(90.0f, vec3(0.0f, 1.0f, 1.0f));
    sphere1->invert_transformation();  // carry this out at the end of all the transformations  
    objects.push_back(sphere1);

    // vec4 center2 = vec4(0.0f, -101.0f, -2.0f, 1.0f);
    // float r2 = 100.0f;
    // Sphere *sphere2 = new Sphere(r2, center2);
    // objects.push_back(sphere2);

    // vec4 normal1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    // vec4 point1 = vec4(0.0f, -1.0f, 0.0f, 1.0f);
    // Plane *plane1 = new Plane(normal1, point1);
    // objects.push_back(plane1);

    // vec4 min1 = vec4(-4.0f, -3.0f, 3.0f, 1.0f);
    // vec4 max1 = vec4(4.0f, 3.0f, -10.0f, 1.0f);
    // Axis_Aligned_Box* bb1 = new Axis_Aligned_Box(min1, max1);
    // objects.push_back(bb1);

    // vec4 min2 = vec4(-2.0f, -2.0f, -5.0f, 1.0f);
    // vec4 max2 = vec4(1.5f, 3.0f, -3.0f, 1.0f);
    // Axis_Aligned_Box* bb2 = new Axis_Aligned_Box(min2, max2);
    // objects.push_back(bb2);
    
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