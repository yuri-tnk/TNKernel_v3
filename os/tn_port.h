/*

  TNKernel real-time kernel

  Copyright © 2004, 2016 Yuri Tiomkin
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

*/

   /* ver 3  */


#ifndef  TN_PORT_H
#define  TN_PORT_H

#ifdef __cplusplus
extern "C"  {
#endif


  /* ========================================================== */
  /*                    MSP430X port                            */
  /* ========================================================== */

#ifdef  TNKERNEL_PORT_MSP430X

#include  "io430.h"
#include  <intrinsics.h>

#define  TN_INLINE   inline
 
#define  TN_ALIGN_ATTR           __no_init
#define  TN_ALIGN_ATTR_START     __no_init
#define  TN_ALIGN_ATTR_END

#define  TN_TIMER_STACK_SIZE     256U   //128 // 64
#define  TN_IDLE_STACK_SIZE      256U   // 96 // 60
#define  TN_MIN_STACK_SIZE        96U   // 60

#define  TN_BITS_IN_INT           16U

#define  TN_ALIG                sizeof(void*)

#define  MAKE_ALIG(a)           ((sizeof(a) + (TN_ALIG-1)) & (~(TN_ALIG-1)))

#define  TN_DQUEUE_ELEMENT      unsigned long

  //----------------------------------------------------

//-- 0..15,  the priority 0 always is used by timers task
#define  TN_NUM_PRIORITY        TN_BITS_IN_INT  

#define  TN_MIN_PRIORITY        15U

#define  TN_NO_WAIT             0UL
#define  TN_WAIT_INFINITE       0xFFFFFFFFUL
#define  TN_FILL_STACK_VAL      0xDEADU
#define  TN_INVALID_VAL         0xFFFFU
#define  TN_INVALID_VAL_UL      0xFFFFFFFFUL

    //-- Assembler & C MSP430 port functions prototypes

  void  tn_switch_context_asm(void);
  void  tn_switch_context_int_asm(void);
  void  tn_start_exe(void);
  unsigned int tn_chk_irq_disabled(void);
  void tn_switch_context_exit(void);
  unsigned char * tn_switch_to_int_stack(void);
  void tn_switch_to_task_stack(volatile unsigned char * val);
  void tn_reset_idle_task_stack(void);
  __task void tn_check_stack(void * tcb);
  __task void tn_switch_context(void);
  void tn_int_exit_asm(void);
  void tn_task_exit_asm(void * tcb);
  unsigned long tn_func_to_ul_1(void (*func)(void * par));
  unsigned long tn_func_to_ul_2(void (*func)(void));

  void * conv_const_ptr_to_ptr(const void * ptr);

    //-- Interrupt processing

#define  TN_INTSAVE_DATA_INT
#define  TN_INTSAVE_DATA          __istate_t tn_save_status_reg;

#define  tn_disable_interrupt()  { tn_save_status_reg = __get_interrupt_state(); \
                                  __disable_interrupt(); }
#define  tn_enable_interrupt()   __set_interrupt_state(tn_save_status_reg)

    //-- MSP430x - there are no nested interrupts

#define  tn_idisable_interrupt() 
#define  tn_ienable_interrupt()  

#define  tn_disable_interrupt_nsc()  __disable_interrupt();  // disable interrupts without saving status reg

/*-------------------------------- */


// Use this macro when CPU uses sleep mode and after interrupt it have 
// to be wake-up

// Temporary store R15
// Put SR before interrupt value to R15
// Check CPUOFF=1 -> CPU was off -> we were in the sleep mode before interrupt 
// If so, clear CPU sleep mode bits (will be used after int exit)
// Pop register R15 to return stack to the original state
// Save registers R4..R15
// Switch to interrupt stack

#define  tn_int_enter(var) \
{asm("PUSHX.A  R15\n"\
     "MOV.W    4(SP), R15\n"\
     "BIT.W    #0x10, R15\n"\
     "JNC      not_sleep_l\n"\
     "BIC.W    #0xf0, 4(SP)\n"\
     "not_sleep_l:   POPX.A    R15\n"\
     "PUSHM.A #12, R15\n");\
     tn_inside_int_cnt++;\
     var = tn_switch_to_int_stack(); }


