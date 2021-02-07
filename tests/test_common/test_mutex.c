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

//    A scenarios of the Mutex Tests 3, 4 are derived from the 
//    Giovanni Di Sirio ChibiOS mutex tests

/*

    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

//    A scenarios of the Mutex Tests 5, 6 and 7 are derived from the 
//    Dmitry Frank TNeo mutex tests

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

//#include "io430.h"

#include <string.h>
#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"
#include "test.h"

extern TN_SEM   g_test_rdy_sem;
extern MEMINFO  g_test_mem;
extern TN_EVENTFLAGS  g_test_end_evf;
extern CURRTEST g_curr_test; 

extern TN_TCB * task_A;
extern TN_TCB * task_B;
extern TN_TCB * task_C;
extern TN_TCB * task_D;
extern TN_TCB * task_E;

extern TN_DQUEUE * gp_A_dque;
extern TN_DQUEUE * gp_B_dque;
extern TN_DQUEUE * gp_C_dque;


TN_MUTEX testMutex_A;
TN_MUTEX testMutex_B;
TN_MUTEX  * g_pmutex_A;
TN_MUTEX  * g_pmutex_B;

//==== Mutex Test 1

static void test_base_func_test_mutex_1(void * par);
static void test_task_A_func_test_mutex_1(void * par); 
static void test_task_B_func_test_mutex_1(void * par); 
static void test_task_C_func_test_mutex_1(void * par); 
static  int check_test_mutex_1_result(CURRTEST * ct); 

#define  SBUF_LEN                   16
#define  NUM_CYCLES_MUTEX_TEST_1     5

//==== Mutex Test 2

static void test_base_func_test_mutex_2(void * par);
static void test_task_A_func_test_mutex_2(void * par);
static void test_task_B_func_test_mutex_2(void * par);
static void test_task_C_func_test_mutex_2(void * par);
static  int check_test_mutex_2_result(CURRTEST * ct); 

#define  NUM_CYCLES_MUTEX_TEST_2     5

//==== Mutex Test 3

static void test_base_func_test_mutex_3(void * par);
static void test_task_A_func_test_mutex_3(void * par);
static void test_task_B_func_test_mutex_3(void * par);
static void test_task_C_func_test_mutex_3(void * par);
static  int check_test_mutex_3_result(CURRTEST * ct); 

#define  TEST3_START_IDX    0
#define  TEST3_A1_IDX       1
#define  TEST3_A2_IDX       2
#define  TEST3_A3_IDX       3
#define  TEST3_A4_IDX       4
#define  TEST3_B1_IDX       5
#define  TEST3_B2_IDX       6
#define  TEST3_C1_IDX       7
#define  TEST3_C2_IDX       8
#define  TEST3_C3_IDX       9
#define  TEST3_C4_IDX      10
#define  TEST3_MAX_IDX     11

//==== Mutex Test 4

static void test_base_func_test_mutex_4(void * par);
static void test_task_A_func_test_mutex_4(void * par);
static void test_task_B_func_test_mutex_4(void * par);
static void test_task_C_func_test_mutex_4(void * par);
static void test_task_D_func_test_mutex_4(void * par);
static void test_task_E_func_test_mutex_4(void * par);
static  int check_test_mutex_4_result(CURRTEST * ct); 

#define  TEST4_START_IDX    0
#define  TEST4_A1_IDX       1
#define  TEST4_A2_IDX       2
#define  TEST4_A3_IDX       3
#define  TEST4_A4_IDX       4
#define  TEST4_B1_IDX       5
#define  TEST4_B2_IDX       6
#define  TEST4_C1_IDX       7
#define  TEST4_C2_IDX       8
#define  TEST4_C3_IDX       9
#define  TEST4_D1_IDX      10
#define  TEST4_D2_IDX      11
#define  TEST4_D3_IDX      12
#define  TEST4_D4_IDX      13
#define  TEST4_D5_IDX      14
#define  TEST4_D6_IDX      15
#define  TEST4_D7_IDX      16
#define  TEST4_E1_IDX      17
#define  TEST4_E2_IDX      18
#define  TEST4_E3_IDX      19
#define  TEST4_MAX_IDX     20

//==== Mutex Test 5

static void test_base_func_test_mutex_5(void * par);
static void test_task_A_func_test_mutex_5(void * par);
static void test_task_B_func_test_mutex_5(void * par);
static void test_task_C_func_test_mutex_5(void * par);
static  int check_test_mutex_5_result(CURRTEST * ct); 

#define  TEST_MUTEX_A_1        10
#define  TEST_MUTEX_A_2        11

#define  TEST_MUTEX_B_1        20 
#define  TEST_MUTEX_B_2        21 

#define  TEST_MUTEX_C_1        30
#define  TEST_MUTEX_C_2        31

#define  TEST_MUTEX_A_EXIT    100
#define  TEST_MUTEX_B_EXIT    200
#define  TEST_MUTEX_C_EXIT    300

//==== Mutex Test 6

static void test_base_func_test_mutex_6(void * par);
static void test_task_A_func_test_mutex_6(void * par);
static void test_task_B_func_test_mutex_6(void * par);
static  int check_test_mutex_6_result(CURRTEST * ct); 

//==== Mutex Test 7

static void test_base_func_test_mutex_7(void * par);
static void test_task_A_func_test_mutex_7(void * par);
static void test_task_B_func_test_mutex_7(void * par);
static  int check_test_mutex_7_result(CURRTEST * ct); 

//==== Mutex Test 8

static void test_base_func_test_mutex_8(void * par);
static  int check_test_mutex_8_result(CURRTEST * ct); 

//==== Mutex Test 9

static void test_base_func_test_mutex_9(void * par);
static int int_timer_func_test_mutex_9(void * par);
static  int check_test_mutex_9_result(CURRTEST * ct); 

//================ All mutex tests to run ============

const test_func g_mutex_tests_arr[] =
{
   test_base_func_test_mutex_1,
   test_base_func_test_mutex_2,
   test_base_func_test_mutex_3,
   test_base_func_test_mutex_4,
   test_base_func_test_mutex_5,
   test_base_func_test_mutex_6,
   test_base_func_test_mutex_7,
   test_base_func_test_mutex_8,
   test_base_func_test_mutex_9,

   NULL // Terminator
};

//=============== Mutex Test 1 =======================

static void test_base_func_test_mutex_1(void * par)
{
   int rc;

   //--- Create test data

   TestPrint("Mutex Test 1 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_1);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_mutex_1); 
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_mutex_1); 
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_NO_ATTR);
   if(rc < 0)
      TestPrint("testMutex_A - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_mutex_delete(&testMutex_A);
   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 1 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

//----------------------------------------------------------------------------
static void test_task_A_func_test_mutex_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "a%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_A_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_B_func_test_mutex_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "b%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_B_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_C_func_test_mutex_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "c%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_C_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static int check_test_mutex_1_result(CURRTEST * ct)
{
   const char ref_str[] = "a0a1a2a3a4b0b1b2b3b4c0c1c2c3c4";

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      TestPrint("Mutex Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 1 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 

//=============== Mutex Test 2 =======================

static void test_base_func_test_mutex_2(void * par)
{
   int rc;

   //--- Create test data

   TestPrint("Mutex Test 2 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_2);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_mutex_2); 
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_mutex_2); 
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
      TestPrint("testMutex_A - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_mutex_delete(&testMutex_A);
   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 2 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

//----------------------------------------------------------------------------
static void test_task_A_func_test_mutex_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "a%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_A_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_B_func_test_mutex_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "b%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_B_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_C_func_test_mutex_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_MUTEX_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "c%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_mutex_unlock(&testMutex_A);

   test_end_do_signal(TASK_C_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static int check_test_mutex_2_result(CURRTEST * ct)
{
   const char ref_str[] = "a0a1a2a3a4b0b1b2b3b4c0c1c2c3c4";

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      TestPrint("Mutex Test 2 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 2 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 


//==== Mutex Test 3  =========================================================

static void test_base_func_test_mutex_3(void * par)
{
/*
 *  Priority inheritance, simple case
 *
 * Three threads are involved in the classic priority inversion scenario, a
 * medium priority thread tries to starve an high priority thread by
 * blocking a low priority thread into a mutex lock zone.
 * The test expects the threads to reach their goal in increasing priority
 * order by rearranging their priorities in order to avoid the priority
 * inversion trap.
 *
 * Scenario
 * This weird looking diagram should explain what happens in the test case:
 * 
 * Time ----> 0     10    20    30    40    50    60    70    80    90    100
 *  Task_C ...AL++++++++++............2+++++++++++AU0---------------++++++G...
 *  Task_B ...............++++++++++++------------------++++++++++++G.........
 *  Task_A ...........................AL..........++++++AUG...................
 *                                    ^           ^
 * Legend:
 *   
 *   +++  - Running
 *   ---  - Ready
 *   ...  - Waiting or Terminated
 *   xL   - Lock operation on mutex 'x'
 *   xUn  - Unlock operation on mutex 'x' with priority returning to level 'n'
 *   G    - Goal
 *   ^    - Priority transition (boost or return).
*/
   int rc;
   unsigned long * arr;

   //--- Create test data

   TestPrint("Mutex Test 3 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   g_curr_test.test_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 
                                     TEST3_MAX_IDX * sizeof(unsigned char *));  
   arr = (unsigned long *)g_curr_test.test_data_1;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_3); // Hi
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_mutex_3); // Medium
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_mutex_3); // Lo
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
      TestPrint("testMutex_A - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

    //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   arr[TEST3_START_IDX] = tn_get_sys_ticks();

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_3_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_mutex_delete(&testMutex_A);
   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 3 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mutex_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST3_A1_IDX] = tn_get_sys_ticks();

   tn_task_sleep(40);

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   arr[TEST3_A2_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST3_A3_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_A);

   arr[TEST3_A4_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "A");

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_mutex_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_task_sleep(20);

   arr[TEST3_B1_IDX] = tn_get_sys_ticks();

   long_job_delay(40);

   arr[TEST3_B2_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "B");

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_C_func_test_mutex_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   arr[TEST3_C1_IDX] = tn_get_sys_ticks();

   long_job_delay(40);

   arr[TEST3_C2_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_A);

   arr[TEST3_C3_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST3_C4_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "C");

