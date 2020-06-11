

#ifndef _DRIVER_W_FLOWMETER_H_
#define _DRIVER_W_FLOWMETER_H_

#include <stdint.h>
#include <stdbool.h>

#define W_FLOWMETER_UPDATE_RATE 200 //ms


class WFlowmeter
{

private:
    uint8_t counter_id; //it saves wich timer is working with 
    uint32_t prev_update_time; //for time aplication purpose

    uint32_t flow_counts; //all saved counts
    uint32_t flow_rate_raw; //instant value

    float counts_per_slm; //counts per standar liter per minute

public:
    
    WFlowmeter(int8_t _counter_id, float _counts_per_slm);
    ~WFlowmeter();

    void Begin(void);
    //methods
    void Loop(void);

    //getters
    float GetFlow(void);
    float GetVolume(bool reset = false);

};



#endif