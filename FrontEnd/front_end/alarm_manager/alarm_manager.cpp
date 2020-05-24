//leer del bus de datos
//activar alarmas segun corresponda
//manejo de perifericos

#include "alarm_manager.h"
#include "data_bus/data_bus.h"
#include "hardware/hardware_front.h"

extern struct ALARMS alarms_state_struct;

void alarm_management(void)
{
///////////// HIGH PRIORITY ALARMS//////////
    if(alarms_state_struct.LowInspP==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.HighInspP==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.ProximalTube==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.VteNotAchived==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.VteOv==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.PatientLeaks==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.ShutDown==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.LowBattery==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.NoBattery==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.HighTemp==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }

    if(alarms_state_struct.NoOxygen==true)
    {
        REDLed(true);
    }else{
        REDLed(false);
    }
/////////////MEDIUM PRIORITY ALARMS////////////
    if(alarms_state_struct.ApneaAlarm==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

    if(alarms_state_struct.HighBreathRate==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

    if(alarms_state_struct.LowVte==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

    if(alarms_state_struct.HighVti==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

    if(alarms_state_struct.BackUpOn==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

    if(alarms_state_struct.UnderPeep==true)
    {
        YELLOWLed(true);
    }else{
        YELLOWLed(false);
    }

}
