/* ---------------------------------------------------------------------------
     TNKernel RTOS port for ARM(c) Cortex-M0..M7(c) CPU

     Copyright © 2004, 2016  Yuri Tiomkin
     Copyright © 2007, 2013  Vyacheslav Ovsiyenko
     Copyright © 2008        Sergey Koshkin
     Copyright © 2014        Dmitry Frank

     All rights reserved.

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
----------------------------------------------------------------------------*/

#include "tn_config.h"

#if   defined ( __CC_ARM ) || defined( __clang__ )

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
  #error Unknown compiler
#endif


#if defined ( __CC_ARM ) || defined( __clang__ )

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
 

  //-- External references

  TN_EXTERN( tn_curr_run_task )
  TN_EXTERN( tn_next_task_to_run )
  TN_EXTERN( tn_system_state )
  TN_EXTERN( tn_func_exit_stack_start )
  TN_EXTERN( delete_task_resources )
  TN_EXTERN( tn_reset_idle_task_stack )
  TN_EXTERN( tn_check_stack )
  TN_EXTERN( tn_idle_task )

  //-- Public functions declared in this file

  TN_GLOBAL( tn_task_exit_asm )
  TN_GLOBAL( tn_switch_context )
  TN_GLOBAL( tn_cpu_save_sr )
  TN_GLOBAL( tn_cpu_restore_sr )
  TN_GLOBAL( tn_start_exe )
  TN_GLOBAL( tn_chk_irq_disabled )
  TN_GLOBAL( PendSV_Handler )
  TN_GLOBAL( tn_int_exit )
  TN_GLOBAL( ffs_asm )
  TN_GLOBAL( tn_func_to_ul_1 )
  TN_GLOBAL( tn_func_to_ul_2 )
  TN_GLOBAL( tn_void_to_ul )
  TN_GLOBAL( tn_arm_disable_interrupts )
  TN_GLOBAL( tn_arm_enable_interrupts )
  TN_GLOBAL(tn_inside_int)
  TN_GLOBAL( do_delay )

TN_EQU(SC_ICSR,  0xE000ED04)              // Interrupt Control and State
TN_EQU(bICSR_PENDSVSET,  (1<<28))         // PendSV set-pending bit

 //--  Interrupt Control State Register Address

TN_EQU(ICSR_ADDR, 0xE000ED04)

 //--  pendSV bit in the Interrupt Control State Register

TN_EQU(PENDSVSET, 0x10000000)

 //--  System Handlers 12-15 Priority Register Address

TN_EQU(PR_12_15_ADDR, 0xE000ED20)


#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P)

 //--  PRI_14 (PendSV) priority in the System Handlers 14-15 Priority Register Address
 //--  PendSV priority is minimal (0xC0)

TN_EQU(PENDS_VPRIORITY, 0x00C00000)
TN_EQU(VECTACTIVE, 0x0000003F)   /* Bits[5:0]*/

#else
 //--  PRI_14 (PendSV) priority in the System Handlers 12-15 Priority Register Address
 //--  PendSV priority is minimal (0xFF)

TN_EQU(PENDS_VPRIORITY, 0x00FF0000)
TN_EQU(VECTACTIVE, 0x000001FF)   /* Bits[8:0]*/

#endif

 //-- M0  System Handlers 12-15 Priority Register Address

TN_EQU(PR_14_15_ADDR, 0xE000ED20)

//----------------------------------------------------------------------------
// Core interrupt is not enabled (yet)
//----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_start_exe)

#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P)
 
       ldr    r1, =PR_14_15_ADDR        //-- Load the System 12-15 Priority Register
       ldr    r0, [r1]

       ldr    r2, =PENDS_VPRIORITY
       orrs   r0, r0, r2                //-- set PRI_14 (PendSV) to 0xFF - minimal
       str    r0, [r1]

       ldr    r1, =tn_system_state      //-- Indicate that system has started
       movs   r0, #1                    //-- 1 -> TN_SYS_STATE_RUNNING
       strb   r0, [r1]

  //---------------

       ldr    r1,  =tn_curr_run_task     // =tn_next_task_to_run
       ldr    r2,  [r1]
       ldr    r0,  [r2]                  //-- in r0 - new task SP

       adds   r0,  r0, #16
       ldmia  r0!, {r4-r7}               //-- restore new context (r8-r11)
       mov    r8,  r4
       mov    r9,  r5
       mov    r10, r6
       mov    r11, r7
       subs   r0,  r0,#32
       ldmia  r0!, {r4-r7}               // restore new context (r4-r7)
       adds   r0,  r0, #16

       msr    PSP, r0
       isb

       movs   r1, #4                    // Force to new process PSP
       mov    r0, lr
       orrs   r0, r0, r1
       mov    lr, r0

       ldr    r1, =ICSR_ADDR            //-- Trigger PendSV exception
       ldr    r0, =PENDSVSET
       str    r0, [r1]

       cpsie  I                         //-- Enable core interrupts
       b  .                             //--- Never reach this

