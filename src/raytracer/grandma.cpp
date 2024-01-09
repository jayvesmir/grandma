#include "grandma.hpp"

namespace raytracer {
    vec3<uint8_t> grandma::compute_pixel(uint32_t x, uint32_t y) {
        auto r = double(x) / (_width - 1);
        auto g = double(y) / (_height - 1);
        auto b = 0;
        return {static_cast<uint8_t>(255.999 * r),
                static_cast<uint8_t>(255.999 * g),
                static_cast<uint8_t>(255.999 * b)};
    }
} // namespace raytracer
