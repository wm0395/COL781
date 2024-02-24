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

void Mesh::dfs_helper(Face *face){
    if (!visited_faces[face->index]){
        visited_faces[face->index] = true;
        triangles.push_back(face->get_face_vertices());
    }
}

void Mesh::dfs(Vertex *v, int &visited_vert_count){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        // print_vis_vert(visited_vertices);
        // vertices[visited_vert_count] = *v->position;
        // normals[visited_vert_count] = *v->normal;
        vertices[v->index] = *v->position;    // yeh doubtful hai abhi
        // std::cout << vertices[v->index].x << " " << vertices[v->index].y << " " << vertices[v->index].z << "\n";
        normals[v->index] = *v->normal;
        HalfEdge *he = v->halfedge;
        HalfEdge *initial = v->halfedge;
        // std::cout << he <<" " << he->next << "\n";
        // std::cout << he->head->index << " " << he->next->head->index << "\n";
        // std::cout << he->head->index << "\n";
        bool boundary = false;
        do{
            Face *face = he->left;
            // std::cout << face->get_face_vertices().x  << " " << face->get_face_vertices().y  << " " << face->get_face_vertices().z << "\n";
            // std::cout << face->get_face_vertices().x << " " << face->get_face_vertices().y << " " << face->get_face_vertices().z << "\n";
            dfs_helper(face);

            std::cout << "here1\n";
            // std::cout << he->next->pair->head->index << "\n";
            // std::
            if (he->next->pair){
                // std::cout << "hereok\n";
                he = he->next;
                dfs(he->head, visited_vert_count);
                he = he->pair;
            }
            else{
                std::cout << "boundary vertex\n";
                boundary = true;
                break;
            }
        }while(he != initial);

        std::cout << initial->head->index <<"\n";
        he = initial->pair;
        // std::cout <

        while(he && boundary){
            Face *face = he->left;
            dfs_helper(face);
            he = he->next->next;
            dfs(he->head, visited_vert_count);
            he = he->pair;
        }
    }

    if (visited_vert_count == num_of_vertices) return;
}