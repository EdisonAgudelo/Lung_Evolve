
#include <Arduino.h>
#include "definitions.h"
#include "basic_function.h"
#include "control.h"

//error variable
static ErrorType main_error;

//warning variable
static WarningType main_warning;

//state machine variables
static MainStates main_state;
static BreathingStates breathing_state;

//breathing config variables
static BreathingParameters breathing_config =
    {
        .is_tunning = false,
        .is_standby = true,
        .is_pressure_controled = true,
        .is_assisted = false,
};

//breathing on run parameters (more specific config)
static BreathingDinamics breathing_dinamic;

//working measurenmet
static MeasureType breathing_measure;

//control variables
static ControlData control_pressure;
static ControlData control_air_flow;

//----------Local funcitions---------//

//this function actually is a machine state which only does breathing control action.
//this functions needs that someone config working parameters to work porperly
void FMSMainLoop(void);

//this funcion measure o calculte all physical variables
void MeasureVariables(void);

//this funcition watch all important variables and makes warnings through WarningType varible which is
//a grup o flags. This function takes sensor instat values and compare with breathing_config thresholds but it doesn't
//take any action in respect
void WarningMonitor(void);

//this function Takes warning flags and does some kind of acctions, also it does a better flag proecessing
void WarningActions(void);

//this function analyzes all config parameters to see if they are valid. if there are valid config, then calculte working parameters
void ComputeParameters(void);

void setup()
{

  //variable inizialitacion
  main_state = kMainInit; //start in idle mode
  breathing_state = kBreathingOutPause;

  main_error.all = 0; //reset all errors;
  main_error.working_configuration_not_initialized = true;

  //reset all warnings
  main_warning.all = 0;

  //hardware initialization
  main_error.init_hardware = !PinInitialization();

  //driver initialization
  main_error.init_driver = !DirverInitialization();

  //set control parameters
  ControlInit(&control_pressure, 1.0, 0.0, 0.0, -1.0);
  ControlInit(&control_air_flow, 1.0, 0.0, 0.0, -1.0);
}

void loop()
{
  MeasureVariables();
  WarningMonitor();
  WarningActions();
  FMSMainLoop();
  DriverLoops();
}










void ComputeParameters(void)
{

  //TODO: make necesary actions to calculte breathing_dinamic parameters
  //if somo error config exist
  //     main_error.working_configuration_not_initialized = true;

  main_error.working_configuration_not_initialized = false;
}

