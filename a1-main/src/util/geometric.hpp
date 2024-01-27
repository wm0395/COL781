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

        float intercept_x(float y);

        float intercept_y(float x);
    };
    class triangle {
    public:
        glm::vec2 a, b, c;
        line A, B, C;

        triangle();
        triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c);

        bool isInside(glm::vec2 p);

    private:
        void make_anti_clockwise();
    };
}

#endif
