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

extern TN_DQUEUE * gp_A_dque;
extern TN_DQUEUE * gp_B_dque;
extern TN_DQUEUE * gp_C_dque;

//TN_SEM testSem_A;

TN_TIMER * p_Timer1;
TN_TIMER * p_Timer2;
TN_TIMER * p_Timer3;
TN_TIMER  g_Timer4;
TN_TIMER  g_Timer5;

//==== Timer Test 1 

static void test_base_func_test_timer_1(void * par);
static void test_task_A_func_test_timer_1(void * par); 
static  int check_test_timer_1_result(CURRTEST * ct); 
static void test1_timer_1_func(TN_TIMER * tmr, void * par); 
static void test1_timer_2_func(TN_TIMER * tmr, void * par); 
static void test1_timer_3_func(TN_TIMER * tmr, void * par); 
static void test1_timer_4_func(TN_TIMER * tmr, void * par); 
static void test1_timer_5_func(TN_TIMER * tmr, void * par); 


#define  TEST_TIMER_1      1
#define  TEST_TIMER_2      2
#define  TEST_TIMER_3      3
#define  TEST_TIMER_4      4
#define  TEST_TIMER_5      5
#define  TEST_EXIT       100


#define  TEST_TIMER_1_NUM_ACTIONS      1
#define  TEST_TIMER_1_PREV_IDX         6
#define  TEST_TIMER_1_CURR_IDX         7
//delta
#define  TEST_TIMER_1_FIRST            3   // for 100 ms -20
#define  TEST_TIMER_1_PERIODIC         5

#define  TEST_TIMER_2_NUM_ACTIONS      2
#define  TEST_TIMER_2_PREV_IDX         8
#define  TEST_TIMER_2_CURR_IDX         9
//delta
#define  TEST_TIMER_2_FIRST           11  // for 100 ms - 1
#define  TEST_TIMER_2_PERIODIC         0

#define  TEST_TIMER_3_NUM_ACTIONS      3
#define  TEST_TIMER_3_PREV_IDX        10
#define  TEST_TIMER_3_CURR_IDX        11
//delta
#define  TEST_TIMER_3_FIRST           11 // for 100 ms - 1
#define  TEST_TIMER_3_PERIODIC         0

#define  TEST_TIMER_4_NUM_ACTIONS      4
#define  TEST_TIMER_4_PREV_IDX        12
#define  TEST_TIMER_4_CURR_IDX        13
//delta
#define  TEST_TIMER_4_FIRST            6   // for 100 ms - 1
#define  TEST_TIMER_4_PERIODIC         0

#define  TEST_TIMER_5_NUM_ACTIONS      5
#define  TEST_TIMER_5_PREV_IDX        14
#define  TEST_TIMER_5_CURR_IDX        15
//delta
#define  TEST_TIMER_5_FIRST            4   // for 100 ms - 16/17
#define  TEST_TIMER_5_PERIODIC         6

#define  TEST_MAX_IDX                 16

//==== Timer Test 2 

static void test_base_func_test_timer_2(void * par);
static void test_task_A_func_test_timer_2(void * par); 
static  int int_timer_func_test_timer_2(void * par);
static  int check_test_timer_2_result(CURRTEST * ct); 
static void test2_timer_1_func(TN_TIMER * tmr, void * par); 
//static void test2_timer_2_func(TN_TIMER * tmr, void * par); 
//static void test1_timer_3_func(TN_TIMER * tmr, void * par); 
//static void test1_timer_4_func(TN_TIMER * tmr, void * par); 
//static void test1_timer_5_func(TN_TIMER * tmr, void * par); 

//================ All timer tests to run ============

const test_func g_timer_tests_arr[] =
{
   test_base_func_test_timer_1,
   test_base_func_test_timer_2,

   NULL // Terminator
};

//=============== Timer Test 1 =======================

