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

enum states {IDLE, BUSY_HIGH, BUSY_LOW, COLLISION};
enum states currentState = IDLE;

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
    
    TIMER_ReadStatusRegister();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    RX_INTERRUPT_StartEx(RX_HANDLER);
    TIMER_INTERRUPT_StartEx(TIMER_HANDLER);
    TIMER_Start();
    

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
