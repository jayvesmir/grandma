#pragma once
#include "../types.hpp"
#include "object.hpp"
#include <memory>
#include <vector>

namespace raytracer {
    struct uv {
        rt_vec3 u, v;
    };

    struct hit {
        bool has_hit;
        intersection point;
        std::shared_ptr<object> obj;
    };

    class camera {
        uv _viewport, _pixel_delta, _bokeh_disk;
        rt_vec3 _pos, _target, _up, _top_left_corner, _u, _v, _w;
        rt_scalar _width, _height, _fov, _focus_distance, _bokeh_angle;

      public:
        camera() = default;
        camera(uint32_t image_width, uint32_t image_height,
               rt_scalar fov = 90.0, rt_scalar bokeh_angle = 0.0,
               rt_scalar focus_distance = -1.0,
               rt_vec3 pos              = rt_vec3(0.0, 0.0, 0.0),
               rt_vec3 target           = rt_vec3(0.0, 0.0, 0.0),
               rt_vec3 up               = rt_vec3(0.0, 1.0, 0.0));

        constexpr const auto& pos() const { return _pos; }
        constexpr auto pos(const rt_vec3& pos) { _pos = pos; }
        constexpr const auto& target() const { return _target; }
        constexpr auto target(const rt_vec3& target) { _target = target; }
        constexpr const auto& up() const { return _up; }
        constexpr auto up(const rt_vec3& up) { _up = up; }

        constexpr const auto& fov() const { return _fov; }
        constexpr const auto& viewport_width() const { return _width; }
        constexpr const auto& viewport_height() const { return _height; }
        constexpr const auto& viewport() const { return _viewport; }
        constexpr const auto& focus_distance() const { return _focus_distance; }
        constexpr const auto& bokeh_angle() const { return _bokeh_angle; }
        constexpr const auto& pixel_delta() const { return _pixel_delta; }
        constexpr const auto& bokeh_disk() const { return _bokeh_disk; }
        constexpr const auto& top_left_corner() const {
            return _top_left_corner;
        }

        rt_vec3 bokeh_sample() const;
        rt_vec3 sample_offset() const;
    };

    class scene {
        camera _camera;
        uint32_t _max_bounces, _max_samples;
        std::shared_ptr<material> _background_mat;
        std::vector<std::shared_ptr<object>> _objects;

        hit intersect_objects(const ray& ray) const;
        rt_vec3 sample_direction(uint32_t x, uint32_t y) const;

      public:
        scene(const camera& camera, uint32_t max_bounces = 12,
              uint32_t max_samples = 32,
              const std::shared_ptr<material>& background_mat =
                  std::make_shared<sky_mat>())
            : _camera(camera), _max_bounces(max_bounces),
              _max_samples(max_samples), _background_mat(background_mat) {}

        constexpr auto& camera() { return _camera; }
        constexpr auto samples() const { return _max_samples; }
        constexpr auto bounces() const { return _max_bounces; }
        constexpr const auto& objects() const { return _objects; }
        constexpr const auto& background_mat() const { return _background_mat; }

        void push_object(const std::shared_ptr<object>& object) {
            _objects.push_back(object);
        }

        void pop_object() { _objects.pop_back(); }

        rt_vec3 trace_ray(const ray& ray) const;
        rt_vec3 trace_ray(uint32_t x, uint32_t y) const;
    };
} // namespace raytracer