// Use this macro when CPU does Not uses sleep mode or after interrupt 
// it have NOT to be wake-up

#define tn_int_enter_ex(var)\
{ asm( "PUSHM.A #12, R15\n");\
tn_inside_int_cnt++;\
var = tn_switch_to_int_stack(); }


#define  tn_int_exit(var)   { tn_switch_to_task_stack(var); tn_int_exit_asm(); } 

typedef struct _INTSTACKINFO
{
   unsigned char * task_sp;
   unsigned int * int_sp;

}INTSTACKINFO;

void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem);

  /* ========================================================== */
  /*                    ARM port                                */
  /* ========================================================== */

#elif defined  TNKERNEL_PORT_ARM

#if defined (__ICCARM__)    // IAR ARM

#define TN_INLINE          inline
#define TN_ALIGN_ATTR_END

#elif defined (__GNUC__)    //-- GNU Compiler

#define TN_INLINE          inline
#define TN_ALIGN_ATTR_END  __attribute__((aligned(0x8)))

#elif defined ( __CC_ARM )  //-- RealView Compiler

#define TN_INLINE          __inline
#define TN_ALIGN_ATTR_END  __attribute__((aligned(0x8)))

#else
  #error Unknown compiler for ARM7
#endif

#define  TN_ALIGN_ATTR
#define  TN_NO_WAIT                0UL
#define  TN_INVALID_VAL_UL         0xFFFFFFFFUL

#define  TN_DQUEUE_ELEMENT      void *

typedef struct _INTSTACKINFO
{
   unsigned char * task_sp;
   unsigned int * int_sp;

}INTSTACKINFO;


// disable interrupts without saving status reg
#define  tn_disable_interrupt_nsc()  (void)tn_cpu_save_sr()  

#define  TN_MIN_STACK_SIZE         64UL

#define  TN_BITS_IN_INT            32UL

#define  TN_ALIG                   sizeof(void*)
#define  MAKE_ALIG(a)  ((sizeof(a) + (TN_ALIG-1UL)) & (~(TN_ALIG-1UL)))

#define  TN_NUM_PRIORITY        TN_BITS_IN_INT  //-- 0..31  Priority 0 always is used by timers task

#define  TN_WAIT_INFINITE       0xFFFFFFFFUL
#define  TN_FILL_STACK_VAL      0xDEADBEEFUL
#define  TN_INVALID_VAL         0xFFFFFFFFUL

  typedef void (*int_func)(void);

  void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem);
  void tn_switch_to_task_stack(volatile unsigned char * val);
  void tn_reset_idle_task_stack(void);
  void tn_check_stack(void * tcb);

    //-- Assembler functions prototypes

  void  tn_switch_context_exit(void);
  void  tn_switch_context(void);
  void  tn_cpu_irq_isr(void);
  void  tn_cpu_fiq_isr(void);

  unsigned int tn_cpu_save_sr(void);
  void  tn_cpu_restore_sr(unsigned int sr);
  void  tn_start_exe(void);

  int   tn_chk_irq_disabled(void);
  int   tn_inside_irq(void);

  int   ffs_asm(unsigned int val);

  void  tn_arm_disable_interrupts(void);
  void  tn_arm_enable_interrupts(void);

  void tn_task_exit_asm(void * tcb);

  unsigned long tn_func_to_ul_1(void (*func)(void * par));
  unsigned long tn_func_to_ul_2(void (*func)(void));
  unsigned long tn_void_to_ul(void * par);
  int_func tn_ul_to_int_func(unsigned long par);

  extern volatile int tn_int_counter;

    //-- ARM7 - the nested interrupts are NOT implemented

#define  TN_INTSAVE_DATA_INT
#define  TN_INTSAVE_DATA          unsigned int tn_save_status_reg;
#define  tn_disable_interrupt()   tn_save_status_reg = tn_cpu_save_sr()
#define  tn_enable_interrupt()    tn_cpu_restore_sr(tn_save_status_reg)
#define  tn_idisable_interrupt()  
#define  tn_ienable_interrupt()   


  /* ========================================================== */
  /*                    Cortex M0..M7 port                      */
  /* ========================================================== */

