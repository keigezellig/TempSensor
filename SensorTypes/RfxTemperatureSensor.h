#ifndef RFXTEMPERATURESENSOR_H
#define RFXTEMPERATURESENSOR_H

#include <stdint.h>
#include "RfxFruitProtocol.h"
#include "X10Wrapper.h"

class RfxTemperatureSensor
{
private:
    float m_temperature;
    uint8_t m_sensorId;
    int m_previousMillis;
    int m_sendInterval;
    RfxFruitProtocol m_fruitProtocol;
    X10Wrapper m_x10Wrapper;
    void sendTemperatureThroughRf();


protected:
    virtual float readTemperature() = 0;
    virtual void setupSensor() { };
public:
    float getTemperature();
    RfxTemperatureSensor(uint8_t sensorId, uint8_t m_txPin, uint8_t m_txLedPin, uint8_t m_txRepeats);
    void setup(int sendIntervalInmsec);
    void run();
};


#endif //RFXTEMPERATURESENSOR_H
