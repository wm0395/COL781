#include "skeleton.hpp"

using namespace std;
using namespace glm;

Mesh::Mesh(vec3* vertices, vec3* normals, ivec3* triangles, int nv, int nt) {
    this->vertices = vertices;
    this->normals = normals;
    this->triangles = triangles;
    this->nv = nv;
    this->nt = nt;
}

Bone::Bone(string name, Bone* parent, vec3 offset, vec3 axis) {
    this->name = name;
    this->parent = parent;
    this->offset = offset;
    this->axis = axis;
    this->bone_mesh = nullptr;  //TODO: bone mesh banani hai
    create_mesh();
}

void Bone::print() {
    cout << "Bone: " << name << endl;
    cout << "Parent: " << parent->name << endl;
    cout << "Offset: " << offset.x << " " << offset.y << " " << offset.z << endl;
    cout << "Axis: " << axis.x << " " << axis.y << " " << axis.z << endl;
}