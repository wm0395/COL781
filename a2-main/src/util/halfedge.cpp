#include "mesh.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

void Vertex::traverse(void (*func)(Face *face)){
    HalfEdge *he = halfedge;
    bool boundary = false;
    do{
        Face *face = he->left;
        func(face);
        //check for boundary
        if(he->next->pair){
            he = he->next->pair;
        }
        else{
            boundary = true;
            break;
        }

    }while(he != halfedge);

    if(!halfedge->pair) return;
    he = halfedge->pair->next->next;
    // reverse traversal for opposite of boundary
    while(he && boundary){
        Face *face = he->left;
        func(face);
        if(!he->pair) return;
        he = he->pair->next->next;
    }
}

int Vertex::traverse(void (*func)(Face *face, Vertex *vertex)){
    HalfEdge *he = halfedge;
    bool boundary = false;
    int n = 0;
    do{
        Face *face = he->left;
        func(face, this);
        n++;
        //check for boundary
        if(he->next->pair){
            he = he->next->pair;
        }
        else{
            boundary = true;
            break;
        }

    }while(he != halfedge);

    if(!halfedge->pair) return n;
    he = halfedge->pair->next->next;
    // reverse traversal for opposite of boundary
    while(he && boundary){
        n++;
        Face *face = he->left;
        func(face, this);
        if(!he->pair) return n;
        he = he->pair->next->next;
    }
    return n;
}

void Vertex::traverse(void (Mesh::*func)(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update), Mesh &mesh, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update){
    HalfEdge *he = halfedge;
    bool boundary = false;
    do{
        Face *face = he->left;
        (mesh.*func)(face, vtx_opr,fac_opr, VL_update, HE_update);
        //check for boundary
        if(he->next->pair){
            he = he->next->pair;
        }
        else{
            boundary = true;
            break;
        }

    }while(he != halfedge);
    
    if(!halfedge->pair) return;
    he = halfedge->pair->next->next;
    // reverse traversal for opposite of boundary
    while(he && boundary){
        Face *face = he->left;
        (mesh.*func)(face, vtx_opr, fac_opr, VL_update, HE_update);
        if(!he->pair) return;
        he = he->pair->next->next;
    }
}

void Face::traverse(void (*func)(Vertex *vertex)){
    HalfEdge *he = halfedge;
    do{
        Vertex *vertex = he->head;
        func(vertex);
        he = he->next;

    }while(he != halfedge);
}

vector<Vertex*> Face::face_vertices(){
    std::vector<Vertex*> vert = {};

    HalfEdge *he = this->halfedge;
    do{
        Vertex *vertex = he->head;
        vert.push_back(vertex);
        he = he->next;

    }while(he != this->halfedge);

    return vert;
}

ivec3 Face::get_face_vertices_indices(){
    std::vector<Vertex*> vert = face_vertices();
    return ivec3(vert[0]->index, vert[1]->index, vert[2]->index);
}

void Face::print_face_vertices(){
    ivec3 v = get_face_vertices_indices();
    std::cout << "Face vertices => " << v.x << " " << v.y << " " << v.z << "\n";
}

vec3 Face::calculate_normal(){
    vec3 a = (halfedge->head->position), b = (halfedge->next->head->position), c = (halfedge->next->next->head->position);
    vec3 norm = glm::cross(b-a, c-a);
    return norm;
}
