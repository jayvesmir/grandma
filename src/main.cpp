#include "CLI/CLI.hpp"
#include "image.hpp"
#include "pixelflut.hpp"
#include "raytracer/grandma.hpp"
#include <print>
#include <string>
#include <vector>

// TODO: proper error handling
int32_t main(int32_t argc, char** argv) {
    try {
        CLI::App app("pixelflut CPU raytracer", "grandma");
        argv = app.ensure_utf8(argv);

        std::string host, port;
        app.add_option("--host", host, "pixelflut server URL");
        app.add_option("--port", port, "pixelflut server port");

        CLI11_PARSE(app, argc, argv);
        if (host.empty() || port.empty())
            throw std::runtime_error(std::format(
                "only partial host address supplied: '{}:{}'", host, port));

        pixelflut_client client(host, port);
        auto size = client.read_size();
        size.y    = 256;

        namespace rt = raytracer;

        rt::grandma granny(
            {{size.x, size.y, 2.0, 1.0, {0.0, 0.5, 1.0}}, 32, 128}, size.x,
            size.y);

        auto sphere0 = std::make_shared<rt::sphere>(
            rt_vec3{-2 / 3.0, 0.5, 0.0}, 0.5,
            std::make_shared<rt::matte_sphere_mat>());
        auto sphere1 = std::make_shared<rt::sphere>(
            rt_vec3{2 / 3.0, 1.0, -0.75}, 1.0,
            std::make_shared<rt::mirror_sphere_mat>(rt_vec3(1.0, 1.0, 1.0),
                                                    0.5));
        auto ground = std::make_shared<rt::sphere>(
            rt_vec3{0.0, -10000.0, 0.0}, 10000,
            std::make_shared<rt::matte_sphere_mat>(rt_vec3(0.5, 0.5, 1.0)));
        granny.scene().push_object(sphere0);
        granny.scene().push_object(sphere1);
        granny.scene().push_object(ground);

        client.trace_rays(granny);

    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}