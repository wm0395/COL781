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

void Vertex::traverse(void (Mesh::*func)(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face)), Mesh &mesh, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face)){
    HalfEdge *he = halfedge;
    bool boundary = false;
    do{
        Face *face = he->left;
        (mesh.*func)(face, vtx_opr,fac_opr);
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
        (mesh.*func)(face, vtx_opr, fac_opr);
        if(!he->pair) return;
        he = he->pair->next->next;
    }
}

void HalfEdge::split_halfedge(Mesh* mesh){
    Vertex* v1 = this->head;
    Vertex* v2 = this->pair->head;
    Vertex* v3 = this->next->head;
    Vertex* v4 = this->pair->next->head;
    Vertex* v = new Vertex;
    v->position = ((v1->position) + (v2->position));
    v->position /= 2;
    v->normal = (v1 -> normal) + (v2->normal);
    v->normal /= 2;
    v->index = mesh->num_of_vertices;
    mesh->num_of_vertices++;

    HalfEdge* h1 = new HalfEdge;
    HalfEdge* h2 = new HalfEdge;
    HalfEdge* h3 = new HalfEdge;
    HalfEdge* h4 = new HalfEdge;
    HalfEdge* h5 = new HalfEdge;
    HalfEdge* h6 = new HalfEdge;
    HalfEdge* h7 = new HalfEdge;
    HalfEdge* h8 = new HalfEdge;

    h1->head = v;
    h1->pair = h2;
    h4->head = v;
    h4->pair = h3;
    h5->head = v;
    h5->pair = h6;
    h8->head = v;
    h8->pair = h7;
    h1->next = h6;
    h5->next = h3;
    h4->next = h7;
    h8->next = h2;

    v->halfedge = h1;

    Face* f1 = new Face;
    Face* f2 = new Face;
    Face* f3 = new Face;
    Face* f4 = new Face;

    f1->halfedge = h1;
    f2->halfedge = h2;
    f3->halfedge = h4;
    f4->halfedge = h3;

    h1->left = f1;
    h2->left = f2;
    h3->left = f4;
    h4->left = f3;
    h5->left = f4;
    h6->left = f1;
    h7->left = f3;
    h8->left = f2;

    // mesh->vertices[mesh->num_of_vertices] = v->position;
    vec3* vert = new vec3[mesh->num_of_vertices];
    copy(mesh->vertices, mesh->vertices + mesh->num_of_vertices - 1, vert);
    vert[mesh->num_of_vertices - 1] = v->position;
    mesh->vertices = vert;

    vec3* norm = new vec3[mesh->num_of_vertices];
    copy(mesh->normals, mesh->normals + mesh->num_of_vertices - 1, norm);
    norm[mesh->num_of_vertices - 1] = v->normal;
    mesh->normals = norm;

    ivec3* faces = new ivec3[mesh->num_of_faces + 2];
    int face_cnt = 0;
    for (int i = 0; i<mesh->num_of_faces; i++){
        if (!(check_same_face(mesh->triangles[i], ivec3(v1->index, v3->index, v2->index)) || check_same_face(mesh->triangles[i], ivec3(v1->index, v2->index, v4->index)))){
            faces[face_cnt] = mesh->triangles[i];
            face_cnt++;
        }
    }
    faces[face_cnt] = ivec3(v->index, v3->index, v2->index);
    f1->index = face_cnt;
    face_cnt++;

    faces[face_cnt] = ivec3(v->index, v2->index, v4->index);
    f2->index = face_cnt;
    face_cnt++;

    faces[face_cnt] = ivec3(v->index, v4->index, v1->index);
    f3->index = face_cnt;
    face_cnt++;

    faces[face_cnt] = ivec3(v->index, v1->index, v3->index);
    f4->index = face_cnt;
    face_cnt++;

    mesh->triangles = faces;
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

bool check_same_face(ivec3 v1, ivec3 v2){
    int a = v1.x;
    int b = v1.y;
    int c = v1.z;
    int x = v2.x;
    int y = v2.y;
    int z = v2.z;

    if (a==x && b==y && c==z) return true;
    if (a==y && b==z && c==x) return true;
    if (a==z && b==x && c==y) return true;
    return false;
}
