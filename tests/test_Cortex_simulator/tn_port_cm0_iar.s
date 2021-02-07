;------------------------------------------------------------------------------
;
;  TNKernel RTOS port for ARM(c) Cortex-M0(c) core
;
;  Copyright © 2004, 2011 Yuri Tiomkin
;  All rights reserved.
;
;  v.2.6
;
;  Assembler:   IAR(c)
;
;
; Permission to use, copy, modify, and distribute this software in source
; and binary forms and its documentation for any purpose and without fee
; is hereby granted, provided that the above copyright notice appear
; in all copies and that both that copyright notice and this permission
; notice appear in supporting documentation.
;
; THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
; SUCH DAMAGE.
;------------------------------------------------------------------------------


        PRESERVE8

  ;-- External references

  IMPORT  tn_curr_run_task
  IMPORT  tn_next_task_to_run
  IMPORT  tn_system_state


  ;-- Public functions declared in this file

  EXPORT  tn_switch_context_exit
  EXPORT  tn_switch_context
  EXPORT  tn_cpu_save_sr
  EXPORT  tn_cpu_restore_sr
  EXPORT  tn_start_exe
  EXPORT  tn_chk_irq_disabled
  EXPORT  PendSV_Handler
  EXPORT  tn_int_exit

 ;--  Interrupt Control State Register Address

ICSR_ADDR        EQU     0xE000ED04

 ;--  pendSV bit in the Interrupt Control State Register

PENDSVSET        EQU     0x10000000

 ;--  System Handlers 12-15 Priority Register Address

PR_14_15_ADDR    EQU     0xE000ED20

 ;--  PRI_14 (PendSV) priority in the System Handlers 14-15 Priority Register Address
 ;--  PendSV priority is minimal (0xC0)

PENDS_VPRIORITY  EQU     0x00C00000

;----------------------------------------------------------------------------
;
;----------------------------------------------------------------------------

       SECTION    .text:CODE(2)
       THUMB
       REQUIRE8
       PRESERVE8


;----------------------------------------------------------------------------
; Interrups not yet enabled
;----------------------------------------------------------------------------
tn_start_exe:

       ldr    r1, =PR_14_15_ADDR        ;-- Load the System 12-15 Priority Register
       ldr    r0, [r1]

       ldr    r2, =PENDS_VPRIORITY
       orrs   r0, r0, r2                ;-- set PRI_14 (PendSV) to 0xFF - minimal

       str    r0, [r1]

       ldr    r1, =tn_system_state      ;-- Indicate that system has started
       movs   r0, #1                    ;-- 1 -> TN_SYS_STATE_RUNNING
       strb   r0, [r1]

  ;---------------

       ldr    r1, =tn_curr_run_task     ; =tn_next_task_to_run
       ldr    r2, [r1]
       ldr    r0, [r2]                  ;-- in r0 - new task SP

       adds   r0, r0, #16
       ldmia  r0!,{r4-r7}               ;-- restore new context (r8-r11)
       mov    r8, r4
       mov    r9, r5
       mov    r10, r6
       mov    r11, r7
       subs   r0, r0,#32
       ldmia  r0!,{r4-r7}               ; restore new context (r4-r7)
       adds   r0, r0, #16

       msr    PSP, r0
       isb

       movs   r1, #4                    ; Force to new process PSP
       mov    r0, lr
       orrs   r0, r0, r1
       mov    lr, r0

 ;tn_switch_context_exit

       ldr    r1, =ICSR_ADDR            ;-- Trigger PendSV exception
       ldr    r0, =PENDSVSET
       str    r0, [r1]

       cpsie  I                         ;-- Enable core interrupts

  ;--- Never reach this

       b  .


