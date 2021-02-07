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

#include <string.h>
#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"
#include "test.h"

extern TN_SEM   g_test_rdy_sem;
extern TN_EVENTFLAGS  g_test_end_evf;
extern CURRTEST g_curr_test; 
extern MEMINFO  g_test_mem;

extern TN_TCB * task_A;
extern TN_TCB * task_B;
extern TN_TCB * task_C;

extern TN_DQUEUE * gp_A_dque;

TN_EVENTFLAGS  g_evf_1;
TN_EVENTFLAGS  g_evf_2;
TN_EVENTFLAGS * g_evf_dyn_1;
TN_EVENTFLAGS * g_evf_dyn_2;

#define TEST_A_1               1  
#define TEST_A_2               2  
#define TEST_A_3               3  
#define TEST_A_4               4  
#define TEST_A_5               5  
#define TEST_A_6               6  
#define TEST_A_7               7  
#define TEST_A_EXIT          123

#define TEST_INT_1             1           
#define TEST_INT_2             2
#define TEST_INT_3             3
#define TEST_INT_4             4
#define TEST_INT_5             5
#define TEST_INT_6             6
#define TEST_INT_7             7
#define TEST_INT_8             8
#define TEST_INT_9             9
#define TEST_INT_10           10
#define TEST_INT_11           11
#define TEST_INT_12           12
#define TEST_INT_13           13
#define TEST_INT_14           14
#define TEST_INT_15           15



#define TEST_PATTERN_WAIT_A  0x1U
#define TEST_PATTERN_WAIT_B  0x2U
#define TEST_PATTERN_WAIT_C  0x4U


#define TEST_PATTERN_SET_TEST_1  0x1111U
#define TEST_PATTERN_SET_TEST_2  (TEST_PATTERN_WAIT_A | TEST_PATTERN_WAIT_B | TEST_PATTERN_WAIT_C)  
#define TEST_PATTERN_SET_TEST_3  (TEST_PATTERN_SET_TEST_1)
#define TEST_PATTERN_SET_TEST_4  (TEST_PATTERN_SET_TEST_2)

#define TEST_PATTERN_CHECK_TEST_1  (TEST_PATTERN_SET_TEST_1)
#define TEST_PATTERN_CHECK_TEST_2  0U
#define TEST_PATTERN_CHECK_TEST_3  (TEST_PATTERN_SET_TEST_1)
#define TEST_PATTERN_CHECK_TEST_4  0U

#define TEST_WAIT_MODE_TEST_1   TN_EVENT_WCOND_OR
#define TEST_WAIT_MODE_TEST_2   (TN_EVENT_WCOND_OR | TN_EVENT_ATTR_BITCLR)
#define TEST_WAIT_MODE_TEST_3   TN_EVENT_WCOND_AND
#define TEST_WAIT_MODE_TEST_4   (TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR)


#define TEST_WAIT_PATTERN_A_TEST_1  TEST_PATTERN_SET_TEST_1
#define TEST_WAIT_PATTERN_B_TEST_1  TEST_PATTERN_SET_TEST_1
#define TEST_WAIT_PATTERN_C_TEST_1  TEST_PATTERN_SET_TEST_1

#define TEST_WAIT_PATTERN_A_TEST_2  TEST_PATTERN_WAIT_A
#define TEST_WAIT_PATTERN_B_TEST_2  TEST_PATTERN_WAIT_B
#define TEST_WAIT_PATTERN_C_TEST_2  TEST_PATTERN_WAIT_C

#define TEST_WAIT_PATTERN_A_TEST_3  TEST_PATTERN_SET_TEST_1
#define TEST_WAIT_PATTERN_B_TEST_3  TEST_PATTERN_SET_TEST_1
#define TEST_WAIT_PATTERN_C_TEST_3  TEST_PATTERN_SET_TEST_1

#define TEST_WAIT_PATTERN_A_TEST_4  TEST_PATTERN_WAIT_A
#define TEST_WAIT_PATTERN_B_TEST_4  TEST_PATTERN_WAIT_B
#define TEST_WAIT_PATTERN_C_TEST_4  TEST_PATTERN_WAIT_C

