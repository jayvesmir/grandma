#pragma once
#include "../types.hpp"
#include <cinttypes>

namespace raytracer {
    class grandma {
        uint32_t _width, _height;

      public:
        grandma(uint32_t width, uint32_t height)
            : _width(width), _height(height) {}

        vec3<uint8_t> compute_pixel(uint32_t x, uint32_t y);
    };
} // namespace raytracer
