#ifndef X10WRAPPER_H
#define X10WRAPPER_H

#include <x10rf.h>
class X10Wrapper
{
private:
    uint8_t m_txPin;
    uint8_t m_txLedPin;
    uint8_t m_txRepeats;
    x10rf m_rftrx;

public:
    void send(uint8_t sensorId, long value);
    void setup();
    X10Wrapper(uint8_t txPin, uint8_t _txLedPin, uint8_t txRepeats);
};


#endif


