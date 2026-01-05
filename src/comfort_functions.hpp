#ifndef COMFORT_FUNCTIONS_HPP
#define COMFORT_FUNCTIONS_HPP

#include <algorithm>
#include "color_utils.hpp"

class comfort_functions
{
public:
    virtual ~comfort_functions() = default;

    virtual void begin();

    virtual void set_pixel(size_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);

    virtual void show();

    inline void clear(size_t count)
    {
        fill(0, count, 0, 0, 0);
    }

    inline void set_pixel(size_t idx, RGB rgb)
    {
        set_pixel(idx, rgb.r, rgb.g, rgb.b, 0);
    }

    inline void set_pixel(size_t idx, HSL hsl)
    {
        set_pixel(idx, hsl.toRGB());
    }

    inline void fill(size_t idx, size_t count, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0)
    {
        for (size_t i = idx; i < idx + count; ++i)
            set_pixel(i, r, g, b, w);
    }

    inline void fill(size_t idx, size_t count, RGB rgb)
    {
        fill(idx, count, rgb.r, rgb.g, rgb.b, 0);
    }

    inline void fill(size_t idx, size_t count, HSL hsl)
    {
        fill(idx, count, hsl.toRGB());
    }

    inline void set_brightness(uint8_t b)
    {
        _brightness = b;
    }

    inline uint8_t brightness()
    {
        return _brightness;
    }

protected:
    uint8_t _brightness = 255;
};

#endif // COMFORT_FUNCTIONS_HPP
