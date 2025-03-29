#include "sensor_values.h"
#include <iomanip>
#include <sstream>

SensorValues::SensorValues()
:   uv{0},
    temp1{0.0},
    temp2{0.0},
    temp3{0.0},
    temp4{0.0},
    humid{0.0},
    humid2{0.0},
    humid3{0.0},
    press{0.0},
    press2{0},
    vsys{0.0}
{}

std::string SensorValues::to_json() const
{
    std::ostringstream sbuff;

    sbuff   << "{\"cycle\":" << cycle
            << ",\"uv\":" << uv
            << std::setprecision(3)
            << ",\"temp1\":" << temp1
            << ",\"temp2\":" << temp2
            << ",\"temp3\":" << temp3
            << ",\"temp4\":" << temp4
            << ",\"humid\":" << humid
            << ",\"humid2\":" << humid2
            << ",\"humid3\":" << humid3
            << ",\"press\":" << press
            << ",\"press2\":" << press2
            << ",\"vsys\":" << vsys
            << "}";

    return sbuff.str();
}
