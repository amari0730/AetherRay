#pragma once

#include <cstdint>

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
    RGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b,  std::uint8_t a) : r(r), g(g), b(b), a(a) {};
    RGBA() {

    };
    RGBA(std::uint8_t r,
         std::uint8_t g,
         std::uint8_t b) : r(r), g(g), b(b) {
        a = 255;
    }
};
