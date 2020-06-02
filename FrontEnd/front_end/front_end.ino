#include "src/screen_manager/screen_manager.h"
#include "src/hardware/hardware_front.h"
#include "src/alarm_manager/alarm_manager.h"
#include "src/backend_manager/backend_manager.h"




void setup()
{
  serial_screen_init();
  serial_backend_init();
	init_hardware_front();
  init_screen_management();
}

void loop()
{
  screen_management();
  backend_management();
  alarm_management_loop();
}