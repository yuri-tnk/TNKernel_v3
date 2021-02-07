/**
  ******************************************************************************
  * @file    system_stm32l0xx.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    06-February-2015
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l0xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l0xx_system
  * @{
  */  
  
/** @addtogroup STM32L0xx_System_Private_Includes
  * @{
  */

#include "stm32l0xx.h"

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)2000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */
   
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */


void SystemClock_Config_32MHz_HSE(void);

/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_Defines
  * @{
  */
/************************* Miscellaneous Configuration ************************/

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
/******************************************************************************/
/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock = 32000000;
__IO const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
__IO const uint8_t PLLMulTable[9] = {3, 4, 6, 8, 12, 16, 24, 32, 48};

/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L0xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{    
/*!< Set MSION bit */
  RCC->CR |= (uint32_t)0x00000100;

  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
  RCC->CFGR &= (uint32_t) 0x88FF400C;
 
  /*!< Reset HSION, HSIDIVEN, HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFF6;
  
  /*!< Reset HSI48ON  bit */
  RCC->CRRCR &= (uint32_t)0xFFFFFFFE;
  
  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /*!< Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
  RCC->CFGR &= (uint32_t)0xFF02FFFF;

  /*!< Disable all interrupts */
  RCC->CIER = 0x00000000;
  
  /* Configure the Vector Table location add offset address ------------------*/

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */

  SystemClock_Config_32MHz_HSE();
}

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI 
  *             value as defined by the MSI range.
  *                                   
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32l0xx_hal.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32l0xx_hal.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmul = 0, plldiv = 0, pllsource = 0, msirange = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp)
  {
    case 0x00:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
    case 0x04:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x08:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x0C:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> 18)];
      plldiv = (plldiv >> 22) + 1;
      
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock selected as PLL clock entry */
        SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
      }
      else
      {
        /* HSE selected as PLL clock entry */
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
      }
      break;
    default: /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

//----------------------------------------------------------------------------
void SystemClock_Config_32MHz_HSE(void)
{
   volatile int i;

  /* (1) Enable power interface clock */
  /* (2) Select voltage scale 1 (1.65V - 1.95V) 
         i.e. (01)  for VOS bits in PWR_CR */
  /* (3) Enable HSE  in RCC-> CR */
  /* (4) Wait for HSE ready flag  */
  /* (5) Set PLL on HSE, multiply by 8 and divided by 2 */
  /* (6) Enable the PLL in RCC_CR register */
  /* (7) Wait for PLL ready flag */
  /* (8) Select PLL as system clock */
  /* (9) Wait for clock switched on PLL */

   RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */

   PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0; /* (2) */
   
   RCC->CR |=  RCC_CR_HSEON; //RCC_CR_HSION | RCC_CR_HSIDIVEN; /* (3) */

   while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) /* (4) */
   {
   }

//--- FLASH latency

   FLASH->ACR |= FLASH_ACR_LATENCY;
   while((FLASH->ACR & FLASH_ACR_LATENCY) == 0U)
   {
   }

   //--- Set PLL source, mul, div
                                   
   RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2; /* (5) */

   //--- AHB, APB1, APB2 Dividers

      //-- AHB
   RCC->CFGR &=  ~RCC_CFGR_HPRE; // AHB prescaler HPRE[3..0] = 0000, SYSCLK not divided
   for(i=0;i<10;i++)
   {
   }
      //-- APB1 CLK Divider - PPRE1[2:0]   HCLK divided by 2   16 Mhz
   RCC->CFGR &= ~RCC_CFGR_PPRE1;
   RCC->CFGR |=  RCC_CFGR_PPRE1_DIV2;
   for(i=0;i<10;i++)
   {
   }
      //-- APB2 CLK Divider - PPRE2[2:0]   HCLK divided by 2   16 Mhz
   RCC->CFGR &= ~RCC_CFGR_PPRE2;
   RCC->CFGR |=  RCC_CFGR_PPRE2_DIV2;

   //-- Start PLL

   RCC->CR   |= RCC_CR_PLLON; /* (6) */
   while ((RCC->CR & RCC_CR_PLLRDY)  == 0U) /* (7) */
   {
    /* For robust implementation, add here time-out management */
   }

   //-- Set PLL as system clock source

   RCC->CFGR |= RCC_CFGR_SW_PLL; /* (8) */
   while ((RCC->CFGR & RCC_CFGR_SWS_PLL)  == 0) /* (9) */
   {
   }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
