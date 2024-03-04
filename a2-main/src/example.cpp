#include "viewer.hpp"
#include "./util/util.hpp"
#include "./util/square_grid.cpp"
// #include "./util/sphere_grid.cpp"
#include <vector>
#include <iostream>

namespace V = COL781::Viewer;
using namespace glm;
using namespace std;

int main() {

    // To run the example given initially, decomment the lines giving the values of vertices, normals, triangles

    // vec3 vertices[] = {
    //     vec3(-0.5, -0.5, 0.0),
    //     vec3( 0.5, -0.5, 0.0),
    //     vec3(-0.5,  0.5, 0.0),
    //     vec3( 0.5,  0.5, 0.0)
    // };
    // vec3 normals[] = {
    //     vec3(0.0, 0.0, 1.0),
    //     vec3(0.0, 0.0, 1.0),
    //     vec3(0.0, 0.0, 1.0),
    //     vec3(0.0, 0.0, 1.0)
    // };
    // ivec3 triangles[] = {
    //     ivec3(0, 1, 2),
    //     ivec3(1, 2, 3),
    // };

    // Mesh *mesh = new Mesh(4, 2, vertices, normals, triangles);

    // Make the square grid, run the following =>
    // Mesh* mesh = square_grid(2,2);

    // Make the sphere usign the following command =>
    // Mesh *mesh = sphere_grid(4,2);


    // To run the example files =>
    // string file = "meshes/teapot.obj";
    // Mesh *mesh = new Mesh(file);
    // mesh->recompute_normals();     // if you want to recompute normals

    int m = 3, n = 3;
    vec3 vertices[(m+1)*(n+1)];
    vec3 normals[(m+1)*(n+1)];
    ivec3 triangles[2*m*n];

    Mesh *mesh = new Mesh((m+1)*(n+1), 2*m*n);
    square_grid(mesh, m, n);

    mesh->edge_flip(mesh->f2f[9]->halfedge->next);
    // int vert_cnt = m*(n-1)+2;
    // int tri_cnt = 2*m*(n-1);

    // vec3 vertices[vert_cnt];
    // vec3 normals[vert_cnt];
    // ivec3 triangles[tri_cnt];

    // Mesh *mesh = new Mesh(vert_cnt, tri_cnt);
    // sphere_grid(mesh, m, n);

    // std::cout << mesh->vertices << "\n";
    // for (int i = 0; i<mesh->num_of_vertices; i++){
    //     vec3 pos = mesh->vertices[i];
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    // }

    // std::cout << mesh->triangles << "\n";
    // for (int i = 0; i<mesh->num_of_faces; i++){
    //     ivec3 pos = mesh->triangles[i];
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    // }

    V::Viewer v;
    if (!v.initialize("Mesh viewer", 640, 480)) {
        return EXIT_FAILURE;
    }
    v.setVertices(mesh->num_of_vertices, mesh->vertices);
    v.setNormals(mesh->num_of_vertices, mesh->normals);
    v.setTriangles(mesh->num_of_faces, mesh->triangles);
    v.view(); 
} 