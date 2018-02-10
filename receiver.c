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
#include <stdio.h>
#include "project.h"
#include "globals.h"
#include "receiver.h"

void receiveBit(bool bit);
void receiveByte(uint8_t byte);
void printMessage(int source, char *text);

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
        int source = message[2] & 0x7F;
        int destination = message[3] & 0x7F;
        int textSize = message[4] & 0x7F;
        
        if (pos==textSize+8) { //entire message has been received
            char text[textSize+1];
            for (int i=0; i<textSize; i++) text[i] = message[i+7] & 0x7F;
            text[textSize] = '\0';
            
            if (destination==myAddress) printMessage(source, text);
            pos = 0;
        }
    }
}

void printMessage(int source, char *text) {
    char output[100];
    sprintf(output, "Message received from 0x%02x: %s\r\n", source, text);
    USBUART_PutString(output);
}

/* [] END OF FILE */