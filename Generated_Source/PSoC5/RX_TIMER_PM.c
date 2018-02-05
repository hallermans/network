/*******************************************************************************
* File Name: RX_TIMER_PM.c
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

#include "RX_TIMER.h"

static RX_TIMER_backupStruct RX_TIMER_backup;


/*******************************************************************************
* Function Name: RX_TIMER_SaveConfig
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
*  RX_TIMER_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RX_TIMER_SaveConfig(void) 
{
    #if (!RX_TIMER_UsingFixedFunction)
        RX_TIMER_backup.TimerUdb = RX_TIMER_ReadCounter();
        RX_TIMER_backup.InterruptMaskValue = RX_TIMER_STATUS_MASK;
        #if (RX_TIMER_UsingHWCaptureCounter)
            RX_TIMER_backup.TimerCaptureCounter = RX_TIMER_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!RX_TIMER_UDB_CONTROL_REG_REMOVED)
            RX_TIMER_backup.TimerControlRegister = RX_TIMER_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: RX_TIMER_RestoreConfig
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
*  RX_TIMER_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RX_TIMER_RestoreConfig(void) 
{   
    #if (!RX_TIMER_UsingFixedFunction)

        RX_TIMER_WriteCounter(RX_TIMER_backup.TimerUdb);
        RX_TIMER_STATUS_MASK =RX_TIMER_backup.InterruptMaskValue;
        #if (RX_TIMER_UsingHWCaptureCounter)
            RX_TIMER_SetCaptureCount(RX_TIMER_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!RX_TIMER_UDB_CONTROL_REG_REMOVED)
            RX_TIMER_WriteControlRegister(RX_TIMER_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: RX_TIMER_Sleep
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
*  RX_TIMER_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void RX_TIMER_Sleep(void) 
{
    #if(!RX_TIMER_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(RX_TIMER_CTRL_ENABLE == (RX_TIMER_CONTROL & RX_TIMER_CTRL_ENABLE))
        {
            /* Timer is enabled */
            RX_TIMER_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            RX_TIMER_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    RX_TIMER_Stop();
    RX_TIMER_SaveConfig();
}


/*******************************************************************************
* Function Name: RX_TIMER_Wakeup
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
*  RX_TIMER_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RX_TIMER_Wakeup(void) 
{
    RX_TIMER_RestoreConfig();
    #if(!RX_TIMER_UDB_CONTROL_REG_REMOVED)
        if(RX_TIMER_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                RX_TIMER_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
