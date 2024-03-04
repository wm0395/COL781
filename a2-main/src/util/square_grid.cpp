#include "std_mesh.hpp"
#include "mesh.cpp"
#include "halfedge.cpp"
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

void square_grid(Mesh* mesh, int m, int n){

    int cnt = 0;
    int tri_cnt = 0;

    std::swap(m,n);

    for (int i = 0; i<=m; i++){
        for (int j = 0; j<=n; j++){
            mesh->vertices[cnt] = vec3(float(i)/float(m)-0.5, float(j)/float(n)-0.5, 0.0);
            mesh->normals[cnt] = vec3(0.0, 0.0, 1.0);
            cnt++;

            if (j!=n && i!=m)
            {
                int x1 = (n+1)*i + j;
                int x2 = x1+1;
                int x3 = (n+1)*(i+1) + j;
                int x4 = x3+1;
                mesh->triangles[tri_cnt] = ivec3(x1, x2, x3);
                tri_cnt++;
                mesh->triangles[tri_cnt] = ivec3(x3, x2, x4);
                tri_cnt++;
            }
        }
    }

    mesh->update_HElist();
}