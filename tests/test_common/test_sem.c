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


TN_SEM testSem;

//==== Sem Test 1

static void test_base_func_test_sem_1(void * par);
static void test_task_A_func_test_sem_1(void * par); 
static void test_task_B_func_test_sem_1(void * par); 
static void test_task_C_func_test_sem_1(void * par); 
static  int check_test_sem_1_result(CURRTEST * ct); 

#define  SBUF_LEN                 16
#define  NUM_CYCLES_SEM_TEST_1     5

//==== Sem Test 2

static void test_base_func_test_sem_2(void * par);
static void test_task_A_func_test_sem_2(void * par);
static void test_task_B_func_test_sem_2(void * par);
static void test_task_C_func_test_sem_2(void * par);
static  int check_test_sem_2_result(CURRTEST * ct); 

#define  NUM_CYCLES_SEM_TEST_2     5

//==== Sem Test 3

static void test_base_func_test_sem_3(void * par);
static void test_task_A_func_test_sem_3(void * par);
static void test_task_B_func_test_sem_3(void * par);
static  int check_test_sem_3_result(CURRTEST * ct); 

//==== Sem Test 4

static void test_base_func_test_sem_4(void * par);
static void test_task_A_func_test_sem_4(void * par);
static void test_task_B_func_test_sem_4(void * par);
static  int check_test_sem_4_result(CURRTEST * ct); 

//==== Sem Test 5

static void test_base_func_test_sem_5(void * par);
static void test_task_A_func_test_sem_5(void * par);
static void test_task_B_func_test_sem_5(void * par);
static  int check_test_sem_5_result(CURRTEST * ct); 

//==== Sem Test 6

static void test_base_func_test_sem_6(void * par);
static void test_task_A_func_test_sem_6(void * par);
static void test_task_B_func_test_sem_6(void * par);
static  int check_test_sem_6_result(CURRTEST * ct); 

//==== Sem Test 7

static void test_base_func_test_sem_7(void * par);
static void test_task_A_func_test_sem_7(void * par);
static void test_task_B_func_test_sem_7(void * par);
static  int check_test_sem_7_result(CURRTEST * ct); 

//==== Sem Test 8

static void test_base_func_test_sem_8(void * par);
static int int_timer_func_test_8(void * par);
static int check_test_sem_8_result(CURRTEST * ct);

//==== Sem Test 9

static void test_base_func_test_sem_9(void * par);
static void test_task_A_func_test_sem_9(void * par);
static int int_timer_func_test_9(void * par);
static  int check_test_sem_9_result(CURRTEST * ct);

#define  TEST_9_NUM_CYCLES 5

//==== Sem Test 10

static void test_base_func_test_sem_10(void * par);
static int int_timer_func_test_10(void * par);
static  int check_test_sem_10_result(CURRTEST * ct);

//==== Sem Test 11

static void test_base_func_test_sem_11(void * par);
static void test_task_A_func_test_sem_11(void * par);
static  int check_test_sem_11_result(CURRTEST * ct);

//==== Sem Test 12

static void test_base_func_test_sem_12(void * par);
static void test_task_A_func_test_sem_12(void * par);
static int int_timer_func_test_12(void * par);
static  int check_test_sem_12_result(CURRTEST * ct);

//================ All sem tests to run ============

const test_func g_sem_tests_arr[] =
{
   test_base_func_test_sem_1,
   test_base_func_test_sem_2,
   test_base_func_test_sem_3,
   test_base_func_test_sem_4,
   test_base_func_test_sem_5,
   test_base_func_test_sem_6,
   test_base_func_test_sem_7,
   test_base_func_test_sem_8,
   test_base_func_test_sem_9,
   test_base_func_test_sem_10,
   test_base_func_test_sem_11,
   test_base_func_test_sem_12,

   NULL // Terminator
};


//=============== Sem Test 1 =======================

static void test_base_func_test_sem_1(void * par)
{
   int rc;

   //--- Create test data

   TestPrint("Sem Test 1 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   //--- Create tasks

   task_A = create_test_task(10,                            // priority
                             test_task_A_func_test_sem_1);   // task_func
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,                            // priority
                             test_task_B_func_test_sem_1);   // task_func
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12,                            // priority
                             test_task_C_func_test_sem_1);   // task_func
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      2,
                      2);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

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
      check_test_sem_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

//   tdump(&tn_objects_mem);

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   //---- Notify parent task about the test end

   TestPrint("Sem Test 1 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);


}

