#include "pixelflut.hpp"
#include "image.hpp"
#include <algorithm>
#include <cctype>
#include <execution>
#include <format>
#include <print>
#include <random>
#include <ranges>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

pixelflut_client::pixelflut_client(const std::string& addr,
                                   const std::string& port) {
    auto res = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
    if (res)
        throw std::runtime_error(std::format("WSAStartup() failed: {}", res));

    addrinfo hint = {
        .ai_family   = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
    };

    res = getaddrinfo(addr.c_str(), port.c_str(), &hint, &_addrinfo);
    if (res)
        throw std::runtime_error(std::format("getaddrinfo() failed: {}", res));

    _socket = socket(_addrinfo->ai_family, _addrinfo->ai_socktype,
                     _addrinfo->ai_protocol);
    if (_socket == INVALID_SOCKET)
        throw std::runtime_error(std::format("socket() failed: {}", res));

    res = connect(_socket, _addrinfo->ai_addr, _addrinfo->ai_addrlen);
    if (res)
        throw std::runtime_error(std::format("connect() failed: {}", res));
}

pixelflut_client::~pixelflut_client() {
    shutdown(_socket, SD_SEND);
    closesocket(_socket);
    WSACleanup();
}

void pixelflut_client::send_msg(const std::string& msg) {
    auto res = send(_socket, msg.c_str(), msg.size(), 0);
    if (res == SOCKET_ERROR)
        throw std::runtime_error(std::format("send() failed: {}", res));
}

std::string pixelflut_client::read_msg() {
    std::string buf;
    buf.resize(1 << 16);
    auto res = recv(_socket, buf.data(), buf.size(), 0);
    if (res == SOCKET_ERROR)
        throw std::runtime_error(std::format("recv() failed: {}", res));
    buf.resize(res);
    return buf;
}

vec2<uint32_t> pixelflut_client::read_size() {
    send_msg("SIZE\n");
    auto buf = read_msg();

    auto xlen = 0;
    while (std::isdigit(*(buf.data() + 5 + xlen++)))
        ;

    auto ylen = 0;
    while (std::isdigit(*(buf.data() + 5 + xlen + ylen++)))
        ;

    auto str = std::string(buf.data() + 5, xlen - 1);
    str      = std::string(buf.data() + 5 + xlen, ylen - 1);

    return {
        static_cast<uint32_t>(std::stoi(std::string(buf.data() + 5, xlen - 1))),
        static_cast<uint32_t>(
            std::stoi(std::string(buf.data() + 5 + xlen, ylen - 1))),
    };
}

void pixelflut_client::send_pixel(int32_t x, int32_t y,
                                  const vec3<uint8_t>& color) {
    send_msg(std::format("PX {} {} {:x}\n", x, y,
                         (color.x << 16) | (color.y << 8) | (color.z << 0)));
}

void pixelflut_client::send_pixel(int32_t x, int32_t y,
                                  const vec4<uint8_t>& color) {
    send_msg(std::format("PX {} {} {:x}\n", x, y,
                         (color.x << 24) | (color.y << 16) | (color.z << 8) |
                             (color.w << 0)));
}

void pixelflut_client::send_image(int32_t x_off, int32_t y_off,
                                  const image& img) {
    std::ranges::iota_view x_iota(0, img.width());
    std::ranges::iota_view y_iota(0, img.height());

    std::for_each(
        std::execution::par_unseq, x_iota.begin(), x_iota.end(), [&](auto x) {
            std::for_each(std::execution::par_unseq, y_iota.begin(),
                          y_iota.end(), [&](auto y) {
                              send_pixel(x + x_off, y + y_off, img.get(x, y));
                          });
        });
}

void pixelflut_client::trace_rays(raytracer::grandma& granny) {
    std::ranges::iota_view x_iota(0u, granny.width());
    std::ranges::iota_view y_iota(0u, granny.height());

    std::for_each(
        std::execution::par_unseq, x_iota.begin(), x_iota.end(), [&](auto x) {
            std::for_each(
                std::execution::par_unseq, y_iota.begin(), y_iota.end(),
                [&](auto y) { send_pixel(x, y, granny.compute_pixel(x, y)); });
        });
}