#include "viewer.hpp"
#include "./util/util.hpp"
#include "./util/square_grid.cpp"
#include <vector>
#include <iostream>

namespace V = COL781::Viewer;
using namespace glm;
using namespace std;

int main() {

    // To make the square grid, run the following =>
    int m = 3, n = 3;
    Mesh *mesh = new Mesh((m+1)*(n+1), 2*m*n);
    square_grid(mesh, m, n);
    // mesh->flip_edge(6,9);    // to flip the middle diagonal of the 3*3 grid, (we are providing the vertex indices as our inputs)
    // mesh->split_edge(6,9);    // to spli the middle diagonal of the 3*3 grid, (we are providing the vertex indices as our inputs)
    // mesh->collapse_edge(6,9);   // to collapse the middle diagonal of the 3*3 grid, (we are providing the vertex indices as our inputs)

    // To make the sphere, run the following  =>
    // int m = 4, n = 4;
    // int vert_cnt = m*(n-1)+2;
    // int tri_cnt = 2*m*(n-1);
    // Mesh *mesh = new Mesh(vert_cnt, tri_cnt);
    // sphere_grid(mesh, m, n);


    // To run the example files =>
    // string file = "meshes/noisycube.obj";
    // Mesh *mesh = new Mesh(file);
    // mesh->recompute_normals();     // if you want to recompute normals without weights associated with neibhouring faces
    // mesh->recompute_normals_weighted();    // if you want to compute normals with weights of neighbouring faces as their areas
    // mesh->naive_smoothing(5, 0.02);    // to carry out naive smoothing
    // mesh->taubin_smoothing(2, 0.33, -0.34);    // to carry out taubin_smoothing
    // mesh = mesh->loop_subdivide();     // to carry out loop subdivision


    V::Viewer v;
    if (!v.initialize("Mesh viewer", 640, 480)) {
        return EXIT_FAILURE;
    }
    v.setVertices(mesh->num_of_vertices, mesh->vertices);
    v.setNormals(mesh->num_of_vertices, mesh->normals);
    v.setTriangles(mesh->num_of_faces, mesh->triangles);
    v.view(); 
} 