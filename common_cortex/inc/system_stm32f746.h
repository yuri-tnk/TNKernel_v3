 /* @attention
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



#ifndef  SYSTEM_STM32F7XX_H
#define  SYSTEM_STM32F7XX_H

#define UNUSED(x) ((void)(x)) 

// This is the HAL system configuration section

#define  VDD_VALUE                    ((uint32_t)3300) /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY            ((uint32_t)0x0F) /*!< tick interrupt priority */
#define  USE_RTOS                     0
#define  PREFETCH_ENABLE              1
#define  ART_ACCLERATOR_ENABLE        1 /* To enable instruction cache and prefetch */ 

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */ 

void delay_1ms(void);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);

extern uint32_t SystemCoreClock;

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;


typedef enum 
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
 
typedef struct
{
  uint32_t PLLState;   /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Config                      */

  uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
                            This parameter must be a value of @ref RCC_PLL_Clock_Source               */           

  uint32_t PLLM;       /*!< PLLM: Division factor for PLL VCO input clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 63    */        

  uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL VCO output clock.
                            This parameter must be a number between Min_Data = 100 and Max_Data = 432 */

  uint32_t PLLP;       /*!< PLLP: Division factor for main system clock (SYSCLK).
                            This parameter must be a value of @ref RCC_PLLP_Clock_Divider             */

  uint32_t PLLQ;       /*!< PLLQ: Division factor for OTG FS, SDMMC and RNG clocks.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 15    */

}RCC_PLLInitTypeDef; 

/**
  * @brief  RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition  
  */
typedef struct
{
  uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                      This parameter can be a value of @ref RCC_Oscillator_Type                   */

  uint32_t HSEState;             /*!< The new state of the HSE.
                                      This parameter can be a value of @ref RCC_HSE_Config                        */

  uint32_t LSEState;             /*!< The new state of the LSE.
                                      This parameter can be a value of @ref RCC_LSE_Config                        */
                                          
  uint32_t HSIState;             /*!< The new state of the HSI.
                                      This parameter can be a value of @ref RCC_HSI_Config                        */

  uint32_t HSICalibrationValue;   /*!< The calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                       This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */
                               
  uint32_t LSIState;             /*!< The new state of the LSI.
                                      This parameter can be a value of @ref RCC_LSI_Config                        */

  RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters                                                    */      

}RCC_OscInitTypeDef; 

/**
  * @brief  RCC System, AHB and APB busses clock configuration structure definition  
  */
