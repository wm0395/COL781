#include "raster.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "../sw.hpp"
using namespace COL781;

namespace raster{
    float sample_aa(int i, int j, int spp, Geometric::triangle &T){
        // std::cout << "in sample_aa\n";
        // glm::vec4 sum = glm::vec4(0.0f);
        float sum= 0;
        for(int k = 0; k < spp; k++){
            for(int l = 0; l < spp; l++){
                float x = i + (float)k/spp;
                float y = j + (float)l/spp;
                if(T.isInside(glm::vec2(x, y))){
                    // std::cout << "sume bada\n";
                    sum += 1;
                }
                // else{
                //     sum += pixels[y][x];
                // }
            }
        }
        // std::cout << "sum : " << sum << std::endl;
        return (sum/float(spp*spp));
    }

    // void anti_alias(Geometric::triangle &T, int spp, std::vector<std::vector<Software::Attribs>> &pointBuffer){
    //     // std::cout << "in anti_alias\n";
    //     int x_min, x_max;
    //     int y_min = std::min(T.a.y, std::min(T.b.y, T.c.y));
    //     int y_max = std::max(T.a.y, std::max(T.b.y, T.c.y));

    //     const int frameWidth = pointBuffer.size();
    //     const float sample_centre = 0.5;

    //     // std::cout << frameWidth << std::endl;
    //     // std::cout << y_min << " " << y_max << std::endl;

    //     for(int j = y_min; j <= y_max; j++){
    //         int l1 = T.A.intercept_x(j + sample_centre), l2 = T.B.intercept_x(j + sample_centre), l3 = T.C.intercept_x(j + sample_centre);

    //         // std::cout << l1 << " " << l2 << " " << l3 << std::endl;

    //         if(l1< std::min(T.a.x, T.b.x) || l1> std::max(T.a.x, T.b.x)){
    //             x_min = std::min(l2, l3);
    //             x_max = std::max(l2, l3);
    //         }
    //         else if(l2< std::min(T.b.x, T.c.x) || l2> std::max(T.b.x, T.c.x)){
    //             x_min = std::min(l1, l3);
    //             x_max = std::max(l1, l3);
    //         }
    //         else{
    //             x_min = std::min(l1, l2);
    //             x_max = std::max(l1, l2);
    //         }
    //         // std::cout << "x_min and x_max => " <<  x_min << " " << x_max << std::endl;
    //         if(x_min > 0 && T.isInside( glm::vec2(x_min-1, j))){
    //             x_min--;
    //         }
    //         // else if(!T.isInside(make_pair(x_min, j))){
    //         //     x_min++;
    //         // }
    //         if(x_max < frameWidth -1 && T.isInside( glm::vec2(x_max+1, j))){
    //             x_max++;
    //         }
    //         // else if(!T.isInside(make_pair(x_max, j))){
    //         //     x_max--;
    //         // }
            
    //         for(int i = x_min; i <= x_max; i++){
    //             // std::cout << "idhar ghusa\n";
    //             // if(pointBuffer[i][j][0] > attrib[0]) continue;
    //             // std::cout << "spp: " << spp << "\n";
    //             float c = sample_aa(i, j, spp, T);
    //             // std::cout << "c: " << c << std::endl;
    //             for(int k = 0; k <= 4; k++){
    //                 // std::cout << k << std::endl;
    //                 pointBuffer[i][j][k] = c*attrib[k];
    //             }
    //         }
    //     }
        
    // }
    void anti_alias(Geometric::triangle &T, int spp, std::vector<std::vector<Software::Attribs>> &pointBuffer){
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
                glm::vec3 bary = T.getBarycentric();
                if(T.isInside(p) && ){
                    ;
                }
            }
        }
    }
}