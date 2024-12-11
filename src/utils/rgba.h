#pragma once

#include <cstdint>

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
<<<<<<< HEAD
=======
    RGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b,  std::uint8_t a) : r(r), g(g), b(b), a(a) {};
    RGBA() {

    };
    RGBA(std::uint8_t r,
         std::uint8_t g,
         std::uint8_t b) : r(r), g(g), b(b) {
        a = 255;
    }
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
};
