/* ---------------------------------------------------------------------------

       TNKernel startup file for STM32F446ZE  processor

       Assembler: GCC
  
       Copyright © 2004, 2016 Yuri Tiomkin
       All rights reserved.

   Permission to use, copy, modify, and distribute this software in source
   and binary forms and its documentation for any purpose and without fee
   is hereby granted, provided that the above copyright notice appear
   in all copies and that both that copyright notice and this permission
   notice appear in supporting documentation.

   THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.
----------------------------------------------------------------------------- */


   /*  STM32F446ZE  device  */

  .section .vectors, "ax"
  .code 16
  .align 0
  .global _vectors

_vectors:

  .word __stack_end__
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDG_IRQHandler                   /* Window WatchDog              */                                        
  .word     PVD_IRQHandler                    /* PVD through EXTI Line detection */                        
  .word     TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */            
  .word     RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */                      
  .word     FLASH_IRQHandler                  /* FLASH                        */                                          
  .word     RCC_IRQHandler                    /* RCC                          */                                            
  .word     EXTI0_IRQHandler                  /* EXTI Line0                   */                        
  .word     EXTI1_IRQHandler                  /* EXTI Line1                   */                          
  .word     EXTI2_IRQHandler                  /* EXTI Line2                   */                          
  .word     EXTI3_IRQHandler                  /* EXTI Line3                   */                          
  .word     EXTI4_IRQHandler                  /* EXTI Line4                   */                          
  .word     DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */                  
  .word     DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */                   
  .word     DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */                   
  .word     DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */                   
  .word     DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */                   
  .word     DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */                   
  .word     DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */                   
  .word     ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */                   
  .word     CAN1_TX_IRQHandler                /* CAN1 TX                      */                         
  .word     CAN1_RX0_IRQHandler               /* CAN1 RX0                     */                          
  .word     CAN1_RX1_IRQHandler               /* CAN1 RX1                     */                          
  .word     CAN1_SCE_IRQHandler               /* CAN1 SCE                     */                          
  .word     EXTI9_5_IRQHandler                /* External Line[9:5]s          */                          
  .word     TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9          */         
  .word     TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10        */         
  .word     TIM1_TRG_COM_TIM11_IRQHandler     /* TIM1 Trigger and Commutation and TIM11 */
  .word     TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */                          
  .word     TIM2_IRQHandler                   /* TIM2                         */                   
  .word     TIM3_IRQHandler                   /* TIM3                         */                   
  .word     TIM4_IRQHandler                   /* TIM4                         */                   
  .word     I2C1_EV_IRQHandler                /* I2C1 Event                   */                          
  .word     I2C1_ER_IRQHandler                /* I2C1 Error                   */                          
  .word     I2C2_EV_IRQHandler                /* I2C2 Event                   */                          
  .word     I2C2_ER_IRQHandler                /* I2C2 Error                   */                            
  .word     SPI1_IRQHandler                   /* SPI1                         */                   
  .word     SPI2_IRQHandler                   /* SPI2                         */                   
  .word     USART1_IRQHandler                 /* USART1                       */                   
  .word     USART2_IRQHandler                 /* USART2                       */                   
  .word     USART3_IRQHandler                 /* USART3                       */                   
  .word     EXTI15_10_IRQHandler              /* External Line[15:10]s        */                          
  .word     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */                 
  .word     OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI line */                       
  .word     TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */         
  .word     TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */         
  .word     TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
  .word     TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */                          
  .word     DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */                          
  .word     FMC_IRQHandler                    /* FMC                          */                   
  .word     SDIO_IRQHandler                   /* SDIO                         */                   
  .word     TIM5_IRQHandler                   /* TIM5                         */                   
  .word     SPI3_IRQHandler                   /* SPI3                         */                   
  .word     UART4_IRQHandler                  /* UART4                        */                   
  .word     UART5_IRQHandler                  /* UART5                        */                   
  .word     TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */                   
  .word     TIM7_IRQHandler                   /* TIM7                         */
  .word     DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */                   
  .word     DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */                   
  .word     DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */                   
  .word     DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */                   
  .word     DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */                   
  .word     0                                 /* Reserved                     */                   
  .word     0                                 /* Reserved                     */                     
  .word     CAN2_TX_IRQHandler                /* CAN2 TX                      */                          
  .word     CAN2_RX0_IRQHandler               /* CAN2 RX0                     */                          
  .word     CAN2_RX1_IRQHandler               /* CAN2 RX1                     */                          
  .word     CAN2_SCE_IRQHandler               /* CAN2 SCE                     */                          
  .word     OTG_FS_IRQHandler                 /* USB OTG FS                   */                   
  .word     DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */                   
  .word     DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */                   
  .word     DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */                   
  .word     USART6_IRQHandler                 /* USART6                       */                    
  .word     I2C3_EV_IRQHandler                /* I2C3 event                   */                          
  .word     I2C3_ER_IRQHandler                /* I2C3 error                   */                          
  .word     OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */                   
  .word     OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */                   
  .word     OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */                         
  .word     OTG_HS_IRQHandler                 /* USB OTG HS                   */                   
  .word     DCMI_IRQHandler                   /* DCMI                         */                   
  .word     0                                 /* Reserved                     */                   
  .word     0                                 /* Reserved                     */
  .word     FPU_IRQHandler                    /* FPU                          */
  .word     0                                 /* Reserved                     */      
  .word     0                                 /* Reserved                     */
  .word     SPI4_IRQHandler                   /* SPI4                         */
  .word     0                                 /* Reserved 					          */
  .word     0                                 /* Reserved					            */
  .word     SAI1_IRQHandler                   /* SAI1						              */
  .word     0                                 /* Reserved					            */
  .word     0                                 /* Reserved					            */
  .word     0                                 /* Reserved    				          */
  .word     SAI2_IRQHandler                   /* SAI2                         */
  .word     QUADSPI_IRQHandler                /* QuadSPI                      */
  .word     CEC_IRQHandler                    /* CEC                          */
  .word     SPDIF_RX_IRQHandler               /* SPDIF RX                     */
  .word     FMPI2C1_Event_IRQHandler          /* FMPI2C 1 Event               */
  .word     FMPI2C1_Error_IRQHandler          /* FMPI2C 1 Error               */

 /* ------------------------------------------------------------- */

  .section .init, "ax"
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

  .thumb_func

  .global  Reset_Handler
  .extern  SystemInit
 

