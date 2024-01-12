#pragma once
#include "image.hpp"
#include "raytracer/grandma.hpp"
#include "types.hpp"
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

class pixelflut_client {
    SOCKET _socket;
    WSADATA _wsa_data;
    addrinfo* _addrinfo;

  public:
    ~pixelflut_client();
    pixelflut_client(const std::string& addr, const std::string& port,
                     const std::string& font = "roboto.ttf");

    std::string read_msg();
    void send_msg(const std::string& msg);

    vec2<uint32_t> read_size();

    void send_pixel(int32_t x, int32_t y, const vec3<uint8_t>& color);
    void send_pixel(int32_t x, int32_t y, const vec4<uint8_t>& color);
    void send_image(int32_t x_off, int32_t y_off, const image& img);

    void trace_rays(raytracer::grandma& granny);
};