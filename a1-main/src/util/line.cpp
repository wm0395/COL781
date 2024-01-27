#include <iostream>
#include "geometric.hpp"

namespace Geometric{
    line::line(){
        a = glm::vec2(0.0f);
        b = glm::vec2(0.0f);
    }
    line::line(glm::vec2 a, glm::vec2 b){
        this->a = a;
        this->b = b;
    }

    bool line::isLeft(glm::vec2 &p){
        glm::vec2 n = glm::vec2(a[1] - b[1], b[0] - a[0]);
        glm::vec2 d = p - a;
        return (glm::dot(n, d) >= 0);
    }

    float line::intercept_x(float y){
        if(a[1] == b[1]){
            return std::min(a[0],b[0])-1;
        }
        else{
            return a[0] + (y - a[1])*(b[0] - a[0])/(b[1] - a[1]);
        }
    }

    float line::intercept_y(float x){
        if(a[0] == b[0]){
            return std::min(a[1], b[1])-1;
        }
        else{
            return a[1] + (x - a[0])*(b[1] - a[1])/(b[1] - a[1]);
        }
    }
}