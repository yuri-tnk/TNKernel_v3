/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/DataExchangeDMA/stm32f0xx_conf.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:/ /www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F0XX_CONF_H
#define STM32F0XX_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion */
//#include "stm32f0xx_adc.h"
//#include "stm32f0xx_can.h"
//#include "stm32f0xx_cec.h"
//#include "stm32f0xx_comp.h"
//#include "stm32f0xx_crc.h"
//#include "stm32f0xx_crs.h"
//#include "stm32f0xx_dac.h"
//#include "stm32f0xx_dbgmcu.h"
#include "stm32f0xx_dma.h"
//#include "stm32f0xx_exti.h"
//#include "stm32f0xx_flash.h"
#include "stm32f0xx_gpio.h"
//#include "stm32f0xx_i2c.h"
//#include "stm32f0xx_iwdg.h"
//#include "stm32f0xx_pwr.h"
#include "stm32f0xx_rcc.h"
//#include "stm32f0xx_rtc.h"
//#include "stm32f0xx_spi.h"
//#include "stm32f0xx_syscfg.h"
//#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
//#include "stm32f0xx_wwdg.h"
//#include "stm32f0xx_misc.h"  // High level functions for NVIC and SysTick (add-on to CMSIS functions)

typedef enum 
{
  LED2 = 0,
  LED_GREEN = LED2
} Led_TypeDef;

#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define LEDn                               1
#define LED2_PIN                           GPIO_PIN_5
#define LED2_GPIO_PORT                     GPIOA

void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_DeInit(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);


#define HSI_VALUE  8000000UL
#define HSE_VALUE  8000000UL

#if !defined  (HSI14_VALUE) 
#define HSI14_VALUE ((uint32_t)14000000) /*!< Value of the Internal High Speed oscillator for ADC in Hz.
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
#endif /* HSI14_VALUE */

#if !defined  (HSI48_VALUE) 
#define HSI48_VALUE ((uint32_t)48000000) /*!< Value of the Internal High Speed oscillator for USB in Hz.
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
#endif /* HSI48_VALUE */

#if !defined  (LSI_VALUE) 
#define LSI_VALUE  ((uint32_t)40000)    /*!< Value of the Internal Low Speed oscillator in Hz
                                             The real value may vary depending on the variations
                                             in voltage and temperature.  */
#endif /* LSI_VALUE */

#if !defined  (LSE_VALUE) 
#define LSE_VALUE  ((uint32_t)32768)    /*!< Value of the External Low Speed oscillator in Hz */
#endif /* LSE_VALUE */


#if !defined  (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT   ((uint16_t)0x5000) /*!< Time out for HSE start up */
#endif /* HSE_STARTUP_TIMEOUT */

/******************  Bit definition for USART_CR1 register  *******************/
// #define  USART_CR1_UE                        ((uint32_t)0x00000001)            // !< USART Enable 
#define  USART_CR1_UESM                      ((uint32_t)0x00000002)            /*!< USART Enable in STOP Mode */

#define  USART_CR2_LBDL                      ((uint32_t)0x00000020)            /*!< LIN Break Detection Length */

