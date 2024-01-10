#include "scene.hpp"

namespace raytracer {
    struct hit {
        rt_scalar distance;
        std::vector<std::shared_ptr<object>>::iterator obj;
    };

    rt_vec3 scene::trace_ray(uint32_t x, uint32_t y) {
        auto pixel_center =
            _camera.top_left_corner() +
            (static_cast<rt_scalar>(x) * _camera.pixel_delta_uv().u) +
            (static_cast<rt_scalar>(y) * _camera.pixel_delta_uv().v);

        ray current(_camera.pos(), pixel_center - _camera.pos());

        if (_objects.size() == 0) {
            auto att = 0.5 * (glm::normalize(current.direction()).y + 1.0);
            return static_cast<rt_scalar>(1.0 - att) * rt_vec3(1.0, 1.0, 1.0) +
                   static_cast<rt_scalar>(att) * rt_vec3(0.5, 0.7, 1.0);
        }

        auto closest =
            hit{_objects.front()->intersects(current), _objects.begin()};
        for (auto it = _objects.begin() + 1; it != _objects.end(); it++) {
            auto t = (*it)->intersects(current);
            if (t < closest.distance)
                closest = {t, it};
        }

        if (closest.distance < 0.0) {
            auto att = 0.5 * (glm::normalize(current.direction()).y + 1.0);
            return static_cast<rt_scalar>(1.0 - att) * rt_vec3(1.0, 1.0, 1.0) +
                   static_cast<rt_scalar>(att) * rt_vec3(0.5, 0.7, 1.0);
        }

        return (*closest.obj)->sample(current.lerp(closest.distance));
    };
} // namespace raytracer