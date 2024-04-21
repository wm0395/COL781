#include "sw.hpp"
#include "camera.hpp"

#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;

// Humanoid mesh using rectangles

const int nv = 4*15; 
const int nt = 2*15;
vec3 vertices[nv];
vec3 normals[nv];
ivec3 triangles[nt];
Bone *hip, *chest, *head,
 *l_upper_arm, *l_lower_arm, *l_hand,
 *r_upper_arm, *r_lower_arm, *r_hand, 
 *l_upper_leg, *l_lower_leg, *l_foot, 
 *r_upper_leg, *r_lower_leg, *r_foot;


const int frames = 12;
const int numSamples = 12*10;
vector<Keyframe> keyFrames =  vector<Keyframe>(frames, Keyframe());

GL::Object object;
GL::AttribBuf vertexBuf, normalBuf;

CameraControl camCtl;

void initializeScene() {
	object = r.createObject();

    // HIP
    vertices[0] = vec3(-0.25, 0.0, 0.0);
    vertices[1] = vec3(0.25, 0.0, 0.0);
    vertices[2] = vec3(-0.25, 0.1, 0.0);
    vertices[3] = vec3(0.25, 0.1, 0.0);

    // CHEST
    vertices[4] = vec3(-0.25, 0.0, 0.0);
    vertices[5] = vec3(0.25, 0.0, 0.0);
    vertices[6] = vec3(-0.25, 0.6, 0.0);
    vertices[7] = vec3(0.25, 0.6, 0.0);

    // HEAD
    vertices[8] = vec3(-0.1, 0.0, 0.0);
    vertices[9] = vec3(0.1, 0.0, 0.0);
    vertices[10] = vec3(-0.1, 0.2, 0.0);
    vertices[11] = vec3(0.1, 0.2, 0.0);


    // ARMS and LEGS
    for(int i = 1; i < 5; i++){
        // UPPER
        vertices[12*i+0] = vec3(-0.1, 0.0, 0.0);
        vertices[12*i+1] = vec3(0.1, 0.0, 0.0);
        vertices[12*i+2] = vec3(-0.1, 0.4, 0.0);
        vertices[12*i+3] = vec3(0.1, 0.4, 0.0);

        // LOWER
        vertices[12*i+4] = vec3(-0.1, 0.0, 0.0);
        vertices[12*i+5] = vec3(0.1, 0.0, 0.0);
        vertices[12*i+6] = vec3(-0.1, 0.4, 0.0);
        vertices[12*i+7] = vec3(0.1, 0.4, 0.0);

        // HAND/FOOT
        vertices[12*i+8] = vec3(-0.1, 0.0, 0.0);
        vertices[12*i+9] = vec3(0.1, 0.0, 0.0);
        vertices[12*i+10] = vec3(-0.1, 0.2, 0.0);
        vertices[12*i+11] = vec3(0.1, 0.2, 0.0);
    }

    for(int i = 0; i < 15; i++){
        triangles[2*i+0] = ivec3(4*i+0, 4*i+1, 4*i+2);
        triangles[2*i+1] = ivec3(4*i+1, 4*i+2, 4*i+3);
    }


	vertexBuf = r.createVertexAttribs(object, 0, nv, vertices);
	normalBuf = r.createVertexAttribs(object, 1, nv, normals);
	r.createTriangleIndices(object, nt, triangles);

    hip = new Bone();
    hip->axis = vec3(1.0, 0.0, 0.0);
    hip->position = vec4(0.0, 0.0, 0.0, 1.0);
    hip->theta = 0.0f;
    hip->verIndex = {0,1,2,3};
    hip->parent = nullptr;

    chest = new Bone();
    chest->axis = vec3(0.0, 1.0, 0.0);
    chest->position = vec4(0.0, 0.1, 0.0, 1.0);
    chest->theta = 0.0f;
    chest->verIndex = {4,5,6,7};
    chest->parent = hip;

    head = new Bone();
    head->axis = vec3(1.0, 0.0, 0.0);
    head->position = vec4(0.0, 0.6, 0.0, 1.0);
    head->theta = 0.0f;
    head->verIndex = {8,9,10,11};
    head->parent = chest;

    l_upper_arm = new Bone();
    l_upper_arm->axis = vec3(0.0, 0.0, 1.0);
    l_upper_arm->position = vec4(-0.35, 0.6, 0.0, 1.0);
    l_upper_arm->theta = M_PI;
    l_upper_arm->verIndex = {12,13,14,15};
    l_upper_arm->parent = chest;

    l_lower_arm = new Bone();
    l_lower_arm->axis = vec3(1.0, 0.0, 0.0);
    l_lower_arm->position = vec4(0.0, 0.4, 0.0, 1.0);
    l_lower_arm->theta = 0.0f;
    l_lower_arm->verIndex = {16,17,18,19};
    l_lower_arm->parent = l_upper_arm;

    l_hand = new Bone();
    l_hand->axis = vec3(0.1, 0.0, 0.0);
    l_hand->position = vec4(0.0, 0.4, 0.0, 1.0);
    l_hand->theta = 0.0f;
    l_hand->verIndex = {20,21,22,23};
    l_hand->parent = l_lower_arm;

    r_upper_arm = new Bone();
    r_upper_arm->axis = vec3(0.0, 0.0, 1.0);
    r_upper_arm->position = vec4(0.35, 0.6, 0.0, 1.0);
    r_upper_arm->theta = M_PI;
    r_upper_arm->verIndex = {24,25,26,27};
    r_upper_arm->parent = chest;

    r_lower_arm = new Bone();
    r_lower_arm->axis = vec3(0.1, 0.0, 0.0);
    r_lower_arm->position = vec4(0.0, 0.4, 0.0, 1.0);
    r_lower_arm->theta = 0.0f;
    r_lower_arm->verIndex = {28,29,30,31};
    r_lower_arm->parent = r_upper_arm;

    r_hand = new Bone();
    r_hand->axis = vec3(0.1, 0.0, 0.0);
    r_hand->position = vec4(0.0, 0.4, 0.0, 1.0);
    r_hand->theta = 0.0f;
    r_hand->verIndex = {32,33,34,35};
    r_hand->parent = r_lower_arm;

    l_upper_leg = new Bone();
    l_upper_leg->axis = vec3(1.0, 0.0, 0.0);
    l_upper_leg->position = vec4(-0.15, 0.0, 0.0, 1.0);
    l_upper_leg->theta = M_PI;
    l_upper_leg->verIndex = {36,37,38,39};
    l_upper_leg->parent = hip;

    l_lower_leg = new Bone();
    l_lower_leg->axis = vec3(1.0, 0.0, 0.0);
    l_lower_leg->position = vec4(0.0, 0.4, 0.0, 1.0);
    l_lower_leg->theta = 0.0f;
    l_lower_leg->verIndex = {40,41,42,43};
    l_lower_leg->parent = l_upper_leg;

    l_foot = new Bone();
    l_foot->axis = vec3(1.0, 0.0, 0.0);
    l_foot->position = vec4(0.0, 0.4, 0.0, 1.0);
    l_foot->theta = 0.0f;
    l_foot->verIndex = {44,45,46,47};
    l_foot->parent = l_lower_leg;

    r_upper_leg = new Bone();
    r_upper_leg->axis = vec3(1.0, 0.0, 0.0);
    r_upper_leg->position = vec4(0.15, 0.0, 0.0, 1.0);
    r_upper_leg->theta = M_PI;
    r_upper_leg->verIndex = {48,49,50,51};
    r_upper_leg->parent = hip;

    r_lower_leg = new Bone();
    r_lower_leg->axis = vec3(1.0, 0.0, 0.0);
    r_lower_leg->position = vec4(0.0, 0.4, 0.0, 1.0);
    r_lower_leg->theta = 0.0f;
    r_lower_leg->verIndex = {52,53,54,55};
    r_lower_leg->parent = r_upper_leg;

    r_foot = new Bone();
    r_foot->axis = vec3(1.0, 0.0, 0.0);
    r_foot->position = vec4(0.0, 0.4, 0.0, 1.0);
    r_foot->theta = 0.0f;
    r_foot->verIndex = {56,57,58,59};
    r_foot->parent = r_lower_leg;

}

