

#ifndef _DRIVER_FLOWMETER_H_
#define _DRIVER_FLOWMETER_H_

#include <stdint.h>
#include <stdbool.h>

#define FLOWMETER_UPDATE_RATE 50 //ms


class Flowmeter
{

private:
    uint8_t counter_id; //it saves wich timer is working with 
    uint32_t prev_update_time; //for time aplication purpose

    uint32_t flow_counts;
    uint32_t flow_rate_raw;

    float counts_per_slm; //countos per standar liter per minute

public:
    
    Flowmeter(int8_t _counter_id, float _counts_per_slm);
    ~Flowmeter();

    //methods
    void Loop(void);

    //getters
    float GetFlow(void);
    float GetVolume(bool reset = false);

};



#endif