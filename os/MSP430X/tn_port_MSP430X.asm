;-----------------------------------------------------------------------------
;
; TNKernel real-time kernel
;
;   MSP430X port
;
;   Compiler: IAR EW430
;
; Copyright © 2004, 2016 Yuri Tiomkin
;
; All rights reserved.
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
;-----------------------------------------------------------------------------


#include "io430.h"
#include "tn_config.h"

 ;-- External references

    EXTERN    tn_curr_run_task
    EXTERN    tn_next_task_to_run
    EXTERN    tn_system_state
    EXTERN    tn_sp_info
    EXTERN    tn_idle_task
    EXTERN    tn_reset_idle_task_stack
    EXTERN    tn_check_stack
    EXTERN    tn_inside_int_cnt
    EXTERN    delete_task_resources
    EXTERN    tn_func_exit_stack_start
    

  ;-- Public functions declared in this file

    PUBLIC    tn_start_exe
    PUBLIC    tn_switch_context_asm
    PUBLIC    tn_chk_irq_disabled
    PUBLIC    tn_switch_context_exit
    PUBLIC    tn_int_exit_asm
    PUBLIC    tn_switch_to_int_stack
    PUBLIC    tn_switch_to_task_stack
    PUBLIC    tn_task_exit_asm
    PUBLIC    tn_func_to_ul_1
    PUBLIC    tn_func_to_ul_2
    PUBLIC    conv_const_ptr_to_ptr


;-----------------------------------------------------------------------------
    RSEG      CODE
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
tn_start_exe

    movx      #1, &tn_system_state  ;--  tn_system_state = 1;

    movx.a    &tn_next_task_to_run, R13
    movx.a    R13, &tn_curr_run_task
    movx.a    @R13, SP

    popm.a    #12, R15             ;-- Restore R4..R15

    reti

   ;-- Should never be here

l_s0
    jmp       l_s0

;----------------------------------------------------------------------------
tn_switch_context_asm

;------Construct interrupt-like stack ------------------------------

    popx.a    R14
    pushx.w   R14

    rram.a    #4, R14              ; merge upper 4 bits of RA into
    and.w     #0xf000, R14         ; upper 4 bits of SR
    bis.w     SR, R14
    pushx.w   R14
;------------------------------------

    pushm.a   #12, R15             ; Save R4..R15
;----------------------------

#if defined  TN_STACK_CHECK

    movx.a    &tn_next_task_to_run, R12
    calla     #tn_check_stack

#endif

#if defined  TN_RESET_IDLE_STACK

    cmpx.a    #tn_idle_task, &tn_next_task_to_run
    jne       nrs_l0
    calla     #tn_reset_idle_task_stack

nrs_l0:

#endif

    movx.a    &tn_curr_run_task, R13
    movx.a    SP, 0(R13)
    movx.a    &tn_next_task_to_run, R13

    movx.a    R13, &tn_curr_run_task
    movx.a    @R13, SP

    popm.a    #12, R15           ; Restore R4..R15

    reti


;----------------------------------------------------------------------------
; 
;----------------------------------------------------------------------------
tn_task_exit_asm

   ; Set stack as 'tn_func_exit_stack_start

    movx.a    &tn_func_exit_stack_start, R13
    movx.a    R13, SP                    ; set new SP

    calla     #delete_task_resources

   ; Switch context - not store current context 

    movx.a    &tn_next_task_to_run, R13
    movx.a    R13, &tn_curr_run_task
    movx.a    @R13, SP

    popm.a    #12, R15           ; Restore R4..R15

    reti

;----------------------------------------------------------------------------
; ToDo
;----------------------------------------------------------------------------
tn_switch_context_exit

    movx.a    &tn_next_task_to_run, R13
    movx.a    R13, &tn_curr_run_task
    movx.a    @R13, SP

    popm.a    #12, R15           ; Restore R4..R15

    reti
    
;----------------------------------------------------------------------------
tn_int_exit_asm

    popx.a    R12  ; Case tn_int_exit_exit() is invoked by CALLA,
                   ; here we clean the stack after CALLA

    cmpx.a    &tn_curr_run_task, &tn_next_task_to_run
    jeq       l_exit2


#if defined  TN_STACK_CHECK

    movx.a    &tn_next_task_to_run, R12
    calla     #tn_check_stack

#endif

#if defined  TN_RESET_IDLE_STACK

    cmpx.a    #tn_idle_task, &tn_next_task_to_run
    jne       nrs_l
    calla     #tn_reset_idle_task_stack

nrs_l:

#endif

    movx.a    &tn_curr_run_task, R13
    movx.a    SP, 0(R13)
    movx.a    &tn_next_task_to_run, R13
    movx.a    R13, &tn_curr_run_task
    movx.a    @R13, SP

l_exit2

    subx.w    #1,  &tn_inside_int_cnt

    popm.a    #12, R15        ;-- restore R4..R15

    reti

;----------------------------------------------------------------------------
tn_chk_irq_disabled

    bit       #8, SR          ;-- Check GIE bit 1- int enable 0 - ints enabled
    jnz       label_1         ;-- ints enabled
    mova      #1, R12

    reta

label_1

    mova      #0, R12

    reta

;----------------------------------------------------------------------------
tn_switch_to_int_stack:

    movx.a    #tn_sp_info, R15
    movx.a    0(R15), R12                ; load current task SP
    
    popx.a    R14                        ; the return address - from stack
    
    cmpx.a    #0, R12                    ; if task SP mark was not zero - do nothing
    jne       int_stack_l
    
    movx.a    4(R15), R13                ; otherwise, load current interrupt SP
  
    movx.a    SP, 0(R15)                 ; store current (task) SP as task SP
    movx.a    R13, SP                    ; set interrupt SP as the current SP 

int_stack_l:

    bra       r14                              

;------------------------------------
tn_switch_to_task_stack:

    popx.a    R14                        ; load the return address from SP
 
    cmpx.a    #0, R12                    ; if old task SP = 0, do nothing
    jne       task_stack_l

              
    movx.a    #tn_sp_info, R15           ; otherwise,load interrupt SP
    movx.a    0(R15), SP                 ; set the task SP as current SP
    movx.a    #0, 0(R15)                 ; wr 0 to task SP as the mark - interrupt SP is valid

task_stack_l:

    bra       R14    

;---------------------Just to make MISRA 2012 happy ----------------------
tn_func_to_ul_1:

    PUSHM.A #0x1, R12
    POPM.W  #0x2, R13
    RETA

;---------------------Just to make MISRA 2012 happy ----------------------
tn_func_to_ul_2:

    PUSHM.A #0x1, R12
    POPM.W  #0x2, R13
    RETA

;---------------------Just to make MISRA 2012 happy ----------------------
conv_const_ptr_to_ptr:

    RETA
;-------------------------------------------

    END


