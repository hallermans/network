/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdlib.h>
#include <stdbool.h>

void receiveBit(bool bit);

CY_ISR(RX_HANDLER) {
    if (RECEIVE_Read()==0) return; //receiver only cares about rising edge

    TIMER_Stop(); TIMER_WriteCounter(26400);  TIMER_Start(); //1100 
    receiveBit(true);

    RECEIVE_ClearInterrupt();
}

CY_ISR(TIMER_HANDLER) {
    receiveBit(false);

    TIMER_ReadStatusRegister(); //clear the timer interrupt
}

void receiveBit(bool bit) {
    static int currentBit = 7;
	static char c;

    c = c | (bit<<currentBit);
    currentBit--;
    if (currentBit<0) {
      TIMER_Stop();
      USBUART_PutChar(c & 0x7F);
      currentBit = 7;
      c = '\0';
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    RX_INTERRUPT_StartEx(RX_HANDLER);
    TIMER_INTERRUPT_StartEx(TIMER_HANDLER);
    TIMER_Start();

    USBUART_Start(0, USBUART_5V_OPERATION);
    while (USBUART_GetConfiguration()==0);
    USBUART_CDC_Init();

    USBUART_PutString("Hello World\r\n");


    for(;;)
    {
        /* Place your application code here. */

    }
}

/* [] END OF FILE */
