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
    image(uint32_t width, uint32_t height);

    constexpr auto width() const { return _width; }
    constexpr auto height() const { return _height; }
    constexpr const auto& pixels() const { return _pixels; }
    vec3<uint8_t> get(uint32_t x, uint32_t y) const;
    std::vector<pixelflut_pt> map() const;

    void resize(int32_t new_width, int32_t new_height);
};