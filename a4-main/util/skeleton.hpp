#include <string>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

class Mesh {
public:
    Mesh(vec3* vertices, vec3* normals, ivec3* triangles, int nv, int nt);
    vec3* vertices;
    vec3* normals;
    ivec3* triangles;
    int nv, nt;
};

class Bone {
public:
    Bone(string name, Bone* parent, vec3 offset, vec3 axis);
    
    string name;
    Bone* parent;
    vec3 offset;
    vec3 axis;
    Mesh* bone_mesh;

    void print();
    void create_mesh();
    void create_mesh(vec3* vertices, vec3* normals, ivec3* triangles, int nv, int nt);
};