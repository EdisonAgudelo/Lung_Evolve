

#include "definitions.h"
#include "basic_function.h"
#include "control.h"

//error variable
static ErrorType main_error;

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

static BreathingDinamics breathing_dinamic;

//control variables
static ControlData control_pressure;
static ControlData control_air_flow;

//function declaration
//FMS declaration
void FMSMainLoop(void);

void setup()
{

  //variable inizialitacion
  main_state = kMainInit; //start in idle mode
  breathing_state = kBreathingOutPause;

  main_error.all = 0; //reset all errors;
  main_error.working_configuration_not_initialized = true;

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
  FMSMainLoop();
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
      //that patient espirarion, because it need to refuel bellows before go to inspiration
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
                          ControlExecute(&control_air_flow,
                                         ((float)(breathing_dinamic.sensor_pressure_ref - SensorGetValue(kSensorIdInPressure))) / 1000.0));
      }
      else
      {
//flow controled

//version 1
#if 0
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows,
                          ControlExecute(&control_air_flow,
                                         ((float)(breathing_dinamic.sensor_air_flow_ref - SensorGetValue(kSensorIdAirFlow))) / 10.0));
#endif
        //version 2
        DriverMotorMoveTo(kMotorIdBellows, kMotorMaxPos);
        DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_foward_const_flow_vel_bellows);
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
      }

      //watch patient triggers
      if (breathing_config.is_assisted)
      {
        switch (breathing_delay.next_state)
        {
        case kBreathingInCicle:
          if (SensorGetValue(kSensorIdInPressure) < breathing_dinamic.sensor_pressure_trigger_ins_value && breathing_config.is_pressure_controled)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else if (SensorGetValue(kSensorIdAirFlow) > breathing_dinamic.sensor_flow_trigger_ins_value && !breathing_config.is_pressure_controled)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          else
          {
            //no trigger from patient
          }

          break;
        case kBreathingOutCicle:

          if (SensorGetValue(kSensorIdOutPressure) > breathing_dinamic.sensor_pressure_trigger_esp_value && breathing_config.is_pressure_controled)
          {
            breathing_state = (BreathingStates)breathing_delay.next_state;
          }
          //this case should be tested
#if 0
          else if (SensorGetValue(kSensorIdAirFlow) > breathing_dinamic.sensor_flow_trigger_esp_value && !breathing_config.is_pressure_controled)
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