typedef struct _TEST_EVF_BASE1
{
   unsigned int wait_pattern_A;
   unsigned int wait_pattern_B;
   unsigned int wait_pattern_C;

   unsigned long tout_A;
   unsigned long tout_B;
   unsigned long tout_C;

   int rc_check_A;
   int rc_check_B;
   int rc_check_C;

   unsigned int set_pattern;
   unsigned int check_pattern;
   unsigned int wait_mode;
   int test_num;
   char test_str[8];
  
}TEST_EVF_BASE1;

TEST_EVF_BASE1 g_evf_base1;

//==== Eventflags Test base1

static void test_base_func_test_evf_base1(void * par);
static void test_task_A_func_test_evf_base1(void * par); 
static void test_task_B_func_test_evf_base1(void * par); 
static void test_task_C_func_test_evf_base1(void * par); 
static  int check_test_evf_base1_result(CURRTEST * ct); 

//==== Eventflags Test 1

static void test_base_func_test_evf_1(void * par);

//==== Eventflags Test 2

static void test_base_func_test_evf_2(void * par);

//==== Eventflags Test 3

static void test_base_func_test_evf_3(void * par);

//==== Eventflags Test 4

static void test_base_func_test_evf_4(void * par);

//==== Eventflags Test 5

static void test_base_func_test_evf_5(void * par);


//==== Eventflags Test 6
/*
   'Wait' after 'set' - all touts, task context
   Also API -  tn_eventflags_clear();
   'p_pattern' field contents
*/

static void test_base_func_test_evf_6(void * par);
static void test_task_A_func_test_evf_6(void * par); 
static  int check_test_evf_6_result(CURRTEST * ct); 

//==== Eventflags Test 7
/*
   set/wait int
   Wait after set - int
   Wrong int context (TN_WAIT_INFINITY, TOUT)
*/

static void test_base_func_test_evf_7(void * par);
static int int_timer_func_test_evf_7(void * par);
static  int check_test_evf_7_result(CURRTEST * ct); 

//==== Eventflags Test 8
/*
   Create/delete
   Wrong params
*/
static void test_base_func_test_evf_8(void * par);
static void test_task_A_func_test_evf_8(void * par); 
static void test_task_B_func_test_evf_8(void * par); 
static void test_task_C_func_test_evf_8(void * par); 
static  int check_test_evf_8_result(CURRTEST * ct); 

//=============

const test_func g_evf_tests_arr[] =
{
   test_base_func_test_evf_1,
   test_base_func_test_evf_2,
   test_base_func_test_evf_3,
   test_base_func_test_evf_4,
   test_base_func_test_evf_5,
   test_base_func_test_evf_6,
   test_base_func_test_evf_7,
   test_base_func_test_evf_8,

   NULL // Terminator
};

static void test_base_func_test_evf_1(void * par)
{
   g_evf_base1.set_pattern   = TEST_PATTERN_SET_TEST_1;
   g_evf_base1.check_pattern = TEST_PATTERN_CHECK_TEST_1;
   g_evf_base1.wait_mode     = TEST_WAIT_MODE_TEST_1;
   g_evf_base1.test_num      = 1;

   g_evf_base1.wait_pattern_A = TEST_WAIT_PATTERN_A_TEST_1;
   g_evf_base1.wait_pattern_B = TEST_WAIT_PATTERN_B_TEST_1;
   g_evf_base1.wait_pattern_C = TEST_WAIT_PATTERN_C_TEST_1;

   g_evf_base1.tout_A = TN_WAIT_INFINITE;
   g_evf_base1.tout_B = TN_WAIT_INFINITE;
   g_evf_base1.tout_C = TN_WAIT_INFINITE;

   g_evf_base1.rc_check_A = TERR_NO_ERR_W;
   g_evf_base1.rc_check_B = TERR_NO_ERR_W;
   g_evf_base1.rc_check_C = TERR_NO_ERR_W;

   strcpy(g_evf_base1.test_str, "ABC");

   test_base_func_test_evf_base1(par);
}

