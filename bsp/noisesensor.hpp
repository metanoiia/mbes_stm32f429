#pragma once
#include "pin.hpp"

template <class dataPin> 
class NoiseSensor_Driver
{
public:
    static void Init() { dataPin::Mode(INPUT); }
    static bool Get() { return dataPin::Signalled(); }
};

using noiseDataPin = Pin<'B', 8, 'H', PIN_SPEED_50MHZ>;
using NoiseSensor = NoiseSensor_Driver<noiseDataPin>;