#else

       ldr    r1, =PR_12_15_ADDR        //-- Load the System 12-15 Priority Register
       ldr    r0, [r1]
       orr    r0, r0, #PENDS_VPRIORITY  //-- set PRI_14 (PendSV) to 0xFF - minimal
       str    r0, [r1]

       ldr    r1, =tn_system_state      //-- Indicate that system has started
       mov    r0, #1                    //-- 1 -> TN_SYS_STATE_RUNNING
       strb   r0, [r1]

  //---------------

       ldr    r1, =tn_curr_run_task     // =tn_next_task_to_run
       ldr    r2, [r1]
       ldr    r0, [r2]                  //-- in r0 - new task SP

#if (TN_SUPPORT_FPU == 1)                   
       ldmia   R0!, {R4-R11, LR}              
#else
       ldmia   R0!, {R4-R11}                
#endif                                      
       msr     PSP, R0                        
                                              
       ldr     R1, =SC_ICSR                    // Trigger PendSV exception
       ldr     R0, =bICSR_PENDSVSET            // for context switching
       str     R0, [R1]                       
                                              
       movs    R0, #0                          // Clear FPCA bit (if any) also
       msr     CONTROL, R0                     // Use the MSP before PendSv switch
       isb                                    
       cpsie   IF                              // Enable core interrupts
       b       .                               // Never reach this
#endif

//----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(PendSV_Handler)

#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P)

       cpsid  I                          //   Disable core int

       ldr    r3, =tn_curr_run_task      //  in R3 - =tn_curr_run_task
       ldr    r1, [r3]                   //  in R1 - tn_curr_run_task
       ldr    r2, =tn_next_task_to_run
       ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
       cmp    r1, r2
       beq    exit_context_switch

  //-------------------------------------

       mrs    r0, psp               // in PSP - process(task) stack pointer,
                                    // put it to R0

       subs   r0, r0, #32	         // expected stack pos
       str    r0, [r1]              // save own sp in tcb

       stmia  r0!, {r4-r7}          // save old context (r4-r7)
       mov    r4, r8
       mov    r5, r9
       mov    r6, r10
       mov    r7, r11
       stmia  r0!,{r4-r7}           // save old context (r8-r11)

       str    r2, [r3]              // in r3 - =tn_curr_run_task
       ldr    r0, [r2]              // in r0 - new task sp

       adds   r0, r0, #16           // position in stack where r8..11 are stored
       ldmia  r0!,{r4-r7}           // restore  r8..r11 to r4..r7
       mov    r8, r4	               // move r8..r11  contents to the original location
       mov    r9, r5
       mov    r10,r6
       mov    r11,r7
       subs   r0, r0, #32           // position in stack where r4..7 are stored
       ldmia  r0!,{r4-r7}           // restore new context (r4-r7)
       adds   r0, r0,#16            // R0 = Stack ptr

       msr    psp, r0               // New stack - in R0, put it to the PSP
       isb

       movs   r1, #4                // Force to new process PSP
       mov    r0, lr
       orrs   r0, r0, r1
       mov    lr, r0

  //-------------------------------------

TN_LABEL(exit_context_switch)

       cpsie  I                     //  enable core int
       
       bx     lr

#else
       cpsid  I                          //   Disable core int
       isb

       ldr    r3, =tn_curr_run_task      //  in R3 - =tn_curr_run_task
       ldr    r1, [r3]                   //  in R1 - tn_curr_run_task
       ldr    r2, =tn_next_task_to_run
       ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
       cmp    r1, r2
       beq    exit_context_switch

  //-------------------------------------

#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
       push   {LR}                 // to save LR
#endif 

#if defined  TN_STACK_CHECK
       mov    r0,   r2            // -- Set parameter for the function 'tn_check_stack'
       LDR    R1, =tn_check_stack 
       BLX    R1
#endif


#if defined  TN_RESET_IDLE_STACK

       ldr    r3, =tn_idle_task      //  

 
       ldr    r2, =tn_next_task_to_run
       ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
       cmp    r3, r2
       bne    nrs_l0

       LDR    R1, =tn_reset_idle_task_stack
       BLX    R1

TN_LABEL(nrs_l0)

#endif


#if defined  ( TN_STACK_CHECK )  || defined ( TN_RESET_IDLE_STACK )
       pop      {r0}                      // restore LR
       mov      lr, r0

       ldr    r3, =tn_curr_run_task      //  in R3 - =tn_curr_run_task
       ldr    r1, [r3]                   //  in R1 - tn_curr_run_task
       ldr    r2, =tn_next_task_to_run
       ldr    r2, [r2]                   //  in R2 - tn_next_task_to_run
#endif

  //-------------------------------------

       mrs    r0, psp                    //  in PSP - process(task) stack pointer
