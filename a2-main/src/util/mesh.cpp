#include "mesh.hpp"

void Mesh::populate_mesh(){   
    for (int i = 0; i<num_of_vertices; i++) visited_vertices.push_back(false);
    
    int visited_vert_count = 0;
    int visited_face_count = 0;

    dfs(starting_vertex, visited_vert_count, visited_face_count);
}

void Mesh::dfs(Vertex *v, int &visited_vert_count, int &visited_face_count){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        // vertices[visited_vert_count] = *v->position;
        // normals[visited_vert_count] = *v->normal;
        vertices[v->index] = *v->position;    // yeh doubtful hai abhi
        normals[v->index] = *v->normal;
        HalfEdge *he = v->halfedge;
        HalfEdge *initial = v->halfedge;
        bool boundary = false;
        do{
            Face *face = he->left;
            if (covered_faces.find(face->index) != covered_faces.end()){
                covered_faces.insert(face->index);
                triangles[visited_face_count] = face->get_face_vertices();
                visited_face_count++;
            }

            if (he->next->pair){
                he = he->next;
                dfs(he->head, visited_vert_count, visited_face_count);
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
            if (covered_faces.find(face->index) != covered_faces.end()){
                covered_faces.insert(face->index);
                triangles[visited_face_count] = face->get_face_vertices();
                visited_face_count++;
            }
            he = he->next->next;
            dfs(he->head, visited_vert_count, visited_face_count);
            he = he->pair;
        }
    }

    if (visited_face_count == num_of_vertices) return;
}