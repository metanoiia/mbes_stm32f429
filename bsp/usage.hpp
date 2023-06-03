#pragma once

#include "pin.hpp"
#include "stm32_uart_dbg.hpp"
#include "gy30.hpp"
#include "noisesensor.hpp"

using UartSTLink = STM32::UART::UartDbg<STM32::UART::SampleUartDbgProps>;

using LED = Pin<'G', 13, 'H', PIN_SPEED_50MHZ>;
