
/*
    Lung Evolve Mechanical Ventilator
    Copyright (C) 2020  Edison Agudelo, Mateo Garcia, Alejandra Londoño

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.html.
    
    */
   
#include "basic_function.h"

#include "low_level/hardware_interface.h"
#include "drivers/driver_stepper.h"
#include "drivers/driver_flowmeter.h"
#include "drivers/driver_diff_pressure.h"
#include "drivers/driver_voltage.h"
#include "drivers/driver_temp.h"
#include "drivers/driver_water_flowmeter.h"
#include "debug.h"

#include "time.h"

//--------motor parameters-------//

const uint8_t kMotorBellowUSteps = 16; //review
const uint8_t kMotorO2USteps = 8;      //review
const uint8_t kMotorAirUSteps = 8;     //review

const uint16_t kMotorBellowSteps = 200; //review
const uint16_t kMotorO2Steps = 32;      //review
const uint16_t kMotorAirSteps = 32;     //review

const float kMotorBellowmmRev = 100.0;      //review
const float kMotorO2mmRev = (1.27 / 63.8);  //review
const float kMotorAirmmRev = (1.27 / 63.8); //review

//------- Flow sensor parameters ------//

const uint8_t kFlowI2CAddrs = 64; //review
const float kCountsPerSLM = 5.0;  //review

//------- Temp sensor parameters -------//

const float kTempMaxValueMotor = 75.0;  //review °C
const float kTempMinValueMotor = -10.0; //review °C

const float kTempMaxValueBattery = 55.0; //review °C
const float kTempMinValueBattery = 15.0; //review °C

//------- Voltage sensor parameters ----//

const float kVoltageMaxValueBattery = 13.7; //review V
const float kVoltageMinValueBattery = 11.0; //review V

const float kVoltageMaxValueSource = 14.0; //review V
const float kVoltageMinValueSource = 13.6; //review V

const float kVoltageAttenuationBattery = (51.0 + 100.0) / (51.0); //for 51k and 100k resistors
const float kVoltageAttenuationSource = (51.0 + 100.0) / (51.0);  //for 51k and 100k resistors

//------ Driver objects creation -------//
DriverLed g_led_red;
DriverLed g_buzzer;
DriverLed g_discharge_rele;

Stepper motor_bellow(kHardwareStepMotor1, kHardwareDirMotor1, kHardwareEnMotor1, kHardwarePWMMotor1);
Stepper motor_valve_o2(kHardwareStepMotor2, kHardwareDirMotor2, kHardwareEnMotor2, kHardwarePWMMotor2);
Stepper motor_valve_air(kHardwareStepMotor3, kHardwareDirMotor3, kHardwareEnMotor3, kHardwarePWMMotor3);

Flowmeter flow_in(kHardI2C, kFlowI2CAddrs);
Flowmeter flow_out(kSoftI2C, kFlowI2CAddrs);
WFlowmeter flow_mixture(kHardwareCounterFlow1, kCountsPerSLM);

DiffPressure pressure_in(kHardwareDiffPressure1);
DiffPressure pressure_out(kHardwareDiffPressure2);

Voltage voltage_bat(kHardwareVoltage1, kVoltageAttenuationBattery);
Voltage voltage_source(kHardwareVoltage2, kVoltageAttenuationSource);

Temp temp_motor(kHardwareTemp1);
Temp temp_bat(kHardwareTemp2);

Stepper *motor[] = {&motor_bellow, &motor_valve_o2, &motor_valve_air};
void *Sensor[] = {(void *)&flow_in, (void *)&flow_out, (void *)&pressure_in, (void *)&pressure_out,
                  (void *)&temp_motor, (void *)&temp_bat, (void *)&voltage_source, (void *)&voltage_bat, (void *)&flow_mixture};
const int valve[] = {kHardwareRele1, kHardwareRele2, kHardwareRele3};

bool DriverMotorMoveTo(int motor_id, float line_pos)
{
  motor[motor_id]->SetPosmm(line_pos);
  return true;
}

bool PinInitialization(void)
{
 // PinConfigDigital(kHardwareLedRedPin, kOutput);
 // PinConfigDigital(kHardwareBuzzerPin, kOutput);

  PinConfigDigital(kHardwareRele1, kOutput);
  PinConfigDigital(kHardwareRele2, kOutput);
  PinConfigDigital(kHardwareRele3, kOutput);
  PinConfigDigital(kHardwareRele4, kOutput);
  digitalWrite(kHardwareRele4, kHigh);

  return true;
}

