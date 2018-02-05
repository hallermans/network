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

#include <stdbool.h>
#include "project.h"
#include "globals.h"
#include "receiver.h"

void receiveBit(bool bit);

void receiver_rx_handler() {
    if (RX_PIN_Read()==0) return; //receiver only cares about rising edge

    RX_TIMER_Stop(); RX_TIMER_WriteCounter(26400);  RX_TIMER_Start(); //1100 us
    receiveBit(true);
}

void receiver_timer_handler() {
    receiveBit(false);
}

void receiveBit(bool bit) {
    static int currentBit = 7;
	static char c;
    
    if (transmitting) return;

    c = c | (bit<<currentBit);
    currentBit--;
    if (currentBit<0) {
      RX_TIMER_Stop();
      USBUART_PutChar(c & 0x7F);
      currentBit = 7;
      c = '\0';
    }
}

/* [] END OF FILE */
