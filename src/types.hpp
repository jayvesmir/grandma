#pragma once
#include <iterator>

template <class scalar_t> struct vec2 {
    scalar_t x, y;
};

template <class scalar_t> struct vec3 {
    scalar_t x, y, z;
};

template <class scalar_t> struct vec4 {
    scalar_t x, y, z, w;
};

template <class iterator_t> struct view {
    iterator_t begin, end;
};