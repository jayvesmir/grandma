#pragma once
#include "../types.hpp"
#include "glm/vec3.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <limits>
#include <memory>

namespace raytracer {
    class object {
      protected:
        std::shared_ptr<material> _mat;

      public:
        virtual ~object() = default;

        virtual intersection intersect(const ray& ray [[maybe_unused]]) {
            return intersection::invalid();
        }

        virtual material_sample sample(const intersection& p [[maybe_unused]]) {
            return {intersection::invalid().pos, {}};
        }
    };

    class sphere : public object {
        rt_vec3 _pos;
        rt_scalar _radius;

      public:
        sphere(const rt_vec3& pos, rt_scalar radius,
               const std::shared_ptr<material>& mat)
            : _pos(pos), _radius(radius) {
            _mat = mat;
        }

        void pos(const rt_vec3& pos) { _pos = pos; }
        void radius(rt_scalar radius) { _radius = radius; }

        constexpr const auto& pos() { return _pos; }
        constexpr const auto& radius() { return _radius; }

        intersection intersect(const ray& ray) override;
        material_sample sample(const intersection& p) override;
    };
} // namespace raytracer