bool DriverValveOpenTo(int valve_id, bool valve_position)
{
  if (valve_position) // full open
    PinSetDigital(valve[valve_id], kHigh);
  else //full close
    PinSetDigital(valve[valve_id], kLow);

  return true;
}

//return filtered sensor values
float SensorGetValue(int sensor_id)
{
  if (sensor_id < 2)
    return ((Flowmeter *)Sensor[sensor_id])->GetFlow();
  if (sensor_id < 4)
    return ((DiffPressure *)Sensor[sensor_id])->GetDiffPressure();
  if (sensor_id < 6)
    return ((Temp *)Sensor[sensor_id])->GetTemp();
  if (sensor_id < 8)
    return ((Voltage *)Sensor[sensor_id])->GetVoltage();
  if (sensor_id < 9)
    return ((WFlowmeter *)Sensor[sensor_id])->GetFlow();

  return 0;
}

bool SensorGetAlarm(int sensor_id)
{
  if (sensor_id < 4)
    return false;
  if (sensor_id < 6)
    return ((Temp *)Sensor[sensor_id])->GetAlarm();
  if (sensor_id < 8)
    return ((Voltage *)Sensor[sensor_id])->GetAlarm();

  return false;
}

bool DriverMotorSetVel(int motor_id, float motor_vel)
{
  motor[motor_id]->SetVelmm(motor_vel);
  return true;
}

void DriverMotorStop(int motor_id)
{
  motor[motor_id]->RequestStop();
}

float DriverMotorActualPos(int motor_id)
{
  return motor[motor_id]->GetPosmm();
}

void DriverMotorSetZeroPos(int motor_id)
{
  motor[motor_id]->SetPosRef();
}

bool DriverMotorIsStop(int motor_id)
{
  return motor[motor_id]->GetState() == kStepperStateStop;
}
bool DirverInitialization(void)
{

  TimeVirtualISRBegin();
  DebugInit();
/*
  DriverLedInit(&g_led_red, kHardwareLedRedPin);
  DriverLedInit(&g_buzzer, kHardwareBuzzerPin);
  DriverLedInit(&g_discharge_rele, kHardwareRele4);
*/
  motor_bellow.Begin();
  motor_valve_o2.Begin();
  motor_valve_air.Begin();

  //flow_in.Begin();
  //flow_out.Begin();
  /*
  flow_mixture.Begin();

  pressure_in.Begin();
  pressure_out.Begin();

  voltage_bat.Begin();
  voltage_source.Begin();

  temp_motor.Begin();
  temp_bat.Begin();

  voltage_source.SetAlarm(kVoltageMaxValueSource, kVoltageMinValueSource);
  voltage_bat.SetAlarm(kVoltageMaxValueBattery, kVoltageMinValueBattery);
  temp_bat.SetAlarm(kTempMaxValueBattery, kTempMinValueBattery);
  temp_motor.SetAlarm(kTempMaxValueMotor, kTempMinValueMotor);*/

  motor_bellow.SetLimitPin(kHardwareSwitchBMotor1, kHardwareSwitchFMotor1);
  motor_valve_o2.SetLimitPin(kHardwareSwitchBMotor2, kHardwareSwitchFMotor2);
  motor_valve_air.SetLimitPin(kHardwareSwitchBMotor3, kHardwareSwitchFMotor3);

  motor_bellow.SetDriverConfig(kMotorBellowSteps, kMotorBellowmmRev, kMotorBellowUSteps);
  motor_valve_o2.SetDriverConfig(kMotorO2Steps, kMotorO2mmRev, kMotorO2USteps);
  motor_valve_air.SetDriverConfig(kMotorAirSteps, kMotorAirmmRev, kMotorAirUSteps);

  return true;
}

bool DriverLoops(void)
{
/*
  DriverLedLoop(&g_led_red);
  DriverLedLoop(&g_buzzer);
  DriverLedLoop(&g_discharge_rele);*/
  // restore
  motor_bellow.Loop();
/*
  motor_valve_o2.Loop();
  motor_valve_air.Loop();

  flow_in.Loop();
  flow_out.Loop();
  flow_mixture.Loop();

  pressure_in.Loop();
  pressure_out.Loop();

  temp_bat.Loop();
  temp_motor.Loop();

  voltage_bat.Loop();
  voltage_source.Loop();*/
}
