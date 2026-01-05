#ifndef COLOR_ORDER_HPP
#define COLOR_ORDER_HPP

struct GRB
{
    static constexpr int num_channels = 3;
    static inline void encode(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t *out)
    {
        out[0] = g;
        out[1] = r;
        out[2] = b;
    }
};

struct WRGB
{
    static constexpr int num_channels = 4;
    static inline void encode(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t *out)
    {
        out[0] = w;
        out[1] = r;
        out[2] = g;
        out[3] = b;
    }
};

struct RGBW
{
    static constexpr int num_channels = 4;
    static inline void encode(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t *out)
    {
        out[0] = r;
        out[1] = g;
        out[2] = b;
        out[3] = w;
    }
};


#endif // COLOR_ORDER_HPP