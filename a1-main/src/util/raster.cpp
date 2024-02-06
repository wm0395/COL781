#include "raster.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "../sw.hpp"
using namespace COL781;

namespace raster{
    float sample_aa(int i, int j, int spp, Geometric::triangle &T){
        float sum = 0;
        for(int k = 0; k < spp; k++){
            for(int l = 0; l < spp; l++){
                float x = i + (float)k/spp;
                float y = j + (float)l/spp;
                if(T.isInside(glm::vec2(x, y))){
                    sum += 1;
                }
            }
        }
        return (sum/float(spp*spp));
    }


    void anti_alias(Geometric::triangle &T, int spp, std::vector<std::vector<Software::Attribs>> &pointBuffer){
        // spp = 3;
        glm::vec4 a = T.a.get<glm::vec4>(0);
        glm::vec4 b = T.b.get<glm::vec4>(0);
        glm::vec4 c = T.c.get<glm::vec4>(0);
        
        int x_min = std::min(a.x, std::min(b.x, c.x));
        int x_max = std::max(a.x, std::max(b.x, c.x));
        int y_min = std::min(a.y, std::min(b.y, c.y));
        int y_max = std::max(a.y, std::max(b.y, c.y));

        for(int j = y_min; j <= y_max; j++){
            for(int i = x_min; i <= x_max; i++){
                glm::vec2 p = glm::vec2(i,j);
                glm::vec3 bary = T.getBarycentric(glm::vec2(i, j));
                float z = bary.x*a.z + bary.y*b.z + bary.z*c.z;
                if(z >= 0 && z <= pointBuffer[i][j].get<float>(0) && T.isInside(p)){
                    float alpha = sample_aa(i,j,spp,T);
                    Software::Attribs pointData = T.interpolateAttrib(p);
                    glm::vec4 color = pointData.get<glm::vec4>(1);
                    glm::vec4 screen = pointBuffer[i][j].get<glm::vec4>(1);
                    float pre_alpha = color.w;
                    color.w *= alpha;
                    color.x = (alpha*pre_alpha*color.x + screen.w*(1-alpha)*screen.x)/pre_alpha;
                    color.y = (alpha*pre_alpha*color.y + screen.w*(1-alpha)*screen.y)/pre_alpha;
                    color.z = (alpha*pre_alpha*color.z + screen.w*(1-alpha)*screen.z)/pre_alpha;
                    pointData.set(1, color);
                    pointData.set(0, z);
                    pointBuffer[i][j] = pointData;
                }
            }
        }
    }
}