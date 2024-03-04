#include "mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

inline Mesh::Mesh(int V, int N){
    num_of_vertices = V;
    num_of_faces = N;
    
    v2v = new Vertex*[V];
    f2f = new Face*[N];
    
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

    visited_vertices = std::vector<bool>(V, false);
    visited_faces = std::vector<bool>(N, false);
    visited_vert_count = 0;
}

inline Mesh::Mesh(int V, int N, Vertex **v2v, Face **f2f){
    num_of_vertices = V;
    num_of_faces = N;

    this->v2v = v2v;
    this->f2f = f2f;

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

    update_VFlist();
}

inline Mesh::Mesh(int V, int N, vec3 &vertex, vec3 &normal, ivec3 &face){
    num_of_vertices = V;
    num_of_faces = N;

    this->vertices = &vertex;
    this->normals = &normal;
    this->triangles = &face;

    visited_vertices = std::vector<bool>(V, false);
    visited_faces = std::vector<bool>(N, false);
    visited_vert_count = 0;

    v2v = new Vertex*[V];
    f2f = new Face*[N];

    update_HElist();
}

inline Mesh::Mesh(string file){
    parse_OBJ(file.c_str());

    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
    visited_vert_count = 0;

    v2v = new Vertex*[num_of_vertices];
    f2f = new Face*[num_of_faces];

    update_HElist();
}

void Mesh::parse_OBJ(const char *filename){
    std::vector<vec3> vertex;
    std::vector<vec3> normal;
    std::vector<ivec3> face;
    std::ifstream in(filename, std::ios::in);

    bool nn = false;
    
    if (!in){
        std::cerr << "Cannot open " << filename << std::endl;
        exit(1);

    }
    std::string line;
    while (std::getline(in, line))
    {
        //check v for vertices
        if (line.substr(0,2)=="v "){
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x,y,z;
            v>>x;v>>y;v>>z;
            vert=glm::vec3(x,y,z);
            vertex.push_back(vert);
        }
        //check for texture co-ordinate
        else if(line.substr(0,2)=="vn"){

            nn = true;
            std::istringstream v(line.substr(3));
            glm::vec3 nor;
            float x,y,z;
            v>>x;v>>y;v>>z;
            nor=glm::vec3(x,y,z);
            normal.push_back(nor);

        }
        //check for faces
        else if(line.substr(0,2)=="f " && nn){
            int a,b,c; //to store mesh index
            int A,B,C; //to store texture index
            const char* chh=line.c_str();
            sscanf (chh, "f %i//%i %i//%i %i//%i",&a,&A,&b,&B,&c,&C); //here it read the line start with f and store the corresponding values in the variables
            a--;b--;c--;
            A--;B--;C--;\
            glm::ivec3 fac = ivec3(a,b,c);
            face.push_back(fac);
        }

        else if (line.substr(0,2)=="f " && !nn){
            int a,b,c;
            const char* chh=line.c_str();
            sscanf(chh, "f %i %i %i", &a, &b, &c);
            a--;b--;c--;
            glm::ivec3 fac = ivec3(a,b,c);
            face.push_back(fac);

        }

    }

    if (!nn){
        for (int i = 0; i<vertex.size(); i++){
            normal.push_back(glm::vec3(0.0, 0.0, 0.0));
        }
    }
    
    vertices = new vec3[vertex.size()];
    copy(vertex.begin(), vertex.end(), vertices);

    normals = new vec3[normal.size()];
    copy(normal.begin(), normal.end(), normals);

    triangles = new ivec3[face.size()];
    copy(face.begin(), face.end(), triangles);

    num_of_vertices = vertex.size();
    num_of_faces = face.size();

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

void Mesh::update_VFlist(){   
    for(int i = 0; i < num_of_vertices; i++){
        if(!visited_vertices[i]){
            dfs(v2v[i], nullptr, nullptr);
        }
    }
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
}

int hash_func(int i, int j, int n){
    return n*std::min(i,j) + std::max(i,j);
}

void Mesh::update_HElist(){
    unordered_map<int, vector<HalfEdge*>> v2h;
    for(int i = 0; i < num_of_vertices; i++){
        v2v[i] = new Vertex();
        v2v[i]->index = i;
        v2v[i]->position = vertices[i];
        v2v[i]->normal = normals[i];
    }

    for(int i = 0; i < num_of_faces; i++){
        f2f[i] = new Face();
        f2f[i]->index = i;
        
        HalfEdge *he = new HalfEdge();
        f2f[i]->halfedge = he;
        he->left = f2f[i]; 
        v2v[triangles[i].x]->halfedge = he;
        he->head = v2v[triangles[i].x];
        int hxsh = hash_func(triangles[i].z, triangles[i].x, num_of_faces);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he);

        he->next = new HalfEdge();
        he->next->left = f2f[i];
        v2v[triangles[i].y]->halfedge = he->next;
        he->next->head = v2v[triangles[i].y];
        hxsh = hash_func(triangles[i].x, triangles[i].y, num_of_faces);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he->next);

        he->next->next = new HalfEdge();
        he->next->next->left = f2f[i];
        v2v[triangles[i].z]->halfedge = he->next->next;
        he->next->next->head = v2v[triangles[i].z];
        hxsh = hash_func(triangles[i].y, triangles[i].z, num_of_faces);
        if(v2h.find(hxsh) == v2h.end()) v2h[hxsh] = vector<HalfEdge*>();
        v2h[hxsh].push_back(he->next->next);
        
        he->next->next->next = he;
    }
    for(auto vhe: v2h){
        if(vhe.second.size() < 2) continue;
        vhe.second[0]->pair = vhe.second[1];
        vhe.second[1]->pair = vhe.second[0];
    }
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
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        vertices[v->index] = v->position;
        normals[v->index] = v->normal;

        v->traverse(&Mesh::dfs_helper, *this, vtx_opr, fac_opr);
        
        if(vtx_opr){
            vtx_opr(v);
        }
    }
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
    for(int i = 0; i < num_of_vertices; i++){
        if(!visited_vertices[i]){
            dfs(v2v[i], avg_normals, nullptr);
        }
    }
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
    update_VFlist();
}

