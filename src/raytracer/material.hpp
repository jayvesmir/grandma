#pragma once
#include "../types.hpp"
#include "intersection.hpp"

namespace raytracer {
    // TODO: make default material
    class material {
      public:
        virtual ~material() = default;

        virtual rt_vec3 sample(const intersection& p [[maybe_unused]]) {
            return intersection::invalid().pos;
        }
    };

    class sky_mat : public material {
        rt_vec3 _bottom_color, _top_color;

      public:
        sky_mat(const rt_vec3& bottom_color = rt_vec3(1.0, 1.0, 1.0),
                const rt_vec3& top_color    = rt_vec3(0.1, 0.4, 1.0))
            : _bottom_color(bottom_color), _top_color(top_color) {}

        rt_vec3 sample(const intersection& p) override;
    };

    class sphere_mat : public material {
        rt_vec3 _color;
        bool _return_normal;

      public:
        sphere_mat() : _color({0.0, 0.0, 0.0}), _return_normal(true) {}
        sphere_mat(const rt_vec3& color)
            : _color(color), _return_normal(false) {}

        rt_vec3 sample(const intersection& p) override;
    };
} // namespace raytracer