#include "Scene.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


Shape::Shape(){
    material = new Material();
    transformation_mat = mat4(1.0f);
}

void Shape::scaling(vec3 s){
    cout << "Before scaling => \n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout << transformation_mat[i][j] << " ";
        }
        cout << endl;
    }

    transformation_mat = scale(transformation_mat, s);

    cout << "After scaling => \n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout << transformation_mat[i][j] << " ";
        }
        cout << endl;
    }
}

void Shape::translation(vec3 t){

    cout << "Before translation => \n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout << transformation_mat[i][j] << " ";
        }
        cout << endl;
    }

    transformation_mat = translate(transformation_mat, t);

    cout << "After translation => \n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout << transformation_mat[i][j] << " ";
        }
        cout << endl;
    }

}

void Shape::rotation(float radian, vec3 axis){
    std::cout << "Before rotation: " << std::endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            std::cout << transformation_mat[i][j] << " ";
        }
        std::cout << std::endl;
    }

    transformation_mat = rotate(transformation_mat, radians(radian), axis);

    std::cout << "After rotation: " << std::endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            std::cout << transformation_mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Shape::invert_transformation(){
    // print before inversion
    std::cout << "Before inversion: " << std::endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            std::cout << transformation_mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    mat4 tmp = inverse(transformation_mat);
    transformation_mat = tmp;
    // transformation_mat = inverse(transformation_mat);

    // print after inversion
    std::cout << "After inversion: " << std::endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            std::cout << transformation_mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Sphere::Sphere(const float &r, const vec4 &c) : radius(r), centre(c) {

}

pair<Ray*, vec4> Sphere::hit(Ray *ray) {
    
    // mat4 world_to_object = inverse(transformation_mat);
    mat4 world_to_object = transformation_mat;

    //print the world_to_object mat4
    // cout << "World to object matrix: \n";
    // for (int i = 0; i<4; i++){
    //     for (int j = 0; j<4; j++){
    //         std::cout << world_to_object[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    ray->o = world_to_object * ray->o;
    ray->d = world_to_object * ray->d;

    float x1 = dot(ray->d, ray->o - centre);
    float norm_d_sq = length(ray->d);
    norm_d_sq *= norm_d_sq;
    float x2 = dot(ray->o - centre, ray->o - centre) - radius * radius;

    if (x1 * x1 - norm_d_sq * x2 < 0){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    float D = sqrt(x1 * x1 - norm_d_sq * x2);
    float t1 = -x1 - D;

    t1 /= norm_d_sq;

    float t2 = -x1 + D;
    t2 /= norm_d_sq;

    if(t2 <= ray->t_near){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
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

vec4 Sphere::normal_ray(vec4 position){
    return position - centre;
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
    ref_ray->d = normal;
    ref_ray->d *= 2*dot(normal,d);
    ref_ray->d = d - ref_ray->d;
    return {ref_ray, normal};
}

Plane::Plane(const vec4 &normal, const vec4 &point_on_plane) : normal(normal), point_on_plane(point_on_plane) {

}

pair<Ray*, vec4> Plane::hit(Ray *ray){

    mat4 world_to_object = inverse(transformation_mat);
    ray->o = world_to_object * ray->o;
    ray->d = world_to_object * ray->d;

    // t = (n · (p0 − o))/(n · d) 
    float t = float(dot(normal, point_on_plane - ray->o)) / dot(normal, ray->d);
    if (t <= ray->t_near){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else{
        ray->t = t;
        return reflected_ray(ray, t);
    }
}

vec4 Plane::normal_ray(vec4 position){
    return normal;
}

pair<Ray*, vec4> Plane::reflected_ray(Ray* ray, float t){
    Ray* ref_ray = new Ray();
    ref_ray->o = ray->o + t * ray->d;
    ref_ray->t_near = ray->t_near;
    ref_ray->t_far = ray->t_far;
    vec4 d = ray->d / length(ray->d);
    vec4 n = normal / length(normal);
    ref_ray->d = n;
    ref_ray->d *= 2*dot(n,d);
    ref_ray->d = d - ref_ray->d; 
    return {ref_ray, normal};
}


Bounding_Box::Bounding_Box(const vec4 &min, const vec4 &max) : min(min), max(max) {

}

pair<Ray*, vec4> Bounding_Box::hit(Ray *ray){
    // txmin = (xmin − ox)/dx

    float txmin = (min.x - ray->o.x) / ray->d.x;
    float txmax = (max.x - ray->o.x) / ray->d.x;
    // Swap the bounds first if dx < 0
    if (ray->d.x < 0){
        float temp = txmin;
        txmin = txmax;
        txmax = temp;
    }

    // cout << txmin << " " << txmax << endl;

    float tymin = (min.y - ray->o.y) / ray->d.y;
    float tymax = (max.y - ray->o.y) / ray->d.y;
    // Swap the bounds first if dy < 0
    if (ray->d.y < 0){
        float temp = tymin;
        tymin = tymax;
        tymax = temp;
    }

    // cout << tymin << " " << tymax << endl;

    float tzmin = (min.z - ray->o.z) / ray->d.z;
    float tzmax = (max.z - ray->o.z) / ray->d.z;
    // Swap the bounds first if dz < 0
    if (ray->d.z < 0){
        float temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }

    // cout << tzmin << " " << tzmax << endl;

    float tmin = std::max(std::max(txmin, tymin), tzmin);
    float tmax = std::min(std::min(txmax, tymax), tzmax);
    int min_plane;
    int max_plane;

    if (tmin == txmin){
        min_plane = 0;
    }
    else if (tmin == tymin){
        min_plane = 1;
    }
    else{
        min_plane = 2;
    }

    if (tmax == txmax){
        max_plane = 0;
    }
    else if (tmax == tymax){
        max_plane = 1;
    }
    else{
        max_plane = 2;
    }

    // cout << tmin << " " << tmax << " " << min_plane << " " << max_plane << "\n";

    if (tmin > tmax){
        ray->t = INT32_MAX;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else{
        ray->t = tmin;
        return reflected_ray(ray, tmin, min_plane);
    }
}

pair<Ray*, vec4> Bounding_Box::reflected_ray(Ray* ray, float t, int min_plane){
    vec4 normal;
    if (min_plane == 0){
        if (ray->d.x > 0){
            normal = vec4(-1.0f, 0.0f, 0.0f, 0.0f);
        }
        else{
            normal = vec4(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    else if (min_plane == 1){
        if (ray->d.y > 0){
            normal = vec4(0.0f, -1.0f, 0.0f, 0.0f);
        }
        else{
            normal = vec4(0.0f, 1.0f, 0.0f, 0.0f);
        }
    }
    else{
        if (ray->d.z > 0){
            normal = vec4(0.0f, 0.0f, -1.0f, 0.0f);
        }
        else{
            normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
        }
    }

    Ray* ref_ray = new Ray();
    ref_ray->o = ray->o + t * ray->d;
    ref_ray->t_near = ray->t_near;
    ref_ray->t_far = ray->t_far;
    vec4 d = ray->d / length(ray->d);
    vec4 n = normal / length(normal);
    ref_ray->d = n;
    ref_ray->d *= 2*dot(n,d);
    ref_ray->d = d - ref_ray->d; 
    return {ref_ray, normal};
}

vec4 Bounding_Box::normal_ray(vec4 position){
    //TODO:
    // vec4 normal;
    // if (min_plane == 0){
    //     if (ray->d.x > 0){
    //         normal = vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    //     }
    //     else{
    //         normal = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    //     }
    // }
    // else if (min_plane == 1){
    //     if (ray->d.y > 0){
    //         normal = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    //     }
    //     else{
    //         normal = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    //     }
    // }
    // else{
    //     if (ray->d.z > 0){
    //         normal = vec4(0.0f, 0.0f, -1.0f, 0.0f);
    //     }
    //     else{
    //         normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    //     }
    // }
    // return normal;

    return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}