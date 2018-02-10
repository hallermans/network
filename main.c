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
#include "globals.h"
#include "receiver.h"
#include "transmitter.h"
#include <stdlib.h>
#include <stdbool.h>

bool transmitting = false;
uint8_t myAddress = 0x07;
uint8_t destinationAddress = 0x08;

CY_ISR(RX_PIN_HANDLER) {
    receiver_rx_handler();
    transmitter_rx_handler();

    RX_PIN_ClearInterrupt();
}

CY_ISR(RX_TIMER_HANDLER) {
    receiver_timer_handler();

    RX_TIMER_ReadStatusRegister(); //clear the timer interrupt
}

CY_ISR(TX_TIMER_HANDLER) {
    transmitter_timer_handler();
    
    TX_TIMER_ReadStatusRegister(); //clear the timer interrupt
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    RX_PIN_INTERRUPT_StartEx(RX_PIN_HANDLER);
	
    RX_TIMER_INTERRUPT_StartEx(RX_TIMER_HANDLER);
    RX_TIMER_Start();
	
    TX_TIMER_INTERRUPT_StartEx(TX_TIMER_HANDLER);
    TX_TIMER_Start();

    USBUART_Start(0, USBUART_5V_OPERATION);
    while (USBUART_GetConfiguration()==0);
    USBUART_CDC_Init();
    USBUART_PutString("Hello World\r\n");

    for(;;)
    {
        /* Place your application code here. */
        while (!USBUART_DataIsReady()); //wait for a character to be received
        transmitter_uart_handler(USBUART_GetChar());
    }
}

/* [] END OF FILE */