static void test_base_func_test_evf_2(void * par)
{
   g_evf_base1.set_pattern   = TEST_PATTERN_SET_TEST_2;
   g_evf_base1.check_pattern = TEST_PATTERN_CHECK_TEST_2;
   g_evf_base1.wait_mode     = TEST_WAIT_MODE_TEST_2;
   g_evf_base1.test_num      = 2;

   g_evf_base1.wait_pattern_A = TEST_PATTERN_WAIT_A;
   g_evf_base1.wait_pattern_B = TEST_PATTERN_WAIT_B;
   g_evf_base1.wait_pattern_C = TEST_PATTERN_WAIT_C;

   g_evf_base1.tout_A = TN_WAIT_INFINITE;
   g_evf_base1.tout_B = TN_WAIT_INFINITE;
   g_evf_base1.tout_C = TN_WAIT_INFINITE;

   g_evf_base1.rc_check_A = TERR_NO_ERR_W;
   g_evf_base1.rc_check_B = TERR_NO_ERR_W;
   g_evf_base1.rc_check_C = TERR_NO_ERR_W;

   strcpy(g_evf_base1.test_str, "ABC");

   test_base_func_test_evf_base1(par);
}

static void test_base_func_test_evf_3(void * par)
{
   g_evf_base1.set_pattern   = TEST_PATTERN_SET_TEST_3;
   g_evf_base1.check_pattern = TEST_PATTERN_CHECK_TEST_3;
   g_evf_base1.wait_mode     = TEST_WAIT_MODE_TEST_3;
   g_evf_base1.test_num      = 3;

   g_evf_base1.wait_pattern_A = TEST_WAIT_PATTERN_A_TEST_3;
   g_evf_base1.wait_pattern_B = TEST_WAIT_PATTERN_B_TEST_3;
   g_evf_base1.wait_pattern_C = TEST_WAIT_PATTERN_C_TEST_3;

   g_evf_base1.tout_A = TN_WAIT_INFINITE;
   g_evf_base1.tout_B = TN_WAIT_INFINITE;
   g_evf_base1.tout_C = TN_WAIT_INFINITE;

   g_evf_base1.rc_check_A = TERR_NO_ERR_W;
   g_evf_base1.rc_check_B = TERR_NO_ERR_W;
   g_evf_base1.rc_check_C = TERR_NO_ERR_W;

   strcpy(g_evf_base1.test_str, "ABC");

   test_base_func_test_evf_base1(par);
}

static void test_base_func_test_evf_4(void * par)
{
   g_evf_base1.set_pattern   = TEST_PATTERN_SET_TEST_4;
   g_evf_base1.check_pattern = TEST_PATTERN_CHECK_TEST_4;
   g_evf_base1.wait_mode     = TEST_WAIT_MODE_TEST_4;
   g_evf_base1.test_num      = 4;

   g_evf_base1.wait_pattern_A = TEST_PATTERN_WAIT_A;
   g_evf_base1.wait_pattern_B = TEST_PATTERN_WAIT_B;
   g_evf_base1.wait_pattern_C = TEST_PATTERN_WAIT_C;

   g_evf_base1.tout_A = TN_WAIT_INFINITE;
   g_evf_base1.tout_B = TN_WAIT_INFINITE;
   g_evf_base1.tout_C = TN_WAIT_INFINITE;

   g_evf_base1.rc_check_A = TERR_NO_ERR_W;
   g_evf_base1.rc_check_B = TERR_NO_ERR_W;
   g_evf_base1.rc_check_C = TERR_NO_ERR_W;

   strcpy(g_evf_base1.test_str, "ABC");

   test_base_func_test_evf_base1(par);
}

