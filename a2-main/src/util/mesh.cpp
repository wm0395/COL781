#include "mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

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
    std::copy(vertex.begin(), vertex.end(), vertices);

    normals = new vec3[normal.size()];
    std::copy(normal.begin(), normal.end(), normals);

    triangles = new ivec3[face.size()];
    std::copy(face.begin(), face.end(), triangles);

    num_of_vertices = vertex.size();
    num_of_faces = face.size();

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

void Mesh::print_VL(){
    std::cout <<"FACES\n";
    for(int i = 0; i < num_of_faces; i++){
        std::cout << "\t [" << triangles[i].x << ", " << triangles[i].y << ", " << triangles[i].z << "]\n";
    }
    std::cout <<"VERTICES\n";
    for(int i = 0; i < num_of_faces; i++){
        std::cout << "\t\t [" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << "]\n";
    }
}

void Mesh::update_VFlist(){   
    for(int i = 0; i < num_of_vertices; i++){
        if(!visited_vertices[i]){
            dfs(v2v[i], nullptr, nullptr, true, true);
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

void Mesh::dfs_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update){
    if (!visited_faces[face->index]){
        if(fac_opr){
            fac_opr(face);
        }
        visited_faces[face->index] = true;

        std::vector<Vertex*> vert = face->face_vertices();
        ivec3 v = face->get_face_vertices_indices();
        triangles[face->index] = v;
        dfs(vert[0], vtx_opr, fac_opr, VL_update, HE_update);
        dfs(vert[1], vtx_opr, fac_opr, VL_update, HE_update);
        dfs(vert[2], vtx_opr, fac_opr, VL_update, HE_update);
    }
}

void Mesh::dfs(Vertex *v, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update){
    if (!visited_vertices[v->index]){
        visited_vertices[v->index] = true;
        visited_vert_count++;
        
        v->traverse(&Mesh::dfs_helper, *this, vtx_opr, fac_opr, VL_update, HE_update);
        
        if(vtx_opr){
            vtx_opr(v);
        }
        
        vec3 tempPos = vertices[v->index];
        vec3 tempNor = normals[v->index];
        if(VL_update){
            vertices[v->index] = v->position;
            normals[v->index] = v->normal;
        }
        if(!HE_update){
            v->position = tempPos;
            v->normal = tempNor;
        }
    }
    if (visited_vert_count == num_of_vertices) return;
}

void add_face_normal(Face *face, Vertex *vertex){
    vertex->normal += face->calculate_normal();
}

void avg_normals(Vertex *vertex){
    vertex->normal = vec3(0,0,0);
    int n = vertex->traverse(add_face_normal, vertex);
    vertex->normal = vec3(vertex->normal.x/n, vertex->normal.y/n, vertex->normal.z/n);
}

void Mesh::recompute_normals(){
    for(int i = 0; i < num_of_vertices; i++){
        if(!visited_vertices[i]){
            dfs(v2v[i], avg_normals, nullptr, true, true);
        }
    }
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
}

void add_face_normal_weighted(Face *face, Vertex *vertex){
    vec3 temp = face->calculate_normal();
    float area = face->calculate_area();
    temp *= area;
    vertex->normal += temp;
    vertex->normal.t += area;
}

void wht_normals(Vertex *vertex){
    vertex->normal = vec3(0,0,0);
    int n = vertex->traverse(add_face_normal_weighted, vertex);
    vertex->normal /= vertex->normal.t;
    vertex->normal.t = 0;
}

void Mesh::recompute_normals_weighted(){
    for(int i = 0; i < num_of_vertices; i++){
        if(!visited_vertices[i]){
            dfs(v2v[i], wht_normals, nullptr, true, true);
        }
    }
    visited_vert_count = 0;
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    visited_faces = std::vector<bool>(num_of_faces, false);
}

void add_nbv(Face *face, Vertex *vertex){
    HalfEdge *he = face->halfedge;
    while(he->head != vertex){
        he = he->next;
    }
    vertex->position += he->next->next->head->position;
    vertex->normal += he->next->next->head->normal;
    if(!he->next->pair){
        vertex->position += he->next->head->position;
    vertex->normal += he->next->head->normal;
    }
}

void umbrella(Vertex* vertex){
    vec3 tempP = vertex->position; 
    vec3 tempN = vertex->normal;
    vertex->position = vec3(0,0,0);
    vertex->normal = vec3(0,0,0);
    int n = vertex->traverse(add_nbv, vertex);
    tempP *= n;
    tempN *= n;
    vertex->position -= tempP;
    vertex->position /= n;
    vertex->normal -= tempN;
    vertex->normal /= n;
}

HalfEdge* give_halfedge(Vertex* v1, Vertex* v2){
    HalfEdge* halfedge = v1->halfedge;
    HalfEdge* he = v1->halfedge;
    bool boundary = false;
    do{
        if (he->next->next->head == v2) return he;
        if(he->next->pair){
            he = he->next->pair;
        }
        else{
            boundary = true;
            break;
        }

    }while(he != halfedge);

    if (!halfedge->pair) return nullptr;
    he = halfedge->pair->next->next;
    // reverse traversal for opposite of boundary
    while(he && boundary){
        if (he->next->next->head == v2) return he;
        if(!he->pair) return nullptr;
        he = he->pair->next->next;
    }
    return nullptr;
}

void Mesh::naive_smoothing(int iter, float lambda){
    for(int i = 0; i < iter; i++){
        for (int i = 0; i<num_of_vertices; i++){
            if (!visited_vertices[i]){
                dfs(v2v[i], umbrella, nullptr, true, false);
            }
        }
        for(int v = 0; v < num_of_vertices; v++){
            vertices[v] *= lambda;
            v2v[v]->position += vertices[v];
            vertices[v] = v2v[v]->position;
        }
        visited_vert_count = 0;
        visited_vertices = std::vector<bool>(num_of_vertices, false);
        visited_faces = std::vector<bool>(num_of_faces, false);
    }
}

void Mesh::taubin_smoothing(int iter, float lambda, float mu){
    for(int i = 0; i < iter; i++){
        for (int i = 0; i<num_of_vertices; i++){
            if (!visited_vertices[i]){
                dfs(v2v[i], umbrella, nullptr, true, false);
            }
        }
        for(int v = 0; v < num_of_vertices; v++){
            vertices[v] *= lambda;
            v2v[v]->position += vertices[v];
            vertices[v] = v2v[v]->position;
        }
        for (int i = 0; i<num_of_vertices; i++){
            if (!visited_vertices[i]){
                dfs(v2v[i], umbrella, nullptr, true, false);
            }
        }
        for(int v = 0; v < num_of_vertices; v++){
            vertices[v] *= mu;
            v2v[v]->position += vertices[v];
            vertices[v] = v2v[v]->position;
        }
        visited_vert_count = 0;
        visited_vertices = std::vector<bool>(num_of_vertices, false);
        visited_faces = std::vector<bool>(num_of_faces, false);
    }
}

bool isPointInsideTriangle(glm::vec3& point, glm::vec3& A, glm::vec3& B, glm::vec3& C) {
    glm::vec3 v0 = C - A;
    glm::vec3 v1 = B - A;
    glm::vec3 v2 = point - A;

    float dot00 = glm::dot(v0, v0);
    float dot01 = glm::dot(v0, v1);
    float dot02 = glm::dot(v0, v2);
    float dot11 = glm::dot(v1, v1);
    float dot12 = glm::dot(v1, v2);

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}


void Mesh::edge_flip_helper(HalfEdge* halfedge){
    if (!halfedge->pair) return;

    Vertex* one = halfedge->head;
    Vertex* two = halfedge->next->head;
    Vertex* three = halfedge->pair->next->head;
    Vertex* four = halfedge->pair->head;

    vec3 pos = two->position + three->position;
    pos/=2;
    if (one->position == pos || four->position == pos) return;
    if (isPointInsideTriangle(four->position, one->position, two->position, three->position)) return;
    if (isPointInsideTriangle(one->position, two->position, four->position, three->position)) return;

    Face* f1 = halfedge->left;
    Face* f2 = halfedge->pair->left;

    Face* f1new = new Face();
    Face* f2new = new Face();
    f1new->halfedge = new HalfEdge();
    f2new->halfedge = new HalfEdge();
    HalfEdge* f11 = halfedge->next->next;
    f11->head->halfedge = f11;
    HalfEdge* f12 = halfedge->pair->next;
    f12->head->halfedge = f12;
    HalfEdge* f21 = halfedge->pair->next->next;
    f21->head->halfedge = f21;
    HalfEdge* f22 = halfedge->next;
    f22->head->halfedge = f22;

    f1new->halfedge->next = f11;
    f1new->halfedge->next->next = f12;
    f1new->halfedge->next->next->next = f1new->halfedge;
    f1new->halfedge->head = f22->head;
    int v1 = f1new->halfedge->head->index;
    f1new->halfedge->left = f1new;
    f1new->halfedge->next->left = f1new;
    f1new->halfedge->next->next->left = f1new;

    f2new->halfedge->next = f21;
    f2new->halfedge->next->next = f22;
    f2new->halfedge->next->next->next = f2new->halfedge;
    f2new->halfedge->head = f12->head;
    int v2 = f2new->halfedge->head->index;
    f2new->halfedge->left = f2new;
    f2new->halfedge->next->left = f2new;
    f2new->halfedge->next->next->left = f2new;

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

void Mesh::edge_flip_helper(HalfEdge* halfedge, bool &flipping){
    if (!halfedge->pair){
        flipping = false;
        return;
    } 

    Vertex* one = halfedge->head;
    Vertex* two = halfedge->next->head;
    Vertex* three = halfedge->pair->next->head;
    Vertex* four = halfedge->pair->head;

    vec3 pos = two->position + three->position;
    pos/=2;
    if (one->position == pos || four->position == pos){
        flipping = false;
        return;
    }
    if (isPointInsideTriangle(four->position, one->position, two->position, three->position)){
        flipping = false;
        return;
    }
    if (isPointInsideTriangle(one->position, two->position, four->position, three->position)){
        flipping = false;
        return;
    }

    Face* f1 = halfedge->left;
    Face* f2 = halfedge->pair->left;

    Face* f1new = new Face();
    Face* f2new = new Face();
    f1new->halfedge = new HalfEdge();
    f2new->halfedge = new HalfEdge();
    HalfEdge* f11 = halfedge->next->next;
    f11->head->halfedge = f11;
    HalfEdge* f12 = halfedge->pair->next;
    f12->head->halfedge = f12;
    HalfEdge* f21 = halfedge->pair->next->next;
    f21->head->halfedge = f21;
    HalfEdge* f22 = halfedge->next;
    f22->head->halfedge = f22;

    f1new->halfedge->next = f11;
    f1new->halfedge->next->next = f12;
    f1new->halfedge->next->next->next = f1new->halfedge;
    f1new->halfedge->head = f22->head;
    int v1 = f1new->halfedge->head->index;
    f1new->halfedge->left = f1new;
    f1new->halfedge->next->left = f1new;
    f1new->halfedge->next->next->left = f1new;

    f2new->halfedge->next = f21;
    f2new->halfedge->next->next = f22;
    f2new->halfedge->next->next->next = f2new->halfedge;
    f2new->halfedge->head = f12->head;
    int v2 = f2new->halfedge->head->index;
    f2new->halfedge->left = f2new;
    f2new->halfedge->next->left = f2new;
    f2new->halfedge->next->next->left = f2new;

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


void Mesh::edge_split_helper(HalfEdge* halfedge){
    bool boundary = true;
    if (halfedge->pair) boundary = false;

    Vertex* v1 = halfedge->head;
    Vertex* v2 = halfedge->next->next->head;
    Vertex* v3 = halfedge->next->head;
    Vertex* v4 = nullptr;
    if (!boundary) v4 = halfedge->pair->next->head;

    Vertex* v = new Vertex;
    v->position = ((v1->position) + (v2->position));
    v->position /= 2;
    v->normal = (v1 -> normal) + (v2->normal);
    v->normal /= 2;
    v->index = num_of_vertices;
    num_of_vertices++;

    HalfEdge* h1 = new HalfEdge;
    HalfEdge* h2 = new HalfEdge;
    HalfEdge* h3 = new HalfEdge;
    HalfEdge* h4 = new HalfEdge;
    HalfEdge* h5 = new HalfEdge;
    HalfEdge* h6 = new HalfEdge;
    HalfEdge* h7 = new HalfEdge;
    HalfEdge* h8 = new HalfEdge;

    h1->head = v;
    if(!boundary) h1->pair = h2;
    h3->head = v1;
    if (!boundary) h3->pair = h4;
    h5->head = v;
    h5->pair = h6;
    if (!boundary) {
        h8->head = v;
        h8->pair = h7;
        h8->next = h2;
        h4->next = h7;
    }
    h1->next = h6;
    h5->next = h3;  

    v->halfedge = h1;

    Face* f1 = new Face;
    Face* f2 = new Face;
    Face* f3 = new Face;
    Face* f4 = new Face;

    f1->halfedge = h1;
    if (!boundary){
        f2->halfedge = h2;
        f3->halfedge = h4;
    }
    f4->halfedge = h3;

    h1->left = f1;
    if (!boundary) h2->left = f2;
    h3->left = f4;
    if (!boundary) h4->left = f3;
    h5->left = f4;
    h6->left = f1;
    if (!boundary) h7->left = f3;
    if (!boundary) h8->left = f2;

    vec3* vert = new vec3[num_of_vertices];
    copy(vertices, vertices + num_of_vertices - 1, vert);
    vert[num_of_vertices - 1] = v->position;
    vertices = vert;

    vec3* norm = new vec3[num_of_vertices];
    copy(normals, normals + num_of_vertices - 1, norm);
    norm[num_of_vertices - 1] = v->normal;
    normals = norm;

    Vertex *v2vnew[num_of_vertices];
    copy(v2v, v2v + num_of_vertices - 1, v2vnew);
    v2vnew[num_of_vertices - 1] = v;
    v2v = v2vnew;

    if (!boundary){
        ivec3* faces = new ivec3[num_of_faces + 2];
        Face *f2fnew[num_of_faces + 2];
        int face_cnt = 0;
        int added = 0;
        for (int i = 0; i<num_of_faces; i++){
            if (!(check_same_face(triangles[i], ivec3(v1->index, v3->index, v2->index)) || check_same_face(triangles[i], ivec3(v1->index, v2->index, v4->index)))){
                faces[face_cnt] = triangles[i];
                face_cnt++;
                f2fnew[i] = f2f[i];
            }
            else{
                if (added == 0){
                    faces[face_cnt] = ivec3(v->index, v3->index, v2->index);
                    f1->index = face_cnt;
                    face_cnt++;
                    added = 1;
                    f2fnew[f1->index] = f1;
                }
                else if (added == 1){                   
                    faces[face_cnt] = ivec3(v->index, v2->index, v4->index);
                    f2->index = face_cnt;
                    face_cnt++;
                    f2fnew[f2->index] = f2;
                }
            }
        }

        faces[face_cnt] = ivec3(v->index, v4->index, v1->index);
        f3->index = face_cnt;
        f2fnew[f3->index] = f3;
        face_cnt++;

        faces[face_cnt] = ivec3(v->index, v1->index, v3->index);
        f4->index = face_cnt;
        f2fnew[f4->index] = f4;
        face_cnt++;

        num_of_faces += 2;
        triangles = faces;
        f2f = f2fnew;
    }

    else{
        ivec3* faces = new ivec3[num_of_faces + 1];
        Face *f2fnew[num_of_faces+1];

        int face_cnt = 0;
        for (int i = 0; i<num_of_faces; i++){
            if (!(check_same_face(triangles[i], ivec3(v1->index, v3->index, v2->index)))){
                faces[face_cnt] = triangles[i];
                face_cnt++;
                f2fnew[i] = f2f[i];
            }
            else{
                faces[face_cnt] = ivec3(v->index, v3->index, v2->index);
                f1->index = face_cnt;
                f2fnew[f1->index] = f1;
                face_cnt++;
            }
        }

        faces[face_cnt] = ivec3(v->index, v1->index, v3->index);
        f4->index = face_cnt;
        f2fnew[f4->index] = f4;
        face_cnt++;

        num_of_faces += 1;
        triangles = faces;
        f2f = f2fnew;
    }
}

void rehead(Face* face, Vertex *vertex){
    HalfEdge* he = face->halfedge;
    while(he->head != vertex){
        he = he->next;
    }
    face->halfedge = he;
}

void reconnect_to_vtx(Face* face, Vertex* vertex, Mesh* mesh){
    int old = face->halfedge->head->index;
    face->halfedge->head = vertex;
    if(mesh->triangles[face->index].x == old){
        mesh->triangles[face->index].x = vertex->index;
    }
    if(mesh->triangles[face->index].y == old){
        mesh->triangles[face->index].y = vertex->index;
    }
    if(mesh->triangles[face->index].z == old){
        mesh->triangles[face->index].z = vertex->index;
    }
}

void reindex(Face* face, Vertex* vertex, Mesh* mesh){
    if(mesh->triangles[face->index].x == face->halfedge->head->index){
        mesh->triangles[face->index].x = vertex->index;
    }
    else if(mesh->triangles[face->index].y == face->halfedge->head->index){
        mesh->triangles[face->index].y = vertex->index;
    }
    else if(mesh->triangles[face->index].z == face->halfedge->head->index){
        mesh->triangles[face->index].z = vertex->index;
    }
}

void reconnect_vertex(Vertex* a, Vertex* b, Mesh *mesh){
    a->traverse(rehead, a);
    a->traverse(reconnect_to_vtx, b, mesh);
}

void Mesh::edge_collapse_helper(HalfEdge* halfedge){
    Face *f1 = halfedge->left;
    Vertex *v1 = halfedge->head, *v2 = halfedge->next->next->head;

    vec3 position = v1->position + v2->position;
    position /=2;
    vec3 normal = v1->normal + v2->normal;
    normal /=2;

    v1->position = position;
    v1->normal = normal;
    reconnect_vertex(v2, v1, this);
    delete_vertex(v2->index);
    update_VFlist();
}

void Mesh::delete_face(int index){
    triangles[index] = triangles[num_of_faces-1];
    f2f[num_of_faces-1]->index = index;
    f2f[index] = f2f[num_of_faces-1];

    num_of_faces--;

    ivec3 temp[num_of_faces];
    copy(triangles, triangles + num_of_faces, temp);
    triangles = temp;

    Face *temp2[num_of_faces];
    copy(f2f, f2f + num_of_faces, temp2);
}

void Mesh::delete_vertex(int index){
    v2v[num_of_vertices-1]->traverse(rehead, v2v[num_of_vertices-1]);
    v2v[num_of_vertices-1]->traverse(reindex, v2v[index], this);
    vertices[index] = vertices[num_of_vertices-1];
    normals[index] = normals[num_of_vertices-1];
    v2v[num_of_vertices-1]->index = index;
    v2v[index] = v2v[num_of_vertices-1];

    num_of_vertices--;
}


void Mesh::flip_edge(int i1, int i2){
    Vertex* v1 = v2v[i1];
    Vertex* v2 = v2v[i2];
    HalfEdge* he = give_halfedge(v1, v2);
    if (!he) he = give_halfedge(v2, v1);
    if (!he) return;
    edge_flip_helper(he);
}

void Mesh::split_edge(int i1, int i2){
    Vertex* v1 = v2v[i1];
    Vertex* v2 = v2v[i2];
    HalfEdge* he = give_halfedge(v1, v2);
    if (!he) he = give_halfedge(v2, v1);
    if (!he) return;
    edge_split_helper(he);
}

void Mesh::collapse_edge(int i1, int i2){
    Vertex* v1 = v2v[i1];
    Vertex* v2 = v2v[i2];
    HalfEdge* he = give_halfedge(v1, v2);
    if (!he) he = give_halfedge(v2, v1);
    if (!he) return;
    edge_collapse_helper(he);
}

bool valid_face(Face* face){
    HalfEdge* he = face->halfedge;
    if (he->left != face){cout << "he->left != face\n"; return false;}
    if (he->pair){
        if (he->pair->pair != he) { cout << "he->pair->pair != he"; return false;}
    }
    else{
        if (he->next->next->head != he->pair->head) {
            cout << "he->next->next->head != he->pair->head\n"; 
            return false;
        }
        if (he != he->next->next->next) {cout << "he != he->next->next->next\n"; return false;}
    }
    return true;
}

bool Mesh::valid_connectivity(){
    for (int i = 0; i<num_of_vertices; i++){
        if (!visited_vertices[i]){
            visited_vertices[i] = true;
            Vertex* v = v2v[i];
            HalfEdge* he = v->halfedge;
            if (v != he->head){ cout << "v != he->head\n"; return false;}
            Face* face = he->left;
            if (!v->traverse(valid_face)) return false;
        }
    }
    visited_vertices = std::vector<bool>(num_of_vertices, false);
    return true;
}


void divide_mesh(Mesh* mesh, unordered_map<int, Vertex*> &new_vtx, unordered_map<int, pair<int,int>> &old_vtx_pair){
    set<int> covered_edge;
    int n = mesh->num_of_vertices;
    for(int i = 0; i < mesh->num_of_faces; i++){
        int j = hash_func(mesh->triangles[i].x, mesh->triangles[i].y, mesh->num_of_vertices);
        HalfEdge* he = mesh->f2f[i]->halfedge;
        while(he->head->index != mesh->triangles[i].y) he = he->next;
        if(covered_edge.find(j) == covered_edge.end()){
            new_vtx[j] = new Vertex();
            new_vtx[j]->position = mesh->vertices[mesh->triangles[i].x] + mesh->vertices[mesh->triangles[i].y];
            new_vtx[j]->normal = mesh->normals[mesh->triangles[i].x] + mesh->normals[mesh->triangles[i].y];
            new_vtx[j]->position /=2.0f;
            new_vtx[j]->normal /=2.0f;
            if(he->pair){
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].z] + mesh->vertices[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].z] + mesh->normals[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->normal += temp;
            }
            else{
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].z];
                temp *= 1.0f/4;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].z];
                temp *= 1.0f/4;
                new_vtx[j]->normal += temp;
            }
            new_vtx[j]->index = n++;
            old_vtx_pair[n-1] = {std::min(mesh->triangles[i].x, mesh->triangles[i].y), std::max(mesh->triangles[i].x, mesh->triangles[i].y)};
            covered_edge.insert(j);
        }
        j = hash_func(mesh->triangles[i].y, mesh->triangles[i].z, mesh->num_of_vertices);
        while(he->head->index != mesh->triangles[i].z) he = he->next;
        if(covered_edge.find(j) == covered_edge.end()){
            new_vtx[j] = new Vertex();
            new_vtx[j]->position = mesh->vertices[mesh->triangles[i].y] + mesh->vertices[mesh->triangles[i].z];
            new_vtx[j]->normal = mesh->normals[mesh->triangles[i].y] + mesh->normals[mesh->triangles[i].z];
            new_vtx[j]->position /=2.0f;
            new_vtx[j]->normal /=2.0f;
            if(he->pair){
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].x] + mesh->vertices[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].x] + mesh->normals[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->normal += temp;
            }
            else{
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].x];
                temp *= 1.0f/4;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].x];
                temp *= 1.0f/4;
                new_vtx[j]->normal += temp;
            }
            new_vtx[j]->index = n++;
            old_vtx_pair[n-1] = {std::min(mesh->triangles[i].z, mesh->triangles[i].y), std::max(mesh->triangles[i].z, mesh->triangles[i].y)};
            covered_edge.insert(j);
        }
        j = hash_func(mesh->triangles[i].x, mesh->triangles[i].z, mesh->num_of_vertices);
        while(he->head->index != mesh->triangles[i].x) he = he->next;
        if(covered_edge.find(j) == covered_edge.end()){
            new_vtx[j] = new Vertex();
            new_vtx[j]->position = mesh->vertices[mesh->triangles[i].x] + mesh->vertices[mesh->triangles[i].z];
            new_vtx[j]->normal = mesh->normals[mesh->triangles[i].x] + mesh->normals[mesh->triangles[i].z];
            new_vtx[j]->position /=2.0f;
            new_vtx[j]->normal /=2.0f;
            if(he->pair){
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].y] + mesh->vertices[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].y] + mesh->normals[he->pair->next->head->index];
                temp *= 1.0f/8;
                new_vtx[j]->normal += temp;
            }
            else{
                new_vtx[j]->position *= 3.0f/4;
                new_vtx[j]->normal *= 3.0f/4;
                vec3 temp = mesh->vertices[mesh->triangles[i].y];
                temp *= 1.0f/4;
                new_vtx[j]->position += temp;
                temp = mesh->normals[mesh->triangles[i].y];
                temp *= 1.0f/4;
                new_vtx[j]->normal += temp;
            }
            new_vtx[j]->index = n++;
            old_vtx_pair[n-1] = {std::min(mesh->triangles[i].z, mesh->triangles[i].x), std::max(mesh->triangles[i].z, mesh->triangles[i].x)};
            covered_edge.insert(j);
        }
    } 
}

