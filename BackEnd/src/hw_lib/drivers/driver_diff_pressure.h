

#ifndef _DRIVER_DIFF_PRESSURE_H_
#define _DRIVER_DIFF_PRESSURE_H_

#define DIFFPRESSURE_CONSTANT (10.197162129779/0.057)  // to compute pressure in cmH2O
#define DIFFPRESSURE_OFFSET 0.5 // to compute pressure in cmH2O
#define DIFFPRESSURE_UPDATE_PERIDO 10 //ms

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

        void Begin(void);
        void Loop(void);


        //getter
        float GetDiffPressure(void);

};






#endif