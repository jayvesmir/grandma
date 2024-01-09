#include "image.hpp"
#include "pixelflut.hpp"
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

        client.trace_rays();

    } catch (const std::exception& e) {
        std::println("[error] {}", e.what());
    }
}