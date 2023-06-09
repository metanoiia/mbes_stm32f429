#include "freertos.hpp"

SemaphoreHandle_t flashSemaphore;
SemaphoreHandle_t initSemaphore;

int a = 0;
void StartNoiseSensorTask(void*)
{ 
  NoiseSensor::Init();
  bool has_noise = 0;
	for(;;)
	{
    has_noise = NoiseSensor::HasNoise();
    if(has_noise)
    {
      ControlUnit::SetNoise(has_noise);
      a++;
      while(NoiseSensor::HasNoise()){ vTaskDelay(10); };
    }
	}
}

void StartLuxSensorTask(void*)
{
  bool init_status = GY30::Init();
  static uint16_t lux = 0;

  initSemaphore = xSemaphoreCreateBinary();

  if(init_status)
    xSemaphoreGive(initSemaphore);

  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
    if(init_status)
    {
      vTaskDelayUntil( &xLastWakeTime, GY30::BH1750_MEASURE_TIME_MS);
      xLastWakeTime = xTaskGetTickCount();

      taskENTER_CRITICAL();
      lux = GY30::ReadLux(); 
      taskEXIT_CRITICAL(); 

      if(lux > ControlUnit::LUX_FLASH_THR && ControlUnit::ReadyToNew())
      {
        ControlUnit::SetLux(lux);
        xSemaphoreGive(flashSemaphore);
        while(GY30::ReadLux() > ControlUnit::LUX_FLASH_THR) { vTaskDelay(GY30::BH1750_MEASURE_TIME_MS); };
      }
    }
	}
}

void StartControlUnitTask(void*)
{
  ControlUnit::Init();
  
  uint32_t start_tick = xTaskGetTickCount(); //ms
  uint32_t ticks_passed = 0; //ms
  for(;;)
  {
    ticks_passed = xTaskGetTickCount() - start_tick;
    start_tick = xTaskGetTickCount();
    ControlUnit::UpdateTime(ticks_passed);
    
    taskENTER_CRITICAL();
    ControlUnit::Execute();
    taskEXIT_CRITICAL(); 
  }
}

void StartUartTask(void*)
{ 
  UartSTLink com_port;
  flashSemaphore = xSemaphoreCreateBinary();

  if(xSemaphoreTake(initSemaphore, GY30::BH1750_INIT_TIMOUT_MS) == pdTRUE)
    com_port <<"BH1750 initialized successfully :)\n\n";
  else 
    com_port <<"Errors occurred during BH1750 initialization :(\n\n";

  int dist = 0;
  for(;;)
  {
    dist = ControlUnit::GetDistance();

    if(dist != ControlUnit::NO_DIST)
    {
      if(dist > ControlUnit::MIN_DIST_THR)
      {
        com_port << "Lightning detected at a dictance " << dist << " meters;\n";
        com_port << "Waiting for next flash...\n\n";
      }
      else
        com_port << "Waiting for next flash...\n\n";
    }
      

      if(xSemaphoreTake(flashSemaphore, 10) == pdTRUE)
        com_port <<"Flash detected;\n";
  }
}

TaskHandle_t luxTask_Handler;
TaskHandle_t noiseTask_Handler;
TaskHandle_t controlUnitTask_Handler;
TaskHandle_t uartTask_Handler;


void tasksInit()
{
  xTaskCreate((TaskFunction_t) StartNoiseSensorTask, (const char*) "NoiseTask",
    (uint16_t) NOISE_TASK_STK_SIZE, (void*) NULL,
    (UBaseType_t) NOISE_TASK_PRIO, (TaskHandle_t*) &noiseTask_Handler);

  xTaskCreate((TaskFunction_t) StartLuxSensorTask, (const char*) "LuxTask",
    (uint16_t) LUX_TASK_STK_SIZE, (void*) NULL,
    (UBaseType_t) LUX_TASK_PRIO, (TaskHandle_t*) &luxTask_Handler);

  xTaskCreate((TaskFunction_t) StartControlUnitTask, (const char*) "ControlUnitTask",
    (uint16_t) CU_TASK_STK_SIZE, (void*) NULL,
    (UBaseType_t) CU_TASK_PRIO, (TaskHandle_t*) &controlUnitTask_Handler);

  xTaskCreate((TaskFunction_t) StartUartTask, (const char*) "UartTask",
    (uint16_t) UART_TASK_STK_SIZE, (void*) NULL,
    (UBaseType_t) UART_TASK_PRIO, (TaskHandle_t*) &uartTask_Handler);
}