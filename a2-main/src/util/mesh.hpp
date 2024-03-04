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

    void dfs(Vertex *v, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update);
    void give_initial_mesh();
    void recompute_normals();

    void taubin_smoothing(int iter, float lambda, float mu);

    void flip_edge(int i1, int i2);
    void split_edge(int i1, int i2);
    void collapse_edge(HalfEdge *halfedge);

    private:
    std::vector<bool> visited_vertices;
    std::vector<bool> visited_faces;
    int visited_vert_count;

    void add_vertex(vec3 &position, vec3 &normal);
    void delete_vertex(int index);
    void update_vertex(int index, vec3 &position, vec3 &normal);

    void add_face(ivec3 &triangle);
    void delete_face(int index);
    void update_face(int index, ivec3 &triangle);


    void dfs_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update);
    void print_vis_vert();
    void print_vis_faces();

    void edge_flip_helper(HalfEdge *halfedge);
    void edge_split_helper(HalfEdge *halfedge);

    void taubin_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
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
    int traverse(void (*func)(Face *face, Vertex *vertex), Vertex *vertex);
    void traverse(void (Mesh::*func)(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update), Mesh &mesh, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face), bool VL_update, bool HE_update);
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
};

bool check_same_face(ivec3 v1, ivec3 v2);
HalfEdge* give_halfedge(Vertex* v1, Vertex* v2);

#endif