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
    float temp2;
    float temp3;
    float temp4;
    float humid;
    float humid2;
    float humid3;
    float press;
    uint32_t press2;
    float vsys;
};

#endif
