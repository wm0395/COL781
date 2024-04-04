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

pair<vec4, float> Shape::refracted_ray(vec4 incidence, vec4 position, vec4 normal, float n1, float n2){
    incidence = normalize(incidence);
    // vec4 normal = normal_ray(position);
    normal = normalize(normal);
    // float cos_theta1 = glm::max(dot(-incidence, normal), 0.0f);
    float cos_theta1 = abs(dot(-incidence, normal));
    // cout << cos_theta1 << "\n";
    // cout << n1 << " " << n2 << "\n";
    float n = n1/n2;
    // cout << n << "\n"
    float f = 1.0f - n*n*(1.0f - cos_theta1*cos_theta1);
    // cout << f << "\n";
    vec4 ref_d = vec4(0.0f);
    if (f < 0){  // case of TIR
        // return the reflected direction
        ref_d = normal;
        ref_d*= 2*dot(normal,incidence);
        ref_d = incidence - ref_d;
        return {ref_d, 1.0f};
    }
    // F = F0 + (1 - F0)(1 - cos_theta)^5
    float ratio = (n1 - n2)/(n1 + n2);
    float f0 = ratio * ratio;
    float cos_theta2 = sqrt(f);
    
    // float F = (f0 + (1 - f0) * pow(1 - std::min(cos_theta1, cos_theta2), 5.0f));
    float F = f0 + (1 - f0) * pow(1 - cos_theta1, 5);
    // cout << F << "\n";
    ref_d = n * incidence + (n * cos_theta1 - cos_theta2) * normal;
    return {ref_d, F};
}

Sphere::Sphere(const float &r, const vec4 &c) : radius(r), centre(c) {

}

