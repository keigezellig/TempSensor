//
// Created by MJOX03 on 6.6.2016.
//

#include "X10Wrapper.h"

X10Wrapper::X10Wrapper(uint8_t txPin, uint8_t txLedPin, uint8_t txRepeats)
        : m_txPin(txPin), m_txLedPin(txLedPin),
          m_txRepeats(txRepeats), m_rftrx(txPin,txLedPin,txRepeats)
{

}

void X10Wrapper::send(uint8_t sensorId, long value)
{

 //Send it according to the RFXMeter protocol with value encoded according to the RfxFruit protocol
  m_rftrx.RFXmeter(sensorId,0,value);
}

void X10Wrapper::setup()
{

  m_rftrx.begin();

}