typedef struct
{
  uint32_t ClockType;             /*!< The clock to be configured.
                                       This parameter can be a value of @ref RCC_System_Clock_Type */
  
  uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                       This parameter can be a value of @ref RCC_System_Clock_Source    */

  uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                       This parameter can be a value of @ref RCC_AHB_Clock_Source       */

  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;

/** 
  * @brief GPIO Init structure definition  
  */ 
typedef struct
{
  uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */

  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref GPIO_mode_define */

  uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref GPIO_pull_define */

  uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                           This parameter can be a value of @ref GPIO_speed_define */

  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins. 
                            This parameter can be a value of @ref GPIO_Alternate_function_selection */
}GPIO_InitTypeDef_Ex;
 
typedef struct
{
  uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
  uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
  uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
}RCC_ClocksTypeDef;

uint32_t HAL_RCC_GetClocksFreq_Ex(RCC_ClocksTypeDef* RCC_Clocks);


#define RCC_OSCILLATORTYPE_NONE            ((uint32_t)0x00000000)
#define RCC_OSCILLATORTYPE_HSE             ((uint32_t)0x00000001)
#define RCC_OSCILLATORTYPE_HSI             ((uint32_t)0x00000002)
#define RCC_OSCILLATORTYPE_LSE             ((uint32_t)0x00000004)
#define RCC_OSCILLATORTYPE_LSI             ((uint32_t)0x00000008) 

/** @defgroup RCC_System_Clock_Source_Status System Clock Source Status
  * @{
  */
#define RCC_SYSCLKSOURCE_STATUS_HSI      RCC_CFGR_SWS_HSI   /*!< HSI used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_HSE      RCC_CFGR_SWS_HSE   /*!< HSE used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK   RCC_CFGR_SWS_PLL   /*!< PLL used as system clock */ 


/** @brief  Macro to get the clock source used as system clock.
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following:
  *              - RCC_SYSCLKSOURCE_STATUS_HSI: HSI used as system clock.
  *              - RCC_SYSCLKSOURCE_STATUS_HSE: HSE used as system clock.
  *              - RCC_SYSCLKSOURCE_STATUS_PLLCLK: PLL used as system clock.
  */     
#define __HAL_RCC_GET_SYSCLK_SOURCE() ((uint32_t)(RCC->CFGR & RCC_CFGR_SWS)) 


/** @brief  Check RCC flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready.
  *            @arg RCC_FLAG_HSERDY: HSE oscillator clock ready.
  *            @arg RCC_FLAG_PLLRDY: Main PLL clock ready.
  *            @arg RCC_FLAG_PLLI2SRDY: PLLI2S clock ready.
  *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready.
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready.
  *            @arg RCC_FLAG_BORRST: POR/PDR or BOR reset.
  *            @arg RCC_FLAG_PINRST: Pin reset.
  *            @arg RCC_FLAG_PORRST: POR/PDR reset.
  *            @arg RCC_FLAG_SFTRST: Software reset.
  *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset.
  *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset.
  *            @arg RCC_FLAG_LPWRRST: Low Power reset.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define RCC_FLAG_MASK  ((uint8_t)0x1F)
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((((__FLAG__) >> 5) == 1)? RCC->CR :((((__FLAG__) >> 5) == 2) ? RCC->BDCR :((((__FLAG__) >> 5) == 3)? RCC->CSR :RCC->CIR))) & ((uint32_t)1 << ((__FLAG__) & RCC_FLAG_MASK)))!= 0)? 1 : 0) 


/* Flags in the CR register */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_PLLI2SRDY               ((uint8_t)0x3B)
#define RCC_FLAG_PLLSAIRDY               ((uint8_t)0x3C) 


/** @defgroup RCC_HSE_Config RCC HSE Config
  * @{
  */
#define RCC_HSE_OFF                      ((uint32_t)0x00000000)
#define RCC_HSE_ON                       RCC_CR_HSEON
#define RCC_HSE_BYPASS                   ((uint32_t)(RCC_CR_HSEBYP | RCC_CR_HSEON)) 

/**
  * @brief  Macro to configure the External High Speed oscillator (__HSE__).
  * @note   After enabling the HSE (RCC_HSE_ON or RCC_HSE_Bypass), the application
  *         software should wait on HSERDY flag to be set indicating that HSE clock
  *         is stable and can be used to clock the PLL and/or system clock.
  * @note   HSE state can not be changed if it is used directly or through the
  *         PLL as system clock. In this case, you have to select another source
  *         of the system clock then change the HSE state (ex. disable it).
  * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.  
  * @note   This function reset the CSSON bit, so if the clock security system(CSS)
  *         was previously enabled you have to enable it again after calling this
  *         function.    
  * @param  __STATE__: specifies the new state of the HSE.
  *         This parameter can be one of the following values:
  *            @arg RCC_HSE_OFF: turn OFF the HSE oscillator, HSERDY flag goes low after
  *                              6 HSE oscillator clock cycles.
  *            @arg RCC_HSE_ON: turn ON the HSE oscillator.
  *            @arg RCC_HSE_BYPASS: HSE oscillator bypassed with external clock.
  */
#define __HAL_RCC_HSE_CONFIG(__STATE__) \
                    do {                                     \
                      CLEAR_BIT(RCC->CR, RCC_CR_HSEON);      \
                      if((__STATE__) == RCC_HSE_ON)          \
                      {                                      \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);   \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);      \
                      }                                      \
                      else if((__STATE__) == RCC_HSE_BYPASS) \
                      {                                      \
                        SET_BIT(RCC->CR, RCC_CR_HSEBYP);     \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);      \
                      }                                      \
                      else                                   \
                      {                                      \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);   \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEON);    \
                        }                                    \
                    } while(0) 

