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

 /* ver 3.0 */

#include <string.h>

#include "tn_config.h"
#include "tn.h"

extern unsigned char _STACK_SIZE; // from linker script

extern __no_init TN_TCB  tn_idle_task;
extern volatile int tn_inside_int_cnt;

void tn_idle_task_func(void * par);

//----------------------------------------------------------------------------
// Processor specific routine - here for MSP430X
//----------------------------------------------------------------------------
unsigned int * tn_stack_init(void (*task_func)(void *param), 
                             void * stack_start, 
                             void * param)
{
   unsigned int * stk;
   unsigned int hi_pc;
   unsigned long tmp;

 //-- filling register's position in stack - for debugging only

   stk  = (unsigned int *)stack_start; // Load stack pointer
   stk--;

   tmp = tn_func_to_ul_2(tn_task_exit);

   *stk = (unsigned int)(tmp >> 16); //(unsigned int)((unsigned long)tn_task_exit >> 16);
   stk--;
   *stk = (unsigned int)(tmp & 0x0000FFFFUL); // (unsigned int)((unsigned long)tn_task_exit & 0x0000FFFFUL);
   stk--;


/*
SP old ->  |  Item n-1             |
           |-----------------------|
           |  PC.15:0              |
           |---------------------- |
SP         |  PC.19:16 |  SR.11:0  |
           |-----------------------|
           |                       |
           |-----------------------|
*/

   tmp = tn_func_to_ul_1(task_func);

   *stk = (unsigned int)(tmp & 0x0000FFFFUL);  //((unsigned long)task_func & 0x0000FFFFUL);
   stk--;

   hi_pc = (unsigned int)(tmp >> 16); //((unsigned long)task_func >> 16);
   hi_pc <<=12;
   *stk = 0x0008U | hi_pc;                     //-- status register  - GIE enabled
   stk--;

   *stk = 0x0015;
   stk--;
   *stk = 0x1515;
   stk--;

   *stk = 0x0014;
   stk--;
   *stk = 0x1414;
   stk--;

   *stk = 0x0013;
   stk--;
   *stk = 0x1313;
   stk--;

   *stk = (unsigned int)((unsigned long)param>>16);  //-- R12 is used for 1st parameter
   stk--;
   *stk = (unsigned int)((unsigned long)param & 0x0000FFFFUL);  //-- R12 is used for 1st parameter
   stk--;

   *stk = 0x0011;
   stk--;
   *stk = 0x1111;
   stk--;

   *stk = 0x0010;
   stk--;
   *stk = 0x1010;
   stk--;

   *stk = 0x0009;
   stk--;
   *stk = 0x0909;
   stk--;

   *stk = 0x0008;
   stk--;
   *stk = 0x0808;
   stk--;

   *stk = 0x0007;
   stk--;
   *stk = 0x0707;
   stk--;

   *stk = 0x0006;
   stk--;
   *stk = 0x0606;
   stk--;

   *stk = 0x0005;
   stk--;
   *stk = 0x0505;
   stk--;

   *stk = 0x0004;
   stk--;
   *stk = 0x0404;

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
__task void tn_switch_context(void)
{ 
   TN_INTSAVE_DATA

   tn_disable_interrupt();

   if(tn_curr_run_task != tn_next_task_to_run)
   {
      if(tn_next_task_to_run == &tn_idle_task)
      {
//P4OUT |= BIT2;
         tn_reset_idle_task_stack();

      }
      else
      {
//P4OUT &= ~BIT2;
      }
      tn_switch_context_asm();
   }

   tn_enable_interrupt();
}

//----------------------------------------------------------------------------
__task void tn_check_stack(void * tcb)
{
   unsigned int * ptr;
   TN_TCB * ptcb = (TN_TCB *)tcb;
   unsigned int stk_size = (unsigned int)ptcb->stk_size - 4U;

   ptr = ptcb->stk_start;   //-- Top of Stack - 2
   ptr = ptr - stk_size;
   if(*ptr != TN_FILL_STACK_VAL)
   {
      for(;;)
      {
      }
   }
} 

//----------------------------------------------------------------------------
BOOL tn_inside_int(void)
{
   int rc = FALSE;
   if(tn_inside_int_cnt > 0)
   {
      rc = TRUE;
   }
   return rc;
}

//----------------------------------------------------------------------------
void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem)
{

   unsigned char * ptr;

#pragma segment = "CSTACK"
   si->int_sp = (unsigned int *)__segment_begin("CSTACK");

   ptr = (unsigned char *)((void*)si->int_sp);

   ptr += (unsigned long)&_STACK_SIZE;

   ptr -= 0x20;      //  The space for up to 8 32 bit local variables.

   si->int_sp = (unsigned int *)((void*)ptr);

   si->task_sp = (unsigned char *)0; 
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

