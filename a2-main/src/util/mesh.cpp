#include "mesh.hpp"
#include <iostream>

inline Mesh::Mesh(int V, int N, Vertex *vertex){
    num_of_vertices = V;
    num_of_faces = N;
    starting_vertex = vertex;
    visited_vertices = std::vector<bool>(V, false);
    visited_faces = std::vector<bool>(N, false);
    visited_vert_count = 0;
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

    dfs(starting_vertex);
    visited_vert_count = 0;
}

void Mesh::dfs_helper(Face *face){
    if (!visited_faces[face->index]){
        visited_faces[face->index] = true;
        std::vector<Vertex*> vert = face->face_vertices();
        ivec3 v = face->get_face_vertices_indices();
        triangles.push_back(v);
        dfs(vert[0]);
        dfs(vert[1]);
        dfs(vert[2]);
    }
}

void Mesh::dfs(Vertex *v){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        vertices[v->index] = *v->position;
        normals[v->index] = *v->normal;
        HalfEdge *he = v->halfedge;
        HalfEdge *initial = v->halfedge;
        bool boundary = false;
        do{
            Face *face = he->left;
            dfs_helper(face);

            if (he->next->pair){
                he = he->next;
                dfs(he->head);
                he = he->pair;
            }
            else{
                boundary = true;
                break;
            }
        }while(he != initial);

        he = initial->pair;

        while(he && boundary){
            Face *face = he->left;
            dfs_helper(face);
            he = he->next->next;
            dfs(he->head);
            he = he->pair;
        }
    }

    if (visited_vert_count == num_of_vertices) return;
}