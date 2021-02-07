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

  /* ver 3 */

#include "tn_config.h"
#include "tn.h"


volatile int tn_int_counter = 0;

//----------------------------------------------------------------------------
unsigned int * tn_stack_init(void (*task_func)(void *param), 
                             void * stack_start, 
                             void * param)
{
   unsigned int * stk;
   unsigned long tmp;

 //-- filling register's position in stack (except R0,CPSR,SPSR positions) -
 //-- for debugging only

   stk  = (unsigned int *)stack_start;      //-- Load stack pointer

   tmp  = tn_func_to_ul_1(task_func);  // To make MISRA-2012 happy
   tmp &= ~1UL;
   *stk = tmp;
   stk--;

   tmp = tn_func_to_ul_2(tn_task_exit);  // LR To make MISRA-2012 happy
   *stk = tmp;
   stk--;
 
   *stk = 0x12121212UL;              //-- R12
   stk--;

   *stk = 0x11111111UL;              //-- R11
   stk--;

   *stk = 0x10101010UL;              //-- R10
   stk--;

   *stk = 0x09090909UL;              //-- R9
   stk--;

   *stk = 0x08080808UL;              //-- R8
   stk--;

   *stk = 0x07070707UL;              //-- R7
   stk--;

   *stk = 0x06060606UL;              //-- R6
   stk--;

   *stk = 0x05050505UL;              //-- R5
   stk--;

   *stk = 0x04040404UL;              //-- R4
   stk--;

   *stk = 0x03030303UL;              //-- R3
   stk--;

   *stk = 0x02020202UL;              //-- R2
   stk--;

   *stk = 0x01010101UL;              //-- R1
   stk--;

   tmp  =  tn_void_to_ul(param);   // To make MISRA-2012 happy
   *stk = tmp;                     //-- R0 - task's function argument
   stk--;


#if 0
   if ((unsigned int)task_func & 1) //-- task func is in the THUMB mode
      *stk = (unsigned int)0x33;    //-- CPSR - Enable both IRQ and FIQ ints + THUMB
   else
      *stk = (unsigned int)0x13;    //-- CPSR - Enable both IRQ and FIQ ints
#endif
   
   // Bit 6- FIQ dis, Bit 7 - IRQ dis
   tmp  = tn_func_to_ul_1(task_func);
   if((tmp & 1UL) == 1UL) //-- task func is in the THUMB mode
   {
      *stk = 0x73UL;    //-- CPSR - Enable IRQ int + THUMB
   }
   else
   {
      *stk = 0x53UL;    //-- CPSR - Enable IRQ int
   }

   return stk;
}

//----------------------------------------------------------------------------
void tn_reset_idle_task_stack(void)
{
   task_function task_func   = tn_idle_task.task_func_addr; 
   void * stack_start        = (void*)tn_idle_task.stk_start; 
   void * param              = tn_idle_task.task_func_param;

   tn_idle_task.task_stk = tn_stack_init(task_func, stack_start, param);
}

//----------------------------------------------------------------------------
void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem)
{
}

//----------------------------------------------------------------------------
void tn_check_stack(void * tcb)
{
   unsigned int * ptr;
   unsigned long end_stack_ul;
   TN_TCB * task = (TN_TCB *)tcb;

   // Stack End(Top) at minimal stack RAM Addr
   end_stack_ul = (unsigned long)task->stk_start - (task->stk_size << 2U) + sizeof(int); 

   ptr = (unsigned int *)end_stack_ul;  // MISRA 2012 18.4
   if(*ptr != TN_FILL_STACK_VAL)
   {
#if 0
      g_dbg_ptr = ptr;
      g_dbg_stk_size = stk_size;
#endif
      // Just stub - put your stack overflow error handler here   
      for(;;)
      {
      }
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


