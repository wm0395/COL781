#ifndef SW_HPP
#define SW_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> // Include quaternion functionality
#include <glm/gtx/quaternion.hpp> // Include quaternion extensions
#include <glm/gtc/matrix_transform.hpp>
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
    vector<int> verIndex;
    
    mat4 getMatrix();
    private:
    mat4 getRotation();
};

vec3 transformPoint(const vec3& point, const mat4& transformationMatrix);


struct Keyframe {
    float time;
    std::vector<float> values;
};

std::vector<float> interpolateCatmullRom(float t, const Keyframe& k0, const Keyframe& k1, const Keyframe& k2, const Keyframe& k3);
std::vector<std::vector<float>> interpolateVectors(const std::vector<Keyframe>& keyframes, int numSamples);

#endif