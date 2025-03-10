#include "breathing.hpp"

#include <stm32f0xx_ll_system.h>
#include <stm32f0xx_ll_rcc.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_utils.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_tim.h>

void light_enable() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_9, LL_GPIO_AF_0);

    using freq::literals::operator"" _mhz;
    constexpr auto HCLOCK = 48_mhz;
    constexpr auto TIM_CLOCK = 1_mhz;
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_TIM_SetPrescaler(TIM3, HCLOCK / TIM_CLOCK - 1);
    LL_TIM_SetAutoReload(TIM3, TIM_CLOCK / breathing::refresh_rate - 1);
    LL_TIM_OC_SetCompareCH4(TIM3, 0);
    LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    LL_TIM_EnableCounter(TIM3);
    LL_TIM_EnableIT_UPDATE(TIM3);
    NVIC_EnableIRQ(TIM3_IRQn);
}

extern "C" void TIM3_IRQHandler() {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
        LL_TIM_ClearFlag_UPDATE(TIM3);
        static uint32_t counter = 0;
        uint32_t on_cycles = breathing::brightnesses[counter] * (LL_TIM_GetAutoReload(TIM3));
        LL_TIM_OC_SetCompareCH4(TIM3, on_cycles);
        counter = (counter + 1) % breathing::brightnesses.size();
    }
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
}

int main() {
    boost();

    light_enable();

    while (true)
        ;
    return 0;
}
