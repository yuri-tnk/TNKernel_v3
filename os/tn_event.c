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


#ifdef  USE_EVENTFLAGS


static BOOL do_check_release_cond(TN_EVENTFLAGS * evf, 
                                 unsigned int wait_mode, 
                                 unsigned int wait_pattern);
static void do_clear_pattern(TN_EVENTFLAGS * evf, 
                             unsigned int wait_mode, 
                             unsigned int wait_pattern,
                             unsigned int * p_flags_pattern);  // [OUT]
static BOOL scan_event_waitqueue(TN_EVENTFLAGS * evf);

//----------------------------------------------------------------------------
//  Structure's field evf->id_event have to be set to 0
//----------------------------------------------------------------------------
int tn_eventflags_create(TN_EVENTFLAGS * evf,
                         unsigned int pattern)  //-- Initial value of the eventflag bit pattern
{
   int rc = TERR_NO_ERR;
#if TN_CHECK_PARAM
   if(evf == NULL || pattern == TN_INVALID_VAL)  // IAR C-STAT MISRA 2012 generates false message here
   {
      rc = TERR_WPARAM;
   }
   else 
#endif
   if(evf->id_event != 0UL)
   {
      rc = TERR_ILUSE;
   }
   else if(tn_inside_int() == TRUE)  //  Check the call context
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      queue_reset(&(evf->wait_queue));
      evf->pattern = pattern;
      evf->id_event = TN_ID_EVENT & TN_ID_MASK;
   }

   return rc;
}

//----------------------------------------------------------------------------
#if defined  USE_DYN_OBJ

