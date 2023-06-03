#include "freertos.hpp"

size_t count = 0;
bool init_status = false;

void StartNoiseSensorTask(void*)
{ 
  NoiseSensor::Init();
	for(;;)
	{
    if(NoiseSensor::HasNoise())
      count++;
	}
}

void StartLuxSensorTask(void*)
{
  taskENTER_CRITICAL();
  UartSTLink com_port;

  init_status = GY30::Init();
  static uint16_t lux = 0;
  static uint16_t lux_new_value = 0;

  taskEXIT_CRITICAL();

  if(init_status)
    com_port << "BH1750 successfully initialized!\n";
  else
    com_port << "Error detected during BH1750 init!\n";

  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
    if(init_status)
    {
      vTaskDelayUntil( &xLastWakeTime, GY30::BH1750_MEASURE_TIME_MS);
      xLastWakeTime = xTaskGetTickCount();

      lux_new_value = GY30::ReadLux();
      if(lux != lux_new_value)
      { 
        lux = lux_new_value;
        com_port << lux <<" lx\n";
      }
    }
	}
}

TaskHandle_t luxTask_Handler;
TaskHandle_t noiseTask_Handler;

void tasksInit()
{
    xTaskCreate((TaskFunction_t) StartNoiseSensorTask, (const char*) "NoiseTask",
			(uint16_t) NOISE_TASK_STK_SIZE, (void*) NULL,
			(UBaseType_t) NOISE_TASK_PRIO, (TaskHandle_t*) &noiseTask_Handler);

    xTaskCreate((TaskFunction_t) StartLuxSensorTask, (const char*) "LuxTask",
			(uint16_t) LUX_TASK_STK_SIZE, (void*) NULL,
			(UBaseType_t) LUX_TASK_PRIO, (TaskHandle_t*) &luxTask_Handler);
}