#if 0

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADDM7                     ((uint32_t)0x00000010)            /*!< 7-bit or 4-bit Address Detection */
#define  USART_CR2_LBDIE                     ((uint32_t)0x00000040)            /*!< LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint32_t)0x00000100)            /*!< Last Bit Clock pulse */
#define  USART_CR2_CPHA                      ((uint32_t)0x00000200)            /*!< Clock Phase */
#define  USART_CR2_CPOL                      ((uint32_t)0x00000400)            /*!< Clock Polarity */
#define  USART_CR2_CLKEN                     ((uint32_t)0x00000800)            /*!< Clock Enable */
#define  USART_CR2_STOP                      ((uint32_t)0x00003000)            /*!< STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint32_t)0x00001000)            /*!< Bit 0 */
#define  USART_CR2_STOP_1                    ((uint32_t)0x00002000)            /*!< Bit 1 */
#define  USART_CR2_LINEN                     ((uint32_t)0x00004000)            /*!< LIN mode enable */
#define  USART_CR2_SWAP                      ((uint32_t)0x00008000)            /*!< SWAP TX/RX pins */
#define  USART_CR2_RXINV                     ((uint32_t)0x00010000)            /*!< RX pin active level inversion */
#define  USART_CR2_TXINV                     ((uint32_t)0x00020000)            /*!< TX pin active level inversion */
#define  USART_CR2_DATAINV                   ((uint32_t)0x00040000)            /*!< Binary data inversion */
#define  USART_CR2_MSBFIRST                  ((uint32_t)0x00080000)            /*!< Most Significant Bit First */
#define  USART_CR2_ABREN                     ((uint32_t)0x00100000)            /*!< Auto Baud-Rate Enable*/
#define  USART_CR2_ABRMODE                   ((uint32_t)0x00600000)            /*!< ABRMOD[1:0] bits (Auto Baud-Rate Mode) */
#define  USART_CR2_ABRMODE_0                 ((uint32_t)0x00200000)            /*!< Bit 0 */
#define  USART_CR2_ABRMODE_1                 ((uint32_t)0x00400000)            /*!< Bit 1 */
#define  USART_CR2_RTOEN                     ((uint32_t)0x00800000)            /*!< Receiver Time-Out enable */
#define  USART_CR2_ADD                       ((uint32_t)0xFF000000UL)            /*!< Address of the USART node */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint32_t)0x00000001)            /*!< Error Interrupt Enable */
#define  USART_CR3_IREN                      ((uint32_t)0x00000002)            /*!< IrDA mode Enable */
#define  USART_CR3_HDSEL                     ((uint32_t)0x00000008)            /*!< Half-Duplex Selection */
#define  USART_CR3_SCEN                      ((uint32_t)0x00000020)            /*!< SmartCard mode enable */
#define  USART_CR3_DMAR                      ((uint32_t)0x00000040)            /*!< DMA Enable Receiver */
#define  USART_CR3_DMAT                      ((uint32_t)0x00000080)            /*!< DMA Enable Transmitter */
#define  USART_CR3_RTSE                      ((uint32_t)0x00000100)            /*!< RTS Enable */
#define  USART_CR3_CTSE                      ((uint32_t)0x00000200)            /*!< CTS Enable */
#define  USART_CR3_CTSIE                     ((uint32_t)0x00000400)            /*!< CTS Interrupt Enable */
#define  USART_CR3_ONEBIT                    ((uint32_t)0x00000800)            /*!< One sample bit method enable */
#define  USART_CR3_OVRDIS                    ((uint32_t)0x00001000)            /*!< Overrun Disable */
#define  USART_CR3_DDRE                      ((uint32_t)0x00002000)            /*!< DMA Disable on Reception Error */
#define  USART_CR3_DEM                       ((uint32_t)0x00004000)            /*!< Driver Enable Mode */
#define  USART_CR3_DEP                       ((uint32_t)0x00008000)            /*!< Driver Enable Polarity Selection */
#define  USART_CR3_SCARCNT_0                 ((uint32_t)0x00020000)            /*!< Bit 0 */
#define  USART_CR3_SCARCNT_1                 ((uint32_t)0x00040000)            /*!< Bit 1 */
#define  USART_CR3_SCARCNT_2                 ((uint32_t)0x00080000)            /*!< Bit 2 */

#endif

#define  USART_CR3_NACK                      ((uint32_t)0x00000010)            /*!< SmartCard NACK enable */
#define  USART_CR3_IRLP                      ((uint32_t)0x00000004)            /*!< IrDA Low-Power */

#define  USART_CR3_WUS                       ((uint32_t)0x00300000)            /*!< WUS[1:0] bits (Wake UP Interrupt Flag Selection) */
#define  USART_CR3_WUS_0                     ((uint32_t)0x00100000)            /*!< Bit 0 */
#define  USART_CR3_WUS_1                     ((uint32_t)0x00200000)            /*!< Bit 1 */
#define  USART_CR3_WUFIE                     ((uint32_t)0x00400000)            /*!< Wake Up Interrupt Enable */

#define  USART_CR3_SCARCNT                   ((uint32_t)0x000E0000)            /*!< SCARCNT[2:0] bits (SmartCard Auto-Retry Count) */

/*******************  Bit definition for RCC_CFGR2 register  ******************/
/* PREDIV1 configuration */
#define  RCC_CFGR2_PREDIV1                   ((uint32_t)0x0000000F)        /*!< PREDIV1[3:0] bits */
#define  RCC_CFGR2_PREDIV1_0                 ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_CFGR2_PREDIV1_1                 ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  RCC_CFGR2_PREDIV1_2                 ((uint32_t)0x00000004)        /*!< Bit 2 */
#define  RCC_CFGR2_PREDIV1_3                 ((uint32_t)0x00000008)        /*!< Bit 3 */

