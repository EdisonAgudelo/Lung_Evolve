//this library is focused to supose external functions that should
//exist o must be created. This functions are oriented to hardware 
//interface and are the basic contructions brinks.


#ifndef _BASIC_FUNCTION_
#define _BASIC_FUNCTION_

  //this function bring a especific motor to a know position
  //return 0 is no succes o 1 if action was totaly complete
  bool DriverMotorMoveTo(int motor_id, int line_pos);

  //this function initialize all develpment board GPIO
  //return 0 if there are some error else return 1
  bool PinInitialization(void);
  
  //this function manage all on off valves
  //return 0 if there are some error else return 1
  bool DriverValveClose(int valve_id);
  

  

#endif
