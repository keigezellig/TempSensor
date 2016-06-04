//
// Created by MJOX03 on 1.6.2016.
//

#include "RfxFruitProtocol.h"
#include "Arduino.h"

uint32_t RfxFruitProtocol::encode(int32_t value, uint8_t unit)
{
    uint32_t value_to_be_converted = abs(value);

    if (value < 0)
    {
        //Add sign bit
        value_to_be_converted |= (uint32_t)(0x80000);
    }

    value_to_be_converted |= (uint32_t)(unit) << 20;

    return value_to_be_converted;
}

RfxFruitValue RfxFruitProtocol::decode(uint32_t received_value)
{
    RfxFruitValue result;
    uint32_t isSignBitSet = received_value & 0x80000;
    uint32_t type = received_value >> 20;
    uint32_t value = received_value & 0x7FFFF;

    int32_t endvalue = isSignBitSet != 0 ? -value : value;

    result.unit = type;
    result.value = endvalue;

    return result;

}







