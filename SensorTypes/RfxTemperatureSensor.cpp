//
// Created by MJOX03 on 6.6.2016.
//



#include "RfxTemperatureSensor.h"


void RfxTemperatureSensor::sendTemperatureThroughRf()
{
    long T = (long)(m_temperature * 100);
    uint32_t encoded = m_fruitProtocol.encode(T,0x01);
    m_x10Wrapper.send(m_sensorId,(long)encoded);
}

float RfxTemperatureSensor::getTemperature()
{
    return m_temperature;
}

RfxTemperatureSensor::RfxTemperatureSensor(uint8_t sensorId, uint8_t txPin, uint8_t txLedPin, uint8_t txRepeats): m_sensorId(sensorId), m_fruitProtocol(), m_x10Wrapper(txPin, txLedPin, txRepeats), m_previousMillis(0)
{
}

void RfxTemperatureSensor::setup(int sendIntervalInmsec)
{
   m_sendInterval = sendIntervalInmsec;
   m_x10Wrapper.setup();
   setupSensor();
}

void RfxTemperatureSensor::run()
{
   m_temperature = readTemperature();

    unsigned long currentMillis = millis();

  //When interval has elapsed sent temperature
  if (currentMillis - m_previousMillis >= m_sendInterval)
  {
    m_previousMillis = currentMillis;
    sendTemperatureThroughRf();
  }


}
