/**
  * @brief  Macro to configure the External Low Speed oscillator (LSE).
  * @note   Transition LSE Bypass to LSE On and LSE On to LSE Bypass are not supported by this macro. 
  *         User should request a transition to LSE Off first and then LSE On or LSE Bypass.  
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using 
  *         HAL_PWR_EnableBkUpAccess() function before to configure the LSE
  *         (to be done once after reset).  
  * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_BYPASS), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  __STATE__: specifies the new state of the LSE.
  *         This parameter can be one of the following values:
  *            @arg RCC_LSE_OFF: turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg RCC_LSE_ON: turn ON the LSE oscillator.
  *            @arg RCC_LSE_BYPASS: LSE oscillator bypassed with external clock.
  */
#define __HAL_RCC_LSE_CONFIG(__STATE__) \
                    do {                                       \
                      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);    \
                      if((__STATE__) == RCC_LSE_ON)            \
                      {                                        \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP); \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);    \
                      }                                        \
                      else if((__STATE__) == RCC_LSE_BYPASS)   \
                      {                                        \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);   \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);    \
                      }                                        \
                      else                                     \
                      {                                        \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP); \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);  \
                      }                                        \
                    } while(0) 

#define HSE_TIMEOUT_VALUE          HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define LSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */  


/** @defgroup RCC_HSI_Config RCC HSI Config
  * @{
  */
#define RCC_HSI_OFF                    ((uint32_t)0x00000000)
#define RCC_HSI_ON                     RCC_CR_HSION 

/** @brief  Macro to adjust the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  __HSICALIBRATIONVALUE__: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  */

#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(__HSICALIBRATIONVALUE__) (MODIFY_REG(RCC->CR,\
        RCC_CR_HSITRIM, (uint32_t)(__HSICALIBRATIONVALUE__) << POSITION_VAL(RCC_CR_HSITRIM))) 

/** @brief  Macros to enable or disable the Internal High Speed oscillator (HSI).
  * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
  *         It is used (enabled by hardware) as system clock source after startup
  *         from Reset, wakeup from STOP and STANDBY mode, or in case of failure
  *         of the HSE used directly or indirectly as system clock (if the Clock
  *         Security System CSS is enabled).             
  * @note   HSI can not be stopped if it is used as system clock source. In this case,
  *         you have to select another source of the system clock then stop the HSI.  
  * @note   After enabling the HSI, the application software should wait on HSIRDY
  *         flag to be set indicating that HSI clock is stable and can be used as
  *         system clock source.  
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles.  
  */
#define __HAL_RCC_HSI_ENABLE() (RCC->CR |= (RCC_CR_HSION))
#define __HAL_RCC_HSI_DISABLE() (RCC->CR &= ~(RCC_CR_HSION)) 


/** @defgroup RCC_LSI_Config RCC LSI Config
  * @{
  */
#define RCC_LSI_OFF                    ((uint32_t)0x00000000)
#define RCC_LSI_ON                     RCC_CSR_LSION 

/** @brief  Macros to enable or disable the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on 
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  * @note   LSI can not be disabled if the IWDG is running.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles. 
  */
#define __HAL_RCC_LSI_ENABLE()  (RCC->CSR |= (RCC_CSR_LSION))
#define __HAL_RCC_LSI_DISABLE() (RCC->CSR &= ~(RCC_CSR_LSION)) 

/* Flags in the CSR register */
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCC_FLAG_BORRST                  ((uint8_t)0x79)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7F) 

#define __HAL_RCC_PWR_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);\
                                        UNUSED(tmpreg); \
                                      } while(0)									  
 
#define RCC_DBP_TIMEOUT_VALUE      ((uint32_t)100)
#define RCC_LSE_TIMEOUT_VALUE      ((uint32_t)5000) 

/** @defgroup RCC_LSE_Config RCC LSE Config
  * @{
  */
#define RCC_LSE_OFF                    ((uint32_t)0x00000000)
#define RCC_LSE_ON                     RCC_BDCR_LSEON
#define RCC_LSE_BYPASS                 ((uint32_t)(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON)) 

/* Flags in the BDCR register */
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41) 

