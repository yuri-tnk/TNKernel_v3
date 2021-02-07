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
//      Circular double-linked list queue
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void queue_reset(CDLL_QUEUE * que)
{
   que->prev = que->next = que;
}

//----------------------------------------------------------------------------
BOOL is_queue_empty(CDLL_QUEUE * que)
{
   BOOL rc = FALSE;
   if(que->next == que && que->prev == que)
   {
      rc = TRUE;
   }
   return rc;
}

//----------------------------------------------------------------------------
void queue_add_head(CDLL_QUEUE * que, CDLL_QUEUE * entry)
{
  //--  Insert an entry at the head of the queue.

   entry->next = que->next;
   entry->prev = que;
   entry->next->prev = entry;
   que->next = entry;
}

//----------------------------------------------------------------------------
void queue_insert_by_priority(CDLL_QUEUE * queue, 
                              CDLL_QUEUE * entry_to_insert,
                              unsigned int priority)
{
   CDLL_QUEUE * curr_que;
   TN_TCB * tmp_task;

   // Case Interrupts are disabled here, the queue walking is safe

   for(curr_que = queue->next; curr_que != queue; curr_que = curr_que->next)
   {
      tmp_task = get_task_by_tsk_queue(curr_que);
      if(tmp_task != NULL) // Make MISRA 2012 happy
      {
         if(tmp_task->priority > priority) 
         {
            break;      // Found the task with lower priority
         }
      }  
   }

   if(curr_que == queue)  // If queue is empty or there are no task(s)
   {                      // with lower priority - append to the end
      queue_add_tail(queue, entry_to_insert);
   }
   else // Insert before 'curr_que', case the 'priority' is higher
   {
      curr_que->prev->next  = entry_to_insert;
      entry_to_insert->prev = curr_que->prev;
      curr_que->prev        = entry_to_insert;
      entry_to_insert->next = curr_que;
   }
}

//----------------------------------------------------------------------------
void queue_add_tail(CDLL_QUEUE * que, CDLL_QUEUE * entry)
{
  //-- Insert an entry at the tail of the queue.

   entry->next = que;
   entry->prev = que->prev;
   entry->prev->next = entry;
   que->prev = entry;
}

//----------------------------------------------------------------------------
CDLL_QUEUE * queue_remove_head(CDLL_QUEUE * que)
{
   //-- Remove and return an entry at the head of the queue.

   CDLL_QUEUE * entry = NULL;

   if(que != NULL && que->next != que) // not empty
   {
      entry = que->next;
      entry->next->prev = que;
      que->next = entry->next;
   }
   return entry;
}

//----------------------------------------------------------------------------
//-- Remove and return an entry from the tail of the queue.
//----------------------------------------------------------------------------
CDLL_QUEUE * queue_remove_tail(CDLL_QUEUE * que)
{

   CDLL_QUEUE * entry = NULL;

   if(que->prev != que) // Not empty
   {
      entry = que->prev;
      entry->prev->next = que;
      que->prev = entry->prev;
   }

   return entry;
}

//----------------------------------------------------------------------------
void queue_remove_entry(CDLL_QUEUE * entry)
{
   //--  Remove an entry from the queue.

   entry->prev->next = entry->next;
   entry->next->prev = entry->prev;
}

//----------------------------------------------------------------------------
BOOL queue_contains_entry(CDLL_QUEUE * que, CDLL_QUEUE * entry)
{
  //-- The entry in the queue ???
   BOOL rc = FALSE;
   CDLL_QUEUE * curr_que;

  // Case Interrupts are disabled here, the queue walking is safe

   for(curr_que = que->next; curr_que != que; curr_que = curr_que->next)
   {
      if(curr_que == entry)
      {
         rc = TRUE;   //-- Found
         break;
      }
   }

   return rc;
}

//---------------------------------------------------------------------------
//    Data queue storage FIFO processing
//---------------------------------------------------------------------------

#if 0
//---------------------------------------------------------------------------
int  dque_fifo_write(TN_DQUE * dque, void * data_ptr)
{
   int rc = TERR_NO_ERR;
#if TN_CHECK_PARAM
   if(dque == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if(dque->num_entries <= 0)
   {
      rc = TERR_OUT_OF_MEM;
   }
   else
   {
      if((dque->tail_cnt == 0 && dque->header_cnt == dque->num_entries - 1)
                || dque->header_cnt == dque->tail_cnt-1)
      {
         rc = TERR_OVERFLOW;  //--  full
      }
      else //-- wr data
      {
         dque->data_fifo[dque->header_cnt] = data_ptr;
         dque->header_cnt++;
         if(dque->header_cnt >= dque->num_entries)
         { 
            dque->header_cnt = 0;
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
int dque_fifo_read(TN_DQUE * dque, void ** data_ptr)
{
   int rc = TERR_NO_ERR; 
#if TN_CHECK_PARAM
   if(dque == NULL || data_ptr == NULL)
   {
      rc = TERR_WPARAM;
   }
   else
#endif
   if(dque->num_entries <= 0)
   {
      rc = TERR_OUT_OF_MEM;
   }
   else if(dque->tail_cnt == dque->header_cnt)
   {
      rc = TERR_UNDERFLOW; //-- empty
   }
   else //-- rd data
   {
      *data_ptr  =  dque->data_fifo[dque->tail_cnt];
      dque->tail_cnt++;
      if(dque->tail_cnt >= dque->num_entries)
      {
         dque->tail_cnt = 0;
      }
   }
   return rc;
}

#endif
//----------------------------------------------------------------------------
//      Singly-linked list queue
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void sll_init(SLLBASE * root)
{
   root->head = NULL;
   root->tail = NULL;
}

//----------------------------------------------------------------------------
SLL_QUEUE * sll_remove_head(SLLBASE * root)
{
   SLL_QUEUE * entry = root->head;

   if(root->head != NULL)
   {
      root->head = root->head->next;
   }

   return entry;
}

//----------------------------------------------------------------------------
void sll_add_tail(SLLBASE * root, SLL_QUEUE * entry)
{
    // Add the entry to the tail of queue 

   entry->next = NULL;

   if(root->head == NULL) // Empty
   { 
      root->head = entry;
      root->tail = root->head;
   }
   else
   {
      root->tail->next = entry;
      root->tail       = entry;
   }
}

//----------------------------------------------------------------------------
void sll_add_head(SLLBASE * root, SLL_QUEUE * entry)
{
   // Add the entry to the head of queue 

   entry->next = root->head;

   if(root->head == NULL)
   {
      root->tail = entry;
   }
   root->head = entry; 
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


