#include "material.hpp"

namespace raytracer {
    rt_vec3 sky_mat::sample(const intersection& p) {
        auto height = static_cast<rt_scalar>(0.5) *
                      (glm::normalize(p.origin_ray.direction()).y + 1.0);
        return static_cast<rt_scalar>(1.0 - height) * _bottom_color +
               static_cast<rt_scalar>(height) * _top_color;
    }

    rt_vec3 matte_sphere_mat::sample(const intersection& p) {
        if (_return_normal)
            return static_cast<rt_scalar>(0.6) * rt_vec3(p.normal.x + 1.0,
                                                         p.normal.y + 1.0,
                                                         p.normal.z + 1.0);
        return _color;
    }
} // namespace raytracer