#include "image.hpp"
#include <algorithm>
#include <execution>
#include <format>
#include <ranges>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"

image::image(const std::string& filepath) {
    auto _ = reinterpret_cast<vec3<uint8_t>*>(
        stbi_load(filepath.c_str(), &_width, &_height, &_comp, 3));
    if (_ == nullptr)
        throw std::runtime_error(std::format("stbi_load() failed"));
    _pixels = {_, _ + (_width * _height * _comp)};
}

image::image(uint32_t width, uint32_t height)
    : _width(width), _height(height), _comp(3) {
    auto* _ = new vec3<uint8_t>[_width * _height];
    _pixels = {_, _ + (_width * _height)};
}

image::~image() { std::free(_pixels.begin); }

vec3<uint8_t> image::get(uint32_t x, uint32_t y) const {
    return *(_pixels.begin + ((y * _width) + x));
}

void image::resize(int32_t new_width, int32_t new_height) {

    auto _ = reinterpret_cast<vec3<uint8_t>*>(stbir_resize_uint8_linear(
        reinterpret_cast<uint8_t*>(_pixels.begin), _width, _height, 0, nullptr,
        new_width, new_height, 0, static_cast<stbir_pixel_layout>(_comp)));
    std::free(_pixels.begin);
    _width  = new_width;
    _height = new_height;
    _pixels = {.begin = _, .end = _ + (_width * _height * _comp)};
}

std::vector<pixelflut_pt> image::map() const {
    std::vector<pixelflut_pt> out(_width * _height);
    auto x = 0, y = 0;
    std::generate(std::execution::par_unseq, out.begin(), out.end(), [&]() {
        pixelflut_pt pt = {{x, y}, get(x, y)};
        if (x < _width)
            x++;
        else {
            x = 0;
            y++;
        }
        return pt;
    });
    return out;
}