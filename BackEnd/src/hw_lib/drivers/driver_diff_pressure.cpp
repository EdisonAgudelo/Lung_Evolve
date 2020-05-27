

#include "driver_diff_pressure.h"
#include "hardware_interface.h"
#include "../time.h"

DiffPressure::DiffPressure(int _analog_pin):
analog_pin(_analog_pin)
{
    PinReadAnalog(analog_pin);
}

DiffPressure::~DiffPressure()
{
}

void DiffPressure::Loop(void)
{
    if(DIFFPRESSURE_UPDATE_PERIDO<GetDiffTime(Millis(), prev_update_time))
    {
        prev_update_time = Millis();

        analog_value=(analog_pin);
    }
}

//getter
float DiffPressure::GetDiffPressure(void)
{
    return (((float)analog_value)/kACDResolution - DIFFPRESSURE_OFFSET)*DIFFPRESSURE_CONSTANT;
}