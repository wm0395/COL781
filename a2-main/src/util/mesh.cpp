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
    // update_VFlist();
}

void add_nbv(Face *face, Vertex *vertex){
    HalfEdge *he = face->halfedge;
    while(he->head != vertex){
        he = he->next;
    }
    vertex->position += he->next->next->head->position;
}

void umbrella(Vertex* vertex){
    vec3 temp = vertex->position; 
    vertex->position = vec3(0,0,0);
    int n = vertex->traverse(add_nbv, vertex);
    temp *= n;
    vertex->position -= temp;
    vertex->position /= n;
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


void Mesh::taubin_smoothing(int iter, float lambda, float mu){
    for(int i = 0; i < iter; i++){
        dfs(v2v[0], umbrella, nullptr, true, false);
        for(int v = 0; v < num_of_vertices; v++){
            vertices[v] *= lambda;
            v2v[v]->position += vertices[v];
            vertices[v] = v2v[v]->position;
        }
        dfs(v2v[0], umbrella, nullptr, true, false);
        for(int v = 0; v < num_of_vertices; v++){
            vertices[v] *= mu;
            v2v[v]->position += vertices[v];
            vertices[v] = v2v[v]->position;
        }
    }
}

void Mesh::edge_flip_helper(HalfEdge* halfedge){
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


    if (!boundary){
        ivec3* faces = new ivec3[num_of_faces + 2];
        int face_cnt = 0;
        for (int i = 0; i<num_of_faces; i++){
            if (!(check_same_face(triangles[i], ivec3(v1->index, v3->index, v2->index)) || check_same_face(triangles[i], ivec3(v1->index, v2->index, v4->index)))){
                faces[face_cnt] = triangles[i];
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

        num_of_faces += 2;
        triangles = faces;
    }

    else{
        ivec3* faces = new ivec3[num_of_faces + 1];
        int face_cnt = 0;
        for (int i = 0; i<num_of_faces; i++){
            if (!(check_same_face(triangles[i], ivec3(v1->index, v3->index, v2->index)))){
                faces[face_cnt] = triangles[i];
                face_cnt++;
            }
        }

        faces[face_cnt] = ivec3(v->index, v3->index, v2->index);
        f1->index = face_cnt;
        face_cnt++;

        faces[face_cnt] = ivec3(v->index, v1->index, v3->index);
        f4->index = face_cnt;
        face_cnt++;

        num_of_faces += 1;
        triangles = faces;
    }
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

void rehead(Face* face, Vertex *vertex){
    HalfEdge* he = face->halfedge;
    while(he->head != vertex){
        he = he->next;
    }
    face->halfedge = he;
}

void reconnect_to_vtx(Face* face, Vertex* vertex){
    face->halfedge->head = vertex;
}

void reconnect_vertex(Vertex* a, Vertex* b){
    a->traverse(rehead, a);
    a->traverse(reconnect_to_vtx, b);
}

void Mesh::collapse_edge(HalfEdge* halfedge){
    Face *f1 = halfedge->left;
    Vertex *v1 = halfedge->head, *v2 = halfedge->next->next->head;

    vec3 position = v1->position + v2->position;
    position /=2;
    vec3 normal = v1->normal + v2->normal;
    normal /=2;

    v1->position = position;
    v1->normal = normal;
    reconnect_vertex(v2, v1);
    delete_vertex(v2->index);

    HalfEdge *neHE = nullptr;
    
    if(halfedge->next->pair && halfedge->next->next->pair){
        HalfEdge *temp = halfedge->next->pair;
        halfedge->next->pair->pair = halfedge->next->next->pair;
        halfedge->next->next->pair->pair = temp;
        neHE = temp;
    }
    else if(halfedge->next->next->pair){
        halfedge->next->next->pair->pair = nullptr;
        neHE = halfedge->next->next->pair;
    }
    else if(halfedge->next->pair){
        halfedge->next->pair->pair = nullptr;
        neHE = halfedge->next->pair;
    }
    delete_face(halfedge->left->index);
    
    if(halfedge->pair){
        if(halfedge->pair->next->pair && halfedge->pair->next->next->pair){
            HalfEdge *temp = halfedge->next->pair;
            halfedge->pair->next->pair->pair = halfedge->pair->next->next->pair;
            halfedge->pair->next->next->pair->pair = temp;
            if(!neHE)
            neHE = halfedge->pair->next->pair;
        }
        else if(halfedge->pair->next->next->pair){
            halfedge->pair->next->next->pair->pair = nullptr;
            if(!neHE)
            neHE = halfedge->pair->next->next->pair;
        }
        else if(halfedge->pair->next->pair){
            halfedge->pair->next->pair->pair = nullptr;
            if(!neHE)
            neHE = halfedge->pair->next->pair;
        }
        delete_face(halfedge->pair->left->index);
    }

    v1->halfedge = neHE;
}

void Mesh::delete_face(int index){
    triangles[index] = triangles[num_of_faces-1];
    f2f[num_of_faces-1]->index = index;
    f2f[index] = f2f[num_of_faces-1];

    num_of_faces--;

    ivec3 temp[num_of_faces];
    copy(triangles, triangles + num_of_faces, temp);
    triangles = temp;
    delete temp;

    Face *temp[num_of_faces];
    copy(f2f, f2f + num_of_faces, temp);
    delete temp;
}

void Mesh::delete_vertex(int index){
    vertices[index] = vertices[num_of_vertices-1];
    normals[index] = normals[num_of_vertices-1];
    v2v[num_of_vertices-1]->index = index;
    v2v[index] = v2v[num_of_vertices-1];

    num_of_vertices--;

    vec3 temp[num_of_vertices];
    copy(vertices, vertices + num_of_vertices, temp);
    vertices = temp;
    copy(normals, normals + num_of_vertices, temp);
    normals = temp;
    delete temp;

    Vertex *temp[num_of_vertices];
    copy(v2v, v2v + num_of_vertices, temp);
    delete temp;
}