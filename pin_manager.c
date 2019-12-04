/*
 * File:   pin_manager.c
 * Author: juand
 *
 * Created on September 26, 2019, 6:57 PM
 */


#include <xc.h>
#include "pin_manager.h"
void PIN_MANAGER_Initialize (void)
{
    LED1_SetDigitalOutput();
    C0_SetDigitalOutput();
    C1_SetDigitalOutput();
    LED1_SetOn();
    Motor_Direction(1); //hacia adelante
}

void Motor_Direction(char direction)
{
    //hacia adelante
    if (direction == 1)
    { 
        C0_SetHigh();
        C1_SetLow();
    }
    //hacia atras
    if (direction == 0)
    { 
        C0_SetLow();
        C1_SetHigh();
    }
    
}