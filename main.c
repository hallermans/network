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

enum states {IDLE, BUSY, COLLISION};
enum states currentState = IDLE;
int waiting = 0;

void changeState(enum states nextState) {
    if (nextState==IDLE) {
        IDLE_PIN_Write(1);
        BUSY_PIN_Write(0);
        COLLISION_PIN_Write(0);
    }
    else if (nextState==BUSY) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(1);
        COLLISION_PIN_Write(0);
    }
    else if (nextState==COLLISION) {
        IDLE_PIN_Write(0);
        BUSY_PIN_Write(0);
        COLLISION_PIN_Write(1);
    }
    
    currentState = nextState;
}

CY_ISR(RX_HANDLER) {
    if (currentState==IDLE) {
        if (RECEIVE_Read()==1) {
            waiting = 0;
            changeState(BUSY);
        }
    }
    else if (currentState==BUSY) {
        if (RECEIVE_Read()==1) {
            waiting = 1;
            TIMER_Stop(); TIMER_WriteCounter(12720);  TIMER_Start();//530 us
        }
        else {
            waiting = 1;
            TIMER_Stop(); TIMER_WriteCounter(196800); TIMER_Start();//8.2 ms
        }
    }
    else if (currentState==COLLISION) {
        if (RECEIVE_Read()==0) {
            waiting = 0;
            changeState(BUSY);
        }
    }
    
    /*if (currentState==COLLISION) changeState(IDLE);
    else changeState(COLLISION);*/
    RECEIVE_ClearInterrupt();
}

CY_ISR(TIMER_HANDLER) {
    if (waiting) {
        waiting = 0;
        if (RECEIVE_Read()==0) {
            changeState(IDLE);
        }
        else {
            changeState(COLLISION);
        }
    }
    
    TIMER_STATUS;
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
