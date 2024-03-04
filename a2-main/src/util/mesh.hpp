#ifndef CONNECTIVITY_HPP
#define CONNECTIVITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <set>
#include <string>

using namespace glm;
using namespace std;

class Vertex;
class HalfEdge;
class Face;

class Mesh{
    public:
    Mesh(int V, int N);
    Mesh(int V, int N, Vertex **v2v, Face **f2f);
    Mesh(int V, int N, vec3 &vertex, vec3 &normal, ivec3 &face);
    Mesh(string file);
    vec3* vertices;
    vec3* normals;
    ivec3* triangles;
    Vertex** v2v; //vertex-list to vertex pointer
    Face** f2f;//face-list to face pointer

    
    int num_of_vertices;
    int num_of_faces;

    void update_VFlist(); 
    void update_HElist();
    void parse_OBJ(const char *file_name);

    void dfs(Vertex *v, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
    void give_initial_mesh();
    vec3* give_vertices();
    vec3* give_normals();
    ivec3* give_triangles();
    void recompute_normals();

    void edge_flip(HalfEdge *halfedge);
    void split_edge(int i1, int i2);


    private:
    std::vector<bool> visited_vertices;
    std::vector<bool> visited_faces;
    int visited_vert_count;

    void dfs_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
    void print_vis_vert();
    void print_vis_faces();
};


class HalfEdge{
    public:
    HalfEdge *pair, *next;
    Vertex *head;
    Face *left;

    void split_halfedge(Mesh* mesh);
};

class Vertex{
    public:
    HalfEdge *halfedge;
    GLint index;
    vec3 position;
    vec3 normal;

    void traverse(void (*func)(Face *face));
    int traverse(void (*func)(Face *face, Vertex *vertex));
    void traverse(void (Mesh::*func)(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face)), Mesh &mesh, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
};

class Face{
    public:
    HalfEdge *halfedge;
    GLint index;

    void traverse(void (*func)(Vertex *vertex));
    std::vector<Vertex*> face_vertices();
    ivec3 get_face_vertices_indices();
    void print_face_vertices();
    vec3 calculate_normal();
    // bool check_same_face(Face* face);
};

#endif