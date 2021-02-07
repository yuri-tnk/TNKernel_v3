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

#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"



//  The System uses internally lowest priority level
//
//   - level (TN_NUM_PRIORITY-1)  (lowest)  for system idle task

// The task for the system OS ticks and (up to user) another goals MUST to be
// created in the user code. Usually, this task has a priority level 0 (highest possible)


    //---- System's  global variables ----

CDLL_QUEUE tn_ready_list[TN_NUM_PRIORITY];        //-- all ready to run(RUNNABLE) tasks
CDLL_QUEUE tn_create_queue;                       //-- all created tasks
volatile int tn_created_tasks_qty;                //-- num of created tasks

unsigned short tn_tslice_ticks[TN_NUM_PRIORITY];  //-- for round-robin scheduler only

volatile int tn_system_state;                     //-- The System state -(running/not running/etc.)
volatile int tn_inside_int_cnt = 0;               //-- is the System inside interrupt

TN_TCB * tn_next_task_to_run;                     //-- Task to be run after switch context
TN_TCB * tn_curr_run_task;                        //-- Task that is running now

volatile unsigned long tn_sys_tick_count;

volatile unsigned int tn_ready_to_run_bmp;
volatile unsigned long tn_idle_count;
//volatile unsigned long tn_curr_performance;

//-- Interrupt stack space

INTSTACKINFO tn_sp_info;

//-- The hook function for the Idle task

idle_task_hook_func  tn_idle_task_hook_func = NULL;

//-- The stack that is used in the tn_task_exit() function

#ifndef TN_EXIT_STACK_SIZE
#define TN_EXIT_STACK_SIZE 128
#endif

#if defined (__ICCARM__)  // IAR ARM
#pragma data_alignment=8
#endif
unsigned int tn_func_exit_stack[TN_EXIT_STACK_SIZE] TN_ALIGN_ATTR_END;
unsigned int * tn_func_exit_stack_start = &tn_func_exit_stack[TN_EXIT_STACK_SIZE - 2];

 //-- System tasks

    //-- idle task - priority (TN_NUM_PRIORITY-1) - lowest

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned int tn_idle_task_stack[TN_IDLE_STACK_SIZE] TN_ALIGN_ATTR_END;
TN_ALIGN_ATTR TN_TCB tn_idle_task;

void tn_idle_task_func(void * par);

TN_SEM tn_sys_tick_sem;

#if 0
extern TN_DQUEUE queueOsTisk;
#endif

//------ System timers wheel  

TN_ALIGN_ATTR  CDLL_QUEUE tn_os_timer_list_gen;
TN_ALIGN_ATTR  CDLL_QUEUE tn_os_timer_list_tick[TN_OS_TICK_LISTS_CNT];

//------ User timers wheel  (if any)

#if defined USER_TIMERS

TN_ALIGN_ATTR  CDLL_QUEUE tn_user_timer_list_gen;
TN_ALIGN_ATTR  CDLL_QUEUE tn_user_timer_list_tick[TN_USER_TICK_LISTS_CNT];

#endif


#if defined USE_DYN_OBJ

MEMINFO tn_objects_mem;

#endif

//----------------------------------------------------------------------------
// TN main function (never returns)
//----------------------------------------------------------------------------
int tn_start_system(unsigned char * sys_obj_mem,
                    unsigned long sys_obj_mem_size,
                    unsigned int * int_stack_mem)
{
   int i;
   int rc;   
#if defined USE_DYN_OBJ
   rc = tn_alloc_init(&tn_objects_mem, 
                      sys_obj_mem, 
                      sys_obj_mem_size);
   if(rc == TERR_NO_ERR)
#endif
   {
   //------ System timers wheel   

      queue_reset(&tn_os_timer_list_gen);
      for(i=0;i < (int)TN_OS_TICK_LISTS_CNT;i++)
      {
         queue_reset(&(tn_os_timer_list_tick[i]));
      }
   //-----------------------------------------------

   //------ User timers wheel   

#if defined USER_TIMERS

      queue_reset(&tn_user_timer_list_gen);
      for(i=0;i < (int)TN_USER_TICK_LISTS_CNT;i++)
      {
         queue_reset(&(tn_user_timer_list_tick[i]));
      }

#endif
   //-----------------------------------------------
      for(i=0;i < (int)TN_NUM_PRIORITY;i++)
      {
         queue_reset(&(tn_ready_list[i]));
         tn_tslice_ticks[i] = NO_TIME_SLICE;
      }

      queue_reset(&tn_create_queue);
      tn_created_tasks_qty = 0;

      tn_system_state = TN_ST_STATE_NOT_RUN;

      tn_ready_to_run_bmp = 0;

      tn_next_task_to_run = NULL;
      tn_curr_run_task    = NULL;

      tn_sys_tick_count   = 0UL;

      tn_idle_count       = 0UL;

      //tn_curr_performance = 0;

     //-- System task, sem etc.

      tn_sys_tick_sem.id_sem = 0UL;
      rc = tn_sem_create(&tn_sys_tick_sem,
                         0,      //-- Start value
                         1);     //-- Max value

      rc = TERR_NO_ERR;
      if(rc == TERR_NO_ERR)
      {
         //--- Idle task

         tn_idle_task.id_task = 0;
         rc = tn_task_create((TN_TCB*)&tn_idle_task,         //-- task TCB
                              tn_idle_task_func,              //-- task function
                              (int)TN_NUM_PRIORITY-1,              //-- task priority
                              &(tn_idle_task_stack            //-- task stack first addr in memory
                                   [TN_IDLE_STACK_SIZE-1]),
                              TN_IDLE_STACK_SIZE,             //-- task stack size (in int,not bytes)
                              NULL,                           //-- task function parameter
                              TN_TASK_IDLE);                  //-- Creation option
         if(rc == TERR_NO_ERR)
         {
            tn_int_stack_init(&tn_sp_info, int_stack_mem);      //-- Init interrupt stack

            tn_curr_run_task     = &tn_idle_task;  
            tn_next_task_to_run  = &tn_idle_task;  

            //-- user's objects initial (tasks etc.) creation
            //-- A OS ticks handler task have to be created here

            rc = tn_app_init();
            if(rc == TERR_NO_ERR)
            {

               //-- Enable peripheral int, the global interrupts will be 
               //-- enabled inside tn_start_exe()

               tn_cpu_int_enable();

               //-- Run OS - first context switch.
               //-- The global interrupts will be enabled inside.
               
               tn_start_exe();   //-- Never returns
            }
         }
      }
   }

   return rc;  // If we are here - something goes wrong
}


