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

#include "definitions.h"
#include "src/hw_lib/basic_function.h"
#include "src/fw_lib/control.h"
#include "src/hw_lib/time.h"
#include "src/hicop_lib/hicop.h"
#include "src/hw_lib/debug.h"

//error variable
static ErrorType main_error;

//warning variable
static WarningType main_warning;          //saves all generated warnings
static WarningType main_warning_mask;     //enable warnings transmission
static WarningType frontend_warning_copy; //to detect changes

//state machine variables
static MainStates main_state;
static BreathingStates breathing_state;
static InitStates init_state;

//breathing config variables
static BreathingParameters breathing_config;

//this data is filled when a new config arrive from front end MCU.
static BreathingParameters breathing_config_request;
static bool breathing_config_change = false;

//breathing on run parameters (more specific config)
static BreathingDinamics breathing_dinamic;

//working measurenmet
static MeasureType system_measure;

//control variables
static ControlData control_pressure;
static ControlData control_air_flow;

//----------Test varibles----------//
#if 0 //for test loop delay
uint32_t max_delay_time = 0;
uint32_t ref_time_delay = 0;
uint32_t ref_time_delay_print = 0;
uint32_t min_delay_time = 1000;
uint32_t any_use_time;
uint32_t count = 0;
#endif

//----------Local funcitions---------//

////this function makes all initialization related to breathing FSM
void FMSMainInit(void);

//this function actually is a machine state which only does breathing control action.
//this function needs that someone config working parameters to work porperly
void FMSMainLoop(void);

// this function make all necesary change during breathing state transition.
// For performance reasons, it should be called only on transtion events
void FMSBreathingConfigHW(void);

//this funcion measures o calcultes all physical variables
void MeasureVariables(void);

//this funcition watch all important variables and makes warnings through WarningType varible which is
//a grup o flags. This function takes sensor instat values and compare with breathing_config thresholds but it doesn't
//take any action in respect
void WarningMonitor(void);

//this function Takes warning flags and does some kind of actions, also it does a better flag processing
void WarningActions(void);

//this function analyzes all config parameters to see if they are valid. if there are valid config, therefore calculte working parameters
void ComputeParameters(void);

//this function init front end comunication
void FrontEndCommunicationInit(void);

//this function sends and receive important data from front end MCU
void FrontEndCommunicationLoop(void);

//general initization

void setup()
{
  main_error.all = 0;   //reset all errors;
  main_warning.all = 0; //reset all warnings

  //hardware initialization
  main_error.init_hardware = !PinInitialization();

  //driver initialization
  main_error.init_driver = !DirverInitialization();

  FMSMainInit();
  FrontEndCommunicationInit();

#if 0 //for test delay loop
  ref_time_delay = Millis();
  ref_time_delay_print = ref_time_delay;
#endif
}

void FMSMainInit(void)
{
  main_warning_mask.all = 0xffffffff; //enable all Warning

  //indicate that memory has random data
  main_error.working_configuration_not_initialized = true;

  //variable inizialitacion
  //resotore
  main_state = kMainInit;               //start in idle mode
  breathing_state = kBreathingOutPause; //default state
  init_state = kInitStartFuelBellow;    //init secuence

  breathing_config.is_tunning = false;
  breathing_config.is_standby = true;
  breathing_config.is_volume_controled = true;
  breathing_config.is_assisted = false;

  //set PID control parameters
  ControlInit(&control_pressure, 1.0, 0.0, 0.0, -1.0);
  ControlInit(&control_air_flow, 1.0, 0.0, 0.0, -1.0);
}

void FrontEndCommunicationInit(void)
{
  //init comunication protocol layer
  HicopInit();
  //init front end coppy to detect when any warning change state
  frontend_warning_copy.all = main_warning.all & main_warning_mask.all;
}

uint32_t time_ref = 0;

void loop()
{

  MeasureVariables();
  WarningMonitor();
  WarningActions();
  FMSMainLoop();
  DriverLoops();
  FrontEndCommunicationLoop();

  int i;

  if (GetDiffTime(Millis(), time_ref) > 1500)
  {
    time_ref = Millis();
    dbprintf("Data: ");
    for (i = 0; i < 14; i++)
    {
      Serial.print(system_measure.general[i]);
      dbprintf(" ");
    }
    Serial.print(flow_in.Available());
    Serial.print(flow_out.Available());
    dbprintf("\n");
  }

#if 0 //for test loop delay

  any_use_time = GetDiffTime(Millis(), ref_time_delay);
  ref_time_delay = Millis();

  count++;

  if (any_use_time > max_delay_time)
    max_delay_time = any_use_time;

  if (any_use_time < min_delay_time)
    min_delay_time = any_use_time;

  if (GetDiffTime(Millis(), ref_time_delay_print) > 5000)
  {
    ref_time_delay_print = Millis();
    dbprintf("INFO: delay time %lu, %lu, %lu \n", max_delay_time, count, min_delay_time);
    count = 0;
    min_delay_time = 1000;
    max_delay_time = 0;
  }

#endif
}

