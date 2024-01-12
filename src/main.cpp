#include "image.hpp"
#include "pixelflut.hpp"
#include "raytracer/grandma.hpp"
#include <algorithm>
#include <execution>
#include <iostream>
#include <print>
#include <ranges>
#include <string>

// TODO: proper error handling

int32_t main(void) {
    try {
        pixelflut_client client("pixelflut.uwu.industries", "1234");
        auto size = client.read_size();

        namespace rt = raytracer;

        rt::grandma granny(
            {{size.x, size.y, 2.0, 1.0, {0.0, 0.5, 1.0}}, 32, 128}, size.x,
            size.y);

        auto sphere =
            std::make_shared<rt::sphere>(rt_vec3{0.0, 0.5, 0.0}, 1.0 / 2.0,
                                         std::make_shared<rt::sphere_mat>());
        auto ground = std::make_shared<rt::sphere>(
            rt_vec3{0.0, -10000.0, 0.0}, 10000,
            std::make_shared<rt::sphere_mat>(rt_vec3(0.5, 0.5, 1.0)));
        granny.scene().push_object(sphere);
        granny.scene().push_object(ground);

        client.trace_rays(granny);

    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}