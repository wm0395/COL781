#ifndef CONNECTIVITY_HPP
#define CONNECTIVITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

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
};

void get_vflist(HalfEdge* head, std::vector<vec3>& vertex, std::vector<vec3>& normals, std::vector<ivec3>& face);

void parse_OBJ();

#endif