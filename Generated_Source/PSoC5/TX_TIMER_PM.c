/*******************************************************************************
* File Name: TX_TIMER_PM.c
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

#include "TX_TIMER.h"

static TX_TIMER_backupStruct TX_TIMER_backup;


/*******************************************************************************
* Function Name: TX_TIMER_SaveConfig
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
*  TX_TIMER_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TX_TIMER_SaveConfig(void) 
{
    #if (!TX_TIMER_UsingFixedFunction)
        TX_TIMER_backup.TimerUdb = TX_TIMER_ReadCounter();
        TX_TIMER_backup.InterruptMaskValue = TX_TIMER_STATUS_MASK;
        #if (TX_TIMER_UsingHWCaptureCounter)
            TX_TIMER_backup.TimerCaptureCounter = TX_TIMER_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TX_TIMER_UDB_CONTROL_REG_REMOVED)
            TX_TIMER_backup.TimerControlRegister = TX_TIMER_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TX_TIMER_RestoreConfig
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
*  TX_TIMER_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TX_TIMER_RestoreConfig(void) 
{   
    #if (!TX_TIMER_UsingFixedFunction)

        TX_TIMER_WriteCounter(TX_TIMER_backup.TimerUdb);
        TX_TIMER_STATUS_MASK =TX_TIMER_backup.InterruptMaskValue;
        #if (TX_TIMER_UsingHWCaptureCounter)
            TX_TIMER_SetCaptureCount(TX_TIMER_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TX_TIMER_UDB_CONTROL_REG_REMOVED)
            TX_TIMER_WriteControlRegister(TX_TIMER_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TX_TIMER_Sleep
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
*  TX_TIMER_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TX_TIMER_Sleep(void) 
{
    #if(!TX_TIMER_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TX_TIMER_CTRL_ENABLE == (TX_TIMER_CONTROL & TX_TIMER_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TX_TIMER_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TX_TIMER_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TX_TIMER_Stop();
    TX_TIMER_SaveConfig();
}


/*******************************************************************************
* Function Name: TX_TIMER_Wakeup
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
*  TX_TIMER_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TX_TIMER_Wakeup(void) 
{
    TX_TIMER_RestoreConfig();
    #if(!TX_TIMER_UDB_CONTROL_REG_REMOVED)
        if(TX_TIMER_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TX_TIMER_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