static void test_base_func_test_timer_1(void * par)
{
   int err;
   int rc;
   int i;
   unsigned long * arr;

   //--- Create test data

   TestPrint("Timer Test 1 - start.\r\n");

   g_curr_test.test_data_1 = (unsigned char *)tn_alloc(&g_test_mem, 
                                     TEST_MAX_IDX * sizeof(unsigned long));  
   arr = (unsigned long *)g_curr_test.test_data_1;
   for(i=0;i<TEST_MAX_IDX; i++)
   {
      arr[i] = 0UL;
   }


   g_curr_test.rc_base = 0;
 
   p_Timer1 = tn_timer_create_dyn(test1_timer_1_func, NULL, &err);
   if(p_Timer1 == NULL || err != TERR_NO_ERR)
   {
      goto exit_err;
   }
   else
   {
      g_curr_test.rc_base++;
   }
   p_Timer2 = tn_timer_create_dyn(test1_timer_2_func, NULL, &err);
   if(p_Timer2 == NULL || err != TERR_NO_ERR)
   {
      goto exit_err;
   }
   else
   {
      g_curr_test.rc_base++;
   }
   p_Timer3 = tn_timer_create_dyn(test1_timer_3_func, NULL, &err);
   if(p_Timer3 == NULL || err != TERR_NO_ERR)
   {
      goto exit_err;
   }
   else
   {
      g_curr_test.rc_base++;
   }

   g_Timer4.id_timer = 0UL;
   rc = tn_timer_create(&g_Timer4, test1_timer_4_func, NULL);
   if(rc != TERR_NO_ERR)
   {
      goto exit_err;
   }
   else
   {
      g_curr_test.rc_base++;
   }
   
   g_Timer5.id_timer = 0UL;
   rc = tn_timer_create(&g_Timer5, test1_timer_5_func, NULL);
   if(rc != TERR_NO_ERR)
   {
      goto exit_err;
   }
   else
   {
      g_curr_test.rc_base++;
   }

   task_A = create_test_task(10, test_task_A_func_test_timer_1);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_err;
   }

   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_err;  
   }

 //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);

   arr[TEST_TIMER_1_CURR_IDX] = tn_get_sys_ticks();
   arr[TEST_TIMER_2_CURR_IDX] = tn_get_sys_ticks();
   arr[TEST_TIMER_3_CURR_IDX] = tn_get_sys_ticks();
   arr[TEST_TIMER_4_CURR_IDX] = tn_get_sys_ticks();
   arr[TEST_TIMER_5_CURR_IDX] = tn_get_sys_ticks();

   rc = tn_timer_set(p_Timer1,
                     TEST_TIMER_1_FIRST,    //        3
                     TEST_TIMER_1_PERIODIC);  //       5
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_set(p_Timer2,
                     TEST_TIMER_2_FIRST,    
                     TEST_TIMER_2_PERIODIC);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_set(p_Timer3,
                     TEST_TIMER_3_FIRST,    
                     TEST_TIMER_3_PERIODIC);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_set(&g_Timer4,
                     TEST_TIMER_4_FIRST,    
                     TEST_TIMER_4_PERIODIC);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_set(&g_Timer5,
                     TEST_TIMER_5_FIRST,    
                     TEST_TIMER_5_PERIODIC);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   tn_task_sleep(100);

   {
      unsigned long tx_data = TEST_EXIT;

      (void)tn_dqueue_send(gp_A_dque,
                           (TN_DQUEUE_ELEMENT)tx_data,
                          TN_NO_WAIT);
   }

   tn_task_sleep(2);

   rc = tn_timer_delete(p_Timer1);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_delete(p_Timer2);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_delete(p_Timer3);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_delete(&g_Timer4);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_timer_delete(&g_Timer5);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG );
   if(rc >= 0) // OK 
   {
      check_test_timer_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

exit_err:

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   test_end_signal_delete();

   tn_dqueue_delete(gp_A_dque);
   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   TestPrint("Timer Test 1 - end.\r\n");
   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_timer_1(void * par) 
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long * arr = (unsigned long *)ct->test_data_1;
   unsigned long delta;


   ct->rc_A = 0;
   for(rc =0; rc < RC_ARR_SIZE; rc++)
   {
      ct->rc_arr[rc] = 0;
   }

   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);

         if(val == TEST_TIMER_1)
         {
            arr[TEST_TIMER_1_PREV_IDX] = arr[TEST_TIMER_1_CURR_IDX];
            arr[TEST_TIMER_1_CURR_IDX] = tn_get_sys_ticks();
            delta = arr[TEST_TIMER_1_CURR_IDX] - arr[TEST_TIMER_1_PREV_IDX]; 
            if(arr[TEST_TIMER_1_NUM_ACTIONS] == 0UL) // First tick
            {
               if(delta == TEST_TIMER_1_FIRST || delta == TEST_TIMER_1_FIRST + 1)
               {
                 // ct->rc_A++;
                  ct->rc_arr[0]++;
               } 
            }
            else // Periodic ticks
            {
               if(delta == TEST_TIMER_1_PERIODIC || delta == TEST_TIMER_1_PERIODIC + 1)
               {
                  //ct->rc_A++;
                  ct->rc_arr[1]++;
               } 
            }
            arr[TEST_TIMER_1_NUM_ACTIONS]++;
         }
         else if(val == TEST_TIMER_2)  // Oneshot
         {
            arr[TEST_TIMER_2_PREV_IDX] = arr[TEST_TIMER_2_CURR_IDX];
            arr[TEST_TIMER_2_CURR_IDX] = tn_get_sys_ticks();
            delta = arr[TEST_TIMER_2_CURR_IDX] - arr[TEST_TIMER_2_PREV_IDX]; 
            if(delta == TEST_TIMER_2_FIRST || delta == TEST_TIMER_2_FIRST + 1)
            {
//                  ct->rc_A++;
                  ct->rc_arr[2]++;

            } 
         }
         else if(val == TEST_TIMER_3)  // Oneshot - same time as Timer 2
         {
            arr[TEST_TIMER_3_PREV_IDX] = arr[TEST_TIMER_3_CURR_IDX];
            arr[TEST_TIMER_3_CURR_IDX] = tn_get_sys_ticks();
            delta = arr[TEST_TIMER_3_CURR_IDX] - arr[TEST_TIMER_3_PREV_IDX]; 
            if(delta == TEST_TIMER_3_FIRST || delta == TEST_TIMER_3_FIRST + 1)
            {
//                  ct->rc_A++;
                  ct->rc_arr[3]++;
            } 
         }
         else if(val == TEST_TIMER_4)  // Oneshot
         {
            arr[TEST_TIMER_4_PREV_IDX] = arr[TEST_TIMER_4_CURR_IDX];
            arr[TEST_TIMER_4_CURR_IDX] = tn_get_sys_ticks();
            delta = arr[TEST_TIMER_4_CURR_IDX] - arr[TEST_TIMER_4_PREV_IDX]; 
            if(delta == TEST_TIMER_4_FIRST || delta == TEST_TIMER_4_FIRST + 1)
            {
//                  ct->rc_A++;
                  ct->rc_arr[4]++;
            } 
         }
         else if(val == TEST_TIMER_5)
         {
            arr[TEST_TIMER_5_PREV_IDX] = arr[TEST_TIMER_5_CURR_IDX];
            arr[TEST_TIMER_5_CURR_IDX] = tn_get_sys_ticks();
            delta = arr[TEST_TIMER_5_CURR_IDX] - arr[TEST_TIMER_5_PREV_IDX]; 
            if(arr[TEST_TIMER_5_NUM_ACTIONS] == 0UL) // First tick
            {
               if((delta == TEST_TIMER_5_FIRST) || (delta == TEST_TIMER_5_FIRST + 1))
               {
//                  ct->rc_A++;
                  ct->rc_arr[5]++;

               } 
            }
            else // Periodic ticks
            {
               if(delta == TEST_TIMER_5_PERIODIC || delta == TEST_TIMER_5_PERIODIC + 1)
               {
                  ct->rc_arr[6]++;
//                  ct->rc_A++;
               } 
            }
            arr[TEST_TIMER_5_NUM_ACTIONS]++;
         }
         else if(val == TEST_EXIT)
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

