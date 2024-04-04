#include "Scene.hpp"

vec4 iso_white(vec4 position, vec4 omega, vec4 normal){
    return vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Diffuse::Diffuse(vec4 color, float kd){
    albedo = color;
    diffuse = iso_white;
    isEmissive = false;
    isDiffuse = true;
    isReflective = false;
    isTransparent = false;
    this->kd = kd;
}

vec4 schlik_approximation(vec4 F0, vec4 v, vec4 n){
    float cos_theta = 0.0f;
    cos_theta = glm::max(glm::dot(v, n), 0.0f);
    vec4 R = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    R -= F0;
    R *=  glm::pow(1.0f-cos_theta, 5.0f);
    R += F0;
    return R;
}

vec4 schlik_metal(vec4 incoming, vec4 outgoing, vec4 normal){
    vec4 F0 = vec4(1.000f, 0.766f, 0.336f, 1.0f);
    float roughness = 0.000001f;

    incoming = normalize(incoming);
    outgoing = normalize(outgoing);
    normal = normalize(normal);
    vec4 h = incoming + outgoing;
    h = normalize(h);

    vec4 R = schlik_approximation(F0, incoming, normal);
    // float R = 0.04f + 0.96f * pow(1.0f - cos_theta, 5.0f);

//  Beckmann
    // float NdotH = glm::max(dot(normal, h), 0.0f);
    // // float NdotH = dot(normal, h);
    // float alpha = roughness * roughness;
    // float cos2Theta = NdotH * NdotH;
    // float tan2Theta = (1.0f - cos2Theta) / cos2Theta;
    // float numerator = exp(-tan2Theta / alpha);
    // float denominator = M_PI_2f32 * alpha * cos2Theta * cos2Theta;
    // float D =  numerator / denominator;
    // // if (NdotH == 0.0f)
    // //     D= 0.0f;
    // cout << NdotH <<'\n';

// // Blinn-Phong
//     float D = pow(glm::max(0.0f,dot(h, normal)), 50.5f);
    
// Rough    
    float D = 1.0f;

// // Perfect Mirror
//     float D = glm::max(0.0f,dot(h, normal)) >= 0.99f ? 1000.0f : 0.0f;
    
    float NdotV = glm::max(dot(normal, incoming), 0.0f);
    float NdotL = glm::max(dot(normal, outgoing), 0.0f);
    float k = (roughness * sqrt(2.0f / M_PI)) / (NdotV + sqrt(roughness * roughness + (1.0f - roughness * roughness) * NdotV * NdotV));
    float kL = (roughness * sqrt(2.0f / M_PI)) / (NdotL + sqrt(roughness * roughness + (1.0f - roughness * roughness) * NdotL * NdotL));
    float G =  glm::min(abs(k * kL), 1.0f);

    vec4 color = F0;
    color *= R;
    // color *= G;
    color *= D;
    color *= 1.0f/(dot(normal, incoming) * dot(normal, outgoing) * 4.0f); 
    return color;
}

Metal::Metal(vec4 color, float ks){
    albedo = color;
    reflectance = schlik_metal;
    isEmissive = false;
    isDiffuse = false;
    isReflective = true;
    isTransparent = false;
    this->ks = ks;
}

Emmissive::Emmissive(vec4 color, float ke){
    albedo = color;
    emmission = iso_white;
    isEmissive = true;
    isDiffuse = false;
    isReflective = false;
    isTransparent = false;
    this->ke = ke;
}

vec4 schlik_dielectric(vec4 incoming, vec4 outgoing, vec4 normal){
    // vec4 F0 = vec4(1.000f, 0.766f, 0.336f, 1.0f);
    float n1 = 1.6f;
    float n2 = 1.0f;
    float F0 = pow((n1-n2)/(n1+n2), 2.0f);
    float roughness = 0.000001f;

    incoming = normalize(incoming);
    outgoing = normalize(outgoing);
    normal = normalize(normal);
    vec4 h = incoming + outgoing;
    h = normalize(h);

    float cos_theta = glm::max(glm::dot(incoming, normal), 0.0f);
    

    // vec4 R = schlik_approximation(F0, incoming, normal);
    float R = F0 + (1.0f - F0) * pow(1.0f - cos_theta, 5.0f);

//  Beckmann
    // float NdotH = glm::max(dot(normal, h), 0.0f);
    // // float NdotH = dot(normal, h);
    // float alpha = roughness * roughness;
    // float cos2Theta = NdotH * NdotH;
    // float tan2Theta = (1.0f - cos2Theta) / cos2Theta;
    // float numerator = exp(-tan2Theta / alpha);
    // float denominator = M_PI_2f32 * alpha * cos2Theta * cos2Theta;
    // float D =  numerator / denominator;
    // // if (NdotH == 0.0f)
    // //     D= 0.0f;
    // cout << NdotH <<'\n';

// // Blinn-Phong
//     float D = pow(glm::max(0.0f,dot(h, normal)), 50.5f);
    
// Rough    
    float D = 1.0f;

// // Perfect Mirror
//     float D = glm::max(0.0f,dot(h, normal)) >= 0.99f ? 1000.0f : 0.0f;
    
    float NdotV = glm::max(dot(normal, incoming), 0.0f);
    float NdotL = glm::max(dot(normal, outgoing), 0.0f);
    float k = (roughness * sqrt(2.0f / M_PI)) / (NdotV + sqrt(roughness * roughness + (1.0f - roughness * roughness) * NdotV * NdotV));
    float kL = (roughness * sqrt(2.0f / M_PI)) / (NdotL + sqrt(roughness * roughness + (1.0f - roughness * roughness) * NdotL * NdotL));
    float G =  glm::min(abs(k * kL), 1.0f);

    vec4 color = vec4(1.0f);
    color *= R;
    // color *= G;
    color *= D;
    color *= 1.0f/(dot(normal, incoming) * dot(normal, outgoing) * 4.0f); 
    return color;
}

Glass::Glass(vec4 color, float ks, float kt){
    albedo = color;
    reflectance = schlik_dielectric;
    transmitance = iso_white;
    isEmissive = false;
    isDiffuse = false;
    isReflective = true;
    isTransparent = true;
    this->ks = ks;
    this->kt = kt;
    mu = 1.55f;
}