#pragma once
#include "types.hpp"
#include <string>
#include <vector>

class image {
    int32_t _width, _height, _comp;
    view<vec3<uint8_t>*> _pixels;

  public:
    ~image();
    image(const std::string& filepath);

    constexpr auto width() { return _width; }
    constexpr auto height() { return _height; }
    constexpr const auto& pixels() { return _pixels; }
    constexpr auto get(uint32_t x, uint32_t y) {
        return *(_pixels.begin + ((y * _width) + x));
    }

    void resize(int32_t new_width, int32_t new_height);
};