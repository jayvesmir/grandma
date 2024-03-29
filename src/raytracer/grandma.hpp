#pragma once
#include "../types.hpp"
#include "glm/geometric.hpp"
#include "glm/vec3.hpp"
#include "scene.hpp"
#include <cinttypes>
#include <memory>

namespace raytracer {
    class grandma {
        scene _scene;
        uint32_t _width, _height;

      public:
        grandma(const scene& scene, uint32_t width, uint32_t height)
            : _scene(scene), _width(width), _height(height) {}

        constexpr auto width() { return _width; }
        constexpr auto height() { return _height; }
        constexpr auto& scene() { return _scene; }

        vec3<uint8_t> compute_pixel(uint32_t x, uint32_t y);
    };
} // namespace raytracer
