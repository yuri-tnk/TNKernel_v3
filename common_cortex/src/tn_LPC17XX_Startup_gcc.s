/* ---------------------------------------------------------------------------

      TNKernel startup file for NXP LPC17XX processors

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


  .section .vectors, "ax"
  .code 16
  .align 0
  .global _vectors

_vectors:
  .word (0x10008000 - 16)  /*__stack_end__ */
  .word reset_handler
  .word NMI_Handler
  .word HardFault_Handler
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
  .word 0xEFFE01CC         /* Checksum for FLASH stand-alone */
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler

 /* ---- External Interrupts ---- */

  .word WDT_IRQHandler            /* 16: Watchdog Timer */
  .word TIMER0_IRQHandler         /* 17: Timer0         */
  .word TIMER1_IRQHandler         /* 18: Timer1         */
  .word TIMER2_IRQHandler         /* 19: Timer2         */
  .word TIMER3_IRQHandler         /* 20: Timer3         */
  .word UART0_IRQHandler          /* 21: UART0          */
  .word UART1_IRQHandler          /* 22: UART1          */
  .word UART2_IRQHandler          /* 23: UART2          */
  .word UART3_IRQHandler          /* 24: UART3          */
  .word PWM1_IRQHandler           /* 25: PWM1           */
  .word I2C0_IRQHandler           /* 26: I2C0           */
  .word I2C1_IRQHandler           /* 27: I2C1           */
  .word I2C2_IRQHandler           /* 28: I2C2           */
  .word SPI_IRQHandler            /* 29: SPI            */
  .word SSP0_IRQHandler           /* 30: SSP0           */
  .word SSP1_IRQHandler           /* 31: SSP1           */
  .word PLL0_IRQHandler           /* 32: PLL0 Lock (Main PLL)           */
  .word RTC_IRQHandler            /* 33: Real Time Clock                */
  .word EINT0_IRQHandler          /* 34: External Interrupt 0           */
  .word EINT1_IRQHandler          /* 35: External Interrupt 1           */
  .word EINT2_IRQHandler          /* 36: External Interrupt 2           */
  .word EINT3_IRQHandler          /* 37: External Interrupt 3           */
  .word ADC_IRQHandler            /* 38: A/D Converter                  */
  .word BOD_IRQHandler            /* 39: Brown-Out Detect               */
  .word USB_IRQHandler            /* 40: USB                            */
  .word CAN_IRQHandler            /* 41: CAN                            */
  .word DMA_IRQHandler            /* 42: General Purpose DMA            */
  .word I2S_IRQHandler            /* 43: I2S                            */
  .word ENET_IRQHandler           /* 44: Ethernet                       */
  .word RIT_IRQHandler            /* 45: Repetitive Interrupt Timer     */
  .word MCPWM_IRQHandler          /* 46: Motor Control PWM              */
  .word QEI_IRQHandler            /* 47: Quadrature Encoder Interface   */
  .word PLL1_IRQHandler           /* 48: PLL1 Lock (USB PLL)            */

  /* --- code protection---- */

  .section .flashprot, "ax"
  .code 16
  .align 0
  .global flash_pcell

flash_pcell:    .word  0 /* 0x87654321 */

 /* ------------------------------------------------------------- */

  .section .init, "ax"
  .thumb_func
  .syntax unified             /* Cortex-M3 */

  .global  reset_handler
  .extern  tn_startup_hardware_init

reset_handler:

        bl      tn_startup_hardware_init

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

def_int_vector  NMI_Handler
def_int_vector  HardFault_Handler
def_int_vector  MemManage_Handler
def_int_vector  BusFault_Handler
def_int_vector  UsageFault_Handler
def_int_vector  SVC_Handler
def_int_vector  DebugMon_Handler
def_int_vector  PendSV_Handler
def_int_vector  SysTick_Handler

def_int_vector  WDT_IRQHandler
def_int_vector  TIMER0_IRQHandler
def_int_vector  TIMER1_IRQHandler
def_int_vector  TIMER2_IRQHandler
def_int_vector  TIMER3_IRQHandler
def_int_vector  UART0_IRQHandler
def_int_vector  UART1_IRQHandler
def_int_vector  UART2_IRQHandler
def_int_vector  UART3_IRQHandler
def_int_vector  PWM1_IRQHandler
def_int_vector  I2C0_IRQHandler
def_int_vector  I2C1_IRQHandler
def_int_vector  I2C2_IRQHandler
def_int_vector  SPI_IRQHandler
def_int_vector  SSP0_IRQHandler
def_int_vector  SSP1_IRQHandler
def_int_vector  PLL0_IRQHandler
def_int_vector  RTC_IRQHandler
def_int_vector  EINT0_IRQHandler
def_int_vector  EINT1_IRQHandler
def_int_vector  EINT2_IRQHandler
def_int_vector  EINT3_IRQHandler
def_int_vector  ADC_IRQHandler
def_int_vector  BOD_IRQHandler
def_int_vector  USB_IRQHandler
def_int_vector  CAN_IRQHandler
def_int_vector  DMA_IRQHandler
def_int_vector  I2S_IRQHandler
def_int_vector  ENET_IRQHandler
def_int_vector  RIT_IRQHandler
def_int_vector  MCPWM_IRQHandler
def_int_vector  QEI_IRQHandler
def_int_vector  PLL1_IRQHandler

/* -------------------------------------------------------------------------- */











