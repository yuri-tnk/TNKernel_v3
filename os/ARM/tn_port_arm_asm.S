/* ---------------------------------------------------------------------------

  TNKernel RTOS port for ARM(c) ARM7

  Copyright © 2004, 2016  Yuri Tiomkin

  All rights reserved.

  ffs_asm() - this is the ffs algorithm devised by D.Seal and posted to
              comp.sys.arm on  16 Feb 1994.

  Interrupt context switch -  this source code is derived on code
              written by WellsK

  Multi-compilers macros - derived from the Dmitry Frank's TNeo RTOS 

 Permission to use, copy, modify, and distribute this software in source
 and binary forms and its documentation for any purpose and without fee
 is hereby granted, provided that the above copyright notice appear
 in all copies and that both that copyright notice and this permission
 notice appear in supporting documentation.

 THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND
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
-----------------------------------------------------------------------------*/

 /* ver 3 */

#include "tn_config.h"

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

  #define   TN_NAME(x)             x
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

   AREA    |.text|, CODE, READONLY
   CODE32

#elif defined ( __ICCARM__ )

   RSEG   ICODE:CODE:NOROOT(2)
   CODE32

#elif defined( __GNUC__ )

   .text
   .code 32
   .align 4

#else
  #error Unknown compiler for ARM7
#endif
 

  //-- External references

  TN_EXTERN( tn_curr_run_task )
  TN_EXTERN( tn_next_task_to_run )
  TN_EXTERN( tn_system_state )
  TN_EXTERN( tn_func_exit_stack_start )
  TN_EXTERN( delete_task_resources )
  TN_EXTERN( tn_reset_idle_task_stack )
  TN_EXTERN( tn_check_stack )
  TN_EXTERN( tn_idle_task )
  TN_EXTERN( tn_cpu_irq_handler)
  TN_EXTERN( tn_int_counter)

  //-- Public functions declared in this file

  TN_GLOBAL( tn_task_exit_asm )
  TN_GLOBAL( tn_switch_context )
  TN_GLOBAL( tn_cpu_save_sr )
  TN_GLOBAL( tn_cpu_restore_sr )
  TN_GLOBAL( tn_start_exe )
  TN_GLOBAL( tn_chk_irq_disabled )
  TN_GLOBAL( ffs_asm )
  TN_GLOBAL( tn_func_to_ul_1 )
  TN_GLOBAL( tn_func_to_ul_2 )
  TN_GLOBAL( tn_void_to_ul )
  TN_GLOBAL( tn_ul_to_int_func )
  TN_GLOBAL( tn_arm_disable_interrupts )
  TN_GLOBAL( tn_arm_enable_interrupts )
  TN_GLOBAL( do_delay )
  TN_GLOBAL( tn_inside_int)
  TN_GLOBAL( tn_cpu_fiq_isr)
  TN_GLOBAL( tn_cpu_irq_isr)

TN_EQU(IRQMODE,   0x12)
TN_EQU(SVCMODE,   0x13)
TN_EQU(ABORTMODE, 0x17)
TN_EQU(UNDEFMODE, 0x1b)
TN_EQU(MODEMASK,  0x1f)
TN_EQU(NOINT,     0xc0)           /* Disable both IRQ & FIR */
TN_EQU(TBIT,      0x20)
TN_EQU(IRQMASK,   0x80)

//----------------------------------------------------------------------------
// Core interrupt is not enabled (yet)
//----------------------------------------------------------------------------
TN_LABEL(tn_start_exe)

     ldr    r1, =tn_system_state    // Indicate that system has started
     mov    r0, #1                  //- 1 -> TN_SYS_STATE_RUNNING
     strb   r0, [r1]

     ldr    r0, =tn_curr_run_task
     ldr    r1, =tn_next_task_to_run
     ldr    r1, [r1]                   /* get stack pointer */
     ldr    sp, [r1]                   /* switch to the new stack */
     str    r1, [r0]                   /* set new current running task tcb address */

     ldmfd  sp!, {r0}
     msr    SPSR_cxsf, r0

    // After the instruction exec, the global interrupts(IRQ) 
    // will be enabled
     
     ldmfd  sp!, {r0-r12,lr,pc}^       
  