void ComputeParameters(void)
{
  uint8_t i;

  //this temporal variables is used to make first calculation a check if time result are sound.
  //If everything is ok, transfer this values to breathing dinamics

  int32_t breathing_in_puase_time;
  int32_t breathing_in_time;
  int32_t breathing_out_puase_time;
  int32_t breathing_out_time;

  //TODO: make necesary actions to calculte chocke valves position

  //if there is new config pending
  if (breathing_config_change)
  {
    breathing_config_change = false;

    dbprintf("INFO: New config parameters received\n");

    //////////////here is where config parameters request is proecessed///////////////

    //transfer flags
    for (i = 0; i < 5; i++)
      breathing_config.all[i] = breathing_config_request.all[i];

    //delete
    //breathing_config_request.breathing_rate/=5;
    //calculate new breathing dinamics
    breathing_in_puase_time = ((int32_t)breathing_config_request.pause_time);
    breathing_in_time = 60000 / (((int32_t)breathing_config_request.breathing_rate) * (((int32_t)breathing_config_request.ie_ratio) + 1));
    breathing_out_puase_time = ((int32_t)breathing_config_request.apnea_time);
    breathing_out_time = (60000 * ((int32_t)breathing_config_request.ie_ratio)) / (((int32_t)breathing_config_request.breathing_rate) * (((int32_t)breathing_config_request.ie_ratio) + 1));

    breathing_out_time -= breathing_out_puase_time;

    if (breathing_out_time <= 0)
    {
      //calculus are no valid. End execution
      main_error.working_configuration_not_initialized = true;
      dbprintf("ERROR: conf parameters invalid\n");
      return;
    }

    //if breathig is controled
    if (!breathing_config.is_assisted)
    {
      breathing_in_time -= breathing_in_puase_time;

      if (breathing_in_time <= 0)
      {
        //calculus are no valid. End execution
        main_error.working_configuration_not_initialized = true;
        dbprintf("ERROR: conf parameters invalid\n");
        return;
      }
    }
    else
    {
      //breathing_dinamic.breathing_in_puase_time
      //is used to determine maximum pause time, so it no should be used on inspiration  time calculation
    }

    //transfer all new parameters
    for (; i < sizeof(breathing_config.all); i++)
      breathing_config.all[i] = breathing_config_request.all[i];

    // transfer main calculations
    breathing_dinamic.breathing_in_puase_time = breathing_in_puase_time;
    breathing_dinamic.breathing_in_time = breathing_in_time;
    breathing_dinamic.breathing_out_puase_time = breathing_out_puase_time;
    breathing_dinamic.breathing_out_time = breathing_out_time;

    //check other data

    //in the worst case, motors should travel all available distance
    breathing_dinamic.motor_return_vel_bellows = (kMotorBellowMaxPos * 1000.0) / (((float)breathing_dinamic.breathing_out_time) * 0.9); //mm/s

    if (breathing_config.is_volume_controled)
      breathing_dinamic.sensor_air_flow_ref = ((float)breathing_config.volume_tidal * 60.0) / ((float)breathing_dinamic.breathing_in_time);
    else
      breathing_dinamic.sensor_pressure_ref = (float)breathing_config.in_presure;

    if (breathing_config.trigger_source_pressure)
    { //  manifold        inhalation         exhalation
      breathing_dinamic.valve_position[kBreathingInCicle] = (ValvePos){kValveFullClose, kValveFullOpen, kValveFullClose};
      breathing_dinamic.valve_position[kBreathingInPause] = (ValvePos){kValveFullClose, kValveFullClose, kValveFullClose};
      breathing_dinamic.valve_position[kBreathingOutCicle] = (ValvePos){kValveFullOpen, kValveFullClose, kValveFullOpen};
      breathing_dinamic.valve_position[kBreathingOutPause] = (ValvePos){kValveFullOpen, kValveFullClose, kValveFullOpen};

      breathing_dinamic.sensor_pressure_trigger_ins_value = (float)breathing_config.minimun_peep - (float)breathing_config.trigger_value;
    }
    else
    {
      breathing_dinamic.valve_position[kBreathingInCicle] = (ValvePos){kValveFullClose, kValveFullOpen, kValveFullClose};
      breathing_dinamic.valve_position[kBreathingInPause] = (ValvePos){kValveFullOpen, kValveFullClose, kValveFullClose};
      breathing_dinamic.valve_position[kBreathingOutCicle] = (ValvePos){kValveFullOpen, kValveFullClose, kValveFullOpen};
      breathing_dinamic.valve_position[kBreathingOutPause] = (ValvePos){kValveFullClose, kValveFullOpen, kValveFullOpen};

      breathing_dinamic.sensor_flow_trigger_ins_value = (float)breathing_config.trigger_value;              //flow deviation
      breathing_dinamic.motor_flow_bias_vel_bellows = (breathing_config.trigger_value + 1) * kBellowFactor; //
    }

    //----------review----------//

    breathing_dinamic.motor_position_o2_choke = ((float)breathing_config.FiO2 - kMinimunFiO2) * kConstantO2Choke; //valve position to control O2 flow through pneumatic system
    //restore
    //breathing_dinamic.motor_position_air_choke = ((float)breathing_config.FiO2 - kMinimunFiO2) * kConstantAirChoke; //valve position to control O2 flow through pneumatic system
    breathing_dinamic.motor_position_air_choke = kMotorChokeMaxPos;
    //--------------------------//
    dbprintf("INFO: config parameters accepted\n");

    main_error.working_configuration_not_initialized = false;
  }
  //if ventilator is waithing parameters and nothing happen
  else if (main_state == kMainIdle)
  {
    main_error.working_configuration_not_initialized = true;
  }
}

