/**
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/*   This file is the digest from the ST HAL and Std periph libraries 
   for the project. 

     The HAL GetTickCount() is not used. Instead, the straightforward function
   delay_1ms() is used - I don't want to enable any interrupts until the RTOS 
   is started. 
*/

#include "stm32f7xx.h"
#include "system_stm32f746.h"
#include "stm32f7xx_conf.h"

static const uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9}; 
uint32_t SystemCoreClock = 0U;

static GPIO_TypeDef * GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
static const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef_Ex * GPIO_Init);
void HAL_GPIO_WritePin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef * RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef * RCC_ClkInitStruct, 
                                      uint32_t FLatency);
void HAL_GPIO_TogglePin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);
static HAL_StatusTypeDef HAL_PWREx_EnableOverDrive(void);
static uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);
static uint32_t HAL_RCC_GetHCLKFreq(void);

void rcc_init(void);
//--- Assembly function
void do_delay(unsigned int loops_per_ms);



#define GPIO_MODE             ((uint32_t)0x00000003)
#define EXTI_MODE             ((uint32_t)0x10000000)
#define GPIO_MODE_IT          ((uint32_t)0x00010000)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000)
#define RISING_EDGE           ((uint32_t)0x00100000)
#define FALLING_EDGE          ((uint32_t)0x00200000)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010)

#define GPIO_NUMBER           ((uint32_t)16)

/*!< USART CR1 register clear Mask ((~(uint16_t)0xE9F3)) */
#define CR1_CLEAR_MASK            ((uint16_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))

/*!< USART CR2 register clock bits clear Mask ((~(uint16_t)0xF0FF)) */
#define CR2_CLOCK_CLEAR_MASK      ((uint16_t)(USART_CR2_CLKEN | USART_CR2_CPOL | \
                                              USART_CR2_CPHA | USART_CR2_LBCL))

/*!< USART CR3 register clear Mask ((~(uint16_t)0xFCFF)) */
#define CR3_CLEAR_MASK            ((uint16_t)(USART_CR3_RTSE | USART_CR3_CTSE))

#define VECT_TAB_OFFSET  0x00UL /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
#define HSE_VALUE        8000000

/**
  * @brief  Enable the FLASH Adaptive Real-Time memory accelerator.
  * @note   The ART accelerator is available only for flash access on ITCM interface.
  * @retval none
  */ 
#define __HAL_FLASH_ART_ENABLE()  SET_BIT(FLASH->ACR, FLASH_ACR_ARTEN)

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
 // #warning Use FPU !
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= 0x00000001UL;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000UL;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFFUL;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010UL;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFFUL;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000UL;

  /* Configure the Vector Table location add offset address ------------------*/

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */

}

