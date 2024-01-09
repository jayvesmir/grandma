#include "image.hpp"
#include <format>
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

image::~image() { std::free(_pixels.begin); }

void image::resize(int32_t new_width, int32_t new_height) {

    auto _ = reinterpret_cast<vec3<uint8_t>*>(stbir_resize_uint8_linear(
        reinterpret_cast<uint8_t*>(_pixels.begin), _width, _height, 0, nullptr,
        new_width, new_height, 0, static_cast<stbir_pixel_layout>(_comp)));
    std::free(_pixels.begin);
    _width  = new_width;
    _height = new_height;
    _pixels = {.begin = _, .end = _ + (_width * _height * _comp)};
}