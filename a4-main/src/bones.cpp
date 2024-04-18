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
    glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

    rotationMatrix[3][0] = position.x;
    rotationMatrix[3][1] = position.y;
    rotationMatrix[3][2] = position.z;
    rotationMatrix[3][3] = 1.0f;

    return rotationMatrix;
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