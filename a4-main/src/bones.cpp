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

// std::vector<float> interpolateCatmullRom(float t, const Keyframe& k0, const Keyframe& k1, const Keyframe& k2, const Keyframe& k3) {
//     std::vector<float> result;

//     // Catmull-Rom spline interpolation formula
//     float t2 = t * t;
//     float t3 = t2 * t;
//     float m0 = ((-0.5f * t3) + t2 - (0.5f * t));
//     float m1 = ((1.5f * t3) - (2.5f * t2) + 1);
//     float m2 = ((-1.5f * t3) + (2 * t2) + (0.5f * t));
//     float m3 = (0.5f * t3) - (0.5f * t2);

//     for (size_t i = 0; i < k0.values.size(); ++i) {
//         float interpolatedValue = k0.values[i] * m0 + k1.values[i] * m1 + k2.values[i] * m2 + k3.values[i] * m3;
//         result.push_back(interpolatedValue);
//     }

//     return result;
// }

// Interpolate a vector of floats between keyframes using Catmull-Rom spline
// std::vector<std::vector<float>> interpolateVectors(const std::vector<Keyframe>& keyframes, int numSamples) {
//     std::vector<std::vector<float>> result;

//     if (keyframes.size() < 4) {
//         // Cannot interpolate with less than 4 keyframes
//         return result;
//     }

//     for (size_t i = 1; i < keyframes.size() - 2; ++i) {
//         // Calculate time steps between adjacent keyframes
//         float dt1 = keyframes[i].time - keyframes[i - 1].time;
//         float dt2 = keyframes[i + 1].time - keyframes[i].time;
//         float dt3 = keyframes[i + 2].time - keyframes[i + 1].time;

//         // Calculate the number of samples between keyframes based on time differences
//         int samples1 = static_cast<int>((dt1 / (dt1 + dt2)) * numSamples);
//         int samples2 = static_cast<int>((dt2 / (dt2 + dt3)) * numSamples);

//         // Adjust sample count for unequal time intervals
//         int totalSamples = samples1 + samples2;

//         // Interpolate between keyframes with the appropriate number of samples
//         for (int j = 0; j < totalSamples; ++j) {
//             float t;
//             if (j < samples1) {
//                 t = static_cast<float>(j) / (samples1 - 1);
//             } else {
//                 t = static_cast<float>(j - samples1) / (samples2 - 1);
//             }

//             // Check boundaries to avoid segmentation fault
//             size_t index0 = std::max(0, static_cast<int>(i) - 1);
//             size_t index1 = i;
//             size_t index2 = std::min(keyframes.size() - 1, i + 1);
//             size_t index3 = std::min(keyframes.size() - 1, i + 2);

//             std::vector<float> interpolatedValues = interpolateCatmullRom(t, keyframes[index0], keyframes[index1], keyframes[index2], keyframes[index3]);
//             result.push_back(interpolatedValues);
//         }
//     }

//     return result;
// }


