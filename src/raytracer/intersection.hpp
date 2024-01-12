#pragma once
#include "../types.hpp"
#include "ray.hpp"
#include <glm/geometric.hpp>

namespace raytracer {
    struct intersection {
        rt_scalar distance;
        rt_vec3 pos, normal;
        bool inside;

        static constexpr auto invalid() {
            return intersection{-std::numeric_limits<rt_scalar>::max(),
                                {-std::numeric_limits<rt_scalar>::max(),
                                 -std::numeric_limits<rt_scalar>::max(),
                                 -std::numeric_limits<rt_scalar>::max()},
                                {-std::numeric_limits<rt_scalar>::max(),
                                 -std::numeric_limits<rt_scalar>::max(),
                                 -std::numeric_limits<rt_scalar>::max()},
                                false};
        }

        constexpr auto check_side(const ray& ray) {
            inside = glm::dot(ray.direction(), normal) < 0;
            normal = inside ? normal : -normal;
            return inside;
        }
    };
} // namespace raytracer