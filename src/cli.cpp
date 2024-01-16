#include "cli.hpp"
#include "CLI/CLI.hpp"

std::pair<std::string, std::string> get_address(int32_t argc, char** argv) {
    CLI::App app("pixelflut CPU raytracer", "grandma");
    argv = app.ensure_utf8(argv);

    std::string host, port;
    app.add_option("--host", host, "pixelflut server URL");
    app.add_option("--port", port, "pixelflut server port");

    app.parse(argc, argv);
    if (host.empty() || port.empty())
        throw std::runtime_error(std::format(
            "only partial host address supplied: '{}:{}'", host, port));
    return {host, port};
}