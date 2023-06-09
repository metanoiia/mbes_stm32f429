#pragma once
#include "pin.hpp"

template <class dataPin> 
class NoiseSensor_Driver
{
public:
    static void Init() { dataPin::Mode(INPUT); }
    static bool HasNoise() { return dataPin::Signalled(); }
};
