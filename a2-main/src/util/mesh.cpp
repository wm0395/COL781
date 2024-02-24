#include "mesh.hpp"
#include <iostream>

inline Mesh::Mesh(int V, int N, Vertex *vertex){
    num_of_vertices = V;
    num_of_faces = N;
    starting_vertex = vertex;
    visited_vertices = std::vector<bool>(V, false);
    visited_faces = std::vector<bool>(N, false);
    visited_vert_count = 0;

    vertices = new vec3[V];
    normals = new vec3[V];
    triangles = new ivec3[N];

    for (int i = 0; i<num_of_vertices; i++){
        vertices[i] = vec3(0.0, 0.0, 0.0);
        normals[i] = vec3(0.0, 0.0, 0.0);
    }
    for (int i = 0; i<num_of_faces; i++){
        triangles[i] = ivec3(0,0,0);
    }

    populate_mesh();
    // vertices = std::vector<vec3>(V, vec3(0.0, 0.0, 0.0));
    // normals = std::vector<vec3>(V, vec3(0.0, 0.0, 0.0));
    // triangles = std::vector<ivec3>(N, ivec3(0, 0, 0));
}

vec3* Mesh::give_vertices(){
    return vertices;
}

vec3* Mesh::give_normals(){
    return normals;
}

ivec3* Mesh::give_triangles(){
    return triangles;
}

void Mesh::give_initial_mesh(){
    std::cout << "Number of vertices => " << num_of_vertices << "\n";
    std::cout << "Starting vertex => " << starting_vertex->index << "\n";
    std::cout << "\n";
}

void Mesh::print_vis_vert(){
    std::cout << "Visited vertices => ";
    for (int i = 0; i<num_of_vertices; i++) std::cout << visited_vertices[i] << " ";
    std::cout << "\n";
}

void Mesh::print_vis_faces(){
    std::cout << "Visited face => ";
    for (int i = 0; i<num_of_faces; i++) std::cout << visited_vertices[i] << " ";
    std::cout << "\n";
}

void Mesh::populate_mesh(){   
    dfs(starting_vertex, nullptr, nullptr);
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);

    // for (int i = 0; i<num_of_vertices; i++){
    //     vec3 pos = vertices[i];
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    // }
    // std::cout << "\n";

    // for (int i = 0; i<num_of_vertices; i++){
    //     vec3 pos = normals[i];
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    // }
    // std::cout << "\n";

    // for (int i = 0; i<num_of_faces; i++){
    //     ivec3 pos = triangles[i];
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    // }
}

void Mesh::dfs_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face)){
    // face->print_face_vertices();
    if (!visited_faces[face->index]){
        if(fac_opr){
            fac_opr(face);
        }
        visited_faces[face->index] = true;

        // std::cout << "printing face vertices => ";
        // print_vis_faces();
        std::vector<Vertex*> vert = face->face_vertices();
        // std::cout << "doabara => ";
        // std::cout << vert[0]->index << " " << vert[1]->index << " " << vert[2]->index << "\n";
        ivec3 v = face->get_face_vertices_indices();
        triangles[face->index] = v;
        dfs(vert[0], vtx_opr, fac_opr);
        dfs(vert[1], vtx_opr, fac_opr);
        dfs(vert[2], vtx_opr, fac_opr);
    }
}

void Mesh::dfs(Vertex *v, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face)){
    // std::cout<< "dfs begin +> " << v->index <<"\n";
    if (!visited_vertices[v->index]){
        // std::cout << v->index << "\n";
        visited_vertices[v->index] = true;
        // print_vis_vert();
        visited_vert_count++;
        vertices[v->index] = v->position;
        normals[v->index] = v->normal;

        // std::cout << vertices[v->index].x << " " <<  vertices[v->index].y << " " <<  vertices[v->index].z << "\n";
        
        v->traverse(&Mesh::dfs_helper, *this, vtx_opr, fac_opr);
        
        if(vtx_opr){
            vtx_opr(v);
        }
    }
    // std::cout<< "dfs end +> " << v->index <<"\n";
    if (visited_vert_count == num_of_vertices) return;
}

void add_face_normal(Face *face, Vertex *vertex){
    vertex->normal += face->calculate_normal();
}

void avg_normals(Vertex *vertex){
    vertex->normal = vec3(0,0,0);
    int n = vertex->traverse(add_face_normal);
    vertex->normal = vec3(vertex->normal.x/n, vertex->normal.y/n, vertex->normal.z/n);
}

void Mesh::recompute_normals(){
    dfs(starting_vertex, avg_normals, nullptr);
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
}