/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  RCC_OscInitStruct: pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef * RCC_OscInitStruct)
{
  uint32_t tickstart = 0UL;  
 
  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));
  
  /*------------------------------- HSE Configuration ------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
  {
    /* Check the parameters */
     assert_param(IS_RCC_HSE(RCC_OscInitStruct->HSEState));

    /* When the HSE is used as system clock or clock source for PLL, 
      It can not be disabled */

     if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE) 
          || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && 
                ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)))
     {
 	     if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != /*RESET*/ 0UL) && 
            (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))
        {
           return HAL_ERROR;
        }
     }
     else
     {
      /* Reset HSEON and HSEBYP bits before configuring the HSE --------------*/

        __HAL_RCC_HSE_CONFIG(RCC_HSE_OFF);
      
      /* Wait till HSE is disabled */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != 0UL)//RESET)
        {
           if(tickstart > HSE_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
      
      /* Set the new HSE configuration ---------------------------------------*/
        __HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);
      
      /* Check the HSE State */
        if(RCC_OscInitStruct->HSEState != RCC_HSE_OFF)
        {
           /* Wait till HSE is ready */  
           tickstart = 0UL;
           while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == 0UL/*RESET*/)
           {
              if(tickstart > HSE_TIMEOUT_VALUE)
              {
                 return HAL_TIMEOUT;
              }
              delay_1ms();
              tickstart++;
           }
        }
        else
        {
           /* Wait till HSE is bypassed or disabled */
           tickstart = 0UL;
           while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET)
           {
              if(tickstart > HSE_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
      }
    }
  }
  /*----------------------------- HSI Configuration --------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == 
               RCC_OSCILLATORTYPE_HSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSI(RCC_OscInitStruct->HSIState));
    assert_param(IS_RCC_CALIBRATION_VALUE(RCC_OscInitStruct->HSICalibrationValue));
    
    /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */ 
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI) 
       || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && 
            ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))
    {
      /* When HSI is used as system clock it will not disabled */
      if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && 
            (RCC_OscInitStruct->HSIState != RCC_HSI_ON))
      {
        return HAL_ERROR;
      }
      /* Otherwise, just the calibration is allowed */
      else
      {
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
    }
    else
    {
      /* Check the HSI State */
      if((RCC_OscInitStruct->HSIState)!= RCC_HSI_OFF)
      {
        /* Enable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_ENABLE();


        /* Wait till HSI is ready */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
           if(tickstart > HSI_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
                
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
      else
      {
        /* Disable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_DISABLE();
      
        /* Wait till HSI is ready */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
        {
           if(tickstart > HSI_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        } 
      }
    }
  }
  /*------------------------------ LSI Configuration -------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == 
                   RCC_OSCILLATORTYPE_LSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LSI(RCC_OscInitStruct->LSIState));

    /* Check the LSI State */
    if((RCC_OscInitStruct->LSIState)!= RCC_LSI_OFF)
    {
      /* Enable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_ENABLE();
      
      
      /* Wait till LSI is ready */
      tickstart = 0UL;
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
      {
           if(tickstart > LSI_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
      }
    }
    else
    {
      /* Disable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_DISABLE();
      
      
      /* Wait till LSI is ready */  
      tickstart = 0UL;
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET)
      {
           if(tickstart > LSI_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
      }
    }
  }
  /*------------------------------ LSE Configuration -------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == 
             RCC_OSCILLATORTYPE_LSE)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_OscInitStruct->LSEState));
    
    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* Enable write access to Backup domain */
    PWR->CR1 |= PWR_CR1_DBP;
    
    /* Wait for Backup domain Write protection disable */
    tickstart = 0UL;
    while((PWR->CR1 & PWR_CR1_DBP) == RESET)    // ???
    {
           if(tickstart > RCC_DBP_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
    }
    
    /* Reset LSEON and LSEBYP bits before configuring the LSE ----------------*/
    __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);
    
    /* Wait till LSE is ready */  
    tickstart = 0UL;
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
    {
           if(tickstart > RCC_LSE_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
    } 
    
    /* Set the new LSE configuration -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);
    /* Check the LSE State */
    if((RCC_OscInitStruct->LSEState) != RCC_LSE_OFF)
    {
      /* Wait till LSE is ready */  
      tickstart = 0UL;
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)  // ???
      {
           if(tickstart > RCC_LSE_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
      }
    }
    else
    {
      /* Wait till LSE is ready */  
      tickstart = 0UL;
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)   // ???
      {
           if(tickstart > RCC_LSE_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
      }
    }
  }
  /*-------------------------------- PLL Configuration -----------------------*/
  /* Check the parameters */
  assert_param(IS_RCC_PLL(RCC_OscInitStruct->PLL.PLLState));
  if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL is used as system clock or not */
    if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    { 
      if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLLSOURCE(RCC_OscInitStruct->PLL.PLLSource));
        assert_param(IS_RCC_PLLM_VALUE(RCC_OscInitStruct->PLL.PLLM));
        assert_param(IS_RCC_PLLN_VALUE(RCC_OscInitStruct->PLL.PLLN));
        assert_param(IS_RCC_PLLP_VALUE(RCC_OscInitStruct->PLL.PLLP));
        assert_param(IS_RCC_PLLQ_VALUE(RCC_OscInitStruct->PLL.PLLQ));
        
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();
        
        /* Wait till PLL is ready */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)
        {
           if(tickstart > PLL_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
        
 /* Configure the main PLL clock source, multiplication and division factors. */
        __HAL_RCC_PLL_CONFIG(RCC_OscInitStruct->PLL.PLLSource,
                             RCC_OscInitStruct->PLL.PLLM,
                             RCC_OscInitStruct->PLL.PLLN,
                             RCC_OscInitStruct->PLL.PLLP,
                             RCC_OscInitStruct->PLL.PLLQ);
        /* Enable the main PLL. */
        __HAL_RCC_PLL_ENABLE();
        
        /* Wait till PLL is ready */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
        {
           if(tickstart > PLL_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
      }
      else
      {
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();
        
        /* Wait till PLL is ready */  
        tickstart = 0UL;
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)
        {
           if(tickstart > PLL_TIMEOUT_VALUE)
           {
              return HAL_TIMEOUT;
           }
           delay_1ms();
           tickstart++;
        }
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }
  return HAL_OK;
} 

//----------------------------------------------------------------------------
static HAL_StatusTypeDef HAL_PWREx_EnableOverDrive(void)
{
  uint32_t tickstart = 0UL;

  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable the Over-drive to extend the clock frequency to 216 MHz */
  __HAL_PWR_OVERDRIVE_ENABLE();

  //tickstart = 0UL;
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODRDY))
  {
     if(tickstart > PWR_OVERDRIVE_TIMEOUT_VALUE)
     {
        return HAL_TIMEOUT;
     }
     delay_1ms();
     tickstart++;
  }
  
  /* Enable the Over-drive switch */
  __HAL_PWR_OVERDRIVESWITCHING_ENABLE();

  tickstart = 0UL;
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODSWRDY))
  {
     if(tickstart > PWR_OVERDRIVE_TIMEOUT_VALUE)
     {
        return HAL_TIMEOUT;
     }
     delay_1ms();
     tickstart++;
  } 
  return HAL_OK;
}
 