void initializeAnimation(){
    // Animation frames
    //                                   hip  chest  head     lua      lla        lh    rua        rla         rh     lul        lll           lf    rul        rll            rf    hipx  hipy   hipz
    keyFrames[0] = *new Keyframe();
    keyFrames[0].time = 0.0f;
    keyFrames[0].values = vector<float>{-0.1f, 0.0f, 0.0f, M_PI,      M_PI-1.5f, 0.0f, M_PI,       M_PI-0.15f, 0.0f, 0.2f,      M_PI-0.1f, 0.0f, 0.2f,       M_PI-0.1f, 0.0f, 0.0f, -0.4, -0.1 };
    keyFrames[1] = *new Keyframe();
    keyFrames[1].time = 0.1f;
    keyFrames[1].values = vector<float>{-0.2f, 0.0f, 0.1f, M_PI-0.05, M_PI-1.5f, 0.0f, M_PI+0.05, M_PI-0.15f, 0.0f, 0.35f,      M_PI-0.3f, 0.0f, 0.35f,       M_PI-0.3f, 0.0f, 0.0f, -0.2, -0.1 };
    keyFrames[2] = *new Keyframe();
    keyFrames[2].time = 0.2;
    keyFrames[2].values = vector<float>{-0.4f, 0.0f, 0.2f, M_PI-0.1, M_PI-1.5f, 0.0f, M_PI+0.1   , M_PI-0.15f, 0.0f, 0.5f,      M_PI-0.7f, 0.0f, 0.5f,       M_PI-0.7f, 0.0f, 0.0f, -0.0, -0.1 };
    keyFrames[3] = *new Keyframe();
    keyFrames[3].time = 0.3f;
    keyFrames[3].values = vector<float>{-0.6f, 0.0f, 0.3f, M_PI-0.2, M_PI-1.5f, 0.0f, M_PI+0.2,   M_PI-0.15f, 0.0f, 0.7f,      M_PI-1.0f, 0.0f, 0.7f,       M_PI-1.0f, 0.0f, 0.0f, 0.1, -0.1 };
    keyFrames[4] = *new Keyframe();
    keyFrames[4].time = 0.333f;
    keyFrames[4].values = vector<float>{-0.4f, 0.0f, 0.3f, M_PI-0.3, M_PI-1.5f, 0.0f, M_PI+0.3,   M_PI-0.15f, 0.0f, 0.9f,      M_PI-1.2f, 0.0f, 0.9f,       M_PI-1.2f, 0.0f, 0.0f, 0.4, -0.1 };
    keyFrames[5] = *new Keyframe();
    keyFrames[5].time = 0.366f;
    keyFrames[5].values = vector<float>{-0.2f, 0.0f, 0.3f, M_PI-0.4, M_PI-1.5f, 0.0f, M_PI+0.4,   M_PI-0.15f, 0.0f, 1.3f,      M_PI-1.4f, 0.0f, 1.3f,       M_PI-1.4f, 0.0f, 0.0f, 0.8, -0.1 };
    keyFrames[6] = *new Keyframe();
    keyFrames[6].time = 0.4f;
    keyFrames[6].values = vector<float>{0.0f, 0.0f, 0.4f, M_PI-0.3, M_PI-1.5f, 0.0f, M_PI+0.3   , M_PI-0.15f, 0.0f, 1.8f,      M_PI-1.8f, 0.0f, 1.8f,       M_PI-1.8f, 0.0f, 0.0f, 1.0, -0.1 };
    keyFrames[7] = *new Keyframe();
    keyFrames[7].time = 0.433f;
    keyFrames[7].values = vector<float>{0.3f, 0.0f, 0.45f, M_PI-0.2, M_PI-1.5f, 0.0f, M_PI+0.2  , M_PI-0.15f, 0.0f, 2.2f,      M_PI-2.0f, 0.0f, 2.2f,       M_PI-2.0f, 0.0f, 0.0f, 1.5, -0.1 };
    keyFrames[8] = *new Keyframe();
    keyFrames[8].time = 0.466f;
    keyFrames[8].values = vector<float>{0.6f, 0.0f, 0.4f, M_PI-0.1, M_PI-1.5f, 0.0f, M_PI+ 0.1,   M_PI-0.15f, 0.0f, 2.8f,      M_PI-2.2f, 0.0f, 2.8f,       M_PI-2.2f, 0.0f, 0.0f, 2.0, -0.1 };
    keyFrames[9] = *new Keyframe();
    keyFrames[9].time = 0.5f;
    keyFrames[9].values = vector<float>{1.0f, 0.0f, 0.2f, M_PI,     M_PI-1.5f, 0.0f, M_PI,       M_PI-0.15f, 0.0f, M_PI-0.3f,  M_PI-2.4f, 0.0f, M_PI-0.3f,  M_PI-2.4f, 0.0f, 0.0f, 3.0, -0.1 };
    keyFrames[10] = *new Keyframe();
    keyFrames[10].time = 0.75f;
    keyFrames[10].values = vector<float>{2.0f, 0.0f, 0.1f, M_PI,     M_PI-1.5f, 0.0f, M_PI,       M_PI-0.15f, 0.0f, M_PI-0.1f,  M_PI-2.4f, 0.0f, M_PI-0.1f,  M_PI-2.4f, 0.0f, 0.0f, 1.5, -0.1 };
    keyFrames[11] = *new Keyframe();
    keyFrames[11].time = 1.0f;
    keyFrames[11].values = vector<float>{3.0f, 0.0f, 0.0f, M_PI,     M_PI-1.5f, 0.0f, M_PI,       M_PI-0.15f, 0.0f, M_PI-0.1f,  M_PI-2.4f, 0.0f, M_PI-0.1f,  M_PI-2.4f, 0.0f, 0.0f, 0.4, -0.1 };
    

}

