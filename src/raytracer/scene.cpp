#include "scene.hpp"
#include "../random.hpp"

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

    rt_vec3 camera::sample_offset() {
        return ((-0.5 + random::scalar()) * _pixel_delta_uv.u) +
               ((-0.5 + random::scalar()) * _pixel_delta_uv.v);
    }

    rt_vec3 scene::sample_direction(uint32_t x, uint32_t y) {
        return ((_camera.top_left_corner() +
                 (static_cast<rt_scalar>(x) * _camera.pixel_delta_uv().u) +
                 (static_cast<rt_scalar>(y) * _camera.pixel_delta_uv().v)) +
                _camera.sample_offset() - _camera.pos());
    }

    hit scene::intersect_objects(const ray& ray) {
        hit closest = {false, intersection::invalid(), nullptr};
        for (auto& obj : _objects) {
            auto current = obj->intersect(ray);
            if (current.distance >= 0.0 &&
                (!closest.has_hit || current.distance < closest.point.distance))
                closest = {true, current, obj};
        }
        return closest;
    }

    rt_vec3 scene::trace_ray(const ray& ray) {
        auto bg_intersection       = intersection::invalid();
        bg_intersection.origin_ray = ray;
        if (_objects.size() == 0)
            return _background_mat->sample(bg_intersection);

        auto closest = intersect_objects(ray);

        if (!closest.has_hit)
            return _background_mat->sample(bg_intersection);

        rt_vec3 color = closest.obj->sample(closest.point);
        for (auto bounce = 0u; bounce < _max_bounces; bounce++) {
            auto hit = intersect_objects(
                {closest.point.pos,
                 static_cast<rt_scalar>(0.5) * closest.point.normal +
                     random::vector_within_bounce_range(closest.point.normal)});

            if (hit.has_hit)
                color *= _reflectance * hit.obj->sample(hit.point);
            else {
                color *= _reflectance * _background_mat->sample(hit.point);
                break;
            }
        }

        return color;
    };

    rt_vec3 scene::trace_ray(uint32_t x, uint32_t y) {
        rt_vec3 out_color(0.0, 0.0, 0.0);
        for (auto sample = 0u; sample < _max_samples; sample++)
            out_color += trace_ray({_camera.pos(), sample_direction(x, y)});
        return glm::sqrt(out_color * (1.0 / _max_samples));
    };
} // namespace raytracer