#include "camera.hpp"
#include "../util/cloth_helper.cpp"
#include "../util/math.cpp"

#include <iostream>
#include <vector>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;
using namespace std;

GL::Rasterizer r;
GL::ShaderProgram program;

GL::Object object;
GL::AttribBuf vertexBuf, normalBuf;

const float cloth_length = 1;
const float cloth_width = 1;
const int particles_along_length = 18;
const int particles_along_width = 18;
const float cloth_mass = 25.0f;
const float mass = cloth_mass / (particles_along_length * particles_along_width);
const float ks_structural = 100000;
const float kd_structural = 10;
const float ks_shear = 300;
const float kd_shear = 1;
const float ks_bend = 30;
const float kd_bend = 1;
const float gravity = 10;

const int num_of_particles = particles_along_length * particles_along_width;
const int nt = 2*(particles_along_length-1)*(particles_along_width-1);

const int fixed1 = 0;
const int fixed2 = particles_along_width * (particles_along_length-1);
vec3 fixed_pos1(0, 0, 0);
vec3 fixed_pos2(0, 0, 0);

const float plane_depth = -0.6;
const float plane_restitution = 0.2;
const float plane_friction = 0.7;
Plane* plane = new Plane(plane_depth, vec3(0, 1, 0), plane_restitution, plane_friction);

float radius1 = 0.1;
const int latitude = 20;
const int longitude = 10;
const vec3 sphere1_vel = vec3(1.0f, 1.0f, 0.0f); 
const vec3 sphere1_ang_vel = vec3(1.0f, 1.0f, 0.0f);
// Sphere* sphere1 = new Sphere(radius1, vec3(0.5, plane_depth + radius1, 0.5), 0.5, 0.7, latitude, longitude, sphere1_vel);
Sphere* sphere1 = new Sphere(radius1, vec3(0.5, plane_depth+radius1, 0.5), 0.5, 0.8, latitude, longitude, sphere1_vel, sphere1_ang_vel);
const int sphere1_vert = latitude*(longitude - 1) + 2;
const int sphere1_tri = 2 * latitude * (longitude - 1);

vector<Obstacles*> obstacles = {plane, sphere1};

vector<vector<float>> Mass_inverse(3*num_of_particles, vector<float>(3*num_of_particles, 0));
vector<float> velocity(3*num_of_particles, 0);
vector<float> position(3*num_of_particles, 0);
vector<float> new_velocity(3*num_of_particles, 0);
vector<float> new_position(3*num_of_particles, 0);
vector<float> force(3*num_of_particles, 0);

vec3 vertices[num_of_particles+ 4 + sphere1_vert];
vec3 normals[num_of_particles + 4 + sphere1_vert];
ivec3 triangles[nt + 2 + sphere1_tri];
Particle* particles[num_of_particles];

CameraControl camCtl;

void add_sphere(){
    for (int i = 0; i<sphere1_vert; i++){
        vertices[num_of_particles+4+i] = sphere1->vertices[i];
        normals[num_of_particles+4+i] = sphere1->normals[i];
    }
    for (int i = 0; i<sphere1_tri; i++){
        triangles[nt+2+i] = sphere1->triangles[i] + ivec3(num_of_particles+4, num_of_particles+4, num_of_particles+4);
    }
}