#elif defined (TN_CORTEX_M0) || defined(TN_CORTEX_M0P) \
          || defined(TN_CORTEX_M3) || defined(TN_CORTEX_M4) \
                || defined(TN_CORTEX_M7) 

#if defined (__ICCARM__)    // IAR ARM

#define TN_INLINE          inline
#define TN_ALIGN_ATTR_END

#elif defined (__GNUC__)    //-- GNU Compiler

#define TN_INLINE          inline
#define TN_ALIGN_ATTR_END  __attribute__((aligned(0x8)))

#elif defined ( __CC_ARM )  //-- RealView Compiler

#define TN_INLINE          __inline
#define TN_ALIGN_ATTR_END  __attribute__((aligned(0x8)))

#elif defined ( __clang__ )

#define TN_INLINE          __inline
#define TN_ALIGN_ATTR_END  __attribute__((aligned(0x8)))

#else

  #error Unknown compiler for Cortex CPU

#endif

//---- New

#define  TN_DQUEUE_ELEMENT      void *

typedef struct _INTSTACKINFO
{
   unsigned char * task_sp;
   unsigned int * int_sp;

}INTSTACKINFO;

void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem);

#define  TN_ALIGN_ATTR

#define  TN_NO_WAIT                0UL
#define  TN_INVALID_VAL_UL         0xFFFFFFFFUL


#if defined(TN_CORTEX_M0) || defined(TN_CORTEX_M0P) || defined(TN_CORTEX_M3) 
#define  TN_MIN_STACK_SIZE          64U     // No FPU  
#elif defined(TN_CORTEX_M4) || defined(TN_CORTEX_M7) 
   #if defined TN_SUPPORT_FPU
      #define  TN_MIN_STACK_SIZE    96U
   #else
      #define  TN_MIN_STACK_SIZE    64U      
   #endif
#endif

#define  TN_BITS_IN_INT             32U

#define  TN_ALIG                    sizeof(void*)
#define  MAKE_ALIG(a)  ((sizeof(a) + (TN_ALIG-1UL)) & (~(TN_ALIG-1UL)))

#define  TN_NUM_PRIORITY        TN_BITS_IN_INT  //-- 0..31  Priority 0 always is used by timers task

#define  TN_WAIT_INFINITE       0xFFFFFFFFU
#define  TN_FILL_STACK_VAL      0xDEADBEEFU
#define  TN_INVALID_VAL         0xFFFFFFFFU

    //-- Assembler functions prototypes

   void  tn_switch_context_exit(void);
   void  tn_switch_context(void);

   unsigned int tn_cpu_save_sr(void);
   void  tn_cpu_restore_sr(unsigned int sr);
   void  tn_start_exe(void);
   int   tn_chk_irq_disabled(void);
   int   ffs_asm(unsigned int val);

   void tn_arm_disable_interrupts(void);
   void tn_arm_enable_interrupts(void);

   unsigned long tn_func_to_ul_1(void (*func)(void * par));
   unsigned long tn_func_to_ul_2(void (*func)(void));
   unsigned long tn_void_to_ul(void * par);

   void tn_int_exit_asm(void);

   void tn_int_exit(void);
   void tn_reset_idle_task_stack(void);
   void tn_check_stack(void * tcb);

   void tn_task_exit_asm(void * tcb);
   void * conv_const_ptr_to_ptr(const void * ptr);

    //-- Interrupt processing - the CPU specific

        // All Cortex-M CPU supports nested interrupts

#define  TN_INTSAVE_DATA_INT     unsigned int tn_save_status_reg;
#define  TN_INTSAVE_DATA         unsigned int tn_save_status_reg;
#define  tn_disable_interrupt()  tn_save_status_reg = tn_cpu_save_sr()
#define  tn_enable_interrupt()   tn_cpu_restore_sr(tn_save_status_reg)

#define  tn_idisable_interrupt() tn_save_status_reg = tn_cpu_save_sr()
#define  tn_ienable_interrupt()  tn_cpu_restore_sr(tn_save_status_reg)

// disable interrupts without saving status reg 
#define  tn_disable_interrupt_nsc()  (void)tn_cpu_save_sr()  


  /* ----------Port not defined  ----- */

#else
   #error "TNKernel port is not defined"
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* #ifndef  TN_PORT_H */








