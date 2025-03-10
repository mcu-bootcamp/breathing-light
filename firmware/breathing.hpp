#pragma once

#include "freq.hpp"

#include <cmath>
#include <cstdint>
#include <array>
#include <algorithm>
#include <ranges>

namespace breathing {
    using namespace freq::literals;
    static constexpr auto refresh_rate = 50_hz;
    static constexpr auto respiratory_rate = 12_rpm;
    static constexpr auto refresh_period = refresh_rate.period<std::chrono::milliseconds>();
    static constexpr auto respiratory_period = respiratory_rate.period<std::chrono::milliseconds>();
    static constexpr auto samples_count = respiratory_period / refresh_period;
    static constexpr float cal_brightness(uint32_t t, uint32_t period) {
        constexpr auto PI = 3.1415926f;
        auto radian = t * 2 * PI / period - PI / 2;
        return (std::sin(radian) + 1) / 2;
    }
    static constexpr auto brightnesses = []() constexpr {
        std::array<float, samples_count> brightnesses;
        std::ranges::generate(brightnesses, [t = 0]() mutable
                              { return cal_brightness(t += refresh_period.count(), respiratory_period.count()); });
        return brightnesses;
    }();
};