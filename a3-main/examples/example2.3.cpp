#include <glm/glm.hpp>
#include <iostream>
#include "../src/Scene.cpp"

using namespace std;
using namespace glm;

int main(){

    Scene *scene = new Scene();
    Camera *cam = new Camera();
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    cam->up = vec3(0.0f, 1.0f, 0.0f);
    cam->lookAt = vec3(0.0f, 0.0f, -1.0f);
    cam->fov = 60.0f;
    cam->near_plane = -1.0f;
    cam->far_plane = -1000.0f;
    cam->aspect = 640.0f / 480.0f;
    cam->updateViewMatrix();
    // cam->translate_camera(vec3(0.5f, 0.0f, 0.5f));
    // cam->rotate_camera(45.0f, vec3(0.0f, 1.0f, 1.0f));
    scene->camera = cam;

    vector<Shape*> objects = {};

    string file = "meshes/cube.obj";
    Mesh *mesh = new Mesh(file);

    mat4 projection_mat = cam->getProjectionMatrix();
    mat4 inverse_proj_mat = inverse(projection_mat);
    float max_z = -1000.0f;
    for (int i = 0; i<mesh->num_of_vertices; i++){
        cout << mesh->vertices[i].x << " " << mesh->vertices[i].y << " " << mesh->vertices[i].z + 0.5f << endl;
        vec4 vertex = vec4(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z, 1.0f);
        vertex = inverse_proj_mat * vertex;
        mesh->vertices[i] = vec3(vertex) / vertex.w;
        if (mesh->vertices[i].z > max_z){
            max_z = mesh->vertices[i].z;
        }

        //print them
        cout << mesh->vertices[i].x << " " << mesh->vertices[i].y << " " << mesh->vertices[i].z << endl;
    }

    for (int i = 0; i<mesh->num_of_vertices; i++){
        mesh->vertices[i].z = mesh->vertices[i].z - max_z - 1.0f;
        cout << mesh->vertices[i].x << " " << mesh->vertices[i].y << " " << mesh->vertices[i].z << endl;
    }

    for (int i = 0; i<mesh->num_of_faces; i++){
        vec4 p0 = vec4(mesh->vertices[mesh->triangles[i].x], 1.0f);
        vec4 p1 = vec4(mesh->vertices[mesh->triangles[i].y], 1.0f);
        vec4 p2 = vec4(mesh->vertices[mesh->triangles[i].z], 1.0f);
        Triangle* tri = new Triangle(p0, p1, p2);
        objects.push_back(tri);
        if (i == 3){
            break;
        }
    }

    scene->objects = objects;
    cout << scene->objects.size() << endl;

    vector<Light*> lights = {};

    // vec4 position1 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity1 = vec4(50.0f, 0.0f, 0.0f, 1.0f);
    // Light *light1 = new Light();
    // light1->Intensity = intensity1;
    // light1->position = position1;
    // lights.push_back(light1);

    // vec4 position2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity2 = vec4(0.0f, 0.0f, 15.0f, 1.0f);
    // Light *light2 = new Light();
    // light2->Intensity = intensity2;
    // light2->position = position2;
    // lights.push_back(light2);

    // vec4 position3 = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    // vec4 intensity3 = vec4(0.0f, 50.0f, 0.0f, 1.0f);
    // Light *light3 = new Light();
    // light3->Intensity = intensity3;
    // light3->position = position3;
    // lights.push_back(light3);

    vec4 position4 = vec4(0.0f, 3.0f, -2.5f, 1.0f);
    vec4 intensity4 = vec4(100.0f, 100.0f, 100.0f, 100.0f);
    Light *light4 = new Light();
    light4->Intensity = intensity4;
    light4->position = position4;
    lights.push_back(light4);
    
    scene->lights = lights;

    Ray_Tracer r;
    if (!r.initialize("Example 3", 640, 480, 1, 1, 1, "point_lambert")){
        std::cout << "failure to initialise\n";
        return EXIT_FAILURE;
    }


    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.draw(scene);
        r.show();
    }
    return EXIT_SUCCESS;
}