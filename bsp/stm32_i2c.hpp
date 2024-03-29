/**
 *  stm32tpl --  STM32 C++ Template Peripheral Library
 *  Visit https://github.com/antongus/stm32tpl for new versions
 *
 *  Copyright (c) 2011-2022 Anton B. Gusev
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  file         : stm32_i2c.h
 *  description  : Simple i2c implementation (no interrupts, no DMA)
 *
 */

#pragma once

#include "stm32.hpp"
#include "pin.hpp"

namespace STM32
{
namespace I2C
{

/**
 * Enumeration for all I2C devices in system
 */
enum I2cNum
{
#if (defined RCC_APB1ENR_I2C1EN)
	I2C_1
#endif
#if (defined RCC_APB1ENR_I2C2EN)
	,I2C_2
#endif
#if (defined RCC_APB1ENR_I2C3EN)
	,I2C_3
#endif
};

namespace detail
{

template<I2cNum num> struct I2cTraits;

#if defined (RCC_APB1ENR_I2C1EN)
template<> struct I2cTraits<I2C_1>
{
	using SCL = Pin<'B', 6>;
	using SDA = Pin<'B', 7>;
	static constexpr uint32_t I2Cx_BASE {I2C1_BASE};

	static void enableClocks()  { RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; __DSB(); }
	static void disableClocks() { RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN; }
	static void reset()
	{
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
		__DSB();
	}
#if (defined STM32TPL_F2xxF4xx) || (defined STM32TPL_STM32L1XX) || (defined STM32TPL_STM32F7XX)
	static constexpr auto ALT_FUNC_I2Cx = ALT_FUNC_I2C1;
#endif
};
#endif

#if defined (RCC_APB1ENR_I2C2EN)
template<> struct I2cTraits<I2C_2>
{
	using SCL = Pin<'B', 10>;
	using SDA = Pin<'B', 11>;
	static constexpr uint32_t I2Cx_BASE {I2C2_BASE};

	static void enableClocks()  { RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; __DSB(); }
	static void disableClocks() { RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN; }
	static void reset()
	{
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
		__DSB();
	}
#if (defined STM32TPL_F2xxF4xx) || (defined STM32TPL_STM32L1XX) || (defined STM32TPL_STM32F7XX)
	static constexpr auto ALT_FUNC_I2Cx = ALT_FUNC_I2C2;
#endif
};
#endif

#if defined (RCC_APB1ENR_I2C3EN)
template<> struct I2cTraits<I2C_3>
{
	using SCL = Pin<'H', 7>;
	using SDA = Pin<'H', 8>;
	static constexpr uint32_t I2Cx_BASE {I2C3_BASE};

	static void enableClocks()  { RCC->APB1ENR |= RCC_APB1ENR_I2C3EN; __DSB(); }
	static void disableClocks() { RCC->APB1ENR &= ~RCC_APB1ENR_I2C3EN; }
	static void reset()
	{
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C3RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C3RST;
		__DSB();
	}
#if (defined STM32TPL_F2xxF4xx) || (defined STM32TPL_STM32L1XX) || (defined STM32TPL_STM32F7XX)
	static constexpr auto ALT_FUNC_I2Cx = ALT_FUNC_I2C3;
#endif
};
#endif

} // namespace detail


template <I2cNum num,
	typename PinSCL = typename detail::I2cTraits<num>::SCL,   // allow arbitrary pins for SCL/SDA
	typename PinSDA = typename detail::I2cTraits<num>::SDA
	>
class SimpleI2c
{
public:
	using Traits = detail::I2cTraits<num>;
	using SCL = PinSCL;
	using SDA = PinSDA;

	static IOStruct<Traits::I2Cx_BASE, I2C_TypeDef> I2Cx;

	static void Init()
	{
		Traits::enableClocks();
		InitPins();

	    I2Cx->CR1 = 0;
	    I2Cx->CR2 = 0x24;  // 0b100100 = 36 MHz
	    I2Cx->CCR = 180;//180;   // 100 KHz
	    I2Cx->CR1 = I2C_CR1_PE;
	}

	static void InitPins()
	{
#if (!defined STM32TPL_STM32F1XX)
		SCL::Alternate(Traits::ALT_FUNC_I2Cx);
		SDA::Alternate(Traits::ALT_FUNC_I2Cx);
#endif
	    SCL::Mode(ALT_OUTPUT_OD);
	    SDA::Mode(ALT_OUTPUT_OD);
	}

