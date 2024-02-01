#include "helper.hpp"
#include "geometric.hpp"
#include <iostream>
#include "../sw.hpp"
using namespace COL781;

namespace Geometric{
    // triangle::triangle(){
    //     a = glm::vec3(0.0f);
    //     b = glm::vec3(0.0f);
    //     c = glm::vec3(0.0f);
    //     A = line(a, b);
    //     B = line(b, c);
    //     C = line(c, a);
    // }
    // triangle::triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    //     // std::cout << "makinng triangle\n";
    //     this->a = a;
    //     this->b = b;
    //     this->c = c;
    //     make_anti_clockwise();
    //     A = line(this->a, this->b);
    //     B = line(this->b, this->c);
    //     C = line(this->c, this->a);
    // }

    triangle::triangle(std::vector<Software::Attribs> vertices){
        a = vertices[0];
        b = vertices[1];
        c = vertices[2];

        make_anti_clockwise();

        A = line(a.get<glm::vec4>(0), b.get<glm::vec4>(0));
        B = line(b.get<glm::vec4>(0), c.get<glm::vec4>(0));
        C = line(c.get<glm::vec4>(0), a.get<glm::vec4>(0));
    }
    
    bool triangle::isInside(glm::vec2 p){
        return A.isLeft(p) && B.isLeft(p) && C.isLeft(p);
    }

    bool triangle::isInside(glm::vec3 p){
        return A.isLeft(p) && B.isLeft(p) && C.isLeft(p);
    }

    void triangle::make_anti_clockwise(){
        line A = line(this->a.get<glm::vec4>(0), this->b.get<glm::vec4>(0));
        if(!A.isLeft(this->c.get<glm::vec4>(0))){
            helper::swap(this->a, this->b);
        }
    }

    // void triangle::print(){
    //     std::cout << a.x << " " << a.y << " " << a.z << "\n";
    //     std::cout << b.x << " " << b.y << " " << b.z << "\n";
    //     std::cout << c.x << " " << c.y << " " << c.z << "\n";
    //     A.print();
    //     B.print();
    //     C.print();
    // }
}