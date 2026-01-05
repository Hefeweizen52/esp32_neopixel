#ifndef COLOR_UTILS_HPP
#define COLOR_UTILS_HPP

#include <algorithm>
#include <math.h>

struct RGB;
struct HSL;

static inline RGB hslToRgb(float h, float s, float l);
static inline HSL rgbToHsl(uint8_t r, uint8_t g, uint8_t b);

struct HSL {
    HSL() = default;

    float h = 0.0, s = 0.0, l = 0.0;

    HSL(float h, float s, float l) : h(h), s(s), l(l) {}

    RGB toRGB();
};

struct RGB {
    RGB() = default;

    uint8_t r = 0, g = 0, b = 0;

    RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    HSL toHSL(); 
};

inline RGB HSL::toRGB() {
    return hslToRgb(h, s, l);
}

inline HSL RGB::toHSL() {
    return rgbToHsl(r, g, b);
}

static inline HSL rgbToHsl(uint8_t r, uint8_t g, uint8_t b)
{
    // Normalisierung der Werte auf [0, 1]
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    float maxVal = std::max({rf, gf, bf});
    float minVal = std::min({rf, gf, bf});
    float delta = maxVal - minVal;

    HSL res;
    res.l = (maxVal + minVal) / 2.0f;

    if (delta == 0)
    {
        res.h = 0;
        res.s = 0;
    }
    else
    {
        res.s = (res.l <= 0.5f) ? (delta / (maxVal + minVal)) : (delta / (2.0f - maxVal - minVal));

        if (maxVal == rf)
        {
            res.h = (gf - bf) / delta + (gf < bf ? 6 : 0);
        }
        else if (maxVal == gf)
        {
            res.h = (bf - rf) / delta + 2;
        }
        else
        {
            res.h = (rf - gf) / delta + 4;
        }
        res.h /= 6.0f;
    }

    res.h *= 360.0f; // Umwandlung in Grad
    return res;
}

static inline float hueToRgb(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;
    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f)
        return q;
    if (t < 2.0f / 3.0f)
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

/*
static inline RGB hslToRgb(float h, float s, float l)
{
    // Normalisierung: h auf [0,1], s und l sind bereits [0,1]
    h /= 360.0f;

    float r, g, b;

    if (s == 0.0f)
    {
        r = g = b = l; // Graustufe
    }
    else
    {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hueToRgb(p, q, h + 1.0f / 3.0f);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - 1.0f / 3.0f);
    }

    return {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)};
} */

static inline RGB hslToRgb(float h, float s, float l)
{
    // s /= 100.0f; 
    // l /= 100.0f;

    // Clamp-Schutz: Sicherstellen, dass S/L im Bereich [0,1] bleiben
    s = std::max(0.0f, std::min(1.0f, s));
    l = std::max(0.0f, std::min(1.0f, l));

    // Hue auf 0..1 normalisieren (und zyklisch halten, falls h > 360)
    h = fmodf(h, 360.0f);
    if (h < 0) h += 360.0f;
    h /= 360.0f;

    float r, g, b;

    if (s == 0.0f) {
        r = g = b = l; // Achromatisch (Grau)
    } else {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hueToRgb(p, q, h + 1.0f / 3.0f);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - 1.0f / 3.0f);
    }

    // +0.5f sorgt für korrektes Runden statt Abschneiden
    return {
        (uint8_t)(std::max(0.0f, std::min(255.0f, r * 255.0f + 0.5f))),
        (uint8_t)(std::max(0.0f, std::min(255.0f, g * 255.0f + 0.5f))),
        (uint8_t)(std::max(0.0f, std::min(255.0f, b * 255.0f + 0.5f)))
    };
}

#endif // COLOR_UTILS_HPP
