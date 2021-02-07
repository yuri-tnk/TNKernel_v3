/*

  TNKernel real-time kernel

  Copyright © 2004, 2016  Yuri Tiomkin
  Copyright © 2008        Sergey Koshkin (Cortex - M3 port)
  Copyright © 2007, 2013  Vyacheslav Ovsiyenko (Cortex - M4F port)

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

#include <string.h>

#include "tn_config.h"
#include "tn.h"

#if 0
volatile unsigned int * g_dbg_ptr = NULL;
volatile unsigned int   g_dbg_stk_size = 0UL;
#endif
//----------------------------------------------------------------------------
//   Processor specific routine - here for Cortex CPU
//
//   sizeof(void*) = sizeof(int)
//----------------------------------------------------------------------------
unsigned int * tn_stack_init(void (*task_func)(void *param), 
                             void * stack_start, 
                             void * param)
{
   unsigned int * stk;
   unsigned long tmp;

 //-- filling register's position in the stack - for debugging only

   stk  = (unsigned int *)stack_start;       //-- Load stack pointer

   *stk = 0x01000000UL;                       //-- xPSR
   stk--;


   tmp  = tn_func_to_ul_1(task_func);  // To make MISRA-2012 happy
   tmp |= 1U;
   *stk = tmp;
   stk--;

   tmp = tn_func_to_ul_2(tn_task_exit);  // To make MISRA-2012 happy
   tmp |= 1U;
   *stk = tmp;
   stk--;

   *stk = 0x12121212UL;                       //-- R12
   stk--;
   *stk = 0x03030303UL;                       //-- R3
   stk--;
   *stk = 0x02020202UL;                       //-- R2
   stk--;
   *stk = 0x01010101UL;                       //-- R1
   stk--;

   tmp  =  tn_void_to_ul(param);   // To make MISRA-2012 happy
   *stk = tmp;                     //-- R0 - task's function argument
   stk--;

#if (TN_SUPPORT_FPU == 1)
   
  // Return to Thread mode, exception return uses non-floating-point state
  // from the PSP and execution uses PSP after return
   
   *stk = 0xFFFFFFFDUL;
   stk--;
#endif

   *stk = 0x11111111UL;                       //-- R11
   stk--;
   *stk = 0x10101010UL;                       //-- R10
   stk--;
   *stk = 0x09090909UL;                       //-- R9
   stk--;
   *stk = 0x08080808UL;                       //-- R8
   stk--;
   *stk = 0x07070707UL;                       //-- R7
   stk--;
   *stk = 0x06060606UL;                       //-- R6
   stk--;
   *stk = 0x05050505UL;                       //-- R5
   stk--;
   *stk = 0x04040404UL;                       //-- R4

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

void tn_check_stack(void * tcb)
{
   unsigned int * ptr;
   TN_TCB * task = (TN_TCB *)tcb;
  // unsigned int stk_size = (unsigned int)ptcb->stk_size - 4U;
  // unsigned long tmp;
   unsigned long end_stack_ul;

 //  ptr = ptcb->stk_start;   //-- Top of Stack - 2
 //  ptr = ptr - stk_size;

   // Stack End(Top) - Minimal RAM Addr
   end_stack_ul = (unsigned long)task->stk_start - (task->stk_size << 2U) + sizeof(int); 

   // MISRA 2012 18.4
 //  tmp = (unsigned long) ptcb->stk_start;     
 //  tmp -= stk_size << 2U; // * sizeof(unsigned int);
 //  ptr = (unsigned int *)tmp;

   ptr = (unsigned int *)end_stack_ul;
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
void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem)
{
  // return TERR_NO_ERR;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


