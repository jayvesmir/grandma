#pragma once
#include "../types.hpp"
#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

namespace raytracer {
    class ray {
        rt_vec3 _origin, _direction;

      public:
        constexpr ray() = default;
        constexpr ray(const rt_vec3& origin, const rt_vec3& direction)
            : _origin(origin), _direction(direction) {}

        constexpr const auto& origin() const { return _origin; }
        constexpr const auto& direction() const { return _direction; }

        constexpr auto at(rt_scalar t) const {
            return _origin + (t * _direction);
        }
    };
} // namespace raytracer