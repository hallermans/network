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
#include "transmitter.h"
#include <stdlib.h>
#include <stdbool.h>

void addHeaderAndCrc(uint8_t *message, int textSize);
bool writeMessage(uint8_t *message, int size);

enum states {IDLE, BUSY_HIGH, BUSY_LOW, COLLISION};
enum states currentState = IDLE;
bool collisionOccured = false;

void changeState(enum states nextState) {
    if (nextState==IDLE) {
        IDLE_PIN_Write(1);
        BUSY_PIN_Write(0);
        COLLISION_PIN_Write(0);
    }
    else if (nextState==BUSY_HIGH) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(1);
        COLLISION_PIN_Write(0);
        
        TX_TIMER_Stop(); TX_TIMER_WriteCounter(12720);  TX_TIMER_Start(); //530 us
    }
    else if (nextState==BUSY_LOW) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(1);
        COLLISION_PIN_Write(0);
        
        TX_TIMER_Stop(); TX_TIMER_WriteCounter(196800); TX_TIMER_Start(); //8.2 ms
    }
    else if (nextState==COLLISION) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(0);
        COLLISION_PIN_Write(1);
        collisionOccured = true;
    }
    
    currentState = nextState;
}

void transmitter_rx_handler() {
    if (currentState==IDLE && RX_PIN_Read()==1) changeState(BUSY_HIGH); //second checks are redundant
    else if (currentState==BUSY_HIGH && RX_PIN_Read()==0) changeState(BUSY_LOW);
    else if (currentState==BUSY_LOW && RX_PIN_Read()==1) changeState(BUSY_HIGH);
    else if (currentState==COLLISION && RX_PIN_Read()==0) changeState(BUSY_LOW);
}

void transmitter_timer_handler() {
    if (currentState==BUSY_LOW) changeState(IDLE);
    else if (currentState==BUSY_HIGH) changeState(COLLISION);
}

void transmitter_uart_handler(char c) {
    static uint8_t message[52]; //7 header + 44 message + 1 CRC
    static char *text = (char *)(message + 7);
    static int textSize = 0;
    
    if (c!='\r') {
        USBUART_PutChar(c);
        text[textSize++] = c | 0x80; //MSB is always high
    }
    else {
        USBUART_PutString("\r\n");
        addHeaderAndCrc(message, textSize);
        transmitting = true;
        while (writeMessage(message, textSize + 8)==false) CyDelay(rand()%1000);
        transmitting = false;
		textSize = 0;
    }
}

void addHeaderAndCrc(uint8_t *message, int textSize) {
    int pos = 0;
    message[pos++] = 0x80; //Start of header
    message[pos++] = 0x81; //Version number
    message[pos++] = sourceAddress;
    message[pos++] = destinationAddress;
    message[pos++] = textSize;
    message[pos++] = 0x80; //CRCs are not used
    message[pos++] = 0xF7; //Header CRC is not used
    
    pos+=textSize;
    message[pos++] = 0xF7; //Message CRC is not used
}


//Returns true if succesful, false if collision
bool writeMessage(uint8_t *message, int size) {
    while (currentState != IDLE);
    collisionOccured = false;
    
    for (int i = 0; i<size; i++) {
        uint8 c = message[i];
        
        for (int b = 7; b>=0; b--) {
            if (collisionOccured) return false;
            
            TX_PIN_Write((c>>b) & 0x01);
            CyDelayUs(500);
            TX_PIN_Write(0);
            CyDelayUs(500);
        }
    }
    
    return true;
}

/* [] END OF FILE */
