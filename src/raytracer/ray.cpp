#include "ray.hpp"

namespace raytracer {
    rt_vec3 ray::at(rt_scalar t) const { return _origin + (t * _direction); }
} // namespace raytracer