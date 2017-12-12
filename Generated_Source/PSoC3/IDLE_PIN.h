/*******************************************************************************
* File Name: IDLE_PIN.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_IDLE_PIN_H) /* Pins IDLE_PIN_H */
#define CY_PINS_IDLE_PIN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IDLE_PIN_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    IDLE_PIN_Write(uint8 value) ;
void    IDLE_PIN_SetDriveMode(uint8 mode) ;
uint8   IDLE_PIN_ReadDataReg(void) ;
uint8   IDLE_PIN_Read(void) ;
void    IDLE_PIN_SetInterruptMode(uint16 position, uint16 mode) ;
uint8   IDLE_PIN_ClearInterrupt(void) ;
/** @} general */

/***************************************
*           API Constants        
***************************************/

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the IDLE_PIN_SetDriveMode() function.
     *  @{
     */
        /* Drive Modes */
        #define IDLE_PIN_DM_ALG_HIZ         PIN_DM_ALG_HIZ   /**< \brief High Impedance Analog   */
        #define IDLE_PIN_DM_DIG_HIZ         PIN_DM_DIG_HIZ   /**< \brief High Impedance Digital  */
        #define IDLE_PIN_DM_RES_UP          PIN_DM_RES_UP    /**< \brief Resistive Pull Up       */
        #define IDLE_PIN_DM_RES_DWN         PIN_DM_RES_DWN   /**< \brief Resistive Pull Down     */
        #define IDLE_PIN_DM_OD_LO           PIN_DM_OD_LO     /**< \brief Open Drain, Drives Low  */
        #define IDLE_PIN_DM_OD_HI           PIN_DM_OD_HI     /**< \brief Open Drain, Drives High */
        #define IDLE_PIN_DM_STRONG          PIN_DM_STRONG    /**< \brief Strong Drive            */
        #define IDLE_PIN_DM_RES_UPDWN       PIN_DM_RES_UPDWN /**< \brief Resistive Pull Up/Down  */
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define IDLE_PIN_MASK               IDLE_PIN__MASK
#define IDLE_PIN_SHIFT              IDLE_PIN__SHIFT
#define IDLE_PIN_WIDTH              1u

/* Interrupt constants */
#if defined(IDLE_PIN__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IDLE_PIN_SetInterruptMode() function.
     *  @{
     */
        #define IDLE_PIN_INTR_NONE      (uint16)(0x0000u)   /**< \brief Disabled             */
        #define IDLE_PIN_INTR_RISING    (uint16)(0x0001u)   /**< \brief Rising edge trigger  */
        #define IDLE_PIN_INTR_FALLING   (uint16)(0x0002u)   /**< \brief Falling edge trigger */
        #define IDLE_PIN_INTR_BOTH      (uint16)(0x0003u)   /**< \brief Both edge trigger    */
        /** @} intrMode */
/** @} group_constants */

    #define IDLE_PIN_INTR_MASK      (0x01u)
#endif /* (IDLE_PIN__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IDLE_PIN_PS                     (* (reg8 *) IDLE_PIN__PS)
/* Data Register */
#define IDLE_PIN_DR                     (* (reg8 *) IDLE_PIN__DR)
/* Port Number */
#define IDLE_PIN_PRT_NUM                (* (reg8 *) IDLE_PIN__PRT) 
/* Connect to Analog Globals */                                                  
#define IDLE_PIN_AG                     (* (reg8 *) IDLE_PIN__AG)                       
/* Analog MUX bux enable */
#define IDLE_PIN_AMUX                   (* (reg8 *) IDLE_PIN__AMUX) 
/* Bidirectional Enable */                                                        
#define IDLE_PIN_BIE                    (* (reg8 *) IDLE_PIN__BIE)
/* Bit-mask for Aliased Register Access */
#define IDLE_PIN_BIT_MASK               (* (reg8 *) IDLE_PIN__BIT_MASK)
/* Bypass Enable */
#define IDLE_PIN_BYP                    (* (reg8 *) IDLE_PIN__BYP)
/* Port wide control signals */                                                   
#define IDLE_PIN_CTL                    (* (reg8 *) IDLE_PIN__CTL)
/* Drive Modes */
#define IDLE_PIN_DM0                    (* (reg8 *) IDLE_PIN__DM0) 
#define IDLE_PIN_DM1                    (* (reg8 *) IDLE_PIN__DM1)
#define IDLE_PIN_DM2                    (* (reg8 *) IDLE_PIN__DM2) 
/* Input Buffer Disable Override */
#define IDLE_PIN_INP_DIS                (* (reg8 *) IDLE_PIN__INP_DIS)
/* LCD Common or Segment Drive */
#define IDLE_PIN_LCD_COM_SEG            (* (reg8 *) IDLE_PIN__LCD_COM_SEG)
/* Enable Segment LCD */
#define IDLE_PIN_LCD_EN                 (* (reg8 *) IDLE_PIN__LCD_EN)
/* Slew Rate Control */
#define IDLE_PIN_SLW                    (* (reg8 *) IDLE_PIN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IDLE_PIN_PRTDSI__CAPS_SEL       (* (reg8 *) IDLE_PIN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IDLE_PIN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IDLE_PIN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IDLE_PIN_PRTDSI__OE_SEL0        (* (reg8 *) IDLE_PIN__PRTDSI__OE_SEL0) 
#define IDLE_PIN_PRTDSI__OE_SEL1        (* (reg8 *) IDLE_PIN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IDLE_PIN_PRTDSI__OUT_SEL0       (* (reg8 *) IDLE_PIN__PRTDSI__OUT_SEL0) 
#define IDLE_PIN_PRTDSI__OUT_SEL1       (* (reg8 *) IDLE_PIN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IDLE_PIN_PRTDSI__SYNC_OUT       (* (reg8 *) IDLE_PIN__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(IDLE_PIN__SIO_CFG)
    #define IDLE_PIN_SIO_HYST_EN        (* (reg8 *) IDLE_PIN__SIO_HYST_EN)
    #define IDLE_PIN_SIO_REG_HIFREQ     (* (reg8 *) IDLE_PIN__SIO_REG_HIFREQ)
    #define IDLE_PIN_SIO_CFG            (* (reg8 *) IDLE_PIN__SIO_CFG)
    #define IDLE_PIN_SIO_DIFF           (* (reg8 *) IDLE_PIN__SIO_DIFF)
#endif /* (IDLE_PIN__SIO_CFG) */

/* Interrupt Registers */
#if defined(IDLE_PIN__INTSTAT)
    #define IDLE_PIN_INTSTAT             (* (reg8 *) IDLE_PIN__INTSTAT)
    #define IDLE_PIN_SNAP                (* (reg8 *) IDLE_PIN__SNAP)
    
	#define IDLE_PIN_0_INTTYPE_REG 		(* (reg8 *) IDLE_PIN__0__INTTYPE)
#endif /* (IDLE_PIN__INTSTAT) */

#endif /* End Pins IDLE_PIN_H */


/* [] END OF FILE */