;----------------------------------------------------------------------------
PendSV_Handler:

       cpsid  I                          ;   Disable core int

       ldr    r3, =tn_curr_run_task      ;  in R3 - =tn_curr_run_task
       ldr    r1, [r3]                   ;  in R1 - tn_curr_run_task
       ldr    r2, =tn_next_task_to_run
       ldr    r2, [r2]                   ;  in R2 - tn_next_task_to_run
       cmp    r1, r2
       beq    exit_context_switch

  ;-------------------------------------

       mrs    r0, psp               ; in PSP - process(task) stack pointer,
                                    ; put it to R0

       subs   r0, r0, #32	   ; expected stack pos
       str    r0, [r1]              ; save own sp in tcb

       stmia  r0!, {r4-r7}          ; save old context (r4-r7)
       mov    r4, r8
       mov    r5, r9
       mov    r6, r10
       mov    r7, r11
       stmia  r0!,{r4-r7}           ; save old context (r8-r11)

       str    r2, [r3]              ; in r3 - =tn_curr_run_task
       ldr    r0, [r2]              ; in r0 - new task sp

       adds   r0, r0, #16           ; position in stack where r8..11 are stored
       ldmia  r0!,{r4-r7}           ; restore  r8..r11 to r4..r7
       mov    r8, r4	           ; move r8..r11  contents to the original location
       mov    r9, r5
       mov    r10,r6
       mov    r11,r7
       subs   r0, r0, #32           ; position in stack where r4..7 are stored
       ldmia  r0!,{r4-r7}           ; restore new context (r4-r7)
       adds   r0, r0,#16            ; R0 = Stack ptr

       msr    psp, r0               ; New stack - in R0, put it to the PSP
       isb

       movs   r1, #4                ; Force to new process PSP
       mov    r0, lr
       orrs   r0, r0, r1
       mov    lr, r0

  ;-------------------------------------

exit_context_switch

       cpsie  I                     ;  enable core int

       bx     lr

;-----------------------------------------------------------------------------
tn_switch_context_exit:

      ;  Thanks to Sergey Koshkin

  ;--- At the interrupt request below, the PSP will be used to
  ;--- store manually saved 8 registers (r4-r11)

       ldr    r1,  =tn_curr_run_task
       ldr    r2,  [r1]
       ldr    r0,  [r2]       ;-- in r0 - tn_cur_run_task stack

  ;--- Preset PSP stack

                              ;-- 8*4 - 8 manually saved registers
       movs    r1, #32
       add    r0,  r0, r1

       msr    psp, r0

  ; --- At the interrupt request below, the MSP will be used
  ;--- to store automaticaly saved 8 registers

  ;--- Preset MSP stack

       mrs    r0,  msp
       adds   r0,  r0, #32   ; 8*4 - 8 automatically saved registers at the PendSV interrupt
       msr    msp, r0

       movs   r0,  #0        ; Default stack MSP(Main stack pointer) is used
       msr    control, r0

  ;-------------------------------

       ldr    r1,  =ICSR_ADDR           ;  Trigger PendSV exception
       ldr    r0,  =PENDSVSET
       str    r0,  [r1]

       cpsie  I                         ;  Enable core interrupts

  ; - Should never reach

       b  .

;-----------------------------------------------------------------------------
tn_switch_context:

     ;---  Just Invoke PendSV exception

       ldr    r1,  =ICSR_ADDR
       ldr    r0,  =PENDSVSET
       str    r0,  [r1]

       bx     lr

;-----------------------------------------------------------------------------
tn_int_exit:

       ldr    r1,  =ICSR_ADDR           ;-- Invoke PendSV exception
       ldr    r0,  =PENDSVSET
       str    r0,  [r1]

       bx     lr

;-----------------------------------------------------------------------------
tn_cpu_save_sr:

       mrs    r0, PRIMASK
       cpsid  I
       bx     lr

;-----------------------------------------------------------------------------
tn_cpu_restore_sr

       msr    PRIMASK, r0
       bx     lr

;-----------------------------------------------------------------------------
tn_chk_irq_disabled:

       mrs    r0,PRIMASK
       bx     lr

;-----------------------------------------------------------------------------
       nop

       END

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------