//   TestPrint("Task_C priority: %u.\r\n",task_C->priority);

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_mutex_3_result(CURRTEST * ct)
{
   const char ref_str[] = "ABC";
   unsigned long * arr = (unsigned long *)g_curr_test.test_data_1;
   int fOK = FALSE;

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      if(tick_val_inside(arr[TEST3_A2_IDX] - arr[TEST3_START_IDX], 60UL, 63UL) == TRUE)
      {
         if(tick_val_inside(arr[TEST3_A3_IDX] - arr[TEST3_START_IDX], 70UL, 73UL) == TRUE)
         {
            if(tick_val_inside(arr[TEST3_B1_IDX] - arr[TEST3_START_IDX], 20UL, 23UL) == TRUE)
            {
               if(tick_val_inside(arr[TEST3_B2_IDX] - arr[TEST3_START_IDX], 90UL, 93UL) == TRUE)
               {
                  if(tick_val_inside(arr[TEST3_C2_IDX] - arr[TEST3_START_IDX], 60UL, 62UL) == TRUE)
                  {
                     if(tick_val_inside(arr[TEST3_C3_IDX] - arr[TEST3_START_IDX], 90UL, 102UL) == TRUE)
                     {
                        if(tick_val_inside(arr[TEST3_C4_IDX] - arr[TEST3_START_IDX], 99UL, 102UL) == TRUE)
                        {
                           fOK = TRUE;     
                        }
                     }
                  }
               }
            }
         }
      }       
   }
