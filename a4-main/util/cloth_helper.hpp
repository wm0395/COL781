#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Particle;

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