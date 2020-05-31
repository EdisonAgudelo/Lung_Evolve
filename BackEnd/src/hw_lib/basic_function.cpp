

#include "basic_function.h"

#include "drivers/hardware_interface.h"
#include "drivers/driver_stepper.h"
#include "drivers/driver_flowmeter.h"
#include "drivers/driver_diff_pressure.h"

#include "time.h"



const uint8_t kMotorBellowUSteps = 8; //review
const uint8_t kMotorO2USteps = 2; //review
const uint8_t kMotorAirUSteps = 2; //review

const uint16_t kMotorBellowSteps = 32; //review
const uint16_t kMotorO2Steps = 200; //review
const uint16_t kMotorAirSteps = 200; //review

const float kMotorBellowmmRev = (1/63.8); //review
const float kMotorO2mmRev = 200.0; //review
const float kMotorAirmmRev = 200.0; //review

const float kCountsPerSLM = 5.0; //review

DriverLed led_red;
DriverLed buzzer;

Stepper motor_bellow(kHardwareStepMotor1, kHardwareDirMotor1, kHardwareEnMotor1, kHardwarePWMMotor1);
Stepper motor_valve_o2(kHardwareStepMotor2, kHardwareDirMotor2, kHardwarePWMMotor2);
Stepper motor_valve_air(kHardwareStepMotor3, kHardwareDirMotor3, kHardwarePWMMotor3);

//Flowmeter flow_in(kHardwareCounterFlow1, kCountsPerSLM);
//Flowmeter flow_out(kHardwareCounterFlow2, kCountsPerSLM);
Flowmeter flow_in(0, kCountsPerSLM);
Flowmeter flow_out(1, kCountsPerSLM);

DiffPressure pressure_in(kHardwareDiffPressure1);
DiffPressure pressure_out(kHardwareDiffPressure1);


Stepper *motor[]={&motor_bellow, &motor_valve_o2, &motor_valve_air};
void *Sensor[]={(void *)&flow_in,(void *)&flow_out, (void *)&pressure_in, (void *)&pressure_out};
const int valve[]={kHarwareRele1, kHarwareRele2, kHarwareRele3};



bool DriverMotorMoveTo(int motor_id, float line_pos)
{
  motor[motor_id]->SetPosmm(line_pos);
  return true;
}

bool PinInitialization(void)
{
  PinConfigDigital(kHardwareLedRedPin, kOutput);
  PinConfigDigital(kHardwareBuzzerPin, kOutput);

  PinConfigDigital(kHarwareRele1, kOutput);
  PinConfigDigital(kHarwareRele2, kOutput);
  PinConfigDigital(kHarwareRele3, kOutput);

  return true;
}


bool DriverValveOpenTo(int valve_id, bool valve_position)
{
  if(valve_position) // full open
    PinSetDigital(valve[valve_id], kHigh);
  else //full close
    PinSetDigital(valve[valve_id], kLow);
    
  return true;
}

//return filtered pressure in cmH20*1000 or air flow in mL/min*1000
long int SensorGetValue(int sensor_id)
{
  if(sensor_id<2)
    return (uint32_t) ((Flowmeter *)Sensor[sensor_id])->GetFlow()*1000;
  
  return (uint32_t) ((DiffPressure *)Sensor[sensor_id])->GetDiffPressure()*1000;
}

bool DriverMotorSetVel(int motor_id, float motor_vel)
{
  motor[motor_id]->SetVelmm(motor_vel);
  return true;
}

float DriverMotorActualPos(int motor_id)
{
  return motor[motor_id]->GetPosmm();
}

bool DirverInitialization(void)
{

  TimeVirtualISRBegin();
   
  DriverLedInit(&led_red, kHardwareLedRedPin); //config pin and initialize pin
  DriverLedInit(&buzzer, kHardwareBuzzerPin); //config pin and initialize pin

  motor_bellow.Begin();
  motor_valve_o2.Begin();
  motor_valve_air.Begin();

  flow_in.Begin();
  flow_out.Begin();

  pressure_in.Begin();
  pressure_out.Begin();


  motor_bellow.SetLimitPin(kHardwareSwitchBMotor1, kHardwareSwitchFMotor1);
/* restore  
  motor_valve_o2.SetLimitPin(kHardwareSwitchBMotor2, kHardwareSwitchFMotor2);
  motor_valve_air.SetLimitPin(kHardwareSwitchBMotor3, kHardwareSwitchFMotor3);
  */
  motor_bellow.SetDriverConfig(kMotorBellowSteps, kMotorBellowmmRev, kMotorBellowUSteps);
  motor_valve_o2.SetDriverConfig(kMotorO2Steps, kMotorO2mmRev, kMotorO2USteps);
  motor_valve_air.SetDriverConfig(kMotorAirSteps, kMotorAirmmRev, kMotorAirUSteps);

  return true;
}

bool DriverLoops(void)
{

  DriverLedLoop(&led_red);
  DriverLedLoop(&buzzer);
  
  motor_bellow.Loop(); //much faster than a for
  motor_valve_o2.Loop();
  motor_valve_air.Loop();

  flow_in.Loop();
  flow_out.Loop();

  pressure_in.Loop();
  pressure_out.Loop();


}