//----------------------------------------------------------------------------
//  In fact, this task is always in the RUNNABLE state
//----------------------------------------------------------------------------
void tn_idle_task_func(void * par)
{
  // tn_idle_count = 0;   // if you use Idle task reset option, you can check it here

#ifdef TN_MEAS_PERFORMANCE
   TN_INTSAVE_DATA
#endif

   for(;;)
   {
#ifdef TN_MEAS_PERFORMANCE
      tn_disable_interrupt();
      tn_idle_count++;
      tn_enable_interrupt();
#endif
      if(tn_idle_task_hook_func != NULL)
      {
         tn_idle_task_hook_func(par);
      } 
   }
}

//----------------------------------------------------------------------------
// Round-robin scheduling:  
//    set time slice ticks value for 'priority'
//
//   If value is NO_TIME_SLICE, there are no round-robin scheduling for tasks 
// with 'priority'. NO_TIME_SLICE is default value.
//----------------------------------------------------------------------------
int tn_sys_tslice_ticks(unsigned int priority, 
                        unsigned int value)
{
   TN_INTSAVE_DATA
   int rc = TERR_WCONTEXT;

   if(tn_system_state == TN_ST_STATE_RUNNING)
   {
      if(tn_inside_int() == FALSE) 
      {

         if(priority == 0U || priority >= TN_NUM_PRIORITY - 1U ||
             value > MAX_TIME_SLICE)
         {
            rc = TERR_WPARAM;
         }
         else
         {  
            tn_disable_interrupt();

            tn_tslice_ticks[priority] = (unsigned short)value;
            rc = TERR_NO_ERR;

            tn_enable_interrupt();
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
// Call inside sys tick interrupt
//----------------------------------------------------------------------------
void tn_tick_int_processing(void)
{
   TN_INTSAVE_DATA_INT

   CDLL_QUEUE * curr_que;   //-- Need 'volatile' here only to solve
   CDLL_QUEUE * pri_queue;  //-- IAR(c) compiler's high optimization mode problem
   unsigned int priority;
#if 0
   unsigned long data;
#endif

   if(tn_system_state == TN_ST_STATE_RUNNING) 
   {
      if(tn_inside_int() == TRUE)
      { 
         tn_idisable_interrupt();

   // First, update sys_tick_count

         tn_sys_tick_count++;

      //-------  Round -robin (if it is used)

         priority = tn_curr_run_task->priority;

         if(tn_tslice_ticks[priority] != NO_TIME_SLICE)
         {
            tn_curr_run_task->tslice_count++;
            if(tn_curr_run_task->tslice_count >= (int)tn_tslice_ticks[priority])
            {
               tn_curr_run_task->tslice_count = 0;

               pri_queue = &tn_ready_list[priority];

              //-- If a ready queue is not empty and qty of queue's tasks > 1

               if(is_queue_empty(pri_queue) == FALSE && pri_queue->next->next != pri_queue)
               {
                  // v.2.7  - Thanks to Vyacheslav Ovsiyenko

                 //-- Remove task from head and add it to the tail of
                 //-- ready queue for current priority

                  curr_que = queue_remove_head(&(tn_ready_list[priority]));
                  queue_add_tail(&(tn_ready_list[priority]), curr_que);
               }
            }
         }

         tn_ienable_interrupt();
      }
   }

#if 0
   data = EVT_OS_TICK;
   (void)tn_dqueue_send(&queueOsTisk, (TN_DQUEUE_ELEMENT)data, TN_NO_WAIT);
#endif

   (void)tn_sem_signal(&tn_sys_tick_sem);
}

//----------------------------------------------------------------------------
unsigned long tn_get_sys_ticks(void)
{
   TN_INTSAVE_DATA
   unsigned long rc;

   
tn_disable_interrupt();
   rc = tn_sys_tick_count;
   tn_enable_interrupt();
   
   return rc;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