void MeasureVariables(void)
{

  // time varibles
  static uint32_t last_inspiration_ref_time = 0;
  static uint32_t last_espiration_ref_time = 0;
  static uint32_t last_dt_ref_time = 0;

  static uint32_t inspiration_time = 1000; // any value to avoid 0 division
  static uint32_t espiration_time = 2000;  //any value
  static uint32_t dt_time = 0;
  //other type variables
  static BreathingStates breathing_previous_state = breathing_state;

  static DataTimeRef measure_ref_time = {0, 0};
  static uint32_t apnea_ref_time = 0;

  //check if slow data has new values
  if (GetDiffTime(Millis(), measure_ref_time.slow_data) > kSlowDataPeriod)
  {

    measure_ref_time.slow_data = Millis();

    //update values
    system_measure.battery_voltage = SensorGetValue(kSensorIdVoltageBattery);
    system_measure.battery_temp = SensorGetValue(kSensorIdTempBattery);
    system_measure.source_volatge = SensorGetValue(kSensorIdVoltageSource);
    system_measure.motor_temp = SensorGetValue(kSensorIdTempMotor);

    //calculate values

    //assumed linear V vs SoC (actually it isn't true but this is only a estimation)
    system_measure.battery_level = (system_measure.battery_voltage - kBatMinVoltage) * kBatDischargerConversion;

    if (system_measure.battery_level < 0.0)
      system_measure.battery_level = 0.0;
  }

  //only Measure breathing variables if ventilator is in working mode
  if (main_state == kMainBreathing)
  {
    //always save presure measure, and flow data
    system_measure.in_pressure = SensorGetValue(kSensorIdPressureIn);
    system_measure.out_pressure = SensorGetValue(kSensorIdPressureOut);
    system_measure.patient_flow = SensorGetValue(kSensorIdAirFlowIn) + SensorGetValue(kSensorIdAirFlowOut);

    switch (breathing_state)
    {

    case kBreathingOutCicle:
      //at the end of in cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;
        last_inspiration_ref_time = Millis();
        inspiration_time = GetDiffTime(last_inspiration_ref_time, last_espiration_ref_time);

        //measure
        system_measure.ie_ratio = ((float)espiration_time) / ((float)inspiration_time);
        system_measure.breathing_rate = 60000.0 / ((float)espiration_time + (float)inspiration_time);

        //maximum value is tidal reference
        system_measure.tidal = system_measure.patient_volume;
      }

      apnea_ref_time = Millis();

      break;

    case kBreathingInCicle:

      //at the end of out cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;
        last_espiration_ref_time = Millis();
        espiration_time = GetDiffTime(last_espiration_ref_time, last_inspiration_ref_time);

        //measure
        system_measure.ie_ratio = ((float)espiration_time) / ((float)inspiration_time);
        system_measure.breathing_rate = 60000.0 / ((float)espiration_time + (float)inspiration_time);

        system_measure.apnea_time = GetDiffTime(Millis(), apnea_ref_time);

        //residual volume (probably positive) at the end of espiration cicle is
        //all leakage volume
        system_measure.patient_leakage = system_measure.patient_volume;

        //reset integer to avoid error accumulation
        last_dt_ref_time = Millis();       //reset dt;
        system_measure.patient_volume = 0; //reset integer
      }

      break;

    default:

      break;
    }

    //integer flow
    dt_time = GetDiffTime(Millis(), last_dt_ref_time);
    last_dt_ref_time = Millis();
    system_measure.patient_volume += system_measure.patient_flow * dt_time;
  }
}

void WarningActions(void)
{
  // this make a local copy of supply warning to reset discharge rele
  // when power source is pluged in
  static bool warning_power_source_prev = false;
  static bool trigger_reset = true;
  static uint32_t trigger_reset_ref_time = 0;

  if (trigger_reset && !main_warning.no_main_supply && GetDiffTime(Millis(), trigger_reset_ref_time) > 6000)
  {
    trigger_reset = false;
    DriverLedShoot(&g_discharge_rele, 5000);
    dbprintf("Reset Battery\n");
  }

  if (main_warning.no_main_supply != warning_power_source_prev)
  {
    warning_power_source_prev = main_warning.no_main_supply;
    //if supply recovery
    if (!warning_power_source_prev)
    {
      trigger_reset = true;
      trigger_reset_ref_time = Millis();
    }
    else
    {
      trigger_reset = false;
    }
  }

#if 0
  //if system is going to break up
  if (main_warning.high_temp_bat || main_warning.high_temp_motor)
  {
    DriverLedTShoot(&buzzer, 300, 10);
  }
#endif
}