/**
  * @brief  Initializes the CPU, AHB and APB busses clocks according to the specified 
  *         parameters in the RCC_ClkInitStruct.
  * @param  RCC_ClkInitStruct: pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency: FLASH Latency, this parameter depend on device selected
  * 
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency 
  *         and updated by HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @note   The HSI is used (enabled by hardware) as system clock source after
  *         startup from Reset, wake-up from STOP and STANDBY mode, or in case
  *         of failure of the HSE used directly or indirectly as system clock
  *         (if the Clock Security System CSS is enabled).
  *           
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after startup delay or PLL locked). 
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready. 
  *         You can use HAL_RCC_GetClockConfig() function to know which clock is
  *         currently used as system clock source.
  * @note   Depending on the device voltage range, the software has to set correctly
  *         HPRE[3:0] bits to ensure that HCLK not exceed the maximum allowed frequency
  *         (for more details refer to section above "Initialization/de-initialization functions")
  * @retval None
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef * RCC_ClkInitStruct, 
                                      uint32_t FLatency)
{
  uint32_t tickstart;// = 0UL;
 
  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));
 
  /* To correctly read data from FLASH memory, the number of wait states (LATENCY) 
     must be correctly programmed according to the frequency of the CPU clock 
     (HCLK) and the supply voltage of the device. */
  
  /* Increasing the CPU frequency */
  if(FLatency > (FLASH->ACR & FLASH_ACR_LATENCY))
  {    
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLatency)
    {
      return HAL_ERROR;
    }

    /*-------------------------- HCLK Configuration --------------------------*/
    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
    {
      assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
      MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
    }

    /*------------------------- SYSCLK Configuration ---------------------------*/ 
    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
    {    
      assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));
      
      /* HSE is selected as System Clock Source */
      if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        /* Check the HSE ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
        {
          return HAL_ERROR;
        }
      }
      /* PLL is selected as System Clock Source */
      else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
      {
        /* Check the PLL ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
        {
          return HAL_ERROR;
        }
      }
      /* HSI is selected as System Clock Source */
      else
      {
        /* Check the HSI ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
          return HAL_ERROR;
        }
      }

      __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);

      tickstart = 0UL;
      if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSE)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
      else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
      else
      {
        while(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
    }    
  }
  /* Decreasing the CPU frequency */
  else
  {
    /*-------------------------- HCLK Configuration --------------------------*/
    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
    {
      assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
      MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
    }

    /*------------------------- SYSCLK Configuration -------------------------*/
    if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
    {    
      assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));
      
      /* HSE is selected as System Clock Source */
      if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        /* Check the HSE ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
        {
          return HAL_ERROR;
        }
      }
      /* PLL is selected as System Clock Source */
      else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
      {
        /* Check the PLL ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
        {
          return HAL_ERROR;
        }
      }
      /* HSI is selected as System Clock Source */
      else
      {
        /* Check the HSI ready flag */  
        if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
          return HAL_ERROR;
        }
      }
      __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);

      tickstart = 0UL;
      if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSE)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
      else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
      else
      {
        while(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI)
        {
          if(tickstart > CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
          delay_1ms();
          tickstart++;
        }
      }
    }
    
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLatency)
    {
      return HAL_ERROR;
    }
 }

  /*-------------------------- PCLK1 Configuration ---------------------------*/ 
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
  }
  
  /*-------------------------- PCLK2 Configuration ---------------------------*/ 
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB2CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3));
  }

  /* Configure the source of time base considering new system clocks settings*/
  (void)HAL_InitTick (TICK_INT_PRIORITY);
  
  return HAL_OK;
}
 
