#include "stm32f4xx_hal_tim.h"
#include "startup_stm32f429.h"

static TIM_HandleTypeDef htim3;
volatile uint32_t FreeRTOSRunTimeTicks;

/**
 * @brief 
 * called in vTaskStartScheduler 
 */
void configureTimerForRunTimeStates(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();

  constexpr uint32_t TIM3_PSC = 72,
                     TIM3_PER = 100;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = TIM3_PSC - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIM3_PER - 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.RepetitionCounter = 0;

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  NVIC_EnableIRQ(TIM3_IRQn);
  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_Base_Start_IT(&htim3); 
}

void sTim3::handler() 
{
	FreeRTOSRunTimeTicks++;
  HAL_TIM_IRQHandler(&htim3);
}