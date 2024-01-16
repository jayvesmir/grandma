#include "scene.hpp"
#include "../random.hpp"
#include "glm/glm.hpp"

namespace raytracer {
    camera::camera(uint32_t image_width, uint32_t image_height, rt_scalar fov,
                   rt_vec3 pos, rt_vec3 target, rt_vec3 up)
        : _pos(pos), _target(target), _up(up), _fov(fov) {
        _focal_length = glm::length(_pos - _target);
        _height       = static_cast<rt_scalar>(2.0) *
                  glm::tan(glm::radians(_fov) / static_cast<rt_scalar>(2.0)) *
                  _focal_length;
        _width = _height * (static_cast<rt_scalar>(image_width) / image_height);

        _w = glm::normalize(_pos - _target);
        _u = glm::normalize(glm::cross(_up, _w));
        _v = glm::cross(_w, _u);

        _viewport_uv = {
            .u = _width * _u,
            .v = _height * -_v,
        };
        _pixel_delta_uv = {
            .u = _viewport_uv.u / static_cast<rt_scalar>(image_width),
            .v = _viewport_uv.v / static_cast<rt_scalar>(image_height),
        };

        auto _ = _pos - (_focal_length * _w) -
                 _viewport_uv.u / static_cast<rt_scalar>(2.0) -
                 _viewport_uv.v / static_cast<rt_scalar>(2.0);
        _top_left_corner = _ + static_cast<rt_scalar>(0.5) *
                                   (_pixel_delta_uv.u + _pixel_delta_uv.v);
    }

    rt_vec3 camera::sample_offset() const {
        return ((-0.5 + random::scalar()) * _pixel_delta_uv.u) +
               ((-0.5 + random::scalar()) * _pixel_delta_uv.v);
    }

    rt_vec3 scene::sample_direction(uint32_t x, uint32_t y) const {
        return ((_camera.top_left_corner() +
                 (static_cast<rt_scalar>(x) * _camera.pixel_delta_uv().u) +
                 (static_cast<rt_scalar>(y) * _camera.pixel_delta_uv().v)) +
                _camera.sample_offset() - _camera.pos());
    }

    hit scene::intersect_objects(const ray& ray) const {
        hit closest = {false, intersection::invalid(), nullptr};
        for (auto& obj : _objects) {
            auto current = obj->intersect(ray);
            if (current.distance >= 0.0 &&
                (!closest.has_hit || current.distance < closest.point.distance))
                closest = {true, current, obj};
        }
        return closest;
    }

    rt_vec3 scene::trace_ray(const ray& ray) const {
        auto bg_intersection       = intersection::invalid();
        bg_intersection.origin_ray = ray;
        if (_objects.size() == 0)
            return _background_mat->sample(bg_intersection).color;

        auto hit = intersect_objects(ray);

        if (!hit.has_hit)
            return _background_mat->sample(bg_intersection).color;

        auto sample = hit.obj->sample(hit.point);
        auto color  = sample.color;

        for (auto bounce = 0u; bounce < _max_bounces; bounce++) {
            auto hit = intersect_objects(sample.child);

            if (hit.has_hit) {
                sample = hit.obj->sample(hit.point);
                color *= sample.color;
            } else {
                sample = _background_mat->sample(hit.point);
                color *= sample.color;
                break;
            }
        }

        return color;
    };

    rt_vec3 scene::trace_ray(uint32_t x, uint32_t y) const {
        rt_vec3 out_color(0.0, 0.0, 0.0);
        for (auto sample = 0u; sample < _max_samples; sample++)
            out_color += trace_ray({_camera.pos(), sample_direction(x, y)});
        return glm::sqrt(out_color * (1.0 / _max_samples));
    };
} // namespace raytracer