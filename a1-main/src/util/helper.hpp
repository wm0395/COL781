#ifndef HELPER_HPP
#define HELPER_HPP

#include <SDL2/SDL.h>

namespace helper{
    template<typename T> void swap(T &a, T&b){
        T c = a;
        a = b;
        b = c;
    }
}

#endif