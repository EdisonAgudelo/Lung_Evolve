#ifndef _DRIVER_LED_H    /* Guard against multiple inclusion */
#define _DRIVER_LED_H

    #include <stdbool.h>
    #include <stdint.h>
    
   enum DriverLedEstates
    {
        kLedOn,
        kLedOff,
        kLedDeath
    };
    
    typedef struct
    {
        int gpio_pin;
        enum DriverLedEstates state;
        bool blink;
        bool one_shoot;
        uint32_t one_shoot_time;
        uint32_t period_ms;
        uint32_t ref_time;

        //for blink n times
        uint32_t period_death_ms; 
        uint8_t n_times_count;
        uint8_t n_times_ref;
        
    } DriverLed;
    
    //constructor
    void DriverLedInit(DriverLed *led, int gpio_output);
    
    //methods

    void DriverLedNBlink(DriverLed *led, uint32_t blink_period, int n_count);
    void DriverLedBlink(DriverLed *led, uint32_t blink_period_ms);

    void DriverLedTShoot(DriverLed *led, uint32_t shoot_period_ms, uint32_t shoot_time);
    void DriverLedShoot(DriverLed *led, uint32_t shoot_time_ms);
    void DriverLedTurnOn(DriverLed *led);
    void DriverLedTurnOff(DriverLed *led);
    
    void DriverLedLoop(DriverLed *led);   
    void set_pin(int gpio);
    void clear_pin(int gpio);
    
#endif 