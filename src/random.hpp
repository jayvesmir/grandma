#pragma once
#include "glm/geometric.hpp"
#include "types.hpp"
#include <random>

namespace random {
    static thread_local std::mt19937_64 engine;

    static inline auto scalar(rt_scalar min = 0.0, rt_scalar max = 1.0) {
        return std::uniform_real_distribution<rt_scalar>(min, max)(engine);
    }

    static inline auto vector(rt_scalar min = 0.0, rt_scalar max = 1.0) {
        auto dist = std::uniform_real_distribution<rt_scalar>(min, max);
        return rt_vec3(dist(engine), dist(engine), dist(engine));
    }

    static inline auto vector_within_sphere() {
        auto vec = vector(-1.0, 1.0);
        while (glm::dot(vec, vec) > static_cast<rt_scalar>(1.0))
            vec = vector(-1.0, 1.0);
        return vec;
    }

    static inline auto vector_within_disk() {
        auto vec = rt_vec3(scalar(-1.0, 1.0), scalar(-1.0, 1.0),
                           static_cast<rt_scalar>(0.0));
        while (glm::dot(vec, vec) > static_cast<rt_scalar>(1.0))
            vec = rt_vec3(scalar(-1.0, 1.0), scalar(-1.0, 1.0),
                          static_cast<rt_scalar>(0.0));
        return vec;
    }

    static inline auto vector_within_bounce_range(const rt_vec3& normal) {
        auto vec = glm::normalize(vector_within_sphere());
        return glm::dot(vec, normal) > static_cast<rt_scalar>(0.0) ? vec : -vec;
    }
} // namespace random