//----------------------------------------------------------------------------
static uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
   return SysTick_Config(TicksNumb);
} 

//----------------------------------------------------------------------------
static uint32_t HAL_RCC_GetHCLKFreq(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  SystemCoreClock = HAL_RCC_GetClocksFreq_Ex(&RCC_Clocks) >> 
                          APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> 
                                POSITION_VAL(RCC_CFGR_HPRE)];
  return SystemCoreClock;
} 

//----------------------------------------------------------------------------
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /*Configure the SysTick to have interrupt in 1ms time basis*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000UL);

  /*Configure the SysTick IRQ priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0UL);

  /* Return function status */
  return HAL_OK;
} 

/**
  * @brief  Returns the SYSCLK frequency
  *        
  * @note   The system frequency computed by this function is not the real 
  *         frequency in the chip. It is calculated based on the predefined 
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(**) 
  *           or HSI_VALUE(*) multiplied/divided by the PLL factors.         
  * @note     (*) HSI_VALUE is a constant defined in stm32f7xx_hal_conf.h file (default value
  *               16 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in stm32f7xx_hal_conf.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *                  
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *           
  * @note   This function can be used by the user application to compute the 
  *         baudrate for the communication peripherals or configure other parameters.
  *           
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *         
  *               
  * @retval SYSCLK frequency
  */
uint32_t HAL_RCC_GetClocksFreq_Ex(RCC_ClocksTypeDef * RCC_Clocks)
{
   uint32_t pllm; 
   uint32_t pllvco = 0UL; 
   uint32_t pllp;
   uint32_t sysclockfreq = 0UL;
   uint32_t tmp;
   uint32_t presc;

  /* Get SYSCLK source -------------------------------------------------------*/

   switch (RCC->CFGR & RCC_CFGR_SWS)
   {
      case RCC_SYSCLKSOURCE_STATUS_HSI:  /* HSI used as system clock source */
    
         sysclockfreq = HSI_VALUE;
         break;
    
      case RCC_SYSCLKSOURCE_STATUS_HSE:  /* HSE used as system clock  source */
    
         sysclockfreq = HSE_VALUE;
         break;
    
      case RCC_SYSCLKSOURCE_STATUS_PLLCLK:  /* PLL used as system clock  source */
    
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLP */

         pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
         if(__HAL_RCC_GET_PLL_OSCSOURCE() != RCC_PLLCFGR_PLLSRC_HSI)
         {
            if(pllm != 0UL)
            { 
             /* HSE used as PLL clock source */
               pllvco = ((HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> POSITION_VAL(RCC_PLLCFGR_PLLN)));
            }
         }
         else
         {
            if(pllm != 0UL)
            { 
            /* HSI used as PLL clock source */
               pllvco = ((HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> POSITION_VAL(RCC_PLLCFGR_PLLN)));    
            }  
         }
         pllp = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> POSITION_VAL(RCC_PLLCFGR_PLLP)) + 1 ) *2);
      
         if(pllp != 0UL)
         { 
            sysclockfreq = pllvco/pllp;
         }
         break;
    
      default:
    
         sysclockfreq = HSI_VALUE;
         break;
   }

   RCC_Clocks->SYSCLK_Frequency = sysclockfreq;

  /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/
  
  /* Get HCLK prescaler */
   tmp = RCC->CFGR & RCC_CFGR_HPRE;
   tmp = tmp >> 4UL;
   presc = APBAHBPrescTable[tmp];

  /* HCLK clock frequency */
   RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

  /* Get PCLK1 prescaler */
   tmp = RCC->CFGR & RCC_CFGR_PPRE1;
   tmp = tmp >> 10UL;
   presc = APBAHBPrescTable[tmp];

  /* PCLK1 clock frequency */
   RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

  /* Get PCLK2 prescaler */
   tmp = RCC->CFGR & RCC_CFGR_PPRE2;
   tmp = tmp >> 13UL;
   presc = APBAHBPrescTable[tmp];

  /* PCLK2 clock frequency */
   RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

   return sysclockfreq;
} 

