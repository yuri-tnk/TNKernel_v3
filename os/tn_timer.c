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

/*******************************************************************************
 *
 * TNeo: real-time kernel initially based on TNKernel
 *
 *    TNKernel:                  copyright © 2004, 2013 Yuri Tiomkin.
 *    PIC32-specific routines:   copyright © 2013, 2014 Anders Montonen.
 *    TNeo:                      copyright © 2014       Dmitry Frank.
 *
 *    TNeo was born as a thorough review and re-implementation of
 *    TNKernel. The new kernel has well-formed code, inherited bugs are fixed
 *    as well as new features being added, and it is tested carefully with
 *    unit-tests.
 *
 *    API is changed somewhat, so it's not 100% compatible with TNKernel,
 *    hence the new name: TNeo.
 *
 *    Permission to use, copy, modify, and distribute this software in source
 *    and binary forms and its documentation for any purpose and without fee
 *    is hereby granted, provided that the above copyright notice appear
 *    in all copies and that both that copyright notice and this permission
 *    notice appear in supporting documentation.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE DMITRY FRANK AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DMITRY FRANK OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *    THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/


  /* ver 3  */

#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"

#if defined USER_TIMERS

static void tn_user_timer_cancel(TN_TIMER * tmr);
static void tn_user_timer_start(TN_TIMER * tmr);

//----------------------------------------------------------------------------
//   Structure's field tmr->id_trigger have to be set to 0
//----------------------------------------------------------------------------
int tn_timer_create(TN_TIMER * tmr,
                    timer_func_handler timer_func,
                    void * param)
{
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(tmr == NULL || timer_func == NULL || tmr->id_timer != 0UL) //-- no recreation
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
      queue_reset(&(tmr->queue));

      tmr->period_ticks = 0UL;
      tmr->cnt          = 0UL;
      tmr->param        = param;  
      tmr->timer_func   = timer_func;
      tmr->id_timer     = TN_ID_TIMER & TN_ID_MASK;
   }

   return rc;
}

//----------------------------------------------------------------------------
#if defined  USE_DYN_OBJ

