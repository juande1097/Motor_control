/*
 * File:   timer0.c
 * Author: juand
 *
 * Created on September 26, 2019, 9:12 PM
 */


#include <xc.h>
#include "timer0.h"
void TMR0_Initialize (void)
{
    T0CON = 0b00001000;
    TMR0_Set_Buffer();
    TMR0ON = 1;
}

void TMR0_Set_Buffer(void)
{
    TMR0H = TMR0_Reg >> 8;
    TMR0L = TMR0_Reg & 255;
}