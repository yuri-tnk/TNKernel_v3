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

  /* ver 3 */

#ifndef TH_H
#define TH_H

#ifndef TN_PRJ_CONFIG
  #error "The project configuration for the TNKernel was not found"
#endif

//--- Types

#ifndef BOOL
#define BOOL      int
#endif

#ifndef TRUE
#define TRUE      1
#endif

#ifndef FALSE
#define FALSE     0
#endif

#ifndef NULL
#define NULL      0
#endif


   //--- Port

#include <stddef.h>

#ifdef __cplusplus       
extern "C"  {
#endif

#include "tn_port.h"



//--- Constants

#define  TN_ST_STATE_NOT_RUN               0
#define  TN_ST_STATE_RUNNING               1

#define  TN_TASK_START_ON_CREATION         1U
#define  TN_TASK_OS_TICK                0x80U
#define  TN_TASK_IDLE                   0x40U

#define  TN_ID_MASK              0x0FFFFFFFUL
#define  TN_DYN_MASK             0x70000000UL
#define  TN_DYN_CREATED          0x50000000UL


#define  TN_ID_TASK              0x07ABCF69UL
#define  TN_ID_SEMAPHORE         0x0FA173EBUL
#define  TN_ID_EVENT             0x0E224F25UL
#define  TN_ID_DATAQUEUE         0x0C8A6C89UL
#define  TN_ID_FSMEMORYPOOL      0x06B7CE8BUL
#define  TN_ID_MUTEX             0x07129E45UL
#define  TN_ID_MAILBOX           0x04289EBDUL
#define  TN_ID_TIMER             0x0B04D447UL

#define  TN_EXIT_AND_DELETE_TASK          1

   //-- Task states

#define  TSK_STATE_RUNNABLE            0x01U
#define  TSK_STATE_WAIT                0x04U
#define  TSK_STATE_SUSPEND             0x08U
#define  TSK_STATE_WAITSUSP            (TSK_STATE_SUSPEND | TSK_STATE_WAIT)
#define  TSK_STATE_DORMANT             0x10U

#define  TN_TASK_SUSPEND_ON_CREATION   TSK_STATE_SUSPEND
   //--- Waiting

#define  TSK_WAIT_REASON_SLEEP            0x0001
#define  TSK_WAIT_REASON_SEM              0x0002
#define  TSK_WAIT_REASON_EVENT            0x0004
#define  TSK_WAIT_REASON_INUSE_Q          0x0008    // Both dqueue and mailbox ver 3
#define  TSK_WAIT_REASON_FREE_Q           0x0010    // Both dqueue and mailbox ver 3
#define  TSK_WAIT_REASON_MUTEX            0x0020    //-- ver 3
#define  TSK_WAIT_REASON_MUTEX_I          0x0040    //-- ver 3
#define  TSK_WAIT_REASON_WFIXMEM          0x2000

#define  TN_EVENT_ATTR_SINGLE            1U // if no the attr, it will be treated as TN_EVENT_ATTR_MULTI
#define  TN_EVENT_ATTR_MULTI             2U // may be assigned at wait

#define  TN_EVENT_ATTR_CLR               4U  // clear all
#define  TN_EVENT_ATTR_BITCLR            8U   // Clear condition bit only

#define  TN_EVENT_WCOND_OR              16U    // A single any bit set is enough
#define  TN_EVENT_WCOND_AND             32U   // An all bit have to be set for the event rising


#define  TN_MUTEX_NO_ATTR                1U  // Just mutex without priority changing
#define  TN_MUTEX_ATTR_INHERIT           4U

  //-- Errors

#define  TERR_NO_ERR_W                   1  // After waiting(it is used mostly for debugging)
#define  TERR_NO_ERR                     0
#define  TERR_OVERFLOW                 (-1) //-- OOV
#define  TERR_WCONTEXT                 (-2) //-- Wrong context context error
#define  TERR_WSTATE                   (-3) //-- Wrong state   state error
#define  TERR_TIMEOUT                  (-4) //-- Polling failure or timeout
#define  TERR_WPARAM                   (-5)
#define  TERR_UNDERFLOW                (-6)
#define  TERR_OUT_OF_MEM               (-7)
#define  TERR_ILUSE                    (-8) //-- Illegal using
#define  TERR_NOEXS                    (-9) //-- Non-valid or Non-existent object
#define  TERR_DLT                     (-10) //-- Waiting object deleted
#define  TERR_YIELD                   (-11)
#define  TERR_WAIT_COMPLETE           (-12)

#define  NO_TIME_SLICE                   0U
#define  MAX_TIME_SLICE             0xFFFEU


#define  TN_SEND_NORMAL                  0
#define  TN_SEND_URGENT                  1

//-- Circular double-linked list queue - for internal usage

typedef struct _CDLL_QUEUE
{
   struct _CDLL_QUEUE * prev;
   struct _CDLL_QUEUE * next;
}CDLL_QUEUE;

//-- singly-linked list queue - for internal usage

typedef struct _SLL_QUEUE
{
   struct _SLL_QUEUE * next;
}SLL_QUEUE;

typedef struct _SLLBASE 
{
   SLL_QUEUE * head;    //!< first entry in singly-linked list
   SLL_QUEUE * tail;    //!< last entry in singly-linked list
}SLLBASE;

//-- Task Control Block --

typedef void (*task_function)(void *param);

typedef struct _TN_TCB
{
   unsigned int * task_stk;       //-- Pointer to task's top of stack
   CDLL_QUEUE task_queue;         //-- Queue is used to include task in ready/wait lists
   CDLL_QUEUE timer_queue;        //-- Queue is used to include task in timer(timeout,etc.) list
   CDLL_QUEUE * pwait_queue;      //-- Ptr to object's(semaphor,event,etc.) wait list,
                                      // that task has been included for waiting (ver 2.x)
   CDLL_QUEUE create_queue;       //-- Queue is used to include task in create list only

#ifdef USE_MUTEXES

   CDLL_QUEUE mutex_queue;        //-- List of all mutexes that tack locked  (ver 2.x)
   int num_hold_pi_mutexes;       //-- Number of the holded mutexes with priority inheritance
   int num_pi_active_op;          //-- Number of the active priority inheritance operations

#endif

   unsigned int * stk_start;      //-- Base address of task's stack space
   unsigned int stk_size;         //-- Task's stack size (in sizeof(void*),not bytes)
   task_function task_func_addr;  //-- filled on creation  (ver 2.x)
   void * task_func_param;        //-- filled on creation  (ver 2.x)

   unsigned int base_priority;    //-- Task base priority  (ver 2.x)
   unsigned int priority;         //-- Task current priority
   unsigned int restore_priority; //   v 3.0
   unsigned long  id_task;        //-- ID for verification(is it a task or another object?)
                                      // All tasks have the same id_task magic number (ver 2.x)

   unsigned int  task_state;  //-- Task state
   int  task_wait_reason;     //-- Reason for waiting
   int  task_wait_rc;         //-- Waiting return code(reason why waiting  finished)
   unsigned long tick_count;  //-- Remaining time until timeout
   int  tslice_count;         //-- Time slice counter

#ifdef  USE_EVENTFLAGS

   unsigned int ewait_pattern;        //-- Event wait pattern
   unsigned int ewait_mode;           //-- Event wait mode:  _AND or _OR

#endif

   void * data_elem;            //-- Store data entry

//   int  activate_count;       //-- Activation request count - for statistic
//   int  wakeup_count;         //-- Wakeup request count - for statistic
//   int  suspend_count;        //-- Suspension count - for statistic

// Other implementation specific fields may be added below

}TN_TCB;

//----- Semaphore -----

typedef struct _TN_SEM
{
   CDLL_QUEUE  wait_queue;
   unsigned int count;
   unsigned int max_count;
   unsigned long id_sem;     //-- ID for verification(is it a semaphore or another object?)
                         // All semaphores have the same id_sem magic number (ver 2.x)
}TN_SEM;

//----- Event flags -----

typedef struct _TN_EVENTFLAGS
{
   CDLL_QUEUE wait_queue;
 //  unsigned int attr;         //-- Eventflag attribute
   unsigned int pattern;      //-- Initial value of the eventflag bit pattern
   unsigned long id_event;    //-- ID for verification(is it a event or another object?)
                              // All events have the same id_event magic number (ver 2.x)
}TN_EVENTFLAGS;

//----- Data queue -----

typedef struct _TN_DQUEUE
{
   SLLBASE inuse_que;
   SLLBASE free_que;

   CDLL_QUEUE inuse_wait_queue;
   CDLL_QUEUE free_wait_queue;
   unsigned long free_cnt;

   unsigned int num_elem;
   unsigned char * data_buf;
   unsigned long id_dqueue;
}TN_DQUEUE;

#define DQUEUE_ENTRY_SIZE  ((int)(sizeof(SLL_QUEUE) + sizeof(TN_DQUEUE_ELEMENT)))

//------------------ Mailbox -------------------------

typedef struct _TN_MAILBOX
{
   SLLBASE inuse_que;
   SLLBASE free_que;

   CDLL_QUEUE inuse_wait_queue;
   CDLL_QUEUE free_wait_queue;
   unsigned long free_cnt;

   unsigned long num_elem;
   unsigned long elem_size;
   unsigned char * data_buf;

   unsigned long id_mailbox;

}TN_MAILBOX;

#define MAILBOX_ENTRY_SIZE(entry_payload_size)  ((unsigned long)(entry_payload_size) + sizeof(SLL_QUEUE) + sizeof(unsigned long))

// IAR C-STAT MISRA 2012 gives false message here
#define ADD_WITH_SEND_PRI(obj)\
   if(send_priority == TN_SEND_URGENT)\
   { sll_add_head(&(obj)->inuse_que, sque); }\
   else { sll_add_tail(&(obj)->inuse_que, sque);}

//----- Fixed-sized blocks memory pool --------------

typedef struct _TN_FMP
{
   CDLL_QUEUE wait_queue;

   unsigned long block_size;         //-- Actual block size (in bytes)
   unsigned long num_blocks;         //-- Capacity (Fixed-sized blocks actual max qty)
   unsigned char * start_addr;       //-- Memory pool aligned start address
   unsigned char * mem_addr;         //-- Memory pool original start address
   void * free_list;                 //-- Ptr to free block list
   unsigned long fblkcnt;            //-- Num of free blocks
   unsigned long id_fmp;             //-- ID for verification(is it a fixed-sized blocks memory pool or another object?)
                                     //   All Fixed-sized blocks memory pool have the same id_fmp magic number (ver 2.x)
}TN_FMP;

//----- Mutex ------------

typedef struct _TN_MUTEX
{
   CDLL_QUEUE wait_queue;          //-- List of tasks that wait a mutex
   CDLL_QUEUE mutex_queue;         //-- To include in task's locked mutexes list (if any)
//   CDLL_QUEUE lock_mutex_queue;  //-- To include in system's locked mutexes list
   unsigned int attr;              //-- Mutex creation attr - CEILING or INHERIT
   int pi_active;                  //-- Priority inheritance process is active 

   TN_TCB * holder;                //-- Current mutex owner(task that locked mutex)
   int ceil_priority;              //-- When mutex created with CEILING attr
   int cnt;                        //-- Recursive locking counter
   unsigned long id_mutex;         //-- ID for verification(is it a mutex or another object?)
                                       // All mutexes have the same id_mutex magic number (ver 2.x)
}TN_MUTEX;

//----- Timer ------------

struct _TN_TIMER;
typedef void (*timer_func_handler)(struct _TN_TIMER * tmr, void * param);
typedef struct _TN_TIMER
{
   CDLL_QUEUE queue;              // The timer queue entry
   unsigned long period_ticks;    // Periodic action time(if not 0)(in sys ticks)
   unsigned long cnt;             // Timer counter
   void * param;                  // The callback function parameter
   timer_func_handler timer_func; // The callback function that to be executed at the timer action triggered
   unsigned long id_timer;        // The timer object ID
}TN_TIMER;

//------------------ sys alloc -------------------------

typedef struct _MEMHDR 
{
   struct _MEMHDR * next;
   unsigned long size;
}MEMHDR;

typedef struct _MEMINFO
{
   MEMHDR * f_next;
   unsigned long t_free;
   unsigned long buf_start_addr;
   unsigned long buf_last_addr;
   TN_SEM  m_sem;
}MEMINFO;


 //-- Global vars

extern CDLL_QUEUE tn_ready_list[TN_NUM_PRIORITY];   //-- all ready to run(RUNNABLE) tasks
extern CDLL_QUEUE tn_create_queue;                  //-- all created tasks(now - for statictic only)
extern volatile int tn_created_tasks_qty;           //-- num of created tasks

extern volatile int tn_system_state;    //-- System state -(running/not running,etc.)

extern TN_TCB * tn_curr_run_task;       //-- Task that  run now
extern TN_TCB * tn_next_task_to_run;    //-- Task to be run after switch context

extern volatile unsigned int tn_ready_to_run_bmp;
extern volatile unsigned long tn_idle_count;
extern volatile unsigned long tn_sys_tick_count;
//extern volatile unsigned long tn_curr_performance;

static TN_INLINE TN_TCB * get_task_by_create_queue(CDLL_QUEUE * que)
{
   TN_TCB * res = NULL;
   unsigned long offset;
   unsigned long obj_addr;
   if(que != NULL)
   {
      offset = offsetof(TN_TCB, create_queue);
      obj_addr = (unsigned long)que;
      obj_addr -= offset;
      res = (TN_TCB *)obj_addr;
      //res = CONTAINING_RECORD(que, TN_TCB, create_queue);
   }
   return res;
}

static TN_INLINE TN_TCB * get_task_by_tsk_queue(CDLL_QUEUE * que)
{
   TN_TCB * res = NULL;
   unsigned long offset;
   unsigned long obj_addr;
   if(que != NULL)
   {
      offset = offsetof(TN_TCB, task_queue);
      obj_addr = (unsigned long)que;
      obj_addr -= offset;
      res = (TN_TCB *)obj_addr;
     // res = CONTAINING_RECORD(que, TN_TCB, task_queue);
   }
   return res;
}

static TN_INLINE TN_TCB * get_task_by_timer_queue(CDLL_QUEUE * que)
{
   TN_TCB * res = NULL;
   unsigned long offset;
   unsigned long obj_addr;
   if(que != NULL)
   {
      offset = offsetof(TN_TCB, timer_queue);
      obj_addr = (unsigned long)que;
      obj_addr -= offset;
      res = (TN_TCB *)obj_addr;
      //res = CONTAINING_RECORD(que, TN_TCB, timer_queue);
   }
   return res; 
}

static TN_INLINE TN_MUTEX * get_mutex_by_mutex_queque(CDLL_QUEUE * que)
{
   TN_MUTEX * res = NULL;
   unsigned long offset;
   unsigned long obj_addr;
   if(que != NULL)
   {
      offset = offsetof(TN_MUTEX, mutex_queue);
      obj_addr = (unsigned long)que;
      obj_addr -= offset;
      res = (TN_MUTEX *)obj_addr;
      //res = CONTAINING_RECORD(que, TN_MUTEX, mutex_queue);
   }
   return res; 
}

static TN_INLINE TN_MUTEX * get_mutex_by_wait_queque(CDLL_QUEUE * que)
{
   TN_MUTEX * res = NULL;
   unsigned long offset;
   unsigned long obj_addr;
   if(que != NULL)
   {
      offset = offsetof(TN_MUTEX, wait_queue);
      obj_addr = (unsigned long)que;
      obj_addr -= offset;
      res = (TN_MUTEX *)obj_addr;
      //res = CONTAINING_RECORD(que, TN_MUTEX, wait_queue);
   }
   return res; 
}

   //--- User function

int tn_app_init(void);

//----- tn.c ----------------------------------

int tn_start_system(unsigned char * sys_obj_mem,
                    unsigned long sys_obj_mem_size,
                    unsigned int * int_stack_mem);
int tn_sys_tslice_ticks(unsigned int priority, 
                        unsigned int value);
unsigned long tn_get_sys_ticks(void);

//----- tn_sys.c

void tn_update_ready_queue(TN_TCB * task, unsigned int old_priority);
void tn_update_wait_queue(TN_TCB * task);
void tn_update_priority(TN_TCB * task, unsigned int new_priority);
void tn_tick_int_processing(void);

//----- tn_task.c ----------------------------------

typedef void (*idle_task_hook_func)(void *param);

int tn_task_create(TN_TCB * task,             
                 void (*task_func)(void *param),
                 unsigned int priority,
                 unsigned int * task_stack_start,
                 unsigned int task_stack_size,
                 void * param,
                 unsigned int option);
TN_TCB * tn_task_create_dyn(void (*task_func)(void *param),
                            unsigned int priority,
                            unsigned int task_stack_size,
                            void * param,
                            unsigned int option, 
                            int * err);

int tn_task_suspend(TN_TCB * task);
int tn_task_resume(TN_TCB * task);
int tn_task_sleep(unsigned long timeout);
int tn_task_activate(TN_TCB * task);
void tn_task_exit(void);   // v 3.0
int tn_task_delete(TN_TCB * task);
int tn_task_change_priority(TN_TCB * task, unsigned int new_priority);

        //-- Routines

void task_set_dormant_state(TN_TCB* task);
void task_to_non_runnable(TN_TCB * task);
void task_to_runnable(TN_TCB * task);
BOOL task_wait_complete(TN_TCB * task);
void task_curr_to_wait_action(CDLL_QUEUE * wait_que,
                              int wait_reason,
                              unsigned long timeout);
void change_running_task_priority(TN_TCB * task, int new_priority); // v 3.0
void set_current_priority(TN_TCB * task, int priority);
void find_next_task_to_run(void);
void delete_task_resources(TN_TCB * task);

//----- tn_sys.c ----------------------------------

//void tnk_update_priority(TN_TCB * task, unsigned int new_priority);
BOOL tn_os_timers_tick_proc(void);
int tn_release_wait_que(CDLL_QUEUE * wait_que);

//----- tn_sem.c ----------------------------------

int tn_sem_create(TN_SEM * sem, unsigned int start_value, unsigned int max_val);
TN_SEM * tn_sem_create_dyn(unsigned int start_value,
                           unsigned int max_val,
                           int * err);  // [OUT] 
int tn_sem_delete(TN_SEM * sem);
int tn_sem_signal(TN_SEM * sem);
int tn_sem_acquire(TN_SEM * sem, unsigned long timeout);

        //-- Routines

int do_sem_acquire(TN_SEM * sem,
                   int  wait_reason,      // if any  
                   unsigned long timeout);
int do_sem_signal(TN_SEM * sem);


//----- tn_dqueue.c ----------------------------------

int tn_dqueue_create(TN_DQUEUE * dque, 
                     unsigned long num_elem,
                     unsigned char * data_buf,
                     unsigned long data_buf_size);   
TN_DQUEUE * tn_dqueue_create_dyn(unsigned long num_elem,
                                  int * err);   /* [OUT] */      
int tn_dqueue_delete(TN_DQUEUE * dque);
int tn_dqueue_receive(TN_DQUEUE * dque,
                      TN_DQUEUE_ELEMENT * data_ptr,        /* [OUT] */
                      unsigned long timeout);
int tn_dqueue_send_ex(TN_DQUEUE * dque, 
                      TN_DQUEUE_ELEMENT data,
                      unsigned long timeout,
                      int  send_priority); 
unsigned long tn_dqueue_num_free_entries(TN_DQUEUE * dque);
BOOL tn_dqueue_is_empty(TN_DQUEUE * dque);

int do_input_sem(SLLBASE * slist,
                 int nodata_rc,
                 SLL_QUEUE ** entry,
                 CDLL_QUEUE * wait_queue, 
                 int wait_reason,
                 unsigned long timeout);
int do_output_sem(CDLL_QUEUE * wait_que, SLLBASE * slist);

static TN_INLINE int tn_dqueue_send(TN_DQUEUE * dque, 
                                 TN_DQUEUE_ELEMENT data,
                                 unsigned long timeout)
{
   return tn_dqueue_send_ex(dque, data, timeout, 0);
}
//----- tn_event.c -----------------------------------

int tn_eventflags_create(TN_EVENTFLAGS * evf,
                         unsigned int pattern);  //-- Initial value of the eventflag bit pattern
TN_EVENTFLAGS * tn_eventflags_create_dyn(unsigned int pattern,
                                         int * err); // [OUT]      
int tn_eventflags_delete(TN_EVENTFLAGS * evf);
int tn_eventflags_wait(TN_EVENTFLAGS * evf,
                       unsigned int wait_pattern,
                       unsigned int wait_mode,
                       unsigned int * p_flags_pattern,  // [OUT]
                       unsigned long timeout);
int tn_eventflags_set(TN_EVENTFLAGS * evf, unsigned int pattern);
int tn_eventflags_clear(TN_EVENTFLAGS * evf, unsigned int pattern);

//----- tn_mem.c -------------------------------------

int tn_fmem_create(TN_FMP  * fmp,
                     unsigned char * start_addr,
                     unsigned long block_size,
                     unsigned long num_blocks);
TN_FMP * tn_fmem_create_dyn(unsigned long block_size,
                            unsigned long num_blocks,
                            int * err);  /* [OUT] */
int tn_fmem_delete(TN_FMP * fmp);
int tn_fmem_get(TN_FMP * fmp, void ** p_data, unsigned long timeout);
int tn_fmem_release(TN_FMP * fmp, void * p_data);


//----- tn_mutex.c -----------------------------------

int tn_mutex_create(TN_MUTEX * mutex,
                    unsigned int attribute);
TN_MUTEX * tn_mutex_create_dyn(unsigned int attribute,
                               int * err);
int tn_mutex_delete(TN_MUTEX * mutex);
int tn_mutex_lock(TN_MUTEX * mutex, unsigned long timeout);
int tn_mutex_lock_polling(TN_MUTEX * mutex);
int tn_mutex_unlock(TN_MUTEX * mutex);

int do_mutex_unlock(TN_TCB * task,     // The mutex holder    v 3.0
                    TN_MUTEX * mutex); // Mutex to unlock
void do_set_mutex_holder_max_priority(TN_TCB * task);

        //-- Routines

int find_max_blocked_priority(TN_MUTEX * mutex, int ref_priority);
int try_lock_mutex(TN_TCB * task);
int do_unlock_mutex(TN_TCB * task, TN_MUTEX * mutex); // v 3.0
void do_release_mutex_queue(TN_TCB * task);


//----- tn_mailbox.c ---------------------------------

int tn_mailbox_create(TN_MAILBOX * mb, 
                     unsigned long num_elem,
                     unsigned long elem_size,  
                     unsigned char * data_buf,
                     unsigned long data_buf_size);

TN_MAILBOX * tn_mailbox_create_dyn(unsigned long num_elem,
                                  unsigned long elem_size,  
                                  int * err);   // [OUT]   

int tn_mailbox_delete(TN_MAILBOX * mb); 

int tn_mailbox_send_ex(TN_MAILBOX * mb, 
                       void * data_ptr,
                       unsigned long data_size,
                       unsigned long timeout, 
                       int  send_priority); 

long tn_mailbox_receive(TN_MAILBOX * mb,
                        void * data_ptr,        /* [OUT] */
                        unsigned long max_len,
                        unsigned long timeout);
unsigned long tn_mailbox_num_free_entries(TN_MAILBOX * mb);
BOOL tn_mailbox_is_empty(TN_MAILBOX * mb);

static TN_INLINE int tn_mailbox_send(TN_MAILBOX * mb, 
                                 void * data_ptr,
                                 unsigned long data_size,
                                 unsigned long timeout) 
{
   return tn_mailbox_send_ex(mb, data_ptr, data_size, timeout, 0);
}

//----- tn_timer.c ----------------------------------

int tn_timer_create(TN_TIMER * tmr,
                    timer_func_handler timer_func,
                    void * param);
TN_TIMER * tn_timer_create_dyn(timer_func_handler timer_func,
                               void * param,
                               int * err);
int tn_timer_delete(TN_TIMER * tmr);
int tn_timer_set(TN_TIMER * tmr,              // Timer to Start/re-start  
                 unsigned long due_ticks,     // timeout for the first action triggered (in sys ticks)
                 unsigned long period_ticks); // Periodic action time(if not 0)(in sys ticks)
int tn_timer_cancel(TN_TIMER * tmr);          // Timer to Cancel(stop)

BOOL tn_user_timers_tick_proc(void);

//----- tn_alloc.c ----------------------------------

int tn_alloc_init(MEMINFO * mi, unsigned char * buf, unsigned int buf_size);
void * tn_alloc(MEMINFO * mi, unsigned long alloc_size);
int tn_dealloc(MEMINFO * mi, void * p_mem);
void * tn_realloc(MEMINFO * mi, void * p_mem, unsigned long new_size);
unsigned long tn_alloc_get_free_size(MEMINFO * mi);

//----- tn_port.c ------------------------------------

unsigned int * tn_stack_init(void (*task_func)(void *param), 
                             void * stack_start, 
                             void * param);

  //--- tn_user.c ---

void tn_cpu_irq_handler(void);
void tn_cpu_int_enable(void);
BOOL tn_inside_int(void);         //--- Usable for Cortex-M3 only

extern idle_task_hook_func  tn_idle_task_hook_func;
extern TN_SEM tn_sys_tick_sem;
extern volatile int tn_inside_int_cnt;
extern unsigned int tn_func_exit_stack[];
extern unsigned int * tn_func_exit_stack_start;

extern CDLL_QUEUE tn_os_timer_list_gen;
extern CDLL_QUEUE tn_os_timer_list_tick[];
extern CDLL_QUEUE tn_user_timer_list_gen;
extern CDLL_QUEUE tn_user_timer_list_tick[];

extern TN_TCB tn_idle_task;
extern unsigned short tn_tslice_ticks[]; 
extern INTSTACKINFO tn_sp_info;


#if defined USE_DYN_OBJ
extern MEMINFO tn_objects_mem;
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* #ifndef TH_H */





