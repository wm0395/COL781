#ifndef GEOMETRIC_HPP
#define GEOMETRIC_HPP

#include <glm/glm.hpp>
using namespace COL781;
namespace Geometric{
    class line {
    public:
        glm::vec2 a, b;
        line();
        line(glm::vec2 a, glm::vec2 b);
        line(glm::vec3 a, glm::vec3 b);
        line(glm::vec4 a, glm::vec4 b);

        bool isLeft(glm::vec2 p);
        bool isLeft(glm::vec3 p);
        bool isLeft(glm::vec4 p);

        float intercept_x(float y);

        float intercept_y(float x);

        void print();
    };
    class triangle {
    public:
        Software::Attribs a, b, c;
        line A, B, C;

        // triangle();
        // triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        triangle(std::vector<Software::Attribs> vertices);

        bool isInside(glm::vec2 p);

        bool isInside(glm::vec3 p);

        void print();

        float calculateTriangleArea(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

        glm::vec3 getBarycentric(glm::vec3 point);

    private:
        void make_anti_clockwise();
        glm::vec3 findZ(glm::vec2 point);
    };
}

#endif
