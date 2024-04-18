#ifndef SW_HPP
#define SW_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> // Include quaternion functionality
#include <glm/gtx/quaternion.hpp> // Include quaternion extensions
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <set>

using namespace glm;
using namespace std;

class Bone{
    public:
    Bone();
    vec4 position;
    Bone* parent;
    vec3 axis;
    bool isVisible = false;
    float theta;
    vector<int> triIndex;
    
    mat4 getMatrix();
    private:
    mat4 getRotation();
};

vec3 transformPoint(const vec3& point, const mat4& transformationMatrix);

#endif