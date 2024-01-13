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
        uv _viewport_uv, _pixel_delta_uv;
        rt_vec3 _pos, _top_left_corner;
        rt_scalar _width, _height, _focal_length;

      public:
        camera() = default;
        camera(uint32_t image_width, uint32_t image_height,
               rt_scalar viewport_height = 2.0, rt_scalar focal_length = 1.0,
               rt_vec3 pos = rt_vec3(0.0, 0.0, 0.0));

        constexpr const auto& pos() const { return _pos; }
        constexpr auto pos(const rt_vec3& pos) { _pos = pos; }
        constexpr const auto& viewport_width() const { return _width; }
        constexpr const auto& viewport_height() const { return _height; }
        constexpr const auto& viewport_uv() const { return _viewport_uv; }
        constexpr const auto& focal_length() const { return _focal_length; }
        constexpr const auto& pixel_delta_uv() const { return _pixel_delta_uv; }
        constexpr const auto& top_left_corner() const {
            return _top_left_corner;
        }

        rt_vec3 sample_offset();
    };

    class scene {
        camera _camera;
        rt_scalar _reflectance;
        uint32_t _max_bounces, _max_samples;
        std::shared_ptr<material> _background_mat;
        std::vector<std::shared_ptr<object>> _objects;

        hit intersect_objects(const ray& ray);
        rt_vec3 sample_direction(uint32_t x, uint32_t y);

      public:
        scene(const camera& camera, uint32_t max_bounces = 12,
              uint32_t max_samples = 32, rt_scalar reflectance = 0.7,
              const std::shared_ptr<material>& background_mat =
                  std::make_shared<sky_mat>())
            : _camera(camera), _reflectance(reflectance),
              _max_bounces(max_bounces), _max_samples(max_samples),
              _background_mat(background_mat) {}

        constexpr auto& camera() { return _camera; }
        constexpr const auto& objects() const { return _objects; }

        void push_object(const std::shared_ptr<object>& object) {
            _objects.push_back(object);
        }

        void pop_object() { _objects.pop_back(); }

        rt_vec3 trace_ray(const ray& ray);
        rt_vec3 trace_ray(uint32_t x, uint32_t y);
    };
} // namespace raytracer