#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

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

struct pixelflut_pt {
    vec2<int32_t> pos;
    vec3<uint8_t> color;
};

using rt_scalar = double;
using rt_vec2   = glm::highp_dvec2;
using rt_vec3   = glm::highp_dvec3;
using rt_vec4   = glm::highp_dvec4;