#pragma once
#include "image.hpp"
#include "schrift.h"
#include "types.hpp"
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

class pixelflut_client {
    SOCKET _socket;
    WSADATA _wsa_data;
    addrinfo* _addrinfo;

    SFT_Font* _font;

  public:
    ~pixelflut_client();
    pixelflut_client(const std::string& addr, const std::string& port,
                     const std::string& font = "roboto.ttf");

    std::string read_msg();
    void send_msg(const std::string& msg);

    vec2<uint32_t> read_size();

    void send_pixel(uint32_t x, uint32_t y, const vec3<uint8_t>& color);
    void send_pixel(uint32_t x, uint32_t y, const vec4<uint8_t>& color);
    void send_image(uint32_t x_off, uint32_t y_off,
                    const std::string& filepath);
    void send_image(uint32_t x_off, uint32_t y_off, const image& img);

    void trace_rays();
};