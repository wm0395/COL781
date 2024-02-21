#ifndef CONNECTIVITY_HPP
#define CONNECTIVITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

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

    void traverse(void (*func)(Face *face));
};

class Face{
    public:
    HalfEdge *halfedge;
    GLint index;

    void traverse(void (*func)(Vertex *vertex));
};

#endif