//this function watches all critial varibles and make warnings if varible reaches threshold
void WarningMonitor(void)
{
  static BreathingStates breathing_previous_state = breathing_state;
  static DataTimeRef monitor_ref_time = {0, 0};

  //update other system warnings
  if (GetDiffTime(Millis(), monitor_ref_time.slow_data) > kSlowDataPeriod)
  {
    monitor_ref_time.slow_data = Millis();

    main_warning.low_battery = system_measure.battery_level < kLowBatThreshold;
    main_warning.no_battery = system_measure.battery_voltage < kNoBatThreshold; //

    main_warning.no_main_supply = system_measure.battery_voltage >= system_measure.source_volatge;

    main_warning.high_temp_bat = SensorGetAlarm(kSensorIdTempBattery);
    main_warning.high_temp_motor = SensorGetAlarm(kSensorIdTempMotor);
  }

  //only check breathing warnings if ventilator is working
  if (main_state == kMainBreathing)
  {
    switch (breathing_state)
    {

    case kBreathingOutCicle:
      //at the end of in cicle
      if (breathing_previous_state != breathing_state)
      {

        breathing_previous_state = breathing_state;
        //check warnings limits

        //check tidal values on pressure control, else reset
        if (!breathing_config.is_volume_controled)
        {
          main_warning.high_in_volume_tidal = system_measure.tidal > breathing_config.maximun_volume_tidal;
          main_warning.low_in_volume_tidal = system_measure.tidal < breathing_config.minimun_volume_tidal;
          main_warning.near_low_in_volume_tidal = system_measure.tidal < (((float)breathing_config.minimun_volume_tidal) * (1.0 + kWarningNearToLowFactor));
        }
        else
        {
          main_warning.high_in_volume_tidal = false;
          main_warning.low_in_volume_tidal = false;
          main_warning.near_low_in_volume_tidal = false;
        }

        main_warning.high_breathing_rate = system_measure.breathing_rate > (((float)breathing_config.breathing_rate) * (1.0 + kWarningMaximunBreathingRateFactor));
        main_warning.low_breathing_rate = system_measure.breathing_rate < (((float)breathing_config.breathing_rate) * (1.0 - kWarningMaximunBreathingRateFactor));

        main_warning.high_ie_ratio = system_measure.ie_ratio > (((float)breathing_config.ie_ratio) * (1.0 + kWarningMaximunIeRatioFactor));
        main_warning.low_ie_ratio = system_measure.ie_ratio < (((float)breathing_config.ie_ratio) * (1.0 - kWarningMaximunIeRatioFactor));
      }

      //check pressure on volume control, else reset
      if (breathing_config.is_volume_controled)
      {
        main_warning.high_out_pressure = system_measure.out_pressure > breathing_config.maximun_out_pressure;
        main_warning.low_out_pressure = system_measure.out_pressure < breathing_config.minimun_out_pressure;
      }
      else
      {
        main_warning.high_out_pressure = false;
        main_warning.low_out_pressure = false;
      }

      break;

    case kBreathingOutPause:
      main_warning.low_peep = system_measure.out_pressure < breathing_config.minimun_peep;
      break;

    case kBreathingInPause:
      main_warning.detached_proximal_tube = system_measure.in_pressure < breathing_config.minimun_peep;
      main_warning.detached_proximal_tube |= system_measure.out_pressure < breathing_config.minimun_peep;
      break;

    case kBreathingInCicle:
      //at the end of out cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;

        //check warnings limits
        main_warning.high_breathing_rate = system_measure.breathing_rate > (((float)breathing_config.breathing_rate) * (1.0 + kWarningMaximunBreathingRateFactor));
        main_warning.low_breathing_rate = system_measure.breathing_rate < (((float)breathing_config.breathing_rate) * (1.0 - kWarningMaximunBreathingRateFactor));

        main_warning.high_ie_ratio = system_measure.ie_ratio > (((float)breathing_config.ie_ratio) * (1.0 + kWarningMaximunIeRatioFactor));
        main_warning.low_ie_ratio = system_measure.ie_ratio < (((float)breathing_config.ie_ratio) * (1.0 - kWarningMaximunIeRatioFactor));

        main_warning.high_volume_leakage = system_measure.patient_leakage > breathing_config.maximum_leakage;

        if (breathing_config.is_assisted)
          main_warning.apnea = system_measure.apnea_time >= breathing_config.apnea_time;
        else
          main_warning.apnea = false;
      }

      //check pressure on volume control, else reset
      if (breathing_config.is_volume_controled)
      {
        main_warning.high_in_pressure = system_measure.in_pressure > breathing_config.maximun_in_pressure;
        main_warning.low_in_pressure = system_measure.in_pressure < breathing_config.minimun_in_pressure;
      }
      else
      {
        main_warning.high_in_pressure = false;
        main_warning.low_in_pressure = false;
      }

      break;

    default:

      break;
    }
  }
  else
  {
    //reset breathing warnings
    main_warning.all &= ~kWarningMaskBreathingFlags;
  }
}

