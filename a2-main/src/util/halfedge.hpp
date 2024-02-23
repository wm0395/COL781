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
    vec3 *position;
    vec3 *normal;

    void traverse(void (*func)(Face *face));
};

class Face{
    public:
    HalfEdge *halfedge;
    GLint index;

    void traverse(void (*func)(Vertex *vertex));
    ivec3 get_face_vertices();
};

void get_vf(HalfEdge* head, vector<vec3>& vertex, vector<vec3>& normals, vector<ivec3>& face);

#endif