void connect_mesh(Mesh* mesh, unordered_map<int, Vertex*>& new_vtx, unordered_map<int, pair<int,int>>& old_vtx_pair, vector<Vertex*> &updated_vtx, vector<ivec3> &updated_fac){
    for(int i = 0; i < mesh->num_of_vertices; i++){

        vec3 vpos = mesh->v2v[i]->position, vnor = mesh->v2v[i]->normal;
        mesh->v2v[i]->position = vec3(0.0f,0.0f,0.0f);
        mesh->v2v[i]->normal = vec3(0.0f,0.0f,0.0f);
        int n = mesh->v2v[i]->traverse(add_nbv, mesh->v2v[i]);

        float u = 3.0f/16.0f;
        if(n > 3) u = 3.0f/(8.0f*n);
        mesh->v2v[i]->position *= u;
        mesh->v2v[i]->normal *= u;
        vpos *= (1.0f-n*u);
        vnor *= (1.0f-n*u);
        mesh->v2v[i]->position += vpos;
        mesh->v2v[i]->normal += vnor;

        updated_vtx[i] = new Vertex();
        updated_vtx[i]->index = mesh->v2v[i]->index;
        updated_vtx[i]->position = mesh->v2v[i]->position;
        updated_vtx[i]->normal = mesh->v2v[i]->normal;

        mesh->v2v[i]->position = mesh->vertices[i];
        mesh->v2v[i]->normal = mesh->normals[i];
    }
    for(auto &pair: new_vtx){
        updated_vtx[pair.second->index] = pair.second;
    }
    
    for(int i = 0; i < mesh->num_of_faces; i++){
        int v1 = mesh->triangles[i].x, v2 = mesh->triangles[i].y, v3 = mesh->triangles[i].z;
        int v4 = new_vtx[hash_func(v1,v2,mesh->num_of_vertices)]->index;
        int v5 = new_vtx[hash_func(v2,v3,mesh->num_of_vertices)]->index;
        int v6 = new_vtx[hash_func(v1,v3,mesh->num_of_vertices)]->index;
        updated_fac[4*i] = ivec3(v1,v4,v6);
        updated_fac[4*i + 1] = ivec3(v4,v2,v5);
        updated_fac[4*i + 2] = ivec3(v4,v5,v6);
        updated_fac[4*i + 3] = ivec3(v5,v3,v6);
    }

}