void FMSBreathingConfigHW(void)
{
  switch (breathing_state)
  {

  case kBreathingOutPause:

    dbprintf("\nINFO: Enter in Out pause\n");
    if (!breathing_config.is_assisted || breathing_config.trigger_source_pressure)
    {
      //pressure trigger or breathing controlled

      //motor is returning
    }
    else
    {
      //flow trigger

      //move motor at calculated velocity to generate a bias flow in patient circuit tubes
      DriverMotorMoveTo(kMotorIdBellows, kMotorBellowMaxPos);
      DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_flow_bias_vel_bellows);
    }

    DriverMotorMoveTo(kMotorIdAirChoke, breathing_dinamic.motor_position_air_choke);
    DriverMotorMoveTo(kMotorIdO2Choke, breathing_dinamic.motor_position_o2_choke);

    goto config_valves;

  case kBreathingInPause:

    dbprintf("INFO: Enter in In pause\n");
    DriverMotorStop(kMotorIdBellows);
    if (!breathing_config.is_assisted || breathing_config.trigger_source_pressure)
    {
      //pressure trigger or breathing controlled
      //motor starts to stop
    }
    else
    {
      //flow trigger
      //move motor full backward
      DriverMotorMoveTo(kMotorIdBellows, kMotorBellowMinPos);
      DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_return_vel_bellows);
    }

    //hold motor position

    goto config_valves;

  case kBreathingOutCicle:

    dbprintf("INFO: Enter in Out cicle\n");
    if (!breathing_config.is_assisted || breathing_config.trigger_source_pressure)
    {
      //pressure trigger or breathing controlled
      DriverMotorMoveTo(kMotorIdBellows, kMotorBellowMinPos);
      DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_return_vel_bellows);
    }
    else
    {
      //flow trigger
      //motor is returning
    }

    goto config_valves;

  case kBreathingInCicle:

    //move motor forward
    dbprintf("INFO: Enter in In cicle\n");
    if (!breathing_config.is_assisted || breathing_config.trigger_source_pressure)
    {
      //pressure trigger or breathing controlled
      DriverMotorMoveTo(kMotorIdBellows, kMotorBellowMaxPos);
      DriverMotorSetVel(kMotorIdBellows, kMotorDefaultReturnVelBellows); //start with any vel
    }
    else
    {
      //flow trigger
      //motor is already running
    }

    //clear PID control
    ControlWindup(&control_pressure);
    ControlWindup(&control_air_flow);

  config_valves:
    //prepare valves for new state
    DriverValveOpenTo(kValveIdExhalation, breathing_dinamic.valve_position[breathing_state].exhalation); //remember PEEP valve
    DriverValveOpenTo(kValveIdInhalation, breathing_dinamic.valve_position[breathing_state].inhalation);
    DriverValveOpenTo(kValveIdManifold, breathing_dinamic.valve_position[breathing_state].manifold); //to start to full bellows

    break;

  default:
    break;
  }
}

