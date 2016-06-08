//
// Created by MJOX03 on 6.6.2016.
//

#include "TH02TemperatureSensor.h"
#include <TH02_dev.h>


TH02TemperatureSensor::TH02TemperatureSensor(uint8_t sensorId, uint8_t m_txPin, uint8_t m_txLedPin, uint8_t m_txRepeats)
        : RfxTemperatureSensor(sensorId, m_txPin, m_txLedPin, m_txRepeats) { }

void TH02TemperatureSensor::sensorSetup()
{
   delay(150);
   TH02.begin();
   delay(100);
}
float TH02TemperatureSensor::readTemperature()
{
    return TH02.ReadTemperature();
}