void Mesh::edge_flip(HalfEdge* halfedge){
    Face* f1 = halfedge->left;
    Face* f2 = halfedge->pair->left;

    Face* f1new = new Face();
    Face* f2new = new Face();
    f1new->halfedge = new HalfEdge();
    f2new->halfedge = new HalfEdge();
    HalfEdge* f11 = halfedge->next->next;
    HalfEdge* f12 = halfedge->pair->next;
    HalfEdge* f21 = halfedge->pair->next->next;
    HalfEdge* f22 = halfedge->next;

    f1new->halfedge->next = f11;
    f1new->halfedge->next->next = f12;
    f1new->halfedge->next->next->next = f1new->halfedge;
    f1new->halfedge->head = f22->head;
    int v1 = f1new->halfedge->head->index;

    f2new->halfedge->next = f21;
    f2new->halfedge->next->next = f22;
    f2new->halfedge->next->next->next = f2new->halfedge;
    f2new->halfedge->head = f12->head;
    int v2 = f2new->halfedge->head->index;

    v2v[v1]->halfedge = f1new->halfedge;
    v2v[v2]->halfedge = f2new->halfedge;

    f1new->index = f1->index;
    f2new->index = f2->index;

    f2f[f1new->index] = f1new;
    f2f[f2new->index] = f2new;

    f1new->halfedge->pair = f2new->halfedge;
    f2new->halfedge->pair = f1new->halfedge;

    triangles[f1new->index] = ivec3(f1new->halfedge->head->index, f1new->halfedge->next->head->index, f1new->halfedge->next->next->head->index);
    triangles[f2new->index] = ivec3(f2new->halfedge->head->index, f2new->halfedge->next->head->index, f2new->halfedge->next->next->head->index);
}