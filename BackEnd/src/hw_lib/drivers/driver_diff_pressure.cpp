

#include "driver_diff_pressure.h"
#include "hardware_interface.h"
#include "../time.h"

DiffPressure::DiffPressure(int _analog_pin) : analog_pin(_analog_pin)
{

}

DiffPressure::~DiffPressure()
{
}

void DiffPressure::Begin(void)
{
    PinConfigAnalog(analog_pin);
    prev_update_time = Millis();
}

void DiffPressure::Loop(void)
{
    int i;
    uint32_t average;

    if (DIFFPRESSURE_UPDATE_PERIDO < GetDiffTime(Millis(), prev_update_time))
    {
        prev_update_time = Millis();
        //compute an averge to try to cancel noise
        average = 0;
        for (i = 0; i < 10; i++)
            average += PinReadAnalog(analog_pin);
        
        //if an interrupt happen, this is to make sure that last value is stored until the end 
        analog_value = average/i;
    }
}

//getter
float DiffPressure::GetDiffPressure(void)
{
    return (((float)analog_value) / ((float)kACDResolution)- DIFFPRESSURE_OFFSET) * DIFFPRESSURE_CONSTANT;
}