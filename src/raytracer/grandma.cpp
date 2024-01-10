#include "grandma.hpp"
#include "ray.hpp"

namespace raytracer {
    camera::camera(uint32_t image_width, uint32_t image_height,
                   rt_scalar viewport_height, rt_scalar focal_length,
                   rt_vec3 pos)
        : _pos(pos), _height(viewport_height), _focal_length(focal_length) {
        _width = _height * (static_cast<rt_scalar>(image_width) / image_height);
        _viewport_uv = {
            .u = rt_vec3(_width, 0.0, 0.0),
            .v = rt_vec3(0.0, -_height, 0.0),
        };
        _pixel_delta_uv = {
            .u = _viewport_uv.u / static_cast<rt_scalar>(image_width),
            .v = _viewport_uv.v / static_cast<rt_scalar>(image_height),
        };

        auto _ = _pos - rt_vec3(0, 0, _focal_length) -
                 _viewport_uv.u / static_cast<rt_scalar>(2.0) -
                 _viewport_uv.v / static_cast<rt_scalar>(2.0);
        _top_left_corner = _ + static_cast<rt_scalar>(0.5) *
                                   (_pixel_delta_uv.u + _pixel_delta_uv.v);
    }

    vec3<uint8_t> grandma::compute_pixel(uint32_t x, uint32_t y) {
        auto out_color = _scene.trace_ray(x, y);
        return {static_cast<uint8_t>(out_color.x * 255.999),
                static_cast<uint8_t>(out_color.y * 255.999),
                static_cast<uint8_t>(out_color.z * 255.999)};
    }
} // namespace raytracer
