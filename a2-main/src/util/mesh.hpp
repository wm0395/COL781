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
    
    vec3* vertices;
    vec3* normals;
    ivec3* triangles;

    Vertex *starting_vertex;

    void populate_mesh();   
    void dfs(Vertex *v, int &visited_vert_count, int &visited_face_count);

    private:
    int num_of_vertices;
    std::vector<bool> visited_vertices;
    std::set<GLint> covered_faces;
};

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
};

class Face{
    public:
    HalfEdge *halfedge;
    GLint index;

    void traverse(void (*func)(Vertex *vertex));
    ivec3 get_face_vertices();
};

void get_vflist(HalfEdge* head, std::vector<vec3>& vertex, std::vector<vec3>& normals, std::vector<ivec3>& face);

void parse_OBJ(const char *filename, std::vector<vec3> &vertex, std::vector<vec3> &normal, std::vector<ivec3> &face);

#endif