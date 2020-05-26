

#include "driver_flowmeter.h"

#include "hardware_interface.h"

#include "../time.h"

Flowmeter::Flowmeter(int8_t _counter_id, float _counts_per_slm) : counter_id(_counter_id), counts_per_slm(_counts_per_slm)
{
    CounterBegin(counter_id);

    prev_update_time = 0;

    flow_counts = 0;
    flow_rate_raw = 0;
}

Flowmeter::~Flowmeter()
{
    //this library suppor instance deletion but it is no recommended
}

//methods
void Flowmeter::Loop(void)
{
    uint32_t tranfer;
    if (GetDiffTime(Millis(), prev_update_time) > FLOWMETER_UPDATE_RATE)
    {
        prev_update_time = Millis();

        tranfer = CounterGetValue(counter_id);
        flow_rate_raw = tranfer*1000/(FLOWMETER_UPDATE_RATE);
        flow_counts += tranfer;
    }
}

//getters
float Flowmeter::GetFlow(void)
{
    return flow_rate_raw/counts_per_slm;
}

float Flowmeter::GetVolume(bool reset = false)
{
    return flow_counts/counts_per_slm;
}
