/* ----------------------------------------------------------------------------

   TNKernel startup hardware init for LPC23XX processors

   Copyright © 2004,2016 Yuri Tiomkin
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

----------------------------------------------------------------------------*/

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
  #define  TN_BYTE                DCB
  #define  TN_DATA               
  #define  TN_OR                  :OR: 

#elif defined ( __GNUC__ )

  #define  TN_NAME(x)             x
  #define  TN_LOCAL_NAME(x)       L ## x
  #define  TN_EXTERN(x)           .extern x
  #define  TN_GLOBAL(x)           .global x
  #define  TN_EQU(symbol, value)  .equ symbol, value
  #define  TN_LABEL(label)        TN_NAME(label):
  #define  TN_LOCAL_LABEL(label)  TN_LOCAL_NAME(label):
  #define  TN_END()               /* nothing */
  #define  TN_THUMB_FUNC()        .thumb_func
  #define  TN_BYTE                .byte
  #define  TN_DATA               
  #define  TN_OR                  |

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
  #define  TN_BYTE                DC8
  #define  TN_DATA                DATA
  #define  TN_OR                  |

#else
  #error Unknown compiler for ARM7
#endif


#if defined ( __CC_ARM )

   PRESERVE8

//   AREA    |.text|, CODE, READONLY
//   CODE32
   AREA    HardwareInit, CODE, READONLY
   CODE32

#elif defined ( __ICCARM__ )

   RSEG ICODE:CODE(2)
   CODE32

#elif defined( __GNUC__ )

   .text
   .code 32
   .align 4

#else
  #error Unknown compiler for ARM7
#endif

TN_EQU(MC_FMR,    0xFFFFFF60)
TN_EQU(WDT_MR,    0xFFFFFD44)
TN_EQU(CKGR_MOR,  0xFFFFFC20)
TN_EQU(PMC_SR,    0xFFFFFC68)
TN_EQU(CKGR_PLLR, 0xFFFFFC2C)
TN_EQU(PMC_MCKR,  0xFFFFFC30)
TN_EQU(MC_RCR,    0xFFFFFF00)

TN_EQU(NOINT,     0xc0)

TN_EQU(rMAMTIM,   0xE01FC004)
TN_EQU(rMAMCR,    0xE01FC000)

TN_EQU(rPLLCON,   0xE01FC080)
TN_EQU(rPLLSTAT,  0xE01FC088)
TN_EQU(rPLLFEED,  0xE01FC08C)
TN_EQU(r_SCS,     0xE01FC1A0)


  // Public functions declared in this file

     TN_GLOBAL( tn_startup_hardware_init )

  //  Extern reference

     TN_EXTERN( reset )


/* ----------------------------------------------------------------------------
  This routine is called immediately after reset to setup hardware that is
 vital for processor's functionality (for instance,SDRAM controller setup,
 PLL setup,etc.)
  It is assumed that other hardware's init routine(s) will be invoked later
 by C-language function call.
----------------------------------------------------------------------------*/

TN_LABEL(tn_startup_hardware_init)

    //-- For LPC23XX

     // Flash speed
     // rMAMTIM = 3

        ldr  r0, =rPLLSTAT
        ldr  r1, [r0]
        tst  r1, #(1<<25) /* 0x02000000 */
        bne  label_1

        ldr   r0, =rPLLCON
        mov   r1, #1
        str   r1, [r0]
        ldr   r0, =rPLLFEED
        mov   r1, #0xAA
        str   r1, [r0]
        mov   r1, #0x55
        str   r1, [r0]

TN_LABEL(label_1)

//   rPLLCON  = 0;                    Disable PLL, disconnected

        ldr   r0, =rPLLCON
        mov   r1, #0
        str   r1, [r0]


//       rPLLFEED = 0xaa;
//       rPLLFEED = 0x55;

      ldr  r0, =rPLLFEED
      mov  r1, #0xAA
      str  r1, [r0]
      mov  r1, #0x55
      str  r1, [r0]

//   r_SCS |= 0x20;                   Enable main OSC

      ldr  r0, =r_SCS
      ldr  r1, [r0]
      orr  r1, r1, #0x20
      str  r1, [r0]

//   while(!(rSCS & 0x40));         Wait until main OSC is usable

TN_LABEL(label_2)

      ldr  r0, =r_SCS
      ldr  r1, [r0]
      tst  r1, #0x40
      beq  label_2


      bx   lr

     TN_END()


