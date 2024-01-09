#include "pixelflut.hpp"
#include <iostream>
#include <print>
#include <string>

// TODO: proper error handling

int32_t main(int32_t argc, char** argv) {
    try {
        pixelflut_client client("pixelflut.uwu.industries", "1234");
        client.send_image(100, 100, "test.jpg");
    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}