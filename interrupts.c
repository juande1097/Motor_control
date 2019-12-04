/*
 * File:   interrupts.c
 * Author: juand
 *
 * Created on September 26, 2019, 10:08 PM
 */


#include <xc.h>
#include "interrupts.h"

void INTERRUPT_Initialize(void)
{
     
    GIEH = 1; // Enable interruption priority
    IPEN = 1; // Enable High priority interruptions
    IC1IE = 1;  //Enable IC1 Interruption
    IC1IP = 1;  //Set IC1 as High Priority
    TMR0IE = 1; //Enable TRMR0 interrupt
    TMR0IP = 1; //TMR0 interrupt as Low Priority
    SSPIE = 1; //I2C Interrupt enable
    SSPIP = 1; //I2C High prirority interruption
}

