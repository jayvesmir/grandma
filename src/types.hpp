#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
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

using rt_scalar = float;
using rt_vec2   = glm::vec2;
using rt_vec3   = glm::vec3;
using rt_vec4   = glm::vec4;

struct pixel {
    uint8_t r, g, b;
};