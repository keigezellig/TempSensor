//
// Created by MJOX03 on 6.6.2016.
//

#ifndef TH02TEMPERATURESENSOR_H
#define TH02TEMPERATURESENSOR_H

#include "RfxTemperatureSensor.h"
class TH02TemperatureSensor : public RfxTemperatureSensor
{

public:
    TH02TemperatureSensor(uint8_t sensorId, uint8_t m_txPin, uint8_t m_txLedPin, uint8_t m_txRepeats);
protected:
    virtual float readTemperature();
    virtual void sensorSetup();

};




#endif //TH02TEMPERATURESENSOR_H
