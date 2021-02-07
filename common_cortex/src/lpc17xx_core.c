
/******************************************************************************
 * @file:    core_cm3.h
 * @purpose: CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version: V1.20
 * @date:    22. May 2009
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <stdint.h>
#include "LPC176x.h"


/* SysTick Control / Status Register Definitions */
#define SysTick_CTRL_COUNTFLAG_Pos         16U                                            /*!< SysTick CTRL: COUNTFLAG Position */
#define SysTick_CTRL_COUNTFLAG_Msk         (1UL << SysTick_CTRL_COUNTFLAG_Pos)            /*!< SysTick CTRL: COUNTFLAG Mask */

#define SysTick_CTRL_CLKSOURCE_Pos          2U                                            /*!< SysTick CTRL: CLKSOURCE Position */
#define SysTick_CTRL_CLKSOURCE_Msk         (1UL << SysTick_CTRL_CLKSOURCE_Pos)            /*!< SysTick CTRL: CLKSOURCE Mask */

#define SysTick_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define SysTick_CTRL_TICKINT_Msk           (1UL << SysTick_CTRL_TICKINT_Pos)              /*!< SysTick CTRL: TICKINT Mask */

#define SysTick_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Msk            (1UL /*<< SysTick_CTRL_ENABLE_Pos*/)           /*!< SysTick CTRL: ENABLE Mask */

/* SysTick Reload Register Definitions */
#define SysTick_LOAD_RELOAD_Pos             0U                                            /*!< SysTick LOAD: RELOAD Position */
#define SysTick_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)    /*!< SysTick LOAD: RELOAD Mask */

/* SysTick Current Register Definitions */
#define SysTick_VAL_CURRENT_Pos             0U                                            /*!< SysTick VAL: CURRENT Position */
#define SysTick_VAL_CURRENT_Msk            (0xFFFFFFUL /*<< SysTick_VAL_CURRENT_Pos*/)    /*!< SysTick VAL: CURRENT Mask */

/* SysTick Calibration Register Definitions */
#define SysTick_CALIB_NOREF_Pos            31U                                            /*!< SysTick CALIB: NOREF Position */
#define SysTick_CALIB_NOREF_Msk            (1UL << SysTick_CALIB_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */

#define SysTick_CALIB_SKEW_Pos             30U                                            /*!< SysTick CALIB: SKEW Position */
#define SysTick_CALIB_SKEW_Msk             (1UL << SysTick_CALIB_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */

#define SysTick_CALIB_TENMS_Pos             0U                                            /*!< SysTick CALIB: TENMS Position */
#define SysTick_CALIB_TENMS_Msk            (0xFFFFFFUL /*<< SysTick_CALIB_TENMS_Pos*/)    /*!< SysTick CALIB: TENMS Mask */

/* ##########################   NVIC functions  #################################### */

/**
 * @brief  Set the Priority Grouping in NVIC Interrupt Controller
 *
 * @param  uint32_t priority_grouping is priority grouping field
 * @return none 
 *
 * Set the priority grouping field using the required unlock sequence.
 * The parameter priority_grouping is assigned to the field 
 * SCB->AIRCR [10:8] PRIGROUP field. Only values from 0..7 are used.
 * In case of a conflict between priority grouping and available
 * priority bits (NVIC_PRIO_BITS) the smallest possible priority group is set.
 */

void NVIC_SetPriorityGrouping(unsigned int PriorityGroup)
{
  unsigned int reg_value;
  unsigned int PriorityGroupTmp = (PriorityGroup & 0x07U);                         /* only values 0..7 are used          */

  reg_value  = SCB->AIRCR;                                                    /* read old register configuration    */
  reg_value &= ~((0xFFFFU << 16U) | (0x0FU << 8U));                              /* clear bits to change               */
  reg_value  = ((reg_value | NVIC_AIRCR_VECTKEY | (PriorityGroupTmp << 8U)));  /* Insert write key and priorty group */
  SCB->AIRCR = reg_value;
}

/** * @brief  Get the Priority Grouping from NVIC Interrupt Controller
 *
 * @param  none
 * @return unsigned int   priority grouping field
 *
 * Get the priority grouping from NVIC Interrupt Controller.
 * priority grouping is SCB->AIRCR [10:8] PRIGROUP field.
 */

unsigned int NVIC_GetPriorityGrouping(void)
{
  return ((SCB->AIRCR >> 8U) & 0x07U);                                          /* read priority grouping field */
}

/**
 * @brief  Enable Interrupt in NVIC Interrupt Controller
 *
 * @param  IRQn_Type IRQn specifies the interrupt number
 * @return none
 *
 * Enable a device specific interupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */

void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC->ISER[((unsigned int)(IRQn) >> 5U)] = (1UL << ((unsigned int)(IRQn) & 0x1FUL)); /* enable interrupt */
}