void Mesh::loop_subdivide(){
    unordered_map<int, Vertex*> new_vtx;
    unordered_map<int, pair<int,int>> old_vtx_pair;

    divide_mesh(this, new_vtx, old_vtx_pair);
    int num_of_edges = new_vtx.size();

    vector<Vertex*> updated_vtx(num_of_vertices + num_of_edges, nullptr);
    vector<ivec3> updated_fac(4*num_of_faces, ivec3(0,0,0));

    connect_mesh(this, new_vtx, old_vtx_pair, updated_vtx, updated_fac);
    vertices = new vec3[num_of_vertices + num_of_edges];
    normals = new vec3[num_of_vertices + num_of_edges];
    triangles = new ivec3[4*num_of_faces];
    v2v = new Vertex*[num_of_vertices+num_of_edges];
    f2f = new Face*[4*num_of_faces];

    for(int i = 0; i < num_of_vertices + num_of_edges; i++){
        vertices[i] = updated_vtx[i]->position;
        normals[i] = updated_vtx[i]->normal;
    }
    for(int i = 0; i < 4*num_of_faces; i++){
        triangles[i] = updated_fac[i];
    }
    int old_V = num_of_vertices, old_N = num_of_faces;
    num_of_vertices += num_of_edges;
    num_of_faces *= 4;

    update_HElist();
}