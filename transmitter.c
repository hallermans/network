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

void createAndSendMessage(char *text, int textSize, int destinationAddress);
void sendMessage(uint8_t *message, int size);

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
    static bool gettingDestination = true; //true while getting destination address, false while getting message
    static int destination;
    static char text[44];
    static int textSize = 0;
    
    if (gettingDestination) {
        if (c!='\r') {
            USBUART_PutChar(c);
            text[textSize++] = c;
        }
        else {
            USBUART_PutString("\r\nEnter Message: ");
            text[textSize] = '\0';
            destination = atoi(text);
            textSize = 0;
            gettingDestination = false;
        }
    }
    else {
        if (c!='\r') {
            USBUART_PutChar(c);
            text[textSize++] = c;
        }
        else {
            USBUART_PutString("\r\nDestination: ");
            createAndSendMessage(text, textSize, destination);
            textSize = 0;
            gettingDestination = true;
        }
    }
}

void createAndSendMessage(char *text, int textSize, int destinationAddress) {
    uint8_t message[52]; //7 header + 44 message + 1 CRC
    int pos = 0;
    
    message[pos++] = 0x80; //Start of header
    message[pos++] = 0x81; //Version number
    message[pos++] = myAddress;
    message[pos++] = destinationAddress;
    message[pos++] = textSize;
    message[pos++] = 0x80; //CRCs are not used
    message[pos++] = 0xF7; //Header CRC is not used
    for (int i = 0; i<textSize; i++) message[pos++] = text[i];
    message[pos++] = 0xF7; //Message CRC is not used
    
    sendMessage(message, textSize+8);
}

void sendMessage(uint8_t *message, int size) {
    while (currentState!=IDLE);
    collisionOccured = false;
    transmitting = true;
    while (true) {
        for (int i = 0; i<size; i++) {
            uint8 c = message[i] | 0x80; //MSB is always high
            
            for (int b = 7; b>=0; b--) {
                TX_PIN_Write((c>>b) & 0x01);
                CyDelayUs(500);
                TX_PIN_Write(0);
                CyDelayUs(500);
                
                if (collisionOccured) break;
            }
            
            if (collisionOccured) break;
        }
        
        if (collisionOccured) {
            CyDelay(rand()%1000);
            while (currentState!=IDLE);
            collisionOccured = false;
        }
        else break;
    }
    
    transmitting = false;
}

/* [] END OF FILE */
