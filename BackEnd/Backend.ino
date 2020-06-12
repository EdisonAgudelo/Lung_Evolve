

#include "definitions.h"
#include "src/hw_lib/basic_function.h"
#include "src/fw_lib/control.h"
#include "src/hw_lib/time.h"

#define HICOP_USE_ARDUINO
#include "src/hicop_lib/hicop.h"

//error variable
static ErrorType main_error;

//warning variable
static WarningType main_warning;          //saves all generated warnings
static WarningType main_warning_mask;     //enable warnings transmission
static WarningType frontend_warning_copy; //to detect changes

//state machine variables
static MainStates main_state;
static BreathingStates breathing_state;

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

//----------Local funcitions---------//

////this function makes all initialization related to breathing FSM
void FMSMainInit(void);

//this function actually is a machine state which only does breathing control action.
//this function needs that someone config working parameters to work porperly
void FMSMainLoop(void);

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
}

void FMSMainInit(void)
{
  main_warning_mask.all = 0xffffffff; //enable all Warning

  //indicate that memory has random data
  main_error.working_configuration_not_initialized = true;

  //variable inizialitacion
  main_state = kMainInit;               //start in idle mode
  breathing_state = kBreathingOutPause; //default state

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

void loop()
{
  MeasureVariables();
  WarningMonitor();
  WarningActions();
  FMSMainLoop();
  DriverLoops();
  FrontEndCommunicationLoop();
}

void ComputeParameters(void)
{
  uint8_t i;

  //this temporal variables is used to make first caculos a check if time result are sound.
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

    //////////////here is where config parameters request is proecessed///////////////

    //transfer flags
    breathing_config.all[0] = breathing_config_request.all[0];

    //calculate new breathing dinamics
    breathing_in_puase_time = ((int32_t)breathing_config_request.pause_time) * 1000;
    breathing_in_time = 60000 / (((int32_t)breathing_config_request.breathing_rate) * (((int32_t)breathing_config_request.ie_ratio) + 1));
    breathing_out_puase_time = ((int32_t)breathing_config_request.apnea_time) * 1000;
    breathing_out_time = (60000 * ((int32_t)breathing_config_request.ie_ratio)) / (((int32_t)breathing_config_request.breathing_rate) * (((int32_t)breathing_config_request.ie_ratio) + 1));

    breathing_out_time -= breathing_out_puase_time;

    if (breathing_out_time <= 0)
    {
      //calculus are no valid. End execution
      main_error.working_configuration_not_initialized = true;
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
        return;
      }
    }
    else
    {
      //breathing_dinamic.breathing_in_puase_time
      //is used to determine maximum pause time, so it no should be used on inspiration  time calculation
    }

    //transfer all new parameters
    for (i = 1; i < sizeof(breathing_config.all); i++)
      breathing_config.all[i] = breathing_config_request.all[i];

    // transfer main calculations
    breathing_dinamic.breathing_in_puase_time = breathing_in_puase_time;
    breathing_dinamic.breathing_in_time = breathing_in_time;
    breathing_dinamic.breathing_out_puase_time = breathing_out_puase_time;
    breathing_dinamic.breathing_out_time = breathing_out_time;

    //check other data

    //in the worst case, motors should travel all available distance
    breathing_dinamic.motor_return_vel_bellows = (kMotorMaxPos * 1000.0) / (((float)breathing_dinamic.breathing_out_time) * 0.9); //mm/s

    if (breathing_config.is_volume_controled)
      breathing_dinamic.sensor_air_flow_ref = breathing_config.volume_tidal;
    else
      breathing_dinamic.sensor_pressure_ref = breathing_config.in_presure;

    //espiration trigger is no supported rigth now

    breathing_dinamic.sensor_pressure_trigger_ins_value = (float)breathing_config.minimun_peep - (float)breathing_config.trigger_value;
    //breathing_dinamic.sensor_pressure_trigger_esp_value;
    breathing_dinamic.sensor_flow_trigger_ins_value = (float)breathing_config.trigger_value; //flow deviation
    //breathing_dinamic.sensor_flow_trigger_esp_value;

    //----------review----------//

    breathing_dinamic.motor_position_o2_choke = ((float)breathing_config.FiO2 - kMinimunFiO2) * kConstantO2Choke;   //valve position to control O2 flow through pneumatic system
    breathing_dinamic.motor_position_air_choke = ((float)breathing_config.FiO2 - kMinimunFiO2) * kConstantAirChoke; //valve position to control O2 flow through pneumatic system

    //--------------------------//

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

  //only Measure breathing variables if ventilator is breathing mode
  if (main_state == kMainBreathing)
  {
    //always save presure measure, and flow data
    system_measure.in_pressure = SensorGetValue(kSensorIdPressureIn);
    system_measure.out_pressure = SensorGetValue(kSensorIdPressureOut);
    system_measure.mixture_flow = SensorGetValue(kSensorIdAirFlowMixture);
    system_measure.patient_flow = SensorGetValue(kSensorIdAirFlowIn) + SensorGetValue(kSensorIdAirFlowOut);

    switch (breathing_state)
    {

    case kBreathingOutCicle:
      //at the end of in cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;
        last_inspiration_ref_time = Millis();
        inspiration_time = last_inspiration_ref_time - last_espiration_ref_time;

        //measure
        system_measure.ie_ratio = (espiration_time) / inspiration_time;
        system_measure.breathing_rate = 60000 / (espiration_time + inspiration_time);

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
        espiration_time = last_espiration_ref_time - last_inspiration_ref_time;

        //measure
        system_measure.ie_ratio = (espiration_time) / inspiration_time;
        system_measure.breathing_rate = 60000 / (espiration_time + inspiration_time);

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

  //if there are warnings, turn on buzzer
  if (main_warning.all != 0)
  {
    DriverLedTShoot(&buzzer, 2000, 100);
  }

  if (main_warning.high_in_pressure)
  {
    DriverLedNBlink(&led_red, 2000, 3);
  }
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
          main_warning.apnea = system_measure.apnea_time > breathing_config.apnea_time;
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

void FMSMainLoop(void)
{

  //init dalay variables
  static FMSDelay breathing_delay = {0, 0, kBreathingDelay, (int *)&breathing_state, kBreathingDelay};
  static uint32_t breathing_state_ref_time;

  static FMSDelay main_delay = {0, 0, kMainDelay, (int *)&main_state, kMainDelay};
  static uint32_t main_state_ref_time;

  switch (main_state)
  {

  case kMainInit:

    //----------state actions---------//

    //this estate shuld bring all actuator to default status or an adecauted position
    DriverMotorMoveTo(kMotorIdAirChoke, kMotorMaxPos); //this is for fuel Bellow
    DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
    DriverMotorMoveTo(kMotorIdBellows, kMotorMinPos);

    //Config default velocities
    DriverMotorSetVel(kMotorIdBellows, kMotorDefaultReturnVelBellows);
    DriverMotorSetVel(kMotorIdAirChoke, kMotorDefaultVelAirChoke);
    DriverMotorSetVel(kMotorIdO2Choke, kMotorDefaultVelO2Choke);

    //close on off valves
    DriverValveOpenTo(kValveIdExhalation, kValveFullClose); //remember PEEP valve
    DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
    DriverValveOpenTo(kValveIdManifold, kValveFullOpen); //this is for fuel Bellow

    //----------transition events---------//

    main_state = kMainIdle;

    break;

  case kMainIdle:

    //----------state actions---------//

    //wait for fully open bellows
    if (DriverMotorActualPos(kMotorIdBellows) == kMotorMinPos)
    {
      //close inputs
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);
    }

    //compute working parameters
    ComputeParameters();

    //----------transition events---------//

    if (!breathing_config.is_standby)
    {

      //make sure that all required parameters are initialized;
      if (!main_error.working_configuration_not_initialized) //this is for patient safety
      {
        //breathing_state_ref_time = Millis();
        main_state = kMainBreathing;
        breathing_state = kBreathingOutPause;

        //reset control energy
        ControlWindup(&control_pressure);
        ControlWindup(&control_air_flow);
      }
      else
      {
        //no parameters were found or valid :(!
      }
    }

    else if (breathing_config.is_tunning)
    {

      main_state = kMainTunning;
    }
    else
    {
      //hold state
      FMSDelaySet(&main_delay, 1000, kMainIdle);
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

    break;

  case kMainBreathing:

    //----------State Actions---------//

    switch (breathing_state)
    {
    case kBreathingOutCicle:

      //----------State Actions---------//

      DriverValveOpenTo(kValveIdExhalation, kValveFullOpen); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullOpen); //to start to full bellows

      //full bellows of o2 and air
      DriverMotorMoveTo(kMotorIdBellows, kMotorMinPos);
      DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_return_vel_bellows);

#if 0
      //close oxigen valve in first period
      if ((Millis() - breathing_state_ref_time) < breathing_dinamic.motor_open_time_air_choke)
      {
        DriverMotorMoveTo(kMotorIdAirChoke, breathing_dinamic.motor_position_air_choke);
        DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      }
      //close air valve in second period
      else if ((Millis() - breathing_state_ref_time) < breathing_dinamic.motor_open_time_o2_choke)
      {
        DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
        DriverMotorMoveTo(kMotorIdO2Choke, breathing_dinamic.motor_position_o2_choke);
      }
      //close all valves and wait for timeout. Technically, this case never happen
      else
      {
        DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
        DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      }
#else
      DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
#endif

      //----------transition events---------//

      //when espirarion time ends, change state. Remember, this should be a little more fast
      //that patient espirarion, because it needs to refuel bellows before go to inspiration cicle
      if ((Millis() - breathing_state_ref_time) >= breathing_dinamic.breathing_out_time)
      {
        breathing_state = kBreathingOutPause;
      }

      break;

    case kBreathingOutPause:

      //----------State Actions---------//

      //this state supose Bellows is full of air to start a new cicle

      DriverValveOpenTo(kValveIdExhalation, kValveFullOpen); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //bring  valve motors to closed postion
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);

      //----------transition events---------//

      //just see if ventilator should stop. This transition triggers
      //is only analyzed here for patient safety

      //compute working parameters and check if there is new working parameters
      ComputeParameters();
      if (breathing_config.is_standby)
      {
        DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
        main_state = kMainIdle;
      }

      //enter to out puase delay and then jump to in cicle
      FMSDelaySet(&breathing_delay, breathing_dinamic.breathing_out_puase_time, kBreathingInCicle);

      break;

    case kBreathingInPause:

      //----------State Actions---------//

      DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //bring  valve motors to closed postion
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      //hold Blows position
      //DriverMotorMoveTo(kMotorIdBellows, DriverMotorActualPos(kMotorIdBellows));
      DriverMotorSetVel(kMotorIdBellows, 0.0);

      //enter to in puase delay and then jump to in cicle
      FMSDelaySet(&breathing_delay, breathing_dinamic.breathing_in_puase_time, kBreathingOutCicle);

      break;

    case kBreathingInCicle:

      //----------State Actions---------//

      //open necesary valves
      DriverValveOpenTo(kValveIdExhalation, kValveFullClose); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullOpen);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //selection of control type
      if (!breathing_config.is_volume_controled)
      {
        //presure controlled

        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_pressure,
                                         ((float)(breathing_dinamic.sensor_pressure_ref - system_measure.in_pressure)) / 1000.0));
      }
      else
      {
        //flow controled

        //version 1
#if 1
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_air_flow,
                                         ((float)(breathing_dinamic.sensor_air_flow_ref - system_measure.mixture_flow)) / 10.0));