/** @defgroup RCC_PLL_Config RCC PLL Config
  * @{
  */
#define RCC_PLL_NONE                   ((uint32_t)0x00000000)
#define RCC_PLL_OFF                    ((uint32_t)0x00000001)
#define RCC_PLL_ON                     ((uint32_t)0x00000002) 

/** @brief  Macros to enable or disable the main PLL.
  * @note   After enabling the main PLL, the application software should wait on 
  *         PLLRDY flag to be set indicating that PLL clock is stable and can
  *         be used as system clock source.
  * @note   The main PLL can not be disabled if it is used as system clock source
  * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCC_PLL_ENABLE() SET_BIT(RCC->CR, RCC_CR_PLLON)
#define __HAL_RCC_PLL_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLLON) 

/** @brief  Macro to configure the main PLL clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL is disabled.
  * @param  __RCC_PLLSource__: specifies the PLL entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLLSOURCE_HSI: HSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSOURCE_HSE: HSE oscillator clock selected as PLL clock entry
  * @note   This clock source (RCC_PLLSource) is common for the main PLL and PLLI2S.  
  * @param  __PLLM__: specifies the division factor for PLL VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   You have to set the PLLM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 2 MHz to limit PLL jitter.
  * @param  __PLLN__: specifies the multiplication factor for PLL VCO output clock
  *         This parameter must be a number between Min_Data = 100 and Max_Data = 432.
  * @note   You have to set the PLLN parameter correctly to ensure that the VCO
  *         output frequency is between 100 and 432 MHz.
  * @param  __PLLP__: specifies the division factor for main system clock (SYSCLK)
  *         This parameter must be a number in the range {2, 4, 6, or 8}.
  * @note   You have to set the PLLP parameter correctly to not exceed 216 MHz on
  *         the System clock frequency.
  * @param  __PLLQ__: specifies the division factor for OTG FS, SDMMC and RNG clocks
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.
  * @note   If the USB OTG FS is used in your application, you have to set the
  *         PLLQ parameter correctly to have 48 MHz clock for the USB. However,
  *         the SDMMC and RNG need a frequency lower than or equal to 48 MHz to work
  *         correctly.
  */
#define __HAL_RCC_PLL_CONFIG(__RCC_PLLSource__, __PLLM__, __PLLN__, __PLLP__, __PLLQ__)\
                            (RCC->PLLCFGR = (0x20000000 | (__PLLM__) | ((__PLLN__) << POSITION_VAL(RCC_PLLCFGR_PLLN)) | \
                            ((((__PLLP__) >> 1) -1) << POSITION_VAL(RCC_PLLCFGR_PLLP)) | (__RCC_PLLSource__) | \
                            ((__PLLQ__) << POSITION_VAL(RCC_PLLCFGR_PLLQ)))) 


/** @brief Macros to enable or disable the Over drive mode.
  */
#define __HAL_PWR_OVERDRIVE_ENABLE() (PWR->CR1 |= (uint32_t)PWR_CR1_ODEN)
#define __HAL_PWR_OVERDRIVE_DISABLE() (PWR->CR1 &= (uint32_t)(~PWR_CR1_ODEN)) 


/** @brief  Check PWR flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *           This parameter can be one of the following values:
  *            @arg PWR_FLAG_WU: Wake Up flag. This flag indicates that a wakeup event 
  *                  was received on the internal wakeup line in standby mode (RTC alarm (Alarm A or Alarm B),
  *                  RTC Tamper event, RTC TimeStamp event or RTC Wakeup)).
  *            @arg PWR_FLAG_SB: StandBy flag. This flag indicates that the system was
  *                  resumed from StandBy mode.    
  *            @arg PWR_FLAG_PVDO: PVD Output. This flag is valid only if PVD is enabled 
  *                  by the HAL_PWR_EnablePVD() function. The PVD is stopped by Standby mode 
  *                  For this reason, this bit is equal to 0 after Standby or reset
  *                  until the PVDE bit is set.
  *            @arg PWR_FLAG_BRR: Backup regulator ready flag. This bit is not reset 
  *                  when the device wakes up from Standby mode or by a system reset 
  *                  or power reset.  
  *            @arg PWR_FLAG_VOSRDY: This flag indicates that the Regulator voltage 
  *                 scaling output selection is ready.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_PWR_GET_FLAG(__FLAG__) ((PWR->CSR1 & (__FLAG__)) == (__FLAG__)) 

/** @defgroup PWREx_Over_Under_Drive_Flag PWREx Over Under Drive Flag
  * @{
  */
