#include "Scene.hpp"

Material* Shape::get_material(){
    return material;
}

// Sphere::Sphere(const float &r, const vec4 &c) : radius(r), centre(c) {

// }

pair<Ray*, vec4> Sphere::hit(Ray *ray) {
    float x1 = dot(ray->d, ray->o - centre);
    float norm_d_sq = dot(ray->d, ray->d);
    float x2 = dot(ray->o - centre, ray->o - centre) - radius * radius;

    float D = sqrt(x1 * x1 - norm_d_sq * x2);
    float t1 = -x1 - D;
    t1 /= norm_d_sq;

    float t2 = -x1 + D;
    t2 /= norm_d_sq;

    if(t2 <= ray->t_near){
        return {nullptr, vec4(0,0,0,0)};
    }
    else if(t1 <= ray->t_near){
        return reflected_ray(ray, t2);
    }
    else{
        return reflected_ray(ray, t1);
    }
}

pair<Ray*, vec4> Sphere::reflected_ray(Ray* ray, float t){
    vec4 normal = ray->o + t * ray->d - centre;
    float norm = length(normal);
    normal /= (norm * norm);
    return {nullptr, normal};
    return 
}