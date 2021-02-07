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

#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"

//-- OS Timers

//int tn_os_timers_tick_proc(void);
int tn_os_timer_start(TN_TCB * task, unsigned long timeout);
void tn_os_timer_cancel(TN_TCB * task);

//----------------------------------------------------------------------------
static TN_INLINE unsigned long  os_tick_list_index(unsigned long timeout)
{
   return (((unsigned long)tn_sys_tick_count + timeout) & (unsigned long)TN_OS_TICK_LISTS_MASK);
}


//----------------------------------------------------------------------------
void tn_update_ready_queue(TN_TCB * task, unsigned int old_priority)
{
   CDLL_QUEUE * que;
   unsigned int tmp;

   //--- Remove task from ready queue for the old priority

   queue_remove_entry(&(task->task_queue));
   que = &tn_ready_list[old_priority];
   if(is_queue_empty(que) == TRUE) // No more ready tasks with the 'old_priority'
   {
      tmp = (unsigned int)(((unsigned int)1) << old_priority);
      tn_ready_to_run_bmp &= ~tmp;
   }

   //--- Add task to the ready queue for the new priority

   que = &tn_ready_list[task->priority];
   if(que != NULL)
   {
      queue_add_tail(que, &(task->task_queue));

      tmp = ((unsigned int)1) << task->priority;
      tn_ready_to_run_bmp |= tmp;
   }

   find_next_task_to_run();

} 

//----------------------------------------------------------------------------
void tn_update_wait_queue(TN_TCB * task)
{
   //-- Task is wait in the queue, stored in task->pwait_queue

   queue_remove_entry(&(task->task_queue));

   queue_insert_by_priority(task->pwait_queue,   // queue, 
                            &(task->task_queue), // entry_to_insert,
                            task->priority);     // new_priority
}

//----------------------------------------------------------------------------
void tn_update_priority(TN_TCB * task, unsigned int new_priority)
{
   int fExit = FALSE;
   TN_MUTEX * mutex;
   unsigned int old_priority;

#ifdef USE_MUTEXES
   if(task->num_pi_active_op == 0 && task->priority < new_priority)
#else
   if(task->priority < new_priority)
#endif
   {
     // We are here when  - there is no pi related op and we want to reduce task priority
   
      if(task->base_priority != task->restore_priority)
      {
         new_priority = task->restore_priority;
      }
      old_priority = task->priority;
      task->priority = new_priority;

      if(task->task_state == TSK_STATE_RUNNABLE) 
      {
         tn_update_ready_queue(task, old_priority);
      }
      else 
      {
         if((task->task_state & TSK_STATE_WAIT) == TSK_STATE_WAIT)
         {
            tn_update_wait_queue(task);
         }
      }
   }
   else // rise current priority
   {
      do
      {
         if(task == NULL)
         {
            fExit = TRUE;
         }  
         else
         {  
            if(task->priority <= new_priority) 
            {
               fExit = TRUE;
            }
            else
            {
               old_priority = task->priority;
               task->priority = new_priority;                // raise current priority
       
               if(task->task_state == TSK_STATE_RUNNABLE)    
               {
                  tn_update_ready_queue(task, old_priority);

                  fExit = TRUE;
               }
               else 
               {
                  if((task->task_state & TSK_STATE_WAIT) == TSK_STATE_WAIT)      
                  {
                     tn_update_wait_queue(task);
       
                     if(task->task_wait_reason == TSK_WAIT_REASON_MUTEX_I)
                     {
                         //-- The task waits a mutex, therefore the mutex should have a holder

                        mutex = get_mutex_by_wait_queque(task->pwait_queue);
                        if(mutex != NULL)
                        {
                           task  = mutex->holder;          // task should not be NULL here
                           if(task == NULL)
                           {
                              fExit = TRUE;
                           }
                        }
                        else
                        {
                           fExit = TRUE;
                        }
                     }
                     else
                     {
                        fExit = TRUE;
                     }
                  }
               }
            }
         }
      }while(fExit == FALSE);   
   }
}


//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void find_next_task_to_run(void)
{
   int tmp;

#ifndef USE_ASM_FFS
   int i;
   unsigned int mask;
#endif

#ifdef USE_ASM_FFS
   tmp = ffs_asm(tn_ready_to_run_bmp);
   tmp--;
#else
   mask = 1;
   tmp = 0;
   for(i=0; i < (int)TN_BITS_IN_INT; i++)  //-- for each bit in bmp
   {
      if((tn_ready_to_run_bmp & mask) != 0U)
      {
         tmp = i;
         break;
      }
      mask = mask<<1;
   }
#endif

   tn_next_task_to_run = get_task_by_tsk_queue(tn_ready_list[tmp].next);
}

