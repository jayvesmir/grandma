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
        size.x    = 384;
        size.y    = 256;

        namespace rt = raytracer;

        rt::scene scene({size.x, size.y});
        rt::grandma granny(scene, size.x, size.y);

        auto sphere = std::make_shared<rt::sphere>(
            rt_vec3{-1.0, 0.0, -1.0}, 0.5, std::make_shared<rt::sphere_mat>());
        auto ground = std::make_shared<rt::sphere>(
            rt_vec3{0.0, -100.0, -1.0}, 99.5,
            std::make_shared<rt::sphere_mat>(rt_vec3(0.5, 0.5, 1.0)));
        granny.scene().push_object(sphere);
        granny.scene().push_object(ground);

        while (true) {
            client.trace_rays(granny);
            sphere->pos(sphere->pos() + rt_vec3(0.1, 0.0, -0.1));
        }

    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}