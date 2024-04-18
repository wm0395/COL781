#include "sw.hpp"
#include "camera.hpp"

#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;

const int nv = 8*3;
const int nt = 12*3;
vec3 vertices[nv];
vec3 normals[nv];
ivec3 triangles[nt];
Bone* hip, *neck, *head;

GL::Object object;
GL::AttribBuf vertexBuf, normalBuf;

CameraControl camCtl;

void initializeScene() {
	object = r.createObject();
    for(int i = 0; i < 3; i++){
        vertices[8*i+0] = vec3(0, 0, 0);
        vertices[8*i+1] = vec3(1, 0, 0);
        vertices[8*i+2] = vec3(1, 0, -1);
        vertices[8*i+3] = vec3(0, 0, -1);
        vertices[8*i+4] = vec3(0, 1, 0);
        vertices[8*i+5] = vec3(1, 1, 0);
        vertices[8*i+6] = vec3(1, 1, -1);
        vertices[8*i+7] = vec3(0, 1, -1);
    
        normals[8*i+0] = vec3(-1, 0, 1);
        normals[8*i+1] = vec3(1, 0, 1);
        normals[8*i+2] = vec3(1, 0, -1);
        normals[8*i+3] = vec3(-1, 0, -1);
        normals[8*i+4] = vec3(-1, 0, 1);
        normals[8*i+5] = vec3(1, 0, 1);
        normals[8*i+6] = vec3(1, 0, -1);
        normals[8*i+7] = vec3(-1, 0, -1);
        
        triangles[12*i+0] = ivec3(8*i+0, 8*i+1, 8*i+5);
        triangles[12*i+1] = ivec3(8*i+0, 8*i+5, 8*i+4);
        triangles[12*i+2] = ivec3(8*i+1, 8*i+2, 8*i+6);
        triangles[12*i+3] = ivec3(8*i+1, 8*i+6, 8*i+5);
        triangles[12*i+4] = ivec3(8*i+2, 8*i+3, 8*i+7);
        triangles[12*i+5] = ivec3(8*i+2, 8*i+7, 8*i+6);
        triangles[12*i+6] = ivec3(8*i+3, 8*i+0, 8*i+4);
        triangles[12*i+7] = ivec3(8*i+3, 8*i+4, 8*i+7);
        triangles[12*i+8] = ivec3(8*i+7, 8*i+4, 8*i+5);
        triangles[12*i+9] = ivec3(8*i+7, 8*i+5, 8*i+6);
        triangles[12*i+10] = ivec3(8*i+1, 8*i+0, 8*i+3);
        triangles[12*i+11] = ivec3(8*i+1, 8*i+3, 8*i+2);
    }
	vertexBuf = r.createVertexAttribs(object, 0, nv, vertices);
	normalBuf = r.createVertexAttribs(object, 1, nv, normals);
	r.createTriangleIndices(object, nt, triangles);

    hip = new Bone();
    hip->position = vec4(0.0f, -2.0f, -1.0f, 1.0f);
    hip->parent = nullptr;
    hip->axis = vec3(1.0f, 0.0f, 0.0f);
    hip->theta = 0.0f;
    for(int i = 0; i < 12; i++)
    hip->triIndex.push_back(i);
    
    neck = new Bone();
    neck->position = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    neck->parent = hip;
    neck->axis = vec3(1.0f, 0.0f, 0.0f);
    neck->theta = 0.0f;
    for(int i = 12; i < 24; i++)
    neck->triIndex.push_back(i);

    head = new Bone();
    head->position = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    head->parent = neck;
    head->axis = vec3(0.0f, 0.0f, 1.0f);
    head->theta = 0.0f;
    for(int i = 24; i < 36; i++)
    head->triIndex.push_back(i);
}

void updateScene(float t) {
	float freq = 2, amp = 1;
	float phase0 = 0, phase1 = 0.5;
	float theta0 = amp*cos(freq*t + phase0), theta1 = amp*cos(freq*t + phase1);
    
    vec3 TfVertices[nv];
    vec3 TfNormals[nv];
    
    neck->theta = theta0;
    head->theta = theta1;

    mat4 tf = hip->getMatrix();
    set<int> verts;

    for(auto i : hip->triIndex){
        verts.insert(triangles[i][0]);
        verts.insert(triangles[i][1]);
        verts.insert(triangles[i][2]);
    }

    for(auto i: verts){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = neck->getMatrix();
    verts = set<int>();

    for(auto i : neck->triIndex){
        verts.insert(triangles[i][0]);
        verts.insert(triangles[i][1]);
        verts.insert(triangles[i][2]);
    }

    for(auto i: verts){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = head->getMatrix();
    verts = set<int>();

    for(auto i : head->triIndex){
        verts.insert(triangles[i][0]);
        verts.insert(triangles[i][1]);
        verts.insert(triangles[i][2]);
    }

    for(auto i: verts){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

	// vertices[0] = vec3(0, -cos(theta0), sin(theta0));
	// vertices[1] = vec3(1, -cos(theta1), sin(theta1));
	r.updateVertexAttribs(vertexBuf, nv, TfVertices);
	// normals[0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[3]-vertices[0]));
	// normals[1] = glm::normalize(glm::cross(vertices[2]-vertices[1], vertices[0]-vertices[1]));
	// normals[2] = glm::normalize(glm::cross(vertices[3]-vertices[2], vertices[1]-vertices[2]));
	// normals[3] = glm::normalize(glm::cross(vertices[0]-vertices[3], vertices[2]-vertices[3]));
	// r.updateVertexAttribs(normalBuf, nv, normals);
}

int main() {
	int width = 640, height = 480;
	if (!r.initialize("Animation", width, height)) {
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