#define  RCC_CFGR2_PREDIV1_DIV1              ((uint32_t)0x00000000)        /*!< PREDIV1 input clock not divided */
#define  RCC_CFGR2_PREDIV1_DIV2              ((uint32_t)0x00000001)        /*!< PREDIV1 input clock divided by 2 */
#define  RCC_CFGR2_PREDIV1_DIV3              ((uint32_t)0x00000002)        /*!< PREDIV1 input clock divided by 3 */
#define  RCC_CFGR2_PREDIV1_DIV4              ((uint32_t)0x00000003)        /*!< PREDIV1 input clock divided by 4 */
#define  RCC_CFGR2_PREDIV1_DIV5              ((uint32_t)0x00000004)        /*!< PREDIV1 input clock divided by 5 */
#define  RCC_CFGR2_PREDIV1_DIV6              ((uint32_t)0x00000005)        /*!< PREDIV1 input clock divided by 6 */
#define  RCC_CFGR2_PREDIV1_DIV7              ((uint32_t)0x00000006)        /*!< PREDIV1 input clock divided by 7 */
#define  RCC_CFGR2_PREDIV1_DIV8              ((uint32_t)0x00000007)        /*!< PREDIV1 input clock divided by 8 */
#define  RCC_CFGR2_PREDIV1_DIV9              ((uint32_t)0x00000008)        /*!< PREDIV1 input clock divided by 9 */
#define  RCC_CFGR2_PREDIV1_DIV10             ((uint32_t)0x00000009)        /*!< PREDIV1 input clock divided by 10 */
#define  RCC_CFGR2_PREDIV1_DIV11             ((uint32_t)0x0000000A)        /*!< PREDIV1 input clock divided by 11 */
#define  RCC_CFGR2_PREDIV1_DIV12             ((uint32_t)0x0000000B)        /*!< PREDIV1 input clock divided by 12 */
#define  RCC_CFGR2_PREDIV1_DIV13             ((uint32_t)0x0000000C)        /*!< PREDIV1 input clock divided by 13 */
#define  RCC_CFGR2_PREDIV1_DIV14             ((uint32_t)0x0000000D)        /*!< PREDIV1 input clock divided by 14 */
#define  RCC_CFGR2_PREDIV1_DIV15             ((uint32_t)0x0000000E)        /*!< PREDIV1 input clock divided by 15 */
#define  RCC_CFGR2_PREDIV1_DIV16             ((uint32_t)0x0000000F)        /*!< PREDIV1 input clock divided by 16 */

/*******************  Bit definition for RCC_CFGR3 register  ******************/
#define  RCC_CFGR3_USART1SW                  ((uint32_t)0x00000003)        /*!< USART1SW[1:0] bits */
#define  RCC_CFGR3_USART1SW_0                ((uint32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_CFGR3_USART1SW_1                ((uint32_t)0x00000002)        /*!< Bit 1 */
#define  RCC_CFGR3_I2C1SW                    ((uint32_t)0x00000010)        /*!< I2C1SW bits */
#define  RCC_CFGR3_CECSW                     ((uint32_t)0x00000040)        /*!< CECSW bits */
// #define  RCC_CFGR3_USBSW                     ((uint32_t)0x00000080)        // !< USBSW bits 
#define  RCC_CFGR3_ADCSW                     ((uint32_t)0x00000100)        /*!< ADCSW bits */
#define  RCC_CFGR3_USART2SW                  ((uint32_t)0x00030000)        /*!< USART2SW[1:0] bits */
#define  RCC_CFGR3_USART2SW_0                ((uint32_t)0x00010000)        /*!< Bit 0 */
#define  RCC_CFGR3_USART2SW_1                ((uint32_t)0x00020000)        /*!< Bit 1 */

#define  RCC_CFGR_MCO_PRE                    ((uint32_t)0x70000000)        /*!< MCO prescaler (these bits are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_1                  ((uint32_t)0x00000000)        /*!< MCO is divided by 1 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_2                  ((uint32_t)0x10000000)        /*!< MCO is divided by 2 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_4                  ((uint32_t)0x20000000)        /*!< MCO is divided by 4 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_8                  ((uint32_t)0x30000000)        /*!< MCO is divided by 8 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_16                 ((uint32_t)0x40000000)        /*!< MCO is divided by 16 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_32                 ((uint32_t)0x50000000)        /*!< MCO is divided by 32 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_64                 ((uint32_t)0x60000000)        /*!< MCO is divided by 64 (this bit are not available in the STM32F051 devices)*/
#define  RCC_CFGR_MCO_PRE_128                ((uint32_t)0x70000000)        /*!< MCO is divided by 128 (this bit are not available in the STM32F051 devices)*/



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */



/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */


#endif /* __STM32F0XX_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
