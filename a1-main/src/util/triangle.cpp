#include "helper.hpp"
#include "geometric.hpp"

namespace Geometric{
    triangle::triangle(){
        a = glm::vec2(0.0f);
        b = glm::vec2(0.0f);
        c = glm::vec2(0.0f);
        A = line(a, b);
        B = line(b, c);
        C = line(c, a);
    }
    triangle::triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c){
        this->a = a;
        this->b = b;
        this->c = c;
        make_anti_clockwise();
        A = line(this->a, this->b);
        B = line(this->b, this->c);
        C = line(this->c, this->a);
    }
    
    bool triangle::isInside(glm::vec2 p){
        return A.isLeft(p) && B.isLeft(p) && C.isLeft(p);
    }

    void triangle::make_anti_clockwise(){
        line A = line(this->a, this->b);
        if(!A.isLeft(this->c)){
            helper::swap(this->a, this->b);
        }
    }
}