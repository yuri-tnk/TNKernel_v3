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

#ifndef  TN_UTILS_H
#define  TN_UTILS_H


#include "tn_config.h"
#include "tn.h"

#ifdef __cplusplus
extern "C"  {
#endif

  //-- Circular double-linked list queue --

void queue_reset(CDLL_QUEUE *que);
BOOL is_queue_empty(CDLL_QUEUE *que);
void queue_add_head(CDLL_QUEUE * que, CDLL_QUEUE * entry);
void queue_add_tail(CDLL_QUEUE * que, CDLL_QUEUE * entry);
CDLL_QUEUE * queue_remove_head(CDLL_QUEUE * que);
CDLL_QUEUE * queue_remove_tail(CDLL_QUEUE * que);
void queue_remove_entry(CDLL_QUEUE * entry);
BOOL queue_contains_entry(CDLL_QUEUE * que, CDLL_QUEUE * entry);
void queue_insert_by_priority(CDLL_QUEUE * queue, 
                              CDLL_QUEUE * entry_to_insert,
                              unsigned int priority);


//int  dque_fifo_write(TN_DQUE * dque, void * data_ptr);
//int  dque_fifo_read(TN_DQUE * dque, void ** data_ptr);

  //-- Singly-linked list queue --

void sll_init(SLLBASE * root);
SLL_QUEUE * sll_remove_head(SLLBASE * root);
void sll_add_tail(SLLBASE * root, SLL_QUEUE * entry);
void sll_add_head(SLLBASE * root, SLL_QUEUE * entry);

#ifdef __cplusplus
}
#endif


#endif

