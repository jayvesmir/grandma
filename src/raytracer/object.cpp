#include "object.hpp"

namespace raytracer {
    intersection sphere::intersect(const ray& ray) {
        auto distance = ray.origin() - _pos;
        auto a        = glm::dot(ray.direction(), ray.direction());
        auto b        = 2.0 * glm::dot(distance, ray.direction());
        auto d        = b * b -
                 4.0 * a * (glm::dot(distance, distance) - _radius * _radius);
        if (d < 0.0)
            return intersection::invalid();
        auto t           = std::min((-b - glm::sqrt(d)) / (2.0 * a),
                                    (-b + glm::sqrt(d)) / (2.0 * a));
        auto hit         = ray.at(t);
        intersection out = {t, hit, (hit - _pos) / _radius, false};
        out.check_side(ray);
        return out;
    }

    rt_vec3 sphere::sample(const intersection& p) { return _mat->sample(p); }
} // namespace raytracer