static int check_test_timer_1_result(CURRTEST * ct) 
{
   int i;
   ct->rc_A = 0;
   for(i=0;i<7;i++)
   {
      ct->rc_A += ct->rc_arr[i];
   }

   if((ct->rc_A == 40 || ct->rc_A == 41) && ct->rc_base == 15) // OK
   {
      TestPrint("Timer Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("Timer Test 1 failed. rc_base: %d A: %d\r\n", ct->rc_base, ct->rc_A);
#if 0
      TestPrint("rc_arr[0]: %d rc_arr[1]: %d rc_arr[2]: %d\r\n",
                 ct->rc_arr[0], ct->rc_arr[1], ct->rc_arr[2]);
      TestPrint("rc_arr[3]: %d rc_arr[4]: %d rc_arr[5]: %d rc_arr[6]: %d\r\n",
                 ct->rc_arr[3], ct->rc_arr[4], ct->rc_arr[5], ct->rc_arr[6]);
#endif

// rc_arr[0]: 0 rc_arr[1]: 19 rc_arr[2]: 1
// rc_arr[3]: 1 rc_arr[4]: 1 rc_arr[5]: 0 rc_arr[6]: 16
   }

   return 0;
}

static void test1_timer_1_func(TN_TIMER * tmr, void * par)
{
   unsigned long tx_data = TEST_TIMER_1;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);
} 

static void test1_timer_2_func(TN_TIMER * tmr, void * par) 
{
   unsigned long tx_data = TEST_TIMER_2;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);
} 
static void test1_timer_3_func(TN_TIMER * tmr, void * par) 
{
   unsigned long tx_data = TEST_TIMER_3;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);
} 
static void test1_timer_4_func(TN_TIMER * tmr, void * par) 
{
   unsigned long tx_data = TEST_TIMER_4;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);
} 
static void test1_timer_5_func(TN_TIMER * tmr, void * par) 
{
   unsigned long tx_data = TEST_TIMER_5;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);
} 


//==== Timer Test 2 ===============================================

