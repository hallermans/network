/*******************************************************************************
* File Name: RECEIVER_TIMER_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "RECEIVER_TIMER.h"

static RECEIVER_TIMER_backupStruct RECEIVER_TIMER_backup;


/*******************************************************************************
* Function Name: RECEIVER_TIMER_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RECEIVER_TIMER_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RECEIVER_TIMER_SaveConfig(void) 
{
    #if (!RECEIVER_TIMER_UsingFixedFunction)
        RECEIVER_TIMER_backup.TimerUdb = RECEIVER_TIMER_ReadCounter();
        RECEIVER_TIMER_backup.InterruptMaskValue = RECEIVER_TIMER_STATUS_MASK;
        #if (RECEIVER_TIMER_UsingHWCaptureCounter)
            RECEIVER_TIMER_backup.TimerCaptureCounter = RECEIVER_TIMER_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!RECEIVER_TIMER_UDB_CONTROL_REG_REMOVED)
            RECEIVER_TIMER_backup.TimerControlRegister = RECEIVER_TIMER_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: RECEIVER_TIMER_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RECEIVER_TIMER_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RECEIVER_TIMER_RestoreConfig(void) 
{   
    #if (!RECEIVER_TIMER_UsingFixedFunction)

        RECEIVER_TIMER_WriteCounter(RECEIVER_TIMER_backup.TimerUdb);
        RECEIVER_TIMER_STATUS_MASK =RECEIVER_TIMER_backup.InterruptMaskValue;
        #if (RECEIVER_TIMER_UsingHWCaptureCounter)
            RECEIVER_TIMER_SetCaptureCount(RECEIVER_TIMER_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!RECEIVER_TIMER_UDB_CONTROL_REG_REMOVED)
            RECEIVER_TIMER_WriteControlRegister(RECEIVER_TIMER_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: RECEIVER_TIMER_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RECEIVER_TIMER_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void RECEIVER_TIMER_Sleep(void) 
{
    #if(!RECEIVER_TIMER_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(RECEIVER_TIMER_CTRL_ENABLE == (RECEIVER_TIMER_CONTROL & RECEIVER_TIMER_CTRL_ENABLE))
        {
            /* Timer is enabled */
            RECEIVER_TIMER_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            RECEIVER_TIMER_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    RECEIVER_TIMER_Stop();
    RECEIVER_TIMER_SaveConfig();
}


/*******************************************************************************
* Function Name: RECEIVER_TIMER_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RECEIVER_TIMER_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RECEIVER_TIMER_Wakeup(void) 
{
    RECEIVER_TIMER_RestoreConfig();
    #if(!RECEIVER_TIMER_UDB_CONTROL_REG_REMOVED)
        if(RECEIVER_TIMER_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                RECEIVER_TIMER_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
