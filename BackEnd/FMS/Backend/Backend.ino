

#include "definitions.h"
#include "basic_function.h"

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
};

static BreathingDinamics breathing_dinamic;

//FMS declaration
void FMSMainLoop(void);

void setup()
{

  //variable inizialitacion
  main_state = kMainInit; //start in idle mode
  breathing_state = kBreathingOutPause;

  main_error.all = 0; //reset all errors;

  //hardware initialization
  main_error.init_hardware = !PinInitialization();
}

void loop()
{
  FMSMainLoop();
}

void FMSMainLoop(void)
{
  static FMSDelay breathing_delay = {0, 0, kBreathingDelay, &breathing_state, kBreathingDelay};
  static uint32_t breathing_state_ref_time;
  // put your main code here, to run repeatedly:

  switch (main_state)
  {

  case kMainInit:
    //this estate shuld bring all actuator to default status
    DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
    DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
    DriverMotorMoveTo(kMotorIdBellows, kMotorMinPos);

    DriverValveOpenTo(kValveIdExhalation, kValveFullClose); //remember PEEP valve
    DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
    DriverValveOpenTo(kValveIdManifold, kValveFullClose);

    main_state = kMainIdle;

    break;

  case kMainIdle:

    //state action

    //state transition control and transition actions
    if (!breathing_config.is_standby)
    {
      breathing_state_ref_time = millis();
      main_state = kMainBreathing;
      breathing_state = kBreathingOutPause;
    }
    else if (breathing_config.is_tunning)
    {
      main_state = kMainTunning;
    }
    else
    {
      //hold state
    }

    break;

  case kMainTunning:
    //state action

    //state transition control and transition actions

    break;

  case kMainBreathing:
    //state action

    switch (breathing_state)
    {
    case kBreathingOutCicle:

      DriverValveOpenTo(kValveIdExhalation, kValveFullOpen); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullOpen); //to start to full bellows

      //full cavity of o2 and air
      DriverMotorMoveTo(kMotorIdBellows, kMotorMinPos);
      DriverMotorSetVel(kMotorIdBellows, breathing_dinamic.motor_bellows_return_vel);

      //close valve of oxigen in first period
      if ((millis() - breathing_state_ref_time) < breathing_dinamic.motor_air_choke_open_time)
      {
        DriverMotorMoveTo(kMotorIdAirChoke, breathing_dinamic.motor_air_choke_position);
        DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      }
      else
      {
        DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
        DriverMotorMoveTo(kMotorIdO2Choke, breathing_dinamic.motor_o2_choke_position);
      }

      //when espirarion time ends, change state
      if ((millis() - breathing_state_ref_time) >= breathing_dinamic.breathing_out_time)
      {
        breathing_state = kBreathingOutPause;
      }

      break;
    case kBreathingOutPause:
      //this state supose Bellows is full of air to start a new cicle

      DriverValveOpenTo(kValveIdExhalation, kValveFullOpen); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //bring  valve motors to closed postion
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);

      //enter to out puase delay and then jump to in cicle
      FMSDelaySet(&breathing_delay, breathing_dinamic.breathing_out_puase_time, kBreathingInCicle);

      break;

    case kBreathingInPause:

      DriverValveOpenTo(kValveIdExhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdInhalation, kValveFullClose);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //bring  valve motors to closed postion
      DriverMotorMoveTo(kMotorIdAirChoke, kMotorMinPos);
      DriverMotorMoveTo(kMotorIdO2Choke, kMotorMinPos);
      //hold Blows position 


      //enter to in puase delay and then jump to in cicle
      FMSDelaySet(&breathing_delay, breathing_dinamic.breathing_in_puase_time, kBreathingOutCicle);

      break;

    case kBreathingInCicle:

      DriverValveOpenTo(kValveIdExhalation, kValveFullClose); //remember PEEP valve
      DriverValveOpenTo(kValveIdInhalation, kValveFullOpen);
      DriverValveOpenTo(kValveIdManifold, kValveFullClose);

      //selection of control type
      if(breathing_config.is_pressure_controled){
        //presure controlled

      }
      else
      {
        //flow controled
        
      }


      break;

    case kBreathingDelay:
      if ((millis() - breathing_delay.ref_time) > breathing_delay.delay_time)
      {
        breathing_state_ref_time = millis();
        breathing_state = breathing_delay.next_state;
      }
      break;

    default:
      //this should be generated by a execution error, reset machine state and inform fault
      break;
    }

    //state transition control and transition actions

    break;
  default:

    //there ares some extrain error, go to init;
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