#else
        //version 2
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_foward_const_flow_vel_bellows);
#endif
      }

      //----------transition events---------//
      if ((Millis() - breathing_state_ref_time) >= breathing_dinamic.breathing_in_time)
      {
        breathing_state = kBreathingInPause;
      }

      break;

    case kBreathingDelay:

      //----------State Actions---------//

      //----------transition events---------//

      //if time trigger happen, change state
      if ((Millis() - breathing_delay.ref_time) > breathing_delay.delay_time)
      {
        breathing_state_ref_time = Millis();
        breathing_state = (BreathingStates)breathing_delay.next_state;
        break;
      }

      //watch patient triggers
      if (breathing_config.is_assisted)
      {
        switch (breathing_delay.next_state)
        {
        case kBreathingInCicle:
          if (!breathing_config.is_volume_controled && system_measure.in_pressure < breathing_dinamic.sensor_pressure_trigger_ins_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else if (breathing_config.is_volume_controled && system_measure.mixture_flow > breathing_dinamic.sensor_flow_trigger_ins_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else
          {
            //no trigger from patient
          }

          break;
        case kBreathingOutCicle:
          /*
          if (!breathing_config.is_volume_controled && system_measure.out_pressure > breathing_dinamic.sensor_pressure_trigger_esp_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          //this case should be tested
#if 0
          else if (breathing_config.is_volume_controled &&  system_measure.mixture_flow > breathing_dinamic.sensor_flow_trigger_esp_value)
          {
            breathing_state = breathing_delay.next_state;
          }
#endif
          else
          {
            //no trigger from patient
          }
          */
          break;

        default:
          break;
        }
      }

      break;

    default:

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
  HicopHeaders rx_flag;

  main_warning_masked.all = main_warning.all & main_warning_mask.all;

  //warnings have a high priority to send to front end MCU
  if (frontend_warning_copy.all != main_warning_masked.all)
  {
    //build message
    for (i = 0; i < 32; i++) //warning variable is a 32 bits register
    {
      //find which flags changed state
      if (frontend_warning_copy.bits[i] != main_warning_masked.bits[i])
      {
        transfer_buffer[transfer_pointer++] = kTxAlarmId[i];
        transfer_buffer[transfer_pointer++] = main_warning_masked.bits[i];
      }
    }
    //save changes
    frontend_warning_copy.all = main_warning_masked.all;

    //send data
    HicopSendData(kHicopHeaderAlarm, transfer_buffer, transfer_pointer);
  }
  //data has lowe priority
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
          transfer_buffer[transfer_pointer++] = (uint8_t)(system_measure.fast_data[i] / 1000);
        }
      }
    }
    //check if it is time to send low data
    if (GetDiffTime(Millis(), last_update_time.slow_data) >= kSlowDataPeriod)
    {
      last_update_time.slow_data = Millis();

      for (i = 0; i < sizeof(kTxSlowDataId); i++)
      {
        transfer_buffer[transfer_pointer++] = kTxSlowDataId[i];
        transfer_buffer[transfer_pointer++] = (uint8_t)(system_measure.slow_data[i] / 1000);
      }
    }

    if (transfer_pointer != 0)
      HicopSendData(kHicopHeaderData, transfer_buffer, transfer_pointer);
  }

  HicopLoop();

  //read if there some commands
  if (HicopReadData(&rx_flag, transfer_buffer, &transfer_pointer))
  {
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

/* //sensor test
uint32_t ref;

void setup()
{
  Serial.begin(115200);
  DirverInitialization();
  ref=Millis();
  
}

void loop()
{
  if(GetDiffTime(Millis(),ref)>100)
  {
    ref = Millis();
    Serial.print("pressure: ");
    Serial.println(SensorGetValue(0));
  }

  DriverLoops();
}
*/

// Motor testing

/*
void AnyCallback(void);

uint32_t ref =0 ;
float ref2 =0.0;
void setup()
{
  
  //DirverInitialization();
  
  PinInitialization();
  DirverInitialization();
  DriverMotorMoveTo(kMotorIdBellows, 50.0);
  DriverMotorSetVel(kMotorIdBellows, 50.0);
  
  Serial.begin(115200);

  //PWMConfigFrecuency(1, kHardwarePWMMotor1);

  ref = Millis();
}


void loop()
{
  
  if(GetDiffTime(Millis(),ref)>100)
  {
    ref = Millis();
    Serial.print("ok vel: ");
    Serial.println((DriverMotorActualPos(kMotorIdBellows)-ref2)/0.1);
    //Serial.println(DriverMotorActualPos(kMotorIdBellows));
    ref2 = DriverMotorActualPos(kMotorIdBellows);
    DriverMotorMoveTo(kMotorIdBellows, 0.0);
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
uint8_t data[] = {0xff,0xfe,0x40,0x3,0x0};
uint8_t len = sizeof(data);

void setup()
{
  DirverInitialization();
  HicopInit();
  HicopSendData(test_type,data,len);
}

void loop()
{
  HicopLoop();
 // DriverLoops();
}
*/