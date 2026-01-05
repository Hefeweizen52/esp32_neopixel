#ifndef LED_TYPES_HPP
#define LED_TYPES_HPP

#include <inttypes.h>

struct WS2812
{
    static constexpr uint32_t T0H = 350;
    static constexpr uint32_t T0L = 800;
    static constexpr uint32_t T1H = 700;
    static constexpr uint32_t T1L = 600;
    static constexpr uint32_t RESET = 50000;
};

struct WS2811
{
    static constexpr uint32_t T0H = 500;
    static constexpr uint32_t T0L = 2000;
    static constexpr uint32_t T1H = 1200;
    static constexpr uint32_t T1L = 1300;
    static constexpr uint32_t RESET = 50000;
};

struct WS2814
{
    static constexpr uint32_t T0H = 300;
    static constexpr uint32_t T0L = 600;
    static constexpr uint32_t T1H = 600;
    static constexpr uint32_t T1L = 600;
    static constexpr uint32_t RESET = 280000;
};

struct SK6812
{
    static constexpr uint32_t T0H = 300;
    static constexpr uint32_t T0L = 900;
    static constexpr uint32_t T1H = 600;
    static constexpr uint32_t T1L = 600;
    static constexpr uint32_t RESET = 80000;
};
#endif // LED_TYPES_HPP