//-----------------------------------------------------------------------------
TN_LABEL(tn_task_exit_asm) // ARM

     ldr    r1,  =tn_curr_run_task
     ldr    r2,  [r1]  // r2 -  tn_curr_run_task
  
  //--- Set exit stack
   
     ldr    r0,  =tn_func_exit_stack_start
     ldr    r0,  [r0]
     MOV    SP,  R0  // We are in SVC mode 

  //-- do delete resources  (r2 -  tn_curr_run_task)

     mov    r0,  r2  // -- Set parameter for the function 'delete_task_resources()'
     LDR    R1,  =delete_task_resources 
     mov    lr,  pc
     BX     R1

  //-- Switch context

     ldr    r0, =tn_curr_run_task
     ldr    r1, =tn_next_task_to_run
     ldr    r1, [r1]                   // get stack pointer
     ldr    sp, [r1]                   // switch to the new stack
     str    r1, [r0]                   // set new current running task tcb address

     ldmfd  sp!, {r0}
     msr    SPSR_cxsf, r0
     ldmfd  sp!, {r0-r12,lr,pc}^

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TN_LABEL(tn_cpu_irq_isr)            // ARM

     sub    lr, lr, #4              // Set lr to the actual return address
     stmfd  sp!, {r0-r12, lr}       // save all registers

     ldr    r2, =tn_int_counter
     ldr    r3, [r2]                // interrupt_counter -> r3 
     add    r0, r3, #1              // interrupt_counter++     
     str    r0, [r2]

     LDR     R0, =tn_cpu_irq_handler
     MOV     LR, PC
     BX      R0

     // interrupt_counter-- 

     ldr    r2, =tn_int_counter
     ldr    r1, [r2]
     sub    r3, r1,#1
     str    r3, [r2]
     cmp    r3, #0x00               // if it is nested int - return
     bne    exit_irq_int            // return from int

     ldr    r1, =tn_curr_run_task
     ldr    r2, [r1]
     ldr    r0, =tn_next_task_to_run
     ldr    r0, [r0]  // r0 - tn_next_task_to_run
     cmp    r0, #0                       // sanity checking
     beq    exit_irq_int            // return from int
     cmp    r2, r0
     beq    exit_irq_int            // return from int

#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
     push   {LR} 
#endif 

#if defined  TN_STACK_CHECK
       // r0 - tn_next_task_to_run
     LDR    R1, =tn_check_stack 
     mov    lr,  pc 
     BX     R1
#endif

#if defined  TN_RESET_IDLE_STACK

     ldr    r3, =tn_idle_task      //  
 
     ldr    r2, =tn_next_task_to_run
     ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
     cmp    r3, r2
     bne    nrs_l3

     LDR    R1, =tn_reset_idle_task_stack
     mov    lr, pc
     BX     R1

TN_LABEL(nrs_l3)

#endif

#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
     pop    {r0}
     mov    lr, r0
#endif

     b      tn_int_ctx_switch

TN_LABEL(exit_irq_int)

     ldmfd  sp!, {r0-r12, pc}^      // exit 

