#pragma once
#include <cstdint>

class ControlUnit_t
{
public:
    static constexpr uint16_t LUX_FLASH_THR = 500;
private:   
    static constexpr uint32_t TIMEOUT_MS = 30000;
    static constexpr uint32_t DEBOUCE_MS = 1000;
    static constexpr uint32_t SOUND_VELOCITY_MpS = 335;
    static constexpr uint32_t TIME_RESET_VALUE = 1;
    

    enum class State : uint8_t
    {
        WAIT_FLASH,
        WAIT_NOISE,
        CALCULATE,
        DEBOUNCE
    };

    static inline State m_state;
    static inline uint16_t m_lux;
    static inline bool m_loud_noise;
    static inline uint32_t m_time_ms;
    static inline int m_distance;

    static int calculateDistaceMeters() { return (SOUND_VELOCITY_MpS * m_time_ms) / 1000;}

public:
    static constexpr int MIN_DIST_THR = 100;
    static constexpr int NO_DIST = -1;

    static void Init()
    {
        m_lux = 0;
        m_loud_noise = 0; 
        m_time_ms = TIME_RESET_VALUE;
        m_state = State::WAIT_FLASH;
        m_distance = NO_DIST;
    }
    static void SetLux(uint16_t lux) { m_lux = (m_state == State::WAIT_FLASH) ? lux : 0; }
    static void SetNoise(bool noise) { m_loud_noise = (m_state == State::WAIT_NOISE) ? noise : 0; }
    static bool ReadyToNew() { return m_state == State::WAIT_FLASH; }

    static void UpdateTime(uint32_t time_ms) 
    {
        if(m_state == State::WAIT_NOISE || m_state == State::DEBOUNCE)
            m_time_ms += time_ms;
    }

    static int GetDistance()
    {
        int result = m_distance;
        m_distance = NO_DIST;
        return result;
    }

    static void Execute()
    {
        switch (m_state)
        {
        case State::WAIT_FLASH:
            if(m_lux >= LUX_FLASH_THR)
            {
                m_state = State::WAIT_NOISE;
            }
            m_time_ms = TIME_RESET_VALUE;
            break;

        case State::WAIT_NOISE:
            if(m_loud_noise)
                m_state = State::CALCULATE;
            else if(m_time_ms > TIMEOUT_MS)
                m_state = State::WAIT_FLASH;

            break;
        case State::CALCULATE:
            m_distance = calculateDistaceMeters();
            m_state = State::DEBOUNCE;
            m_loud_noise = 0;
            m_time_ms = TIME_RESET_VALUE;
            break;

        case State::DEBOUNCE:
            if(m_time_ms >= DEBOUCE_MS)
            {
                m_state = State::WAIT_FLASH;
                m_time_ms = TIME_RESET_VALUE;
                m_lux = 0;
            }
        default:
            break;
        }
    }
};
