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

//#include <stdarg.h>

#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"
#include "test.h"

   //----- Test global data

#define   BASE_TEST_TASK_PRIORITY          9
#if defined  TNKERNEL_PORT_MSP430X
  #define   BASE_TEST_TASK_STACK_SIZE      512
#else
  #define   BASE_TEST_TASK_STACK_SIZE      96 //128
#endif

TN_TCB   g_base_test_task;

#if defined (__ICCARM__) || defined (__ICC430__)   // IAR
#pragma data_alignment = 8
__no_init unsigned int g_base_test_task_stack[BASE_TEST_TASK_STACK_SIZE] TN_ALIGN_ATTR_END;
#else
unsigned int g_base_test_task_stack[BASE_TEST_TASK_STACK_SIZE];
#endif

TN_SEM   g_test_rdy_sem;

MEMINFO  g_test_mem;
#if defined (__ICCARM__) || defined (__ICC430__)   // IAR
#pragma data_alignment = 8
__no_init unsigned char g_test_mem_buf[TEST_MEM_POOL_SIZE];
#else
unsigned char g_test_mem_buf[TEST_MEM_POOL_SIZE] TN_ALIGN_ATTR_END;
#endif

TN_EVENTFLAGS  g_test_end_evf;
CURRTEST g_curr_test; 

#if defined TNKERNEL_PORT_MSP430X
#define  TEST_TASK_STACK_SIZE    256 
#else
#define  TEST_TASK_STACK_SIZE    96 //128 
#endif

TN_TCB * task_A = NULL;
TN_TCB * task_B = NULL;
TN_TCB * task_C = NULL;
TN_TCB * task_D = NULL;
TN_TCB * task_E = NULL;

TN_DQUEUE * gp_A_dque;
TN_DQUEUE * gp_B_dque;
TN_DQUEUE * gp_C_dque;

//----------------------------------------------------------------------------
void setup_test_int_timer(unsigned long timeout, // in OS ticks; for MSP430X - max 511
                          int_test_func test_int_func)
{
   g_curr_test.test_int_handler = test_int_func;
   if(g_curr_test.test_int_handler != NULL && timeout != 0UL)
   {

      test_arch_set_int_timer(&g_curr_test, timeout);
   }
}

//----------------------------------------------------------------------------
void do_tests(const test_func main_test_func_arr[])  // g_sem_tests_arr[] =
{
   test_func test_fn;
   int i;
   int rc;

   g_base_test_task.id_task = 0UL;

   for(i=0;;i++)
   {
      test_fn = main_test_func_arr[i];
      if(test_fn == NULL)
      {
       //  TestPrint("test malloc: %lu\r\n",  g_test_mem.memt.size);
         TestPrint("============= END =============\r\n");   
         break;
      }
      else
      {

         rc = tn_alloc_init(&g_test_mem, 
                            g_test_mem_buf, 
                            (unsigned long)TEST_MEM_POOL_SIZE);
         if(rc != TERR_NO_ERR)
         {
            TestPrint("g_test_mem - init failed\r\n");
         }

         // Create base test task. Run immediately

         rc = tn_task_create(&g_base_test_task,           //-- task TCB
                        test_fn,                        //-- task function
                        BASE_TEST_TASK_PRIORITY,          //-- task priority
                        &(g_base_test_task_stack          //-- task stack first addr in memory
                        [BASE_TEST_TASK_STACK_SIZE - 1]),
                        BASE_TEST_TASK_STACK_SIZE,        //-- task stack size (in int,not bytes)
                        (void*)&g_curr_test,              //-- task function parameter
                        TN_TASK_START_ON_CREATION);       //-- Creation option
         if(rc == TERR_NO_ERR)
         {
            // ToDo
         }
         else
         {
            TestPrint("g_base_test_task - creation failed.\r\n");
         }

         //--- Wait signal from the base test task about test end

         rc = tn_sem_acquire(&g_test_rdy_sem, TN_WAIT_INFINITE);
         if(rc >= 0)
         {
           //-- Test finished

           //-- Delete base test task

            tn_task_delete(&g_base_test_task);
            tn_task_sleep(2); 
         }
         else
         {
            // ToDo
         }
      }
   }
}

//----------------------------------------------------------------------------
//  Test End event
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void test_end_signal_create(void)
{
   (void)tn_eventflags_create(&g_test_end_evf,
                              0U);  //-- Initial value of the eventflag bit pattern
}

//----------------------------------------------------------------------------
void test_end_signal_delete(void)
{
   tn_eventflags_delete(&g_test_end_evf);
}

//----------------------------------------------------------------------------
void test_end_do_signal(unsigned int pattern)
{
   tn_eventflags_set(&g_test_end_evf, pattern);
}

//----------------------------------------------------------------------------
int test_end_wait(unsigned int test_end_pattern)
{
   int rc;
   unsigned int  p_flags_pattern;

   rc = tn_eventflags_wait(&g_test_end_evf,
                           test_end_pattern,   // unsigned int wait_pattern,
                           TN_EVENT_WCOND_AND, // all 'wait_pattern' flags must be set  unsigned int wait_mode,
                           &p_flags_pattern,   // [OUT]
                           TN_WAIT_INFINITE);
   return rc;
}

//----------------------------------------------------------------------------
TN_TCB * create_test_task(unsigned int priority, 
                          test_func task_fn)   
{
   TN_TCB * res;
   int err;

   res = tn_task_create_dyn(task_fn,               //-- task function
                            priority,              //-- task priority
                            TEST_TASK_STACK_SIZE,  //-- task stack size (in sizeof(void*),not bytes)
                            (void *)&g_curr_test,  //-- task function parameter
                            0U,                    //-- Creation option
                            &err);                 //-- [OUT] - errorcode
   return res;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

