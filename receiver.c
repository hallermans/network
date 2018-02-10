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
void receiveByte(uint8_t byte);

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
      receiveByte(c);
      currentBit = 7;
      c = '\0';
    }
}

void receiveByte(uint8_t byte) {
    static uint8_t message[52]; //7 header + 44 message + 1 CRC
    static int pos = 0;
    
    message[pos++] = byte;

    if (pos==1 && message[0]!=0x80) pos = 0;
    else if (pos==2 && message[1]!=0x81) pos = 0;
    else if (pos>=8) {
        int textSize = message[4] & 0x7F;
        if (pos==textSize+8) {
            for (int i=7; i<textSize+7; i++) message[i] &= 0x7F;
            message[textSize+7] = '\0';
            USBUART_PutString((char *)(message+7));
            pos = 0;
        }
    }
}

/* [] END OF FILE */