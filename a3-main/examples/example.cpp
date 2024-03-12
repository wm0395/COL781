#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

using namespace std;
using namespace glm;

int main(){

    Ray_Tracer r;
    if (!r.initialize("Example 1", 640, 480)){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.show();
    }
    return EXIT_SUCCESS;
}