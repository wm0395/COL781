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

        // std::cout << "in triangle.cpp\n";
        // a.print();

        // std::cout << a.get<glm::vec4>(0).x << std::endl;

        A = line(a.get<glm::vec4>(0), b.get<glm::vec4>(0));
        B = line(b.get<glm::vec4>(0), c.get<glm::vec4>(0));
        C = line(c.get<glm::vec4>(0), a.get<glm::vec4>(0));
    }
    
    bool triangle::isInside(glm::vec2 p){
        // return isInside(findZ(p));
        bool in = A.isLeft(p) && B.isLeft(p) && C.isLeft(p); 
        return in;
    }

    bool triangle::isInside(glm::vec3 p){
        // glm::vec3 bary = getBarycentric(p);
        // bool in = bary.x >= 0 && bary.x <=1 && bary.y >= 0 && bary.y <=1 && bary.z >= 0 && bary.z <=1;
        bool in = A.isLeft(p) && B.isLeft(p) && C.isLeft(p); 
        return in;
    }

    void triangle::make_anti_clockwise(){
        line A = line(this->a.get<glm::vec4>(0), this->b.get<glm::vec4>(0));
        if(!A.isLeft(this->c.get<glm::vec4>(0))){
            helper::swap(this->a, this->b);
        }
    }

    float triangle::calculateTriangleArea(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
        // Calculate two vectors representing two sides of the triangle
        glm::vec3 side1 = v2 - v1;
        glm::vec3 side2 = v3 - v1;

        // Calculate the cross product of the two sides
        glm::vec3 crossProduct = glm::cross(side1, side2);

        // Calculate the magnitude of the cross product
        float area = 0.5f * glm::length(crossProduct);

        return area;
    }

    glm::vec3 triangle::getBarycentric(glm::vec2 point){
        return getBarycentric(findZ(point));
    }

    glm::vec3 triangle::getBarycentric(glm::vec3 point){
        glm::vec3 a = glm::vec3(this->a.get<glm::vec4>(0).x, this->a.get<glm::vec4>(0).y, this->a.get<glm::vec4>(0).z);
        glm::vec3 b = glm::vec3(this->b.get<glm::vec4>(0).x, this->b.get<glm::vec4>(0).y, this->b.get<glm::vec4>(0).z);
        glm::vec3 c = glm::vec3(this->c.get<glm::vec4>(0).x, this->c.get<glm::vec4>(0).y, this->c.get<glm::vec4>(0).z);

        float triangle_area = triangle::calculateTriangleArea(a,b,c);

        float phi1 = triangle::calculateTriangleArea(b,c,point)/triangle_area;
        float phi2 = triangle::calculateTriangleArea(c,a,point)/triangle_area;
        float phi3 = triangle::calculateTriangleArea(a,b,point)/triangle_area;

        return glm::vec3(phi1, phi2, phi3);
    }

    Software::Attribs triangle::interpolateAttrib(glm::vec2 p){
        // std::cout << "vec2 \n";
        // glm::vec3 hehe = findZ(p);
        // std::cout << hehe.x << " " << hehe.y << " " << hehe.z << std::endl;
        return interpolateAttrib(findZ(p));
    }

    Software::Attribs triangle::interpolateAttrib(glm::vec3 p){
        glm::vec3 bary = getBarycentric(p);
        // std::cout << bary.x << " " << bary.y << " " << bary.z << std::endl;
        Software::Attribs pointData = Software::Attribs();
        // float z_interpolate = bary.x/a.get<glm::vec4>(0).z + bary.y/b.get<glm::vec4>(0).z + bary.z/c.get<glm::vec4>(0).z;
        glm::vec4 pos = bary.x*a.get<glm::vec4>(0) + bary.y*b.get<glm::vec4>(0) + bary.z*c.get<glm::vec4>(0);
        pointData.set(0, pos.z);
        // std::cout << a.get<glm::vec4>(0).z << " " << b.get<glm::vec4>(0).z << " " << c.get<glm::vec4>(0).z << "\n";
        float z_div = bary.x/a.get<glm::vec4>(0).z + bary.y/b.get<glm::vec4>(0).z + bary.z/c.get<glm::vec4>(0).z;
        // pointData.set(1, ((bary.x*a.get<glm::vec4>(1)/a.get<glm::vec4>(0).z + bary.y*b.get<glm::vec4>(1)/b.get<glm::vec4>(0).z + bary.z*c.get<glm::vec4>(1))/c.get<glm::vec4>(0).z)/z_div);
        pointData.set(1, (bary.x*a.get<glm::vec4>(1) + bary.y*b.get<glm::vec4>(1) + bary.z*c.get<glm::vec4>(1)));
        // pointData.print();
        return pointData;
    }

    glm::vec3 triangle::findZ(glm::vec2 point){
        glm::vec3 a = glm::vec3(this->a.get<glm::vec4>(0).x, this->a.get<glm::vec4>(0).y, this->a.get<glm::vec4>(0).z);
        glm::vec3 b = glm::vec3(this->b.get<glm::vec4>(0).x, this->b.get<glm::vec4>(0).y, this->b.get<glm::vec4>(0).z);
        glm::vec3 c = glm::vec3(this->c.get<glm::vec4>(0).x, this->c.get<glm::vec4>(0).y, this->c.get<glm::vec4>(0).z);

        glm::vec3 normal = glm::cross(b-a, c-a);
        float z = normal.x * (a.x - point.x) + normal.y * (a.y - point.y) + normal.z * a.z;
        z /= normal.z;
        return glm::vec3(point.x, point.y, z);
    }

    void triangle::print(){
        std::cout << "vertex 1 => ";
        std::cout << this->a.get<glm::vec4>(0).x << " " << this->a.get<glm::vec4>(0).y << " " << this->a.get<glm::vec4>(0).z << " " << this->a.get<glm::vec4>(0).w << " ";
        std::cout << this->a.get<glm::vec4>(1).x << " " << this->a.get<glm::vec4>(1).y << " " << this->a.get<glm::vec4>(1).z << " " << this->a.get<glm::vec4>(1).w << "\n ";

        std::cout << "vertex 2 => ";
        std::cout << this->b.get<glm::vec4>(0).x << " " << this->b.get<glm::vec4>(0).y << " " << this->b.get<glm::vec4>(0).z << " " << this->b.get<glm::vec4>(0).w << " ";
        std::cout << this->a.get<glm::vec4>(1).x << " " << this->b.get<glm::vec4>(1).y << " " << this->b.get<glm::vec4>(1).z << " " << this->b.get<glm::vec4>(1).w << "\n ";

        std::cout << "vertex 3 => ";
        std::cout << this->c.get<glm::vec4>(0).x << " " << this->c.get<glm::vec4>(0).y << " " << this->c.get<glm::vec4>(0).z << " " << this->c.get<glm::vec4>(0).w << " ";
        std::cout << this->c.get<glm::vec4>(1).x << " " << this->c.get<glm::vec4>(1).y << " " << this->c.get<glm::vec4>(1).z << " " << this->c.get<glm::vec4>(1).w << "\n ";
    }
}