static void test_base_func_test_evf_5(void * par)
{
   g_evf_base1.set_pattern   = 0x1U;
   g_evf_base1.check_pattern = 0U;
   g_evf_base1.wait_mode     = TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR;
   g_evf_base1.test_num      = 5;

   g_evf_base1.wait_pattern_A = 0x1U;
   g_evf_base1.wait_pattern_B = 0x1U;
   g_evf_base1.wait_pattern_C = 0x1U;

   g_evf_base1.tout_A = TN_WAIT_INFINITE;
   g_evf_base1.tout_B = 10UL;
   g_evf_base1.tout_C = TN_NO_WAIT;

   g_evf_base1.rc_check_A = TERR_NO_ERR_W;
   g_evf_base1.rc_check_B = TERR_TIMEOUT;
   g_evf_base1.rc_check_C = TERR_TIMEOUT;

   strcpy(g_evf_base1.test_str, "CAB");

   test_base_func_test_evf_base1(par);
}

//==== Eventflags Test base1 =================================================

static void test_base_func_test_evf_base1(void * par)
{
   int rc;

   //--- Create test data

   TestPrint("Eventflags Test %d - start.\r\n", g_evf_base1.test_num);

   g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
   g_curr_test.test_res_data_1[0] = 0;


   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10,                            // priority
                             test_task_A_func_test_evf_base1);   // task_func
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,                            // priority
                             test_task_B_func_test_evf_base1);   // task_func
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12,                            // priority
                             test_task_C_func_test_evf_base1);   // task_func
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - eventflag)

   rc = tn_eventflags_create(&g_evf_1, 0U);
   if(rc < 0)
      TestPrint("testEventflags - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Test
   tn_task_sleep(1);

   rc = tn_eventflags_set(&g_evf_1, g_evf_base1.set_pattern);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      if(g_evf_1.pattern == g_evf_base1.check_pattern)
      {
         g_curr_test.rc_base++;
      }
      check_test_evf_base1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   tn_eventflags_delete(&g_evf_1);

   test_end_signal_delete();
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_res_data_1);

   //---- Notify parent task about the test end

   TestPrint("Eventflags Test %d - end.\r\n", g_evf_base1.test_num);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_evf_base1(void * par)
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   rc = tn_eventflags_wait(&g_evf_1,
                           g_evf_base1.wait_pattern_A,  //unsigned int wait_pattern,
                           g_evf_base1.wait_mode, // unsigned int wait_mode,
                           NULL,  // unsigned int * p_flags_pattern,  // [OUT]
                           g_evf_base1.tout_A); // unsigned long timeout)

   if(rc == g_evf_base1.rc_check_A)
   {
      strcat((char*)ct->test_res_data_1, "A");
      tn_task_sleep(2);
   } 

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_evf_base1(void * par) 
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   rc = tn_eventflags_wait(&g_evf_1,
                           g_evf_base1.wait_pattern_B,  //unsigned int wait_pattern,
                           g_evf_base1.wait_mode, // unsigned int wait_mode,
                           NULL,  // unsigned int * p_flags_pattern,  // [OUT]
                           g_evf_base1.tout_B); // unsigned long timeout)

   if(rc == g_evf_base1.rc_check_B)
   {
      strcat((char*)ct->test_res_data_1, "B");
      tn_task_sleep(2);
   } 

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);

} 

static void test_task_C_func_test_evf_base1(void * par) 
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   rc = tn_eventflags_wait(&g_evf_1,
                           g_evf_base1.wait_pattern_C,  //unsigned int wait_pattern,
                           g_evf_base1.wait_mode, // unsigned int wait_mode,
                           NULL,  // unsigned int * p_flags_pattern,  // [OUT]
                           g_evf_base1.tout_C); // unsigned long timeout)

   if(rc == g_evf_base1.rc_check_C)
   {
      strcat((char*)ct->test_res_data_1, "C");
      tn_task_sleep(2);
   } 

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
} 

static  int check_test_evf_base1_result(CURRTEST * ct) 
{
   if(strcmp(g_evf_base1.test_str, (char*)ct->test_res_data_1) == 0 &&
                 g_curr_test.rc_base == 2)
   {
      TestPrint("Eventflags Test %d passed.\r\n",g_evf_base1.test_num);
   }
   else
   {
      TestPrint("Eventflags Test %d failed. Base: %d Str: %s\r\n", g_evf_base1.test_num,
                           g_curr_test.rc_base,
                           ct->test_res_data_1);
   }

   return 0;
} 

