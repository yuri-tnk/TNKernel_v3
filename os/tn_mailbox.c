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

#include <string.h>
#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"


#ifndef  MIN
#define  MIN(a,b)    (((a)<(b))?(a):(b))
#endif
#ifndef  MAX
#define  MAX(a,b)    (((a)>(b))?(a):(b))
#endif


#define MAILBOX_ENTRY_HEADER_SIZE  ((unsigned long)elem_size + sizeof(SLL_QUEUE) + sizeof(unsigned long))

static TN_INLINE void do_copy_to_mb(TN_MAILBOX * mb,
                          SLL_QUEUE * que,
                          void * data_ptr,
                          unsigned long data_size);

static TN_INLINE unsigned long do_copy_from_mb(TN_MAILBOX * mb,
                                     SLL_QUEUE * que,
                                     void * data_ptr,
                                     unsigned long max_len);


//----------------------------------------------------------------------------
static TN_INLINE unsigned long * get_len_addr(SLL_QUEUE * queue)
{
   unsigned long tmp = (unsigned long)queue;
   tmp += sizeof(SLL_QUEUE);

   return (unsigned long *)tmp;
   //return ((unsigned long *) ((void*)((unsigned char *)((void*)queue) + sizeof(SLL_QUEUE)))); 
}

//----------------------------------------------------------------------------
static TN_INLINE unsigned char * get_data_addr(SLL_QUEUE * queue)
{
   unsigned long tmp = (unsigned long)queue;
   tmp += sizeof(SLL_QUEUE) + sizeof(unsigned long);

   return (unsigned char *)tmp;

   //return ((unsigned char *)((void*)queue) + sizeof(SLL_QUEUE) + sizeof(unsigned long)); 
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int tn_mailbox_create(TN_MAILBOX * mb, 
                     unsigned long num_elem,
                     unsigned long elem_size,  
                     unsigned char * data_buf,
                     unsigned long data_buf_size)   
{
   int rc = TERR_NO_ERR;
   unsigned char * ptr;
   long i;
   unsigned char * max_ptr;

#if TN_CHECK_PARAM
   if(mb == NULL || num_elem > 65535UL)     // second checking - to make IAR C-STAT MISRA 2012 happy
   {                                 
      rc = TERR_WPARAM; 
   }
   else
#endif
   if(mb->id_mailbox != 0UL)
   {
      rc = TERR_ILUSE; 
   }
   else if(data_buf == NULL || num_elem == 0UL ||  
       elem_size == 0UL || data_buf_size < MAILBOX_ENTRY_HEADER_SIZE + 1UL)      
   {
      rc = TERR_WPARAM; 
   }
   else if(tn_inside_int() == TRUE)  //  Check the call context
   {
      rc = TERR_WCONTEXT; 
   }
   else
   {
//      max_ptr = data_buf + data_buf_size;
      max_ptr = &data_buf[data_buf_size];  // MISRA 2012 18.4
      mb->data_buf  = data_buf;
      mb->elem_size = elem_size;

      sll_init(&mb->inuse_que);
      sll_init(&mb->free_que);


   /**
      Each element contains: 
        - SLL_QUEUE           -- to link
        - unsigned int len;    -- actual data size in the entry
        - data                 -- max data size = mb->elem_size
   */
      
      ptr = data_buf;
      i = 0;
      while(i < (long)num_elem)
      {
         if(ptr >= max_ptr)
         {
            if(i > 0L)
            { 
               i--;
            }
            break; 
         }
         else
         {
            i++;
         } 

         sll_add_tail(&mb->free_que, (SLL_QUEUE *)((void*)ptr));
         //ptr += MAILBOX_ENTRY_HEADER_SIZE;
         ptr = &ptr[MAILBOX_ENTRY_HEADER_SIZE]; // MISRA 2012 18.4
      } 

      mb->num_elem = (unsigned long)i; // Actual number of an elements

      if(mb->num_elem > 0U)
      {
         queue_reset(&(mb->inuse_wait_queue));
         queue_reset(&(mb->free_wait_queue));
         mb->free_cnt = mb->num_elem; //-- Max value

         mb->id_mailbox = TN_ID_MAILBOX & TN_ID_MASK;
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
TN_MAILBOX *  tn_mailbox_create_dyn(unsigned long num_elem,
                                   unsigned long elem_size,  
                                   int * err)   // [OUT]   
{
   int rc;
   TN_MAILBOX * mb;
   unsigned long data_mem_size;
   unsigned char * data;

#if TN_CHECK_PARAM
   if(num_elem == 0U || elem_size == 0U)
   {
      mb = NULL;
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
         mb = NULL;
         if(err != NULL)
         { 
            *err = TERR_WCONTEXT;
         }
      }
      else
      {
         mb = (TN_MAILBOX *)tn_alloc(&tn_objects_mem, sizeof(TN_MAILBOX));
         if(mb != NULL)
         {
            data_mem_size = (unsigned long)MAILBOX_ENTRY_HEADER_SIZE * (unsigned long)num_elem;
            if(data_mem_size < MAILBOX_ENTRY_HEADER_SIZE + 1UL)
            {
               data_mem_size = MAILBOX_ENTRY_HEADER_SIZE + 1UL;
            }

            data = (unsigned char *)tn_alloc(&tn_objects_mem, data_mem_size);
            if(data == NULL)
            {
               (void)tn_dealloc(&tn_objects_mem, mb);
               mb = NULL;
               if(err != NULL)
               { 
                  *err = TERR_OUT_OF_MEM;
               }
            }
            else
            {
               mb->id_mailbox = 0UL;
               rc = tn_mailbox_create(mb, 
                                     num_elem,
                                     elem_size,  
                                     data,
                                     data_mem_size);
               if(rc == TERR_NO_ERR)
               {
                  if(num_elem != mb->num_elem)
                  {
                     rc = TERR_ILUSE;
                  } 
               }

               if(rc != TERR_NO_ERR) // Error
               {
                  (void)tn_dealloc(&tn_objects_mem, mb);
                  (void)tn_dealloc(&tn_objects_mem, data);
                  mb = NULL;
                  if(err != NULL)
                  { 
                     *err = rc;
                  }
               }
            }
         }
      }

      if(mb != NULL)
      {
         mb->id_mailbox |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }
   return mb;
}

#endif /* #if defined  USE_DYN_OBJ */

//----------------------------------------------------------------------------
int tn_mailbox_delete(TN_MAILBOX * mb) 
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
   else if((mb->id_mailbox & TN_ID_MASK) != TN_ID_MAILBOX)
   {
      rc = TERR_NOEXS; 
   }
   else if( tn_system_state != TN_ST_STATE_RUNNING)
   {
      rc = TERR_WCONTEXT; 
   }
   else
   {
      tn_disable_interrupt();

      (void)tn_release_wait_que(&(mb->free_wait_queue));
      (void)tn_release_wait_que(&(mb->inuse_wait_queue));

#if defined  USE_DYN_OBJ
      id = mb->id_mailbox;
#endif
      mb->id_mailbox = 0UL;  // Object set as 'not exists'

      tn_enable_interrupt();

#if defined  USE_DYN_OBJ
      if((id & TN_DYN_MASK) == TN_DYN_CREATED) // was created dynamically
      {
         (void)tn_dealloc(&tn_objects_mem, mb->data_buf);
         (void)tn_dealloc(&tn_objects_mem, mb);
      }
#endif
   }

   return rc;
}

//----------------------------------------------------------------------------
long tn_mailbox_receive(TN_MAILBOX * mb,
                        void * data_ptr,        /* [OUT] */
                        unsigned long max_len,
                        unsigned long timeout)
{
   SLL_QUEUE * sque;
   int rc;
   int rc_ex;
   long res = 0L;
   unsigned long data_len;

#if TN_CHECK_PARAM
   if(mb == NULL || data_ptr == NULL || max_len == 0UL)
   {
      res = TERR_WPARAM; 
   }
   else 
#endif
   if((mb->id_mailbox & TN_ID_MASK) != TN_ID_MAILBOX)
   {
      res = TERR_NOEXS; 
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

            rc = do_input_sem(&mb->inuse_que,
                              TERR_UNDERFLOW,
                              &sque,
                              &mb->inuse_wait_queue, 
                              TSK_WAIT_REASON_INUSE_Q,
                              timeout);

            tn_ienable_interrupt();

            if(rc >= 0) //  TERR_NO_ERR, TERR_NO_ERR_W
            {
               // copy data FROM entry
               data_len = do_copy_from_mb(mb,
                                          sque,
                                          data_ptr,
                                          max_len);
               res = (long)data_len;

               tn_idisable_interrupt();

               sll_add_tail(&mb->free_que, sque);
               mb->free_cnt++;
              
               (void)do_output_sem(&mb->free_wait_queue, 
                                   &mb->free_que);

               tn_ienable_interrupt();
            }
         }
      }
      else   // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_input_sem(&mb->inuse_que,
                           TERR_UNDERFLOW,
                           &sque,
                           &mb->inuse_wait_queue, 
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

         if(rc >= 0) //  TERR_NO_ERR, TERR_NO_ERR_W
         {
              // copy data FROM entry
            data_len = do_copy_from_mb(mb,
                                       sque,
                                       data_ptr,
                                       max_len);
            res = (long)data_len;

            tn_disable_interrupt();

            sll_add_tail(&mb->free_que, sque);
            mb->free_cnt++;
              
            rc_ex = do_output_sem(&mb->free_wait_queue, 
                                  &mb->free_que);

            tn_enable_interrupt();

            if(rc_ex == TERR_YIELD)
            {
               tn_switch_context();
            }
         }
      }
      if(rc < 0) // Err
      {
         res = rc;
      }
   }
   return res;
}

