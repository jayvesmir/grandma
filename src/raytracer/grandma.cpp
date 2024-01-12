#include "grandma.hpp"
#include "ray.hpp"

namespace raytracer {
    vec3<uint8_t> grandma::compute_pixel(uint32_t x, uint32_t y) {
        if (x >= _width || y >= _height)
            return {0, 0, 0};
        auto out_color = _scene.trace_ray(x, y);
        return {static_cast<uint8_t>(out_color.x * 255.999),
                static_cast<uint8_t>(out_color.y * 255.999),
                static_cast<uint8_t>(out_color.z * 255.999)};
    }
} // namespace raytracer
