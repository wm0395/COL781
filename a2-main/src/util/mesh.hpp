#include "halfedge.hpp"
#include <set>

using namespace glm;

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