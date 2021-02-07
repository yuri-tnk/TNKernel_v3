;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 21638 $
;;

;---------------------------------------------------------------
; Macros and definitions for the whole file
;---------------------------------------------------------------


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)

;
; The module in this file are included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.

        SECTION .intvec:CODE:NOROOT(2)

        PUBLIC  __vector
        PUBLIC  __vector_0x14
        PUBLIC  __iar_program_start
;        EXTERN undef_handler, prefetch_handler, data_handler,
        EXTERN tn_cpu_irq_isr, tn_cpu_fiq_isr 


        ARM
__vector:
        ;;
        ldr   pc,[pc,#+24]              ;; Reset
        ldr   pc,[pc,#+24]              ;; Undefined instructions
        B   .                           ;; Software interrupt (SWI/SVC)
        ldr   pc,[pc,#+24]              ;; Prefetch abort
        ldr   pc,[pc,#+24]              ;; Data abort
__vector_0x14
        DC32  0xFFFFFFFF                ;; 0 - (sum of other vectors instructions)
        ldr   pc,[pc,#+24]              ;; IRQ
        ldr   pc,[pc,#+24]              ;; FIQ

        DC32  __iar_program_start       ;; Reset
        DC32  undef_handler             ;; Undefined instructions
        DC32  0                         ;; Software interrupt (SWI/SVC)
        DC32  prefetch_handler          ;; Prefetch abort
        DC32  data_handler              ;; Data abort
        DC32  0                         ;; RESERVED
        DC32  tn_cpu_irq_isr   ; IRQ    (+24)
        DC32  tn_cpu_fiq_isr   ; FRQ    (+28)




; --------------------------------------------------
; ?cstartup -- low-level system initialization code.
;
; After a reser execution starts here, the mode is ARM, supervisor
; with interrupts disabled.
;



        SECTION .text:CODE:NOROOT(2)

;        PUBLIC  ?cstartup
        EXTERN  ?main
        REQUIRE __vector

        EXTERN tn_startup_hardware_init
        ARM
        
undef_handler:
          B .

prefetch_handler: 
          B .

data_handler:
          B .


__iar_program_start:
?cstartup:

; Add initialization nedded before setup of stackpointers here


                ldr     r0, =tn_startup_hardware_init
                mov     lr, pc
                bx      r0


; Mode, correspords to bits 0-5 in CPSR

MODE_BITS       DEFINE        0x1F                ; Bit mask for mode bits in CPSR
USR_MODE        DEFINE        0x10                ; User mode
FIQ_MODE        DEFINE        0x11                ; Fast Interrupt Request mode
IRQ_MODE        DEFINE        0x12                ; Interrupt Request mode
SVC_MODE        DEFINE        0x13                ; Supervisor mode
ABT_MODE        DEFINE        0x17                ; Abort mode
UND_MODE        DEFINE        0x1B                ; Undefined Instruction mode
SYS_MODE        DEFINE        0x1F                ; System mode

; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
                mrs     r0,cpsr                             ; Original PSR value
                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(IRQ_STACK); & 0xFFFFFFF8     ; End of IRQ_STACK
;-------- My -----------
                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#FIQ_MODE                     ; Set FIQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(FIQ_STACK); & 0xFFFFFFF8     ; End of FIQ_STACK
   ;-- In SVC mode
                bic     r0,r0,#MODE_BITS                  ; Clear the mode bits
                orr     r0,r0,#SVC_MODE                   ; Set Supervisor mode bits
                msr     cpsr_c,r0                         ; Change the mode
                ldr     sp,=SFE(CSTACK) ;& 0xFFFFFFF8      ; End of CSTACK

;------- End My --------


#ifdef __ARMVFP__
        ;; Enable the VFP coprocessor.

        MOV     r0, #0x40000000         ; Set EN bit in VFP
        FMXR    fpexc, r0               ; FPEXC, clear others.

;
; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
;

        MOV     r0, #0x01000000         ; Set FZ bit in VFP
        FMXR    fpscr, r0               ; FPSCR, clear others.
#endif

;
; Add more initialization here
;

; Continue to ?main for C-level initialization.

        LDR     r0, =?main
        BX      r0

        END
        
        