//----------------------------------------------------------------------------
TN_EVENTFLAGS * tn_eventflags_create_dyn(unsigned int pattern,  //-- Initial value of the eventflag bit pattern
                                         int * err)   // [OUT]      
{
   TN_EVENTFLAGS * evf;
   int rc;
   unsigned long tmp;
 
   if(tn_inside_int() == TRUE)  //  Check the call context
   {
      evf = NULL;
      if(err != NULL)
      { 
         *err = TERR_WCONTEXT;
      }
   }
   else // Check input parameters inside a static creation func - optimistic approach
   {
      tmp = sizeof(TN_EVENTFLAGS);  // To make MISRAC 2012 happy
      evf = (TN_EVENTFLAGS *)tn_alloc(&tn_objects_mem, (long)tmp);
      if(evf != NULL)
      {
         evf->id_event = 0UL;
         rc = tn_eventflags_create(evf,
                                   pattern);  //-- Initial value of the eventflag bit pattern
         if(rc != TERR_NO_ERR)
         {
            (void)tn_dealloc(&tn_objects_mem, evf);
            evf = NULL;
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

      if(evf != NULL)
      {
         evf->id_event |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }

   return evf;
}
 

#endif  /* #if defined  USE_DYN_OBJ  */

//----------------------------------------------------------------------------
int tn_eventflags_delete(TN_EVENTFLAGS * evf)
{
   TN_INTSAVE_DATA
   volatile int rc = TERR_NO_ERR;  // IAR 430 not properly handle when non-volatile
   int fYield;
#if defined  USE_DYN_OBJ
   unsigned long id;
#endif
#if TN_CHECK_PARAM
   if(evf == NULL)
   {
      rc = TERR_WPARAM;
   }
   else 
#endif
   if((evf->id_event & TN_ID_MASK) != TN_ID_EVENT)
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
         tn_disable_interrupt();    // v.2.7 - thanks to Eugene Scopal

         fYield = tn_release_wait_que(&(evf->wait_queue));

#if defined  USE_DYN_OBJ
         id = evf->id_event; 
#endif
         evf->id_event = 0UL; // Event not exists now

         tn_enable_interrupt();

#if defined  USE_DYN_OBJ
         if((id & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
         {
            (void)tn_dealloc(&tn_objects_mem, evf);
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
int tn_eventflags_wait(TN_EVENTFLAGS * evf,
                       unsigned int wait_pattern,
                       unsigned int wait_mode,
                       unsigned int * p_flags_pattern,  // [OUT]
                       unsigned long timeout)
{
   int rc;
   int fYield = FALSE;

#if TN_CHECK_PARAM
   if(evf == NULL || wait_pattern == 0U)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((evf->id_event & TN_ID_MASK) != TN_ID_EVENT)
   {
      rc = TERR_NOEXS;
   }
   else 
   {
      if(tn_inside_int() == TRUE) // Inside interrupt
      {
         TN_INTSAVE_DATA_INT

         if(timeout != TN_NO_WAIT) 
         {
            rc = TERR_WCONTEXT; 
         }
         else
         {
            tn_idisable_interrupt();

            rc = do_check_release_cond(evf, 
                                       wait_mode, 
                                       wait_pattern);
            if(rc == TRUE) // Got event rising condition
            {
               do_clear_pattern(evf, 
                                wait_mode, 
                                wait_pattern,
                                p_flags_pattern);  // [OUT]
               rc = TERR_NO_ERR;
            }
            else
            {
               if(p_flags_pattern != NULL)
               {
                  *p_flags_pattern = evf->pattern;
               }
               rc = TERR_TIMEOUT; // Here - Polling failed 
            }

            tn_ienable_interrupt();
         }
      }
      else // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_check_release_cond(evf, 
                                    wait_mode, 
                                    wait_pattern);
         if(rc == TRUE) // Got event rising condition
         {
            do_clear_pattern(evf, 
                             wait_mode, 
                             wait_pattern,
                             p_flags_pattern);  // [OUT]
            rc = TERR_NO_ERR;
         }
         else
         {
            if(p_flags_pattern != NULL)
            {
               *p_flags_pattern = evf->pattern;
            }
            rc = TERR_TIMEOUT; // Here - Polling failed
         }

         if(timeout != TN_NO_WAIT) // Not Polling
         {
            if(rc != TERR_NO_ERR)  // Need Waiting
            {
               tn_curr_run_task->ewait_mode    = wait_mode;
               tn_curr_run_task->ewait_pattern = wait_pattern;

               task_curr_to_wait_action(&(evf->wait_queue),
                                        TSK_WAIT_REASON_EVENT,
                                        timeout);
               fYield  = TRUE;
            }
         }

         tn_enable_interrupt();

         if(fYield == TRUE)
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
               if(p_flags_pattern != NULL)
               {  
                 *p_flags_pattern = tn_curr_run_task->ewait_pattern;
               }
            }
         }
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_eventflags_set(TN_EVENTFLAGS * evf, unsigned int pattern)
{
   int rc = TERR_NO_ERR;
   int fYield = FALSE;

#if TN_CHECK_PARAM
   if(evf == NULL || pattern == 0U)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((evf->id_event & TN_ID_MASK) != TN_ID_EVENT)
   {
      rc = TERR_NOEXS;
   }
   else
   { 
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         evf->pattern |= pattern;     

         if(scan_event_waitqueue(evf) == TRUE) //-- There are task(s) that waiting state is complete
         {
            rc = TERR_NO_ERR;
         }

         tn_ienable_interrupt();
      }
      else //Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         evf->pattern |= pattern;     

         if(scan_event_waitqueue(evf) == TRUE) //-- There are task(s) that waiting state is complete
         {
            fYield = TRUE;
            rc = TERR_NO_ERR;
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
int tn_eventflags_clear(TN_EVENTFLAGS * evf, unsigned int pattern)
{
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(evf == NULL || pattern == TN_INVALID_VAL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((evf->id_event & TN_ID_MASK) != TN_ID_EVENT)
   { 
      rc = TERR_NOEXS;
   }
   else
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         evf->pattern &= pattern;

         tn_ienable_interrupt();
      }
      else //Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         evf->pattern &= pattern;

         tn_enable_interrupt();
      }
   }
   return rc;
}


//----------------------------------------------------------------------------
// Routines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
static BOOL do_check_release_cond(TN_EVENTFLAGS * evf, 
                                 unsigned int wait_mode, 
                                 unsigned int wait_pattern)
{
   BOOL fCond = FALSE;

      //-- Check release condition

   if((wait_mode & TN_EVENT_WCOND_OR) == TN_EVENT_WCOND_OR) //-- any setted bit is enough for release condition
   {
      if((evf->pattern & wait_pattern) != 0U)
      {
         fCond = TRUE;
      }
   }
   else             //-- TN_EVENT_WCOND_AND is default mode
   {
      if((evf->pattern & wait_pattern) == wait_pattern)
      {
         fCond = TRUE;
      }
   }
   return fCond;
}

//----------------------------------------------------------------------------
static void do_clear_pattern(TN_EVENTFLAGS * evf, 
                             unsigned int wait_mode, 
                             unsigned int wait_pattern,
                             unsigned int * p_flags_pattern)  // [OUT]
{
   if(p_flags_pattern != NULL)
   {
      *p_flags_pattern = evf->pattern;
   }

   if((wait_mode & TN_EVENT_ATTR_BITCLR) == TN_EVENT_ATTR_BITCLR)
   {
      evf->pattern &= ~wait_pattern;
   }
}

//----------------------------------------------------------------------------
static BOOL scan_event_waitqueue(TN_EVENTFLAGS * evf)
{
   CDLL_QUEUE * que;
   CDLL_QUEUE * que_next;
   TN_TCB * task;
   BOOL rc    = FALSE;

   for(que = evf->wait_queue.next; que != &(evf->wait_queue); que = que_next)
   {
      que_next = que->next;

      task = get_task_by_tsk_queue(que);
      if(task != NULL)
      {
         rc = do_check_release_cond(evf, 
                                    task->ewait_mode,     // wait_mode, 
                                    task->ewait_pattern); // wait_pattern);

         if(rc == TRUE) // Got event condition
         {
            queue_remove_entry(&task->task_queue);
            if(task_wait_complete(task) == TRUE)
            {
               rc = TRUE;
            }

            if((task->ewait_mode  & TN_EVENT_ATTR_BITCLR) == TN_EVENT_ATTR_BITCLR)
            {
               evf->pattern &= ~task->ewait_pattern;
            }
    
            if(evf->pattern == 0U) // No reason to continue
            {
               break;
            }   
         }
      }
   }

   return rc;
}


#endif   /* #ifdef  USE_EVENTFLAGS */


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
