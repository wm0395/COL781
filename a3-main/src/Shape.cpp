#include "Scene.hpp"
#include <iostream>


Shape::Shape(){
    
}

Sphere::Sphere(const float &r, const vec4 &c) : radius(r), centre(c) {

}

pair<Ray*, vec4> Sphere::hit(Ray *ray) {
    
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

    t1 /= norm_d_sq;

    float t2 = -x1 + D;
    t2 /= norm_d_sq;

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

    Ray* ref_ray = new Ray();
    ref_ray->o = ray->o + t * ray->d;
    ref_ray->t_near = ray->t_near;
    ref_ray->t_far = ray->t_far;
    vec4 d = ray->d / length(ray->d);
    ref_ray->d = d - 2*dot(normal,d)*normal;
    return {new Ray(), normal};
}

Plane::Plane(const vec4 &normal, const vec4 &point_on_plane) : normal(normal), point_on_plane(point_on_plane) {

}

pair<Ray*, vec4> Plane::hit(Ray *ray){
    // t = (n · (p0 − o))/(n · d) 
    float t = float(dot(normal, point_on_plane - ray->o)) / dot(normal, ray->d);
    if (t <= ray->t_near){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0,0,0,0)};
    }
    else{
        ray->t = t;
        return reflected_ray(ray, t);
    }
}

pair<Ray*, vec4> Plane::reflected_ray(Ray* ray, float t){
    Ray* ref_ray = new Ray();
    ref_ray->o = ray->o + t * ray->d;
    ref_ray->t_near = ray->t_near;
    ref_ray->t_far = ray->t_far;
    vec4 d = ray->d / length(ray->d);
    vec4 n = normal / length(normal);
    ref_ray->d = d - 2*dot(n,d)*n;
    return {ref_ray, normal};
}