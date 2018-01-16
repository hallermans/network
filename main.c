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
        
        TIMER_Stop(); TIMER_WriteCounter(12720);  TIMER_Start(); //530 us
    }
    else if (nextState==BUSY_LOW) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(1);
        COLLISION_PIN_Write(0);
        
        TIMER_Stop(); TIMER_WriteCounter(196800); TIMER_Start(); //8.2 ms
    }
    else if (nextState==COLLISION) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(0);
        COLLISION_PIN_Write(1);
        collisionOccured = true;
    }
    
    currentState = nextState;
}

CY_ISR(RX_HANDLER) {
    if (currentState==IDLE && RECEIVE_Read()==1) changeState(BUSY_HIGH); //second checks are redundant
    else if (currentState==BUSY_HIGH && RECEIVE_Read()==0) changeState(BUSY_LOW);
    else if (currentState==BUSY_LOW && RECEIVE_Read()==1) changeState(BUSY_HIGH);
    else if (currentState==COLLISION && RECEIVE_Read()==0) changeState(BUSY_LOW);

    RECEIVE_ClearInterrupt();
}

CY_ISR(TIMER_HANDLER) {
    if (currentState==BUSY_LOW) changeState(IDLE);
    else if (currentState==BUSY_HIGH) changeState(COLLISION);
    
    TIMER_ReadStatusRegister(); //clear the timer interrupt
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
        //read the message from UART
        char message[100];
        int size = 0;
        while (1) {
            while (!USBUART_DataIsReady()); //wait for a character to be received
            char c = USBUART_GetChar();
            
            if (c=='\r') {
                USBUART_PutString("\r\n");
                break;
            }
            else {
                USBUART_PutChar(c);
                message[size++] = c;
            }
        }
        
        //Write the message to TX_PIN
        bool rc = writeMessage(message, size);
        while (rc) {
            collisionOccured = false;
            CyDelay(rand()%1000);
            rc = writeMessage(message, size);
        }
    }
}

/*
 * Write message to TX_PIN
 * MSB is always set to 1
 * Returns 0 if succesful, 1 if collision
 */
int writeMessage(char *message, int size) {
    while (currentState != IDLE);
    
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
