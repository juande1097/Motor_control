/*
 * File:   system.c
 * Author: juand
 *
 * Created on September 26, 2019, 7:01 PM
 */


#include <xc.h>
#include "system.h"
#include "pin_manager.h"
#include "pwm.h"
#include "interrupts.h"
#include "timer0.h"
#include "qei.h"
#include "I2C_SLAVE.h"

void SYSTEM_Initialize(void)
{
    OSCCON = 0b01110000;
    PIN_MANAGER_Initialize();
    PWM_Initialize();
    TMR0_Initialize();
    INTERRUPT_Initialize();
    Quadrature_Encoder_Initialize();
    I2C_Slave_Init();
    WDTCONbits.SWDTEN = 1; //Watchdog Timer is on
    
}