Reset_Handler:

 .extern     __bss_start
 .extern     __bss_end__

     /* ----- Clear BSS (zero init) */

        movs   r0,#0
        ldr    r1,=__bss_start
        ldr    r2,=__bss_end__
2:      cmp    r1,r2
        itt    lo
        strlo  r0, [r1],#4
        blo    2b

    /*---- Copy Initialized data from FLASH to RAM */

 .extern  _etext
 .extern  _data
 .extern  _edata

        ldr    r1,=_etext
        ldr    r2,=_data
        ldr    r3,=_edata
1:      cmp    r2,r3
        ittt   lo
        ldrlo  r0,[r1],#4
        strlo  r0,[r2],#4
        blo    1b

   /* ---- */

        .extern   main

        bl     SystemInit

        movs   r0, #0
        mov    r1, r0
        bl     main
        b      .

/* ------------------------------------------------------------------------- */

.macro def_int_vector name=
  .thumb_func
  .weak \name
\name:
  b .
.endm

def_int_vector  NMI_Handler
def_int_vector  HardFault_Handler
def_int_vector  MemManage_Handler
def_int_vector  BusFault_Handler
def_int_vector  UsageFault_Handler
def_int_vector  SVC_Handler
def_int_vector  DebugMon_Handler
def_int_vector  PendSV_Handler
def_int_vector  SysTick_Handler

