/* ---------------------------------------------------------------------------

    TNKernel startup file for STMicroelectronics STM32F070B processor

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

   /*  STM32F070xB device  */

  .section .vectors, "ax"
  .syntax	unified
  .arch     armv6-m
  .align    3
  .code     16

  .global   _vectors

_vectors:
  .word __stack_end__ -16
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
  .word 0
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler

 /* ---- External Interrupts ---- */

        .word     WWDG_IRQHandler                /* Window Watchdog  */
        .word     0                              /* Reserved          */
        .word     RTC_IRQHandler                 /* RTC through EXTI Line   */
        .word     FLASH_IRQHandler               /* FLASH  */
        .word     RCC_IRQHandler                 /* RCC  */
        .word     EXTI0_1_IRQHandler             /* EXTI Line 0 and 1  */
        .word     EXTI2_3_IRQHandler             /* EXTI Line 2 and 3  */
        .word     EXTI4_15_IRQHandler            /* EXTI Line 4 to 15  */
        .word     0                              /* Reserved  */
        .word     DMA1_Channel1_IRQHandler       /* DMA1 Channel 1  */
        .word     DMA1_Channel2_3_IRQHandler     /* DMA1 Channel 2 and Channel 3  */
        .word     DMA1_Channel4_5_IRQHandler     /* DMA1 Channel 4 and Channel 5  */
        .word     ADC1_IRQHandler                /* ADC1  */
        .word     TIM1_BRK_UP_TRG_COM_IRQHandler /* TIM1 Break, Update, Trigger and Commutation  */
        .word     TIM1_CC_IRQHandler             /* TIM1 Capture Compare  */
        .word     0                              /* Reserved  */
        .word     TIM3_IRQHandler                /* TIM3  */
        .word     TIM6_IRQHandler                /* TIM6  */
        .word     TIM7_IRQHandler                /* TIM7  */
        .word     TIM14_IRQHandler               /* TIM14 */
        .word     TIM15_IRQHandler               /* TIM15 */
        .word     TIM16_IRQHandler               /* TIM16 */
        .word     TIM17_IRQHandler               /* TIM17 */
        .word     I2C1_IRQHandler                /* I2C1  */
        .word     I2C2_IRQHandler                /* I2C2  */
        .word     SPI1_IRQHandler                /* SPI1  */
        .word     SPI2_IRQHandler                /* SPI2  */
        .word     USART1_IRQHandler              /* USART1 */
        .word     USART2_IRQHandler              /* USART2 */
        .word     USART3_4_IRQHandler            /* USART3 and USART4 */
        .word     0                              /* Reserved  */
        .word     USB_IRQHandler                 /* USB  */

 /* ------------------------------------------------------------- */

  .section .init, "ax"
  .syntax unified
  .cpu cortex-m0
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

     /* ----- Clear BSS (zero init)  Cortex-M0 */

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

    /*---- Copy Initialized data from FLASH to RAM Cortex-M0 */

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

     /*   bl  SystemInit */

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
def_int_vector PendSV_Handler
def_int_vector SysTick_Handler

def_int_vector WWDG_IRQHandler               
def_int_vector RTC_IRQHandler                
def_int_vector FLASH_IRQHandler              
def_int_vector RCC_IRQHandler                
def_int_vector EXTI0_1_IRQHandler            
def_int_vector EXTI2_3_IRQHandler            
def_int_vector EXTI4_15_IRQHandler           
def_int_vector DMA1_Channel1_IRQHandler      
def_int_vector DMA1_Channel2_3_IRQHandler    
def_int_vector DMA1_Channel4_5_IRQHandler    
def_int_vector ADC1_IRQHandler               
def_int_vector TIM1_BRK_UP_TRG_COM_IRQHandler
def_int_vector TIM1_CC_IRQHandler            
def_int_vector TIM3_IRQHandler               
def_int_vector TIM6_IRQHandler               
def_int_vector TIM7_IRQHandler               
def_int_vector TIM14_IRQHandler              
def_int_vector TIM15_IRQHandler              
def_int_vector TIM16_IRQHandler              
def_int_vector TIM17_IRQHandler              
def_int_vector I2C1_IRQHandler               
def_int_vector I2C2_IRQHandler               
def_int_vector SPI1_IRQHandler               
def_int_vector SPI2_IRQHandler               
def_int_vector USART1_IRQHandler             
def_int_vector USART2_IRQHandler             
def_int_vector USART3_4_IRQHandler           
def_int_vector USB_IRQHandler                

/* -------------------------------------------------------------------------- */











