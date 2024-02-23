#include "viewer.hpp"
#include "./util/util.hpp"
#include "./util/mesh.cpp"
#include "./util/halfedge.cpp"
#include <vector>
#include <iostream>

namespace V = COL781::Viewer;
using namespace glm;
using namespace std;

int main() {
    int m,n;
    m = 2;
    n = 2;
    // vec3 vertices[(m+1)*(n+1)];
    // vec3 normals[(m+1)*(n+1)];
    // ivec3 triangles[2*m*n];

    vector<vec3> vertices((m+1)*(n+1));
    vector<vec3> normals((m+1)*(n+1));
    vector<ivec3> triangles(2*m*n);

    int cnt = 0;
    int tri_cnt = 0;

    std::swap(m,n);

    for (int i = 0; i<=m; i++){
        for (int j = 0; j<=n; j++){
            vertices[cnt] = vec3(float(i)/float(m)-0.5, float(j)/float(n)-0.5, 0.0);
            normals[cnt] = vec3(0.0, 0.0, 1.0);
            cnt++;

            if (j!=n && i!=m)
            {
                int x1 = (n+1)*i + j;
                int x2 = x1+1;
                int x3 = (n+1)*(i+1) + j;
                int x4 = x3+1;
                triangles[tri_cnt] = ivec3(x1, x2, x3);
                tri_cnt++;
                triangles[tri_cnt] = ivec3(x2, x3, x4);
                tri_cnt++;
            }
        }
    }

    HalfEdge *he = nullptr;
    get_vflist(he, vertices, normals, triangles);
    cout << "he head id " << he->head->index << endl; 
    cout << "he left id " << he->left->index << endl; 

    Mesh *mesh = new Mesh(vertices.size(), he->head);
    mesh->give_initial_mesh();
    mesh->populate_mesh();

    // V::Viewer v;
    // if (!v.initialize("Mesh viewer", 640, 480)) {
    //     return EXIT_FAILURE;
    // }
    // v.setVertices((m+1)*(n+1), mesh->vertices);
    // v.setNormals((m+1)*(n+1), mesh->normals);
    // v.setTriangles(2*m*n, mesh->triangles);
    // v.view();
}