//---------------------------------------------------------------------------
TN_LABEL(tn_int_ctx_switch)

  // The goal - to obtain all registers of the interrrupted task 
  // (saved in IRQ's stack) and than save them in the interrupted task's stack

     mrs    r0,  spsr                  /* Get interrupted task's CPRS  */
     stmfd  sp!, {r0}                 /* Save it in the IRQ stack */
     add    sp, sp, #4                /* Restore stack ptr after CPSR saving */
     ldmfd  sp!, {r0-r12, lr}         /* Put all saved registers from IRQ */
                                      /*   stack back to registers */
     mov    r1, sp                    /* r1 <-  IRQ's SP */
     sub    r1, r1, #4
     msr    cpsr_c, #(NOINT TN_OR SVCMODE) /* Change to SVC mode; INT are disabled */

   /* Now - in SVC mode; in r1 - IRQ's SP */

     ldr    r0, [r1], #-14*4    /* r0 <- task's lr (instead pc)+ rewind stack ptr */
     stmfd  sp!, {r0}
     stmfd  sp!, {r2-r12, lr}    /* Save registers in interrupted task's */
                                 /*  stack,except CPSR,r0,r1 */
     ldr    r0, [r1], #2*4       /* Get interrupted task's CPSR (pos 0(-15))*/
     ldr    r2, [r1], #-4        /* Get valid r1 to save (pos 2(-13)) */
     ldr    r1, [r1]             /* Get valid r0 to save (pos 1(-14)) */
     stmfd  sp!, {r0-r2}         /* Save r0, r1, and CPSR */

  /* Registers has been saved. Now - switch context */

     ldr    r0, =tn_curr_run_task
     ldr    r0, [r0]
     str    sp, [r0]               /* SP <- curr task */

     ldr    r0, =tn_next_task_to_run
     ldr    r2, [r0]
     ldr    r0, =tn_curr_run_task
     str    r2, [r0]

     ldr    sp, [r2]

  /* Return */

     ldmfd  sp!, {r0}                  /* Get CPSR */
     msr    spsr_cxsf, r0              /* SPSR <- CPSR */
     ldmfd  sp!, {r0-r12, lr, pc}^     /* Restore all registers, CPSR also */


//-----------------------------------------------------------------------------
TN_LABEL(tn_switch_context)   // ARM

     stmfd  sp!, {lr}                   // Save return address
     stmfd  sp!, {r0-r12,lr}            // Save curr task registers
     mrs    r0,  cpsr
     tst    LR,  #1                      // from Thumb mode (???)
     orrne  R0,  R0, #0x20              // set the THUMB bit
     stmfd  sp!, {r0}                   // Save current CPSR

     mrs    r0,  cpsr
     orr    r0, r0, #NOINT              // Disable Int
     msr    CPSR_c, r0

     ldr    r1, =tn_curr_run_task
     ldr    r2, [r1]
     ldr    r0, =tn_next_task_to_run
     ldr    r0, [r0]  // r0 - tn_next_task_to_run
     cmp    r0, #0                       // sanity checking
     beq    tn_sw_restore
     cmp    r2, r0
     beq    tn_sw_restore

#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
     push   {LR} 
#endif 

#if defined  TN_STACK_CHECK
       // r0 - tn_next_task_to_run
     ldr    r1,  =tn_check_stack 
     mov    lr,  pc
     bx     r1
#endif

#if defined  TN_RESET_IDLE_STACK

     ldr    r3, =tn_idle_task      //  
 
     ldr    r2, =tn_next_task_to_run
     ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
     cmp    r3, r2
     bne    nrs_l0

     LDR    R1,  =tn_reset_idle_task_stack
     mov    lr,  pc
     BX     R1

TN_LABEL(nrs_l0)

#endif

#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
     pop      {r0}
     mov      lr, r0

     ldr    r1, =tn_curr_run_task
     ldr    r2, [r1]
     ldr    r0, =tn_next_task_to_run
     ldr    r0, [r0]  // r0 - tn_next_task_to_run
#endif

     str    sp, [r2]            // store sp in preempted tasks's TCB
     ldr    sp, [r0]            // get new task's sp
     str    r0, [r1]            // tn_curr_run_task = tn_next_task_to_run

TN_LABEL(tn_sw_restore)

     ldmfd   sp!, {r0}
     msr     SPSR_cxsf, r0
     ldmfd   sp!, {r0-r12,lr,pc}^