void MeasureVariables(void)
{

  // time varibles
  static uint32_t last_inspiration_ref_time = 0;
  static uint32_t last_espiration_ref_time = 0;
  static uint32_t last_dt_ref_time = 0;

  static uint32_t inspiration_time = 1000; //to avoid 0 division
  static uint32_t espiration_time = 2000;
  static uint32_t dt_time = 0;

  //other type variables
  static BreathingStates breathing_previous_state = breathing_state;
  static uint32_t integer_tidal = 0;

  //only MeasureVariables all varibles if ventilator is normally working
  if (main_state == kMainBreathing)
  {
     //always save presure measure, and flow data
    breathing_measure.in_pressure = SensorGetValue(kSensorIdInPressure);
    breathing_measure.out_pressure = SensorGetValue(kSensorIdOutPressure);
    breathing_measure.mixture_flow = SensorGetValue(kSensorIdAirFlow);

    switch (breathing_state)
    {

    case kBreathingOutCicle:
      //at the end of in cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;
        last_inspiration_ref_time = millis();
        inspiration_time = last_inspiration_ref_time - last_espiration_ref_time;

        //measure
        breathing_measure.ie_ratio = (espiration_time * 1000) / inspiration_time;
        breathing_measure.breathing_rate = 60000000 / (espiration_time + inspiration_time);
        breathing_measure.tidal = integer_tidal / inspiration_time;
      }
      break;

    case kBreathingInCicle:
      //at the end of out cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;
        last_espiration_ref_time = millis();
        espiration_time = last_espiration_ref_time - last_inspiration_ref_time;

        //measure
        breathing_measure.ie_ratio = (espiration_time * 1000) / inspiration_time;
        breathing_measure.breathing_rate = 60000000 / (espiration_time + inspiration_time);

        last_dt_ref_time = millis(); //reset dt;
        integer_tidal = 0;           //reset integer
      }

      //integer flow
      dt_time = millis() - last_dt_ref_time;
      last_dt_ref_time = millis();
      integer_tidal +=  breathing_measure.mixture_flow * dt_time;

      break;

    default:

      break;
    }
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

  //only check warnings if ventilator is working
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

        main_warning.high_in_volume_tidal = breathing_measure.tidal > breathing_config.maximun_volume_tidal ? true : false;
        main_warning.low_in_volume_tidal = breathing_measure.tidal < breathing_config.minimun_volume_tidal ? true : false;

        main_warning.high_breathing_rate = breathing_measure.breathing_rate > breathing_config.breathing_rate * (1.0 + kMaximun_deviation_breathing_rate) ? true : false;
        main_warning.low_breathing_rate = breathing_measure.breathing_rate < breathing_config.breathing_rate * (1.0 - kMaximun_deviation_breathing_rate) ? true : false;

        main_warning.high_ie_ratio = breathing_measure.ie_ratio > breathing_config.ie_ratio * (1.0 + kMaximun_deviation_ie_ratio) ? true : false;
        main_warning.low_ie_ratio = breathing_measure.ie_ratio < breathing_config.ie_ratio * (1.0 - kMaximun_deviation_ie_ratio) ? true : false;
      }
      break;

    case kBreathingInCicle:
      //at the end of out cicle
      if (breathing_previous_state != breathing_state)
      {
        breathing_previous_state = breathing_state;

        //check warnings limits
        main_warning.high_breathing_rate = breathing_measure.breathing_rate > breathing_config.breathing_rate * (1.0 + kMaximun_deviation_breathing_rate) ? true : false;
        main_warning.low_breathing_rate = breathing_measure.breathing_rate < breathing_config.breathing_rate * (1.0 - kMaximun_deviation_breathing_rate) ? true : false;

        main_warning.high_ie_ratio = breathing_measure.ie_ratio > breathing_config.ie_ratio * (1.0 + kMaximun_deviation_ie_ratio) ? true : false;
        main_warning.low_ie_ratio = breathing_measure.ie_ratio < breathing_config.ie_ratio * (1.0 - kMaximun_deviation_ie_ratio) ? true : false;
      }

      break;

    default:

      break;
    }

    // only analize warnings in respective cicle
    if (breathing_previous_state == kBreathingOutCicle)
    {
      main_warning.high_out_pressure = breathing_measure.out_pressure > breathing_config.maximun_out_pressure ? true : false;
      main_warning.low_out_pressure = breathing_measure.out_pressure < breathing_config.minimun_out_pressure ? true : false;
    }
    else //(breathing_previous_state == kBreathinginCicle)
    {
      //check warnings
      main_warning.high_in_pressure = breathing_measure.in_pressure > breathing_config.maximun_in_pressure ? true : false;
      main_warning.low_in_pressure = breathing_measure.in_pressure < breathing_config.minimun_in_pressure ? true : false;
    }
  }
  else
  {
    //reset warnings
    main_warning.all = 0;
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

    //----------transition events---------//

    if (!breathing_config.is_standby)
    {
      //compute working parameters
      ComputeParameters();

      //make sure that all required parameters are initialized;
      if (!main_error.working_configuration_not_initialized) //this is for patient safety
      {
        //breathing_state_ref_time = millis();
        main_state = kMainBreathing;
        breathing_state = kBreathingOutPause;

        //reset control energy
        ControlWindup(&control_pressure);
        ControlWindup(&control_air_flow);
      }
      else
      {
        //no parameters were found :(!
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

      //close oxigen valve in first period
      if ((millis() - breathing_state_ref_time) < breathing_dinamic.motor_open_time_air_choke)
      {
        DriverMotorMoveTo(kMotorIdAirChoke, breathing_dinamic.motor_position_air_choke);
        DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      }
      //close air valve in second period
      else if ((millis() - breathing_state_ref_time) < breathing_dinamic.motor_open_time_o2_choke)
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

      //----------transition events---------//

      //when espirarion time ends, change state. Remember, this should be a little more fast
      //that patient espirarion, because it needs to refuel bellows before go to inspiration cicle
      if ((millis() - breathing_state_ref_time) >= breathing_dinamic.breathing_out_time)
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
      DriverMotorMoveTo(kMotorIdBellows, DriverMotorActualPos(kMotorIdBellows));

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
      if (breathing_config.is_pressure_controled)
      {
        //presure controlled

        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_pressure,
                                         ((float)(breathing_dinamic.sensor_pressure_ref -  breathing_measure.in_pressure)) / 1000.0));
      }
      else
      {
        //flow controled

        //version 1
#if 0
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_air_flow,
                                         ((float)(breathing_dinamic.sensor_air_flow_ref -  breathing_measure.mixture_flow)) / 10.0));
#else
        //version 2
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_foward_const_flow_vel_bellows);
#endif
      }

      //----------transition events---------//
      if ((millis() - breathing_state_ref_time) >= breathing_dinamic.breathing_in_time)
      {
        breathing_state = kBreathingInPause;
      }

      break;

    case kBreathingDelay:

      //----------State Actions---------//

      //----------transition events---------//

      //if time trigger happen, change state
      if ((millis() - breathing_delay.ref_time) > breathing_delay.delay_time)
      {
        breathing_state_ref_time = millis();
        breathing_state = (BreathingStates)breathing_delay.next_state;
        break;
      }

      //watch patient triggers
      if (breathing_config.is_assisted)
      {
        switch (breathing_delay.next_state)
        {
        case kBreathingInCicle:
          if (breathing_config.is_pressure_controled &&  breathing_measure.in_pressure < breathing_dinamic.sensor_pressure_trigger_ins_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else if (!breathing_config.is_pressure_controled &&  breathing_measure.mixture_flow > breathing_dinamic.sensor_flow_trigger_ins_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else
          {
            //no trigger from patient
          }

          break;
        case kBreathingOutCicle:

          if (breathing_config.is_pressure_controled &&  breathing_measure.out_pressure > breathing_dinamic.sensor_pressure_trigger_esp_value)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          //this case should be tested
#if 0
          else if (!breathing_config.is_pressure_controled &&  breathing_measure.mixture_flow > breathing_dinamic.sensor_flow_trigger_esp_value)
          {
            breathing_state = breathing_delay.next_state;
          }
#endif
          else
          {
            //no trigger from patient
          }
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
    if ((millis() - main_delay.ref_time) > main_delay.delay_time)
    {
      main_state_ref_time = millis();
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
  object->ref_time = millis();
  object->delay_time = delay_time;
  object->next_state = nex_state;
  *(object->actual_state) = object->delay_state;
}