pair<Ray*, vec4> Sphere::hit(Ray *ray) {
    
    mat4 world_to_object = transformation_mat;
    Ray *hit_ray = new Ray();
    hit_ray->o = world_to_object * ray->o;
    hit_ray->d = world_to_object * ray->d;
    hit_ray->t = ray->t;
    hit_ray->t_near = 0.0f;
    hit_ray->t_far = 1000.0f;

    // cout << radius << " " << centre.x << " " << centre.y << " " << centre.z << "\n";
    
    float x1 = dot(hit_ray->d, hit_ray->o - centre);
    float norm_d_sq = length(ray->d);
    norm_d_sq *= norm_d_sq;
    float x2 = dot(hit_ray->o - centre, hit_ray->o - centre) - radius * radius;

    if (x1 * x1 - norm_d_sq * x2 < 0){
        hit_ray->t = hit_ray->t_far;
        // cout << ray->t << "\n";
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    float D = sqrt(x1 * x1 - norm_d_sq * x2);
    float t1 = -x1 - D;

    t1 /= norm_d_sq;

    float t2 = -x1 + D;
    t2 /= norm_d_sq;

    // cout << t1 << " " << t2 << "\n";

    if(t2 <= hit_ray->t_near){
        ray->t = ray->t_far;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else if(t1 <= hit_ray->t_near){
        ray->t = t2;
        return reflected_ray(hit_ray, t2);
    }
    else{
        ray->t = t1;
        return reflected_ray(hit_ray, t1);
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
    ref_ray->d = normal;
    ref_ray->d *= 2*dot(normal,d);
    ref_ray->d = d - ref_ray->d;
    normal = transpose(transformation_mat) * normal;
    return {ref_ray, normal};
}

vec4 Sphere::normal_ray(vec4 position){
    vec4 normal = position - centre;
    normal = normal / length(normal);
    normal = transpose(transformation_mat) * normal;
    return normal;
}

Plane::Plane(const vec4 &normal, const vec4 &point_on_plane) : normal(normal), point_on_plane(point_on_plane) {

}

pair<Ray*, vec4> Plane::hit(Ray *ray){

    mat4 world_to_object = transformation_mat;
    Ray *hit_ray = new Ray();
    hit_ray->o = world_to_object * ray->o;
    hit_ray->d = world_to_object * ray->d;

    // t = (n · (p0 − o))/(n · d) 
    float t = float(dot(normal, point_on_plane - hit_ray->o)) / dot(normal, hit_ray->d);
    if (t <= hit_ray->t_near){
        ray->t = ray->t_far;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else{
        ray->t = t;
        return reflected_ray(hit_ray, t);
    }
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
    normal = transpose(transformation_mat) * normal;
    return {ref_ray, normal};
}

vec4 Plane::normal_ray(vec4 position){
    normal = normal / length(normal);
    normal = transpose(transformation_mat) * normal;
    return normal;
}


Axis_Aligned_Box::Axis_Aligned_Box(const vec4 &min, const vec4 &max) : min(min), max(max) {

}

pair<Ray*, vec4> Axis_Aligned_Box::hit(Ray *ray){

    mat4 world_to_object = transformation_mat;
    ray->o = world_to_object * ray->o;
    ray->d = world_to_object * ray->d;

    // txmin = (xmin − ox)/dx

    float txmin = (min.x - ray->o.x) / ray->d.x;
    float txmax = (max.x - ray->o.x) / ray->d.x;
    if (txmin > txmax) swap(txmin, txmax);
    // // Swap the bounds first if dx < 0
    // if (ray->d.x < 0){
    //     float temp = txmin;
    //     txmin = txmax;
    //     txmax = temp;
    // }

    float tymin = (min.y - ray->o.y) / ray->d.y;
    float tymax = (max.y - ray->o.y) / ray->d.y;
    if (tymin > tymax) swap(tymin, tymax);
    // Swap the bounds first if dy < 0
    // if (ray->d.y < 0){
    //     float temp = tymin;
    //     tymin = tymax;
    //     tymax = temp;
    // }

    float tzmin = (min.z - ray->o.z) / ray->d.z;
    float tzmax = (max.z - ray->o.z) / ray->d.z;
    if (tzmin > tzmax) swap(tzmin, tzmax);
    // Swap the bounds first if dz < 0
    // if (ray->d.z < 0){
    //     float temp = tzmin;
    //     tzmin = tzmax;
    //     tzmax = temp;
    // }

    // cout << txmin << " " << tymin << " " << tzmin << "\n";
    // cout << txmax << " " << tymax << " " << tzmax << "\n";

    float tmin = std::max(std::max(txmin, tymin), tzmin);
    float tmax = std::min(std::min(txmax, tymax), tzmax);

    // cout << tmin << " " << tmax << "\n";
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

    if (tmin > tmax){
        ray->t = ray->t_far;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else if (tmin < ray->t_near && tmax < ray->t_near){
        ray->t = ray->t_far;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else if (tmin < ray->t_near && tmax > ray->t_near){
        ray->t = tmax;
        return reflected_ray(ray, tmax, max_plane);
    }
    else{
        ray->t = tmin;
        return reflected_ray(ray, tmin, min_plane);
    }
}

pair<Ray*, vec4> Axis_Aligned_Box::reflected_ray(Ray* ray, float t, int min_plane){
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
    normal = transpose(transformation_mat) * normal;
    return {ref_ray, normal};
}

vec4 Axis_Aligned_Box::normal_ray(vec4 position){   // TODO: Yeh gadbad hai abhi
    //firstly find out the face on which the vec4 position is present

    vec4 normal;
    if (position.x == min.x){
        normal = vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    }
    else if (position.x == max.x){
        normal = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    }
    else if (position.y == min.y){
        normal = vec4(0.0f, -1.0f, 0.0f, 0.0f);
    }
    else if (position.y == max.y){
        normal = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    else if (position.z == min.z){
        normal = vec4(0.0f, 0.0f, -1.0f, 0.0f);
    }
    else{
        normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    }
    normal = normal / length(normal);
    normal = transpose(transformation_mat) * normal;
    return normal;
}

Triangle::Triangle(const vec4 &p0, const vec4 &p1, const vec4 &p2) : p0(p0), p1(p1), p2(p2) {

}

pair<Ray*, vec4> Triangle::hit(Ray *ray){
    mat4 world_to_object = inverse(transformation_mat);
    ray->o = world_to_object * ray->o;
    ray->d = world_to_object * ray->d;

    vec4 v1 = ray->o - p0;
    mat4 A = transpose(mat4(-ray->d, p1 - p0, p2 - p0, vec4(0.0f, 0.0f, 0.0f, 1.0f)));
    // A = transpose(A);
    vec4 soln = v1 * inverse(A);
    float t = soln.x;
    float b1 = soln.y;
    float b2 = soln.z;

    if (b1<=0 || b2<=0 || b1+b2>=1 || t<=ray->t_near){
        ray->t = ray->t_far;
        return {nullptr, vec4(0.0f,0.0f,0.0f,0.0f)};
    }
    else{
        ray->t = t;
        return reflected_ray(ray, t);
    }
}

pair<Ray*, vec4> Triangle::reflected_ray(Ray* ray, float t){
    vec4 v1 = p1-p0;
    vec4 v2 = p2-p0;
    vec3 x1 = vec3(v1.x, v1.y, v1.z);
    vec3 x2 = vec3(v2.x, v2.y, v2.z);
    vec3 n1 = cross(x1, x2);
    vec4 normal = vec4(n1.x, n1.y, n1.z, 0.0f);
    normal = normal / length(normal);

    Ray* ref_ray = new Ray();
    ref_ray->o = ray->o + t * ray->d;
    ref_ray->t_near = ray->t_near;
    ref_ray->t_far = ray->t_far;
    vec4 d = ray->d / length(ray->d);
    vec4 n = normal / length(normal);
    ref_ray->d = n;
    ref_ray->d *= 2*dot(n,d);
    ref_ray->d = d - ref_ray->d; 
    normal = transpose(transformation_mat) * normal;
    return {ref_ray, normal};
}

vec4 Triangle::normal_ray(vec4 position){
    vec4 v1 = p1-p0;
    vec4 v2 = p2-p0;
    vec3 x1 = vec3(v1.x, v1.y, v1.z);
    vec3 x2 = vec3(v2.x, v2.y, v2.z);
    vec3 n1 = cross(x1, x2);
    vec4 normal = vec4(n1.x, n1.y, n1.z, 0.0f);
    normal = normal / length(normal);
    normal = transpose(transformation_mat) * normal;
    return normal;
}