//-----------------------------------------------------------------------------
void task_to_non_runnable(TN_TCB * task)   // v 3.0
{
   unsigned int priority;
   unsigned int tmp;   // to make MISRA 2012 happy
   CDLL_QUEUE * que;

   priority = task->priority;
   que = &(tn_ready_list[priority]);

   task->task_state  &= ~TSK_STATE_RUNNABLE;
   //-- remove the curr task from any queue (now - from ready queue)

   queue_remove_entry(&(task->task_queue));

   if(is_queue_empty(que) == TRUE)  //-- No ready tasks for the curr priority
   {
      //-- remove 'ready to run' from the curr priority

      tmp = (unsigned int)(((unsigned int)1) << priority);
      tn_ready_to_run_bmp &= ~tmp;

      //-- Find highest priority ready to run -
      //-- at least, MSB bit must be set for the idle task

      find_next_task_to_run();   //-- v.2.6
   }
   else //-- There are 'ready to run' task(s) for the curr priority
   {
      if(tn_next_task_to_run == task)
      { 
         //-- the task that just became non-runnable was the "next task to run",
         //-- so we should select new next task to run
         tn_next_task_to_run = get_task_by_tsk_queue(que->next);
      }
   }
}

//----------------------------------------------------------------------------
void task_to_runnable(TN_TCB * task)  // ver 3.0
{
   unsigned int priority;
   unsigned int tmp; // to make MISRA-2012 happy

   priority          = task->priority;
   task->task_state  = TSK_STATE_RUNNABLE;
   task->pwait_queue = NULL;

   //-- Add the task to the end of 'ready queue' for the current priority

   queue_add_tail(&(tn_ready_list[priority]), &(task->task_queue));
   tmp = ((unsigned int)1) << priority;
   tn_ready_to_run_bmp |= tmp;

   if(tn_next_task_to_run != NULL)
   {
      //-- less value - greater priority, so '<' operation is used here

      if(priority < tn_next_task_to_run->priority)
      {
         tn_next_task_to_run = task;
      }
   }
}

//----------------------------------------------------------------------------
BOOL task_wait_complete(TN_TCB * task) //-- v. 3.0
{
   BOOL rc = FALSE;

   task->pwait_queue  = NULL;
   task->task_wait_rc = TERR_WAIT_COMPLETE;

   tn_os_timer_cancel(task);

   if((task->task_state & TSK_STATE_SUSPEND) == 0U)
   {
      task_to_runnable(task);
      rc = TRUE;
   }
   else  //-- remove WAIT state
   {
      task->task_state = TSK_STATE_SUSPEND;
   }

   task->task_wait_reason = 0; //-- Clear wait reason

   return rc;
}

//----------------------------------------------------------------------------
void task_curr_to_wait_action(CDLL_QUEUE * wait_que,
                              int wait_reason,
                              unsigned long timeout)
{
   task_to_non_runnable(tn_curr_run_task);

   tn_curr_run_task->task_state       = TSK_STATE_WAIT;
   tn_curr_run_task->task_wait_reason = wait_reason;
   tn_curr_run_task->tick_count       = timeout;

   //--- Add to the wait queue

   if(wait_que == NULL) //-- Thanks to Vavilov D.O.
   {
      queue_reset(&(tn_curr_run_task->task_queue));
   }
   else // Add by priority
   {
      queue_insert_by_priority(wait_que, 
                               &(tn_curr_run_task->task_queue),
                               tn_curr_run_task->priority);

      tn_curr_run_task->pwait_queue = wait_que;
   }

   //--- Add to the timers queue

   if(timeout != TN_WAIT_INFINITE && timeout != TN_NO_WAIT)
   {
      (void)tn_os_timer_start(tn_curr_run_task, timeout);

      //queue_add_tail(&tn_wait_timeout_list, &(tn_curr_run_task->timer_queue));
   }
}

//----------------------------------------------------------------------------
void task_set_dormant_state(TN_TCB * task)  // v 3.0
{
             // v.2.7 - thanks to Alexander Gacov, Vyacheslav Ovsiyenko
   queue_reset(&(task->task_queue));
   tn_os_timer_cancel(task);

#ifdef USE_MUTEXES

   //-- Unlock all mutexes, locked by the task - a potentially unsafe
   //-- operation, case in the general case, 
   //-- some locked resource(s) may be in an inconsistent state.
   //-- The user should carefully check the case.

    do_release_mutex_queue(task);

    task->num_pi_active_op      = 0;
    task->num_hold_pi_mutexes = 0;
    
#endif

   task->pwait_queue = NULL;

   task->priority    = task->restore_priority; //-- Task curr priority
   task->task_state  = TSK_STATE_DORMANT;      //-- Task state
   task->task_wait_reason = 0;                 //-- Reason for waiting
   task->task_wait_rc = TERR_NO_ERR;

#ifdef USE_EVENTS

   task->ewait_pattern = 0;                 //-- Event wait pattern
   task->ewait_mode    = 0;                 //-- Event wait mode:  _AND or _OR

#endif

   task->data_elem     = NULL;              //-- Store data queue entry,if data queue is full

   task->tick_count    = TN_WAIT_INFINITE;  //-- Remaining time until timeout
   //task->wakeup_count  = 0;                 //-- Wakeup request count
   //task->suspend_count = 0;                 //-- Suspension count

   task->tslice_count  = 0;
}

