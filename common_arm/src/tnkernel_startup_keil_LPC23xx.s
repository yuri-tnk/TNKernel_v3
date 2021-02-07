;------------------------------------------------------------------------------
;
;    TNKernel startup file for LPC21XX/LPC22XX/LPC23XX processors
;
;    ARM(c) assembler
;
; Copyright © 2004,2016 Yuri Tiomkin
; All rights reserved.
;
;Permission to use, copy, modify, and distribute this software in source
;and binary forms and its documentation for any purpose and without fee
;is hereby granted, provided that the above copyright notice appear
;in all copies and that both that copyright notice and this permission
;notice appear in supporting documentation.
;
;THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
;ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
;FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;SUCH DAMAGE.

;  ver 3


;-- Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

Mode_USR  EQU      0x10
Mode_FIQ  EQU      0x11
Mode_IRQ  EQU      0x12
Mode_SVC  EQU      0x13
Mode_ABT  EQU      0x17
Mode_UND  EQU      0x1B
Mode_SYS  EQU      0x1F

I_Bit     EQU      0x80    ; when I bit is set, IRQ is disabled
F_Bit     EQU      0x40    ; when F bit is set, FIQ is disabled


;--- A place to set stacks size

UND_Stack_Size  EQU   0x00000004
SVC_Stack_Size  EQU   0x00000004	   ;-- Does not uses here
ABT_Stack_Size  EQU   0x00000004
FIQ_Stack_Size  EQU   0x00000004
IRQ_Stack_Size  EQU   512
USR_Stack_Size  EQU  1024	   ;-- Application Stack - in fact, For SVC mode here

Stack_Size      EQU     (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + \
                         FIQ_Stack_Size + IRQ_Stack_Size + USR_Stack_Size)

;---------------------------------------------------------------------------
     EXPORT     __initial_sp
     AREA   STACK, NOINIT, READWRITE,ALIGN=3 ;ALIGN=2

Stack_Mem       SPACE   Stack_Size
__initial_sp    

Top_Stack       EQU     Stack_Mem + Stack_Size

Heap_Size       EQU     0x00000000

    AREA    HEAP, NOINIT, READWRITE, ALIGN=3

Heap_Mem        SPACE   Heap_Size


  ;--------------- Start ----------------------------------------------

       AREA    RESET, CODE, READONLY
       ARM

       IMPORT  tn_cpu_irq_isr
       IMPORT  tn_cpu_fiq_isr
       IMPORT  tn_startup_hardware_init
       EXPORT  reset

reset
        ldr  pc, reset_handler_address
        ldr  pc, undef_handler_address
        ldr  pc, swi_handler_address
        ldr  pc, pabort_handler_address
        ldr  pc, dabort_handler_address
        DCD  0xB8A06F58                  ; 0 - (sum of other vectors instructions)
        ldr  pc, irq_handler_address
        ldr  pc, fiq_handler_address

reset_handler_address   DCD  Reset_Handler
undef_handler_address   DCD  undef_handler
swi_handler_address     DCD  swi_handler
pabort_handler_address  DCD  pabort_handler
dabort_handler_address  DCD  dabort_handler
                        DCD   0x00
irq_handler_address     DCD  tn_cpu_irq_isr
fiq_handler_address     DCD  tn_cpu_fiq_isr

        EXPORT  Reset_Handler

Reset_Handler
        b  _start
undef_handler
        b  undef_handler
swi_handler
        b  swi_handler
pabort_handler
        b  pabort_handler
dabort_handler
        b  dabort_handler


;----------------------------------------------------------------------------

_start

        ldr   r0,=tn_startup_hardware_init      ;-- vital hardware init
        mov   lr,pc
        bx    r0

     ;--- init stacks

        ldr   r0, =Top_Stack

;-- Enter Undefined Instruction Mode and set its Stack Pointer

        MSR   CPSR_c, #Mode_UND :OR: I_Bit :OR: F_Bit
        MOV   SP, R0
        SUB   R0, R0, #UND_Stack_Size

;-- Enter Abort Mode and set its Stack Pointer

        MSR   CPSR_c, #Mode_ABT :OR: I_Bit :OR: F_Bit
        MOV   SP, R0
        SUB   R0, R0, #ABT_Stack_Size

;-- Enter FIQ Mode and set its Stack Pointer

        MSR   CPSR_c, #Mode_FIQ :OR: I_Bit :OR: F_Bit
        MOV   SP, R0
        SUB   R0, R0, #FIQ_Stack_Size

;-- Enter IRQ Mode and set its Stack Pointer

        MSR   CPSR_c, #Mode_IRQ :OR: I_Bit :OR: F_Bit
        MOV   SP, R0
        SUB   R0, R0, #IRQ_Stack_Size

;--- Enter Supervisor Mode and set its Stack Pointer
;--- This is application  stack top
;--- Leave core in SVC mode

        MSR   CPSR_c, #Mode_SVC :OR: I_Bit :OR: F_Bit
        MOV   SP, R0

;--- Enter the C code

; Enter the C code

                IMPORT  __main
                LDR     R0, =__main
                BX      R0


; User Initial Stack & Heap
                AREA    |.text|, CODE, READONLY

           ;     IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem +      Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR


                END
;----------------------------------------------------------------------------
