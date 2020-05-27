

#ifndef _DRIVER_DIFF_PRESSURE_H_
#define _DRIVER_DIFF_PRESSURE_H_

#define DIFFPRESSURE_CONSTANT 10.197162129779/0.057
#define DIFFPRESSURE_OFFSET 0.5
#define DIFFPRESSURE_UPDATE_PERIDO 10

#include <stdint.h>

class DiffPressure
{
    private:

    int analog_pin;
    uint16_t analog_value;

    uint32_t prev_update_time;

    public:

    DiffPressure(int _analog_pin);
    ~DiffPressure();

    void Loop(void);


    //getter
    float GetDiffPressure(void);


};






#endif