#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Sphere{

    Sphere(vec4 &c, float r);

    public:
    vec4 center;
    float radius;
};