void updateScene() {
    vec3 TfVertices[nv];
    vec3 TfNormals[nv];

    mat4 tf = hip->getMatrix();

    for(auto i: hip->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = chest->getMatrix();

    for(auto i: chest->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = head->getMatrix();

    for(auto i: head->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = l_upper_arm->getMatrix();

    for(auto i: l_upper_arm->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = l_lower_arm->getMatrix();

    for(auto i: l_lower_arm->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = l_hand->getMatrix();

    for(auto i: l_hand->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = r_upper_arm->getMatrix();

    for(auto i: r_upper_arm->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = r_lower_arm->getMatrix();

    for(auto i: r_lower_arm->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = r_hand->getMatrix();

    for(auto i: r_hand->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }


    tf = l_upper_leg->getMatrix();

    for(auto i: l_upper_leg->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = l_lower_leg->getMatrix();

    for(auto i: l_lower_leg->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = l_foot->getMatrix();

    for(auto i: l_foot->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = r_upper_leg->getMatrix();

    for(auto i: r_upper_leg->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    
    tf = r_lower_leg->getMatrix();

    for(auto i: r_lower_leg->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }

    tf = r_foot->getMatrix();

    for(auto i: r_foot->verIndex){
        TfVertices[i] = transformPoint(vertices[i], tf);
    }
    

	r.updateVertexAttribs(vertexBuf, nv, TfVertices);

    for(int i = 0; i < 15; i++){
        normals[4*i+0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[2]-vertices[0]));
        normals[4*i+1] = glm::normalize(glm::cross(vertices[3]-vertices[1], vertices[0]-vertices[1]));
        normals[4*i+2] = glm::normalize(glm::cross(vertices[2]-vertices[3], vertices[1]-vertices[3]));
        normals[4*i+3] = glm::normalize(glm::cross(vertices[0]-vertices[2], vertices[3]-vertices[2]));
    }
	r.updateVertexAttribs(normalBuf, nv, normals);
}

int main() {
	int width = 1000, height = 800;
	if (!r.initialize("Animation", width, height)) {
		return EXIT_FAILURE;
	}
	camCtl.initialize(width, height);
	camCtl.camera.setCameraView(vec3(1.5, 4.0, 5.0), vec3(0.5, -0.5, 0.0), vec3(0.0, 1.0, 0.0));
	program = r.createShaderProgram(
		r.vsBlinnPhong(),
		r.fsBlinnPhong()
	);

	initializeScene();
    initializeAnimation();

    int i = 0;
    vector<vector<float>> animation = interpolateVectors(keyFrames, numSamples);
    while (!r.shouldQuit()) {
        float t = SDL_GetTicks64()*3e-1;
        int i = (int)t % animation.size();
        // i = (i+1)% animation.size();

        hip->theta = animation[i][0];
        chest->theta = animation[i][1];
        head->theta = animation[i][2];
        l_upper_arm->theta = animation[i][3];
        l_lower_arm->theta = animation[i][4];
        l_hand->theta = animation[i][5];
        r_upper_arm->theta = animation[i][6];
        r_lower_arm->theta = animation[i][7];
        r_hand->theta = animation[i][8];
        l_upper_leg->theta = animation[i][9];
        l_lower_leg->theta = animation[i][10];
        l_foot->theta = animation[i][11];
        r_upper_leg->theta = animation[i][12];
        r_lower_leg->theta = animation[i][13];
        r_foot->theta = animation[i][14];
        hip->position = vec4(animation[i][15], animation[i][16], animation[i][17], 1.0f);
        updateScene();

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
        i++;
        i %= animation.size();
	}
}