//----------------------------------------------------------------------------
TN_TIMER * tn_timer_create_dyn(timer_func_handler timer_func,
                               void * param,
                               int * err)
{
   int rc;
   TN_TIMER * tmr;

   if(tn_inside_int() == TRUE)  //  Check the call context
   {
      tmr = NULL;
      if(err != NULL)
      {
         *err = TERR_WCONTEXT;
      }
   }
   else // Check input parameters inside a static creation func - optimistic approach
   {
      tmr = (TN_TIMER *)tn_alloc(&tn_objects_mem, sizeof(TN_TIMER));
      if(tmr != NULL)
      {
         tmr->id_timer = 0UL;
         rc = tn_timer_create(tmr, timer_func, param);
         if(rc != TERR_NO_ERR)
         {
            (void)tn_dealloc(&tn_objects_mem, tmr);
            tmr = NULL;
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

      if(tmr != NULL)
      {
         tmr->id_timer |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }

   return tmr;
}

#endif  /* #if defined  USE_DYN_OBJ  */

//----------------------------------------------------------------------------
int tn_timer_delete(TN_TIMER * tmr)
{
   int rc = TERR_NO_ERR;
#if defined  USE_DYN_OBJ
   unsigned long id;
#endif

#if TN_CHECK_PARAM
   if(tmr == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((tmr->id_timer & TN_ID_MASK) != TN_ID_TIMER)
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
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         tn_user_timer_cancel(tmr);

#if defined  USE_DYN_OBJ
         id = tmr->id_timer;
#endif
         tmr->id_timer = 0UL; // Timer set as 'not exists'

         tn_enable_interrupt();

#if defined  USE_DYN_OBJ
         if((id & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
         {
            (void)tn_dealloc(&tn_objects_mem, tmr);
         }
#endif
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_timer_set(TN_TIMER * tmr,              // Timer to Start/re-start 
                 unsigned long due_ticks,     // timeout for the first action triggered (in sys ticks)
                 unsigned long period_ticks)  // Periodic action time(if not 0)(in sys ticks)
{
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(tmr == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if(due_ticks == 0UL || period_ticks == TN_WAIT_INFINITE)
   {
      rc = TERR_WPARAM;
   }
   else if((tmr->id_timer & TN_ID_MASK) != TN_ID_TIMER)
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
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         tn_user_timer_cancel(tmr);

         tmr->cnt           = due_ticks;   
         tmr->period_ticks  = period_ticks;

         tn_user_timer_start(tmr);

         tn_enable_interrupt();
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_timer_cancel(TN_TIMER * tmr)
{
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(tmr == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((tmr->id_timer & TN_ID_MASK) != TN_ID_TIMER)
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
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         tn_user_timer_cancel(tmr);

         tn_enable_interrupt();
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
static TN_INLINE TN_TIMER * get_timer_by_timer_queue(CDLL_QUEUE * que)
{
   return (TN_TIMER *)((void*)que);
}

//----------------------------------------------------------------------------
static TN_INLINE unsigned long  user_tick_list_index(unsigned long timeout)
{
   return (((unsigned long)tn_sys_tick_count + timeout) & (unsigned long)TN_USER_TICK_LISTS_MASK);
}

//----------------------------------------------------------------------------
BOOL tn_user_timers_tick_proc(void)
{
   //-- tn_sys_tick_count is incremented inside interrupt

   TN_INTSAVE_DATA

   CDLL_QUEUE * que;
   CDLL_QUEUE * que_next;
   CDLL_QUEUE * p_cur_timer_list;
   CDLL_QUEUE periodic_que;

   TN_TIMER * tmr;
   int tick_list_index;
   int rc = FALSE;

   queue_reset(&periodic_que);

   tick_list_index = (int)user_tick_list_index(0);

   tn_disable_interrupt();

   if(tick_list_index == 0)
   {
      //-- it happens each TN_TICK_LISTS_CNT-th system tick:
      //   now we should walk through all the timers in the "generic" timer
      //   list, decrement the timeout of each one by TN_TICK_LISTS_CNT,
      //   and if resulting timeout is less than TN_TICK_LISTS_CNT,
      //   then move the timer to the appropriate "tick" timer list.

      //-- handle "generic" timer list 

      for(que = tn_user_timer_list_gen.next; 
             que != &tn_user_timer_list_gen; que = que_next)
      {
         que_next = que->next;

         tmr = get_timer_by_timer_queue(que);
         if(tmr != NULL)
         { 
            tmr->cnt -= (unsigned long)TN_USER_TICK_LISTS_CNT;

            if(tmr->cnt < (unsigned long)TN_USER_TICK_LISTS_CNT)
            {
               //-- it's the time to move this timer to the "tick" list
               queue_remove_entry(&(tmr->queue));
               queue_add_tail(&tn_user_timer_list_tick[user_tick_list_index(tmr->cnt)],
                              &(tmr->queue));
            }
         }
      }
   }

   //-- it happens every system tick:
   //   we should walk through all the timers in the current "tick" timer list,
   //   and fire them all, unconditionally.

   //-- handle current "tick" timer list

   p_cur_timer_list =  &tn_user_timer_list_tick[tick_list_index];

      //-- now, p_cur_timer_list is a list of timers that we should
      //   fire NOW, unconditionally.

   while(is_queue_empty(p_cur_timer_list) == FALSE)
   {
      que = p_cur_timer_list->next;
      tmr = get_timer_by_timer_queue(que);
      if(tmr != NULL)
      {
         tn_user_timer_cancel(tmr);
         //-- if timer is periodic: 
         //    1. Reload timer counter 
         //    2. Add timer to the local periodic queue  
         if(tmr->period_ticks != 0UL)
         {  
            tmr->cnt = tmr->period_ticks; 
            queue_add_tail(&periodic_que, &tmr->queue);
         }
         //-- Exec timer callback func
         if(tmr->timer_func != NULL)
         {
            tmr->timer_func(tmr, tmr->param);
         }
      }
   }

   while(is_queue_empty(&periodic_que) == FALSE)
   {
      que = queue_remove_head(&periodic_que);
      tmr = get_timer_by_timer_queue(que);
      if(tmr != NULL)
      {
         //--  restart timer
         tn_user_timer_start(tmr);
      }
   }

   tn_enable_interrupt();

   return rc;
}

//----------------------------------------------------------------------------
// Interrupts have to be disabled here
//----------------------------------------------------------------------------
static void tn_user_timer_cancel(TN_TIMER * tmr)
{
   if(is_queue_empty(&(tmr->queue)) == FALSE)
   {
      tmr->cnt = 0UL;
      queue_remove_entry(&(tmr->queue));
      queue_reset(&(tmr->queue));
   }
}

//----------------------------------------------------------------------------
// Interrupts have to be disabled here
//----------------------------------------------------------------------------
static void tn_user_timer_start(TN_TIMER * tmr)
{
   unsigned long tick_list_index; 
     // suppose that timer is not active here
   
   if(tmr->cnt < (unsigned long)TN_USER_TICK_LISTS_CNT)
   {
         //-- timer should be added to the one of "tick" lists.
      tick_list_index = user_tick_list_index(tmr->cnt);
      tmr->cnt = tick_list_index;

      queue_add_tail(&tn_user_timer_list_tick[tick_list_index], &(tmr->queue));
   } 
   else 
   {
         //-- timer should be added to the "generic" list.
         //   We should set timeout_cur adding current "tick" index to it.
      tmr->cnt = tmr->cnt + user_tick_list_index(0);
      queue_add_tail(&tn_user_timer_list_gen, &(tmr->queue));
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#endif /* #if defined USER_TIMERS */