//----------------------------------------------------------------------------
static void test_task_A_func_test_sem_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "a%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_A_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_B_func_test_sem_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "b%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_B_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_C_func_test_sem_1(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_1; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "c%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_C_RDY_FLAG);

   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static int check_test_sem_1_result(CURRTEST * ct)
{
   const char ref_str[] = "a0b0a1b1a2b2a3b3a4b4c0c1c2c3c4"; //"a0a1a2a3a4b0b1b2b3b4c0c1c2c3c4";

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      TestPrint("Sem Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 1 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 

//=============== Sem Test 2 =======================

static void test_base_func_test_sem_2(void * par)
{
   int rc;

   //--- Create test data

   TestPrint("Sem Test 2 - start.\r\n");

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_2);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_2);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");
   task_C = create_test_task(12, test_task_C_func_test_sem_2);
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      1,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

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
      check_test_sem_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   //---- Notify parent task about the test end

   TestPrint("Sem Test 2 - end.\r\n");
//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);

}

//----------------------------------------------------------------------------
static void test_task_A_func_test_sem_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_2; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "a%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 
   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_B_func_test_sem_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_2; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "b%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 
   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static void test_task_C_func_test_sem_2(void * par)
{
   unsigned char sbuf[SBUF_LEN];
   int i;
   CURRTEST * ct = (CURRTEST *)par;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   for(i=0; i < NUM_CYCLES_SEM_TEST_2; i++)
   {
      tn_snprintf((char*)sbuf, SBUF_LEN, "c%d", i);
      strcat((char*)ct->test_res_data_1, (char*)sbuf);
      tn_task_sleep(2);
   } 

   tn_sem_signal(&testSem);
   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
} 

//----------------------------------------------------------------------------
static int check_test_sem_2_result(CURRTEST * ct)
{
   const char ref_str[] = "a0a1a2a3a4b0b1b2b3b4c0c1c2c3c4";

   if(strcmp(ref_str, (char*)ct->test_res_data_1) == 0) // OK
   {
      TestPrint("Sem Test 2 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 2 failed. %s\r\n", ct->test_res_data_1);
   }

   return 0;
} 

//==== Sem Test 3 ============================================================

static void test_base_func_test_sem_3(void * par)
{
   int rc;
/*
N3 -timeout

Tasks: A, B
sem: start 1, max 1
  A acquire(TN_WAIT_INFINITE)
  B acquire(10)
  Res: B - TERR_TIMEOUT
*/

   TestPrint("Sem Test 3 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_3);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_3);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      1,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_3_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 3 - end.\r\n");
//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);

}

static void test_task_A_func_test_sem_3(void * par)
{
   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_sem_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = tn_sem_acquire(&testSem, 10);

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_sem_3_result(CURRTEST * ct)
{
   if(ct->rc_B == TERR_TIMEOUT) // OK
   {
      TestPrint("Sem Test 3 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 3 failed. %d\r\n", ct->rc_B);
   }

   return 0;
} 


//==== Sem Test 4 ============================================================

static void test_base_func_test_sem_4(void * par)
{
   int rc;
/*
N4 - Got sem after waiting

Tasks: A, B
sem: start 0, max 1
  A 
     acquire(10)
     Res A - timeout expired
     tn_task_sleep(A, 10);
     tn_sem_signal()
  B 
     acquire(TN_WAIT_INFINITE)
     Res: B - TERR_NO_ERR_W
*/
   TestPrint("Sem Test 4 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_4);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_4);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      0,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_4_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 4 - end.\r\n");
//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);


}

static void test_task_A_func_test_sem_4(void * par)
{
   tn_sem_acquire(&testSem, 10);
   tn_task_sleep(2);
   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_sem_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_sem_4_result(CURRTEST * ct)
{
   if(ct->rc_B == TERR_NO_ERR_W) // OK
   {
      TestPrint("Sem Test 4 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 4 failed. %d\r\n", ct->rc_B);
   }

   return 0;
} 


//==== Sem Test 5 ============================================================

static void test_base_func_test_sem_5(void * par)
{
   int rc;
/*
N5 - Got sem immediately & no wait

Tasks: A,B
sem: start 1, max 1
  A 
     acquire(TN_WAIT_INFINITE)
     Res A - TERR_NO_ERR
     tn_task_sleep(A, 10);
     tn_sem_signal();
  B
     tn_task_sleep(B, 15);
     acquire(TN_NO_WAIT);
     Res B - TERR_NO_ERR
*/
   TestPrint("Sem Test 5 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_5);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_5);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      1,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_5_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 5 - end.\r\n");
//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);

}

static void test_task_A_func_test_sem_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = tn_sem_acquire(&testSem, TN_WAIT_INFINITE);
   tn_task_sleep(10);
   tn_sem_signal(&testSem);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_sem_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   tn_task_sleep(15);
   ct->rc_B = tn_sem_acquire(&testSem, TN_NO_WAIT);

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_sem_5_result(CURRTEST * ct)
{
   if(ct->rc_A == TERR_NO_ERR && 
            ct->rc_B == TERR_NO_ERR) // OK
   {
      TestPrint("Sem Test 5 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 5 failed. A: %d  B: %d\r\n", ct->rc_A, ct->rc_B);
   }

   return 0;
} 


//==== Sem Test 6 ============================================================

static void test_base_func_test_sem_6(void * par)
{
   int rc;
/*
N6 - no wait TERR_UNDERFLOW; // For no wait only

Tasks: A,B
sem: start 1, max 1
  A 
     acquire(TN_WAIT_INFINITE)

  B
     acquire(TN_NO_WAIT)
     Res: B - TERR_UNDERFLOW
*/
   TestPrint("Sem Test 6 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_6);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_6);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      1,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_6_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 6 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_sem_6(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_sem_6(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = tn_sem_acquire(&testSem, TN_NO_WAIT);

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_sem_6_result(CURRTEST * ct)
{
   if(ct->rc_A == TERR_NO_ERR && 
            ct->rc_B == TERR_UNDERFLOW) // OK
   {
      TestPrint("Sem Test 6 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 6 failed. A: %d  B: %d\r\n", ct->rc_A, ct->rc_B);
   }

   return 0;
} 

//==== Sem Test 7 ============================================================

static void test_base_func_test_sem_7(void * par)
{
   int rc;
/*
N7 - signal

Tasks: A, B
sem: start 0, max 1

  A 
    tn_task_sleep(10)
    tn_sem_signal()
    Res A - TERR_NO_ERR_W

  B 
    acquire(TN_WAIT_INFINITE)
    tn_sem_signal()
    Res B - TERR_NO_ERR
    tn_sem_signal()
    Res B - TERR_OVERFLOW
*/
   TestPrint("Sem Test 7 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_7);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");
   task_B = create_test_task(11, test_task_B_func_test_sem_7);
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      0,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,      // unsigned long timeout, 
                        NULL);  // test_int_timer_func func, 
   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_7_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 7 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_sem_7(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;

   tn_task_sleep(5);

   ct->rc_A = tn_sem_signal(&testSem);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_sem_7(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_B = 0;

   tn_sem_acquire(&testSem, TN_WAIT_INFINITE);

   rc = tn_sem_signal(&testSem);
   if(rc == TERR_NO_ERR)
      ct->rc_B++;
   rc = tn_sem_signal(&testSem);
   if(rc == TERR_OVERFLOW)
      ct->rc_B++;

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_sem_7_result(CURRTEST * ct)
{
   if(ct->rc_A == TERR_NO_ERR_W && 
                  ct->rc_B == 2) // OK
   {
      TestPrint("Sem Test 7 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 7 failed. A: %d  B: %d\r\n", ct->rc_A, ct->rc_B);
   }

   return 0;
} 

//==== Sem Test 8 ============================================================

static void test_base_func_test_sem_8(void * par)
{
   int rc;
/*
N8 - int; TERR_UNDERFLOW; // For no wait only 

Tasks: No
sem: start 1, max 1
int_timer: 10

  INT 
      acquire(TN_NO_WAIT)
      Res Int - TERR_NO_ERR

      acquire(TN_NO_WAIT)
      Res Int - TERR_UNDERFLOW; 

      tn_sem_signal()
      Res Int - TERR_NO_ERR

      tn_sem_signal()
      Res Int - TERR_OVERFLOW
*/
   TestPrint("Sem Test 8 - start.\r\n");

   //--- Create tasks


   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      1,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - int timer & handler

   setup_test_int_timer(10,      // unsigned long timeout, 
                        int_timer_func_test_8);

   //--- Start test

   //--- Wait until all test participants finished
   rc = test_end_wait(INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_8_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 8 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

int int_timer_func_test_8(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_int = 0;
   rc = tn_sem_acquire(&testSem, TN_NO_WAIT);
   if(rc == TERR_NO_ERR)
      ct->rc_int++;
   rc = tn_sem_acquire(&testSem, TN_NO_WAIT);
   if(rc == TERR_UNDERFLOW) 
      ct->rc_int++;
   rc = tn_sem_signal(&testSem);
   if(rc == TERR_NO_ERR)
      ct->rc_int++;
   rc = tn_sem_signal(&testSem);
   if(rc == TERR_OVERFLOW)
      ct->rc_int++;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;
}

static  int check_test_sem_8_result(CURRTEST * ct)
{
   if(ct->rc_int == 4) // OK
   {
      TestPrint("Sem Test 8 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 8 failed. rc_int: %d\r\n", ct->rc_int);
   }

   return 0;
} 


//==== Sem Test 9 ============================================================

static void test_base_func_test_sem_9(void * par)
{
   int rc;
/*
N9 - signal (int) 5 times

Tasks: A
sem: start 1, max 1
int_timer: 10
  A
     acq(TN_WAIT_INFINITE);
     Res A - TERR_NO_ERR_W
  INT
     tn_sem_signal()

*/
   TestPrint("Sem Test 9 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_sem_9);
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      0,
                      1);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   g_curr_test.int_cnt = 0;
   setup_test_int_timer(10,      // unsigned long timeout, 
                        int_timer_func_test_9);

   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_9_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 9 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_sem_9(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_A = 0;
   for(;;)
   {
      rc = tn_sem_acquire(&testSem, 100); //TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR_W)
      {
         ct->rc_A++;
         if(ct->rc_A >= TEST_9_NUM_CYCLES)
            break; 
      } 
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

int int_timer_func_test_9(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc = 2; // in progress

   tn_sem_signal(&testSem);
   ct->int_cnt++;
   if(ct->int_cnt >= TEST_9_NUM_CYCLES)
   {
      test_arch_disable_timer_int();
      rc = TERR_NO_ERR;  //-- End of the test
   }
   else
   {
      test_arch_restart_int_timer(ct);   
   }

   return rc;
}

static  int check_test_sem_9_result(CURRTEST * ct)
{
   if(ct->rc_A == TEST_9_NUM_CYCLES) // OK
   {
      TestPrint("Sem Test 9 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 9 failed. A: %d\r\n", ct->rc_A);
   }

   return 0;
} 


//==== Sem Test 10 ============================================================

static void test_base_func_test_sem_10(void * par)
{
   int rc;
/*
N10 - wrong context int

Tasks: No
sem: start 4, max 10
int_timer: 10

  INT 
     tn_sem_acq(TN_WAIT_INFINITE)
     Res Int - TERR_WCONTEXT
*/
   TestPrint("Sem Test 10 - start.\r\n");

   //--- Create tasks

   //--- Create test object (here - sem)

   rc = tn_sem_create(&testSem,
                      4,
                      10);
   if(rc < 0)
      TestPrint("testSem - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(10,      // unsigned long timeout, 
                        int_timer_func_test_10);

   //--- Start test

   //--- Wait until all test participants finished
   rc = test_end_wait(INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_10_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 10 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

int int_timer_func_test_10(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_int = 0;

   rc = tn_sem_acquire(&testSem, TN_WAIT_INFINITE);
   if(rc == TERR_WCONTEXT)
      ct->rc_int++;
   rc = tn_sem_acquire(&testSem, 123);
   if(rc == TERR_WCONTEXT)
      ct->rc_int++;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;
}

static  int check_test_sem_10_result(CURRTEST * ct)
{
   if(ct->rc_int == 2) // OK
   {
      TestPrint("Sem Test 10 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 10 failed. rc_int: %d\r\n", ct->rc_int);
   }

   return 0;
} 


//==== Sem Test 11 ============================================================

static void test_base_func_test_sem_11(void * par)
{
   int rc;
/*
N11 - create/delete sem with bad param

Tasks: A
  A
     tn_create_sem()
     Res A - TERR_NO_ERR
     tn_delete_sem()
     Res A - TERR_NO_ERR

     tn_create_sem_dyn()
     Res A - TERR_NO_ERR
     tn_delete_sem()
     Res A - TERR_NO_ERR
     //-- Bad param
     tn_create_sem(sem == NULL)
     Res A - TERR_WPARAM;
     tn_create_sem(max_val == 0)
     Res A - TERR_WPARAM;
     tn_create_sem(start_value > max_val)
     Res A - TERR_WPARAM;
     tn_create_sem(sem->id_sem != 0UL)
     Res A - TERR_WPARAM;
    
     tn_delete_sem(NULL)
     Res A - TERR_WPARAM;
*/
   TestPrint("Sem Test 11 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10,                              // priority
                             test_task_A_func_test_sem_11);   // task_func
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,  
                        NULL);

   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG );
   if(rc >= 0) // OK 
   {
      check_test_sem_11_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 11 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_sem_11(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   int err;
   TN_SEM sem;
   TN_SEM * psem;

   ct->rc_A = 0;
   sem.id_sem = 0UL;

     //-- Good param

   rc = tn_sem_create(&sem, 1, 1);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;
   rc = tn_sem_delete(&sem);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;
   psem = tn_sem_create_dyn(1, 1, &err);
   if(psem != NULL)
      ct->rc_A++;
   rc = tn_sem_delete(psem);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

     //-- Bad param

   sem.id_sem = 0UL;

   rc = tn_sem_create(NULL, 1, 1);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_sem_create(&sem, 1, 0); // max_val == 0
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_sem_create(&sem, 14, 2); //start_value > max_val
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   sem.id_sem = 1UL;
   rc = tn_sem_create(&sem, 1, 1); // sem->id_sem != 0UL
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   rc = tn_sem_delete(NULL);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_sem_delete(&sem);    // already deleted
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static int check_test_sem_11_result(CURRTEST * ct)
{
   if(ct->rc_A == 10) // OK
   {
      TestPrint("Sem Test 11 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 11 failed. A: %d\r\n", ct->rc_A);
   }

   return 0;
} 

//==== Sem Test 12 ===========================================================

static void test_base_func_test_sem_12(void * par)
{
   int rc;

/*
N12 - acq/signal bad param

Tasks: A
sem 1 max 1
int_timer: 30
  A
     tn_sem_create()
     Res A - TERR_NO_ERR
     tn_sem_acq(NULL, TN_NO_WAIT)
     Res A - TERR_WPARAM;
     tn_sem_signal(NULL, TN_NO_WAIT)
     Res A - TERR_WPARAM;
     tn_sem_delete()
     Res A - TERR_NO_ERR
     tn_sem_acq(sem, TN_NO_WAIT)
     Res A - TERR_NOEXS;
     tn_sem_signal(sem, TN_NO_WAIT)
     Res A - TERR_NOEXS;
  INT
     tn_sem_acq(sem, TN_NO_WAIT)
     Res int - TERR_NOEXS;
     tn_sem_signal(sem, TN_NO_WAIT)
     Res int - TERR_NOEXS;

*/
   TestPrint("Sem Test 12 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10,                            // priority
                             test_task_A_func_test_sem_12);   // task_func
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   //--- Create test object (here - sem)

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   g_curr_test.int_cnt = 0;
   setup_test_int_timer(30,      // unsigned long timeout, 
                        int_timer_func_test_12);
   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_sem_12_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

   tn_sem_delete(&testSem);
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Sem Test 12 - end.\r\n");

//   tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_sem_12(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_A = 0;

   rc = tn_sem_create(&testSem, 1, 1);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

   rc = tn_sem_acquire(NULL, TN_NO_WAIT);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_sem_signal(NULL);
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   rc = tn_sem_delete(&testSem);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

   rc = tn_sem_acquire(&testSem, TN_NO_WAIT);
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   rc = tn_sem_signal(&testSem);
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

int int_timer_func_test_12(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_int = 0;

   rc = tn_sem_acquire(&testSem, TN_NO_WAIT);
   if(rc == TERR_NOEXS)
      ct->rc_int++;

   rc = tn_sem_signal(&testSem);
   if(rc == TERR_NOEXS)
      ct->rc_int++;

   test_arch_disable_timer_int();
   rc = TERR_NO_ERR;  //-- End of the test

   return rc;
}

static  int check_test_sem_12_result(CURRTEST * ct)
{
   if(ct->rc_A == 6 && ct->rc_int == 2) // OK
   {
      TestPrint("Sem Test 12 passed.\r\n");
   }
   else
   {
      TestPrint("Sem Test 12 failed. rc_int: %d A: %d\r\n", ct->rc_int, ct->rc_A);
   }

   return 0;
} 

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

