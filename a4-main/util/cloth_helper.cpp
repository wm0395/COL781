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
    force = vec3(0, -10.0f, 0);
    for(Spring* s : springs) {
        // cout << "Spring connecting particles: " << s->p1->identity << " " << s->p2->identity << endl;
        int x1 = s->p1->identity;
        int x2 = s->p2->identity;
        vec3 xij;
        if (x1 == identity){
            xij = pos - s->p2->pos;
        } 
        else {
            xij = pos - s->p1->pos;
        }
        // vec3 xij = s->p1 < s->p2 ? pos - s->p2->pos : pos - s->p1->pos;
        float l = length(xij);
        vec3 dir = normalize(xij);
        vec3 f = -s->ks*(l - s->l0)*dir - s->kd*dot(vel, dir)*dir;
        force += f;
    }
}