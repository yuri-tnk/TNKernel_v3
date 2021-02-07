/* ---------------------------------------------------------------------------

  TNKernel startup hardware init for LPC176x processors


  Copyright © 2004, 2016 Yuri Tiomkin
  All rights reserved.

  Multi-compilers macros - derived from the Dmitry Frank's TNeo RTOS 


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
---------------------------------------------------------------------------- */

/*  ver 3 */

#if   defined ( __CC_ARM )

  #define  TN_NAME(x)             x
  #define  TN_LOCAL_NAME(x)       x
  #define  TN_EXTERN(x)           IMPORT TN_NAME(x)
  #define  TN_GLOBAL(x)           EXPORT TN_NAME(x)
  #define  TN_EQU(symbol, value)  symbol EQU value
  #define  TN_LABEL(label)        TN_NAME(label)
  #define  TN_LOCAL_LABEL(label)  TN_LOCAL_NAME(label)
  #define  TN_END()               END
  #define  TN_THUMB_FUNC()        /* nothing */

#elif defined ( __GNUC__ )

  #define   TN_NAME(x)             x
  #define  TN_LOCAL_NAME(x)       L ## x
  #define  TN_EXTERN(x)           .extern x
  #define  TN_GLOBAL(x)           .global x
  #define  TN_EQU(symbol, value)  .equ symbol, value
  #define  TN_LABEL(label)        TN_NAME(label):
  #define  TN_LOCAL_LABEL(label)  TN_LOCAL_NAME(label):
  #define  TN_END()               /* nothing */
  #define  TN_THUMB_FUNC()        .thumb_func

#elif defined ( __ICCARM__ )

  #define  TN_NAME(x)             x
  #define  TN_LOCAL_NAME(x)       x
  #define  TN_EXTERN(x)           IMPORT x
  #define  TN_GLOBAL(x)           EXPORT x
  #define  TN_EQU(symbol, value)  symbol EQU value
  #define  TN_LABEL(label)        label:
  #define  TN_LOCAL_LABEL(label)  label:
  #define  TN_END()               END
  #define  TN_THUMB_FUNC()        /* nothing */

#else
  #error Unknown compiler for Cortex-M
#endif


#if defined ( __CC_ARM )

   AREA    |.text|, CODE, READONLY, ALIGN=3
   THUMB
   REQUIRE8
   PRESERVE8

#elif defined ( __ICCARM__ )

   SECTION  .text:CODE(2)
   THUMB
   REQUIRE8
   PRESERVE8

#elif defined( __clang__ )

   .text
   .syntax unified
   .thumb

#elif defined( __GNUC__ )

   .text
   .syntax unified
   .thumb

#else
  #error Unknown compiler for Cortex-M
#endif
 


TN_EQU(PCLKSEL0, 0x400FC1A8)  /* PCLKSEL0 */

  /* -- Public functions declared in this file */

     TN_GLOBAL(tn_startup_hardware_init)

/* ----------------------------------------------------------------------------
  This routine is called immediately after reset to setup hardware that is
 vital for processor's functionality (for instance,SDRAM controller setup,
 PLL setup,etc.)
  It is assumed that other hardware's init routine(s) will be invoked later
 by C-language function call.
---------------------------------------------------------------------------- */

TN_THUMB_FUNC()
TN_LABEL(tn_startup_hardware_init)

    /* PCLKSEL0 - UART0, SPI, Timer 1 (MAT1.1), PWM1 (PWM1.3) */

     ldr    r1, =PCLKSEL0
     ldr    r0, [r1]
     orr    r0, r0, #0x01000000
     orr    r0, r0, #(1<<6)   /* UART0   - Bits 7:6   = 01;  PCLK_peripheral = CCLK */
     orr    r0, r0, #(1<<16)  /* SPI     - Bits 17:16 = 01;  PCLK_peripheral = CCLK */
     orr    r0, r0, #(1<<4)   /* Timer 1 - Bits 5:4   = 01;  PCLK_peripheral = CCLK */
     orr    r0, r0, #(1<<12)  /* PWM1    - Bits 13:12 = 01;  PCLK_peripheral = CCLK */
     str    r0, [r1]

     bx      lr

     TN_END()

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */




