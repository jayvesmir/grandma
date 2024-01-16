#include "material.hpp"
#include "../random.hpp"

namespace raytracer {
    material_sample sky_mat::sample(const intersection& p) {
        auto height = static_cast<rt_scalar>(0.5) *
                      (glm::normalize(p.origin_ray.direction()).y + 1.0);
        return {static_cast<rt_scalar>(1.0 - height) * _bottom_color +
                    static_cast<rt_scalar>(height) * _top_color,
                {}};
    }

    material_sample matte_sphere_mat::sample(const intersection& p) {
        if (_return_normal)
            return {_reflectance * static_cast<rt_scalar>(0.6) *
                        rt_vec3(p.normal.x + 1.0, p.normal.y + 1.0,
                                p.normal.z + 1.0),
                    {p.pos,
                     p.normal + random::vector_within_bounce_range(p.normal)}};
        return {
            _reflectance * _color,
            {p.pos, p.normal + random::vector_within_bounce_range(p.normal)}};
    }

    material_sample mirror_sphere_mat::sample(const intersection& p) {
        return {
            _color,
            {p.pos,
             glm::reflect(glm::normalize(p.origin_ray.direction()), p.normal) +
                 _roughness * random::vector_within_bounce_range(p.normal)}};
    }
} // namespace raytracer