void FMSMainLoop(void)
{

  //init dalay variables
  static uint32_t breathing_state_ref_time;
  static uint8_t any_use_count;

  static FMSDelay main_delay = {0, 0, kMainDelay, (int *)&main_state, kMainDelay};
  static uint32_t main_state_ref_time;

  switch (main_state)
  {

  case kMainInit:

    //----------state actions---------//

    //this estate should bring all actuator to default status or an adecauted position

    switch (init_state)
    {
    case kInitStartFuelBellow:

      dbprintf("INFO: Init procedure started\n");
      //how motor will be in a unknow position, make all posible travel until stop switch is activated
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorChokeMaxPos);  //this is for fuel Bellow
      DriverMotorMoveTo(kMotorIdO2Choke, -kMotorChokeMaxPos);  //full close Choke valve
      DriverMotorMoveTo(kMotorIdBellows, -kMotorBellowMaxPos); //full open Bellow

      //Config default velocities
      DriverMotorSetVel(kMotorIdBellows, kMotorDefaultReturnVelBellows);
      DriverMotorSetVel(kMotorIdAirChoke, kMotorDefaultVelAirChoke);
      DriverMotorSetVel(kMotorIdO2Choke, kMotorDefaultVelO2Choke);

      //close on off valves
      DriverValveOpenTo(kValveIdExhalation, kValveFullClose); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullOpen); //this is for fuel Bellow

      init_state = kInitWaitFuelBellow;
      main_state_ref_time = Millis();
      break;

    case kInitWaitFuelBellow:

      //wait driver motor start
      if (GetDiffTime(Millis(), main_state_ref_time) > 200)
      {
        //check if main motor stops
        if (DriverMotorIsStop(kMotorIdBellows))
        {
          dbprintf("INFO: Init Bellows is full\n");
          init_state = kInitMoveChokeDefault;
          DriverMotorStop(kMotorIdAirChoke);
          DriverMotorMoveTo(kMotorIdAirChoke, -3.0 * kMotorChokeMaxPos);
        }
      }
      break;

    case kInitMoveChokeDefault:

      //bellow is now full open. Make it the zero position
      DriverMotorSetZeroPos(kMotorIdBellows);

      //close bellow inputs
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      init_state = kInitWaitChokeDefault;
      any_use_count = 0;
      main_state_ref_time = Millis();
      break;

    case kInitWaitChokeDefault:

      //wait motor drive has any action on control
      if (GetDiffTime(Millis(), main_state_ref_time) > 200)
      {
        //check if main motor stops
        if (DriverMotorIsStop(kMotorIdAirChoke))
        {
          //if is the firt time, bring air choke to a close position
          if (any_use_count == 0)
          {
            dbprintf("INFO: Init open air input\n");
            DriverMotorMoveTo(kMotorIdAirChoke, kMotorChokeMaxPos); //close air choke valve
            DriverMotorSetZeroPos(kMotorIdAirChoke);
            any_use_count++;
            main_state_ref_time = Millis();
          }
          else
          {
            dbprintf("INFO: Init Wait O2 choke\n");
            init_state = kInitFinalStep;
          }
        }
      }
      break;

    case kInitFinalStep:
      //make sure that valves are full closed
      if (DriverMotorIsStop(kMotorIdO2Choke))
      {
        //DriverMotorSetZeroPos(kMotorIdAirChoke);
        DriverMotorSetZeroPos(kMotorIdO2Choke);
        init_state = kInitStartFuelBellow;
        main_state = kMainIdle; //end init senquence
        dbprintf("INFO: Init procedure finished\n");
      }
      break;
    default:
      break;
    }

    break;

  case kMainIdle:

    //----------state actions---------//

    //make sure that all valves are closed
    DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
    DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
    DriverValveOpenTo(kValveIdManifold, kValveFullClose);

    //compute working parameters
    ComputeParameters();

    //----------transition events---------//

    if (!breathing_config.is_standby)
    {
      dbprintf("INFO: Trying to start breathing mode\n");
      //make sure that all required parameters are initialized;
      if (!main_error.working_configuration_not_initialized) //this is for patient safety
      {
        //breathing_state_ref_time = Millis();
        main_state = kMainBreathing;
        breathing_state = kBreathingOutPause;

        //prepare hardware for start breathing
        dbprintf("INFO: Breathing mode started\n");
        FMSBreathingConfigHW();
      }
      else
      {
        breathing_config.is_standby = true;
        //no parameters were found or valid :(!
        dbprintf("ERROR: no valid parameter, hold idle state\n");
      }
    }
    else if (breathing_config.is_tunning)
    {
      dbprintf("INFO: Tunning mode started\n");
      main_state = kMainTunning;
    }
    else
    {
      //hold state and only review on 500ms after
      FMSDelaySet(&main_delay, 500, kMainIdle);
    }
    //Do not put more code here
    break;

  case kMainTunning:

    //----------state actions---------//

    //TODO: make tunnig actions/code

    //state transition control and transition actions

    //----------transition events---------//
    main_state = kMainIdle;
    breathing_config.is_tunning = false;
    dbprintf("INFO: Tunning finished\n");

    break;

  case kMainBreathing:

    //----------State Actions---------//

    switch (breathing_state)
    {
    case kBreathingOutCicle:

      //----------State Actions---------//

      // bellow is returning to an open state

      //----------transition events---------//

      //when espirarion time ends, change state. Remember, Bellow Motor should be a little more fast
      //that patient espirarion, because it needs to refuel bellows before go to inspiration cicle
      if (GetDiffTime(Millis(), breathing_state_ref_time) >= breathing_dinamic.breathing_out_time)
      {
        breathing_state_ref_time = Millis();
        breathing_state = kBreathingOutPause;
        FMSBreathingConfigHW();
      }

      break;

    case kBreathingOutPause:

      //----------State Actions---------//

      //this state supose Bellows is full of air to start a new cicle

      //----------transition events---------//

      //Just see if ventilator should stop or there is new working parameters.
      //This transition trigger is only analyzed here for patient safety
      ComputeParameters();
      if (breathing_config.is_standby)
      {
        dbprintf("INFO: leaving breathing mode\n");
        main_state = kMainIdle;

        // return to default position
        DriverMotorStop(kMotorIdBellows);
        DriverMotorMoveTo(kMotorIdBellows, kMotorBellowMinPos);
        DriverMotorSetVel(kMotorIdBellows, kMotorDefaultReturnVelBellows);
        break;
      }

      //if out pause ends
      if (GetDiffTime(Millis(), breathing_state_ref_time) >= breathing_dinamic.breathing_out_puase_time)
      {
        dbprintf("INFO: Time trigger detected\n");
        goto kBreathingOutPause_change_state;
      }
      else if (breathing_config.is_assisted)
      {
        //check if it is presure controled
        if (!breathing_config.is_volume_controled && system_measure.in_pressure < breathing_dinamic.sensor_pressure_trigger_ins_value)
        {
          dbprintf("INFO: Patient trigger detected\n");
          goto kBreathingOutPause_change_state;
        }
        //check if volume controled
        else if (breathing_config.is_volume_controled && system_measure.patient_flow > breathing_dinamic.sensor_flow_trigger_ins_value)
        {
          dbprintf("INFO: Patient trigger detected\n");
          goto kBreathingOutPause_change_state;
        }
        else
        {
          //no trigger from patient
        }
      }

      break;

    kBreathingOutPause_change_state:
      breathing_state_ref_time = Millis();
      breathing_state = kBreathingInCicle;
      FMSBreathingConfigHW();
      break;

    case kBreathingInPause:

      //----------State Actions---------//

      //bellow start to stop and hold position

      //---------State transition events ----//

      //if there is a time out
      if (GetDiffTime(Millis(), breathing_state_ref_time) >= breathing_dinamic.breathing_in_puase_time)
      {

        breathing_state_ref_time = Millis();
        breathing_state = kBreathingOutCicle;
        FMSBreathingConfigHW();
      }

      break;

    case kBreathingInCicle:

      //----------State Actions---------//

      //motor is moving foward to full open position
#if 0
      //selection of control type
      if (!breathing_config.is_volume_controled)
      {
        // if it is presure controlled
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_pressure,
                                         (float)(breathing_dinamic.sensor_pressure_ref - system_measure.in_pressure)));
      }
      else
      {
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_air_flow,
                                         (float)(breathing_dinamic.sensor_air_flow_ref - system_measure.patient_flow)));
      }