#if 0
TestPrint(" %lu\r\n", arr[TEST3_A2_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_A3_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_B1_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_B2_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_C2_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_C3_IDX] - arr[TEST3_START_IDX]);
TestPrint(" %lu\r\n", arr[TEST3_C4_IDX] - arr[TEST3_START_IDX]);
#endif

   if(fOK == TRUE)
   {
      TestPrint("Mutex Test 3 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 3 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 

//==== Mutex Test 4  =========================================================

static void test_base_func_test_mutex_4(void * par)
{
/*
 * Priority inheritance, complex case
 * 
 * Five threads are involved in the complex priority inversion scenario,
 * please refer to the diagram below for the complete scenario.
 * The test expects the threads to perform their operations in increasing
 * priority order by rearranging their priorities in order to avoid the
 * priority inversion trap.
 *
 * Scenario
 * This weird looking diagram should explain what happens in the test case:
 * 
 * Time ----> 0     10    20    30    40    50    60    70    80    90    100   110
 *  Task_E ...BL++++------------2+++++------4+++++BU0---------------------------G.....
 *  Task_D .........AL++++2+++++BL----------4-----++++++BU4+++AU1---------------G.....
 *  Task_C ...............AL----------------------------------------------++++++AUG...
 *  Task_B ...........................+++++++-----------------------++++++G...........
 *  Task_A .................................AL................++++++AUG...............
 *                        ^     ^           ^     ^     ^     ^
 * Legend:
 *   
 *   +++  - Running
 *   ---  - Ready
 *   ...  - Waiting or Terminated
 *   xL   - Lock operation on mutex 'x'
 *   xUn  - Unlock operation on mutex 'x' with priority returning to level 'n'
 *   ^    - Priority transition (boost or return).
 */
   int rc;
   unsigned long * arr;

   //--- Create test data

   TestPrint("Mutex Test 4 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   g_curr_test.test_data_1 = (unsigned char *)tn_alloc(&g_test_mem, TEST4_MAX_IDX * sizeof(unsigned char *));  
   arr = (unsigned long *)g_curr_test.test_data_1;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_4); // Hi
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_mutex_4); // Medium
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_mutex_4); // Lo
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   task_D = create_test_task(13, test_task_D_func_test_mutex_4); // Lo
   if(task_D == NULL)
      TestPrint("task_D == NULL\r\n");

   task_E = create_test_task(14, test_task_E_func_test_mutex_4); // Lo
   if(task_E == NULL)
      TestPrint("task_E == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
      TestPrint("testMutex_A - creation failed.\r\n");
   rc = tn_mutex_create(&testMutex_B, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
      TestPrint("testMutex_B - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);
   tn_task_activate(task_D);
   tn_task_activate(task_E);

   arr[TEST4_START_IDX] = tn_get_sys_ticks();

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG |
                      TASK_D_RDY_FLAG | TASK_E_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_4_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B,C,D,E already finished and auto released resources

   tn_mutex_delete(&testMutex_A);
   tn_mutex_delete(&testMutex_B);
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 4 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mutex_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST4_A1_IDX] = tn_get_sys_ticks();

   tn_task_sleep(50);

   tn_mutex_lock(&testMutex_B, TN_WAIT_INFINITE);

   arr[TEST4_A2_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST4_A3_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_B);

   arr[TEST4_A4_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "A");

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_mutex_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_task_sleep(40);

   arr[TEST4_B1_IDX] = tn_get_sys_ticks();

   long_job_delay(20);

   arr[TEST4_B2_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "B");

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_C_func_test_mutex_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_task_sleep(20);

   tn_mutex_lock(&testMutex_B, TN_WAIT_INFINITE);

   arr[TEST4_C1_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST4_C2_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_B);

   arr[TEST4_C3_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "C");

//   TestPrint("Task_C priority: %u.\r\n",task_C->priority);

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_D_func_test_mutex_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_task_sleep(10);

   tn_mutex_lock(&testMutex_B, TN_WAIT_INFINITE);

   arr[TEST4_D1_IDX] = tn_get_sys_ticks();

   long_job_delay(20);

   arr[TEST4_D2_IDX] = tn_get_sys_ticks();

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   arr[TEST4_D3_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST4_D4_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_A);

   arr[TEST4_D5_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST4_D6_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_B);

   arr[TEST4_D7_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "D");

//   TestPrint("Task_C priority: %u.\r\n",task_C->priority);

   test_end_do_signal(TASK_D_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_E_func_test_mutex_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);

   arr[TEST4_E1_IDX] = tn_get_sys_ticks();

   long_job_delay(30);

   arr[TEST4_E2_IDX] = tn_get_sys_ticks();

   tn_mutex_unlock(&testMutex_A);

   arr[TEST4_E3_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "E");

//   TestPrint("Task_C priority: %u.\r\n",task_C->priority);

   test_end_do_signal(TASK_E_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_mutex_4_result(CURRTEST * ct)
{
   const char ref_str[] = "ABCDE";
   unsigned long * arr = (unsigned long *)g_curr_test.test_data_1;
   int fOK = FALSE;

/*
   TestPrint("TEST4_A1_IDX: %lu TEST4_A2_IDX: %lu TEST4_A3_IDX: %lu TEST4_A4_IDX: %lu\r\n",
               arr[TEST4_A1_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_A2_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_A3_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_A4_IDX] - arr[TEST4_START_IDX]);   

   TestPrint("TEST4_B1_IDX: %lu TEST4_B2_IDX: %lu\r\n",
               arr[TEST4_B1_IDX] - arr[TEST4_START_IDX],              
               arr[TEST4_B2_IDX] - arr[TEST4_START_IDX]);   

   TestPrint("TEST4_D1_IDX: %lu TEST4_D2_IDX: %lu TEST4_D3_IDX: %lu TEST4_D4_IDX: %lu\r\n",
               arr[TEST4_D1_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_D2_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_D3_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_D4_IDX] - arr[TEST4_START_IDX]);   

   TestPrint("TEST4_D5_IDX: %lu TEST4_D6_IDX: %lu TEST4_D7_IDX: %lu\r\n",
               arr[TEST4_D5_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_D6_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_D7_IDX] - arr[TEST4_START_IDX]);   

   TestPrint("TEST4_E1_IDX: %lu TEST4_E2_IDX: %lu TEST4_E3_IDX: %lu\r\n",
               arr[TEST4_E1_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_E2_IDX] - arr[TEST4_START_IDX],   
               arr[TEST4_E3_IDX] - arr[TEST4_START_IDX]);   
*/
   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      if(tick_val_inside(arr[TEST4_A2_IDX] - arr[TEST4_START_IDX], 80UL, 83UL) == TRUE &&
         tick_val_inside(arr[TEST4_A3_IDX] - arr[TEST4_START_IDX], 90UL, 93UL) == TRUE &&
           tick_val_inside(arr[TEST4_A4_IDX] - arr[TEST4_START_IDX], 90UL, 93UL) == TRUE)
      {
         if(tick_val_inside(arr[TEST4_B1_IDX] - arr[TEST4_START_IDX], 40UL, 43UL) == TRUE &&
            tick_val_inside(arr[TEST4_B2_IDX] - arr[TEST4_START_IDX], 100UL, 103UL) == TRUE)  
         {
            if(tick_val_inside(arr[TEST4_D1_IDX] - arr[TEST4_START_IDX], 10UL, 13UL) == TRUE &&
               tick_val_inside(arr[TEST4_D2_IDX] - arr[TEST4_START_IDX], 30UL, 33UL) == TRUE &&
                tick_val_inside(arr[TEST4_D3_IDX] - arr[TEST4_START_IDX], 60UL, 63UL) == TRUE &&
                 tick_val_inside(arr[TEST4_D4_IDX] - arr[TEST4_START_IDX], 70UL, 73UL) == TRUE &&
                  tick_val_inside(arr[TEST4_D5_IDX] - arr[TEST4_START_IDX], 70UL, 73UL) == TRUE &&
                   tick_val_inside(arr[TEST4_D6_IDX] - arr[TEST4_START_IDX], 80UL, 83UL) == TRUE &&
                    tick_val_inside(arr[TEST4_D7_IDX] - arr[TEST4_START_IDX], 110UL, 113UL) == TRUE)
            {
               if(tick_val_inside(arr[TEST4_E2_IDX] - arr[TEST4_START_IDX], 60UL, 93UL) == TRUE &&
                  tick_val_inside(arr[TEST4_E3_IDX] - arr[TEST4_START_IDX], 110UL, 113UL) == TRUE)
               {
                  fOK = TRUE;     
               }
            }
         }
      }       
   }
   if(fOK == TRUE)
   {
      TestPrint("Mutex Test 4 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 4 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
}


//==== Mutex Test 5 ============================================================

static void test_base_func_test_mutex_5(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;

   TestPrint("Mutex Test 5 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   g_curr_test.test_data_1 = tn_alloc(&g_test_mem, 3 * sizeof(void*));
   if(g_curr_test.test_data_1 == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_B_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_B_dque == NULL)
   {
      TestPrint("gp_B_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_C_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_C_dque == NULL)
   {
      TestPrint("gp_C_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_5);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   task_B = create_test_task(11, test_task_B_func_test_mutex_5);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }

   task_C = create_test_task(12, test_task_C_func_test_mutex_5);
   if(task_C == NULL)
   {
      TestPrint("task_C == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mutex)
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
   {
      TestPrint("testMutex_A - creation failed.\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

//--- Base task test operating

//C locks M1
   tx_data = (unsigned long)TEST_MUTEX_C_1;
   tn_dqueue_send(gp_C_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(testMutex_A.holder == task_C)
      g_curr_test.rc_base++; 
//B tries to lock M1 -> B blocks, C has priority of B
   tx_data = (unsigned long)TEST_MUTEX_B_1;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(task_C->priority == task_B->priority)
      g_curr_test.rc_base++; 
//A tries to lock M1 -> B blocks, C has priority of A"
   tx_data = (unsigned long)TEST_MUTEX_A_1;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(task_C->priority == task_A->priority)
      g_curr_test.rc_base++; 
//C deleted M1 -> B and A become runnable and have retval TN_RC_DELETED, C has its base priority");
   tx_data = (unsigned long)TEST_MUTEX_C_2;
   tn_dqueue_send(gp_C_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(testMutex_A.holder == NULL)
      g_curr_test.rc_base++; 
   if(testMutex_A.id_mutex == 0)
      g_curr_test.rc_base++; 
   if(task_B->priority == 11)
      g_curr_test.rc_base++; 
   if(task_C->priority == 12)
      g_curr_test.rc_base++; 

//--- Signal to task A, B, C to terminate

   tx_data = (unsigned long)TEST_MUTEX_A_EXIT;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tx_data = (unsigned long)TEST_MUTEX_B_EXIT;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tx_data = (unsigned long)TEST_MUTEX_C_EXIT;
   tn_dqueue_send(gp_C_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_5_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B, C already finished

exit_l:

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   tn_dqueue_delete(gp_A_dque);
   tn_dqueue_delete(gp_B_dque);
   tn_dqueue_delete(gp_C_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 5 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mutex_5(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_A_1)
         {
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc == TERR_DLT)
              ct->rc_A++;
         }
         else if(val == TEST_MUTEX_A_2)
         {
         }
         else if(val == TEST_MUTEX_A_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("A - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_mutex_5(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_B_dque,
                            (TN_DQUEUE_ELEMENT *)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_B_1)
         {
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc == TERR_DLT)
              ct->rc_B++;
         }
         else if(val == TEST_MUTEX_B_2)
         {
         }
         else if(val == TEST_MUTEX_B_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("B - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_C_func_test_mutex_5(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_C = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_C_dque,
                            (TN_DQUEUE_ELEMENT *)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_C_1)
         {
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc >= 0)
              ct->rc_C++;
         }
         else if(val == TEST_MUTEX_C_2)
         {
            rc = tn_mutex_delete(&testMutex_A);
            if(rc >= 0)
              ct->rc_C++;
         }
         else if(val == TEST_MUTEX_C_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("C - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
} 

static  int check_test_mutex_5_result(CURRTEST * ct)
{
   if(ct->rc_base == 7 && ct->rc_A == 1 && ct->rc_B == 1 && ct->rc_C == 2)
   {
      TestPrint("Mutex Test 5 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 5 failed. Base: %d A: %d  B: %d  C: %d\r\n", 
                                ct->rc_base, ct->rc_A, ct->rc_B, ct->rc_C);
   }

   return 0;
} 

//==== Mutex Test 6 ============================================================

static void test_base_func_test_mutex_6(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;

   TestPrint("Mutex Test 6 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   g_curr_test.test_data_1 = tn_alloc(&g_test_mem, 3 * sizeof(void*));
   if(g_curr_test.test_data_1 == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_B_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_B_dque == NULL)
   {
      TestPrint("gp_B_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_6);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   task_B = create_test_task(11, test_task_B_func_test_mutex_6);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mutex)
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
   {
      TestPrint("testMutex_A - creation failed.\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

//--- Base task test operating


//B locks M1
   tx_data = (unsigned long)TEST_MUTEX_B_1;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(testMutex_A.holder == task_B)
      g_curr_test.rc_base++; 
//A tries to lock M1 -> A blocks, B has priority of A
   tx_data = (unsigned long)TEST_MUTEX_A_1;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(task_B->priority == task_A->priority)
      g_curr_test.rc_base++; 
//B exits -> M1 gets unlocked and A locks it
   tx_data = (unsigned long)TEST_MUTEX_B_EXIT;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(5);
   //--Check
   if(testMutex_A.holder == task_A)
      g_curr_test.rc_base++; 
//A unlocks M1
   tx_data = (unsigned long)TEST_MUTEX_A_2;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(testMutex_A.holder == NULL)
      g_curr_test.rc_base++; 

//--- Signal to task A to terminate

   tx_data = (unsigned long)TEST_MUTEX_A_EXIT;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
//   tx_data = (unsigned long)TEST_MUTEX_B_EXIT;
//   tn_dqueue_send(gp_B_dque, (void *)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_6_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B, already finished

exit_l:

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   tn_dqueue_delete(gp_A_dque);
   tn_dqueue_delete(gp_B_dque);

   tn_mutex_delete(&testMutex_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 6 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mutex_6(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_A_1)
         {
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc == TERR_NO_ERR_W)
              ct->rc_A++;
         }
         else if(val == TEST_MUTEX_A_2)
         {
            rc = tn_mutex_unlock(&testMutex_A);
            if(rc == TERR_NO_ERR)
              ct->rc_A++;
         }
         else if(val == TEST_MUTEX_A_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("A - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_mutex_6(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_B_dque,
                            (TN_DQUEUE_ELEMENT *)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_B_1)
         {
//B locks M1
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc >= 0) //TERR_DLT)
              ct->rc_B++;
         }
         else if(val == TEST_MUTEX_B_2)
         {
         }
         else if(val == TEST_MUTEX_B_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("B - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

static  int check_test_mutex_6_result(CURRTEST * ct)
{
   if(ct->rc_base == 4 && ct->rc_A == 2 && ct->rc_B == 1)
   {
      TestPrint("Mutex Test 6 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 6 failed. Base: %d A: %d  B: %d \r\n", 
                                ct->rc_base, ct->rc_A, ct->rc_B);
   }

   return 0;
} 

//==== Mutex Test 7 ==========================================================

static void test_base_func_test_mutex_7(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;

   TestPrint("Mutex Test 7 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   g_curr_test.test_data_1 = tn_alloc(&g_test_mem, 3 * sizeof(void*));
   if(g_curr_test.test_data_1 == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_B_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_B_dque == NULL)
   {
      TestPrint("gp_B_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mutex_7);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   task_B = create_test_task(11, test_task_B_func_test_mutex_7);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mutex)
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc < 0)
   {
      TestPrint("testMutex_A - creation failed.\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

//--- Base task test operating


//B locks M1
   tx_data = (unsigned long)TEST_MUTEX_B_1;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(testMutex_A.holder == task_B)
      g_curr_test.rc_base++; 
//A tries to lock M1 -> A blocks, B raised priority
   tx_data = (unsigned long)TEST_MUTEX_A_1;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   //--Check
   if(task_B->priority == task_A->priority)
      g_curr_test.rc_base++; 

//A exits ->  B has a native priority

   tn_task_delete(task_A);

//   tx_data = (unsigned long)TEST_MUTEX_A_EXIT;
//   tn_dqueue_send(gp_A_dque, (void *)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(10);
   //--Check
   if(testMutex_A.holder == task_B)
      g_curr_test.rc_base++; 
   if(task_B->priority == 11)
      g_curr_test.rc_base++; 

//B exits -> M1 gets unlocked

   tx_data = (unsigned long)TEST_MUTEX_B_EXIT;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(5);
   //--Check
   if(testMutex_A.holder == NULL)
      g_curr_test.rc_base++; 

   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_7_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B, already finished

exit_l:

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   tn_dqueue_delete(gp_A_dque);
   tn_dqueue_delete(gp_B_dque);

   tn_mutex_delete(&testMutex_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 7 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mutex_7(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   //int i;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_A_1)
         {
            // Never returns in this test 
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE); 
            if(rc == TERR_NO_ERR_W)
            { 
            }
         }
         else if(val == TEST_MUTEX_A_2)
         {
         }
         else if(val == TEST_MUTEX_A_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("A - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_mutex_7(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   //int i;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_B_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_MUTEX_B_1)
         {
            rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
            if(rc == TERR_NO_ERR) //TERR_DLT)
              ct->rc_B++;
         }
         else if(val == TEST_MUTEX_B_2)
         {
         }
         else if(val == TEST_MUTEX_B_EXIT)
         {
            break;
         }
         else
         {
            TestPrint("B - unknown msg\r\n");   
         }  
      }
   }

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

static  int check_test_mutex_7_result(CURRTEST * ct)
{
   if(ct->rc_base == 5 && ct->rc_A == 0 && ct->rc_B == 1)
   {
      TestPrint("Mutex Test 7 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 7 failed. Base: %d A: %d  B: %d \r\n", 
                                ct->rc_base, ct->rc_A, ct->rc_B);
   }

   return 0;
} 

//==== Mutex Test 8 ==========================================================

static void test_base_func_test_mutex_8(void * par)
{
   int rc;
   int err;
   //unsigned long tx_data;

   TestPrint("Mutex Test 8 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   //--- Create end test signal event

   //--- Start test


//--- Base task test operating

// Create static
   testMutex_A.id_mutex = 0UL;
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc == TERR_NO_ERR)
   {
      //-- Check

      g_curr_test.rc_base++;      
   }
// Delete
   rc = tn_mutex_delete(&testMutex_A);
   if(rc == TERR_NO_ERR)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create static
  // testMutex_A.id_mutex = 0UL;
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_NO_ATTR);
   if(rc == TERR_NO_ERR)
   {
      //-- Check

      g_curr_test.rc_base++;      
   }
// Delete
   rc = tn_mutex_delete(&testMutex_A);
   if(rc == TERR_NO_ERR)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create dyn
   g_pmutex_A = tn_mutex_create_dyn(TN_MUTEX_ATTR_INHERIT, &err);
   if(g_pmutex_A != NULL)
   {
      //-- Check
      if(err == TERR_NO_ERR)
         g_curr_test.rc_base++;      
   }

// Delete
   rc = tn_mutex_delete(g_pmutex_A);
   if(rc == TERR_NO_ERR)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create dyn
   g_pmutex_A = tn_mutex_create_dyn(TN_MUTEX_NO_ATTR, &err);
   if(g_pmutex_A != NULL)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Delete
   rc = tn_mutex_delete(g_pmutex_A);
   if(rc == TERR_NO_ERR)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Delete unexisting
   rc = tn_mutex_delete(g_pmutex_A);
   if(rc == TERR_NOEXS)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create dyn - bad param
   g_pmutex_A = tn_mutex_create_dyn(TN_MUTEX_NO_ATTR | TN_MUTEX_ATTR_INHERIT, &err);
   if(g_pmutex_A == NULL)
   {
      //-- Check
      if(err == TERR_WPARAM)
         g_curr_test.rc_base++;      
   }

// Delete unexisting

   rc = tn_mutex_delete(g_pmutex_A);
   if(rc == TERR_WPARAM)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create stat - bad param

  //-- Check
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_NO_ATTR | TN_MUTEX_ATTR_INHERIT);
   if(rc == TERR_WPARAM)
   {
      //-- Check

      g_curr_test.rc_base++;      
   }

// Create stat - bad param

  //-- Check
   rc = tn_mutex_create(NULL, TN_MUTEX_ATTR_INHERIT);
   if(rc == TERR_WPARAM)
   {
      //-- Check

      g_curr_test.rc_base++;      
   }


// Delete - bad param

   rc = tn_mutex_delete(NULL);
   if(rc == TERR_WPARAM)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Create static
  
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_NO_ATTR);
   if(rc == TERR_NO_ERR)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// Recreate

   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_NO_ATTR);
   if(rc == TERR_WPARAM)
   {
      //-- Check
      g_curr_test.rc_base++;      
   }

// lock/unlock

   rc = tn_mutex_lock(&testMutex_A, TN_NO_WAIT);
   if(rc == TERR_NO_ERR)
   {  //-- Check
      g_curr_test.rc_base++;      
   }
   rc = tn_mutex_lock(&testMutex_A, TN_NO_WAIT);
   if(rc == TERR_ILUSE)
   {  //-- Check
      g_curr_test.rc_base++;      
   }

   rc = tn_mutex_unlock(&testMutex_A);
   if(rc == TERR_NO_ERR)
   {  //-- Check
      g_curr_test.rc_base++;      
   }
   rc = tn_mutex_unlock(&testMutex_A);
   if(rc == TERR_ILUSE)
   {  //-- Check
      g_curr_test.rc_base++;      
   }

// lock/unlock - bad param, unexisting

   rc = tn_mutex_lock(&testMutex_A, 1);
   if(rc == TERR_NO_ERR)
   {  //-- Check
      g_curr_test.rc_base++;      
   }

   rc = tn_mutex_unlock((TN_MUTEX*)((void*)task_A));
   if(rc == TERR_NOEXS)
   {  //-- Check
      g_curr_test.rc_base++;      
   }

   rc = tn_mutex_unlock(NULL);
   if(rc == TERR_WPARAM)
   {  //-- Check
      g_curr_test.rc_base++;      
   }

   check_test_mutex_8_result(&g_curr_test);  //--- Check test results

   //--- Clear resources
      //-- Tasks A,B, already finished

//exit_l:

   tn_mutex_delete(&testMutex_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 8 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static  int check_test_mutex_8_result(CURRTEST * ct)
{
   if(ct->rc_base == 23)
   {
      TestPrint("Mutex Test 8 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 8 failed. Base: %d\r\n", ct->rc_base);
   }

   return 0;
} 

//==== Mutex Test 9 ==========================================================

static void test_base_func_test_mutex_9(void * par)
{
   int rc;
   int err;

   TestPrint("Mutex Test 9 - start.\r\n");

   testMutex_A.id_mutex = 0UL;
   rc = tn_mutex_create(&testMutex_A, TN_MUTEX_ATTR_INHERIT);
   if(rc != TERR_NO_ERR)
   {
   }
   g_pmutex_A = tn_mutex_create_dyn(TN_MUTEX_ATTR_INHERIT, &err);
   if(g_pmutex_A == NULL)
   {
   }
   rc = tn_mutex_lock(g_pmutex_A,TN_WAIT_INFINITE);
   if(rc != TERR_NO_ERR)
   {
   }

  //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(10,      // unsigned long timeout, 
                        int_timer_func_test_mutex_9);

   //--- Start test

   //--- Wait until all test participants finished
   rc = test_end_wait(INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mutex_9_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources

   tn_mutex_delete(&testMutex_A);
   tn_mutex_delete(g_pmutex_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mutex Test 9 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static int int_timer_func_test_mutex_9(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   int err;

   ct->rc_int = 0;

   rc = tn_mutex_create(&testMutex_B, TN_MUTEX_ATTR_INHERIT);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   rc = tn_mutex_delete(&testMutex_A);
   if(rc == TERR_WCONTEXT) 
   {
      ct->rc_int++;
   }
   g_pmutex_B = tn_mutex_create_dyn(TN_MUTEX_ATTR_INHERIT, &err);
   if(err == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   rc = tn_mutex_lock(&testMutex_A, TN_NO_WAIT);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   rc = tn_mutex_lock(&testMutex_A, 1);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   rc = tn_mutex_lock(&testMutex_A, TN_WAIT_INFINITE);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   rc = tn_mutex_unlock(g_pmutex_A);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }

   test_arch_disable_timer_int();

   return TERR_NO_ERR;
}

static  int check_test_mutex_9_result(CURRTEST * ct)
{
   if(ct->rc_int == 7)
   {
      TestPrint("Mutex Test 9 passed.\r\n");
   }
   else
   {
      TestPrint("Mutex Test 9 failed. int: %d\r\n", ct->rc_int);
   }

   return 0;
}
 

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


