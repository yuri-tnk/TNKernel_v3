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

static int do_task_suspend(TN_TCB * task);
static int do_task_resume(TN_TCB * task);

//-----------------------------------------------------------------------------
int tn_task_create(TN_TCB * task,                 //-- task TCB
                 void (*task_func)(void *param),  //-- task function
                 unsigned int priority,           //-- task priority
                 unsigned int * task_stack_start, //-- task stack first addr in memory (bottom)
                 unsigned int task_stack_size,    //-- task stack size (in sizeof(int),not bytes)
                 void * param,                    //-- task function parameter
                 unsigned int option)             //-- Creation option
{
   unsigned long tn_save_status_reg = 0U;
   int rc = TERR_NO_ERR;
   unsigned int * ptr_stack;
   unsigned int i;

   //-- Light weight checking of system tasks recreation

   if((priority == 0U && ((option & TN_TASK_OS_TICK) == 0U)) ||
       (priority == TN_NUM_PRIORITY-1U && (option & TN_TASK_IDLE) == 0U))
   {
      rc = TERR_WPARAM;
   }
   else if(priority > TN_NUM_PRIORITY - 1U ||
            task_stack_size < TN_MIN_STACK_SIZE ||
               task_func == NULL || task == NULL || task_stack_start == NULL ||
                   task->id_task != 0UL ||  //-- recreation
                   ( (option & (TN_TASK_OS_TICK | 
                                  TN_TASK_IDLE |
                                     TN_TASK_START_ON_CREATION | 
                                        TN_TASK_SUSPEND_ON_CREATION)) == 0U))
   {
      rc = TERR_WPARAM;
   }
   else if(tn_inside_int() == TRUE)
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      if(tn_system_state == TN_ST_STATE_RUNNING)
      {
         tn_disable_interrupt();
      }

      //--- Init task TCB

      task->task_func_addr   = task_func;
      task->task_func_param  = param;
      task->stk_start        = (unsigned int*)task_stack_start;  //-- Base address of task stack space
      task->stk_size         = task_stack_size;                  //-- Task stack size (in bytes)
      task->base_priority    = priority;                         //-- Task base priority
      task->priority         = priority;   
      task->restore_priority = priority;   
      task->id_task          = TN_ID_TASK & TN_ID_MASK; // Static creation

      //-- Fill all task stack space by TN_FILL_STACK_VAL - only inside create_task
      ptr_stack = task->stk_start;
      for(i = 0;i < task->stk_size; i++)
      {
         *ptr_stack-- = TN_FILL_STACK_VAL;
      }

#ifdef USE_MUTEXES               // v 3.0

      queue_reset(&task->mutex_queue);
      task->num_hold_pi_mutexes = 0;  
      task->num_pi_active_op    = 0;  

#endif

      queue_reset(&task->create_queue);
      queue_reset(&(task->timer_queue));


      task_set_dormant_state(task);

      //--- Init task stack

      ptr_stack = tn_stack_init(task->task_func_addr,
                                task->stk_start,
                                task->task_func_param);

      task->task_stk = ptr_stack;    //-- Pointer to task top of stack,
                                     //-- when not running

      //-- Add task to created task queue

      queue_add_tail(&tn_create_queue,&(task->create_queue));
      tn_created_tasks_qty++;

      if((option & (TN_TASK_OS_TICK | TN_TASK_IDLE |
                              TN_TASK_START_ON_CREATION )) != 0U)
      {
         task_to_runnable(task);
      }
      else  // TN_TASK_SUSPEND_ON_CREATION
      {
         task->task_state |= TSK_STATE_SUSPEND;
      }

      if(tn_system_state == TN_ST_STATE_RUNNING)
      {
         tn_enable_interrupt();
      }
   }
   return rc;
}

//-----------------------------------------------------------------------------

#if defined  USE_DYN_OBJ

