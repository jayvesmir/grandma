#include "scene.hpp"
#include "../random.hpp"
#include "glm/glm.hpp"

namespace raytracer {
    camera::camera(uint32_t image_width, uint32_t image_height, rt_scalar fov,
                   rt_scalar bokeh_angle, rt_scalar focus_distance, rt_vec3 pos,
                   rt_vec3 target, rt_vec3 up)
        : _pos(pos), _target(target), _up(up), _fov(fov),
          _focus_distance(focus_distance), _bokeh_angle(bokeh_angle) {
        if (_focus_distance < static_cast<rt_scalar>(0.0))
            _focus_distance = glm::length(_pos - _target);

        _height = static_cast<rt_scalar>(2.0) * glm::radians(_fov) *
                  static_cast<rt_scalar>(0.5) * _focus_distance;
        _width = _height * (static_cast<rt_scalar>(image_width) / image_height);

        _w = glm::normalize(_pos - _target);
        _u = glm::normalize(glm::cross(_up, _w));
        _v = glm::cross(_w, _u);

        _viewport = {
            .u = _width * _u,
            .v = _height * -_v,
        };

        _pixel_delta = {
            .u = _viewport.u / static_cast<rt_scalar>(image_width),
            .v = _viewport.v / static_cast<rt_scalar>(image_height),
        };

        auto _ = _pos - (_focus_distance * _w) -
                 (_viewport.u * static_cast<rt_scalar>(0.5)) -
                 (_viewport.v * static_cast<rt_scalar>(0.5));
        _top_left_corner =
            _ + static_cast<rt_scalar>(0.5) * (_pixel_delta.u + _pixel_delta.v);

        auto bokeh_radius =
            glm::radians(_bokeh_angle) * static_cast<rt_scalar>(0.5);

        _bokeh_disk = {
            .u = bokeh_radius * _u,
            .v = bokeh_radius * _v,
        };
    }

    rt_vec3 camera::sample_offset() const {
        return ((random::scalar(-0.5, 0.5)) * _pixel_delta.u) +
               ((random::scalar(-0.5, 0.5)) * _pixel_delta.v);
    }

    rt_vec3 camera::bokeh_sample() const {
        if (_bokeh_angle <= static_cast<rt_scalar>(0.0))
            return _pos;
        auto _ = random::vector_within_disk();
        return _pos + (_.x * _bokeh_disk.u) + (_.y * _bokeh_disk.v);
    }

    rt_vec3 scene::sample_direction(uint32_t x, uint32_t y) const {
        return ((_camera.top_left_corner() +
                 (static_cast<rt_scalar>(x) * _camera.pixel_delta().u) +
                 (static_cast<rt_scalar>(y) * _camera.pixel_delta().v)) +
                _camera.sample_offset());
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
        for (auto sample = 0u; sample < _max_samples; sample++) {
            auto origin = _camera.bokeh_sample();
            out_color += trace_ray({origin, sample_direction(x, y) - origin});
        }
        return glm::sqrt(out_color / static_cast<rt_scalar>(_max_samples));
    };
} // namespace raytracer