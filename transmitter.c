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

int writeMessage(char *message, int size);

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
    static char message[100];
    static int size = 0;
    
    if (c=='\r') {
        USBUART_PutString("\r\n");
        transmitting = true;
        while (writeMessage(message, size)!=0) CyDelay(rand()%1000);
        transmitting = false;
		size = 0;
    }
    else {
        USBUART_PutChar(c);
        message[size++] = c;
    }
}

/*
 * Write message to TX_PIN
 * MSB is always set to 1
 * Returns 0 if succesful, 1 if collision
 */
int writeMessage(char *message, int size) {
    while (currentState != IDLE);
    collisionOccured = false;
    
    for (int i = 0; i<size; i++) {
        uint8 c = message[i] | 0x80; //MSB is always high
        
        for (int b = 7; b>=0; b--) {
            if (collisionOccured) return 1;
            
            TX_PIN_Write((c>>b) & 0x01);
            CyDelayUs(500);
            TX_PIN_Write(0);
            CyDelayUs(500);
        }
    }
    
    return 0;
}

/* [] END OF FILE */
