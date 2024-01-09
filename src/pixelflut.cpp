#include "pixelflut.hpp"
#include "image.hpp"
#include <cctype>
#include <format>
#include <print>

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

vec2<int32_t> pixelflut_client::read_size() {
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

    return {std::stoi(std::string(buf.data() + 5, xlen - 1)),
            std::stoi(std::string(buf.data() + 5 + xlen, ylen - 1))};
}

void pixelflut_client::send_pixel(uint32_t x, uint32_t y,
                                  const vec3<uint8_t>& color) {
    send_msg(std::format("PX {} {} {:x}\n", x, y,
                         (color.x << 16) | (color.y << 8) | (color.z << 0)));
}

void pixelflut_client::send_image(uint32_t x_off, uint32_t y_off,
                                  const std::string& filepath) {
    image img(filepath);
    img.resize(512, 512);
    for (auto x = 0; x < img.width(); x++)
        for (auto y = 0; y < img.height(); y++)
            send_pixel(x + x_off, y + y_off, img.get(x, y));
}