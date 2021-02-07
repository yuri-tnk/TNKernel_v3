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


//----------------------------------------------------------------------------
//   Structure's field sem->id_sem have to be set to 0
//----------------------------------------------------------------------------
int tn_sem_create(TN_SEM * sem,
                  unsigned int start_value,
                  unsigned int max_val)
{
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(sem == NULL || max_val == 0U || 
             start_value > max_val || sem->id_sem != 0UL) //-- no recreation
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if(tn_inside_int() == TRUE)
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      queue_reset(&(sem->wait_queue));

      sem->count     = start_value;
      sem->max_count = max_val;
      sem->id_sem    = TN_ID_SEMAPHORE & TN_ID_MASK;
   }

   return rc;
}

//----------------------------------------------------------------------------
#if defined  USE_DYN_OBJ

//----------------------------------------------------------------------------
TN_SEM * tn_sem_create_dyn(unsigned int start_value,
                           unsigned int max_val,
                           int * err)  // [OUT] 
{
   int rc;
   TN_SEM * sem;

   if(tn_inside_int() == TRUE)  //  Check the call context
   {
      sem = NULL;
      if(err != NULL)
      {
         *err = TERR_WCONTEXT;
      }
   }
   else // Check input parameters inside a static creation func - optimistic approach
   {
      sem = (TN_SEM *)tn_alloc(&tn_objects_mem, sizeof(TN_SEM));
      if(sem != NULL)
      {
         sem->id_sem = 0UL;
         rc = tn_sem_create(sem,
                            start_value,
                            max_val);
         if(rc != TERR_NO_ERR)
         {
            (void)tn_dealloc(&tn_objects_mem, sem);
            sem = NULL;
            if(err != NULL)
            { 
               *err = rc;
            }
         }
      }
      else
      {
         if(err != NULL)
         { 
            *err = TERR_OUT_OF_MEM;
         }
      }

      if(sem != NULL)
      {
         sem->id_sem |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }

   return sem;
}

#endif  /* #if defined  USE_DYN_OBJ  */

//----------------------------------------------------------------------------
int tn_sem_delete(TN_SEM * sem)
{
   TN_INTSAVE_DATA

   int rc = TERR_NO_ERR;
   int fYield; // = FALSE;
#if defined  USE_DYN_OBJ
   unsigned long id;
#endif

#if TN_CHECK_PARAM
   if(sem == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((sem->id_sem & TN_ID_MASK) != TN_ID_SEMAPHORE)
   {
      rc = TERR_NOEXS;
   }
   else
   {
      if(tn_inside_int() == TRUE)
      {
         rc = TERR_WCONTEXT;
      }
      else
      {
         tn_disable_interrupt();

         fYield = tn_release_wait_que(&(sem->wait_queue));
#if defined  USE_DYN_OBJ
         id = sem->id_sem;
#endif

         sem->id_sem = 0; // Semaphore set as 'not exists'

         tn_enable_interrupt();

#if defined  USE_DYN_OBJ
         if((id & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
         {
            (void)tn_dealloc(&tn_objects_mem, sem);
         }
#endif
         if(fYield == TRUE)
         {
            tn_switch_context();
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
//  Release Semaphore Resource
//----------------------------------------------------------------------------
int tn_sem_signal(TN_SEM * sem)
{
   int rc;// = TERR_NO_ERR; //-- return code

#if TN_CHECK_PARAM
   if(sem == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((sem->id_sem & TN_ID_MASK) != TN_ID_SEMAPHORE)
   {
      rc = TERR_NOEXS;
   }
   else
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         rc = do_sem_signal(sem);

         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA 

         tn_disable_interrupt();

         rc = do_sem_signal(sem);

         tn_enable_interrupt();

         if(rc == TERR_YIELD)
         {
            tn_switch_context();
#if defined TN_TEST
            rc = TERR_NO_ERR_W;
#else
            rc = TERR_NO_ERR;
#endif
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
//   Acquire Semaphore Resource
//----------------------------------------------------------------------------
int tn_sem_acquire(TN_SEM * sem, unsigned long timeout)
{
   int rc; //-- return code

#if TN_CHECK_PARAM
   if(sem == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((sem->id_sem & TN_ID_MASK)!= TN_ID_SEMAPHORE)
   {
      rc = TERR_NOEXS;
   }
   else
   {
      if(tn_inside_int() == TRUE) // Inside interrupt
      {
         if(timeout != TN_NO_WAIT)
         {
            rc = TERR_WCONTEXT; 
         }
         else
         {  
            TN_INTSAVE_DATA_INT

            tn_idisable_interrupt();

            rc = do_sem_acquire(sem,
                                TSK_WAIT_REASON_SEM, 
                                timeout); 

            tn_ienable_interrupt();
         }
      }
      else  // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_sem_acquire(sem,
                             TSK_WAIT_REASON_SEM, 
                             timeout); 

         tn_enable_interrupt();

         if(rc == TERR_YIELD)
         {
            tn_switch_context();

            rc = tn_curr_run_task->task_wait_rc;
            if(rc == TERR_WAIT_COMPLETE)
            {
#if defined TN_TEST
               rc = TERR_NO_ERR_W;
#else
               rc = TERR_NO_ERR;
#endif
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
// Routines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Shared with TN_DQUEUE and TN_MAILBOX
//----------------------------------------------------------------------------
int do_sem_signal(TN_SEM * sem)
{
   CDLL_QUEUE * que;
   TN_TCB * task;
   int rc = TERR_NO_ERR;

   if(is_queue_empty(&(sem->wait_queue)) == FALSE)
   {
      //--- delete from the sem wait queue

      que = queue_remove_head(&(sem->wait_queue));
      task = get_task_by_tsk_queue(que);
      if(task != NULL)
      {
         if(task_wait_complete(task) == TRUE)
         {
            rc = TERR_YIELD;
         }
         else
         {
            rc = TERR_NOEXS;
         }
      }
   }
   else
   {
      if(sem->count < sem->max_count)
      {
         sem->count++;
      }
      else
      {
         rc = TERR_OVERFLOW;
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
/*
static int do_acquire(TN_SEM * sem)
{
   int rc = TERR_NO_ERR;
   if(sem->count >= 1U)
   {
      sem->count--;
   }
   else
   {
      rc = TERR_UNDERFLOW;
   }

   return rc; 
}
*/

//----------------------------------------------------------------------------
// Shared with TN_DQUEUE and TN_MAILBOX
//----------------------------------------------------------------------------
int do_sem_acquire(TN_SEM * sem,
                   int  wait_reason,      // if any  
                   unsigned long timeout)
{
   int rc = TERR_NO_ERR;

   if(sem->count >= 1U)
   {
      sem->count--;
   }
   else
   {
      rc = TERR_UNDERFLOW;
   }

   if(timeout != TN_NO_WAIT) // Not polling
   {
      if(rc != TERR_NO_ERR)  //-- No free entries in the data queue
      {
         task_curr_to_wait_action(&(sem->wait_queue), 
                                  wait_reason,
                                  timeout);
         rc = TERR_YIELD;
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