//----------------------------------------------------------------------------
int tn_mailbox_send_ex(TN_MAILBOX * mb, 
                      void * data_ptr,
                      unsigned long data_size,
                      unsigned long timeout, 
                      int send_priority)  // TN_SEND_NORMAL or TN_SEND_URGENT
{
   SLL_QUEUE * sque;
   int rc;
   int rc_ex;

#if TN_CHECK_PARAM
   if(mb == NULL || data_ptr == NULL || data_size == 0UL)
   {
      rc = TERR_WPARAM; 
   }
   else 
#endif
   if((mb->id_mailbox & TN_ID_MASK) != TN_ID_MAILBOX)
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

            rc = do_input_sem(&mb->free_que,
                              TERR_OVERFLOW,
                              &sque,
                              &mb->free_wait_queue, 
                              TSK_WAIT_REASON_FREE_Q,
                              timeout);
            if(rc == TERR_NO_ERR)
            {
               mb->free_cnt--; 
            }
            tn_ienable_interrupt();

            if(rc >= 0)   //  TERR_NO_ERR, TERR_NO_ERR_W
            {
               // copy data TO entry
               do_copy_to_mb(mb,
                             sque,
                             data_ptr,
                             data_size);
 
               tn_idisable_interrupt();

               ADD_WITH_SEND_PRI(mb);              

               (void)do_output_sem(&mb->inuse_wait_queue, 
                                   &mb->inuse_que);

               tn_ienable_interrupt();
            }
         }
      }
      else   // Task context
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();

         rc = do_input_sem(&mb->free_que,
                           TERR_OVERFLOW,
                           &sque,
                           &mb->free_wait_queue, 
                           TSK_WAIT_REASON_FREE_Q,
                           timeout);
         if(rc == TERR_NO_ERR)
         {
            mb->free_cnt--; 
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

         if(rc >= 0)  //  TERR_NO_ERR, TERR_NO_ERR_W
         {
               // copy data TO entry
            do_copy_to_mb(mb,
                          sque,
                          data_ptr,
                          data_size);

            tn_disable_interrupt();

            ADD_WITH_SEND_PRI(mb);              

            rc_ex = do_output_sem(&mb->inuse_wait_queue, 
                                  &mb->inuse_que);

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
unsigned long tn_mailbox_num_free_entries(TN_MAILBOX * mb)
{ 
   unsigned long rc = TN_INVALID_VAL_UL;

   if(mb != NULL)
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();
         rc = mb->free_cnt;
         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();
         rc = mb->free_cnt;
         tn_enable_interrupt();
      }
   }

   return rc;   
}
//----------------------------------------------------------------------------
BOOL tn_mailbox_is_empty(TN_MAILBOX * mb)
{ 
   BOOL rc = FALSE;

   if(mb != NULL)
   {
      if(tn_inside_int() == TRUE)
      {
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();
         if(mb->free_cnt == mb->num_elem)
         {
            rc = TRUE;
         }
         tn_ienable_interrupt();
      }
      else
      {
         TN_INTSAVE_DATA

         tn_disable_interrupt();
         if(mb->free_cnt == mb->num_elem)
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
static TN_INLINE void do_copy_to_mb(TN_MAILBOX * mb,
                                 SLL_QUEUE * que,
                                 void * data_ptr,
                                 unsigned long data_size)
{
   unsigned long * p_len;
   unsigned char * p_data;

     // copy to mailbox

   data_size = MIN(data_size, mb->elem_size);
   p_len = get_len_addr(que);
   *p_len = data_size;
   p_data = get_data_addr(que);
   (void)memcpy(p_data, data_ptr, data_size);
}

//----------------------------------------------------------------------------
static TN_INLINE unsigned long do_copy_from_mb(TN_MAILBOX * mb,
                                            SLL_QUEUE * que,
                                            void * data_ptr,
                                            unsigned long max_len)
{
   unsigned long * p_len;
   unsigned long data_size; 
   unsigned char * p_data;

       // copy from mailbox

   p_len  = get_len_addr(que);
   data_size = *p_len;
   data_size = MIN(data_size, max_len);

   p_data = get_data_addr(que);
   (void)memcpy(data_ptr, p_data, data_size);

   return  data_size;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