#define PWR_FLAG_ODRDY                  PWR_CSR1_ODRDY
#define PWR_FLAG_ODSWRDY                PWR_CSR1_ODSWRDY
#define PWR_FLAG_UDRDY                  PWR_CSR1_UDSWRDY 

/** @addtogroup PWREx_Private_Constants
  * @{
  */    
#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000
#define PWR_UDERDRIVE_TIMEOUT_VALUE  1000
#define PWR_BKPREG_TIMEOUT_VALUE     1000
#define PWR_VOSRDY_TIMEOUT_VALUE     1000 

/** @brief Macros to enable or disable the Over drive switching.
  */
#define __HAL_PWR_OVERDRIVESWITCHING_ENABLE() (PWR->CR1 |= (uint32_t)PWR_CR1_ODSWEN)
#define __HAL_PWR_OVERDRIVESWITCHING_DISABLE() (PWR->CR1 &= (uint32_t)(~PWR_CR1_ODSWEN)) 

#define __HAL_FLASH_SET_LATENCY(__LATENCY__) \
                  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(__LATENCY__)) 

/** @defgroup RCC_System_Clock_Type RCC System Clock Type
  * @{
  */
#define RCC_CLOCKTYPE_SYSCLK             ((uint32_t)0x00000001)
#define RCC_CLOCKTYPE_HCLK               ((uint32_t)0x00000002)
#define RCC_CLOCKTYPE_PCLK1              ((uint32_t)0x00000004)
#define RCC_CLOCKTYPE_PCLK2              ((uint32_t)0x00000008) 

/** @defgroup RCC_System_Clock_Source RCC System Clock Source
  * @{
  */
#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR_SW_HSI
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR_SW_HSE
#define RCC_SYSCLKSOURCE_PLLCLK          RCC_CFGR_SW_PLL 

/**
  * @brief Macro to configure the system clock source.
  * @param __RCC_SYSCLKSOURCE__: specifies the system clock source.
  * This parameter can be one of the following values:
  *              - RCC_SYSCLKSOURCE_HSI: HSI oscillator is used as system clock source.
  *              - RCC_SYSCLKSOURCE_HSE: HSE oscillator is used as system clock source.
  *              - RCC_SYSCLKSOURCE_PLLCLK: PLL output is used as system clock source.
  */
#define __HAL_RCC_SYSCLK_CONFIG(__RCC_SYSCLKSOURCE__) MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (__RCC_SYSCLKSOURCE__)) 

/** @brief  Macro to get the oscillator used as PLL clock source.
  * @retval The oscillator used as PLL clock source. The returned value can be one
  *         of the following:
  *              - RCC_PLLSOURCE_HSI: HSI oscillator is used as PLL clock source.
  *              - RCC_PLLSOURCE_HSE: HSE oscillator is used as PLL clock source.
  */
#define __HAL_RCC_GET_PLL_OSCSOURCE() ((uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC)) 


/** @defgroup RCC_PLL_Clock_Source PLL Clock Source
  * @{
  */
#define RCC_PLLSOURCE_HSI                RCC_PLLCFGR_PLLSRC_HSI
#define RCC_PLLSOURCE_HSE                RCC_PLLCFGR_PLLSRC_HSE 

//============================================================================
#define __HAL_PWR_VOLTAGESCALING_CONFIG(__REGULATOR__) do {                                                     \
                                                          __IO uint32_t tmpreg = 0x00;                               \
                                                            MODIFY_REG(PWR->CR1, PWR_CR1_VOS, (__REGULATOR__)); \
                                                            /* Delay after an RCC peripheral clock enabling */  \
                                                            tmpreg = READ_BIT(PWR->CR1, PWR_CR1_VOS);           \
                                                            UNUSED(tmpreg);                                     \
                                                          } while(0)
 

