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

    he = halfedge->pair->next->next;
    // reverse traversal for opposite of boundary
    while(he && boundary){
        Face *face = he->left;
        func(face);
        he = he->pair->next->next;
    }
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

int hash_func(int i, int j, int n){
    return n*std::min(i,j) + std::max(i,j);
}

void get_vflist(HalfEdge* &head, vector<vec3>& vertex, vector<vec3>& normal, vector<ivec3>& face){
    int N = face.size();
    int V = vertex.size();
    vector<Vertex*> v2v(V, nullptr); //vertex-list to vertex pointer
    unordered_map<int, vector<HalfEdge*>> v2h;
    vector<Face*> f2f(N, nullptr);//face-list to face pointer
    for(int i = 0; i < V; i++){
        v2v[i] = new Vertex();
        v2v[i]->index = i;
        v2v[i]->position = &vertex[i];
        v2v[i]->normal = &normal[i];
    }

    // for (int i = 0; i<V; i++){
    //     Vertex *v = v2v[i];
    //     std::cout << "vertex => " << v << "\n";
    //     std::cout << "Index => " << v->index << "\n";
    //     vec3 pos = *v->position;
    //     std::cout << "position => " << pos.x << " " << pos.y << " " << pos.z << "\n";
    //     std::cout << "\n";
    // }

    for(int i = 0; i < N; i++){
        f2f[i] = new Face();
        f2f[i]->index = i;
        
        HalfEdge *he = new HalfEdge();
        f2f[i]->halfedge = he;
        he->left = f2f[i]; 
        v2v[face[i].x]->halfedge = he;
        he->head = v2v[face[i].x];
        int hxsh = hash_func(face[i].z, face[i].x, N);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he);

        he->next = new HalfEdge();
        he->next->left = f2f[i];
        v2v[face[i].y]->halfedge = he->next;
        he->next->head = v2v[face[i].y];
        hxsh = hash_func(face[i].x, face[i].y, N);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he->next);

        he->next->next = new HalfEdge();
        he->next->next->left = f2f[i];
        v2v[face[i].z]->halfedge = he->next->next;
        he->next->next->head = v2v[face[i].z];
        hxsh = hash_func(face[i].y, face[i].z, N);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he->next->next);
        
        he->next->next->next = he;
    }
    for(auto vhe: v2h){
        if(vhe.second.size() < 2) continue;
        vhe.second[0]->pair = vhe.second[1];
        vhe.second[1]->pair = vhe.second[0];
    }
    head = v2v[0]->halfedge;
}