//==== Eventflags Test 6 =====================================================

static void test_base_func_test_evf_6(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;

   TestPrint("Eventflags Test 6 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

 //  g_curr_test.test_res_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 80);  
//   if(g_curr_test.test_res_data_1 == NULL)
//   {
//      TestPrint("g_curr_res.test_res_data_1 == NULL\r\n");
//      goto exit_l;  
//   }
//   g_curr_test.test_res_data_1[0] = 0;


   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_evf_6);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - eventflags)
   rc = tn_eventflags_create(&g_evf_1, 0U);
   if(rc < 0)
   {
      TestPrint("testEventflags - creation failed.\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);

//--- Base task test operating

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR) 
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_1;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   if(g_evf_1.pattern == TEST_PATTERN_SET_TEST_1)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_eventflags_clear(&g_evf_1, 0x1000U);
   if(rc == TERR_NO_ERR)
   {
      if(g_evf_1.pattern == 0x1000U)
      {
         g_curr_test.rc_base++;
      }
   }

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR) 
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_2;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   if(g_evf_1.pattern == TEST_PATTERN_SET_TEST_1)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_3;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   if(g_evf_1.pattern == ((~0x0011UL) & TEST_PATTERN_SET_TEST_1))
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_4;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   if(g_evf_1.pattern == ((~0x0111UL) & TEST_PATTERN_SET_TEST_1))
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_5;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(4);
   if(g_evf_1.pattern == ((~0x0111UL) & TEST_PATTERN_SET_TEST_1))
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   tx_data = (unsigned long)TEST_A_6;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
   if(g_evf_1.pattern == ((~0x0111UL) & TEST_PATTERN_SET_TEST_1))
   {
      g_curr_test.rc_base++;
   }

//--- Signal to task A to terminate

   tx_data = (unsigned long)TEST_A_EXIT;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   
   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_evf_6_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources

exit_l:

  // (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   tn_eventflags_delete(&g_evf_1);

   tn_dqueue_delete(gp_A_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Eventflags Test 6 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_evf_6(void * par) 
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;
   unsigned int p_flags_pattern = 0U;
   ct->rc_A = 0;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_A_1)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    TEST_PATTERN_SET_TEST_1,  //unsigned int wait_pattern,
                                    TN_EVENT_WCOND_AND, // unsigned int wait_mode,
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    TN_WAIT_INFINITE); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_2)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    TEST_PATTERN_SET_TEST_1,  //unsigned int wait_pattern,
                                    TN_EVENT_WCOND_OR, // unsigned int wait_mode,
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    TN_WAIT_INFINITE); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_3)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    0x0011,  //unsigned int wait_pattern,
                                    TN_EVENT_WCOND_OR | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    TN_WAIT_INFINITE); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_4)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    0x0111,  //unsigned int wait_pattern,
                                    TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    TN_WAIT_INFINITE); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_5)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    0x0111,  //unsigned int wait_pattern,
                                    TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    2); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_6)
         {
            rc = tn_eventflags_wait(&g_evf_1,
                                    0x0111,  //unsigned int wait_pattern,
                                    TN_EVENT_ATTR_BITCLR, // TN_EVENT_WCOND_AND - default
                                    &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                                    TN_NO_WAIT); // unsigned long timeout)
            if(rc == TERR_NO_ERR)
            { 
               ct->rc_A++;
               if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
               { 
                  ct->rc_A++;
               }
            }
         }
         else if(val == TEST_A_EXIT)
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

static int check_test_evf_6_result(CURRTEST * ct) 
{
   if(g_curr_test.rc_base == 13 && g_curr_test.rc_A == 12)
   {
      TestPrint("Eventflags Test 6 passed.\r\n");
   }
   else
   {
      TestPrint("Eventflags Test 6 failed. Base: %d A: %d\r\n",
                           g_curr_test.rc_base,
                           g_curr_test.rc_A);
   }

   return 0;
}

//==== Eventflags Test 7 =====================================================