#if (TN_SUPPORT_FPU == 1)                    
       tst       LR, #0x10                     // if FPU context is active
       it        eq                            // we should save its registers
       vstmdbeq  R0!, {S16-S31}                // on the process stack
       stmdb     R0!, {R4-R11, LR}             
#else
       stmdb     R0!, {R4-R11}            //  stmdb  r0!, {r4-r11}
#endif 

       str    r0, [r1]                   //  save own SP in TCB
       str    r2, [r3]                   //  in r3 - =tn_curr_run_task
       ldr    r0, [r2]                   //  in r0 - new task SP
#if (TN_SUPPORT_FPU == 1)                      
       ldmia     R0!, {R4-R11, LR}             // restore the high GPRs
       tst       LR, #0x10                     // look whether the
       it        eq                            // active FPU context
       vldmiaeq  R0!, {S16-S31}                
#else
       ldmia     r0!, {r4-r11}
#endif

       msr    psp, r0
       isb

TN_LABEL(exit_context_switch)

       orr    lr, lr, #0x04              //  Force to new process PSP
       cpsie  I                          //  enable core int
       bx     lr

#endif
  
//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_task_exit_asm)

       ldr    r1,  =tn_curr_run_task
       ldr    r2,  [r1]
       mov    r1,   r2      // -- Save r2
  
  //--- Set exit stack
   
       ldr    r0,  =tn_func_exit_stack_start
       ldr    r0,  [r0]
 
       msr    psp, r0

#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P)

      movs   r1, #4                // Force to new process PSP
       mov    r0, lr
       orrs   r0, r0, r1
       mov    lr, r0
#else
       orr    lr, lr, #0x04              //  Force to exit stack
#endif
       mov    r0,   r2      // -- Set parameter for the function 'delete_task_resources()'
       LDR    R1, =delete_task_resources 
       BLX    R1

  //--- MSP stack

       mrs    r0,  msp
       adds   r0,  r0, #32   // 8*4 - 8 automatically saved registers at the PendSV interrupt
       msr    msp, r0

       movs   r0,  #0        // Default stack MSP(Main stack pointer) is used
       msr    control, r0
       dsb
       isb 

  //-------------------------------

       ldr    R1, =SC_ICSR                    // Trigger PendSV exception
       ldr    R0, =bICSR_PENDSVSET            
       str    R0,  [R1]                       
       cpsie  I                               // Enable core interrupts
       b      .                               // Should never reach

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_switch_context)

     //---  Just Invoke PendSV exception

       ldr    r1,  =ICSR_ADDR
       ldr    r0,  =PENDSVSET
       str    r0,  [r1]

       bx     lr

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_int_exit)

       ldr    r1,  =ICSR_ADDR           //-- Invoke PendSV exception
       ldr    r0,  =PENDSVSET
       str    r0,  [r1]

       bx     lr

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_cpu_save_sr)

       mrs    r0, PRIMASK


        cpsid  I
       bx     lr

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_cpu_restore_sr)

       msr    PRIMASK, r0
       bx     lr

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_chk_irq_disabled)

       mrs    r0,PRIMASK
       bx     lr

//-----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(ffs_asm)

#if defined(TN_CORTEX_M3) || defined(TN_CORTEX_M4) || defined(TN_CORTEX_M7)

       mov    r1, r0                    //-- tmp = in
       rsbs   r0, r1, #0                //-- in = -in
       ands   r0, r0, r1                //-- in = in & tmp
       CLZ.W  r0, r0
       rsb    r0, r0, #0x20             //-- 32 - in
#endif
       bx     lr

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_func_to_ul_1)

        BX       LR

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_func_to_ul_2)

        BX       LR  

//----- Just to make MISRA 2012 happy -----------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_void_to_ul)

        BX       LR  

//----------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_arm_disable_interrupts)

     cpsid  I
     bx     lr


// ---------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_arm_enable_interrupts)

     cpsie I
     bx   lr

// ---------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(tn_inside_int)

        MRS      R0, IPSR

#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P)
        LSLS     R0, R0, #26  /* Logical Shift Left 32-26 = 6 #0x3F */
#else
        LSLS     R0, R0, #23  /* Logical Shift Left 32-23 = 9 #0x1FF */ 
#endif
        BEQ      tn_inside_int_0

        MOVS     R0, #1
        BX       LR             

TN_LABEL(tn_inside_int_0)

        MOVS     R0, #0
        BX       LR             
    
// ---------------------------------------------------------------------------
TN_THUMB_FUNC()
TN_LABEL(do_delay)

/*
     subs r0, #1
     bne do_delay
*/
     SUBS     R0, R0, #1
     CMP      R0, #0
     BNE.N    do_delay
     
     bx   lr

#if defined(TN_CORTEX_M3) || defined(TN_CORTEX_M4) || defined(TN_CORTEX_M7)

     nop     // For alignment only 
#endif

     TN_END()

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

