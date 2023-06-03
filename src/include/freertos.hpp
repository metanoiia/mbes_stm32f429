#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "usage.hpp"

#define LUX_TASK_PRIO       4 
#define LUX_TASK_STK_SIZE   4 * configMINIMAL_STACK_SIZE

#define NOISE_TASK_PRIO     4
#define NOISE_TASK_STK_SIZE 4 * configMINIMAL_STACK_SIZE

void tasksInit();
