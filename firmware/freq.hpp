#pragma once

#include <chrono>

namespace freq {
    template <typename Rep, typename Freq = std::ratio<1>>
    struct freq_t {
        Rep value;

        constexpr freq_t(Rep value) : value(value) {}
        constexpr Rep count() const { return value; }

        template <typename T>
        constexpr auto period() const {
            if constexpr (Freq::num >= Freq::den) {
                return std::chrono::duration_cast<T>(std::chrono::seconds{1}) / (count() * Freq::num / Freq::den);
            }
            else {
                return std::chrono::duration_cast<T>(std::chrono::seconds{1} * Freq::den / Freq::num / count());
            }
        }
    };

    using hz = freq_t<std::size_t>;
    using khz = freq_t<std::size_t, std::kilo>;
    using mhz = freq_t<std::size_t, std::mega>;
    using ghz = freq_t<std::size_t, std::giga>;
    using per_minutes = std::ratio<1, 60>;
    using rpm = freq_t<std::size_t, per_minutes>;

    template <typename Rep, typename Freq>
    struct period_type;
    template <typename Rep>
    struct period_type<Rep, per_minutes> { using type = std::chrono::duration<Rep, std::milli>; };
    template <typename Rep>
    struct period_type<Rep, std::ratio<1>> { using type = std::chrono::duration<Rep, std::milli>; };
    template <typename Rep>
    struct period_type<Rep, std::kilo> { using type = std::chrono::duration<Rep, std::micro>; };
    template <typename Rep>
    struct period_type<Rep, std::mega> { using type = std::chrono::duration<Rep, std::nano>; };
    template <typename Rep>
    struct period_type<Rep, std::giga> { using type = std::chrono::duration<Rep, std::pico>; };
    template <typename Rep, typename Freq>
    using period_t = typename period_type<Rep, Freq>::type;

    template <typename Rep, typename Freq>
    constexpr Rep operator/(const freq_t<Rep, Freq>& lhs, const freq_t<Rep, Freq>& rhs) {
        return lhs.count() / rhs.count();
    }

    template <typename Freq1, typename Freq2>
    struct ratio_greater : std::bool_constant<((Freq1::num * Freq2::den) > (Freq2::num * Freq1::den))> {};

    template <typename Rep1, typename Freq1, typename Rep2, typename Freq2>
        requires ratio_greater<Freq1, Freq2>::value
    constexpr std::common_type_t<Rep1, Rep2> operator/(const freq_t<Rep1, Freq1>& lhs, const freq_t<Rep2, Freq2>& rhs) {
        return lhs.count() * Freq1::num * Freq2::den / rhs.count() / Freq2::num / Freq1::den;
    }

    namespace literals {
        constexpr rpm operator"" _rpm(unsigned long long value) { return {static_cast<std::size_t>(value)}; }
        constexpr hz operator"" _hz(unsigned long long value) { return {static_cast<std::size_t>(value)}; }
        constexpr khz operator"" _khz(unsigned long long value) { return {static_cast<std::size_t>(value)}; }
        constexpr mhz operator"" _mhz(unsigned long long value) { return {static_cast<std::size_t>(value)}; }
        constexpr ghz operator"" _ghz(unsigned long long value) { return {static_cast<std::size_t>(value)}; }
    }

    namespace _testing {
        using namespace literals;
        using namespace std::chrono;
        using picoseconds = duration<int64_t, std::pico>;
        static_assert(1_hz .period<seconds>().count() == 1);
        static_assert(1_khz .period<milliseconds>().count() == 1);
        static_assert(1_mhz .period<microseconds>().count() == 1);
        static_assert(1_ghz .period<nanoseconds>().count() == 1);
        static_assert(10_khz .period<microseconds>().count() == 100);
        static_assert(10_mhz .period<nanoseconds>().count() == 100);
        static_assert(10_ghz .period<picoseconds>().count() == 100);
        static_assert(20_khz .period<microseconds>().count() == 50);
        static_assert(20_mhz .period<nanoseconds>().count() == 50);
        static_assert(20_ghz .period<picoseconds>().count() == 50);
        static_assert(48_khz .period<microseconds>().count() == 20);
        static_assert(48_mhz .period<nanoseconds>().count() == 20);
        static_assert(48_ghz .period<picoseconds>().count() == 20);
        static_assert(1_rpm .period<minutes>().count() == 1);
        static_assert(60_rpm .period<seconds>().count() == 1);
        static_assert(12_rpm .period<milliseconds>().count() == 60'000u / 12);
        static_assert(8_khz / 8_khz == 1);
        static_assert(48_khz / 1_khz == 48);
        static_assert(50_hz / 10_hz == 5);
        static_assert(50_hz / 12_rpm == 250);
        static_assert(7_khz / 8_khz == 0);
        static_assert(48_khz / 1_hz == 48'000);
    }
} // namespace freq