#else
//be carefull, this is for testing porpuses
      DriverMotorSetVel(kMotorIdBellows, (breathing_dinamic.sensor_air_flow_ref) * kBellowFactor);

#endif
      //----------transition events---------//
      if (GetDiffTime(Millis(), breathing_state_ref_time) >= breathing_dinamic.breathing_in_time)
      {
        breathing_state_ref_time = Millis();
        breathing_state = kBreathingInPause;
        FMSBreathingConfigHW();
      }

      break;

    default:

      dbprintf("ERROR: No valid breathing state\n");

      //this should be generated by a execution error, reset machine state and inform fault
      main_error.breathing_state_fault = true;

#if 0
      breathing_state=kBreathingOutPause;
#else
      main_state = kMainIdle;
#endif

      break;
    }

    //state transition control and transition actions

    break;

  case kMainDelay:

    //----------State Actions---------//

    //----------transition events---------//

    //if time trigger happen, change state
    if ((Millis() - main_delay.ref_time) > main_delay.delay_time)
    {
      main_state_ref_time = Millis();
      main_state = (MainStates)main_delay.next_state;
    }
    break;

  default:

    dbprintf("ERROR: No valid main state\n");

    //there is some extrain error, go to init;
    main_state = kMainInit;

    //inform fault
    main_error.main_state_fault = true;
    break;
  }
}

void FMSDelaySet(FMSDelay *object, uint32_t delay_time, int nex_state)
{
  object->ref_time = Millis();
  object->delay_time = delay_time;
  object->next_state = nex_state;
  *(object->actual_state) = object->delay_state;
}

void FrontEndCommunicationLoop(void)
{
  static DataTimeRef last_update_time = {0, 0};

  WarningType main_warning_masked;

  uint8_t transfer_buffer[kTxBufferLength];
  uint8_t transfer_pointer = 0;
  uint16_t i;
  uint16_t j;
  HicopHeaders rx_flag;
  uint32_t mask = 0x0;

  main_warning_masked.all = main_warning.all & main_warning_mask.all;

  //warnings have a high priority to send to front end MCU
  if (frontend_warning_copy.all != main_warning_masked.all)
  {
    dbprintf("INFO: New Warning info available: ");
    mask = 0b1;
    //build message
    for (i = 0; i < 32; i++) //warning variable is a 32 bits register
    {

      //find which flags changed state
      if ((frontend_warning_copy.all & mask) != (main_warning_masked.all & mask))
      {
        dbprintf("%i,%i ", kTxAlarmId[i], 0 != (main_warning_masked.all & mask));
        transfer_buffer[transfer_pointer++] = kTxAlarmId[i];
        transfer_buffer[transfer_pointer++] = (0 != (main_warning_masked.all & mask));
      }
      mask <<= 1;
    }
    dbprintf("\n\n");
    //save changes
    frontend_warning_copy.all = main_warning_masked.all;

    //send data
    HicopSendData(kHicopHeaderAlarm, transfer_buffer, transfer_pointer);
  }
  //data has lower priority
  else
  {
    //check if it is time to send fast data
    if (GetDiffTime(Millis(), last_update_time.fast_data) >= kFastDataPeriod)
    {
      last_update_time.fast_data = Millis();

      //check if there is an interesting data to send depending of machine state
      if (main_state == kMainBreathing)
      {
        for (i = 0; i < sizeof(kTxFastDataId); i++)
        {
          transfer_buffer[transfer_pointer++] = kTxFastDataId[i];
          for (j = 0; j < sizeof(float); j++)
            transfer_buffer[transfer_pointer++] = (uint8_t)(system_measure.fast_data[i * 4 + j]);
        }
      }
    }
    //check if it is time to send low data
    if (GetDiffTime(Millis(), last_update_time.slow_data) >= kSlowDataPeriod)
    {
      // system_measure.battery_level=50.0;
      //dbprintf("INFO: data send %i\n",(uint8_t) system_measure.battery_level);
      last_update_time.slow_data = Millis();

      for (i = 0; i < sizeof(kTxSlowDataId); i++)
      {
        transfer_buffer[transfer_pointer++] = kTxSlowDataId[i];
        for (j = 0; j < sizeof(float); j++)
          transfer_buffer[transfer_pointer++] = (uint8_t)(system_measure.slow_data[i * 4 + j]);
      }
    }

    //if there is data on transfer_buffer, send it
    if (transfer_pointer != 0)
      HicopSendData(kHicopHeaderData, transfer_buffer, transfer_pointer);
  }

  HicopLoop();

  //read if there some commands
  if (HicopReadData(&rx_flag, transfer_buffer, &transfer_pointer))
  {
    dbprintf("INFO: Hicop new %i data\n", transfer_pointer);
    switch (rx_flag)
    {
    case kHicopHeaderConfig:
      //update parameters
      while (transfer_pointer--)
      {
        breathing_config_request.all[transfer_pointer] = transfer_buffer[transfer_pointer];
      }
      breathing_config_change = true;
      break;

    default:
      //do nothing
      break;
    }
  }
}

