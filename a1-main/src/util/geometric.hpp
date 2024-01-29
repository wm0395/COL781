#ifndef GEOMETRIC_HPP
#define GEOMETRIC_HPP

#include <glm/glm.hpp>
namespace Geometric{
    class line {
    public:
        glm::vec2 a, b;
        line();
        line(glm::vec2 a, glm::vec2 b);

        bool isLeft(glm::vec2 &p);

        bool isLeft(glm::vec3 &p);

        float intercept_x(float y);

        float intercept_y(float x);
    };
    class triangle {
    public:
        glm::vec3 a, b, c;
        line A, B, C;

        triangle();
        triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

        bool isInside(glm::vec2 p);

        bool isInside(glm::vec3 p);

    private:
        void make_anti_clockwise();
    };
}

#endif
