

#include "driver_water_flowmeter.h"

#include "../low_level/hardware_interface.h"

#include "../time.h"

WFlowmeter::WFlowmeter(int8_t _counter_id, float _counts_per_slm) : counter_id(_counter_id), counts_per_slm(_counts_per_slm)
{
 
}

WFlowmeter::~WFlowmeter()
{
    //this library suppor instance deletion but it is no recommended
}

void WFlowmeter::Begin(void)
{
    CounterBegin(counter_id);

    prev_update_time = 0;

    flow_counts = 0;
    flow_rate_raw = 0;
}

//methods
void WFlowmeter::Loop(void)
{
    uint32_t tranfer;
    if (GetDiffTime(Millis(), prev_update_time) >= W_FLOWMETER_UPDATE_RATE)
    {
        tranfer = CounterGetValue(counter_id);
        flow_rate_raw = tranfer*1000/(GetDiffTime(Millis(), prev_update_time));
        flow_counts += tranfer;

        prev_update_time = Millis();
    }
}

//getters
float WFlowmeter::GetFlow(void)
{
    return ((float)flow_rate_raw)/counts_per_slm;
}

float WFlowmeter::GetVolume(bool reset)
{
    float temp = ((float)flow_counts)/counts_per_slm;
    if(reset)
        flow_counts = 0;
    return temp;
}
