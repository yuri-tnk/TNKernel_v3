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

TN_TCB g_task_F;
#define  TASK_F_STK_SIZE          128
unsigned int task_F_stack[TASK_F_STK_SIZE];

extern TN_DQUEUE * gp_A_dque;
extern TN_DQUEUE * gp_B_dque;
extern TN_DQUEUE * gp_C_dque;

TN_SEM testSem_A;

//==== Task Test 1 

static void test_base_func_test_task_1(void * par);
static void test_task_A_func_test_task_1(void * par); 
static void test_task_B_func_test_task_1(void * par); 
static void test_task_C_func_test_task_1(void * par); 
static  int check_test_task_1_result(CURRTEST * ct); 

#define  TEST_START_IDX    0
#define  TEST_A1_IDX       1
#define  TEST_A2_IDX       2
#define  TEST_A3_IDX       3
#define  TEST_A4_IDX       4
#define  TEST_B1_IDX       5
#define  TEST_B2_IDX       6
#define  TEST_B3_IDX       7
#define  TEST_B4_IDX       8
#define  TEST_C1_IDX       9
#define  TEST_C2_IDX      10 
#define  TEST_C3_IDX      11
#define  TEST_C4_IDX      12
#define  TEST_MAX_IDX     13


//==== Task Test 2 

static void test_base_func_test_task_2(void * par);
static void test_task_A_func_test_task_2(void * par); 
static void test_task_B_func_test_task_2(void * par); 
static void test_task_C_func_test_task_2(void * par); 
static  int check_test_task_2_result(CURRTEST * ct); 

//==== Task Test 3 

static void test_base_func_test_task_3(void * par);
static void test_task_A_func_test_task_3(void * par); 
static void test_task_B_func_test_task_3(void * par); 
static void test_task_C_func_test_task_3(void * par); 
static void test_task_F_func_test_task_3(void * par); 
static  int check_test_task_3_result(CURRTEST * ct); 

//==== Task Test 4 

static void test_base_func_test_task_4(void * par);
static void test_task_A_func_test_task_4(void * par); 
static  int int_timer_func_test_task_4(void * par);
static  int check_test_task_4_result(CURRTEST * ct); 

#define  TEST_TASK_INT_1     1
#define  TEST_TASK_INT_2     2
#define  TEST_TASK_INT_3     3

//================ All task tests to run ============

const test_func g_task_tests_arr[] =
{
   test_base_func_test_task_1,
   test_base_func_test_task_2,
   test_base_func_test_task_3,
   test_base_func_test_task_4,

   NULL // Terminator
};

//=============== Task Test 1 =======================

/*
 *   Round- robin scheduler for equal priority tasks
 *
 * Ticks----> 0   10   20  30   40   50   60  70   80   90   99  108  117
 *  Task_C    +++++_________+++++_________+++++__________+++++............
 *  Task_B    _____+++++_________+++++_________+++++__________+++++.......
 *  Task_A    __________+++++_________+++++_________+++++__________+++++..
 *                                    
 * Legend:
 *   
 *   +++  - Running
 *   ___  - Ready
 *   ...  - Terminated
 *
 * Each task makes long job (endless loop) with duration 39 ticks
 * Round-robin period is 10 ticks
 *
*/

