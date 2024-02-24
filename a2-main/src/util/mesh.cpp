#include "mesh.hpp"
#include <iostream>

inline Mesh::Mesh(int V, int N, Vertex *vertex){
    num_of_vertices = V;
    num_of_faces = N;
    starting_vertex = vertex;
    visited_vertices = std::vector<bool>(V, false);
    visited_faces = std::vector<bool>(N, false);
}

void Mesh::give_initial_mesh(){
    std::cout << "Number of vertices => " << num_of_vertices << "\n";
    std::cout << "Starting vertex => " << starting_vertex->index << "\n";
}

void Mesh::print_vis_vert(std::vector<bool> &visited_vertices){
    for (int i = 0; i<visited_vertices.size(); i++) std::cout << visited_vertices[i] << " ";
    std::cout << "\n";
}

void Mesh::populate_mesh(){   
    for (int i = 0; i<num_of_vertices; i++){
        vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
        normals.push_back(glm::vec3(0.0, 0.0, 0.0));
    }

    int visited_vert_count = 0;

    dfs(starting_vertex, visited_vert_count);
}

void Mesh::dfs_helper(Face *face, void (*fac_opr)(Face *face)){
    if (!visited_faces[face->index]){
        if(fac_opr){
            fac_opr(face);
        }
        visited_faces[face->index] = true;
        triangles.push_back(face->get_face_vertices());
    }
}

void Mesh::dfs(Vertex *v, int &visited_vert_count, void (*vtx_opr)(Vertex *vertex) = nullptr , void (*fac_opr)(Face *face)){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        
        vertices[v->index] = *v->position;    // yeh doubtful hai abhi
        normals[v->index] = *v->normal;
        
        v->traverse(&Mesh::dfs_helper, *this, fac_opr);
        if(vtx_opr){
            vtx_opr(v);
        }
    }

    if (visited_vert_count == num_of_vertices) return;
}