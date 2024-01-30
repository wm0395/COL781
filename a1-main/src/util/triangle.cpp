#include "helper.hpp"
#include "geometric.hpp"
#include <iostream>

namespace Geometric{
    triangle::triangle(){
        a = glm::vec3(0.0f);
        b = glm::vec3(0.0f);
        c = glm::vec3(0.0f);
        A = line(a, b);
        B = line(b, c);
        C = line(c, a);
    }
    triangle::triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c){
        // std::cout << "makinng triangle\n";
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

    bool triangle::isInside(glm::vec3 p){
        return A.isLeft(p) && B.isLeft(p) && C.isLeft(p);
    }

    void triangle::make_anti_clockwise(){
        line A = line(this->a, this->b);
        if(!A.isLeft(this->c)){
            helper::swap(this->a, this->b);
        }
    }

    void triangle::print(){
        std::cout << a.x << " " << a.y << " " << a.z << "\n";
        std::cout << b.x << " " << b.y << " " << b.z << "\n";
        std::cout << c.x << " " << c.y << " " << c.z << "\n";
        A.print();
        B.print();
        C.print();
    }
}