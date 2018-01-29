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

CY_ISR(RX_HANDLER) {
    RECEIVE_ClearInterrupt();
}

CY_ISR(TIMER_HANDLER) {
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
        
    }
}

/* [] END OF FILE */
