#include "Ray.hpp"

inline Ray::Ray(vec4 &o, vec4 &d){
    this->origin = o;
    this->direction = d;
}