/** @defgroup PWR_Regulator_Voltage_Scale PWR Regulator Voltage Scale
  * @{
  */
#define PWR_REGULATOR_VOLTAGE_SCALE1         PWR_CR1_VOS
#define PWR_REGULATOR_VOLTAGE_SCALE2         PWR_CR1_VOS_1
#define PWR_REGULATOR_VOLTAGE_SCALE3         PWR_CR1_VOS_0  

/** @defgroup RCC_AHB_Clock_Source RCC AHB Clock Source
  * @{
  */
#define RCC_SYSCLK_DIV1                  RCC_CFGR_HPRE_DIV1
#define RCC_SYSCLK_DIV2                  RCC_CFGR_HPRE_DIV2
#define RCC_SYSCLK_DIV4                  RCC_CFGR_HPRE_DIV4
#define RCC_SYSCLK_DIV8                  RCC_CFGR_HPRE_DIV8
#define RCC_SYSCLK_DIV16                 RCC_CFGR_HPRE_DIV16
#define RCC_SYSCLK_DIV64                 RCC_CFGR_HPRE_DIV64
#define RCC_SYSCLK_DIV128                RCC_CFGR_HPRE_DIV128
#define RCC_SYSCLK_DIV256                RCC_CFGR_HPRE_DIV256
#define RCC_SYSCLK_DIV512                RCC_CFGR_HPRE_DIV512 

/** @defgroup RCC_APB1_APB2_Clock_Source RCC APB1/APB2 Clock Source
  * @{
  */
#define RCC_HCLK_DIV1                    RCC_CFGR_PPRE1_DIV1
#define RCC_HCLK_DIV2                    RCC_CFGR_PPRE1_DIV2
#define RCC_HCLK_DIV4                    RCC_CFGR_PPRE1_DIV4
#define RCC_HCLK_DIV8                    RCC_CFGR_PPRE1_DIV8
#define RCC_HCLK_DIV16                   RCC_CFGR_PPRE1_DIV16 

/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0                FLASH_ACR_LATENCY_0WS   /*!< FLASH Zero Latency cycle      */
#define FLASH_LATENCY_1                FLASH_ACR_LATENCY_1WS   /*!< FLASH One Latency cycle       */
#define FLASH_LATENCY_2                FLASH_ACR_LATENCY_2WS   /*!< FLASH Two Latency cycles      */
#define FLASH_LATENCY_3                FLASH_ACR_LATENCY_3WS   /*!< FLASH Three Latency cycles    */
#define FLASH_LATENCY_4                FLASH_ACR_LATENCY_4WS   /*!< FLASH Four Latency cycles     */
#define FLASH_LATENCY_5                FLASH_ACR_LATENCY_5WS   /*!< FLASH Five Latency cycles     */
#define FLASH_LATENCY_6                FLASH_ACR_LATENCY_6WS   /*!< FLASH Six Latency cycles      */
#define FLASH_LATENCY_7                FLASH_ACR_LATENCY_7WS   /*!< FLASH Seven Latency cycles    */
#define FLASH_LATENCY_8                FLASH_ACR_LATENCY_8WS   /*!< FLASH Eight Latency cycles    */
#define FLASH_LATENCY_9                FLASH_ACR_LATENCY_9WS   /*!< FLASH Nine Latency cycles     */
#define FLASH_LATENCY_10               FLASH_ACR_LATENCY_10WS  /*!< FLASH Ten Latency cycles      */
#define FLASH_LATENCY_11               FLASH_ACR_LATENCY_11WS  /*!< FLASH Eleven Latency cycles   */
#define FLASH_LATENCY_12               FLASH_ACR_LATENCY_12WS  /*!< FLASH Twelve Latency cycles   */
#define FLASH_LATENCY_13               FLASH_ACR_LATENCY_13WS  /*!< FLASH Thirteen Latency cycles */
#define FLASH_LATENCY_14               FLASH_ACR_LATENCY_14WS  /*!< FLASH Fourteen Latency cycles */
#define FLASH_LATENCY_15               FLASH_ACR_LATENCY_15WS  /*!< FLASH Fifteen Latency cycles  */ 