static void test_base_func_test_task_1(void * par)
{
   int rc;
   unsigned long * arr;

   //--- Create test data

   TestPrint("Task Test 1 - start.\r\n");

   g_curr_test.test_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 
                                     TEST_MAX_IDX * sizeof(unsigned long));  
   arr = (unsigned long *)g_curr_test.test_data_1;


   g_curr_test.rc_base = 0;

   //--- Create tasks - equal priority

   task_A = create_test_task(10, test_task_A_func_test_task_1);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(10,test_task_B_func_test_task_1); 
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(10, test_task_C_func_test_task_1); 
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   
   //--- Create end test signal event

   test_end_signal_create();


   //--- Start test
   rc = tn_sys_tslice_ticks(10U,  // priority, 
                            10U); // tick's value
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   arr[TEST_START_IDX] = tn_get_sys_ticks();

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_task_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   test_end_signal_delete();

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   {
      TN_INTSAVE_DATA
 
      tn_disable_interrupt();
      tn_tslice_ticks[10] = NO_TIME_SLICE;
      tn_enable_interrupt();
   }
   //---- Notify parent task about the test end

   TestPrint("Task Test 1 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

//----------------------------------------------------------------------------
static void test_task_A_func_test_task_1(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST_A1_IDX] = tn_get_sys_ticks();
   long_job_delay(39);
   arr[TEST_A2_IDX] = tn_get_sys_ticks();

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_B_func_test_task_1(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST_B1_IDX] = tn_get_sys_ticks();
   long_job_delay(39);
   arr[TEST_B2_IDX] = tn_get_sys_ticks();

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_C_func_test_task_1(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST_C1_IDX] = tn_get_sys_ticks();
   long_job_delay(39);
   arr[TEST_C2_IDX] = tn_get_sys_ticks();

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static int check_test_task_1_result(CURRTEST * ct)
{
   unsigned long * arr = (unsigned long *)ct->test_data_1;
   unsigned long a1;
   unsigned long a2;
   unsigned long b1;
   unsigned long b2;
   unsigned long c1;
   unsigned long c2;

   a1 = arr[TEST_A1_IDX] - arr[TEST_START_IDX];
   a2 = arr[TEST_A2_IDX] - arr[TEST_START_IDX];

   b1 = arr[TEST_B1_IDX] - arr[TEST_START_IDX];
   b2 = arr[TEST_B2_IDX] - arr[TEST_START_IDX];

   c1 = arr[TEST_C1_IDX] - arr[TEST_START_IDX];
   c2 = arr[TEST_C2_IDX] - arr[TEST_START_IDX];

   if((a1 == 0UL || a1 == 1UL) && (a2 == 99UL || a2 == 100UL) &&
        (b1 == 10UL || b1 == 11UL) && (b2 == 108UL || b2 == 109UL) &&
           (c1 == 20UL || c1 == 21UL) && (c2 == 117UL || c2 == 118UL))
   {
      TestPrint("Task Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("Task Test 1 failed. a1:%ld a2:%ld, b1:%ld, "
                "b2:%ld, c1:%ld, c2:%ld\r\n",
                 a1, a2, b1, b2, c1, c2);
   }

   return 0;
} 

//==== Task Test 2  =========================================================

static void test_base_func_test_task_2(void * par)
{
/*
 *    Manual Priority Ceiling Protocol
 *
 *    Three threads are involved in the classic priority inversion scenario, a
 *  medium priority thread tries to starve an high priority thread by blocking 
 *  a low priority thread into a regular semaphore lock zone.
 *
 *    The test provides a Manual rising the "minimal priority task" priority 
 *  in the ceiling grop by calling 'tn_task_change_priority()' function. 
 *    It works similar to the priority inheritance protocol here (See Mutex Test 3).
 */
   int rc;
   unsigned long * arr;

   //--- Create test data

   TestPrint("Task Test 2 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   g_curr_test.test_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 
                                     TEST_MAX_IDX * sizeof(unsigned long));  
   arr = (unsigned long *)g_curr_test.test_data_1;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_task_2); // Hi
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_task_2); // Medium
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_task_2); // Lo
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   testSem_A.id_sem = 0UL;
   rc = tn_sem_create(&testSem_A, 1, 1);
   if(rc < 0)
      TestPrint("testSem_A - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   arr[TEST_START_IDX] = tn_get_sys_ticks();

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_task_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_sem_delete(&testSem_A);
   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   //---- Notify parent task about the test end

   TestPrint("Task Test 2 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_task_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   arr[TEST_A1_IDX] = tn_get_sys_ticks();

   tn_task_sleep(40);

// Rise min priority in the ceil group to ceil priority
   tn_task_change_priority(task_C, 10U);

   tn_sem_acquire(&testSem_A, TN_WAIT_INFINITE);

   arr[TEST_A2_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST_A3_IDX] = tn_get_sys_ticks();

   tn_sem_signal(&testSem_A);

   arr[TEST_A4_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "A");

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_task_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_task_sleep(20);

   arr[TEST_B1_IDX] = tn_get_sys_ticks();

   long_job_delay(40);

   arr[TEST_B2_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "B");

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_C_func_test_task_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;

   tn_sem_acquire(&testSem_A, TN_WAIT_INFINITE);

   arr[TEST_C1_IDX] = tn_get_sys_ticks();

   long_job_delay(40);

   arr[TEST_C2_IDX] = tn_get_sys_ticks();

// Before release sem, return raised task priority to the original

   tn_task_change_priority(task_C, 12U);

   tn_sem_signal(&testSem_A);

   arr[TEST_C3_IDX] = tn_get_sys_ticks();

   long_job_delay(10);

   arr[TEST_C4_IDX] = tn_get_sys_ticks();

   strcat((char*)ct->test_res_data_1, "C");

//   TestPrint("Task_C priority: %u.\r\n",task_C->priority);

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_task_2_result(CURRTEST * ct)
{
   const char ref_str[] = "ABC";
   unsigned long * arr = (unsigned long *)g_curr_test.test_data_1;
   int fOK = FALSE;

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      if(tick_val_inside(arr[TEST_A2_IDX] - arr[TEST_START_IDX], 60UL, 63UL) == TRUE)
      {
         if(tick_val_inside(arr[TEST_A3_IDX] - arr[TEST_START_IDX], 70UL, 73UL) == TRUE)
         {
            if(tick_val_inside(arr[TEST_B1_IDX] - arr[TEST_START_IDX], 20UL, 23UL) == TRUE)
            {
               if(tick_val_inside(arr[TEST_B2_IDX] - arr[TEST_START_IDX], 90UL, 93UL) == TRUE)
               {
                  if(tick_val_inside(arr[TEST_C2_IDX] - arr[TEST_START_IDX], 60UL, 62UL) == TRUE)
                  {
                     if(tick_val_inside(arr[TEST_C3_IDX] - arr[TEST_START_IDX], 90UL, 102UL) == TRUE)
                     {
                        if(tick_val_inside(arr[TEST_C4_IDX] - arr[TEST_START_IDX], 99UL, 102UL) == TRUE)
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
TestPrint(" %lu\r\n", arr[TEST_A2_IDX] - arr[TEST_START_IDX]);   //   60UL, 63UL
TestPrint(" %lu\r\n", arr[TEST_A3_IDX] - arr[TEST_START_IDX]);   //   70UL, 73UL
TestPrint(" %lu\r\n", arr[TEST_B1_IDX] - arr[TEST_START_IDX]);   //   20UL, 23UL
TestPrint(" %lu\r\n", arr[TEST_B2_IDX] - arr[TEST_START_IDX]);   //   90UL, 93UL
TestPrint(" %lu\r\n", arr[TEST_C2_IDX] - arr[TEST_START_IDX]);   //   60UL, 62UL
TestPrint(" %lu\r\n", arr[TEST_C3_IDX] - arr[TEST_START_IDX]);   //   90UL, 102UL
TestPrint(" %lu\r\n", arr[TEST_C4_IDX] - arr[TEST_START_IDX]);   //   99UL, 102UL
#endif     
                                                              
   if(fOK == TRUE)                                                 
   {                                                               
      TestPrint("Task Test 2 passed.\r\n");                        
   }                                                               
   else
   {
      TestPrint("Task Test 2 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 


//==== Task Test 3  =========================================================

static void test_base_func_test_task_3(void * par)
{
   int rc;
   int err;

   //--- Create test data

   TestPrint("Task Test 3 - start.\r\n");
   
   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_task_3); // Hi
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,test_task_B_func_test_task_3); // Medium
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12, test_task_C_func_test_task_3); // Lo
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");


   testSem_A.id_sem = 0UL;
   rc = tn_sem_create(&testSem_A, 0, 1);
   if(rc < 0)
      TestPrint("testSem_A - creation failed.\r\n");

   //--- Create end test signal event

//   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);
   
 //-- Wrong param


   g_task_F.id_task = 0UL;
//-- TCB
   rc = tn_task_create(NULL,             
                       test_task_F_func_test_task_3,
                       14U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
//-- func
   rc = tn_task_create(&g_task_F,
                       NULL,
                       14U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//-- Prio TN_NUM_PRIORITY-1U
   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       TN_NUM_PRIORITY-1U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
//-- Prio > TN_NUM_PRIORITY-1U
   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       TN_NUM_PRIORITY,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
//-- Prio = 0
   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       0U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//-- task_stack_size

   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       14U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TN_MIN_STACK_SIZE - 1,         //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//-- task_stack_start == NULL
   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       14U,    //priority,
                       NULL,  
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//-- Ok
   g_curr_test.rc_F = 0;

   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       14U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x00012345L,            //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   tn_task_sleep(2);
//-- Recreate
   rc = tn_task_create(&g_task_F,             
                       test_task_F_func_test_task_3,
                       14U,    //priority,
                       &(task_F_stack          //-- task stack first addr in memory
                          [TASK_F_STK_SIZE-1]),
                       TASK_F_STK_SIZE,               //-- task stack size (in int,not bytes)
                       (void*)0x123456,               //-- task function parameter
                       TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
//----------- Dyn

 //-- Stack size
   task_E = tn_task_create_dyn(test_task_F_func_test_task_3,
                            14U,
                            TN_MIN_STACK_SIZE - 1,  
                            NULL,
                            0, 
                            &err);
   if(task_E == NULL && err == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//------------ Delete

   rc = tn_task_delete(NULL);
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_task_delete(&g_task_F);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
  //-- Re-delete 
   rc = tn_task_delete(&g_task_F);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

//      rc = TERR_WPARAM;
//      rc = TERR_NOEXS;
//      rc = TERR_WCONTEXT;
//      rc = TERR_WCONTEXT;

   //--- Now delete sem -> Tasks A,B,C have to exit

   tn_sem_delete(&testSem_A);

   tn_task_sleep(5);
 
   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources


//   test_end_signal_delete();

   check_test_task_3_result(&g_curr_test);  //--- Check test results

   //---- Notify parent task about the test end

   TestPrint("Task Test 3 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_task_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   ct->rc_A = 0; 
   for(;;)
   {
      rc = tn_sem_acquire(&testSem_A, TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_A++; 
      }
      tn_task_exit();
   }

//   test_end_do_signal(TASK_A_RDY_FLAG);
//   tn_task_sleep(2);
}

static void test_task_B_func_test_task_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_B = 0; 
   for(;;)
   {
      rc = tn_sem_acquire(&testSem_A, TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_B++; 
      }
      tn_task_exit();
   }
}

static void test_task_C_func_test_task_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_C = 0; 
   for(;;)
   {
      rc = tn_sem_acquire(&testSem_A, TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_C++; 
      }
      tn_task_exit();
   }
}

static void test_task_F_func_test_task_3(void * par) 
{
   long val;

   val = (long)par;
   if(val == 0x00012345L)  //-- task function parameter
   {
      g_curr_test.rc_F = 1;
   }

   for(;;)
   {
      tn_task_sleep(100);
   }
}

static  int check_test_task_3_result(CURRTEST * ct)
{
   if(ct->rc_base == 13 && ct->rc_A == 1 && 
       ct->rc_B == 1 && ct->rc_C == 1 && ct->rc_F == 1)
   {                                                               
      TestPrint("Task Test 3 passed.\r\n");                        
   }                                                               
   else
   {
      TestPrint("Task Test 3 failed. rc_base: %d A: %d B: %d C: %d F: %d \r\n",
                ct->rc_base, ct->rc_A, ct->rc_B, ct->rc_C, ct->rc_F);
   }

   return 0;
} 


//==== Task Test 4 ===========================================================

static void test_base_func_test_task_4(void * par)
{
   int rc;
   int tmp;
   //--- Create test data

   TestPrint("Task Test 4 - start.\r\n");
   
   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_task_4); // Hi
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");


   //--- Start test

   tn_task_activate(task_A);

  //--- Suspend/resume 

//   tn_task_sleep(15);

   tn_task_sleep(10); // 3  ct_A

   rc = tn_task_suspend(task_A);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   if(g_curr_test.rc_A == 2 || g_curr_test.rc_A == 3)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_task_resume(task_A);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   (void)tn_task_sleep(13); // 4  ct_A

   rc = tn_task_suspend(task_A);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   (void)tn_task_sleep(10); // If not suspended, rc_A will be incremented

   if(g_curr_test.rc_A == 7)
   {
      g_curr_test.rc_base++;
   }

   g_curr_test.rc_int = 0;

  //--- Interrupt N1 - iresume

   g_curr_test.rc_A = 0;

   g_curr_test.int_state = TEST_TASK_INT_1;

   setup_test_int_timer(2,      // unsigned long timeout, 
                        int_timer_func_test_task_4);

   rc = tn_task_sleep(11); 
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   if(g_curr_test.rc_A == 3 || g_curr_test.rc_A == 4)
   {
      g_curr_test.rc_base++;
   }
   tmp = g_curr_test.rc_A;


  //--- Interrupt N2 - isuspend

   g_curr_test.int_state = TEST_TASK_INT_2;

   setup_test_int_timer(2,      // unsigned long timeout, 
                        int_timer_func_test_task_4);

   rc = tn_task_sleep(10); 
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   if(g_curr_test.rc_A == tmp || g_curr_test.rc_A == tmp + 1)
   {
      g_curr_test.rc_base++;
   }

//   test_end_signal_delete();

   rc = tn_task_delete(task_A);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

  //--- Interrupt Last

   g_curr_test.int_state = TEST_TASK_INT_3;

   setup_test_int_timer(5,      // unsigned long timeout, 
                        int_timer_func_test_task_4);

   rc = tn_task_sleep(10); 
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   check_test_task_4_result(&g_curr_test);  //--- Check test results

   //---- Notify parent task about the test end

   TestPrint("Task Test 4 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_task_4(void * par) 
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
   for(;;)
   {
      tn_task_sleep(3);
      ct->rc_A++;
   }
}

//   ct->rc_int += cnt;
//
//   test_arch_disable_timer_int();
//
//   return TERR_NO_ERR;  // Stop int

static  int int_timer_func_test_task_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   if(ct->int_state == TEST_TASK_INT_1)
   {
      rc = tn_task_resume(task_A);
      if(rc == TERR_NO_ERR)
      {
         ct->rc_int++;
      }

      rc = tn_task_resume(task_A);
      if(rc == TERR_WSTATE)
      {
         ct->rc_int++;
      }
   }
   else if(ct->int_state == TEST_TASK_INT_2)
   {
      rc = tn_task_suspend(task_A);
      if(rc == TERR_NO_ERR)
      {
         ct->rc_int++;
      }
      rc = tn_task_suspend(task_A);
      if(rc == TERR_OVERFLOW)
      {
         ct->rc_int++;
      }
   }
   else if(ct->int_state == TEST_TASK_INT_3)
   {
      rc = tn_task_suspend(task_A);
      if(rc == TERR_NOEXS)
      {
         ct->rc_int++;
      }

      rc = tn_task_resume(task_A);
      if(rc == TERR_NOEXS)
      {
         ct->rc_int++;
      }
   }

   test_arch_disable_timer_int();

   return TERR_NO_ERR;
}

static  int check_test_task_4_result(CURRTEST * ct) 
{
   if(ct->rc_base == 11 && ct->rc_int == 6)
   {                                                               
      TestPrint("Task Test 4 passed.\r\n");                        
   }                                                               
   else
   {
      TestPrint("Task Test 4 failed. rc_base: %d A: %d Int: %d \r\n",
                ct->rc_base, ct->rc_A, ct->rc_int);
   }

   return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
