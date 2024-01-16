#include "cli.hpp"
#include "image.hpp"
#include "pixelflut.hpp"
#include "raytracer/grandma.hpp"
#include <print>

// TODO: proper error handling
int32_t main(int32_t argc, char** argv) {
    try {
        auto addr = get_address(argc, argv);

        pixelflut_client client(addr.first, addr.second);
        auto size = client.read_size();

        namespace rt = raytracer;

        rt::camera cam(size.x, size.y, 30.0, {-2.0, 3.5, 7.0},
                       {0.0, 0.75, 0.0});
        rt::grandma granny({cam, 32, 256}, size.x, size.y);

        auto sphere_matte = std::make_shared<rt::sphere>(
            rt_vec3{-2 / 3.0, 0.5, 0.0}, 0.5,
            std::make_shared<rt::matte_sphere_mat>());
        auto sphere_mirror = std::make_shared<rt::sphere>(
            rt_vec3{2 / 3.0, 1.0, -0.75}, 1.0,
            std::make_shared<rt::mirror_sphere_mat>());

        auto ground = std::make_shared<rt::sphere>(
            rt_vec3{0.0, -100.0, 0.0}, 100,
            std::make_shared<rt::matte_sphere_mat>(rt_vec3(0.5, 0.5, 1.0)));
        granny.scene().push_object(sphere_matte);
        granny.scene().push_object(sphere_mirror);
        granny.scene().push_object(ground);

        client.trace_rays(granny);

    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}