//----------------------------------------------------------------------------
void delete_task_resources(TN_TCB * task)
{

#if defined  USE_DYN_OBJ

   unsigned char * stack_ptr;
   unsigned long off;
   //TN_TCB * task = tn_curr_run_task;


   if((task->id_task & TN_DYN_CREATED) == TN_DYN_CREATED)
   {
      task->id_task = 0UL;
      off = ((unsigned long)task->stk_size - 1UL) * sizeof(unsigned int);
      stack_ptr = (unsigned char *)((void*)task->stk_start) - off;
      (void)tn_dealloc(&tn_objects_mem, stack_ptr);
      (void)tn_dealloc(&tn_objects_mem, task);
   }
#endif
}

//----------------------------------------------------------------------------
//  OS timers - uses by the system for the task sleep(), timeout services etc
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
BOOL tn_os_timers_tick_proc(void)
{
   //-- tn_sys_tick_count is incremented inside interrupt

   TN_INTSAVE_DATA

   CDLL_QUEUE * que;
   CDLL_QUEUE * que_next;
   CDLL_QUEUE * p_cur_timer_list;
   TN_TCB * task;
   int tick_list_index;
   int rc = FALSE;

   tick_list_index = (int)os_tick_list_index(0);

   tn_disable_interrupt();

   if(tick_list_index == 0)
   {
      //-- it happens each TN_TICK_LISTS_CNT-th system tick:
      //   now we should walk through all the timers in the "generic" timer
      //   list, decrement the timeout of each one by TN_TICK_LISTS_CNT,
      //   and if resulting timeout is less than TN_TICK_LISTS_CNT,
      //   then move the timer to the appropriate "tick" timer list.

      //-- handle "generic" timer list 

      for(que = tn_os_timer_list_gen.next; 
             que != &tn_os_timer_list_gen; que = que_next)
      {
         que_next = que->next;

         task = get_task_by_timer_queue(que);
         if(task != NULL)
         { 
            task->tick_count -= (unsigned long)TN_OS_TICK_LISTS_CNT;

            if(task->tick_count < (unsigned long)TN_OS_TICK_LISTS_CNT)
            {
               //-- it's the time to move this timer to the "tick" list
               queue_remove_entry(&(task->timer_queue));
               queue_add_tail(&tn_os_timer_list_tick[os_tick_list_index(task->tick_count)],
                              &(task->timer_queue));
            }
         }
      }
   }

   //-- it happens every system tick:
   //   we should walk through all the timers in the current "tick" timer list,
   //   and fire them all, unconditionally.

   //-- handle current "tick" timer list

   p_cur_timer_list =  &tn_os_timer_list_tick[tick_list_index];

      //-- now, p_cur_timer_list is a list of timers that we should
      //   fire NOW, unconditionally.

   while(is_queue_empty(p_cur_timer_list) == FALSE)
   {
      que = p_cur_timer_list->next;
      task = get_task_by_timer_queue(que);
      if(task != NULL)
      {
         tn_os_timer_cancel(task);

         //--- Exclude Task from wait

         queue_remove_entry(&(task->task_queue));
         (void)task_wait_complete(task);
         task->task_wait_rc = TERR_TIMEOUT;
         rc = TRUE;  // to switch context after the func exit
      }
   }

   tn_enable_interrupt();

   return rc;
}

//----------------------------------------------------------------------------
// Interrupts have to be disabled here
//----------------------------------------------------------------------------
int tn_os_timer_start(TN_TCB * task, unsigned long timeout)
{
   int rc = TERR_NO_ERR;

   if(timeout == TN_WAIT_INFINITE || timeout == 0UL)
   {
      rc = TERR_WPARAM;
   } 
   else  // suppose that timer is not active here
   {
      if(timeout < (unsigned long)TN_OS_TICK_LISTS_CNT)
      {
            //-- timer should be added to the one of "tick" lists.
         unsigned long tick_list_index = os_tick_list_index(timeout);
         task->tick_count = (unsigned long)tick_list_index;

         queue_add_tail(&tn_os_timer_list_tick[tick_list_index],
                        &(task->timer_queue));
      } 
      else 
      {
            //-- timer should be added to the "generic" list.
            //   We should set timeout_cur adding current "tick" index to it.
         task->tick_count = timeout + os_tick_list_index(0);

         queue_add_tail(&tn_os_timer_list_gen, &(task->timer_queue));
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
// Interrupts have to be disabled here
//----------------------------------------------------------------------------
void tn_os_timer_cancel(TN_TCB * task)
{
   if(is_queue_empty(&(task->timer_queue)) == FALSE)
   {
      task->tick_count = TN_WAIT_INFINITE;
      queue_remove_entry(&(task->timer_queue));
      queue_reset(&(task->timer_queue));
   }
}

//----------------------------------------------------------------------------
int tn_release_wait_que(CDLL_QUEUE * wait_que)
{
   CDLL_QUEUE * que;
   TN_TCB * task;
   int rc = FALSE;

   while(is_queue_empty(wait_que) == FALSE)
   {
      que  = queue_remove_head(wait_que);
      task = get_task_by_tsk_queue(que);
      if(task != NULL)
      {
         if(task_wait_complete(task) == TRUE)
         {
            task->task_wait_rc = TERR_DLT;
            rc = TRUE; //fYield = TRUE;
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------