std::vector<float> interpolateCubicHermiteSpline(float t, const Keyframe& k0, const Keyframe& k1) {
    std::vector<float> result;

    // Calculate interpolation parameter
    float dt = k1.time - k0.time;
    // float t01 = (t - k0.time) / dt;
    float t01 = t;

    // Cubic Hermite spline interpolation formula
    float h00 = 2.0f * t01 * t01 * t01 - 3.0f * t01 * t01 + 1.0f;
    float h10 = t01 * t01 * t01 - 2.0f * t01 * t01 + t01;
    float h01 = -2.0f * t01 * t01 * t01 + 3.0f * t01 * t01;
    float h11 = t01 * t01 * t01 - t01 * t01;

    // Interpolate values for each component
    for (size_t i = 0; i < k0.values.size(); ++i) {
        float interpolatedValue = k0.values[i] * h00 + k1.values[i] * h01 +
                                   (k1.values[i] - k0.values[i]) * h10 * dt +
                                   (k1.values[i] - k0.values[i]) * h11 * dt;
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

    // Calculate total duration of the animation
    float totalDuration = keyframes.back().time - keyframes.front().time;

    // Calculate the total number of segments between keyframes
    size_t numSegments = keyframes.size() - 1;

    // Calculate the number of samples per segment based on time differences
    std::vector<int> segmentSamples;
    int totalSegmentSamples = 0;
    for (size_t i = 0; i < numSegments; ++i) {
        float segmentDuration = keyframes[i + 1].time - keyframes[i].time;
        int segmentSampleCount = static_cast<int>((segmentDuration / totalDuration) * numSamples);
        segmentSamples.push_back(segmentSampleCount);
        totalSegmentSamples += segmentSampleCount;
    }

    // Adjust the sample count for rounding errors
    int remainingSamples = numSamples - totalSegmentSamples;
    for (size_t i = 0; i < numSegments && remainingSamples > 0; ++i) {
        segmentSamples[i]++;
        remainingSamples--;
    }

    // Interpolate between keyframes with the appropriate number of samples for each segment
    for (size_t i = 0; i < numSegments; ++i) {
        const Keyframe& k0 = keyframes[i];
        const Keyframe& k1 = keyframes[i + 1];
        int segmentSampleCount = segmentSamples[i];
        
        for (int j = 0; j < segmentSampleCount; ++j) {
            float t = static_cast<float>(j) / (segmentSampleCount - 1);
            std::vector<float> interpolatedValues = interpolateCubicHermiteSpline(t, k0, k1);
            result.push_back(interpolatedValues);
        }
    }

    return result;
}


// std::vector<float> interpolateCubicHermiteSpline(float t, const Keyframe& k0, const Keyframe& k1) {
//     std::vector<float> result;

//     // Calculate interpolation parameter
//     float dt = k1.time - k0.time;
//     // float t01 = (t - k0.time) / dt;
//     float t01 = t;

//     // Cubic Hermite spline interpolation formula
//     float h00 = 2.0f * t01 * t01 * t01 - 3.0f * t01 * t01 + 1.0f;
//     float h10 = t01 * t01 * t01 - 2.0f * t01 * t01 + t01;
//     float h01 = -2.0f * t01 * t01 * t01 + 3.0f * t01 * t01;
//     float h11 = t01 * t01 * t01 - t01 * t01;

//     // Calculate derivatives at keyframes
//     std::vector<float> derivative0, derivative1;
//     for (size_t i = 0; i < k0.values.size(); ++i) {
//         // First derivative at k0
//         float m0 = (k1.values[i] - k0.values[i]) / dt;

//         // First derivative at k1
//         float m1;
//         if (i < k1.values.size() - 1) {
//             float m1_next = (k1.values[i + 1] - k1.values[i]) / (k1.time - k0.time);
//             m1 = 0.5f * (m0 + m1_next); // Average of slopes at k0 and k1
//         } else {
//             m1 = m0; // If k1 is the last keyframe, use the slope at k0
//         }

//         derivative0.push_back(m0);
//         derivative1.push_back(m1);
//     }

//     // Interpolate values for each component
//     for (size_t i = 0; i < k0.values.size(); ++i) {
//         float interpolatedValue = k0.values[i] * h00 + k1.values[i] * h01 +
//                                    derivative0[i] * h10 * dt +
//                                    derivative1[i] * h11 * dt;
//         result.push_back(interpolatedValue);
//     }

//     return result;
// }

// std::vector<std::vector<float>> interpolateVectors(const std::vector<Keyframe>& keyframes, int numSamples) {
//     std::vector<std::vector<float>> result;

//     if (keyframes.size() < 4) {
//         // Cannot interpolate with less than 4 keyframes
//         return result;
//     }

//     // Calculate total duration of the animation
//     float totalDuration = keyframes.back().time - keyframes.front().time;

//     // Calculate the total number of segments between keyframes
//     size_t numSegments = keyframes.size() - 1;

//     // Calculate the number of samples per segment based on time differences
//     std::vector<int> segmentSamples;
//     int totalSegmentSamples = 0;
//     for (size_t i = 0; i < numSegments; ++i) {
//         float segmentDuration = keyframes[i + 1].time - keyframes[i].time;
//         int segmentSampleCount = static_cast<int>((segmentDuration / totalDuration) * numSamples);
//         segmentSamples.push_back(segmentSampleCount);
//         totalSegmentSamples += segmentSampleCount;
//     }

//     // Adjust the sample count for rounding errors
//     int remainingSamples = numSamples - totalSegmentSamples;
//     for (size_t i = 0; i < numSegments && remainingSamples > 0; ++i) {
//         segmentSamples[i]++;
//         remainingSamples--;
//     }

//     // Interpolate between keyframes with the appropriate number of samples for each segment
//     for (size_t i = 0; i < numSegments; ++i) {
//         const Keyframe& k0 = keyframes[i];
//         const Keyframe& k1 = keyframes[i + 1];
//         int segmentSampleCount = segmentSamples[i];
        
//         for (int j = 0; j < segmentSampleCount; ++j) {
//             float t = static_cast<float>(j) / (segmentSampleCount - 1);
//             std::vector<float> interpolatedValues = interpolateCubicHermiteSpline(t, k0, k1);
//             result.push_back(interpolatedValues);
//         }
//     }

//     return result;
// }