static void test_base_func_test_timer_2(void * par)
{
   int err;
   int rc;

   //--- Create test data

   TestPrint("Timer Test 2 - start.\r\n");

   g_curr_test.rc_base = 0;

   task_A = create_test_task(10, test_task_A_func_test_timer_2);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_err;
   }

   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_A_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_err;  
   }

 //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
 
      //---- Wrong param

   p_Timer1 = tn_timer_create_dyn(NULL, NULL, &err);
   if(p_Timer1 == NULL && err == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   p_Timer1 = tn_timer_create_dyn(NULL, NULL, NULL);
   if(p_Timer1 == NULL && err == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   g_Timer4.id_timer = 0UL;
   rc = tn_timer_create(&g_Timer4, test1_timer_1_func, NULL);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   //-- Recreate
   rc = tn_timer_create(&g_Timer4, test1_timer_1_func, NULL);
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
   //-- Delete
   rc = tn_timer_delete(&g_Timer4);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   //-- Delete again
   rc = tn_timer_delete(&g_Timer4);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }
   //-- Op with not-exists 
   rc = tn_timer_set(&g_Timer4,      // Start/re-start timer 
                     2UL,            //   unsigned long due_ticks,     // timeout for the first action triggered (in sys ticks)
                     1UL);           //unsigned long period_ticks); // Periodic action time(if not 0)(in sys ticks)
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_timer_cancel(&g_Timer4);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

//--- Bad param op

   p_Timer2 = tn_timer_create_dyn(test2_timer_1_func, NULL, &err);
   if(p_Timer2 != NULL && err == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_timer_set(p_Timer2,      // Start/re-start timer 
                     0UL,      
                     1UL);     
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_timer_set(p_Timer2,      // Start/re-start timer 
                     10UL,      
                     TN_WAIT_INFINITE);     
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

//--- Operating

   rc = tn_timer_set(p_Timer2,      // Start/re-start timer 
                     2UL,      
                     3UL);     
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   long_job_delay(19);  // 21 -> 7 timer firing, 19-> 6

   rc = tn_timer_cancel(p_Timer2);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   tn_task_sleep(15);

  //--- Interrupt

   setup_test_int_timer(5,      // unsigned long timeout, 
                        int_timer_func_test_timer_2);

   tn_task_sleep(15);

//----- Deleting

   rc = tn_timer_delete(p_Timer2);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   {
      unsigned long tx_data = TEST_EXIT;

      (void)tn_dqueue_send(gp_A_dque,
                           (TN_DQUEUE_ELEMENT)tx_data,
                          TN_NO_WAIT);
   }

   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG );
   if(rc >= 0) // OK 
   {
      check_test_timer_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

exit_err:

   //--- Clear resources
      //-- Tasks A,B and C already finished and auto released resources

   test_end_signal_delete();
   tn_dqueue_delete(gp_A_dque);

   TestPrint("Timer Test 2 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);

}

static void test_task_A_func_test_timer_2(void * par) 
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

         if(val == TEST_TIMER_1)
         {
            ct->rc_A++;
         }
         else if(val == TEST_EXIT)
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

static  int int_timer_func_test_timer_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   //int err;

   ct->rc_int = 0;

   g_Timer4.id_timer = 0UL;
   rc = tn_timer_create(&g_Timer4, test2_timer_1_func, NULL);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }
   p_Timer3 = tn_timer_create_dyn(test2_timer_1_func, (void*)123U, NULL);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }

   //-- Existing timer
   rc = tn_timer_set(p_Timer2,      // Start/re-start timer 
                     2UL,            //   unsigned long due_ticks,     // timeout for the first action triggered (in sys ticks)
                     1UL);           //unsigned long period_ticks); // Periodic action time(if not 0)(in sys ticks)
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }

   rc = tn_timer_cancel(p_Timer2);
   if(rc == TERR_WCONTEXT)
   {
      ct->rc_int++;
   }

   rc = tn_timer_delete(p_Timer2);
   if(rc == TERR_WCONTEXT) 
   {
      ct->rc_int++;
   }

   test_arch_disable_timer_int();

   return TERR_NO_ERR;

}

static  int check_test_timer_2_result(CURRTEST * ct) 
{
   if((ct->rc_A == 7 || ct->rc_A == 6) && 
        ct->rc_base == 14 &&
          ct->rc_int == 5) 
   {
      TestPrint("Timer Test 2 passed.\r\n");
   }
   else
   {
      TestPrint("Timer Teat 2 failed. rc_base: %d A: %d Int: %d\r\n", 
                            ct->rc_base, ct->rc_A, ct->rc_int);
   }

   return 0;
}

static void test2_timer_1_func(TN_TIMER * tmr, void * par) 
{
   unsigned long tx_data = TEST_TIMER_1;

   (void)tn_dqueue_send(gp_A_dque,
                       (TN_DQUEUE_ELEMENT)tx_data,
                       TN_NO_WAIT);

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------







