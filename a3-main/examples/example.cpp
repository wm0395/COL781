#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

using namespace std;
using namespace glm;

int main(){

    Scene *scene;
    Camera *cam;
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    scene->camera = cam;

    vector<Shape*> objects = {};
    Sphere sphere1(1.0f, vec4(0, 0, -2, 1));
    objects.push_back(&sphere1);
    
    scene->objects = objects;

    Ray_Tracer r;
    if (!r.initialize("Example 1", 100, 200)){
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