//-----------------------------------------------------------------------------
TN_TCB * tn_task_create_dyn(void (*task_func)(void *param),  //-- task function
                            unsigned int priority,     //-- task priority
                            unsigned int task_stack_size,       //-- task stack size (in sizeof(int), not bytes)
                            void * param,              //-- task function parameter
                            unsigned int option,       //-- Creation option
                            int * err)                 //-- [OUT] - errorcode
{
   int rc;
   TN_TCB * task;
   unsigned long stack_alloc_size;
   unsigned int * stack_start;

#if TN_CHECK_PARAM
   if(task_stack_size < TN_MIN_STACK_SIZE)
   {
      task = NULL;
      if(err != NULL)
      { 
         *err = TERR_WPARAM;
      }
   }  
   else
#endif
   {
      if(tn_inside_int() == TRUE)
      {
         task = NULL;
         if(err != NULL)
         { 
            *err = TERR_WCONTEXT;
         }
      }
      else
      {
         task = (TN_TCB *)tn_alloc(&tn_objects_mem, sizeof(TN_TCB));
         if(task != NULL)
         {
            stack_alloc_size = (unsigned long)sizeof(unsigned int) * (unsigned long)task_stack_size;

            stack_start = (unsigned int *)tn_alloc(&tn_objects_mem, stack_alloc_size);
            if(stack_start == NULL)
            {
               (void)tn_dealloc(&tn_objects_mem, task);
               task = NULL;
               if(err != NULL)
               { 
                  *err = TERR_OUT_OF_MEM;
               }
            }
            else
            {
               task->id_task = 0UL;
               rc = tn_task_create(task,             //-- task TCB
                                   task_func,        //-- task function
                                   priority,         //-- task priority
                                   &(stack_start     //-- task stack first addr in memory
                                      [task_stack_size - 1U]),
                                   (unsigned int)task_stack_size,       //-- task stack size (in sizeof(int), not bytes)
                                   param,                          //-- task function parameter
                                   option);    //-- Creation option
               if(rc != TERR_NO_ERR)
               {
                  (void)tn_dealloc(&tn_objects_mem, task);
                  (void)tn_dealloc(&tn_objects_mem, stack_start);
                  task = NULL;
                  if(err != NULL)
                  { 
                     *err = rc;
                  }
               } 
            }
         }
      } 

      if(task != NULL)
      {
         task->id_task |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }
   return task;
}

#endif  /* #if defined  USE_DYN_OBJ */

//-----------------------------------------------------------------------------
// delete itself (the same like a leaving the task's function)
//-----------------------------------------------------------------------------
void tn_task_exit(void)  // v 3.0
{
   TN_TCB * task = tn_curr_run_task;

   if(tn_inside_int() == FALSE)         //   Check the call is not in the interrupt context
   {
      if(tn_system_state == TN_ST_STATE_RUNNING)
      {
         (void)tn_disable_interrupt_nsc();  // disable interrupts without saving status reg

#ifdef USE_MUTEXES

   //-- Unlock all mutexes, locked by the task - a potentially unsafe
   //-- operation, case in the general case, 
   //-- some locked resource(s) may be in the inconsistant state.
   //-- The user should carefully check the case.

         do_release_mutex_queue(task);
#endif

      //-- Task to dormant

         task_to_non_runnable(task);  // task->task_state == TSK_STATE_RUNNABLE)

         task_set_dormant_state(task);

         queue_remove_entry(&(task->create_queue));
         tn_created_tasks_qty--;
         if((task->id_task & TN_DYN_CREATED) == TN_DYN_CREATED)
         {
            task->id_task = TN_DYN_CREATED; // to free mem later
         }
         else
         { 
            task->id_task = 0UL;
         }

      //-- Now - change stack to 'exit' stack & put 'task' to new stack
      //-- Delete 'task', task_stack, if created dyn 
      //-- Switch context exit - not need save current task stack - 

         tn_task_exit_asm(task); // delete objects(if any), re-enable int - inside 
      }
   }
}

//-----------------------------------------------------------------------------
//    Not from interrupt, not itself   v 3.0
//-----------------------------------------------------------------------------
int tn_task_delete(TN_TCB * task)
{
   TN_INTSAVE_DATA

   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(task == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((task->id_task & TN_ID_MASK) != TN_ID_TASK)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_inside_int() == TRUE)  // Checking splitted to make MISRA 2012 happy
   {
      rc = TERR_WCONTEXT;
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING && task == tn_curr_run_task)
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      tn_disable_interrupt();

#ifdef USE_MUTEXES

      //-- Unlock all mutexes, locked by the task - a potentially unsafe
      //-- operation, case in the general case, some locked resource(s) 
      //-- may be in the inconsistant state.
      //-- The user should carefully check the case.

      do_release_mutex_queue(task);
#endif

      //-- Task to dormant

      if(task->task_state == TSK_STATE_RUNNABLE)
      {
         task_to_non_runnable(task);
      }
      else 
      {
         if((task->task_state & TSK_STATE_WAIT) == TSK_STATE_WAIT)
         {
            //-- Free all queues, involved in the 'waiting'

            queue_remove_entry(&(task->task_queue));

            if(task->tick_count != TN_WAIT_INFINITE)
            {
               queue_remove_entry(&(task->timer_queue));
            }

#ifdef USE_MUTEXES

            if(task->task_wait_reason == TSK_WAIT_REASON_MUTEX_I)
            {
               do_set_mutex_holder_max_priority(task);
            }
#endif
         }
      }

      task_set_dormant_state(task);

      queue_remove_entry(&(task->create_queue));
      tn_created_tasks_qty--;

      if((task->id_task & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
      {
        // Prevent from the usage but OK for the resources release
         task->id_task = TN_DYN_CREATED; 
      }
      else
      {
         task->id_task = 0UL; 
      }

      tn_enable_interrupt();

      delete_task_resources(task); // chek DYN_OBJ - inside
   }

   return rc;
}


//----------------------------------------------------------------------------
//  If the task is runnable, it is moved to the SUSPENDED state. If the task
//  is in the WAITING state, it is moved to the WAITING­SUSPENDED state.
//----------------------------------------------------------------------------
int tn_task_suspend(TN_TCB * task)
{
   int rc;

#if TN_CHECK_PARAM
   if(task == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((task->id_task & TN_ID_MASK) != TN_ID_TASK)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT;    
   }
   else
   {
      if(tn_inside_int() == TRUE) // Checking splitted to make MISRA 2012 happy
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         rc = do_task_suspend(task);

         tn_ienable_interrupt();

         if(rc == TERR_YIELD)
         {
            rc = TERR_NO_ERR;
         }
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_task_suspend(task);

         tn_enable_interrupt();

         if(rc == TERR_YIELD)
         {
            tn_switch_context();

            rc = TERR_NO_ERR;
         }
      }
   }


   return rc;
}

//----------------------------------------------------------------------------
static int do_task_suspend(TN_TCB * task)
{
   int rc = TERR_NO_ERR;

   if((task->task_state & TSK_STATE_SUSPEND) == TSK_STATE_SUSPEND)
   {
      rc = TERR_OVERFLOW;
   }
   else
   {
      if(task->task_state == TSK_STATE_DORMANT)
      {
         rc = TERR_WSTATE;
      }
      else
      {
         if(task->task_state == TSK_STATE_RUNNABLE)
         {
            task->task_state = TSK_STATE_SUSPEND;
            task_to_non_runnable(task);

            rc = TERR_YIELD;
         }
         else
         {
            task->task_state |= TSK_STATE_SUSPEND;
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
int tn_task_resume(TN_TCB * task)
{
   int rc;

#if TN_CHECK_PARAM
   if(task == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((task->id_task & TN_ID_MASK) != TN_ID_TASK)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         rc = do_task_resume(task);

         tn_ienable_interrupt();

         if(rc == TERR_YIELD)
         {
            rc = TERR_NO_ERR;
         }
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_task_resume(task);

         tn_enable_interrupt();

         if(rc == TERR_YIELD)
         {
            tn_switch_context();

            rc = TERR_NO_ERR;
         }
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
static int do_task_resume(TN_TCB * task)
{
   int rc = TERR_NO_ERR;
   if((task->task_state & TSK_STATE_SUSPEND) == 0U)
   {
      rc = TERR_WSTATE;
   }
   else
   {
      if((task->task_state & TSK_STATE_WAIT) == 0U) //- The task is not in the WAIT-SUSPEND state
      {
         task_to_runnable(task);
         rc = TERR_YIELD;
      }
      else  //-- Just remove TSK_STATE_SUSPEND from the task state
      {
         task->task_state &= ~TSK_STATE_SUSPEND;
      }
   }

   return rc; 
}

//----------------------------------------------------------------------------
int tn_task_sleep(unsigned long timeout)
{
   TN_INTSAVE_DATA
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM

   if(timeout == 0UL)
   {
     rc = TERR_WPARAM;
   }
   else
#endif
   if(tn_inside_int() == TRUE)  // Checking splitted to make MISRA 2012 happy
   {
      rc = TERR_WCONTEXT;   
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT;   
   }
   else
   {
      tn_disable_interrupt();
      task_curr_to_wait_action(NULL, TSK_WAIT_REASON_SLEEP, timeout);
      tn_enable_interrupt();

      tn_switch_context();
   }
   return rc;
}

//----------------------------------------------------------------------------
int tn_task_activate(TN_TCB * task)
{
   int rc = TERR_NO_ERR;
   int fYield = FALSE;

#if TN_CHECK_PARAM
   if(task == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((task->id_task & TN_ID_MASK) != TN_ID_TASK)
   {
      rc = TERR_NOEXS;
   }
   else if( tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT;   
   }
   else
   {
      if(tn_inside_int() == TRUE)  
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         if(task->task_state == TSK_STATE_DORMANT)
         {
            task_to_runnable(task);
         }
         else
         {
            rc = TERR_WCONTEXT;
         }

         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         if(task->task_state == TSK_STATE_DORMANT)
         {
            task_to_runnable(task);
            fYield = TRUE;
         }
         else
         {
            rc = TERR_WCONTEXT;
         }

         tn_enable_interrupt();

         if(fYield == TRUE)
         {
            tn_switch_context();
         }
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_task_change_priority(TN_TCB * task, unsigned int new_priority)
{
   TN_INTSAVE_DATA
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(task == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((task->id_task  & TN_ID_MASK) != TN_ID_TASK)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_inside_int() == TRUE) 
   {
      rc = TERR_WCONTEXT;   
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT;   
   }
   else if(new_priority == 0U || new_priority > TN_NUM_PRIORITY - 2U) //-- try to set pri
   {                                                             // reserved by sys
      rc = TERR_WPARAM;                                
   }
   else
   {
      tn_disable_interrupt();

      if((new_priority < task->restore_priority) 
#if USE_MUTEXES
         ||  (task->num_hold_pi_mutexes == task->num_pi_active_op)
#endif
      )
      {                       
         task->restore_priority = new_priority;
      }

      task->base_priority = new_priority;

      tn_update_priority(task, new_priority);  // Update 'task->priority'

      tn_enable_interrupt();
   }

   return rc;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
