/* ---------------------------------------------------------------------------

       TNKernel startup file for STMicroelectronics STM32F053 processor

       Assembler: GCC

       Copyright © 2004,2016 Yuri Tiomkin
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

   /*  STM32F053 device  */

  .section .vectors, "ax"
  .code 16
  .align 0
  .global _vectors

_vectors:
  .word __stack_end__
  .word reset_handler
  .word NMI_Handler
  .word HardFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler

 /* ---- External Interrupts ---- */

     .word     WWDG_IRQHandler                   /* Window WatchDog              */
     .word     PVD_IRQHandler                    /* PVD through EXTI Line detection */
     .word     RTC_IRQHandler                    /* RTC through the EXTI line     */
     .word     FLASH_IRQHandler                  /* FLASH                        */
     .word     RCC_CRS_IRQHandler                /* RCC and CRS                  */
     .word     EXTI0_1_IRQHandler                /* EXTI Line 0 and 1            */
     .word     EXTI2_3_IRQHandler                /* EXTI Line 2 and 3            */
     .word     EXTI4_15_IRQHandler               /* EXTI Line 4 to 15            */
     .word     TSC_IRQHandler                    /* TSC                           */
     .word     DMA1_Channel1_IRQHandler          /* DMA1 Channel 1               */
     .word     DMA1_Channel2_3_IRQHandler        /* DMA1 Channel 2 and Channel 3 */
     .word     DMA1_Channel4_5_6_7_IRQHandler    /* DMA1 Channel 4, Channel 5, Channel 6 and Channel 7*/
     .word     ADC1_COMP_IRQHandler              /* ADC1, COMP1 and COMP2        */
     .word     LPTIM1_IRQHandler                 /* LPTIM1                       */
     .word     0                                 /* Reserved                     */
     .word     TIM2_IRQHandler                   /* TIM2                         */
     .word     0                                 /* Reserved                     */
     .word     TIM6_DAC_IRQHandler               /* TIM6 and DAC                 */
     .word     0                                 /* Reserved                     */
     .word     0                                 /* Reserved                     */
     .word     TIM21_IRQHandler                  /* TIM21                        */
     .word     0                                 /* Reserved                     */
     .word     TIM22_IRQHandler                  /* TIM22                        */
     .word     I2C1_IRQHandler                   /* I2C1                         */
     .word     I2C2_IRQHandler                   /* I2C2                         */
     .word     SPI1_IRQHandler                   /* SPI1                         */
     .word     SPI2_IRQHandler                   /* SPI2                         */
     .word     USART1_IRQHandler                 /* USART1                       */
     .word     USART2_IRQHandler                 /* USART2                       */
     .word     RNG_LPUART1_IRQHandler            /* RNG and LPUART1              */
     .word     LCD_IRQHandler                    /* LCD                          */
     .word     USB_IRQHandler                    /* USB                          */

 /* ------------------------------------------------------------- */

  .section .init, "ax"
  .syntax unified
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb
 
  .thumb_func
  .global  reset_handler
  .extern  SystemInit

reset_handler:

        LDR     R0, =SystemInit
        BLX     R0

 .extern     __bss_start
 .extern     __bss_end__

     /* ----- Clear BSS (zero init)  Cortex-M0/M0P */

        ldr    r1, =__bss_start
        ldr    r2, =__bss_end__

        movs   r0, 0

        subs   r2, r1
        ble    .L_loop3_done

.L_loop3:
        subs   r2, #4
        str    r0, [r1, r2]
        bgt    .L_loop3

.L_loop3_done:

    /*---- Copy Initialized data from FLASH to RAM Cortex-M0/M0P */

 .extern  _etext
 .extern  _data
 .extern  _edata

        ldr    r1, =_etext
        ldr    r2, =_data
        ldr    r3, =_edata

	     subs   r3, r2
        ble	   .L_loop1_done

.L_loop1:
        subs   r3, #4
        ldr	   r0, [r1,r3]
        str    r0, [r2,r3]
        bgt    .L_loop1

.L_loop1_done:

   /* ---- */

      .extern   main

        movs   r0, #0
        mov    r1, r0
        bl     main
        b   .

/* ------------------------------------------------------------------------- */

.macro def_int_vector name=
  .thumb_func
  .weak \name
\name:
  b .
.endm

def_int_vector NMI_Handler
def_int_vector HardFault_Handler
def_int_vector SVC_Handler
def_int_vector DebugMon_Handler
def_int_vector PendSV_Handler
def_int_vector SysTick_Handler

def_int_vector WWDG_IRQHandler
def_int_vector PVD_IRQHandler
def_int_vector RTC_IRQHandler
def_int_vector FLASH_IRQHandler
def_int_vector RCC_CRS_IRQHandler
def_int_vector EXTI0_1_IRQHandler
def_int_vector EXTI2_3_IRQHandler
def_int_vector EXTI4_15_IRQHandler
def_int_vector TSC_IRQHandler
def_int_vector DMA1_Channel1_IRQHandler
def_int_vector DMA1_Channel2_3_IRQHandler
def_int_vector DMA1_Channel4_5_6_7_IRQHandler
def_int_vector ADC1_COMP_IRQHandler
def_int_vector LPTIM1_IRQHandler
def_int_vector TIM2_IRQHandler
def_int_vector TIM6_DAC_IRQHandler
def_int_vector TIM21_IRQHandler
def_int_vector TIM22_IRQHandler
def_int_vector I2C1_IRQHandler
def_int_vector I2C2_IRQHandler
def_int_vector SPI1_IRQHandler
def_int_vector SPI2_IRQHandler
def_int_vector USART1_IRQHandler
def_int_vector USART2_IRQHandler
def_int_vector RNG_LPUART1_IRQHandler
def_int_vector LCD_IRQHandler
def_int_vector USB_IRQHandler

/* -------------------------------------------------------------------------- */












