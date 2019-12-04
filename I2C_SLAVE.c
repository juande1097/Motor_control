/*
 * File:   I2C_SLAVE.c
 * Author: juand
 *
 * Created on June 17, 2019, 12:41 PM
 */


#include <pic18f2331.h>
#include "I2C_SLAVE.h"
unsigned char z;

void I2C_Slave_Init()
{
    SSPCON = 0b00110110; //0b00100110   0b00110110
    //SSPSTAT = 0;
    SSPADD = Pic_der_ADDR<<1;
    //SSPADD = Pic_izq_ADDR<<1;
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC4 = 1;

}

void I2C_Slave_Send(int input)
{
    
    z = SSPBUF;
    SSPBUF = input  ;
    SSPCONbits.CKP = 1; 
    while(SSPSTATbits.BF);
    SSPCONbits.CKP = 1; 
            
}

unsigned char I2C_Slave_Receive(void)
{
    unsigned char selection;
    z = SSPBUF;
    //while(!SSPSTATbits.BF);
    selection = SSPBUF;
    SSPCONbits.CKP = 1;   
    return selection;        
            
}

void I2C_Slave_collision(void)
{
    z = SSPBUF;
    SSPCONbits.SSPOV = 0;
    SSPCONbits.WCOL = 0;
    SSPCONbits.CKP = 1;   
}


    