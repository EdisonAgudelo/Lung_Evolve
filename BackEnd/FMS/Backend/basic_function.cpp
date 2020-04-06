

#include "basic_function.h"

#include "hardware_interface.h"


 DriverLed led_red;
 DriverLed buzzer;

bool DriverMotorMoveTo(int motor_id, int line_pos)
{
  return true;
}

bool PinInitialization(void)
{
  pin_config_digital(kHardwareLedRedPin, kOutput);
  pin_config_digital(kHardwareBuzzerPin, kOutput);
  return true;
}

bool DriverValveClose(int valve_id)
{
  return true;
}

bool DriverValveOpenTo(int valve_id, bool valve_position)
{
  return true;
}

//return filtered pressure in cmH20*1000 or air flow in mL/min*1000
long int SensorGetValue(int sensor_id)
{
  return 0;
}

bool DriverMotorSetVel(int motor_id, int motor_vel)
{
  return true;
}

int DriverMotorActualPos(int motor_id)
{
  return 100;
}

bool DirverInitialization(void)
{

  DriverLedInit(&led_red, kHardwareLedRedPin); //config pin and initialize pin
 // DriverLedInit(&buzzer, kHardwareBuzzerPin); //config pin and initialize pin

  return true;
}

bool DriverLoops(void)
{
 // DriverLedLoop(&led_red);
 // DriverLedLoop(&buzzer);
}
