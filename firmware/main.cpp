#include "breathing.hpp"

#include <stm32f0xx_ll_system.h>
#include <stm32f0xx_ll_rcc.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_utils.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_tim.h>
#include <stm32f0xx_ll_dma.h>

void light_enable() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_9, LL_GPIO_AF_0);

    using freq::literals::operator"" _mhz;
    constexpr auto HCLOCK = 48_mhz;
    constexpr auto TIM_CLOCK = 1_mhz;
    constexpr auto TIM_CYCLE = TIM_CLOCK / breathing::refresh_rate - 1;
    constexpr auto duty_cycles = [TIM_CYCLE]() {
        std::array<uint32_t, breathing::brightnesses.size()> duty_cycles;
        std::ranges::transform(breathing::brightnesses, duty_cycles.begin(),
            [=](auto brightness) { return static_cast<uint32_t>(brightness * TIM_CYCLE); });
        return duty_cycles;
    }();

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t) &(TIM3->CCR4));
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t) &duty_cycles[0]);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, duty_cycles.size());
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_WORD);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_WORD);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_CIRCULAR);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_TIM_SetPrescaler(TIM3, HCLOCK / TIM_CLOCK - 1);
    LL_TIM_SetAutoReload(TIM3, TIM_CYCLE);
    LL_TIM_OC_SetCompareCH4(TIM3, 0);
    LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    LL_TIM_EnableCounter(TIM3);
    LL_TIM_EnableDMAReq_UPDATE(TIM3);
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