static void test_base_func_test_evf_7(void * par)
{
   int rc;
//   int err;
//   unsigned long tx_data;

   TestPrint("Eventflags Test 7 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;
   g_curr_test.rc_int  = 0;

   //--- Create test object (here - eventflags)

   rc = tn_eventflags_create(&g_evf_1, 0U);
   if(rc < 0)
   {
      TestPrint("testEventflags - creation failed.\r\n");
      goto exit_l;  
   }

   //--- Start test

   //--- 1
   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_1;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 2
   rc = tn_eventflags_set(&g_evf_1, TEST_PATTERN_SET_TEST_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_2;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 3
   rc = tn_eventflags_set(&g_evf_1, 0x0011);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_3;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 4
   rc = tn_eventflags_set(&g_evf_1, 0x0111);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_4;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 5
   rc = tn_eventflags_set(&g_evf_1, 0xFFFFU);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_eventflags_clear(&g_evf_1, 0x1U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_5;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 6
   rc = tn_eventflags_set(&g_evf_1, 0x1111U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_6;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 7
   rc = tn_eventflags_set(&g_evf_1, 0x1111U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_7;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 8
   rc = tn_eventflags_set(&g_evf_1, 0x1111U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_8;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 9
   rc = tn_eventflags_clear(&g_evf_1, 0U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   g_curr_test.int_state = TEST_INT_9;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 10
   g_curr_test.int_state = TEST_INT_10;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

   //--- 11
   g_curr_test.int_state = TEST_INT_11;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);
   
   //--- 12
   g_curr_test.int_state = TEST_INT_12;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_evf_7);
   tn_task_sleep(10);

//------------ End - check results

   check_test_evf_7_result(&g_curr_test);  //--- Check test results

   //--- Clear resources

   tn_eventflags_delete(&g_evf_1);

exit_l:

   //---- Notify parent task about the test end

   TestPrint("Eventflags Test 7 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static int int_timer_func_test_evf_7(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned int p_flags_pattern;
   int cnt = 0;
   int rc;

   if(ct->int_state == TEST_INT_1)
   {
      //-- Wait with wait_mode = TN_EVENT_WCOND_AND
      rc = tn_eventflags_wait(&g_evf_1,
                              TEST_PATTERN_SET_TEST_1,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_AND, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_NO_ERR)
      { 
         cnt++;
         if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
         { 
            cnt++;
         }
      }
   }
   else if(ct->int_state == TEST_INT_2)
   {
      //-- Wait with wait_mode = TN_EVENT_WCOND_OR
      rc = tn_eventflags_wait(&g_evf_1,
                              TEST_PATTERN_SET_TEST_1,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_OR, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_NO_ERR)
      { 
         cnt++;
         if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
         { 
            cnt++;
         }
      }
   }
   else if(ct->int_state == TEST_INT_3)
   {
      //-- Wait with wait_mode = TN_EVENT_WCOND_OR | TN_EVENT_ATTR_BITCLR
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0011,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_OR | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_NO_ERR)
      { 
         cnt++;
         if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
         { 
            cnt++;
            if(g_evf_1.pattern == 0x1100U)
            {
               cnt++;
            }
         }
      }
   }
   else if(ct->int_state == TEST_INT_4)
   {
      //-- Wait with wait_mode = TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_NO_ERR)
      { 
         cnt++;
         if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
         { 
            cnt++;
            if(g_evf_1.pattern == 0x1000U)
            {
               cnt++;
            }
         }
      }
   }
   else if(ct->int_state == TEST_INT_5)
   {
      // Wait when NOT 'set' before
      // In this test, the pattern is clear to 0x0001 before
      // call tn_eventflags_wait()
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_TIMEOUT)
      { 
         cnt++;
         if(p_flags_pattern == 0x0001U)
         { 
            cnt++;
            if(g_evf_1.pattern == 0x0001U)
            {
               cnt++;
            }
         }
      }
   }
   else if(ct->int_state == TEST_INT_6)
   {
      //-- Wait with wait_mode = default (TN_EVENT_WCOND_AND)

      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111,  //unsigned int wait_pattern,
                              TN_EVENT_ATTR_BITCLR, // TN_EVENT_WCOND_AND - default
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_NO_ERR)
      { 
         cnt++;
         if(p_flags_pattern == TEST_PATTERN_SET_TEST_1)
         { 
            cnt++;
            if(g_evf_1.pattern == 0x1000U)
            {
               cnt++;
            }
         }
      }
   }
   else if(ct->int_state == TEST_INT_7)
   {
      //-- Wrong context - Timeout != 0
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111U,  //unsigned int wait_pattern,
                              TN_EVENT_WCOND_AND | TN_EVENT_ATTR_BITCLR, // unsigned int wait_mode,
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              2); // unsigned long timeout)
      if(rc == TERR_WCONTEXT)
      { 
         cnt++;
      }
   }
   else if(ct->int_state == TEST_INT_8)
   {
      //-- Wrong context - TN_WAIT_INFINITE
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111,  //unsigned int wait_pattern,
                              TN_EVENT_ATTR_BITCLR, // TN_EVENT_WCOND_AND - default
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_WAIT_INFINITE); // unsigned long timeout)
      if(rc == TERR_WCONTEXT)
      { 
         cnt++;
      }
   }
   else if(ct->int_state == TEST_INT_9)
   {
// Default mode(AND)
      rc = tn_eventflags_wait(&g_evf_1,
                              0x0111U,  //unsigned int wait_pattern,
                              TN_EVENT_ATTR_BITCLR, // TN_EVENT_WCOND_AND - default
                              &p_flags_pattern,  // unsigned int * p_flags_pattern,  // [OUT]
                              TN_NO_WAIT); // unsigned long timeout)
      if(rc == TERR_TIMEOUT)
      { 
         cnt++;
         if(p_flags_pattern == 0U)
         { 
            cnt++;
         }
      }

      //-- Set after wait
      rc = tn_eventflags_set(&g_evf_1,  0x1234U);
      if(rc == TERR_NO_ERR)
      {
         cnt++;
      }
   }
   else if(ct->int_state == TEST_INT_10)
   {
      //-- Wrong context - Create static
      g_evf_2.id_event = 0U;
      rc = tn_eventflags_create(&g_evf_2, 0U);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }

      g_evf_2.id_event = 1U;
      rc = tn_eventflags_create(&g_evf_2, 0U);
      if(rc == TERR_ILUSE)
      {
         cnt++;
      }
   }
   else if(ct->int_state == TEST_INT_11)
   {
      TN_EVENTFLAGS * evf;
      int err;

      //-- Wrong context - Create dynamic
      evf = tn_eventflags_create_dyn(0U,
                                    &err);   // [OUT]      
      if(evf == NULL && err == TERR_WCONTEXT)
      {
         cnt++;
      }
   }
   else if(ct->int_state == TEST_INT_12)
   {
      //-- Wrong context - Delete

      rc = tn_eventflags_delete(&g_evf_2);
      if(rc == TERR_NOEXS)
      {
         cnt++;
      }

      rc = tn_eventflags_delete(&g_evf_1);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
   }
   ct->rc_int += cnt;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;  // Stop int
}

