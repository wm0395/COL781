#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

class Particle;
class Plane;

class Spring {
public:
    Spring(float ks, float kd, float l0, Particle* p1, Particle* p2);

    float ks, kd, l0;
    Particle* p1;
    Particle* p2;
};

class Particle {
public:
    Particle(int identity, float mass, vec3 pos, vec3 vel, vec3 force);

    float mass;
    vec3 pos, vel, force;
    int identity;
    vector<Spring*> springs;

    void compute_force();
};

class Obstacles {
public:
    float coefficient_of_restitution;
    float friction_coefficient;
    vec3 velocity = vec3(0, 0, 0);
    vec3 angular_velocity = vec3(0, 0, 0);
    bool isMoving = false;
    bool isRotating = false;
    mat4 rotation_mat = mat4(1.0f);

    virtual float collision(Particle* particle) = 0;
    virtual vec3 collision_normal(Particle* particle) = 0;
    virtual vec3 tangetial_velocity(Particle* particle) = 0;
    virtual void update(float dt, float t) = 0;
};

class Plane : public Obstacles {
public:
    Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient);
    Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient, vec3 velocity);
    Plane(float depth, vec3 normal, float coefficient_of_restitution, float friction_coefficient, vec3 velocity, vec3 angular_velocity);
    float depth;
    vec3 normal;

    float collision(Particle* particle);
    vec3 collision_normal(Particle* particle);
    vec3 tangetial_velocity(Particle* particle);
    void update(float dt, float t);
};

class Sphere : public Obstacles {
public:
    Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude);
    Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude, vec3 velocity);
    Sphere(float radius, vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude, vec3 velocity, vec3 angular_velocity);
    float radius;
    vec3 center;

    float collision(Particle* particle);
    vec3 collision_normal(Particle* particle);
    vec3 tangetial_velocity(Particle* particle);
    void update(float dt, float t);

    int latitude, longitude;
    int num_vertices;
    int num_triangles;
    vec3 *vertices;
    vec3 *normals;
    ivec3 *triangles;

    vec3* initial_vertices;
    vec3* initial_normals;
    void sphere_grid();
};