/**
 * @brief  Disable the interrupt line for external interrupt specified
 *
 * @param  IRQn_Type IRQn is the positive number of the external interrupt
 * @return none
 *
 * Disable a device specific interupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  NVIC->ICER[((unsigned int)(IRQn) >> 5U)] = (1UL << ((unsigned int)(IRQn) & 0x1FUL)); /* disable interrupt */
}

/**
 * @brief  Read the interrupt pending bit for a device specific interrupt source
 *
 * @param  IRQn_Type IRQn is the number of the device specifc interrupt
 * @return unsigned int 1 if pending interrupt else 0
 *
 * Read the pending register in NVIC and return 1 if its status is pending,
 * otherwise it returns 0
 */

unsigned int NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
   unsigned int tmp;
   unsigned int tmp1;
   unsigned int rc = 0UL;

   tmp  = NVIC->ISPR[(unsigned int)(IRQn) >> 5U];
   tmp1 = 1UL << ((unsigned int)(IRQn) & 0x1FUL);
   if((tmp & tmp1) == tmp1)
   {
      rc = 1UL; 
   }   

   return rc;
//  return((unsigned int) ((NVIC->ISPR[(unsigned int)(IRQn) >> 5U] & (1UL << ((unsigned int)(IRQn) & 0x1FUL)))?1UL:0UL)); // Return 1 if pending else 0 
}

/**
 * @brief  Set the pending bit for an external interrupt
 *
 * @param  IRQn_Type IRQn is the Number of the interrupt
 * @return none
 *
 * Set the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ISPR[((unsigned int)(IRQn) >> 5U)] = (1UL << ((unsigned int)(IRQn) & 0x1FUL)); /* set interrupt pending */
}

/**
 * @brief  Clear the pending bit for an external interrupt
 *
 * @param  IRQn_Type IRQn is the Number of the interrupt
 * @return none
 *
 * Clear the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ICPR[((unsigned int)(IRQn) >> 5UL)] = (1UL << ((unsigned int)(IRQn) & 0x1FUL)); /* Clear pending interrupt */
}

/**
 * @brief  Read the active bit for an external interrupt
 *
 * @param  IRQn_Type  IRQn is the Number of the interrupt
 * @return unsigned int   1 if active else 0
 *
 * Read the active register in NVIC and returns 1 if its status is active,
 * otherwise it returns 0.
 */
unsigned int NVIC_GetActive(IRQn_Type IRQn)
{
  unsigned int tmp;
  unsigned int tmp1;
  unsigned int rc = 0UL;

  tmp  = (unsigned int)NVIC->IABR[(unsigned int)(IRQn) >> 5UL]; 
  tmp1 = 1UL << ((unsigned int)(IRQn) & 0x1FUL);
  if((tmp & tmp1) == tmp1)
  {
     rc = 1UL; 
  }
//  return((unsigned int)((NVIC->IABR[(unsigned int)(IRQn) >> 5UL] & (1UL << ((unsigned int)(IRQn) & 0x1FUL)))?1UL:0UL)); // Return 1 if active else 0

  return rc;
}

/**
 * @brief  Set the priority for an interrupt
 *
 * @param  IRQn_Type IRQn is the Number of the interrupt
 * @param  priority is the priority for the interrupt
 * @return none
 *
 * Set the priority for the specified interrupt. The interrupt
 * number can be positive to specify an external (device specific)
 * interrupt, or negative to specify an internal (core) interrupt. \n
 *
 * Note: The priority cannot be set for every core interrupt.
 */
void NVIC_SetPriority(IRQn_Type IRQn, unsigned int priority)
{

#if defined (__ICCARM__) 
#pragma cstat_suppress="MISRAC2012-Rule-14.3_a"
   if((int32_t)(IRQn) < 0L)   // IAR C-STAT MISRA 2012 provides a false message here
#else
   if((int32_t)(IRQn) < 0L)   // For non-IAR compiler
#endif   
   {
      SCB->SHP[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
   }
   else
   {
      NVIC->IP[((uint32_t)(int32_t)IRQn)]  = (uint8_t)((priority << (8U - NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
   }

}


/* ##################################    SysTick function  ############################################ */


/* SysTick constants */

#define SYSTICK_ENABLE              0                                          /* Config-Bit to start or stop the SysTick Timer                         */
#define SYSTICK_TICKINT             1                                          /* Config-Bit to enable or disable the SysTick interrupt                 */
#define SYSTICK_CLKSOURCE           2                                          /* Clocksource has the offset 2 in SysTick Control and Status Register   */
#define SYSTICK_MAXCOUNT       ((1UL<<24UL) -1UL)                                    /* SysTick MaxCount                                                      */

/**
 * @brief  Initialize and start the SysTick counter and its interrupt.
 *
 * @param  unsigned int ticks is the number of ticks between two interrupts
 * @return  none
 *
 * Initialise the system tick timer and its interrupt and start the
 * system tick timer / counter in free running mode to generate
 * periodical interrupts.
 */
unsigned int SysTick_Config(unsigned int ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------







