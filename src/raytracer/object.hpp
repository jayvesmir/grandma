#pragma once
#include "../types.hpp"
#include "glm/vec3.hpp"
#include "ray.hpp"
#include <limits>

namespace raytracer {
    class object {
      public:
        virtual ~object() = default;

        virtual rt_scalar intersects(const ray& ray [[maybe_unused]]) {
            return -std::numeric_limits<rt_scalar>::max();
        }

        virtual rt_vec3 sample(const rt_vec3& p [[maybe_unused]]) {
            return {-std::numeric_limits<rt_scalar>::max(),
                    -std::numeric_limits<rt_scalar>::max(),
                    -std::numeric_limits<rt_scalar>::max()};
        }
    };

    class sphere : public object {
        rt_vec3 _pos;
        rt_scalar _radius;

      public:
        sphere(const rt_vec3& pos, rt_scalar radius)
            : _pos(pos), _radius(radius) {}

        void pos(const rt_vec3& pos) { _pos = pos; }
        void radius(rt_scalar radius) { _radius = radius; }

        constexpr const auto& pos() { return _pos; }
        constexpr const auto& radius() { return _radius; }

        rt_scalar intersects(const ray& ray) override;
        rt_vec3 sample(const rt_vec3& p) override;
    };
} // namespace raytracer