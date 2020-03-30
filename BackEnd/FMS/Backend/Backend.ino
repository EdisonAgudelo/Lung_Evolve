

#include "definitions.h"
#include "basic_function.h"

static MainStates main_state;
static ErrorType main_error;
static BreathingParameters breathing_config =
    {
        .is_tunning = false,
        .is_standby = true,
        .is_pressure_controled = true,
};

void setup()
{

  //variable inizialitacion
  main_state = kMainInit; //start in idle mode
  main_error.all = 0;     //reset all errors;

  //hardware initialization
  main_error.init_hardware = !PinInitialization();
}

void loop()
{
  // put your main code here, to run repeatedly:

  switch (main_state)
  {

  case kMainInit:
    //this estate shuld bring all actuator to default status
    DriverMotorMoveTo(kMotorIdAirChoke, 0);
    DriverMotorMoveTo(kMotorIdO2Choke, 0);
    DriverMotorMoveTo(kMotorIdBellows, 0);

    DriverValveClose(kValveExhalation);
    DriverValveClose(kValveIdInhalation);
    DriverValveClose(kValveIdManifold);

    main_state = kMainIdle;

    break;

  case kMainIdle:

    //state action

    //state transition control and transition actions
    if (breathing_config.is_standby)
    {
      main_state = kMainBreathing;
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

    //state transition control and transition actions

    break;
  default:

    //there ares some extrain error, go to init;
    main_state=kMainInit;

    //inform fault
    main_error.main_state_fault=true;
    break;
  }
}