/**
  * @brief  Sets the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f7xxxx.h))
  * @param  PreemptPriority: The preemption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority 
  * @param  SubPriority: the subpriority level for the IRQ channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.          
  * @retval None
  */
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{ 
   uint32_t prioritygroup;// = 0UL;
  
  /* Check the parameters */
   assert_param(IS_NVIC_SUB_PRIORITY(SubPriority));
   assert_param(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));
  
   prioritygroup = NVIC_GetPriorityGrouping();
  
   NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
}
 

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */

//----------------------------------------------------------------------------
void rcc_init(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

//  core_cm7.h

   SCB_EnableICache();
   SCB_EnableDCache();
	
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* Set voltage scaling */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	
//  PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
//  8000000/8 *432 = 432 Mhz/2 = 216 MHz
//  432/9= 48
	
  /* Enable HSE Oscillator and activate PLL with HSE as source */

   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState       = RCC_HSE_ON; // RCC_HSE_BYPASS;
   RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 8;
   RCC_OscInitStruct.PLL.PLLN = 432;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 9;

   if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      //Error_Handler();
   }

	/* Activate the Over-Drive mode */
	if(HAL_PWREx_EnableOverDrive() != HAL_OK) 
   {
      //Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */

   RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK | 
                                      RCC_CLOCKTYPE_HCLK | 
                                      RCC_CLOCKTYPE_PCLK1 | 
                                      RCC_CLOCKTYPE_PCLK2;

   RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
   if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
   {
     // Error_Handler();
   }
}

//----------------------------------------------------------------------------
// This delay is not very accurate, but it is good enough for the its purpose
//----------------------------------------------------------------------------
void delay_1ms(void)
{
   unsigned int clock;
   unsigned int loops_per_ms;
   RCC_ClocksTypeDef RCC_Clocks;

   clock = HAL_RCC_GetClocksFreq_Ex(&RCC_Clocks) >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> POSITION_VAL(RCC_CFGR_HPRE)];

   loops_per_ms = (clock/1000UL) / 3UL; //3 clock cycles per loop
   if(loops_per_ms == 0U)
   {
      loops_per_ms++;
   }
   do_delay(loops_per_ms); // Parameter - in R0
}

void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
   assert_param(IS_USART_ALL_PERIPH(USARTx));
   assert_param(IS_USART_DMAREQ(USART_DMAReq));  
   assert_param(IS_FUNCTIONAL_STATE(NewState)); 

   if(NewState != DISABLE)
   {
    /* Enable the DMA transfer for selected requests by setting the DMAT and/or
       DMAR bits in the USART CR3 register */
      USARTx->CR3 |= USART_DMAReq;
   }
   else
   {
    /* Disable the DMA transfer for selected requests by clearing the DMAT and/or
       DMAR bits in the USART CR3 register */
      USARTx->CR3 &= (uint16_t)~USART_DMAReq;
   }
}

