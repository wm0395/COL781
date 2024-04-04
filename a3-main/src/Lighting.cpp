#include "Scene.hpp"

Renderer::Renderer(int bounces, int paths, int samples, string sampling) : MAX_BOUNCES(bounces), PATHS(paths), SAMPLES(samples), SAMPLING(sampling){

}


vec4 Renderer::rand_hemisphere(){
    float e1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float e2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return vec4(sqrt(1-e1*e1)*cos(2*M_PI*e2), sqrt(1-e1*e1)*sin(2*M_PI*e2), e1, 0.0f);
}

vec4 Renderer::cos_hemisphere(){
    float e1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float e2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // float theta = acos(sqrt(e1));
    // float phi = 2 * M_1_PIf32 * e2;
 
    // float x = cos(phi) * sin(theta);
    // float y = sin(phi) * sin(theta);
    // float z = cos(theta);

    float x = sqrt(e1) * cos(2.0f * M_1_PIf32 * e2);
    float y = sqrt(e1) * sin(2.0f * M_1_PIf32 * e2);
    float z = sqrt(1.0f - e1);
 
    return vec4(x, y, z, 0.0f);
}


pair<int, vec4> Renderer::incident_ray(vec4 position, vec4 direction){
    Ray *ray = new Ray();
    ray->o = position;
    ray->d = normalize( direction);
    // ray->t_near = 0.0f;
    // ray->t_far = -scene->camera->far_plane;
    float t = -scene->camera->far_plane;
    int hit_id = -1;
    for(int i = 0; i < scene->objects.size(); i++){
        // cout << i << "\n";
        ray->t = 0.0f;
        pair<Ray*, vec4> hit = scene->objects[i]->hit(ray);
        // cout << ray->t << "\n";
        if(ray->t > ray->t_near && ray->t < t){
            t = ray->t;
            hit_id = i;
        }
    }
    vec4 p = position + t * ray->d;
    // cout << hit_id << "\n";
    return {hit_id, p};
}

int Renderer::shadow_ray(int light_id, vec4 position){
    Ray *ray = new Ray();
    ray->o = scene->lights[light_id]->position;
    ray->d = normalize(position - scene->lights[light_id]->position);
    // ray->t_near = 0.0f;
    // ray->t_far = scene->camera->far_plane;
    // int bias = 0.0
    float t = INT32_MAX;
    int hit_id = -1;
    for(int i = 0; i < scene->objects.size(); i++){
        ray->t = 0.0f;
        pair<Ray*, vec4> hit = scene->objects[i]->hit(ray);
        if(ray->t > ray->t_near && ray->t < t && ray->t < INT32_MAX){
            t = ray->t;
            hit_id = i;
        }
    }
    vec4 p = ray->o + t * ray->d;
    if(dot(p- position, p- position) > 0.00001f)
        hit_id = -1;
    return hit_id;
}

vec4 Renderer::point_lambert(Ray *ray){
    pair<int, vec4> hit = incident_ray(ray->o, ray->d);
    // pair<*Ray, vec4> reflect = scene->objects[hit.first]->hit(ray);
    if(hit.first == -1) return scene->sky;
    
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    if(scene->objects[hit.first]->material->emmission){
        // color += scene->objects[hit.first]->material->emmission(ray->o, ray->d, scene->objects[hit.first]->normal_ray(hit.second));
        color += scene->objects[hit.first]->material->emmission(ray->o, ray->d, scene->objects[hit.first]->hit(ray).second);   //TODO: check if correct
    }
    
    float N = 0.0f;
    
    vec4 intensity = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for(int i = 0; i < scene->lights.size(); i++){
        if(shadow_ray(i, hit.second) != hit.first){ 
            // cout<<hit.first<<"Ray Blocked\n";
            continue;
        }

        N+= 1.0f;
        vec4 irradiace = scene->lights[i]->Intensity;
        float fall_off = 4.0f*(float)M_PI*glm::dot(scene->lights[i]->position - hit.second, scene->lights[i]->position - hit.second);
        vec4 out = scene->lights[i]->position - hit.second;
        vec4 normal = scene->objects[hit.first]->hit(ray).second;   //TODO: check if correct
        float out_norm = glm::length(out);
        float normal_norm = glm::length(normal);
        float cos_theta = glm::dot(out, normal)/ (out_norm * normal_norm);
        // cout << cos_theta <<"\n";
        // float cos_theta = glm::dot(hit.second - scene->lights[i]->position, -(ray->d))/ (sqrt(glm::dot(hit.second - scene->lights[i]->position, hit.second - scene->lights[i]->position)) * sqrt(glm::dot(ray->d, ray->d)));
        // float cos_theta = 1.0f;
        irradiace *= (cos_theta/ fall_off);
        irradiace.w /= (cos_theta/ fall_off);
        intensity += irradiace;
    }
    
    // intensity = vec4(glm::min(1.0f, intensity.x), glm::min(1.0f, intensity.y), glm::min(1.0f, intensity.z), glm::min(1.0f, intensity.w));

    if(N == 0.0f){
        // cout<< hit.first <<"no light intersect\n";
        return color;
    }

    intensity = vec4(pow(intensity.x, 1.0f/2.2), pow(intensity.y, 1.0f/2.2), pow(intensity.z, 1.0f/2.2), pow(intensity.w, 1.0f/2.2));
    // intensity *= (2.0f/pow(N,1.0f/4.0f));
    vec4 temp = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // if(scene->objects[hit.first]->material->diffuse)
    //     temp = scene->objects[hit.first]->material->diffuse(ray->o, ray->d);
    color = color + ((scene->objects[hit.first]->material->albedo * intensity));
    
    color = vec4(glm::min(1.0f, color.x), glm::min(1.0f, color.y), glm::min(1.0f, color.z), glm::min(1.0f, color.w));
    return color;
}

