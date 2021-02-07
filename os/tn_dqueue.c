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
static TN_INLINE TN_DQUEUE_ELEMENT * get_sll_data_addr(SLL_QUEUE * queue)
{
   unsigned long tmp = (unsigned long)queue;
   tmp += sizeof(SLL_QUEUE);

   return (TN_DQUEUE_ELEMENT*)tmp;
   //return ((TN_DQUEUE_ELEMENT*) ((void*)((unsigned char *)((void*)queue) + sizeof(SLL_QUEUE)))); 
}

//----------------------------------------------------------------------------
int tn_dqueue_create(TN_DQUEUE * dque, 
                     unsigned long num_elem,
                     unsigned char * data_buf,
                     unsigned long data_buf_size)   
{
   int rc;
   unsigned char * ptr;
   long i;
   unsigned char * max_ptr;

#if TN_CHECK_PARAM
   if(dque == NULL || num_elem > 65535UL)
   {
      rc = TERR_WPARAM; 
   }
   else 
#endif
   if(tn_inside_int() == TRUE)  //  Check the call context
   {
      rc = TERR_WCONTEXT;
   }
   else if(data_buf == NULL || num_elem == 0U ||
       data_buf_size < (unsigned long)DQUEUE_ENTRY_SIZE)
   {   
      rc = TERR_WPARAM;
   }
   else if(dque->id_dqueue != 0UL)
   {
      rc = TERR_ILUSE; 
   }
   else
   {
      //max_ptr = data_buf + data_buf_size;
      max_ptr = &data_buf[data_buf_size];  // MISRA 2012 18.4

      dque->data_buf  = data_buf;

      sll_init(&dque->inuse_que);
      sll_init(&dque->free_que);

   /**
      Each element contains: 
        - SLL_QUEUE           -- to link
        - data                -- void *
   */
      
      ptr = data_buf;
      i = 0L;
      while(i < (long)num_elem)
      {
         if(ptr >= max_ptr)
         {
            if(i > 0)
            {
               i--; 
            }
            break; 
         }
         else
         {
            i++;
         }  
         sll_add_tail(&dque->free_que, (SLL_QUEUE *)((void*)ptr));
        // ptr += DQUEUE_ENTRY_SIZE;
         ptr = &ptr[DQUEUE_ENTRY_SIZE];  // MISRA 2012 18.4
      } 

      dque->num_elem = (unsigned int)i; // Actual number of an elements

      if(dque->num_elem > 0U)
      {
         queue_reset(&(dque->inuse_wait_queue));
         queue_reset(&(dque->free_wait_queue));
         dque->free_cnt = dque->num_elem; //-- Max value

         dque->id_dqueue = TN_ID_DATAQUEUE & TN_ID_MASK;

         rc = TERR_NO_ERR;
      }
      else
      {
         rc = TERR_WPARAM;
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
#if defined  USE_DYN_OBJ

//----------------------------------------------------------------------------
TN_DQUEUE * tn_dqueue_create_dyn(unsigned long num_elem,
                                 int * err)   // [OUT]      
{
   int rc;
   TN_DQUEUE * dque;
   unsigned long data_mem_size;
   unsigned char * data;

#if TN_CHECK_PARAM
   if(num_elem == 0UL)
   {
      dque = NULL;
      if(err != NULL)
      { 
         *err = TERR_WPARAM;
      }
   }  
   else
#endif
   {
      if(tn_inside_int() == TRUE)  //  Check the call context
      {
         dque = NULL;
         if(err != NULL)
         { 
            *err = TERR_WCONTEXT;
         }
      }
      else
      {

         dque = (TN_DQUEUE *)tn_alloc(&tn_objects_mem, sizeof(TN_DQUEUE));
         if(dque != NULL)
         {
            data_mem_size = (unsigned long)DQUEUE_ENTRY_SIZE * (unsigned long)num_elem;

            data = (unsigned char *)tn_alloc(&tn_objects_mem, data_mem_size);
            if(data == NULL)
            {
               (void)tn_dealloc(&tn_objects_mem, dque);
               dque = NULL;
               if(err != NULL)
               { 
                  *err = TERR_OUT_OF_MEM;
               }
            }
            else
            {
               dque->id_dqueue = 0UL;
               rc = tn_dqueue_create(dque, 
                                     num_elem,
                                     data,
                                     data_mem_size);   
               if(rc == TERR_NO_ERR)
               {
                  if(num_elem != dque->num_elem)
                  {
                     rc = TERR_ILUSE;
                  } 
               }

               if(rc != TERR_NO_ERR) // Error
               {
                  (void)tn_dealloc(&tn_objects_mem, dque);
                  dque = NULL;
                  (void)tn_dealloc(&tn_objects_mem, data);
                  if(err != NULL)
                  { 
                     *err = TERR_OUT_OF_MEM;
                  }
               }
            }
         }
      }
      if(dque != NULL)
      {
         dque->id_dqueue |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }
   return dque;
}

#endif  /* #if defined  USE_DYN_OBJ  */

//----------------------------------------------------------------------------
int tn_dqueue_delete(TN_DQUEUE * dque) 
{
   TN_INTSAVE_DATA
   int rc = TERR_NO_ERR;
#if defined  USE_DYN_OBJ
   unsigned long id;
#endif 
   if(tn_inside_int() == TRUE)
   {
      rc = TERR_WCONTEXT; 
   }
   else if((dque->id_dqueue & TN_ID_MASK) != TN_ID_DATAQUEUE)
   {
      rc = TERR_NOEXS; 
   }
   else if(tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT; 
   }
   else
   {
      tn_disable_interrupt();

      (void)tn_release_wait_que(&(dque->free_wait_queue));
      (void)tn_release_wait_que(&(dque->inuse_wait_queue));

#if defined  USE_DYN_OBJ
      id = dque->id_dqueue;
#endif
      dque->id_dqueue = 0UL;  // Object set as 'not exists'

      tn_enable_interrupt();

#if defined  USE_DYN_OBJ

      if((id & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
      {
         (void)tn_dealloc(&tn_objects_mem, dque->data_buf);
         (void)tn_dealloc(&tn_objects_mem, dque);
      }

#endif
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_dqueue_receive(TN_DQUEUE * dque,
                      TN_DQUEUE_ELEMENT * data_ptr,        /* [OUT] */
                      unsigned long timeout)
{
   SLL_QUEUE * sque;
   int rc;
   int rc_ex;
   TN_DQUEUE_ELEMENT * pdata;

#if TN_CHECK_PARAM
   if(dque == NULL || data_ptr == NULL)
   {
      rc = TERR_WPARAM; 
   }
   else 
#endif
   if((dque->id_dqueue & TN_ID_MASK) != TN_ID_DATAQUEUE)
   {
      rc = TERR_NOEXS; 
   }
   else 
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         if(timeout != TN_NO_WAIT)
         {
            rc = TERR_WCONTEXT; 
         }
         else
         {  
            tn_idisable_interrupt();

            rc = do_input_sem(&dque->inuse_que,
                              TERR_UNDERFLOW,
                              &sque,
                              &dque->inuse_wait_queue, 
                              TSK_WAIT_REASON_INUSE_Q,
                              timeout);

            tn_ienable_interrupt();

            if(rc >= 0)  //  TERR_NO_ERR, TERR_NO_ERR_W
            {
               pdata  = get_sll_data_addr(sque);
               *data_ptr = *pdata;

               tn_idisable_interrupt();

               sll_add_tail(&dque->free_que, sque);
               dque->free_cnt++;
              
               (void)do_output_sem(&dque->free_wait_queue, 
                                   &dque->free_que);

               tn_ienable_interrupt();
            }
         }
      }
      else   // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_input_sem(&dque->inuse_que,
                           TERR_UNDERFLOW,
                           &sque,
                           &dque->inuse_wait_queue, 
                           TSK_WAIT_REASON_INUSE_Q,
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
               sque = (SLL_QUEUE *)tn_curr_run_task->data_elem;  
            }
         }

         if(rc >= 0)   //  TERR_NO_ERR, TERR_NO_ERR_W
         {
            pdata  = get_sll_data_addr(sque);
            *data_ptr = *pdata;

            tn_disable_interrupt();

            sll_add_tail(&dque->free_que, sque);
            dque->free_cnt++;
              
            rc_ex = do_output_sem(&dque->free_wait_queue, 
                                  &dque->free_que);

            tn_enable_interrupt();

            if(rc_ex == TERR_YIELD)
            {
               tn_switch_context();
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
int tn_dqueue_send_ex(TN_DQUEUE * dque, 
                      TN_DQUEUE_ELEMENT data,
                      unsigned long timeout,
                      int send_priority)  // TN_SEND_NORMAL or TN_SEND_URGENT
{
   SLL_QUEUE * sque;
   TN_DQUEUE_ELEMENT * pdata;
   int rc;
   int rc_ex;

#if TN_CHECK_PARAM
   if(dque == NULL)
   {
      rc = TERR_WPARAM; 
   }
   else 
#endif
   if((dque->id_dqueue & TN_ID_MASK) != TN_ID_DATAQUEUE)
   {
      rc = TERR_NOEXS; 
   }
   else 
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         if(timeout != TN_NO_WAIT)
         {
            rc = TERR_WCONTEXT; 
         }
         else
         {  
            tn_idisable_interrupt();

            rc = do_input_sem(&dque->free_que,
                              TERR_OVERFLOW,
                              &sque,
                              &dque->free_wait_queue, 
                              TSK_WAIT_REASON_FREE_Q,
                              timeout);
            if(rc == TERR_NO_ERR)
            {
               dque->free_cnt--; 
            }
            tn_ienable_interrupt();

            if(rc >= 0) 
            {
               // copy data TO entry
               pdata  = get_sll_data_addr(sque);
               *pdata = data;
 
               tn_idisable_interrupt();

               ADD_WITH_SEND_PRI(dque);              

               (void)do_output_sem(&dque->inuse_wait_queue, 
                                   &dque->inuse_que);

               tn_ienable_interrupt();
            }
         }
      }
      else   // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_input_sem(&dque->free_que,
                           TERR_OVERFLOW,
                           &sque,
                           &dque->free_wait_queue, 
                           TSK_WAIT_REASON_FREE_Q,
                           timeout);
         if(rc == TERR_NO_ERR)
         {
            dque->free_cnt--; 
         }
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
               sque = (SLL_QUEUE *)tn_curr_run_task->data_elem;  
            }
         }

         if(rc >= 0)   //  TERR_NO_ERR, TERR_NO_ERR_W
         {
               // copy data TO entry
            pdata  = get_sll_data_addr(sque);
            *pdata = data;

            tn_disable_interrupt();

            ADD_WITH_SEND_PRI(dque);              

            rc_ex = do_output_sem(&dque->inuse_wait_queue, 
                                  &dque->inuse_que);

            tn_enable_interrupt();

            if(rc_ex == TERR_YIELD)
            {
               tn_switch_context();
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
unsigned long tn_dqueue_num_free_entries(TN_DQUEUE * dque)
{ 
   unsigned long rc = TN_INVALID_VAL_UL;

   if(dque != NULL)
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();
         rc = dque->free_cnt;
         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();
         rc = dque->free_cnt;
         tn_enable_interrupt();
      }
   }
   return rc;   
} 

//----------------------------------------------------------------------------
BOOL tn_dqueue_is_empty(TN_DQUEUE * dque)
{ 
   BOOL rc = FALSE;

   if(dque != NULL)
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();
         if(dque->free_cnt == dque->num_elem)
         {
            rc = TRUE;
         }
         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();
         if(dque->free_cnt == dque->num_elem)
         {
            rc = TRUE;
         }
         tn_enable_interrupt();
      }
   }
   return rc;   
} 

//----------------------------------------------------------------------------
// Routines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Shared: TN_MAILBOX, TN_DQUEUE 
//----------------------------------------------------------------------------
int do_input_sem(SLLBASE * slist,
                 int nodata_rc,
                 SLL_QUEUE ** entry,
                 CDLL_QUEUE * wait_queue, 
                 int wait_reason,
                 unsigned long timeout)
{
   SLL_QUEUE * sque;
   int rc;

   sque = sll_remove_head(slist);
   if(sque == NULL)
   {
      rc = nodata_rc; 
   }
   else
   {
      rc = TERR_NO_ERR;
   }

   if(timeout != TN_NO_WAIT) // Not polling
   {
      if(rc != TERR_NO_ERR)  //-- No free entries in the data queue
      {
         task_curr_to_wait_action(wait_queue, 
                                  wait_reason,
                                  timeout);
         rc = TERR_YIELD;
      }
   }

   if(rc == TERR_NO_ERR)
   {
      *entry = sque;
   }

   return rc;    
}

//----------------------------------------------------------------------------
// Shared: TN_MAILBOX, TN_DQUEUE 
//----------------------------------------------------------------------------
int do_output_sem(CDLL_QUEUE * wait_que, SLLBASE * slist)
{
   CDLL_QUEUE * que;
   SLL_QUEUE * sque;
   TN_TCB * task;
   int rc = TERR_ILUSE;

   if(is_queue_empty(wait_que) == FALSE)
   {
      que = queue_remove_head(wait_que);
      task = get_task_by_tsk_queue(que);
      if(task != NULL)
      {
         sque = sll_remove_head(slist);
         if(sque != NULL) //sanity
         { 
            task->data_elem = (void*)sque; 
            if(task_wait_complete(task) == TRUE)
            {
               rc = TERR_YIELD;
            }
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



