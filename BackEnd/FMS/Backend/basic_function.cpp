

#include "basic_function.h"

bool DriverMotorMoveTo(int motor_id, int line_pos)
{
  return true;
}

bool PinInitialization(void)
{
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