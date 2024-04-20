# include "cloth_helper.hpp"

using namespace glm;
using namespace std;

Spring::Spring(float ks, float kd, float l0, Particle* p1, Particle* p2){
    this->ks = ks;
    this->kd = kd;
    this->l0 = l0;
    this->p1 = p1;
    this->p2 = p2;
}

Particle::Particle(int identity, float mass, glm::vec3 pos, glm::vec3 vel, glm::vec3 force) {
    this->identity = identity;
    this->mass = mass;
    this->pos = pos;
    this->vel = vel;
    this->force = force;
}

void Particle::compute_force() {
    force = vec3(0, 0, 0);
    for(Spring* s : springs) {
        int x1 = s->p1->identity;
        int x2 = s->p2->identity;
        vec3 xij;
        if (x1 == identity){
            xij = pos - s->p2->pos;
        } 
        else {
            xij = pos - s->p1->pos;
        }
        float l = length(xij);
        vec3 dir = normalize(xij);
        vec3 f = -s->ks*(l - s->l0)*dir - s->kd*dot(vel, dir)*dir;
        force += f;
    }
}

Plane::Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient){
    this->depth = depth;
    this->normal = normal;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;
    this->velocity = vec3(0, 0, 0);
    this->angular_velocity = vec3(0, 0, 0);
    this->isMoving = false;
}

Plane::Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient, vec3 velocity){
    this->depth = depth;
    this->normal = normal;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;
    this->velocity = velocity;
    this->angular_velocity = vec3(0, 0, 0);
    this->isMoving = true;
}

Plane::Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient, vec3 velocity, vec3 angular_velocity){
    this->depth = depth;
    this->normal = normal;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;
    this->velocity = velocity;
    this->angular_velocity = angular_velocity;
    this->isMoving = true;
}

float Plane::collision(Particle* particle){
    return particle->pos.y - depth;
}

vec3 Plane::collision_normal(Particle* particle){
    return normal;
}

vec3 Plane::tangetial_velocity(Particle* particle){
    return particle->vel - dot(particle->vel, normal)*normal;
}

void Plane::update(float dt, float t){
    return;  // TODO: if you want it could be done later
}


Sphere::Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude){
    this->radius = radius;
    this->center = center;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;
    this->latitude = latitude;
    this->longitude = longitude;
    this->num_vertices = latitude * (longitude - 1) + 2;
    this->num_triangles = 2 * latitude * (longitude - 1);
    this->vertices = new glm::vec3[num_vertices];
    this->normals = new glm::vec3[num_vertices];
    this->triangles = new glm::ivec3[num_triangles];
    sphere_grid();
}

Sphere::Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude, vec3 velocity){
    this->radius = radius;
    this->center = center;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;
    this->latitude = latitude;
    this->longitude = longitude;
    this->num_vertices = latitude * (longitude - 1) + 2;
    this->num_triangles = 2 * latitude * (longitude - 1);
    this->vertices = new glm::vec3[num_vertices];
    this->normals = new glm::vec3[num_vertices];
    this->triangles = new glm::ivec3[num_triangles];
    sphere_grid();
    this->velocity = velocity;
    this->angular_velocity = vec3(0, 0, 0);
    this->isMoving = true;
    this->angular_matrix = mat4(0.0f);
    angular_matrix[3][3] = 1.0f;
}

void print_mat4(mat4 m){
    for (int i = 0; i<4; i++){
        for (int j = 0; j<4; j++){
            cout<<m[i][j]<<" ";
        }
        cout<<endl;
    }
    cout << endl;
}

Sphere::Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude, vec3 velocity, vec3 angular_velocity){
    this->radius = radius;
    this->center = center;
    this->coefficient_of_restitution = coefficient_of_restitution;
    this->friction_coefficient = friction_coefficient;

    this->latitude = latitude;
    this->longitude = longitude;
    this->num_vertices = latitude * (longitude - 1) + 2;
    this->num_triangles = 2 * latitude * (longitude - 1);
    this->vertices = new glm::vec3[num_vertices];
    this->normals = new glm::vec3[num_vertices];
    this->triangles = new glm::ivec3[num_triangles];
    sphere_grid();

    this->velocity = velocity;
    this->angular_velocity = angular_velocity;
    this->isMoving = true;
    this->isRotating = true;
    // vec3 rotation_axis = normalize(angular_velocity);  //TODO: check if correct
    // translate the axis so that it passes through center of the sphere
    // this->rotation_mat = translate(rotation_mat, center);
    // // // print_mat4(rotation_mat);
    // this->rotation_mat = rotate(rotation_mat, radians(0.0f), rotation_axis);
    // // // print_mat4(rotation_mat);
    // this->rotation_mat = translate(rotation_mat, -center);
    // cout << "Before any updation => \n";
    // print_mat4(rotation_mat);
    // print_mat4(rotation_mat);
    // this->angular_matrix = mat4(vec4(0.0f, -angular_velocity.z, angular_velocity.y, 0.0f),
    //                             vec4(angular_velocity.z, 0.0f, -angular_velocity.x, 0.0f),
    //                             vec4(-angular_velocity.y, angular_velocity.x, 0.0f, 0.0f),
    //                             vec4(0.0f, 0.0f, 0.0f, 1.0f));
}


