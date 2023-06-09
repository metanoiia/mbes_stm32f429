#pragma once

#include "pin.hpp"
#include "stm32_uart_dbg.hpp"
#include "gy30.hpp"
#include "noisesensor.hpp"
#include "controlunit.hpp"

using UartSTLink = STM32::UART::UartDbg<STM32::UART::SampleUartDbgProps>;

using LED = Pin<'G', 13, 'H', PIN_SPEED_50MHZ>;

using I2C = STM32::I2C::SimpleI2c<STM32::I2C::I2C_1>;
using GY30 = GY30_Driver<I2C>;

using noiseDataPin = Pin<'B', 8, 'H', PIN_SPEED_50MHZ>;
using NoiseSensor = NoiseSensor_Driver<noiseDataPin>;
using ControlUnit = ControlUnit_t;