static int check_test_evf_7_result(CURRTEST * ct)
{

   if(g_curr_test.rc_base == 10 && g_curr_test.rc_int == 26)
   {
      TestPrint("Eventflags Test 7 passed.\r\n");
   }
   else
   {
      TestPrint("Eventflags Test 7 failed. Base: %d int: %d\r\n",
                           g_curr_test.rc_base,
                           g_curr_test.rc_int);
   }

   return 0;
}

//==== Eventflags Test 8 =====================================================

static void test_base_func_test_evf_8(void * par)
{
   int rc;
   int err;

   //--- Create test data

   TestPrint("Eventflags Test 8 - start.\r\n");


   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10,                            // priority
                             test_task_A_func_test_evf_8);   // task_func
   if(task_A == NULL)
      TestPrint("task_A == NULL\r\n");

   task_B = create_test_task(11,                            // priority
                             test_task_B_func_test_evf_8);   // task_func
   if(task_B == NULL)
      TestPrint("task_B == NULL\r\n");

   task_C = create_test_task(12,                            // priority
                             test_task_C_func_test_evf_8);   // task_func
   if(task_C == NULL)
      TestPrint("task_C == NULL\r\n");

   //--- Create test object (here - eventflag)

   rc = tn_eventflags_create(&g_evf_1, 0U);
   if(rc < 0)
      TestPrint("testEventflags - creation failed.\r\n");

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Test
   tn_task_sleep(1);


   // Create stat - NULL
   rc = tn_eventflags_create(NULL, 0U);
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   // Create stat
   g_evf_2.id_event = 0UL;
   rc = tn_eventflags_create(&g_evf_2, 0U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   // Re Create stat
   rc = tn_eventflags_create(&g_evf_2, 0U);
   if(rc == TERR_ILUSE)
   {
      g_curr_test.rc_base++;
   }

   // Delete stat(NULL)
   rc = tn_eventflags_delete(NULL);
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   // Delete stat
   rc = tn_eventflags_delete(&g_evf_2);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   // Re-Delete stat
   rc = tn_eventflags_delete(&g_evf_2);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

   // Create dyn
   g_evf_dyn_1 = tn_eventflags_create_dyn(0U,
                                          &err);
   if(g_evf_dyn_1 != NULL && err == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   g_evf_dyn_2 = tn_eventflags_create_dyn(0U,
                                          NULL);
   if(g_evf_dyn_2 != NULL)
   {
      g_curr_test.rc_base++;
   }

   // Delete dyn(NULL)
   rc = tn_eventflags_delete(NULL);
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   // Delete dyn
   rc = tn_eventflags_delete(g_evf_dyn_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_delete(g_evf_dyn_2);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   // Re-Delete dyn

   rc = tn_eventflags_delete(g_evf_dyn_2);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }



   //--- Delete base eventflag

   rc = tn_eventflags_set(&g_evf_1, 0x8000U);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_eventflags_delete(&g_evf_1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_evf_8_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

  // tn_eventflags_delete(&g_evf_1);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Eventflags Test 8 - end.\r\n");

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_evf_8(void * par)
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
   rc = tn_eventflags_wait(&g_evf_1,
                           0x000AU,
                           TN_EVENT_WCOND_OR, // unsigned int wait_mode,
                           NULL,  // unsigned int * p_flags_pattern,  // [OUT]
                           TN_WAIT_INFINITE); // unsigned long timeout)

   if(rc == TERR_DLT)
   {
      ct->rc_A++;
   } 

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_evf_8(void * par) 
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = 0;
   rc = tn_eventflags_wait(&g_evf_1,
                           0x00B0U,
                           TN_EVENT_WCOND_OR, // unsigned int wait_mode,
                           NULL,  // unsigned int * p_flags_pattern,  // [OUT]
                           TN_WAIT_INFINITE); // unsigned long timeout)

   if(rc == TERR_DLT)
   {
      ct->rc_B++;
   } 

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_C_func_test_evf_8(void * par) 
{
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_C = 0;
   rc = tn_eventflags_wait(&g_evf_1,
                           0x0C00U,
                           TN_EVENT_WCOND_OR, // unsigned int wait_mode,
                           NULL,              // unsigned int * p_flags_pattern,  // [OUT]
                           TN_WAIT_INFINITE); // unsigned long timeout)
   if(rc == TERR_DLT)
   {
      ct->rc_C++;
   } 

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
} 

static  int check_test_evf_8_result(CURRTEST * ct) 
{
   if(g_curr_test.rc_base == 14 && 
       g_curr_test.rc_A == 1 &&
         g_curr_test.rc_B == 1 &&
           g_curr_test.rc_C == 1)
   {
      TestPrint("Eventflags Test 8 passed.\r\n");
   }
   else
   {
      TestPrint("Eventflags Test 8 failed. Base: %d A: %d B: %d C: %d\r\n", 
                            g_curr_test.rc_base,
                            g_curr_test.rc_A,
                            g_curr_test.rc_B,
                            g_curr_test.rc_C);
   }

   return 0;
} 

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