void Sphere::sphere_grid(){
    float r = this->radius - 0.01f; //TODO: check if we want to subtract a small value
    int m = this->latitude;
    int n = this->longitude;
    vec3 c = this->center;
    int vert_cnt = this->num_vertices;
    int tri_cnt = this->num_triangles;

    int cnt = 1;
    int pres_tri_cnt = latitude;

    this->vertices[0] = vec3(c.x+0.0, c.y+r, c.z+0.0);
    this->normals[0] = normalize(this->vertices[0] - c);
    this->vertices[vert_cnt-1] = vec3(c.x+0.0, c.y-r, c.z+0.0);
    this->normals[vert_cnt-1] = normalize(this->vertices[vert_cnt-1] - c);

    for (int i = 0; i<m; i++){
        this->triangles[i] = ivec3(0,i+1,((i+1)%m)+1);
    }

    for (int lat = 1; lat<=n-1; lat++){
        for (int log = 0; log<m; log++){
            this->vertices[cnt] = vec3(c.x + r * std::cos(2*M_PI*log/float(m)) * std::sin(M_PI*lat/float(n)), c.y+r * std::cos(M_PI*lat/float(n)), c.z+r * std::sin(2*M_PI*log/float(m)) * std::sin(M_PI*lat/float(n)));
            this->normals[cnt] = normalize(this->vertices[cnt] - c);
            cnt++;

            if (lat != n-1){
                if (log != m-1){
                    int x1 = (lat-1)*m + log + 1;
                    int x2 = x1 + 1;
                    int x3 = x1 + m;
                    int x4 = x3 + 1;
                    this->triangles[pres_tri_cnt] = ivec3(x1, x3, x4);
                    pres_tri_cnt++;
                    this->triangles[pres_tri_cnt] = ivec3(x2, x1, x4);
                    pres_tri_cnt++;
                }
                else if (log == m-1){
                    int x1 = (lat-1)*m + log + 1;
                    int x2 = (lat-1)*m + 1;
                    int x3 = x1 + m;
                    int x4 = x2 + m;
                    this->triangles[pres_tri_cnt] = ivec3(x1, x3, x4);
                    pres_tri_cnt++;
                    this->triangles[pres_tri_cnt] = ivec3(x1, x2, x4);
                    pres_tri_cnt++;
                }
            }

            if (lat == n-1 && log!=m-1){
                int vert = m*(n-2) + log + 1;
                this->triangles[pres_tri_cnt] = ivec3(vert, vert+1, m*(n-1)+1);
                pres_tri_cnt++;
            }
            else if (lat == n-1 && log == m-1){
                this->triangles[pres_tri_cnt] = ivec3(m*(n-1), m*(n-2)+1, m*(n-1)+1);
                pres_tri_cnt++;
            }
        }
    }
    return;
}

float Sphere::collision(Particle* particle){
    return length(particle->pos - center) - radius;
}

vec3 Sphere::collision_normal(Particle* particle){
    return normalize(particle->pos - center);  //TODO: check if we want to normalize or not
}

vec3 Sphere::tangetial_velocity(Particle* particle){
    vec3 normal = collision_normal(particle);   // TODO: use relative velocity
    vec3 sph_vel = velocity + cross(angular_velocity, particle->pos - center);
    vec3 rel_vel = particle->vel - sph_vel;
    // return particle->vel - dot(particle->vel, normal)*(normal);
    return rel_vel - dot(rel_vel, normal)*(normal);
}

void Sphere::update(float dt, float t){
    if (isMoving){
        // rotation_mat = translate(rotation_mat, -center);
        // print_mat4(rotation_mat);
        // mat4 new_rotation_mat = angular_matrix * rotation_mat;
        // print_mat4(new_rotation_mat);
        // new_rotation_mat *= dt;
        // print_mat4(new_rotation_mat);
        // new_rotation_mat += rotation_mat;
        // // print_mat4(new_rotation_mat);
        // rotation_mat = new_rotation_mat;
        // print_mat4(rotation_mat);
        // rotation_mat = translate(rotation_mat, center);
        // print_mat4(rotation_mat);

        // cout << "Center: " << center.x << " " << center.y << " " << center.z << endl;
        center += velocity * dt;
        if (isRotating){
            rotation_mat = mat4(1.0f);
            rotation_mat = translate(rotation_mat, center);
            // print_mat4(rotation_mat);
            rotation_mat = rotate(rotation_mat, radians(length(angular_velocity)*t), normalize(angular_velocity));
            // print_mat4(rotation_mat);
            rotation_mat = translate(rotation_mat, -center);
        }

        //check if the rotation_mat is a valid rotation matrix
        // print_mat4(transpose(rotation_mat) * rotation_mat);

        // multiply each of the vertices and normals with the transformation matrix
        for (int i = 0; i<num_vertices; i++){
            vec4 v = vec4(vertices[i], 1.0f);
            v = rotation_mat * v;
            vertices[i] /= v.w;
            vertices[i] = vec3(v.x, v.y, v.z);
            vertices[i] += velocity*dt;
            vec4 n = vec4(normals[i], 0.0f);
            n = rotation_mat * n;
            normals[i] = vec3(n.x, n.y, n.z);  // TODO: check normals wala case
        }
        vec4 c = vec4(center, 1.0f);
        c = rotation_mat * c;
        c /= c.w;
        center = vec3(c.x, c.y, c.z);
        // center += velocity * dt;
    }
    return;
}