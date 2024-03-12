#include <glm/glm.hpp>
using namespace glm;
using namespace std;

class Ray{

    Ray(vec4 &o, vec4 &d);

    public:
    vec4 origin;
    vec4 direction;
};