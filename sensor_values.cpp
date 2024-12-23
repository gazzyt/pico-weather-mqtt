#include "sensor_values.h"
#include <iomanip>
#include <sstream>

SensorValues::SensorValues()
:   uv{0},
    temp1{0.0},
    humid{0.0},
    temp2{0.0},
    press{0.0},
    vsys{0.0}
{}

std::string SensorValues::to_json() const
{
    std::ostringstream sbuff;

    sbuff   << "{\"cycle\":" << cycle
            << ",\"uv\":" << uv
            << std::setprecision(3)
            << ",\"temp1\":" << temp1
            << ",\"humid\":" << humid
            << ",\"temp2\":" << temp2
            << ",\"press\":" << press
            << ",\"vsys\":" << vsys
            << "}";

    return sbuff.str();
}