void initializeScene() {
	object = r.createObject();

    float l = cloth_length / (particles_along_length-1);
    float w = cloth_width / (particles_along_width-1);

    for(int i = 0; i < particles_along_length; i++) {
        for(int j = 0; j < particles_along_width; j++) {
            glm::vec3 pos(l*i, 0.0f, w*j);
            glm::vec3 vel(0.0f, 0.0f, 0.0f);
            glm::vec3 force(0.0f, -gravity, 0.0f);
            Particle* p = new Particle(i*particles_along_width + j, mass, pos, vel, force);
            particles[i*particles_along_width + j] = p;
            vertices[i*particles_along_width + j] = pos;
            normals[i*particles_along_width + j] = vec3(0, 1, 0);
        }
    }

    // add structural springs
    for(int i = 0; i < particles_along_length; i++) {
        for(int j = 0; j < particles_along_width; j++) {
            if(i < particles_along_length-1) {
                Spring* s = new Spring(ks_structural, kd_structural, l, particles[i*particles_along_width + j], particles[(i+1)*particles_along_width + j]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[(i+1)*particles_along_width + j]->springs.push_back(s);
            }
            if(j < particles_along_width-1) {
                Spring* s = new Spring(ks_structural, kd_structural, w, particles[i*particles_along_width + j], particles[i*particles_along_width + j + 1]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[i*particles_along_width + j + 1]->springs.push_back(s);
            }
        }
    }

    // add shear springs
    for(int i = 0; i < particles_along_length; i++) {
        for(int j = 0; j < particles_along_width; j++) {
            if(i < particles_along_length-1 && j < particles_along_width-1) {
                Spring* s = new Spring(ks_shear, kd_shear, sqrt(l*l + w*w), particles[i*particles_along_width + j], particles[(i+1)*particles_along_width + j + 1]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[(i+1)*particles_along_width + j + 1]->springs.push_back(s);
            }
            if(i < particles_along_length-1 && j > 0) {
                Spring* s = new Spring(ks_shear, kd_shear, sqrt(l*l + w*w), particles[i*particles_along_width + j], particles[(i+1)*particles_along_width + j - 1]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[(i+1)*particles_along_width + j - 1]->springs.push_back(s);
            }
        }
    }

    // add bend springs
    for(int i = 0; i < particles_along_length; i++) {
        for(int j = 0; j < particles_along_width; j++) {
            if(i < particles_along_length-2) {
                Spring* s = new Spring(ks_bend, kd_bend, 2*l, particles[i*particles_along_width + j], particles[(i+2)*particles_along_width + j]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[(i+2)*particles_along_width + j]->springs.push_back(s);
            }
            if(j < particles_along_width-2) {
                Spring* s = new Spring(ks_bend, kd_bend, 2*w, particles[i*particles_along_width + j], particles[i*particles_along_width + j + 2]);
                particles[i*particles_along_width + j]->springs.push_back(s);
                particles[i*particles_along_width + j + 2]->springs.push_back(s);
            }
        }
    }

    for(int i = 0; i < particles_along_length-1; i++) {
        for(int j = 0; j < particles_along_width-1; j++) {
            triangles[2*(i*(particles_along_width-1) + j)] = vec3(i*particles_along_width + j, i*particles_along_width + j + 1, (i+1)*particles_along_width + j);
            triangles[2*(i*(particles_along_width-1) + j) + 1] = vec3(i*particles_along_width + j + 1, (i+1)*particles_along_width + j + 1, (i+1)*particles_along_width + j);
        }
    }

    // add a big square below the above cloth
    vertices[num_of_particles] = vec3(-100, plane_depth, 100);
    vertices[num_of_particles+1] = vec3(100, plane_depth, 100);
    vertices[num_of_particles+2] = vec3(100, plane_depth, -100);
    vertices[num_of_particles+3] = vec3(-100, plane_depth, -100);
    normals[num_of_particles] = vec3(0, 1, 0);
    normals[num_of_particles+1] = vec3(0, 1, 0);
    normals[num_of_particles+2] = vec3(0, 1, 0);
    normals[num_of_particles+3] = vec3(0, 1, 0);
    triangles[nt] = ivec3(num_of_particles, num_of_particles+1, num_of_particles+2);
    triangles[nt+1] = ivec3(num_of_particles, num_of_particles+2, num_of_particles+3);

    add_sphere();

    vertexBuf = r.createVertexAttribs(object, 0, num_of_particles+4+sphere1_vert, vertices);
    normalBuf = r.createVertexAttribs(object, 1, num_of_particles+4+sphere1_vert, normals);
    r.createTriangleIndices(object, nt+2+sphere1_tri, triangles);

    for(int i = 0; i < num_of_particles; i++) {
        for(int j = 0; j < 3; j++) {
            Mass_inverse[3*i+j][3*i+j] = 1/particles[i]->mass;
        }
    }

    // fill the velocity and position arrays
    for(int i = 0; i < num_of_particles; i++) {
        for(int j = 0; j < 3; j++) {
            velocity[3*i+j] = particles[i]->vel[j];
            position[3*i+j] = particles[i]->pos[j];
            force[3*i+j] = particles[i]->force[j];
        }
    }

    fixed_pos1 = particles[fixed1]->pos;
    fixed_pos2 = particles[fixed2]->pos;
}

void updateScene(float t) {
    float dt = 0.0005;
    for(int i = 0; i < num_of_particles; i++) {
        particles[i]->compute_force();
        particles[i]->force.y -= gravity;
        for(int j = 0; j < 3; j++) {
            force[3*i+j] = particles[i]->force[j];
        }
    }

    new_velocity = product(Mass_inverse, force);
    new_velocity = scale(new_velocity, dt);
    new_velocity = sum(velocity, new_velocity);
    new_position = sum(position, scale(new_velocity, dt));

    velocity = new_velocity;
    position = new_position;

    for(int i = 0; i < num_of_particles; i++) {
        particles[i]->pos = vec3(position[3*i], position[3*i+1], position[3*i+2]);
        particles[i]->vel = vec3(velocity[3*i], velocity[3*i+1], velocity[3*i+2]);
        particles[i]->force = vec3(force[3*i], force[3*i+1], force[3*i+2]);
        vertices[i] = particles[i]->pos;
    }

    particles[fixed1]->pos = fixed_pos1;
    particles[fixed1]->vel = vec3(0, 0, 0);
    vertices[fixed1] = fixed_pos1;
    particles[fixed2]->pos = fixed_pos2;
    particles[fixed2]->vel = vec3(0, 0, 0);
    vertices[fixed2] = fixed_pos2;

    for (auto obs : obstacles){
        obs->update(dt, t);
        add_sphere();
    }

    for (int i = 0; i<num_of_particles; i++){
        for (auto obs : obstacles){
            float phix = obs->collision(particles[i]);
            vec3 normal = obs->collision_normal(particles[i]);
            if (phix < 0){
                float vn = dot(particles[i]->vel, normal);
                if (vn < 0){
                    float jn = -(1 + obs->coefficient_of_restitution)*vn*particles[i]->mass;
                    vec3 tangetial_vel = obs->tangetial_velocity(particles[i]);
                    vec3 jt = -std::min(obs->friction_coefficient*jn, particles[i]->mass*length(tangetial_vel))*normalize(tangetial_vel);
                    particles[i]->vel += (jn*normal + jt)/particles[i]->mass;
                    velocity[3*i] = particles[i]->vel.x;
                    velocity[3*i+1] = particles[i]->vel.y;
                    velocity[3*i+2] = particles[i]->vel.z;
                }
                float del_xn = -phix;
                particles[i]->pos += del_xn*normal;
                position[3*i] = particles[i]->pos.x;
                position[3*i+1] = particles[i]->pos.y;
                position[3*i+2] = particles[i]->pos.z;
            }
        }
    }

    r.updateVertexAttribs(vertexBuf, num_of_particles+4+sphere1_vert, vertices);

    for(int i = 0; i < particles_along_length; i++) {
        for(int j = 0; j < particles_along_width; j++) {
            vec3 nv(0, 0, 0);
            if(i > 0 && i < particles_along_length-1 && j > 0 && j < particles_along_width-1) {
                vec3 n1 = glm::normalize(vertices[(i-1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n2 = glm::normalize(vertices[(i+1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n3 = glm::normalize(vertices[i*particles_along_width + j-1] - vertices[i*particles_along_width + j]);
                vec3 n4 = glm::normalize(vertices[i*particles_along_width + j+1] - vertices[i*particles_along_width + j]);
                nv = glm::normalize(glm::cross(n3, n1) + glm::cross(n4, n2));
            }
            else if (i == 0 && j == 0) {
                vec3 n1 = glm::normalize(vertices[(i+1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n2 = glm::normalize(vertices[i*particles_along_width + j+1] - vertices[i*particles_along_width + j]);
                nv = glm::normalize(glm::cross(n2, n1));
            }
            else if (i == 0 && j == particles_along_width-1) {
                vec3 n1 = glm::normalize(vertices[(i+1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n2 = glm::normalize(vertices[i*particles_along_width + j-1] - vertices[i*particles_along_width + j]);
                nv = glm::normalize(glm::cross(n1, n2));
            }
            else if (i == particles_along_length-1 && j == 0) {
                vec3 n1 = glm::normalize(vertices[(i-1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n2 = glm::normalize(vertices[i*particles_along_width + j+1] - vertices[i*particles_along_width + j]);
                nv = glm::normalize(glm::cross(n1, n2));
            }
            else if (i == particles_along_length-1 && j == particles_along_width-1) {
                vec3 n1 = glm::normalize(vertices[(i-1)*particles_along_width + j] - vertices[i*particles_along_width + j]);
                vec3 n2 = glm::normalize(vertices[i*particles_along_width + j-1] - vertices[i*particles_along_width + j]);
                nv = glm::normalize(glm::cross(n2, n1));
            }
            normals[i*particles_along_width + j] = nv;
        }
    }

    r.updateVertexAttribs(normalBuf, num_of_particles+4+sphere1_vert, normals);
}

int main() {
	int width = 1000, height = 900;
	if (!r.initialize("Cloth", width, height)) {
		return EXIT_FAILURE;
	}
	camCtl.initialize(width, height);
	camCtl.camera.setCameraView(vec3(0.5, -0.5, 1.5), vec3(0.5, -0.5, 0.0), vec3(0.0, 1.0, 0.0));
	program = r.createShaderProgram(
		r.vsBlinnPhong(),
		r.fsBlinnPhong()
	);

	initializeScene();

	while (!r.shouldQuit()) {
        float t = SDL_GetTicks64()*1e-3;
		updateScene(t);

		camCtl.update();
		Camera &camera = camCtl.camera;

		r.clear(vec4(1.0, 1.0, 1.0, 1.0));
		r.enableDepthTest();
		r.useShaderProgram(program);

		r.setUniform(program, "model", glm::mat4(1.0));
		r.setUniform(program, "view", camera.getViewMatrix());
		r.setUniform(program, "projection", camera.getProjectionMatrix());
		r.setUniform(program, "lightPos", camera.position);
		r.setUniform(program, "viewPos", camera.position);
		r.setUniform(program, "lightColor", vec3(1.0f, 1.0f, 1.0f));

		r.setupFilledFaces();
        glm::vec3 orange(1.0f, 0.6f, 0.2f);
        glm::vec3 white(1.0f, 1.0f, 1.0f);
        r.setUniform(program, "ambientColor", 0.4f*orange);
        r.setUniform(program, "diffuseColor", 0.9f*orange);
        r.setUniform(program, "specularColor", 0.8f*white);
        r.setUniform(program, "phongExponent", 100.f);
		r.drawObject(object);

		r.setupWireFrame();
        glm::vec3 black(0.0f, 0.0f, 0.0f);
        r.setUniform(program, "ambientColor", black);
        r.setUniform(program, "diffuseColor", black);
        r.setUniform(program, "specularColor", black);
        r.setUniform(program, "phongExponent", 0.f);
		r.drawObject(object);

		r.show();
	}
}