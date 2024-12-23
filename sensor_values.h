#ifndef SENSOR_VALUES_H
#define SENSOR_VALUES_H

#include <cstdint>
#include <string>

class SensorValues
{
public:
    SensorValues();
    std::string to_json() const;

public:
    uint16_t cycle;
    uint16_t uv;
    float temp1;
    float humid;
    float temp2;
    float press;
    float vsys;
};

#endif
