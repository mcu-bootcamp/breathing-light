#include <cstdint>
#include <cmath>
#include <stm32f0xx_ll_system.h>
#include <stm32f0xx_ll_rcc.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_utils.h>

void delay_ms(unsigned int ms) {
    LL_mDelay(ms);
}

void light_enable() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
}

void light_turn_on() {
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
}

void light_turn_off() {
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
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

void boost()
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    LL_RCC_HSI48_Enable();
    while (LL_RCC_HSI48_IsReady() != 1)
        ;
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI48);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI48)
        ;

    SystemCoreClockUpdate();

    LL_Init1msTick(SystemCoreClock);
}

int main() {
    boost();

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
