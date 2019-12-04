/*
 * File:   main.c
 * Author: juand
 *
 * Created on September 26, 2019, 6:45 PM
 */

#include <pic18f2331.h>
#include <xc.h>
#include "pin_manager.h"
#include "pwm.h"
#include "system.h"
#include "timer0.h"
#include "I2C_SLAVE.h"

#pragma config MCLRE = OFF            // Master Clear Enable (MCLR Disabled, RG5 Enabled)

#pragma config WDTEN = ON             // Watchdog Timer (WDT controlled by SWDTEN bit setting)
#pragma config WDPS = 512            // Watchdog Postscaler (1:16) 0.064s 502

#define _XTAL_FREQ 16000000
//#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)

int counter = 0;
double rpm = 5;
double rpm_send = 0;

unsigned char selection;

double REF = 0;
double REF_Ant = 0;
double Entrada_Actual = 0;
double Entrada_Aux = 0;
double Entrada_Pasada = 0;
double Error_Actual = 0;
double Error_Pasado = 0;
double Error_Antepasado = 0;

double ganancia_a = 10.75; //10.75
double ganancia_b = -0.3136; //-10.3136
double ganancia_c = 0.1; //0.1

int trash = 0;

int cont_my_watch = 0;
char bool_watch = 0;

void __interrupt(high_priority) high(void)
{
    if (SSPIF == 1 && SSPIE == 1) {
        SSPIF = 0;
        //SSPCONbits.CKP = 0;
        
        if(SSPCONbits.SSPOV == 1 || SSPCONbits.WCOL == 1)
        {
            I2C_Slave_collision();
            return;
        }
        
        // if it is data and read
         if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW)
        {
            trash = SSPBUF;
            while(!BF)
            {
                if ((TMR0H > 125) && (bool_watch ==  0) )
                {
                    bool_watch = 1;
                    cont_my_watch++;
                }
                if ((TMR0H < 125) && (bool_watch ==  1) )
                {
                    bool_watch = 0;
                }
                if (cont_my_watch > 10)
                {
                    cont_my_watch = 0;
                    return;
                }
            }
            REF = SSPBUF;
            SSPCONbits.CKP = 1;
            if((unsigned int)REF == 0 || (unsigned int)REF > 200 )
            {
                  LED1_Toggle();
            }
            if (REF == 0)
            {
                REF = REF_Ant;
            }
            if (REF > 100 && REF < 200)
            {
                REF = REF - 100 + 5;
                Motor_Direction(1);
                REF_Ant = REF; 
                CLRWDT();        //clear the WDT and postscaler counts when executed
            }
            else if (REF < 100 && REF> 0)
            {
                REF = REF + 5;
                Motor_Direction(0);
                REF_Ant = REF; 
                CLRWDT();        //clear the WDT and postscaler counts when executed
            }
            else if(REF == 215)
            {
                REF = 0;     
            }
            
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) //If last byte was Address + write
        {     
            trash = SSPBUF;
            SSPBUF = rpm_send  ;
            SSPCONbits.CKP = 1; 
            while(SSPSTATbits.BF);
            SSPCONbits.CKP = 1;
        }  
        //SSPCONbits.CKP = 1; 
    }

    if(IC1IF == 1 && IC1IE == 1){
        IC1IF = 0; 
        
        counter = ((unsigned int)VELRH<<8) | (unsigned int)VELRL;
        rpm = 150000.0/(double)counter;    //Izquierdo
        //rpm = 107142.8571/(double)counter; //Derecho
        if (PORTCbits.RC0 == 1 && rpm != 0)
        {
            rpm_send = rpm + 100;
        }
        else
        {
            rpm_send = rpm;
        }
    }
    //Interrupt every 10ms
    if(TMR0IE == 1 && TMR0IF == 1){
        
        TMR0IF=0;                   // Turn Off TMR0 interrupt flag
        TMR0_Set_Buffer();
        LED1_Toggle();
        
        Error_Actual = REF-rpm;
        Entrada_Aux = (100*Entrada_Pasada + ganancia_a*Error_Actual+ ganancia_b*Error_Pasado + ganancia_c*Error_Antepasado);
        Entrada_Actual = (int)(Entrada_Aux/100);
        if(Entrada_Actual >= 400){
        
            PWM_Set_Duty(400);
        }else if(Entrada_Actual <0){
        
            PWM_Set_Duty(0);
        
        }else
        {
        
            PWM_Set_Duty(Entrada_Actual);
        
        }
        if (Entrada_Actual <= 15)
        {
            rpm_send = 0;
        }
            
        Error_Antepasado = Error_Pasado;
        Error_Pasado = Error_Actual;
        Entrada_Pasada = Entrada_Actual;
    }
}


void main(void) {
    
    SYSTEM_Initialize();
    PWM_Set_Duty(0);
    //CONFIG2H = 0b00101001;

    while(1)
    {
        //LED1_SetOn();
        //CLRWDT();        //clear the WDT and postscaler counts when executed
        //PWM_Set_Duty(550);
        //Motor_Direction(0);
    }
}
