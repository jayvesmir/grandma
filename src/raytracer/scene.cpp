#include "scene.hpp"

namespace raytracer {
    struct hit {
        bool has_hit;
        intersection p;
        std::shared_ptr<object> obj;
    };

    rt_vec3 scene::trace_ray(uint32_t x, uint32_t y) const {
        auto pixel_center =
            _camera.top_left_corner() +
            (static_cast<rt_scalar>(x) * _camera.pixel_delta_uv().u) +
            (static_cast<rt_scalar>(y) * _camera.pixel_delta_uv().v);

        ray ray(_camera.pos(), pixel_center - _camera.pos());

        if (_objects.size() == 0) {
            auto att = 0.5 * (glm::normalize(ray.direction()).y + 1.0);
            return static_cast<rt_scalar>(1.0 - att) * rt_vec3(1.0, 1.0, 1.0) +
                   static_cast<rt_scalar>(att) * rt_vec3(0.5, 0.7, 1.0);
        }

        // TODO: this can be optimized
        auto closest =
            hit{false, _objects.front()->intersect(ray), _objects.front()};
        closest.has_hit = closest.p.distance >= 0.0;
        for (auto& obj : _objects) {
            auto current = obj->intersect(ray);
            if (current.distance >= 0.0 &&
                (!closest.has_hit || current.distance < closest.p.distance))
                closest = {true, current, obj};
        }

        if (!closest.has_hit) {
            auto height = 0.5 * (glm::normalize(ray.direction()).y + 1.0);
            return static_cast<rt_scalar>(1.0 - height) *
                       rt_vec3(1.0, 1.0, 1.0) +
                   static_cast<rt_scalar>(height) * rt_vec3(0.5, 0.7, 1.0);
        }

        return closest.obj->sample(closest.p);
    };
} // namespace raytracer