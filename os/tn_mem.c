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


//-- Local function prototypes --

static int do_get(TN_FMP * fmp,void ** p_data);
static int do_release(TN_FMP * fmp, void * p_data);
static void * fm_get(TN_FMP * fmp);
static int fm_put(TN_FMP * fmp, void * mem);

//----------------------------------------------------------------------------
//  Structure's field fmp->id_fmp have to be set to 0
//----------------------------------------------------------------------------
int tn_fmem_create(TN_FMP * fmp,
                     unsigned char * start_addr,
                     unsigned long block_size,
                     unsigned long num_blocks)
{
   unsigned char ** p_tmp;
   unsigned char * p_block;
   unsigned long i;
   unsigned long j;
   unsigned long idx;
   unsigned long tmp;
   int rc = TERR_NO_ERR;

#if TN_CHECK_PARAM
   if(fmp == NULL || start_addr == NULL || fmp->id_fmp != 0UL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if(num_blocks < 2UL || block_size < sizeof(int))
   {
      fmp->fblkcnt = 0;
      fmp->num_blocks = 0;
      fmp->id_fmp = 0;
      fmp->free_list = NULL;

      rc = TERR_WPARAM;
   }
   else if(tn_inside_int() == TRUE)
   {
      rc = TERR_WCONTEXT;
   }
   else
   {
      queue_reset(&(fmp->wait_queue));

     //-- Prepare addr/block aligment

      fmp->mem_addr = start_addr;
      i = (unsigned long)start_addr;
      i = (i + (TN_ALIG -1UL)) & (~(TN_ALIG-1UL));
      fmp->start_addr  = (unsigned char*)i;
      fmp->block_size = (unsigned int)((block_size + (TN_ALIG -1UL)) & (~(TN_ALIG-1UL)));

      i = (unsigned long)start_addr;
      i = i + block_size * num_blocks;
      j = (unsigned long)fmp->start_addr + (unsigned long)fmp->block_size * (unsigned long)num_blocks;

      fmp->num_blocks = num_blocks;

      while(j > i)  //-- Get actual num_blocks
      {
         j -= fmp->block_size;
         fmp->num_blocks--;
      }

      if(fmp->num_blocks < 2UL)
      {
         fmp->fblkcnt    = 0;
         fmp->num_blocks = 0;
         fmp->free_list  = NULL;

         rc = TERR_WPARAM;
      }
      else
      {
        //-- Set blocks ptrs for allocation -------
         tmp = (unsigned long)fmp->start_addr;
         p_tmp   = (unsigned char **)tmp;

         p_block = fmp->start_addr + fmp->block_size;
         for(idx = 0UL; idx < (fmp->num_blocks - 1UL); idx++)
         {
            *p_tmp  = (void *)p_block;  //-- contents of cell = addr of next block

            tmp    = (unsigned long)p_block;
            p_tmp  = (unsigned char **)tmp;

           // p_block += fmp->block_size;
            tmp  = (unsigned long)p_block;  // MISRA 2012 18.4
            tmp += fmp->block_size;
            p_block = (unsigned char *)tmp;
         }
         *p_tmp = NULL;          //-- Last memory block first cell contents -  NULL

         fmp->free_list = fmp->start_addr;
         fmp->fblkcnt   = fmp->num_blocks;

         fmp->id_fmp = TN_ID_FSMEMORYPOOL & TN_ID_MASK;

        //-----------------------------------------
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
#if defined  USE_DYN_OBJ

//----------------------------------------------------------------------------
TN_FMP * tn_fmem_create_dyn(unsigned long block_size,
                            unsigned long num_blocks,
                            int * err)  /* [OUT] */
{
   int rc;
   TN_FMP * fmp;
   unsigned long pool_mem_size;
   unsigned long block_size_ex;
   unsigned char * data;

#if TN_CHECK_PARAM
   if(block_size == 0UL || num_blocks == 0UL)
   {
      fmp = NULL;
      if(err != NULL)
      { 
         *err = TERR_WPARAM;
      }
   }  
   else
#endif
   if(tn_inside_int() == TRUE)
   {
      fmp = NULL;
      if(err != NULL)
      { 
         *err = TERR_WCONTEXT;
      }
   }
   else
   {
      fmp = (TN_FMP *)tn_alloc(&tn_objects_mem, sizeof(TN_FMP));
      if(fmp != NULL)
      {
         block_size_ex = ((block_size + (TN_ALIG -1UL)) & (~(TN_ALIG-1UL)));

         pool_mem_size = (unsigned long)block_size_ex * (unsigned long)num_blocks;

         data = (unsigned char *)tn_alloc(&tn_objects_mem, pool_mem_size);
         if(data == NULL)
         {
            (void)tn_dealloc(&tn_objects_mem, fmp);
            fmp = NULL;
            if(err != NULL)
            { 
               *err = TERR_OUT_OF_MEM;
            }
         }
         else
         {
            fmp->id_fmp = 0UL;
            rc = tn_fmem_create(fmp,
                                data,
                                block_size_ex, // Aligned
                                num_blocks);
            if(rc == TERR_NO_ERR)
            {
               if(num_blocks != fmp->num_blocks)
               {
                  rc = TERR_ILUSE;
               } 
            }

            if(rc != TERR_NO_ERR) // Error
            {
               (void)tn_dealloc(&tn_objects_mem, data);
               (void)tn_dealloc(&tn_objects_mem, fmp);
               fmp = NULL;
               if(err != NULL)
               { 
                  *err = TERR_OUT_OF_MEM;
               }
            }
         }
      }

      if(fmp != NULL)
      {
         fmp->id_fmp |= TN_DYN_CREATED;
         if(err != NULL)
         { 
            *err = TERR_NO_ERR;
         }
      }
   }
   return fmp;
}

#endif /* #if defined  USE_DYN_OBJ */

//----------------------------------------------------------------------------
int tn_fmem_delete(TN_FMP * fmp)
{
   TN_INTSAVE_DATA

   int rc = TERR_NO_ERR;
   int fYield;
#if defined  USE_DYN_OBJ
   unsigned long id;
#endif

#if TN_CHECK_PARAM
   if(fmp == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((fmp->id_fmp & TN_ID_MASK) != TN_ID_FSMEMORYPOOL)
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
   else
   {
      tn_disable_interrupt();

      fYield = tn_release_wait_que(&(fmp->wait_queue));

#if defined  USE_DYN_OBJ
      id = fmp->id_fmp;
#endif
      fmp->id_fmp = 0;   //-- Fixed-size memory pool set as 'not exists' now

      tn_enable_interrupt();

#if defined  USE_DYN_OBJ
      if((id & TN_DYN_CREATED) == TN_DYN_CREATED)
      {
         (void)tn_dealloc(&tn_objects_mem, fmp->mem_addr);
         (void)tn_dealloc(&tn_objects_mem, fmp);
      }
#endif

      if(fYield == TRUE)
      { 
         tn_switch_context();
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int tn_fmem_get(TN_FMP * fmp, void ** p_data, unsigned long timeout)
{
   int rc;
 
#if TN_CHECK_PARAM
   if(fmp == NULL || p_data == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((fmp->id_fmp & TN_ID_MASK) != TN_ID_FSMEMORYPOOL)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_inside_int() == TRUE) // Interrupt op
   {
      if(timeout != TN_NO_WAIT)
      {
         rc = TERR_WCONTEXT; 
      }
      else
      {  
         TN_INTSAVE_DATA_INT

         tn_idisable_interrupt();

         rc = do_get(fmp, p_data);

         tn_ienable_interrupt();
      }
   }
   else // Regular(task) op
   {
      TN_INTSAVE_DATA

      tn_disable_interrupt();

      rc = do_get(fmp, p_data);

      if(timeout != TN_NO_WAIT) // Not polling
      {
         if(rc != TERR_NO_ERR)  //-- No free entries in the data queue
         {
            task_curr_to_wait_action(&(fmp->wait_queue),
                                     TSK_WAIT_REASON_WFIXMEM, 
                                     timeout);
            rc = TERR_YIELD;
         }
      }

      tn_enable_interrupt();

      if(rc == TERR_YIELD)
      {
         tn_switch_context();

            //-- When returns to this point,  the 'data_elem' contains a valid value

         *p_data = tn_curr_run_task->data_elem; //-- Return to caller
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

   return rc;
}   
 
//----------------------------------------------------------------------------
int tn_fmem_release(TN_FMP * fmp, void * p_data)
{
   int rc;

#if TN_CHECK_PARAM
   if(fmp == NULL || p_data == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if((fmp->id_fmp & TN_ID_MASK) != TN_ID_FSMEMORYPOOL)
   {
      rc = TERR_NOEXS;
   }
   else if(tn_inside_int() == TRUE) // Interrupt op
   {
      TN_INTSAVE_DATA_INT

      tn_idisable_interrupt();

      rc = do_release(fmp, p_data); 
      if(rc == TERR_YIELD)
      {
#if defined TN_TEST
         rc = TERR_NO_ERR_W;
#else
         rc = TERR_NO_ERR;
#endif
      }

      tn_ienable_interrupt();
   }
   else // Regular(task) op
   {
      TN_INTSAVE_DATA

      tn_disable_interrupt();

      rc = do_release(fmp, p_data); 

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

   return rc;
}

//----------------------------------------------------------------------------
// Routines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
static int do_get(TN_FMP * fmp,void ** p_data)
{
   int rc = TERR_NO_ERR;
   void * ptr;

   ptr = fm_get(fmp);
   if(ptr != NULL) //-- Get memory
   {
      *p_data = ptr;
   }
   else
   {
      rc = TERR_UNDERFLOW;
   }

   return rc;
}

//----------------------------------------------------------------------------
static int do_release(TN_FMP * fmp, void * p_data)
{
   CDLL_QUEUE * que;
   TN_TCB * task;
   int rc; // = TERR_NO_ERR;

   if(is_queue_empty(&(fmp->wait_queue)) == FALSE)
   {
      que  = queue_remove_head(&(fmp->wait_queue));
      task = get_task_by_tsk_queue(que);
      if(task != NULL)
      {
         task->data_elem = p_data;

         if(task_wait_complete(task) == TRUE)
         {
            rc = TERR_YIELD;
         }
         else
         {
            rc = TERR_NOEXS;
         }
      }
      else
      {
         rc = TERR_NOEXS;
      }
   }
   else
   {
      rc = fm_put(fmp, p_data);
   }

   return rc;
}

//----------------------------------------------------------------------------
static void * fm_get(TN_FMP * fmp)
{
   void * p_tmp = NULL;

   if(fmp->fblkcnt > 0UL)
   {
      p_tmp = fmp->free_list;
      fmp->free_list = *(void **)fmp->free_list;   //-- ptr - to new free list
      fmp->fblkcnt--;
   }

   return p_tmp;
}

//----------------------------------------------------------------------------
static int fm_put(TN_FMP * fmp, void * mem)
{
   int rc = TERR_OVERFLOW;

   if(fmp->fblkcnt < fmp->num_blocks)
   {
      *(void **)mem  = fmp->free_list;   //-- insert block into free block list
      fmp->free_list = mem;
      fmp->fblkcnt++;

      rc = TERR_NO_ERR;
   }

   return rc;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


