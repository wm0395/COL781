#include "mesh.hpp"
#include <iostream>

inline Mesh::Mesh(int n, Vertex *vertex){
    num_of_vertices = n;
    starting_vertex = vertex;
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
        visited_vertices.push_back(false);
        vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
        normals.push_back(glm::vec3(0.0, 0.0, 0.0));
    }
    covered_faces.empty();
    
    int visited_vert_count = 0;
    int visited_face_count = 0;

    dfs(starting_vertex, visited_vert_count, visited_face_count);
}

void Mesh::dfs(Vertex *v, int &visited_vert_count, int &visited_face_count){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        print_vis_vert(visited_vertices);
        // vertices[visited_vert_count] = *v->position;
        // normals[visited_vert_count] = *v->normal;
        vertices[v->index] = *v->position;    // yeh doubtful hai abhi
        std::cout << vertices[v->index].x << " " << vertices[v->index].y << " " << vertices[v->index].z << "\n";
        normals[v->index] = *v->normal;
        HalfEdge *he = v->halfedge;
        HalfEdge *initial = v->halfedge;
        std::cout << he <<" " << he->next << "\n";
        std::cout << he->head->index << " " << he->next->head->index << "\n";
        // std::cout << he->head->index << "\n";
        bool boundary = false;
        do{
            Face *face = he->left;
            // std::cout << face->get_face_vertices().x  << " " << face->get_face_vertices().y  << " " << face->get_face_vertices().z << "\n";
            // std::cout << face->get_face_vertices().x << " " << face->get_face_vertices().y << " " << face->get_face_vertices().z << "\n";
            if (covered_faces.empty()){
                covered_faces.insert(face->index);
                std::cout << face->index << "\n";
                // triangles.push_back(face->get_face_vertices());
                visited_face_count++;
            }
            else if (covered_faces.find(face->index) != covered_faces.end()){
                std::cout << "here\n";
                covered_faces.insert(face->index);
                std::cout << face->index << "\n";
                triangles.push_back(face->get_face_vertices());
                visited_face_count++;
            }

            std::cout << "here1\n";
            std::cout << he->next->pair->head->index << "\n";
            if (he->next->pair){
                he = he->next;
                dfs(he->head, visited_vert_count, visited_face_count);
                he = he->pair;
            }
            else{
                std::cout << "boundary vertex\n";
                boundary = true;
                break;
            }
        }while(he != initial);

        he = initial->pair;

        while(he && boundary){
            Face *face = he->left;
            if (covered_faces.find(face->index) != covered_faces.end()){
                covered_faces.insert(face->index);
                triangles.push_back(face->get_face_vertices());
                visited_face_count++;
            }
            he = he->next->next;
            dfs(he->head, visited_vert_count, visited_face_count);
            he = he->pair;
        }
    }

    if (visited_face_count == num_of_vertices) return;
}