//----------------------------------------------------------------------------
TN_LABEL(tn_cpu_fiq_isr)        // ARM

     stmfd  SP!, {R0-R12,LR}
     ldmfd  SP!, {R0-R12,LR}    // restore registers of interrupted task's stack
     subs   PC, LR, #4          // return from FIQ

//-----------------------------------------------------------------------------
TN_LABEL(tn_cpu_save_sr)   // ARM

     mrs    r0, CPSR              // Disable both IRQ & FIQ interrupts
     orr    r1, r0, #NOINT
     msr    CPSR_c, r1

     bx     lr

//-----------------------------------------------------------------------------
TN_LABEL(tn_cpu_restore_sr)     // ARM

     msr    CPSR_c,r0
     bx     lr

//-----------------------------------------------------------------------------
TN_LABEL(tn_chk_irq_disabled)    // ARM

     mrs    r0, cpsr
     and    r0, r0, #IRQMASK
     bx     lr

//-----------------------------------------------------------------------------
TN_LABEL(ffs_asm)

   // Standard trick to isolate bottom bit in r0 or 0 if r0 = 0 on entry
     rsb     r1, r0, #0
     ands    r0, r0, r1

      // now r0 has at most one set bit, call this X
      // if X = 0, all further instructions are skipped

     adrne   r2, L_ffs_table
     orrne   r0, r0, r0, lsl #4       // r0 = X * 0x11
     orrne   r0, r0, r0, lsl #6       // r0 = X * 0x451
     rsbne   r0, r0, r0, lsl #16      // r0 = X * 0x0450fbaf

     // now lookup in table indexed on top 6 bits of r0
     ldrneb  r0, [ r2, r0, lsr #26 ]

     bx      lr

     TN_DATA

TN_LABEL( L_ffs_table )
              //   0   1   2   3   4   5   6   7
    TN_BYTE        0,  1,  2, 13,  3,  7,  0, 14  //  0- 7
    TN_BYTE        4,  0,  8,  0,  0,  0,  0, 15  //  8-15
    TN_BYTE       11,  5,  0,  0,  9,  0,  0, 26  // 16-23
    TN_BYTE        0,  0,  0,  0,  0, 22, 28, 16  // 24-31
    TN_BYTE       32, 12,  6,  0,  0,  0,  0,  0  // 32-39
    TN_BYTE       10,  0,  0, 25,  0,  0, 21, 27  // 40-47
    TN_BYTE       31,  0,  0,  0,  0, 24,  0, 20  // 48-55
    TN_BYTE       30,  0, 23, 19, 29, 18, 17,  0  // 56-63


//----- Just to make MISRA 2012 happy -----------------------------------------
TN_LABEL(tn_func_to_ul_1)

     BX     LR

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_LABEL(tn_func_to_ul_2)

     BX     LR  

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_LABEL(tn_void_to_ul)

     BX     LR  

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_LABEL(tn_ul_to_int_func)

     BX     LR  

//----------------------------------------------------------------------------
TN_LABEL(tn_arm_disable_interrupts)   // ARM

     mrs    r0, cpsr
     orr    r0, r0, #NOINT
     msr    cpsr_c, r0

     bx     lr

// ---------------------------------------------------------------------------
TN_LABEL(tn_arm_enable_interrupts)   // ARM

     mrs    r0, cpsr
     bic    r0, r0, #NOINT
     msr    cpsr_c, r0

     bx     lr

// ---------------------------------------------------------------------------
TN_LABEL(tn_inside_int)

     mrs    r0, cpsr
     and    r0, r0, #MODEMASK
     cmp    r0, #IRQMODE
     bne    tiint_l

     mov    r0, #1
     bx     lr

TN_LABEL(tiint_l)

     mov    r0, #0
     bx     lr

// ---------------------------------------------------------------------------
TN_LABEL(do_delay)

     SUBS   R0, R0, #1
     CMP    R0, #0
     BNE    do_delay
     bx     lr
//----------------------------------------------------------------------------

     TN_END()

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