/** @defgroup RCC_PLLP_Clock_Divider PLLP Clock Divider
  * @{
  */
#define RCC_PLLP_DIV2                  ((uint32_t)0x00000002)
#define RCC_PLLP_DIV4                  ((uint32_t)0x00000004)
#define RCC_PLLP_DIV6                  ((uint32_t)0x00000006)
#define RCC_PLLP_DIV8                  ((uint32_t)0x00000008) 

/** @defgroup CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  * @{
  */
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bits for pre-emption priority
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bits for pre-emption priority
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority
                                                                 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority */


#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xF20880CC) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xF20886CC) == 0x00) && ((PERIPH) != 0x00))

#define IS_RCC_AHB1_CLOCK_PERIPH(PERIPH) ((((PERIPH) & 0x010BE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x51FE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_LPMODE_PERIPH(PERIPH) ((((PERIPH) & 0x01106800) == 0x00) && ((PERIPH) != 0x00))

void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);

#define __HAL_RCC_USART1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_DMA1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_USART1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_GPIOA_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)
    
#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_GPIOD_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)


#define __HAL_RCC_SYSCFG_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_USART3_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg = 0x00; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

/** @defgroup GPIOEx_Get_Port_Index GPIO Get Port Index
  * @{
  */
#define GPIO_GET_INDEX(__GPIOx__)   (uint8_t)(((__GPIOx__) == (GPIOA))? 0U :\
                                              ((__GPIOx__) == (GPIOB))? 1U :\
                                              ((__GPIOx__) == (GPIOC))? 2U :\
                                              ((__GPIOx__) == (GPIOD))? 3U :\
                                              ((__GPIOx__) == (GPIOE))? 4U :\
                                              ((__GPIOx__) == (GPIOF))? 5U :\
                                              ((__GPIOx__) == (GPIOG))? 6U :\
                                              ((__GPIOx__) == (GPIOH))? 7U :\
                                              ((__GPIOx__) == (GPIOI))? 8U :\
                                              ((__GPIOx__) == (GPIOJ))? 9U : 10U)			

/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_PIN_MASK              ((uint32_t)0x0000FFFF) /* PIN mask for assert test */

/** @defgroup GPIO_mode_define GPIO mode define
  * @brief GPIO Configuration Mode 
  *        Elements values convention: 0xX0yz00YZ
  *           - X  : GPIO mode or EXTI Mode
  *           - y  : External IT or Event trigger detection 
  *           - z  : IO configuration on External IT or Event
  *           - Y  : Output type (Push Pull or Open Drain)
  *           - Z  : IO Direction mode (Input, Output, Alternate or Analog)
  * @{
  */ 
#define  GPIO_MODE_INPUT                        ((uint32_t)0x00000000)   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001)   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_OUTPUT_OD                    ((uint32_t)0x00000011)   /*!< Output Open Drain Mode                */
#define  GPIO_MODE_AF_PP                        ((uint32_t)0x00000002)   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_AF_OD                        ((uint32_t)0x00000012)   /*!< Alternate Function Open Drain Mode    */

 /** @defgroup GPIO_pull_define GPIO pull define
   * @brief GPIO Pull-Up or Pull-Down Activation
   * @{
   */  
#define  GPIO_NOPULL        ((uint32_t)0x00000000)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        ((uint32_t)0x00000001)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      ((uint32_t)0x00000002)   /*!< Pull-down activation                */

/** @defgroup GPIO_speed_define  GPIO speed define
  * @brief GPIO Output Maximum frequency
  * @{
  */  
#define  GPIO_SPEED_FREQ_LOW         ((uint32_t)0x00000000)  /*!< Low speed     */
#define  GPIO_SPEED_FREQ_MEDIUM      ((uint32_t)0x00000001)  /*!< Medium speed  */
#define  GPIO_SPEED_FREQ_HIGH        ((uint32_t)0x00000002)  /*!< Fast speed    */
#define  GPIO_SPEED_FREQ_VERY_HIGH   ((uint32_t)0x00000003)  /*!< High speed    */

#endif   /* #ifndef  SYSTEM_STM32F7XX_H */
