#include "Scene.hpp"

Renderer::Renderer(int bounces, int paths, int samples, string sampling) : MAX_BOUNCES(bounces), PATHS(paths), SAMPLES(samples), SAMPLING(sampling){

}

vec4 Renderer::rand_hemisphere(){
    float e1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float e2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return vec4(sqrt(1-e1*e1)*cos(2*M_PI*e2), sqrt(1-e1*e1)*sin(2*M_PI*e2), e1, 0.0f);
}

pair<int, vec4> Renderer::incident_ray(vec4 position, vec4 direction){
    Ray *ray = new Ray();
    ray->o = position;
    ray->d = direction;
    ray->t_near = 0.01f;
    ray->t_far = 1000.0f;
    float t = INT32_MAX;
    int hit_id = -1;
    for(int i = 0; i < scene->objects.size(); i++){
        ray->t = 0;
        pair<Ray*, vec4> hit = scene->objects[i]->hit(ray);
        if(ray->t > 0 && ray->t < t){
            t = ray->t;
            hit_id = i;
        }
    }
    vec4 p = position + ray->t*ray->d;
    return {hit_id, p};
}

int Renderer::shadow_ray(int light_id, vec4 position){
    Ray *ray = new Ray();
    ray->o = scene->lights[light_id]->position;
    ray->d = position - scene->lights[light_id]->position;
    ray->t_near = -0.01f;
    ray->t_far = 1000.0f;
    // int bias = 0.0
    float t = INT32_MAX;
    int hit_id = -1;
    for(int i = 0; i < scene->objects.size(); i++){
        ray->t = 0;
        pair<Ray*, vec4> hit = scene->objects[i]->hit(ray);
        if(ray->t > 0 && ray->t < t){
            t = ray->t;
            hit_id = i;
        }
    }
    return hit_id;
}

vec4 Renderer::point_lambert(Ray *ray){
    pair<int, vec4> hit = incident_ray(ray->o, ray->d);
    // pair<*Ray, vec4> reflect = scene->objects[hit.first]->hit(ray);
    if(hit.first == -1) return scene->sky;
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    int N = 0;
    for(int i = 0; i < scene->lights.size(); i++){
        if(shadow_ray(i, hit.second) != hit.first) continue;

        N++;
        vec4 irradiace = scene->lights[i]->Intensity;
        irradiace /= (4*M_PI*glm::length(scene->lights[i]->position - hit.second));
        color += scene->objects[hit.first]->material->albedo * irradiace;
    }
    if(N == 0)
    return color;
    color *= (2/N);
    return color;
}

vec4 Renderer::normal_map(Ray *ray){
    for(int i = 0; i < scene->objects.size(); i++){
        ray->t = 0;
        pair<Ray*, vec4> hit = scene->objects[i]->hit(ray);
        // std::cout << ray->t << "\n";
        if(ray->t > 0 && ray->t < t){
            t = ray->t;
            // color = get_color();
            vec4 normal = hit.second;
            color = (normal + vec4(1.0f, 1.0f, 1.0f, 1.0f));    // if hit
            color /= 2;
        }
    }
}

vec4 Renderer::MC_Sampling(int obj_id, vec4 position, vec4 out_dir, int depth){
    vec4 F = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i < SAMPLES; i++){
        vec4 rand_dir = rand_hemisphere();
        pair<int, vec4> hit = incident_ray(position, rand_dir);
        int hit_id = hit.first; 
        vec4 p = hit.second;
        F += path_trace(i, p, rand_dir, depth+1);
    }
}

vec4 Renderer::path_trace(int obj_id, vec4 position, vec4 out_dir, int depth){
    vec4 color = scene->objects[obj_id]->material->emmission(position, -out_dir);

}

vec4 Renderer::render(Ray* ray){
    if(SAMPLING.compare("normal_map"))
        return normal_map(ray);
    else if(SAMPLING.compare("point_lambert"))
        return point_lambert(ray);
}