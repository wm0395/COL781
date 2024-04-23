#include "sw.hpp"

Bone::Bone(){

}

mat4 Bone::getMatrix(){
    if(parent)
        return parent->getMatrix() * getRotation();
    else
        return getRotation();
}

mat4 Bone::getRotation(){
    glm::quat rotationQuat = glm::angleAxis(theta, axis);
    rotationQuat = glm::normalize(rotationQuat);
    glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), vec3(position.x, position.y, position.z));

    return translationMatrix * rotationMatrix;
}

vec3 transformPoint(const vec3& point, const mat4& transformationMatrix) {
    // Extend the 3D point to a 4D homogeneous coordinate
    glm::vec4 homogeneousPoint(point.x, point.y, point.z, 1.0f);

    // Multiply the 4D point by the transformation matrix
    glm::vec4 transformedPoint = transformationMatrix * homogeneousPoint;

    // If the last component is not 1 (due to perspective transformations), normalize
    if (transformedPoint.w != 1.0f) {
        transformedPoint /= transformedPoint.w;
    }

    // Extract the transformed 3D point from the 4D vector
    return glm::vec3(transformedPoint);
}

float CatmullRomInterpolation(float t, float p0, float p1, float p2, float p3) {
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.5f * ((2 * p1) +
                   (-p0 + p2) * t +
                   (2 * p0 - 5 * p1 + 4 * p2 - p3) * t2 +
                   (-p0 + 3 * p1 - 3 * p2 + p3) * t3);
}

std::vector<float> interpolateCatmullRomSpline(float t, const Keyframe& k0, const Keyframe& k1, const Keyframe& k2, const Keyframe& k3) {
    std::vector<float> result;

    float t0 = k0.time;
    float t1 = k1.time;
    float t2 = k2.time;
    float t3 = k3.time;

    float t01 = (t - t1) / (t2 - t1);
    float t12 = (t - t0) / (t2 - t0);
    float t23 = (t - t1) / (t3 - t1);

    for (size_t i = 0; i < k0.values.size(); ++i) {
        float interpolatedValue = CatmullRomInterpolation(t01, k0.values[i], k1.values[i], k2.values[i], k3.values[i]);
        result.push_back(interpolatedValue);
    }

    return result;
}

std::vector<std::vector<float>> interpolateVectors(const std::vector<Keyframe>& keyframes, int numSamples) {
    std::vector<std::vector<float>> result;

    if (keyframes.size() < 4) {
        // Cannot interpolate with less than 4 keyframes
        return result;
    }

    // Interpolate between keyframes with the appropriate number of samples for each segment
    for (size_t i = 1; i < keyframes.size() - 2; ++i) {
        const Keyframe& k0 = keyframes[i - 1];
        const Keyframe& k1 = keyframes[i];
        const Keyframe& k2 = keyframes[i + 1];
        const Keyframe& k3 = keyframes[i + 2];

        for (int j = 0; j < numSamples; ++j) {
            float t = k1.time + (static_cast<float>(j) / (numSamples - 1)) * (k2.time - k1.time);
            std::vector<float> interpolatedValues = interpolateCatmullRomSpline(t, k0, k1, k2, k3);
            result.push_back(interpolatedValues);
        }
    }

    return result;
}