#include "material.hpp"

namespace raytracer {
    rt_vec3 sphere_mat::sample(const intersection& p) {
        if (_return_normal)
            return static_cast<rt_scalar>(0.5) * rt_vec3(p.normal.x + 1.0,
                                                         p.normal.y + 1.0,
                                                         p.normal.z + 1.0);
        return _color;
    }
} // namespace raytracer