/**
  * @brief  Clears the USARTx's pending flags.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
  *         UART peripheral.
  * @param  USART_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5).
  *            @arg USART_FLAG_LBD:  LIN Break detection flag.
  *            @arg USART_FLAG_TC:   Transmission Complete flag.
  *            @arg USART_FLAG_RXNE: Receive data register not empty flag.
  *   
  * @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *          error) and IDLE (Idle line detected) flags are cleared by software 
  *          sequence: a read operation to USART_SR register (USART_GetFlagStatus()) 
  *          followed by a read operation to USART_DR register (USART_ReceiveData()).
  * @note   RXNE flag can be also cleared by a read to the USART_DR register 
  *          (USART_ReceiveData()).
  * @note   TC flag can be also cleared by software sequence: a read operation to 
  *          USART_SR register (USART_GetFlagStatus()) followed by a write operation
  *          to USART_DR register (USART_SendData()).
  * @note   TXE flag is cleared only by a write to the USART_DR register 
  *          (USART_SendData()).
  *   
  * @retval None
  */
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  /* Check the parameters */
   assert_param(IS_USART_ALL_PERIPH(USARTx));
   assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));

  /* The CTS flag is not available for UART4 and UART5 */
   if((USART_FLAG & USART_FLAG_CTS) == USART_FLAG_CTS)
   {
      assert_param(IS_USART_1236_PERIPH(USARTx));
   } 
       
   USARTx->ISR = (uint16_t)~USART_FLAG;   // YVT
}

/**
  * @brief  Enables or disables the specified USART peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
  *         UART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters */
   assert_param(IS_USART_ALL_PERIPH(USARTx));
   assert_param(IS_FUNCTIONAL_STATE(NewState));
  
   if(NewState != DISABLE)
   {
    /* Enable the selected USART by setting the UE bit in the CR1 register */
      USARTx->CR1 |= USART_CR1_UE;
   }
   else
   {
    /* Disable the selected USART by clearing the UE bit in the CR1 register */
      USARTx->CR1 &= (uint16_t)~((uint16_t)USART_CR1_UE);
   }
}

