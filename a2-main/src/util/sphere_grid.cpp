// #include "square_grid.cpp"
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

void sphere_grid(Mesh* mesh, int m, int n){
    float r = 0.5;   //yeh doubtful hai abhi !!

    int vert_cnt = m*(n-1)+2;
    int tri_cnt = 2*m*(n-1);

    int cnt = 1;
    int pres_tri_cnt = m;

    mesh->vertices[0] = vec3(0.0, r, 0.0);
    mesh->normals[0] = vec3(0.0, 1.0, 0.0);
    mesh->vertices[vert_cnt-1] = vec3(0.0, -r, 0.0);
    mesh->normals[vert_cnt-1] = vec3(0.0, -1.0, 0.0);

    for (int i = 0; i<m; i++){
        mesh->triangles[i] = ivec3(0,i+1,((i+1)%m)+1);
    }

    for (int lat = 1; lat<=n-1; lat++){
        for (int log = 0; log<m; log++){
            mesh->vertices[cnt] = vec3(r * std::cos(2*M_PI*log/m) * std::sin(M_PI*lat/n), r * std::cos(M_PI*lat/n), r * std::sin(2*M_PI*log/m) * std::sin(M_PI*lat/n));
            mesh->normals[cnt] = mesh->vertices[cnt]/r;
            cnt++;

            if (lat != n-1){
                if (log != m-1){
                    int x1 = (lat-1)*m + log + 1;
                    int x2 = x1 + 1;
                    int x3 = x1 + m;
                    int x4 = x3 + 1;
                    mesh->triangles[pres_tri_cnt] = ivec3(x1, x3, x4);
                    pres_tri_cnt++;
                    mesh->triangles[pres_tri_cnt] = ivec3(x1, x2, x4);
                    pres_tri_cnt++;
                }
                else if (log == m-1){
                    int x1 = (lat-1)*m + log + 1;
                    int x2 = (lat-1)*m + 1;
                    int x3 = x1 + m;
                    int x4 = x2 + m;
                    mesh->triangles[pres_tri_cnt] = ivec3(x1, x3, x4);
                    pres_tri_cnt++;
                    mesh->triangles[pres_tri_cnt] = ivec3(x1, x2, x4);
                    pres_tri_cnt++;
                }
            }

            if (lat == n-1 && log!=m-1){
                int vert = m*(n-2) + log + 1;
                mesh->triangles[pres_tri_cnt] = ivec3(vert, vert+1, m*(n-1)+1);
                pres_tri_cnt++;
            }
            else if (lat == n-1 && log == m-1){
                mesh->triangles[pres_tri_cnt] = ivec3(m*(n-1), m*(n-2)+1, m*(n-1)+1);
                pres_tri_cnt++;
            }
        }
    }

    // Mesh* mesh = new Mesh(vert_cnt, tri_cnt, vertices, normals, triangles);

    std::cout << mesh->vertices << "\n";

    for (int i = 0; i<mesh->num_of_vertices; i++){
        vec3 pos = mesh->vertices[i];
        std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
    return;
}