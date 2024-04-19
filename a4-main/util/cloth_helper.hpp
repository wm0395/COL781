#include <glm/glm.hpp>
#include <vector>
#include <iostream>

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
    Particle(int identity, float mass, glm::vec3 pos, glm::vec3 vel, glm::vec3 force);

    float mass;
    glm::vec3 pos, vel, force;
    int identity;
    std::vector<Spring*> springs;

    void compute_force();
};

class Obstacles {
public:
    float coefficient_of_restitution;
    float friction_coefficient;

    virtual float collision(Particle* particle) = 0;
    virtual glm::vec3 collision_normal(Particle* particle) = 0;
    virtual glm::vec3 tangetial_velocity(Particle* particle) = 0;
};

class Plane : public Obstacles {
public:
    Plane(float depth, glm::vec3 normal, float coefficient_of_restitution, float friction_coefficient);
    float depth;
    glm::vec3 normal;

    float collision(Particle* particle);
    glm::vec3 collision_normal(Particle* particle);
    glm::vec3 tangetial_velocity(Particle* particle);
};

class Sphere : public Obstacles {
public:
    Sphere(float radius, glm::vec3 center, float coefficient_of_restitution, float friction_coefficient, int latitude, int longitude);
        float radius;
        glm::vec3 center;

        float collision(Particle* particle);
        glm::vec3 collision_normal(Particle* particle);
        glm::vec3 tangetial_velocity(Particle* particle);

        int latitude, longitude;
        int num_vertices;
        int num_triangles;
        glm::vec3 *vertices;
        glm::vec3 *normals;
        glm::ivec3 *triangles;
        void sphere_grid();
};