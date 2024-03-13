#include "Scene.hpp"
#include <iostream>

Material* Shape::get_material(){
    return material;
}

Shape::Shape(){

}

Sphere::Sphere(const float &r, const vec4 &c) : radius(r), centre(c) {

}

pair<Ray*, vec4> Sphere::hit(Ray *ray) {

    // cout << ray->o.x << " " << ray->o.y << " " << ray->o.z << " " << ray->o.w << "\n";
    // // cout << ray->d.x << " " << ray->d.y << " " << ray->d.z << " " << ray->d.w << "\n";
    // cout << centre.x << " " << centre.y << " " << centre.z << " " << centre.w << "\n";
    
    float x1 = dot(ray->d, ray->o - centre);
    float norm_d_sq = length(ray->d);
    norm_d_sq *= norm_d_sq;
    float x2 = dot(ray->o - centre, ray->o - centre) - radius * radius;

    if (x1 * x1 - norm_d_sq * x2 < 0){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0,0,0,0)};
    }
    float D = sqrt(x1 * x1 - norm_d_sq * x2);
    float t1 = -x1 - D;

    // std::cout << norm_d_sq << "\n";
    t1 /= norm_d_sq;

    float t2 = -x1 + D;
    t2 /= norm_d_sq;

    // std::cout << t1 << " " << t2 << "\n";

    if(t2 <= ray->t_near){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0,0,0,0)};
    }
    else if(t1 <= ray->t_near){
        ray->t = t2;
        return reflected_ray(ray, t2);
    }
    else{
        ray->t = t1;
        return reflected_ray(ray, t1);
    }
}

pair<Ray*, vec4> Sphere::reflected_ray(Ray* ray, float t){
    vec4 normal = ray->o + t * ray->d - centre;
    float norm = length(normal);
    normal /= norm;
    return {new Ray(), normal};
}