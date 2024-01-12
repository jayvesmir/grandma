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