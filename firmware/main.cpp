#include <cstdint>
#include <cmath>

void delay(volatile unsigned int t) {
    while (t-- > 0);
}

void delay_ms(unsigned int ms) {
    delay(671u * ms);
}

constexpr uintptr_t GPIOC = 0x4800'0800;

void light_enable() {
    constexpr uintptr_t RCC = 0x4002'1000;
    *(uint32_t *) (RCC + 0x14) |= 1 << 19;

    *(uint32_t *) (GPIOC) |= 1 << 18;
    *(uint32_t *) (GPIOC) &= ~(1 << 19);
}

void light_turn_on() {
    *(uint32_t *) (GPIOC + 0x14) |= (1 << 9);
}

void light_turn_off() {
    *(uint32_t *) (GPIOC + 0x14) &= ~(1 << 9);
}

void light_set_brightness(float brightness, uint32_t period) {
    uint32_t on_time = brightness * period;
    if (on_time > 0) {
        light_turn_on();
        delay_ms(on_time);
    }
    uint32_t off_time = period - on_time;
    if (off_time > 0) {
        light_turn_off();
        delay_ms(off_time);
    }
}

float breathing_brightness(uint32_t t, uint32_t respiratory_period) {
    constexpr auto PI = 3.1415926f;
    auto radian = t * 2 * PI / respiratory_period - PI / 2;
    return (std::sin(radian) + 1) / 2;
}

int main() {
    light_enable();

    constexpr uint32_t refresh_rate = 50;
    constexpr auto refresh_period = 1000u / refresh_rate;
    constexpr auto respiratory_rate = 12u;
    constexpr auto respiratory_period = 60'000u / respiratory_rate;
    for (uint32_t t = 0; true; t = (t + refresh_period) % respiratory_period) {
        auto brightness = breathing_brightness(t, respiratory_period);
        light_set_brightness(brightness, refresh_period);
    }
    return 0;
}