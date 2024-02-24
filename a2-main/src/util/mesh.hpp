#ifndef CONNECTIVITY_HPP
#define CONNECTIVITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <set>

using namespace glm;

class Vertex;
class HalfEdge;
class Face;

class Mesh{
    public:
    Mesh(int V, int N, Vertex *vertex);
    vec3* vertices;
    vec3* normals;
    ivec3* triangles;
    // std::vector<vec3> vertices;
    // std::vector<vec3> normals;
    // std::vector<ivec3> triangles;

    Vertex *starting_vertex;

    void populate_mesh();   
    void dfs(Vertex *v, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
    void give_initial_mesh();
    vec3* give_vertices();
    vec3* give_normals();
    ivec3* give_triangles();


    private:
    int num_of_vertices;
    int num_of_faces;
    std::vector<bool> visited_vertices;
    std::vector<bool> visited_faces;
    int visited_vert_count;

    void dfs_helper(Face *face, void (*vtx_opr)(Vertex *vertex), void (*fac_opr)(Face *face));
    void print_vis_vert();
    void print_vis_faces();
};

void get_vflist(HalfEdge* &head, std::vector<vec3>& vertex, std::vector<vec3>& normals, std::vector<ivec3>& face);


// Things to be implemented:
// efficient link between vertex locations and triangle locations
// How to send this to raster API?

class HalfEdge{
    public:
    HalfEdge *pair, *next;
    Vertex *head;
    Face *left;
};

class Vertex{
    public:
    HalfEdge *halfedge;
    GLint index;
    vec3* position;
    vec3* normal;

    void traverse(void (*func)(Face *face));
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
};

void parse_OBJ(const char *filename, std::vector<vec3> &vertex, std::vector<vec3> &normal, std::vector<ivec3> &face);

#endif