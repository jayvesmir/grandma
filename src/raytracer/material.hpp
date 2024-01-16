#pragma once
#include "../types.hpp"
#include "intersection.hpp"

namespace raytracer {
    struct material_sample {
        rt_vec3 color;
        ray child;
    };

    // TODO: make default material
    class material {
      public:
        virtual ~material() = default;

        virtual material_sample sample(const intersection& p [[maybe_unused]]) {
            return {intersection::invalid().pos, {}};
        }
    };

    class sky_mat : public material {
        rt_vec3 _bottom_color, _top_color;

      public:
        sky_mat(const rt_vec3& bottom_color = rt_vec3(1.0, 1.0, 1.0),
                const rt_vec3& top_color    = rt_vec3(0.1, 0.4, 1.0))
            : _bottom_color(bottom_color), _top_color(top_color) {}

        material_sample sample(const intersection& p) override;
    };

    class matte_sphere_mat : public material {
        rt_vec3 _color;
        bool _return_normal;
        rt_scalar _reflectance;

      public:
        matte_sphere_mat()
            : _color({0.0, 0.0, 0.0}), _return_normal(true), _reflectance(0.8) {
        }
        matte_sphere_mat(const rt_vec3& color, rt_scalar reflectance = 0.8)
            : _color(color), _return_normal(false), _reflectance(reflectance) {}

        material_sample sample(const intersection& p) override;
    };

    class mirror_sphere_mat : public material {
        rt_vec3 _color;
        rt_scalar _roughness;

      public:
        mirror_sphere_mat() : _color({1.0, 1.0, 1.0}), _roughness(0.0) {}
        mirror_sphere_mat(const rt_vec3& color, rt_scalar roughness)
            : _color(color), _roughness(roughness) {}

        material_sample sample(const intersection& p) override;
    };
} // namespace raytracer