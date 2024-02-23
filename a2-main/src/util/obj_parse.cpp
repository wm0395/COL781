#include "mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void parse_OBJ(const char *filename, std::vector<vec3> &vertex, std::vector<vec3> &normal, std::vector<ivec3> &face){
    std::ifstream in(filename, std::ios::in);

    bool normals = false;
    
    if (!in){
        std::cerr << "Cannot open " << filename << std::endl;
        exit(1);

    }
    std::string line;
    while (std::getline(in, line))
    {
        //check v for vertices
        if (line.substr(0,2)=="v "){
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x,y,z;
            v>>x;v>>y;v>>z;
            vert=glm::vec3(x,y,z);
            vertex.push_back(vert);
        }
        //check for texture co-ordinate
        else if(line.substr(0,2)=="vn"){

            normals = true;
            std::istringstream v(line.substr(3));
            glm::vec3 nor;
            float x,y,z;
            v>>x;v>>y;v>>z;
            nor=glm::vec3(x,y,z);
            normal.push_back(nor);

        }
        //check for faces
        else if(line.substr(0,2)=="f " && normals){
            int a,b,c; //to store mesh index
            int A,B,C; //to store texture index
            //std::istringstream v;
            //v.str(line.substr(2));
            const char* chh=line.c_str();
            sscanf (chh, "f %i//%i %i//%i %i//%i",&a,&A,&b,&B,&c,&C); //here it read the line start with f and store the corresponding values in the variables

            //v>>a;v>>b;v>>c;
            a--;b--;c--;
            A--;B--;C--;
            //std::cout<<a<<b<<c<<A<<B<<C;
            // faceIndex.push_back(a);//textureIndex.push_back(A);
            // faceIndex.push_back(b);//textureIndex.push_back(B);
            // faceIndex.push_back(c);//textureIndex.push_back(C);
            glm::ivec3 fac = ivec3(a,b,c);
            face.push_back(fac);
        }

        else if (line.substr(0,2)=="f " && !normals){
            int a,b,c;
            const char* chh=line.c_str();
            sscanf(chh, "f %i %i %i", &a, &b, &c);
            a--;b--;c--;
            glm::ivec3 fac = ivec3(a,b,c);
            face.push_back(fac);

            for (int i = 0; i<vertex.size(); i++){
                normal.push_back(glm::vec3(0.0, 0.0, 0.0));
            }
        }

    }
    //the mesh data is finally calculated here
    // for(unsigned int i=0;i<faceIndex.size();i++)
    // {
    //     glm::vec3 meshData;
    //     glm::vec2 texData;
    //     meshData=glm::vec3(vertices[faceIndex[i]].x,vertices[faceIndex[i]].y,vertices[faceIndex[i]].z);
    //     texData=glm::vec2(texture[textureIndex[i]].x,texture[textureIndex[i]].y);
    //     meshVertices.push_back(meshData);
    //     texCoord.push_back(texData);
    // }
}