void USART_Init(USART_TypeDef * USARTx, USART_InitTypeDef * USART_InitStruct)
{
   uint32_t tmpreg;// = 0U; 
   uint32_t apbclock;// = 0U;
   uint32_t integerdivider;// = 0U;
   uint32_t fractionaldivider;// = 0U;

   RCC_ClocksTypeDef RCC_ClocksStatus;

  /* Check the parameters */
   assert_param(IS_USART_ALL_PERIPH(USARTx));
   assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
   assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
   assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
   assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
   assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
   assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

  /* The hardware flow control is available only for USART1, USART2, USART3 and USART6 */
   if(USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
   {
      assert_param(IS_USART_1236_PERIPH(USARTx));
   }

/*---------------------------- USART CR2 Configuration -----------------------*/
   tmpreg = USARTx->CR2;

  /* Clear STOP[13:12] bits */
   tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit :
      Set STOP[13:12] bits according to USART_StopBits value */
   tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;
  
  /* Write to USART CR2 */
   USARTx->CR2 = (uint16_t)tmpreg;

/*---------------------------- USART CR1 Configuration -----------------------*/
   tmpreg = USARTx->CR1;

  /* Clear M, PCE, PS, TE and RE bits */
   tmpreg &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);

  /* Configure the USART Word Length, Parity and mode: 
     Set the M bits according to USART_WordLength value 
     Set PCE and PS bits according to USART_Parity value
     Set TE and RE bits according to USART_Mode value */
   tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | 
                  USART_InitStruct->USART_Parity |
                  USART_InitStruct->USART_Mode;

  /* Write to USART CR1 */
   USARTx->CR1 = (uint16_t)tmpreg;

/*---------------------------- USART CR3 Configuration -----------------------*/  
   tmpreg = USARTx->CR3;

  /* Clear CTSE and RTSE bits */
   tmpreg &= (uint32_t)~((uint32_t)CR3_CLEAR_MASK);

  /* Configure the USART HFC : 
      Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
   tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

  /* Write to USART CR3 */
   USARTx->CR3 = (uint16_t)tmpreg;

/*---------------------------- USART BRR Configuration -----------------------*/
  /* Configure the USART Baud Rate */
   (void)HAL_RCC_GetClocksFreq_Ex(&RCC_ClocksStatus);

   if((USARTx == USART1) || (USARTx == USART6))
   {
      apbclock = RCC_ClocksStatus.PCLK2_Frequency;
   }
   else
   {
      apbclock = RCC_ClocksStatus.PCLK1_Frequency;
   }
  
  /* Determine the integer part */
   if((USARTx->CR1 & USART_CR1_OVER8) != 0)
   {
     /* Integer part computing in case Oversampling mode is 8 Samples */
      integerdivider = ((25 * apbclock) / (2 * (USART_InitStruct->USART_BaudRate)));    
   }
   else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
   {
    /* Integer part computing in case Oversampling mode is 16 Samples */
      integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));    
   }
   tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
   fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
   if((USARTx->CR1 & USART_CR1_OVER8) != 0)
   {
      tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
   }
   else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
   {
      tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
   }
  
  /* Write to USART BRR register */
   USARTx->BRR = (uint16_t)tmpreg;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg  LED1
  *     @arg  LED2
  *     @arg  LED3
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef_Ex  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral.
  * @param  GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef  * GPIOx, GPIO_InitTypeDef_Ex * GPIO_Init)
{
  uint32_t position;// = 0x00;
  uint32_t ioposition;// = 0x00;
  uint32_t iocurrent;// = 0x00;
  uint32_t temp;// = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

  /* Configure the port pins */
  for(position = 0; position < GPIO_NUMBER; position++)
  {
    /* Get the IO position */
    ioposition = ((uint32_t)0x01) << position;
    /* Get the current IO position */
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

    if(iocurrent == ioposition)
    {
      /*--------------------- GPIO Mode Configuration ------------------------*/
      /* In case of Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_AF_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check the Alternate function parameter */
        assert_param(IS_GPIO_AF(GPIO_Init->Alternate));
        
        /* Configure Alternate function mapped with the current IO */
        temp = GPIOx->AFR[position >> 3];
        temp &= ~((uint32_t)0xF << ((uint32_t)(position & (uint32_t)0x07) * 4)) ;
        temp |= ((uint32_t)(GPIO_Init->Alternate) << (((uint32_t)position & (uint32_t)0x07) * 4));
        GPIOx->AFR[position >> 3] = temp;
      }

      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODER0 << (position * 2));
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2));
      GPIOx->MODER = temp;

      /* In case of Output or Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_PP) ||
         (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check the Speed parameter */
        assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
        /* Configure the IO Speed */
        temp = GPIOx->OSPEEDR; 
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2));
        temp |= (GPIO_Init->Speed << (position * 2));
        GPIOx->OSPEEDR = temp;

        /* Configure the IO Output Type */
        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;
        temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4) << position);
        GPIOx->OTYPER = temp;
      }

      /* Activate the Pull-up or Pull down resistor for the current IO */
      temp = GPIOx->PUPDR;
      temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2));
      temp |= ((GPIO_Init->Pull) << (position * 2));
      GPIOx->PUPDR = temp;

      /*--------------------- EXTI Mode Configuration ------------------------*/
      /* Configure the External Interrupt or event for the current IO */
      if((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)
      {
        /* Enable SYSCFG Clock */
        __HAL_RCC_SYSCFG_CLK_ENABLE();

        temp = SYSCFG->EXTICR[position >> 2];
        temp &= ~(((uint32_t)0x0F) << (4UL * (position & 0x03UL)));
        temp |= ((uint32_t)(GPIO_GET_INDEX(GPIOx)) << (4UL * (position & 0x03UL)));
        SYSCFG->EXTICR[position >> 2] = temp;

        /* Clear EXTI line configuration */
        temp = EXTI->IMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
        {
          temp |= iocurrent;
        }
        EXTI->IMR = temp;

        temp = EXTI->EMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
        {
          temp |= iocurrent;
        }
        EXTI->EMR = temp;

        /* Clear Rising Falling edge configuration */
        temp = EXTI->RTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)
        {
          temp |= iocurrent;
        }
        EXTI->RTSR = temp;

        temp = EXTI->FTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)
        {
          temp |= iocurrent;
        }
        EXTI->FTSR = temp;
      }
    }
  }
}

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_BSRR register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @param  PinState: specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_PIN_ACTION(PinState));

  if(PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16;
  }
}

//----------------------------------------------------------------------------
void HAL_GPIO_TogglePin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR ^= GPIO_Pin;
}


/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg  LED1
  *     @arg  LED2
  *     @arg  LED3 
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}


void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);

void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_AHB1_RESET_PERIPH(RCC_AHB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHB1RSTR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1RSTR &= ~RCC_AHB1Periph;
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



