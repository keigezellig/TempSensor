//
// Created by MJOX03 on 1.6.2016.
//

#ifndef UNTITLED_RFXFRUITPROTOCOL_H
#define UNTITLED_RFXFRUITPROTOCOL_H

#include <stdint.h>

struct RfxFruitValue
{
    int32_t value;
    uint8_t unit;
};


class RfxFruitProtocol
{
public:
    uint32_t encode(int32_t value, uint8_t unit);
    RfxFruitValue decode(uint32_t received_value);
};




#endif //UNTITLED_RFXFRUITPROTOCOL_H