def_int_vector  WWDG_IRQHandler
def_int_vector  PVD_IRQHandler
def_int_vector  TAMP_STAMP_IRQHandler
def_int_vector  RTC_WKUP_IRQHandler
def_int_vector  FLASH_IRQHandler
def_int_vector  RCC_IRQHandler
def_int_vector  EXTI0_IRQHandler
def_int_vector  EXTI1_IRQHandler
def_int_vector  EXTI2_IRQHandler
def_int_vector  EXTI3_IRQHandler
def_int_vector  EXTI4_IRQHandler
def_int_vector  DMA1_Stream0_IRQHandler
def_int_vector  DMA1_Stream1_IRQHandler
def_int_vector  DMA1_Stream2_IRQHandler
def_int_vector  DMA1_Stream3_IRQHandler
def_int_vector  DMA1_Stream4_IRQHandler
def_int_vector  DMA1_Stream5_IRQHandler
def_int_vector  DMA1_Stream6_IRQHandler
def_int_vector  ADC_IRQHandler
def_int_vector  CAN1_TX_IRQHandler
def_int_vector  CAN1_RX0_IRQHandler
def_int_vector  CAN1_RX1_IRQHandler
def_int_vector  CAN1_SCE_IRQHandler
def_int_vector  EXTI9_5_IRQHandler
def_int_vector  TIM1_BRK_TIM9_IRQHandler
def_int_vector  TIM1_UP_TIM10_IRQHandler
def_int_vector  TIM1_TRG_COM_TIM11_IRQHandler
def_int_vector  TIM1_CC_IRQHandler
def_int_vector  TIM2_IRQHandler
def_int_vector  TIM3_IRQHandler
def_int_vector  TIM4_IRQHandler
def_int_vector  I2C1_EV_IRQHandler
def_int_vector  I2C1_ER_IRQHandler
def_int_vector  I2C2_EV_IRQHandler
def_int_vector  I2C2_ER_IRQHandler
def_int_vector  SPI1_IRQHandler
def_int_vector  SPI2_IRQHandler
def_int_vector  USART1_IRQHandler
def_int_vector  USART2_IRQHandler
def_int_vector  USART3_IRQHandler
def_int_vector  EXTI15_10_IRQHandler
def_int_vector  RTC_Alarm_IRQHandler
def_int_vector  OTG_FS_WKUP_IRQHandler
def_int_vector  TIM8_BRK_TIM12_IRQHandler
def_int_vector  TIM8_UP_TIM13_IRQHandler
def_int_vector  TIM8_TRG_COM_TIM14_IRQHandler
def_int_vector  TIM8_CC_IRQHandler
def_int_vector  DMA1_Stream7_IRQHandler
def_int_vector  FMC_IRQHandler
def_int_vector  SDIO_IRQHandler
def_int_vector  TIM5_IRQHandler
def_int_vector  SPI3_IRQHandler
def_int_vector  UART4_IRQHandler
def_int_vector  UART5_IRQHandler
def_int_vector  TIM6_DAC_IRQHandler
def_int_vector  TIM7_IRQHandler
def_int_vector  DMA2_Stream0_IRQHandler
def_int_vector  DMA2_Stream1_IRQHandler
def_int_vector  DMA2_Stream2_IRQHandler
def_int_vector  DMA2_Stream3_IRQHandler
def_int_vector  DMA2_Stream4_IRQHandler
def_int_vector  CAN2_TX_IRQHandler
def_int_vector  CAN2_RX0_IRQHandler
def_int_vector  CAN2_RX1_IRQHandler
def_int_vector  CAN2_SCE_IRQHandler
def_int_vector  OTG_FS_IRQHandler
def_int_vector  DMA2_Stream5_IRQHandler
def_int_vector  DMA2_Stream6_IRQHandler
def_int_vector  DMA2_Stream7_IRQHandler
def_int_vector  USART6_IRQHandler
def_int_vector  I2C3_EV_IRQHandler
def_int_vector  I2C3_ER_IRQHandler
def_int_vector  OTG_HS_EP1_OUT_IRQHandler
def_int_vector  OTG_HS_EP1_IN_IRQHandler
def_int_vector  OTG_HS_WKUP_IRQHandler
def_int_vector  OTG_HS_IRQHandler
def_int_vector  DCMI_IRQHandler
def_int_vector  FPU_IRQHandler
def_int_vector  SPI4_IRQHandler
def_int_vector  SAI1_IRQHandler
def_int_vector  SAI2_IRQHandler
def_int_vector  QUADSPI_IRQHandler
def_int_vector  CEC_IRQHandler
def_int_vector  SPDIF_RX_IRQHandler
def_int_vector  FMPI2C1_Event_IRQHandler
def_int_vector  FMPI2C1_Error_IRQHandler

/* -------------------------------------------------------------------------- */

