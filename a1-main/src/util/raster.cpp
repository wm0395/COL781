#include "raster.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

namespace raster{
    float sample_aa(int i, int j, int spp, Geometric::triangle &T){
        // glm::vec4 sum = glm::vec4(0.0f);
        float sum= 0;
        for(int k = 0; k < spp; k++){
            for(int l = 0; l < spp; l++){
                float x = i + (float)k/spp;
                float y = j + (float)l/spp;
                if(T.isInside(glm::vec2(x, y))){
                    sum += 1;
                }
                // else{
                //     sum += pixels[y][x];
                // }
            }
        }
        return (sum/(spp*spp));
    }

    void anti_alias(Geometric::triangle &T, int spp, std::vector<float> &attrib, std::vector<std::vector<std::vector<float>>> &pointBuffer){
        int x_min, x_max;
        int y_min = std::min(T.a[1], std::min(T.b[1], T.c[1]));
        int y_max = std::max(T.a[1], std::max(T.b[1], T.c[1]));

        const int frameWidth = pointBuffer.size();
        const float sample_centre = 0.5;

        for(int j = y_min; j <= y_max; j++){
            int l1 = T.A.intercept_x(j + sample_centre), l2 = T.B.intercept_x(j + sample_centre), l3 = T.C.intercept_x(j + sample_centre);

            if(l1< std::min(T.a[0], T.b[0]) || l1> std::max(T.a[0], T.b[0])){
                x_min = std::min(l2, l3);
                x_max = std::max(l2, l3);
            }
            else if(l2< std::min(T.b[0], T.c[0]) || l2> std::max(T.b[0], T.c[0])){
                x_min = std::min(l1, l3);
                x_max = std::max(l1, l3);
            }
            else{
                x_min = std::min(l1, l2);
                x_max = std::max(l1, l2);
            }
            if(x_min > 0 && T.isInside( glm::vec2(x_min-1, j))){
                x_min--;
            }
            // else if(!T.isInside(make_pair(x_min, j))){
            //     x_min++;
            // }
            if(x_max < frameWidth -1 && T.isInside( glm::vec2(x_max+1, j))){
                x_max++;
            }
            // else if(!T.isInside(make_pair(x_max, j))){
            //     x_max--;
            // }
            for(int i = x_min; i <= x_max; i++){
                if(pointBuffer[i][j][0] > attrib[0]) continue;
                float c = sample_aa(i, j, spp, T);
                for(int x = 1; x < 4; x++)
                pointBuffer[i][j][x] = c*attrib[x];
            }
        }
        
    }
}