	static void DeinitPins()
	{
#if (!defined STM32TPL_STM32F1XX)
		SCL::Alternate((PinAltFunction)0);
		SDA::Alternate((PinAltFunction)0);
#endif
		SCL::Mode(OUTPUT_OD);
		SDA::Mode(OUTPUT_OD);
	}

	static void Reset()
	{
		// turn off i2c peripheral
		Traits::disableClocks();

		// reset i2c peripheral
		Traits::reset();

	    // configure pins as outputs (disconnect it from i2c peripheral)
		DeinitPins();
		SCL::On();
		SDA::On();

		// feed clock to slave until it releases SDA
		for (int i = 0; i < 20; ++i)  // 9 should be enough
		{
			Hdel();
			if (SDA::Signalled())
				break;
			SCL::Off();
			Hdel();
			SCL::On();
		}
		softStop();
		InitPins();
	}

	static bool Read(uint8_t device, uint8_t addr, void *data, size_t count)
	{
		uint8_t *p = reinterpret_cast<uint8_t *>(data);
		bool ret = start() && sendAddress(device) && sendData(addr);

		ret = ret && start() && sendAddress(device | 0x01);

		while (ret && ((count--) > 1))
		{
			ret = ret && readData(p++);
		}

		if (ret)
		{
		    I2Cx->CR1 &= ~I2C_CR1_ACK;
		    stop();
		    ret = ret && readData(p) && waitBusy();
		    I2Cx->CR1 |= I2C_CR1_ACK;
		}
		return ret;
	}

	static bool Read(uint8_t device, void *data, size_t count)
	{
		uint8_t *p = reinterpret_cast<uint8_t *>(data);

		bool ret = start() && sendAddress(device | 0x01);

		while (ret && ((count--) > 1))
		{
			ret = ret && readData(p++);
		}

		if (ret)
		{
		    I2Cx->CR1 &= ~I2C_CR1_ACK;
		    stop();
		    ret = ret && readData(p) && waitBusy();
		    I2Cx->CR1 |= I2C_CR1_ACK;
		}
		return ret;
	}

	static bool Write(uint8_t device, uint8_t addr, void *data, size_t count)
	{
		uint8_t *p = reinterpret_cast<uint8_t *>(data);
		bool ret = start() && sendAddress(device) && sendData(addr);

		while (ret && count--)
		{
			ret = ret && sendData(*p++);
		}
	    stop();
		return ret;
	}

	static bool Write(uint8_t device, uint8_t data)
	{
		bool ret = start() && sendAddress(device);

		if(ret)
			ret &= sendData(data);
		stop();

		return ret;
	}

	static size_t Scan()
	{
		size_t count = 0;
		for(size_t i = 0; i < 127; i++)
		{	
			start();
			if(sendAddress(i))
				count++;
			stop();
		}
		return count;
	}

protected:
//public:
	static bool waitSR1(uint16_t flag)
	{
		for (uint32_t timeout = 500000; timeout; --timeout) //500000
			if (I2Cx->SR1 & flag)
				return true;
		return false;
	}

	static bool waitBusy()
	{
		for (uint32_t timeout = 500000; timeout; --timeout)
			if (!(I2Cx->SR2 & I2C_SR2_BUSY))
				return true;
		return false;
	}

	static bool start()
	{
		I2Cx->CR1 |= I2C_CR1_START;
		return waitSR1(I2C_SR1_SB);
	}

	static void stop()
	{
	    I2Cx->CR1 |= I2C_CR1_STOP;
	}

	static bool sendAddress(uint8_t addr)
	{
	    I2Cx->DR = addr;
	    bool ret = waitSR1(I2C_SR1_ADDR);
	    I2Cx->SR2;  // clear ADDR bit by reading SR2
	    return ret;
	}

	static bool sendData(uint8_t data)
	{
	    I2Cx->DR = data;
	    return waitSR1(I2C_SR1_BTF);
	}

	static bool readData(uint8_t* b)
	{
		if (waitSR1(I2C_SR1_RXNE))
		{
			*b = I2Cx->DR;
			return true;
		}
	    return false;
	}

	static constexpr auto NOP_COUNT = 100;
	static void qdel() __attribute__((noinline))
	{
		for (auto i = 0; i < NOP_COUNT; i++)
			__asm__ __volatile__ ("nop");
	}
	static void Hdel()
	{
		qdel();
		qdel();
	}
	static void softStop()
	{
		SDA::Off();
		Hdel();
		SCL::On();
		qdel();
		SDA::On();
		Hdel();
	}
};

} // namespace I2C
} // namespace STM32