//sensor test
/*
uint32_t ref;

void setup()
{
  Serial.begin(115200);
  DirverInitialization();
  ref = Millis();
}

void loop()
{
  int i;
  if (GetDiffTime(Millis(), ref) > 500)
  {
    ref = Millis();
    //Serial.print("flow: ");

    for (i = 0; i < 8; i++)
    {
      Serial.print(SensorGetValue(i));
      Serial.print(" ");
    }
    Serial.print(flow_in.Available());
  Serial.println(flow_out.Available());

  }

  DriverLoops();
}
*/

// Motor testing
/*
void AnyCallback(void);

uint32_t ref = 0;
float ref2 = 0.0;
float ref3 = 0.0;
void setup()
{

  PinInitialization();
  DirverInitialization();
  DriverMotorMoveTo(kMotorIdO2Choke, -6.0);
  DriverMotorSetVel(kMotorIdO2Choke, 0.5);
  DriverMotorMoveTo(kMotorIdAirChoke, -6.0);
  DriverMotorSetVel(kMotorIdAirChoke, 0.5);
  DriverMotorMoveTo(kMotorIdBellows, 80.0);
  DriverMotorSetVel(kMotorIdBellows, 13.0);

  Serial.begin(115200);

  ref = Millis();
  ref3 = 50.0;
  DriverValveOpenTo(kValveIdInhalation, kValveFullOpen);
}

void loop()
{

  if (GetDiffTime(millis(), ref) > 100)
  {
    ref = Millis();
    Serial.print("ok flow: ");
    //Serial.println((DriverMotorActualPos(kMotorIdBellows) - ref2) / 0.1);
    //ref2 = DriverMotorActualPos(kMotorIdBellows);
    Serial.println(SensorGetValue(kSensorIdPressureIn));

    DriverMotorMoveTo(kMotorIdAirChoke, 0.0);
    DriverMotorMoveTo(kMotorIdO2Choke, 0.0);

    if (DriverMotorIsStop(kMotorIdBellows))
    {
      if (ref3 < 1.0)
      {
        DriverValveOpenTo(kValveIdInhalation, kValveFullOpen);
        DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
        ref3 = 50.0;
        DriverMotorMoveTo(kMotorIdBellows, 80.0);
      }
      else
      {
        DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
        DriverValveOpenTo(kValveIdExhalation, kValveFullOpen);
        ref3 = 0.0;
        DriverMotorMoveTo(kMotorIdBellows, 0.0);
      }
    }
  }

  DriverLoops();
}

void AnyCallback(void)
{
  Serial.print(Micros());
  Serial.println(" ok\n");
}
*/
//for comunication layer test
/*
HicopHeaders test_type = kHicopHeaderAlarm;
uint8_t data[] = {0x19,0x1};
uint8_t len = sizeof(data);

void setup()
{
  DirverInitialization();
  HicopInit();
  Delay(1000);
  HicopSendData(test_type,data,len);
}

void loop()
{
  HicopLoop();
 // DriverLoops();
}
*/

//electrovalve test

/*
uint32_t ref = 0;

void setup()
{

  PinInitialization();
  DirverInitialization();

  ref = Millis();

  DriverValveOpenTo(kValveIdManifold, kValveFullClose);
  DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
  DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
}

void loop()
{

  static int valve = 0;

  if (GetDiffTime(Millis(), ref) > 2000)
  {
    ref = Millis();

    switch (valve)
    {
    case 0:
      DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullOpen);
      dbprintf("kValveIdManifold\n");
      valve++;
      break;
    case 1:
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);
      DriverValveOpenTo(kValveIdInhalation, kValveFullOpen);
      dbprintf("kValveIdInhalation\n");
      valve++;
      break;
    case 2:
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdExhalation, kValveFullOpen);
      dbprintf("kValveIdExhalation\n");
      valve = 0;
      break;
    default:
      valve = 0;
      dbprintf("RARO!!\n");
      break;
    }
  }
}
*/