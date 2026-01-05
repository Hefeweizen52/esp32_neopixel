#ifndef ESP32_RMT_HPP
#define ESP32_RMT_HPP

#include <Arduino.h>
#include "driver/rmt.h"
#include <vector>
#include "color_order.hpp"
#include "led_types.hpp"

#define CLK_DIV 2

template <typename led_type, typename color_order>
class esp32_rmt
{
public:
    esp32_rmt() = delete;

    esp32_rmt(gpio_num_t pin, rmt_channel_t channel, size_t num_pixels) : _pin(pin), _channel(channel), _num_pixels(num_pixels)
    {
        _pixel_buf.resize(_num_pixels * color_order::num_channels);
        _rmt_buf.resize((color_order::num_channels * 8 * _num_pixels) + 1);
    };

    void begin()
    {
#if CONFIG_IDF_TARGET_ESP32C3
        if (_channel < RMT_CHANNEL_0 || _channel > RMT_CHANNEL_1)
            throw "ESP32-C3 only supports RMT_CHANNEL_0 and RMT_CHANNEL_1 as transmitter";
#endif

        // #if CONFIG_IDF_TARGET_ESP32S3
        //     if (_channel < RMT_CHANNEL_0 || _channel > RMT_CHANNEL_1)
        //         throw "ESP32-C3 only supports RMT_CHANNEL_0 and RMT_CHANNEL_1 as transmitter";
        // #endif

        rmt_config_t config;
        config.rmt_mode = RMT_MODE_TX;
        config.channel = _channel;
        config.gpio_num = _pin;
        config.mem_block_num = 1;
        config.tx_config.loop_en = false;
        config.tx_config.carrier_en = false;
        config.tx_config.idle_output_en = true;
        config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
        config.clk_div = CLK_DIV; // 80MHz/2 = 40MHz → 25ns pro Tick

        rmt_config(&config);
        rmt_driver_install(config.channel, 0, 0);

        uint32_t t0h_ticks = ns_to_ticks(led_type::T0H, config.clk_div);
        uint32_t t0l_ticks = ns_to_ticks(led_type::T0L, config.clk_div);
        uint32_t t1h_ticks = ns_to_ticks(led_type::T1H, config.clk_div);
        uint32_t t1l_ticks = ns_to_ticks(led_type::T1L, config.clk_div);

        _bit0.level0 = 1;
        _bit0.duration0 = t0h_ticks;
        _bit0.level1 = 0;
        _bit0.duration1 = t0l_ticks;

        _bit1.level0 = 1;
        _bit1.duration0 = t1h_ticks;
        _bit1.level1 = 0;
        _bit1.duration1 = t1l_ticks;
    }

    void set_pixel(size_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0)
    {
        color_order::encode(r, g, b, w, &_pixel_buf[idx * color_order::num_channels]);
    }

    void show()
    {
        size_t buf_idx = 0; // Index im rmt_data_buf

        for (uint8_t byte : _pixel_buf)
        {
            for (int bit = 7; bit >= 0; --bit)
            {
                bool b = byte & (1 << bit);

                rmt_item32_t &item = _rmt_buf[buf_idx++];

                item = b ? _bit1 : _bit0;
            }
        }

        rmt_item32_t &reset_item = _rmt_buf.at(buf_idx++);
        reset_item.level0 = 0;
        reset_item.duration0 = ns_to_ticks(led_type::RESET);
        reset_item.level1 = 0;
        reset_item.duration1 = 0;

        // Senden
        // rmt_write_items(_channel, (const rmt_item32_t *)_rmt_buf.data(), buf_idx, true);
        // rmt_wait_tx_done(_channel, portMAX_DELAY);

        rmt_write_items(_channel, (const rmt_item32_t *)_rmt_buf.data(), buf_idx, false);
    }

    static inline uint32_t ns_to_ticks(uint32_t ns, double clk_div = CLK_DIV)
    {
        // RMT läuft mit 80 MHz Basis (12.5 ns pro Tick)
        double tick_ns = (1e9 / 80000000.0) * clk_div;
        return (uint32_t)(ns / tick_ns);
    }

private:
    gpio_num_t _pin;
    rmt_channel_t _channel;
    size_t _num_pixels;
    std::vector<uint8_t> _pixel_buf;
    std::vector<rmt_item32_t> _rmt_buf;

    rmt_item32_t _bit0, _bit1;
};

#endif // ESP32_RMT_HPP