vec4 Renderer::normal_map(Ray *ray){
    pair<int, vec4> i_ray = incident_ray(ray->o, ray->d);
    // cout << i_ray.first << "\n";
    if(i_ray.first == -1) return vec4(0.0f, 0.0f, 0.0f, 0.0f);

    pair<Ray*, vec4> hit = scene->objects[i_ray.first]->hit(ray);
    vec4 normal = hit.second;
    vec4 color = (normal + vec4(1.0f, 1.0f, 1.0f, 1.0f));    // if hit
    color /= 2;
    return color;
}

vec4 Renderer::MC_Sampling(int obj_id, vec4 position, vec4 out_dir, int depth){
    
    vec4 F = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    Ray* ray = new Ray();
    ray->o = position;
    ray->d = out_dir;
    // ray->t_far = scene->camera->far_plane;
    
    vec4 normal = scene->objects[obj_id]->normal_ray(position);   //TODO: check if correct
    normal = normalize(normal);
    float normal_norm = glm::length(normal);
    mat4 Tf;
    if(scene->objects[obj_id]->material->isDiffuse){
        vec3 z = normalize(vec3(normal.x, normal.y, normal.z));
        vec3 x = normalize(glm::cross(vec3(out_dir.x, out_dir.y, out_dir.z), z));
        vec3 y = normalize(glm::cross(z, x));
        Tf = mat4(vec4(x.x, x.y, x.z, 0.0f),vec4(y.x, y.y, y.z, 0.0f),vec4(z.x, z.y, z.z, 0.0f),vec4(0.0f, 0.0f, 0.0f, 1.0f));
        // Tf = transpose(Tf);
        // Tf = inverse(Tf);
    }
    
    vec4 D = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 R = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 T = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    float e = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    for(int i = 0; i < PATHS && e <= ((MAX_BOUNCES - 1.0f)/MAX_BOUNCES) && depth <= 20; i++){
        // cout << depth << "\n";
        e = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        if(scene->objects[obj_id]->material->isDiffuse){
            
            // Uniform Hemisphere
            vec4 rand_dir = rand_hemisphere();
            rand_dir = Tf * rand_dir;

            // // Cos weighted Hemisphere
            // vec4 rand_dir = cos_hemisphere();
            // // rand_dir = Tf * rand_dir;

            pair<int, vec4> hit = incident_ray(position, rand_dir);
            if(hit.first == -1){
                D += scene->sky;
            }
            else{
                Ray *branch_ray = new Ray();
                branch_ray->o = position ;//+ 0.000001f * normal;
                branch_ray->d = normalize(rand_dir);
                branch_ray->t = 0.0f;
                pair<Ray*, vec4> hit_out = scene->objects[hit.first]->hit(branch_ray);

                float fall_off = 4.0f*(float)M_PI*glm::dot(hit.second - position, hit.second - position);
                
                
                vec4 irradiance = MC_Sampling(hit.first, hit.second, -rand_dir, depth+1);
                // Light Sampling
                if(scene->objects[hit.first]->material->isEmissive){
                    // irradiance += scene->objects[hit.first]->material->emmission(hit.second, rand_dir, rand_dir) * scene->objects[hit.first]->material->ke;
                    irradiance += scene->objects[hit.first]->material->albedo * scene->objects[hit.first]->material->emmission(hit.second, rand_dir, rand_dir) * scene->objects[hit.first]->material->ke;
                }
                
                // Not needed for Cos sampling
                float out_norm = glm::length(rand_dir);
                float cos_theta = glm::dot(rand_dir, normal)/ (out_norm * normal_norm);
                irradiance *= (cos_theta/ fall_off);
                irradiance.w /= (cos_theta/ fall_off);

                D += scene->objects[obj_id]->material->diffuse(out_dir, rand_dir, normal) * irradiance * scene->objects[obj_id]->material->kd;// * (M_1_PIf32 / rand_dir.z);
            }
        }

        if(scene->objects[obj_id]->material->isReflective && !scene->objects[obj_id]->material->isTransparent){
            vec4 reflect = -2.0f * dot(-out_dir, normal) * normal - out_dir;
            
            pair<int, vec4> hit = incident_ray(position, reflect);
            if(hit.first == -1){
                R += scene->sky;
            }
            else{ 
                Ray *branch_ray = new Ray();
                branch_ray->o = position ;//+ 0.000001f * normal;
                branch_ray->d = normalize(reflect);
                branch_ray->t = 0.0f;
                pair<Ray*, vec4> hit_out = scene->objects[hit.first]->hit(branch_ray);

                float fall_off = 4.0f*(float)M_PI*glm::dot(hit.second - position, hit.second - position);
                
                
                vec4 irradiance = MC_Sampling(hit.first, hit.second, -reflect, depth);
                // Light Sampling
                if(scene->objects[hit.first]->material->isEmissive){
                    irradiance += scene->objects[hit.first]->material->albedo * scene->objects[hit.first]->material->emmission(hit.second, reflect, reflect) * scene->objects[hit.first]->material->ke;
                }
                
                // Not needed for Cos sampling
                float out_norm = glm::length(reflect);
                float cos_theta = glm::dot(reflect, normal)/ (out_norm * normal_norm);
                irradiance *= (cos_theta/ fall_off);
                irradiance.w /= (cos_theta/ fall_off);

                R += scene->objects[obj_id]->material->reflectance(out_dir, reflect, normal) * irradiance * scene->objects[obj_id]->material->ks;
            }
        }

        if(scene->objects[obj_id]->material->isTransparent){
            pair<vec4, float> refrac = scene->objects[obj_id]->refracted_ray(-out_dir, position , normal, scene->mu, scene->objects[obj_id]->material->mu);
            pair<int, vec4> refrac_hit = incident_ray(position, refrac.first);

            if(refrac_hit.first != obj_id){ // Another object inside volume
                vec4 irradiance = vec4(0.0f);
                if(refrac_hit.first == -1){
                    irradiance = scene->sky;
                }
                else{
                    Ray *trans_ray = new Ray();
                    trans_ray->o = position - 0.0000001f * normal;
                    trans_ray->d = normalize(refrac.first);
                    trans_ray->t = 0.0f;
                    pair<Ray*, vec4> hit_out = scene->objects[obj_id]->hit(trans_ray);

                    float fall_off = 4.0f*(float)M_PI*glm::dot(refrac_hit.second - position, refrac_hit.second - position);
                    
                    
                    irradiance = MC_Sampling(refrac_hit.first, refrac_hit.second, -refrac.first, depth+1);
                    // Light Sampling
                    if(scene->objects[refrac_hit.first]->material->isEmissive){
                        irradiance += scene->objects[refrac_hit.first]->material->albedo * scene->objects[refrac_hit.first]->material->emmission(refrac_hit.second, refrac.first, refrac.first) * scene->objects[refrac_hit.first]->material->ke;
                    }
                    
                    // Not needed for Cos sampling
                    float out_norm = glm::length(refrac.first);
                    float cos_theta = glm::dot(refrac.first, normal)/ (out_norm * normal_norm);
                    irradiance *= (cos_theta/ fall_off);
                    irradiance.w /= (cos_theta/ fall_off);
                }
                T += glm::max(vec4(1.0f) - refrac.second, 0.0f) * irradiance * scene->objects[obj_id]->material->kt;
                // cout<< refrac_hit.first <<"hit\n";
            }
            else{ // Cohesive Volume
                vec4 normal2 = -scene->objects[obj_id]->normal_ray(refrac_hit.second);
                normal2 = normalize(normal2);

                pair<vec4, float> emerg = scene->objects[obj_id]->refracted_ray(refrac.first, refrac_hit.second, normal2, scene->objects[obj_id]->material->mu, scene->mu);

                pair<int, vec4> hit = incident_ray(position, emerg.first);

                vec4 irradiance = vec4(0.0f);
                if(hit.first == -1){
                    irradiance = scene->sky;
                }
                else{
                    Ray *trans_ray = new Ray();
                    trans_ray->o = refrac_hit.second ;//+ 0.00001f * normal2;
                    trans_ray->d = normalize(refrac.first);
                    trans_ray->t = 0.0f;
                    // trans_ray->t_near = 0.0f;
                    // trans_ray->t_far = 1000.0f;
                    pair<Ray*, vec4> hit_out = scene->objects[obj_id]->hit(trans_ray);
                    
                    // ASSUMING NO FALL INSIDE THE MATERIAL
                    float fall_off = 4.0f*(float)M_PI*glm::dot(hit.second - refrac_hit.second, hit.second - refrac_hit.second);
                    //Assuming falloff inside material
                    // fall_off *= 4.0f*(float)M_PI*glm::dot(position - refrac_hit.second, position - refrac_hit.second);
                    
                    
                    irradiance = MC_Sampling(hit.first, hit.second, -emerg.first, depth);
                    // Light Sampling
                    if(scene->objects[hit.first]->material->isEmissive){
                        // irradiance += scene->objects[hit.first]->material->emmission(hit.second, reflect, reflect) * scene->objects[hit.first]->material->ke;
                        irradiance += scene->objects[hit.first]->material->albedo * scene->objects[hit.first]->material->emmission(hit.second, emerg.first, emerg.first) * scene->objects[hit.first]->material->ke;
                    }
                    
                    // Not needed for Cos sampling
                    float out_norm = glm::length(emerg.first);
                    float cos_theta = glm::dot(emerg.first, normal)/ (out_norm * normal_norm);
                    irradiance *= (cos_theta/ fall_off);
                    irradiance.w /= (cos_theta/ fall_off);
                }

                T += (vec4(1.0f) - refrac.second) * irradiance * scene->objects[obj_id]->material->kt;
            }
            
            // REFLECTION
                vec4 normal = scene->objects[obj_id]->normal_ray(position);
                normal = normalize(normal);
                vec4 reflect = 2.0f * dot(out_dir, normal) * normal - out_dir;
                
                pair<int, vec4> reflect_hit = incident_ray(position, reflect);
                vec4 ref_irradiance = vec4(0.0f);
                if(reflect_hit.first == -1){
                    ref_irradiance = scene->sky;
                }
                else{
                    Ray *ref_ray = new Ray();
                    ref_ray->o = position; //+ 0.000001f * normal;
                    ref_ray->d = normalize(reflect);
                    ref_ray->t = 0.0f;
                    pair<Ray*, vec4> hit_out = scene->objects[reflect_hit.first]->hit(ref_ray);

                    float fall_off = 4.0f*(float)M_PI*glm::dot(reflect_hit.second - position, reflect_hit.second - position);
                    
                    ref_irradiance = MC_Sampling(reflect_hit.first, reflect_hit.second, -reflect, depth+2);
                    // Light Sampling
                    if(scene->objects[reflect_hit.first]->material->isEmissive){
                        ref_irradiance += scene->objects[reflect_hit.first]->material->albedo * scene->objects[reflect_hit.first]->material->emmission(reflect_hit.second, reflect, reflect) * scene->objects[reflect_hit.first]->material->ke;
                    }
                    
                    // Not needed for Cos sampling
                    float out_norm = glm::length(reflect);
                    float ref_cos_theta = glm::dot(reflect, normal)/ (out_norm * normal_norm);
                    ref_irradiance *= (ref_cos_theta/ fall_off);
                    ref_irradiance.w /= (ref_cos_theta/ fall_off);
                }
                // cout << refrac.second<< "\n";
                R += refrac.second * ref_irradiance * scene->objects[obj_id]->material->ks;

        }
    }

    if(scene->objects[obj_id]->material->isEmissive){
        F += scene->objects[obj_id]->material->albedo * scene->objects[obj_id]->material->emmission(position, -out_dir, normal) * scene->objects[obj_id]->material->ke * scene->objects[obj_id]->material->ke;
    }
    F += D;
    F += R;
    F += T;
    return scene->objects[obj_id]->material->albedo * F * (MAX_BOUNCES/(MAX_BOUNCES - 1.0f));
}

