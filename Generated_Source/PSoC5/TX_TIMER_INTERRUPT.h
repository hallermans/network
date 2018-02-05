/*******************************************************************************
* File Name: TX_TIMER_INTERRUPT.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_TX_TIMER_INTERRUPT_H)
#define CY_ISR_TX_TIMER_INTERRUPT_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void TX_TIMER_INTERRUPT_Start(void);
void TX_TIMER_INTERRUPT_StartEx(cyisraddress address);
void TX_TIMER_INTERRUPT_Stop(void);

CY_ISR_PROTO(TX_TIMER_INTERRUPT_Interrupt);

void TX_TIMER_INTERRUPT_SetVector(cyisraddress address);
cyisraddress TX_TIMER_INTERRUPT_GetVector(void);

void TX_TIMER_INTERRUPT_SetPriority(uint8 priority);
uint8 TX_TIMER_INTERRUPT_GetPriority(void);

void TX_TIMER_INTERRUPT_Enable(void);
uint8 TX_TIMER_INTERRUPT_GetState(void);
void TX_TIMER_INTERRUPT_Disable(void);

void TX_TIMER_INTERRUPT_SetPending(void);
void TX_TIMER_INTERRUPT_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the TX_TIMER_INTERRUPT ISR. */
#define TX_TIMER_INTERRUPT_INTC_VECTOR            ((reg32 *) TX_TIMER_INTERRUPT__INTC_VECT)

/* Address of the TX_TIMER_INTERRUPT ISR priority. */
#define TX_TIMER_INTERRUPT_INTC_PRIOR             ((reg8 *) TX_TIMER_INTERRUPT__INTC_PRIOR_REG)

/* Priority of the TX_TIMER_INTERRUPT interrupt. */
#define TX_TIMER_INTERRUPT_INTC_PRIOR_NUMBER      TX_TIMER_INTERRUPT__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable TX_TIMER_INTERRUPT interrupt. */
#define TX_TIMER_INTERRUPT_INTC_SET_EN            ((reg32 *) TX_TIMER_INTERRUPT__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the TX_TIMER_INTERRUPT interrupt. */
#define TX_TIMER_INTERRUPT_INTC_CLR_EN            ((reg32 *) TX_TIMER_INTERRUPT__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the TX_TIMER_INTERRUPT interrupt state to pending. */
#define TX_TIMER_INTERRUPT_INTC_SET_PD            ((reg32 *) TX_TIMER_INTERRUPT__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the TX_TIMER_INTERRUPT interrupt. */
#define TX_TIMER_INTERRUPT_INTC_CLR_PD            ((reg32 *) TX_TIMER_INTERRUPT__INTC_CLR_PD_REG)


#endif /* CY_ISR_TX_TIMER_INTERRUPT_H */


/* [] END OF FILE */
