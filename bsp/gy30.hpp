#include "stm32_i2c.hpp"

template <class I2C_Master> 
class GY30_Driver
{
private:
    static constexpr uint8_t BH1750_ADDR = 0x23 << 1; //сдвиг для добавления бита r/!w

    using i2c = I2C_Master;
    
    enum Opecode : uint8_t
    {
        PWR_DOWN            = 0x00,
        PWR_ON              = 0x01,
        RESET               = 0x07,
        CONT_HRES_MODE      = 0x10,
        CONT_HRES_MODE2     = 0x11,
        CONT_LRES_MODE      = 0x13,
        ONE_TIME_HRES_MODE  = 0x20,
        ONE_TIME_HRES_MODE2 = 0x21,
        ONE_TIME_LRES_MODE  = 0x23
    };

    static uint16_t convert(uint8_t* buff)
    {
        uint16_t value = buff[0];

        value = (value << 8) | buff[1];

        return value;
    }

    static bool sendOpecode(Opecode opc) { return i2c::Write(BH1750_ADDR, opc); }
public:
    static constexpr uint8_t BH1750_MEASURE_TIME_MS = 240;
    
    static bool Init() noexcept
    {
        i2c::Reset(); //необходимо на случай, если датчик завис при остановке транзакции без stop
        i2c::Init();

        bool init_status = true;

        init_status &= sendOpecode(PWR_ON);
        init_status &= sendOpecode(RESET);
        init_status &= sendOpecode(CONT_HRES_MODE);    

        return init_status;
    }

    static uint16_t ReadLux() noexcept
    {
        uint8_t buff[2] = {0, 0};

        i2c::Read(BH1750_ADDR, buff, 2);
        return convert(buff);
    }
};

using I2C = STM32::I2C::SimpleI2c<STM32::I2C::I2C_1>;
using GY30 = GY30_Driver<I2C>;