vec4 Renderer::ray_trace(Ray *ray){
    pair<int, vec4> hit = incident_ray(ray->o, ray->d);
    if(hit.first == -1) return scene->sky;

    vec4 intensity = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i < SAMPLES; i++){ 
        intensity += MC_Sampling(hit.first, hit.second, -(ray->d), 1);
    }
    
    intensity = vec4(pow(intensity.x, 1.0f/2.2f), pow(intensity.y, 1.0f/2.2f), pow(intensity.z, 1.0f/2.2f), pow(intensity.w, 1.0f/2.2f));
    intensity *= (2.0f*M_1_PI/glm::pow((float)SAMPLES, 1.0f/1.0f));
    intensity = vec4(glm::min(1.0f, intensity.x), glm::min(1.0f, intensity.y), glm::min(1.0f, intensity.z), glm::min(1.0f, intensity.w));
    
    return scene->objects[hit.first]->material->albedo * intensity;
}

vec4 Renderer::render(Ray* ray){
    if(SAMPLING.compare("normal_map") == 0)
        return normal_map(ray);
    else if(SAMPLING.compare("point_lambert") == 0)
        return point_lambert(ray);
    else if(SAMPLING.compare("ray_trace") == 0)
        return ray_trace(ray);
    else 
        return vec4(0,0,0,0);
}