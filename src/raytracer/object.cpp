#include "object.hpp"

namespace raytracer {
    rt_scalar sphere::intersects(const ray& ray) {
        auto distance = ray.origin() - _pos;
        auto a        = glm::dot(ray.direction(), ray.direction());
        auto b        = 2.0 * glm::dot(distance, ray.direction());
        auto d        = b * b -
                 4.0 * a * (glm::dot(distance, distance) - _radius * _radius);
        if (d < 0.0)
            return -std::numeric_limits<rt_scalar>::max();

        return (-b - glm::sqrt(d)) / (2.0 * a);
    }

    rt_vec3 sphere::sample(const rt_vec3& p) {
        auto normal = glm::normalize(p - rt_vec3(0.0, 0.0, -1.0));
        return static_cast<rt_scalar>(0.5) *
               rt_vec3(normal.x + 1.0, normal.y + 1.0, normal.z + 1.0);
    }
} // namespace raytracer