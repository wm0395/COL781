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
        glm::vec2 n = glm::vec2(a.y - b.y, b.x - a.x);
        glm::vec2 d = p - a;
        return (glm::dot(n, d) >= 0);
    }

    bool line::isLeft(glm::vec3 &p){
        glm::vec2 p_ = glm::vec2(p.x, p.y);
        glm::vec2 n = glm::vec2(a.y - b.y, b.x - a.x);
        glm::vec2 d = p_ - a;
        return (glm::dot(n, d) >= 0);
    }

    float line::intercept_x(float y){
        if(a.y == b.y){
            std::cout << "parallel line case\n";
            return std::min(a.x,b.x)-1;
        }
        else{
            return a.x + (y - a.y)*(b.x - a.x)/(b.y - a.y);
        }
    }

    float line::intercept_y(float x){
        if(a.x == b.x){
            return std::min(a.y, b.y)-1;
        }
        else{
            return a.y + (x - a.x)*(b.y - a.y)/(b.y - a.y);
        }
    }

    void line::print(){
        std::cout << "( " << a.x << "," << a.y << ")" << ", (" << b.x << "," << b.y << ")\n"; 
    }
}