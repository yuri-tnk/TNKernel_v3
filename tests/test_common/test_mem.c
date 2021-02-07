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
extern TN_DQUEUE * gp_A_dque;
extern TN_DQUEUE * gp_B_dque;
extern TN_DQUEUE * gp_C_dque;


TN_FMP    * gp_tfmp_fmp;
TN_DQUEUE * gp_tfmp_dque;

TN_FMP g_fmp;

#if defined (__ICCARM__) || defined (__ICC430__)   // IAR ARM
#pragma data_alignment=8
__no_init unsigned char g_fmp_pool[64];
#else
unsigned char g_fmp_pool[64] TN_ALIGN_ATTR_END;
#endif

//==== FMP Test 1

static void test_base_func_test_fmp_1(void * par);
static void test_task_A_func_test_fmp_1(void * par); 
static void test_task_B_func_test_fmp_1(void * par); 
static  int check_test_fmp_1_result(CURRTEST * ct); 

#define  TEST1_BLOCK_SIZE   22
#define  TEST1_BLOCK_NUM     5
#define  TEST1_NUM_CYCLES   100

//==== FMP Test 2

static void test_base_func_test_fmp_2(void * par);
static void test_task_A_func_test_fmp_2(void * par); 
static int  int_timer_func_test_fmp_2(void * par);
static  int check_test_fmp_2_result(CURRTEST * ct); 

#define  TEST2_NUM_CYCLES   100

//==== FMP Test 3

static void test_base_func_test_fmp_3(void * par);
static void test_task_A_func_test_fmp_3(void * par); 
static int  int_timer_func_test_fmp_3(void * par);
static  int check_test_fmp_3_result(CURRTEST * ct); 

//==== FMP Test 4

static void test_base_func_test_fmp_4(void * par);

#define  TEST4_BLOCK_SIZE           12  
#define  TEST4_BLOCK_NUM            23 

//==== FMP Test 5

static void test_base_func_test_fmp_5(void * par);
static void test_task_A_func_test_fmp_5(void * par); 
static void test_task_B_func_test_fmp_5(void * par); 
static int  int_timer_func_test_fmp_5(void * par);
static  int check_test_fmp_5_result(CURRTEST * ct); 

#define  TEST5_BLOCK_SIZE            9  
#define  TEST5_BLOCK_SIZE_ACTUAL    12  
#define  TEST5_BLOCK_NUM             3

#define  TEST_FMP_A_1             1   
#define  TEST_FMP_A_2             2   
#define  TEST_FMP_A_3             3   
#define  TEST_FMP_A_4             4   
#define  TEST_FMP_A_5             5   
#define  TEST_FMP_A_6             6   

#define  TEST_FMP_A_EXIT        100

#define  TEST_FMP_B_1            11   
#define  TEST_FMP_B_2            12   
#define  TEST_FMP_B_3            13   
#define  TEST_FMP_B_4            14   
#define  TEST_FMP_B_5            15   
#define  TEST_FMP_B_6            16   

#define  TEST_FMP_B_EXIT        101
                                
#define  TEST_FMP_INT_1          21   
#define  TEST_FMP_INT_2          22   
#define  TEST_FMP_INT_3          23   
#define  TEST_FMP_INT_4          24   
#define  TEST_FMP_INT_5          25   
#define  TEST_FMP_INT_6          26   

//==== FMP Test 6

static void test_base_func_test_fmp_6(void * par);
static void test_task_A_func_test_fmp_6(void * par);
static int int_timer_func_fmp_6(void * par);
static  int check_test_fmp_6_result(CURRTEST * ct);
    
//==== FMP Test 7

static void test_base_func_test_fmp_7(void * par);
static void test_task_A_func_test_fmp_7(void * par);
static  int check_test_fmp_7_result(CURRTEST * ct);


//================ All FMP tests to run ============

const test_func g_fmp_tests_arr[] =
{
   test_base_func_test_fmp_1,
   test_base_func_test_fmp_2,
   test_base_func_test_fmp_3,
   test_base_func_test_fmp_4,
   test_base_func_test_fmp_5,
   test_base_func_test_fmp_6,
   test_base_func_test_fmp_7,

   NULL // Terminator
};

//=============== FMP Test 1 =======================

static void test_base_func_test_fmp_1(void * par)
{
   int rc;
   int err;

   TestPrint("FMP Test 1 - start.\r\n");

   //--- Create test data

   gp_tfmp_dque = tn_dqueue_create_dyn((unsigned long)TEST1_BLOCK_NUM, &err);
   if(gp_tfmp_dque == NULL)
   {
      TestPrint("gp_tfmp_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_fmp_1);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }
   task_B = create_test_task(11, test_task_B_func_test_fmp_1);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - fixed-sized memory pool)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST1_BLOCK_SIZE,
                                    (unsigned long)TEST1_BLOCK_NUM,
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      goto exit_l;  
   }

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
      check_test_fmp_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished and auto released resources

exit_l:

   tn_fmem_delete(gp_tfmp_fmp);
   tn_dqueue_delete(gp_tfmp_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("FMP Test 1 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_1(void * par)
{
   int i;
   int rc;
   unsigned long rx_data;
   unsigned char * rx_data_buf;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(i = 0; i < TEST1_NUM_CYCLES; i++)
   {
      rc = tn_dqueue_receive(gp_tfmp_dque,
                       (TN_DQUEUE_ELEMENT *)&rx_data,
                       TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         rx_data_buf = (unsigned char *)((void*)rx_data);
         rc = chk_random_buf(rx_data_buf);
         if(rc == TERR_NO_ERR)
         {
            ct->rc_A++;
         } 

         rc = tn_fmem_release(gp_tfmp_fmp, (void *) rx_data);
         if(rc >= 0) // OK
         {
            ct->rc_A++;
         } 
      }
   }   

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static void test_task_B_func_test_fmp_1(void * par)
{
   int i;
   int rc;
   unsigned char * rx_data_buf;
   unsigned long tx_data;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_B = 0;
 
   for(i = 0; i < TEST1_NUM_CYCLES; i++)
   {
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&rx_data_buf, TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         ct->rc_B++;
      } 

      fill_random_buf(rx_data_buf, (unsigned long)TEST1_BLOCK_SIZE);

      tx_data = (unsigned long)rx_data_buf;

      rc = tn_dqueue_send(gp_tfmp_dque,
                          (TN_DQUEUE_ELEMENT)tx_data,
                          TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         ct->rc_B++;
      }
   }   

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
} 

static int check_test_fmp_1_result(CURRTEST * ct)
{
   if(ct->rc_A == TEST1_NUM_CYCLES * 2 &&
       ct->rc_B == TEST1_NUM_CYCLES * 2)
   {
      TestPrint("FMP Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 1 failed. A: %d  B: %d\r\n", ct->rc_A, ct->rc_B);
   }

   return 0;
} 

//==== FMP Test 2 ============================================================

static void test_base_func_test_fmp_2(void * par)
{
   int rc;
   int err;

   TestPrint("FMP Test 2 - start.\r\n");

   //--- Create test data

   gp_tfmp_dque = tn_dqueue_create_dyn((unsigned long)TEST1_BLOCK_NUM, &err);
   if(gp_tfmp_dque == NULL)
   {
      TestPrint("gp_tfmp_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_fmp_2);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - fixed-sized memory pool)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST1_BLOCK_SIZE,
                                    (unsigned long)TEST1_BLOCK_NUM,
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - int timer & handler

   g_curr_test.timer_val = 2UL;
   g_curr_test.rc_int    = 0;
   g_curr_test.int_cnt   = 0;

   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_2);
   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_fmp_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

exit_l:

   tn_fmem_delete(gp_tfmp_fmp);
   tn_dqueue_delete(gp_tfmp_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("FMP Test 2 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_2(void * par)
{
   int i;
   int rc;
   unsigned char * rx_data_buf;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(i = 0; i < TEST2_NUM_CYCLES; i++)
   {
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&rx_data_buf, TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         ct->rc_A++;
      } 

      fill_random_buf(rx_data_buf, (unsigned long)TEST1_BLOCK_SIZE);

      rc = tn_dqueue_send(gp_tfmp_dque,
                          (TN_DQUEUE_ELEMENT)rx_data_buf,
                          TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         ct->rc_A++;
      }
   }   

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
} 

static int int_timer_func_test_fmp_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc = 2; // in progress
   int cnt = 0;
   TN_DQUEUE_ELEMENT rx_data;

   for(;;)
   {
      rc = tn_dqueue_receive(gp_tfmp_dque,
                             (TN_DQUEUE_ELEMENT*)&rx_data,
                             TN_NO_WAIT);
      if(rc >= 0) // OK
      {
         rc = chk_random_buf((unsigned char *)((void*)rx_data));
         if(rc == TERR_NO_ERR)
         {
            cnt++;
         } 
         rc = tn_fmem_release(gp_tfmp_fmp, (void *) rx_data);
         if(rc >= 0) // OK
         {
            cnt++;
         } 
      

         ct->int_cnt++;
      }
      else
      {
         break;
      }
   }

   ct->rc_int += cnt;

   if(ct->int_cnt >= TEST2_NUM_CYCLES)
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

static  int check_test_fmp_2_result(CURRTEST * ct)
{

   if(ct->rc_A == TEST2_NUM_CYCLES * 2 &&
       ct->rc_int == TEST2_NUM_CYCLES * 2)
   {
      TestPrint("FMP Test 2 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 2 failed. A: %d  int: %d\r\n", ct->rc_A, ct->rc_int);
   }

   return 0;
} 

//==== FMP Test 3 ============================================================

static void test_base_func_test_fmp_3(void * par)
{
   int rc;
   int err;

   TestPrint("FMP Test 3 - start.\r\n");

   //--- Create test data

   gp_tfmp_dque = tn_dqueue_create_dyn((unsigned long)TEST1_BLOCK_NUM, &err);
   if(gp_tfmp_dque == NULL)
   {
      TestPrint("gp_tfmp_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_fmp_3);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - fixed-sized memory pool)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST1_BLOCK_SIZE,
                                    (unsigned long)TEST1_BLOCK_NUM,
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - int timer & handler

   g_curr_test.timer_val = 1UL;
   g_curr_test.rc_int    = 0;
   g_curr_test.int_cnt   = 0;

   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_3);
   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_fmp_3_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

exit_l:

   tn_fmem_delete(gp_tfmp_fmp);
   tn_dqueue_delete(gp_tfmp_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("FMP Test 3 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_3(void * par)
{
   int i;
   int rc;
   TN_DQUEUE_ELEMENT * rx_data;

   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(i = 0; i < TEST2_NUM_CYCLES; i++)
   {
      rc = tn_dqueue_receive(gp_tfmp_dque,
                             (TN_DQUEUE_ELEMENT*)&rx_data,
                             TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         rc = chk_random_buf((unsigned char *)((void*)rx_data));
         if(rc == TERR_NO_ERR)
         {
            ct->rc_A++;
         } 

         rc = tn_fmem_release(gp_tfmp_fmp, (void *) rx_data);
         if(rc >= 0) // OK
         {
            ct->rc_A++;
         } 
      }
   }   

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static int int_timer_func_test_fmp_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc = 2; // in progress
   int cnt = 0;
   unsigned char * tx_data_buf;

   rc = tn_fmem_get(gp_tfmp_fmp, (void **)&tx_data_buf, TN_NO_WAIT);
   if(rc >= 0) // OK
   {
      cnt++;
   } 

   fill_random_buf(tx_data_buf, (unsigned long)TEST1_BLOCK_SIZE);

   rc = tn_dqueue_send(gp_tfmp_dque,
                       (TN_DQUEUE_ELEMENT)((void *)tx_data_buf),
                       TN_NO_WAIT);
   if(rc >= 0) // OK
   {
      cnt++;
   }

   ct->int_cnt++;
   ct->rc_int += cnt;

   if(ct->int_cnt >= TEST2_NUM_CYCLES)
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

static  int check_test_fmp_3_result(CURRTEST * ct)
{

   if(ct->rc_A == TEST2_NUM_CYCLES * 2 && 
       ct->rc_int == TEST2_NUM_CYCLES * 2)
   {
      TestPrint("FMP Test 3 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 3 failed. A: %d  int: %d\r\n", ct->rc_A, ct->rc_int);
   }

   return 0;
} 

//==== FMP Test 4 ============================================================

static void test_base_func_test_fmp_4(void * par)
{
   int rc;
   int err;
   int i;
   int cnt = 0;
   unsigned char ** test_arr;
   unsigned char * p_blk;


   TestPrint("FMP Test 4 - start.\r\n");

   //--- Create test data
   test_arr =  (unsigned char **)tn_alloc(&g_test_mem, TEST4_BLOCK_NUM * sizeof(unsigned char *));
   if(test_arr == NULL)
   {
      TestPrint("test_arr == NULL\r\n");
      goto exit_l;
   } 
   //--- Create tasks

   //--- Create test object (here - fixed-sized memory pool)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST4_BLOCK_SIZE,
                                    (unsigned long)TEST4_BLOCK_NUM,
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event


   //-- This test - No int timer & handler

   //---  test
   // store blocks in array
   for(i = 0; i < TEST4_BLOCK_NUM; i++)
   {
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&p_blk, TN_NO_WAIT);
      if(rc >= 0) // OK
      {
         cnt++;
      } 

      fill_random_buf(p_blk, (unsigned long)TEST4_BLOCK_SIZE);
      test_arr[i] = p_blk;
   }
   //--- Random read
   for(;;)
   {
      rc = find_occupied_cell(test_arr, TEST4_BLOCK_NUM);
      if(rc >= 0)
      {
         i = rc; 
         rc = chk_random_buf(test_arr[i]);
         if(rc == TERR_NO_ERR)
         {
            cnt++;
         } 
         test_arr[i] = NULL;  
      }
      else
      {
         break;
      }
   }

   //--- Check results

   if(cnt == TEST4_BLOCK_NUM * 2)
   {
      TestPrint("FMP Test 4 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 4 failed. cnt: %d\r\n", cnt);
   }

   //--- Clear resources

exit_l:

   tn_fmem_delete(gp_tfmp_fmp);
   (void)tn_dealloc(&g_test_mem, test_arr);

   //---- Notify parent task about the test end

   TestPrint("FMP Test 4 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

//==== FMP Test 5 ============================================================

static void test_base_func_test_fmp_5(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;

   TestPrint("FMP Test 5 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_int = 0;

   g_curr_test.test_data_1 = tn_alloc(&g_test_mem, 3 * sizeof(void*));
   if(g_curr_test.test_data_1 == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_A_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_tfmp_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }
   gp_B_dque = tn_dqueue_create_dyn(8UL, &err);
   if(gp_tfmp_dque == NULL)
   {
      TestPrint("gp_A_dque == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_fmp_5);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }
   task_B = create_test_task(11, test_task_B_func_test_fmp_5);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - fixed-sized memory pool)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST5_BLOCK_SIZE,  
                                    (unsigned long)TEST5_BLOCK_NUM,   // 3
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      goto exit_l;  
   }

   g_curr_test.rc_base = 0;
   if(gp_tfmp_fmp->id_fmp == (TN_ID_FSMEMORYPOOL | TN_DYN_CREATED))
      g_curr_test.rc_base++;
   if(gp_tfmp_fmp->block_size == (unsigned long)TEST5_BLOCK_SIZE_ACTUAL) 
      g_curr_test.rc_base++;
   if(gp_tfmp_fmp->num_blocks == (unsigned long)TEST5_BLOCK_NUM)
      g_curr_test.rc_base++;
   if(gp_tfmp_fmp->fblkcnt == (unsigned long)TEST5_BLOCK_NUM)
      g_curr_test.rc_base++; 

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);

//--- Base task test operating

// A - get all blocks from 'gp_tfmp_fmp'
   tx_data = (unsigned long)TEST_FMP_A_1;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// B tries to get a block and is blocked
   tx_data = (unsigned long)TEST_FMP_B_1;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// A releases item [0] -> B unblocks and gets it";
   tx_data = (unsigned long)TEST_FMP_A_2;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// A tries to get one more item with zero timeout, and receives TERR_UNDERFLOW immediately
   tx_data = (unsigned long)TEST_FMP_A_3;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// Interrupt tries to get_polling one more item, and receives TERR_UNDERFLOW immediately");
   g_curr_test.int_state = TEST_FMP_INT_1;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);
//A tries to get one more item with non-zero and non-infinite timeout
   tx_data = (unsigned long)TEST_FMP_A_4;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(8);
//A releases item [2] ");
   tx_data = (unsigned long)TEST_FMP_A_5;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// Interrupt tries to get_polling one more item, and succeeds");
   g_curr_test.int_state = TEST_FMP_INT_2;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);
//Interrupt releases item [2]"
   g_curr_test.int_state = TEST_FMP_INT_3;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);
//Interrupt tries to get, and succeeds
   g_curr_test.int_state = TEST_FMP_INT_4;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);
//B tries to get one more item and blocks"
   tx_data = (unsigned long)TEST_FMP_B_2;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
// Interrupt releases item [2] -> B unblocks and gets it"  
   g_curr_test.int_state = TEST_FMP_INT_5;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);
//B release_all_items" - see STATE  TEST_FMP_B_2
   //
//A tries to release already released item [2], and gets TN_RC_OVERFLOW"
   tx_data = (unsigned long)TEST_FMP_A_6;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);
//Interrupt tries to release already released item [2], and gets TN_RC_OVERFLOW"
   g_curr_test.int_state = TEST_FMP_INT_6;
   setup_test_int_timer(2UL,      // unsigned long timeout, 
                        int_timer_func_test_fmp_5);  // test_int_timer_func func, 
   tn_task_sleep(5);

//--- Signal to task A, B to terminate

   tx_data = (unsigned long)TEST_FMP_A_EXIT;
   tn_dqueue_send(gp_A_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tx_data = (unsigned long)TEST_FMP_B_EXIT;
   tn_dqueue_send(gp_B_dque, (TN_DQUEUE_ELEMENT)tx_data, TN_WAIT_INFINITE);
   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_fmp_5_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A,B already finished

exit_l:

   (void)tn_dealloc(&g_test_mem, g_curr_test.test_data_1);

   tn_fmem_delete(gp_tfmp_fmp);
   tn_dqueue_delete(gp_tfmp_dque);
   tn_dqueue_delete(gp_A_dque);
   tn_dqueue_delete(gp_B_dque);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("FMP Test 5 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_5(void * par)
{
   TN_DQUEUE_ELEMENT rx_data;
   int val;
   int i;
   TN_FMP ** fmp_arr;
   TN_FMP * p_fmp;
   CURRTEST * ct = (CURRTEST *)par;
   int rc;

   ct->rc_A = 0;
   fmp_arr = (TN_FMP ** )ct->test_data_1;
 
   for(;;)
   {
      rc = tn_dqueue_receive(gp_A_dque,
                            (TN_DQUEUE_ELEMENT*)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_FMP_A_1)
         {
// A - get all blocks from 'gp_tfmp_fmp'
            for(i = 0; i < 3; i++)
            {
               rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[i], TN_NO_WAIT);   
               if(rc >= 0) // OK
               {
                  ct->rc_A++;
               }  
            } 
         }
         else if(val == TEST_FMP_A_2)
         {
// A releases item [0] -> B unblocks and gets it";
            rc = tn_fmem_release(gp_tfmp_fmp, fmp_arr[0]);
            if(rc == TERR_NO_ERR_W)
            {
               ct->rc_A++;
            }   
         }
         else if(val == TEST_FMP_A_3)
         {
// A tries to get one more item with zero timeout, and receives TERR_UNDERFLOW immediately
           rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[0], TN_NO_WAIT);   
            if(rc == TERR_UNDERFLOW)
            {
               ct->rc_A++;
            }   
         }
         else if(val == TEST_FMP_A_4)
         {
//A tries to get one more item with non-zero and non-infinite timeout
            rc = tn_fmem_get(gp_tfmp_fmp, (void **)&p_fmp, 7);   
            if(rc == TERR_TIMEOUT)
            {
               ct->rc_A++;
            }   
         }
         else if(val == TEST_FMP_A_5)
         {
//A releases item [2] ");
            rc = tn_fmem_release(gp_tfmp_fmp, (void *)fmp_arr[2]);
            if(rc == TERR_NO_ERR)
            {
               ct->rc_A++;
            }   
         }
         else if(val == TEST_FMP_A_6)
         {
//A tries to release already released item [2], and gets TN_RC_OVERFLOW"
            rc = tn_fmem_release(gp_tfmp_fmp, (void *)fmp_arr[2]);
            if(rc == TERR_OVERFLOW)
            {
               ct->rc_A++;
            }   
         }
         else if(val == TEST_FMP_A_EXIT)
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

static void test_task_B_func_test_fmp_5(void * par)
{

   TN_DQUEUE_ELEMENT rx_data;
   int val;
   CURRTEST * ct = (CURRTEST *)par;
 //  TN_FMP * p_blk = NULL;
   TN_FMP ** fmp_arr;
   int rc;
   int i;

   fmp_arr = (TN_FMP ** )ct->test_data_1;
   ct->rc_B = 0;

   for(;;)
   {
      rc = tn_dqueue_receive(gp_B_dque,
                            (TN_DQUEUE_ELEMENT *)&rx_data,
                            TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_FMP_B_1)
         {
// B tries to get a block and is blocked
            rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[0], TN_WAIT_INFINITE);   
     // A releases item [0] -> B unblocks and gets it";
            if(rc == TERR_NO_ERR_W) // OK
               ct->rc_B++;
         }
         else if(val == TEST_FMP_B_2)
         {
//B tries to get one more item and blocks"
            rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[2], TN_WAIT_INFINITE);   
    // Interrupt releases item [2] -> B unblocks and gets it"   
    //B release_all_items" - see STATE  TEST_FMP_B_2
            if(rc == TERR_NO_ERR_W) 
            {                                               
               for(i = 0; i < 3; i++)
               {
                  rc = tn_fmem_release(gp_tfmp_fmp, fmp_arr[i]);   
                  if(rc >= 0) // OK
                  {
                     ct->rc_B++;
                  }  
               } 
            }                                               
         }
         else if(val == TEST_FMP_B_EXIT)
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
 
static int int_timer_func_test_fmp_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int cnt = 0;
   int rc;
   TN_FMP * p_blk = NULL;
   TN_FMP ** fmp_arr;

   fmp_arr = (TN_FMP ** )ct->test_data_1;

   if(ct->int_state == TEST_FMP_INT_1)
   {
// Interrupt tries to get_polling one more item, and receives TERR_UNDERFLOW immediately");
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&p_blk, TN_NO_WAIT); 
      if(rc == TERR_UNDERFLOW)
         cnt++;
   }
   else if(ct->int_state == TEST_FMP_INT_2)
   {
// Interrupt tries to get_polling one more item(item[2]), and succeeds");
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[2], TN_NO_WAIT); 
      if(rc == TERR_NO_ERR)
         cnt++;
   }
   else if(ct->int_state == TEST_FMP_INT_3)
   {
//Interrupt releases item [2]"
      rc = tn_fmem_release(gp_tfmp_fmp, (void *)fmp_arr[2]);   
      if(rc == TERR_NO_ERR) // OK
      {
         cnt++;
      }  
   }
   else if(ct->int_state == TEST_FMP_INT_4)
   {
//Interrupt tries to get, and succeeds
      rc = tn_fmem_get(gp_tfmp_fmp, (void **)&fmp_arr[2], TN_NO_WAIT); 
      if(rc == TERR_NO_ERR)
      {
         cnt++;
         if(gp_tfmp_fmp->fblkcnt == 0UL)
            cnt++;
      }
   }
   else if(ct->int_state == TEST_FMP_INT_5)
   {
// Interrupt releases item [2] -> B unblocks and gets it"  
      rc = tn_fmem_release(gp_tfmp_fmp, (void *)fmp_arr[2]);   
      if(rc == TERR_NO_ERR_W) // OK
      {
         cnt++;
      }  
   }
   else if(ct->int_state == TEST_FMP_INT_6)
   {
//Interrupt tries to release already released item [2], and gets TERR_OVERFLOW"
      rc = tn_fmem_release(gp_tfmp_fmp, (void *)fmp_arr[2]);   
      if(rc == TERR_OVERFLOW) // OK
      {
         cnt++;
      }  
   }

   ct->rc_int += cnt;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;  // Stop int
}

static  int check_test_fmp_5_result(CURRTEST * ct)
{
   if(ct->rc_A == 8 && ct->rc_B == 4 && ct->rc_int == 7)
   {
      TestPrint("FMP Test 5 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 5 failed. Base: %d A: %d  B: %d  int: %d\r\n", 
                                ct->rc_base, ct->rc_A, ct->rc_B, ct->rc_int);
   }

   return 0;
} 

//==== FMP Test 6 ===========================================================

static void test_base_func_test_fmp_6(void * par)
{
   int rc;
   int err;

/*
N6 - get/release bad param
    wrong context 
*/
   TestPrint("FMP Test 6 - start.\r\n");

   //--- Create tasks

   gp_tfmp_fmp = NULL;
   task_A = create_test_task(10, test_task_A_func_test_fmp_6);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l; 
   }
   //--- Create test object (here - FMP)

   gp_tfmp_fmp = tn_fmem_create_dyn((unsigned long)TEST5_BLOCK_SIZE,  
                                    (unsigned long)TEST5_BLOCK_NUM,   // 3
                                    &err);
   if(gp_tfmp_fmp == NULL)
   {
      TestPrint("gp_tfmp_fmp == NULL\r\n");
      tn_task_delete(task_A);
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - int timer & handler

   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_fmp_6_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

exit_l:
 
   if(gp_tfmp_fmp != NULL)
   {
      tn_fmem_delete(gp_tfmp_fmp);
      gp_tfmp_fmp = NULL;
   }
   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("FMP Test 6 - end.\r\n");
//tdump(&tn_objects_mem);


   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_6(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   TN_FMP * p_blk = (TN_FMP *)123UL;

   ct->rc_A = 0;

   rc = tn_fmem_get(NULL, (void**)&p_blk, TN_NO_WAIT);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_get(gp_tfmp_fmp, NULL, TN_WAIT_INFINITE);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_release(NULL, p_blk);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_release(gp_tfmp_fmp, NULL);
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   rc = tn_fmem_delete(gp_tfmp_fmp);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

   rc = tn_fmem_get(gp_tfmp_fmp, (void**)&p_blk, TN_WAIT_INFINITE);
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   p_blk = (TN_FMP *)123UL;
   rc = tn_fmem_release(gp_tfmp_fmp, (void*)p_blk);
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   ct->rc_int = 0;

   //--- Wrong context
   g_curr_test.int_state = TEST_FMP_INT_1;
   setup_test_int_timer(2,      // unsigned long timeout, 
                        int_timer_func_fmp_6);
   tn_task_sleep(10);

   g_curr_test.int_state = TEST_FMP_INT_2;
   setup_test_int_timer(2,      // unsigned long timeout, 
                        int_timer_func_fmp_6);

   tn_task_sleep(10);

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static int int_timer_func_fmp_6(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   TN_FMP * p_blk = (TN_FMP *)123UL;
   int cnt = 0;

   if(ct->int_state == TEST_FMP_INT_1)
   {
      rc = tn_fmem_get(gp_tfmp_fmp, (void**)&p_blk, TN_WAIT_INFINITE);
      if(rc == TERR_NOEXS)
         cnt++;

      rc = tn_fmem_get(gp_tfmp_fmp, (void**)&p_blk, 5);
      if(rc == TERR_NOEXS)
         cnt++;


      g_fmp.id_fmp = 0UL;
      rc = tn_fmem_create(&g_fmp,
                          g_fmp_pool, //  void * start_addr,
                          7UL,  // block_size,
                          5UL); // num_blocks
      if(rc == TERR_WCONTEXT)
         cnt++;
   }
   else if(ct->int_state == TEST_FMP_INT_2)
   {
      rc = tn_fmem_get(gp_tfmp_fmp, (void**)&p_blk, TN_NO_WAIT);
      if(rc == TERR_NOEXS)
         cnt++;

      p_blk = (TN_FMP *)123UL;
      rc = tn_fmem_release(gp_tfmp_fmp, (void*)p_blk);
      if(rc == TERR_NOEXS)
         cnt++;

      gp_tfmp_fmp = NULL;
   }

   ct->rc_int += cnt;

   test_arch_disable_timer_int();
   rc = TERR_NO_ERR;  //-- End of the test

   return rc;
}

static  int check_test_fmp_6_result(CURRTEST * ct)
{
   if(ct->rc_A == 7 && ct->rc_int == 5) // OK
   {
      TestPrint("FMP Test 6 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 6 failed. rc_int: %d A: %d\r\n", ct->rc_int, ct->rc_A);
   }

   return 0;
} 

//----------------------------------------------------------------------------

//==== FMP Test 7 ============================================================

static void test_base_func_test_fmp_7(void * par)
{
   int rc;
/*
  bad parameters create/delete
*/
   TestPrint("FMP Test 7 - start.\r\n");

   //--- Create tasks

   task_A = create_test_task(10,                              // priority
                             test_task_A_func_test_fmp_7);   // task_func
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;
   }
   //--- Create end test signal event

   test_end_signal_create();

   //-- This test - no int timer & handler

   setup_test_int_timer(0,  
                        NULL);

   //--- Start test

   tn_task_activate(task_A);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_fmp_7_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished and auto released resources

   test_end_signal_delete();

   //---- Notify parent task about the test end
exit_l:
   TestPrint("FMP Test 7 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_fmp_7(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   int rc;
   int err;

   ct->rc_A = 0;
   g_fmp.id_fmp = 0UL;

   //-- Good param - static

   rc = tn_fmem_create(&g_fmp,
                       g_fmp_pool, //  void * start_addr,
                       7UL,  // block_size,
                       5UL); // num_blocks
   if(rc == TERR_NO_ERR)
      ct->rc_A++;
   rc = tn_fmem_delete(&g_fmp);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

   gp_tfmp_fmp = tn_fmem_create_dyn(7UL,  // block_size,
                                    5UL,  // num_blocks
                                    &err);
   if(gp_tfmp_fmp != NULL)
      ct->rc_A++;
   rc = tn_fmem_delete(gp_tfmp_fmp);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;

     //-- Bad param

   g_fmp.id_fmp = 1UL;
   rc = tn_fmem_create(&g_fmp,
                       g_fmp_pool, //  void * start_addr,
                       7UL,  // block_size,
                       5UL); // num_blocks
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   g_fmp.id_fmp = 0UL;

   rc = tn_fmem_create(NULL,
                       g_fmp_pool, //  void * start_addr,
                       7UL,  // block_size,
                       5UL); // num_blocks
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_create(&g_fmp,
                       NULL, //  void * start_addr,
                       7UL,  // block_size,
                       5UL); // num_blocks
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_create(&g_fmp,
                       g_fmp_pool, //  void * start_addr,
                       7UL,  // block_size,
                       1UL); // num_blocks < 2
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   rc = tn_fmem_create(&g_fmp,
                       g_fmp_pool, //  void * start_addr,
                       1UL,  // block_size < sizeof(int)
                       4UL); // num_blocks < 2
   if(rc == TERR_WPARAM)
      ct->rc_A++;

   rc = tn_fmem_create(&g_fmp,
                       g_fmp_pool, //  void * start_addr,
                       7UL,  // block_size,
                       5UL); // num_blocks
   if(rc == TERR_NO_ERR)
      ct->rc_A++;
   rc = tn_sem_delete(NULL);
   if(rc == TERR_WPARAM)
      ct->rc_A++;
   rc = tn_fmem_delete(&g_fmp);
   if(rc == TERR_NO_ERR)
      ct->rc_A++;
   rc = tn_fmem_delete(&g_fmp);    // already deleted
   if(rc == TERR_NOEXS)
      ct->rc_A++;

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static int check_test_fmp_7_result(CURRTEST * ct)
{
   if(ct->rc_A == 13) // OK
   {
      TestPrint("FMP Test 7 passed.\r\n");
   }
   else
   {
      TestPrint("FMP Test 7 failed. A: %d\r\n", ct->rc_A);
   }

   return 0;
} 


#if 0

N1
---

Tasks A,B
Queue: 5
FMP  block_size: 10 blocks_num: 5
  A
    que_receive
    check_mem
    mem_release
  B
    100 times

    fmp_get_mem
    fill_mem
    que_send 

N2
---
 The same as N1, but Task A-> to Int

N3
---
 The same as N1, but Int -> Task A

N4 Random release
----
 A


   Find free cell
   if(exists)
   {  
      Alloc block
      Fill()
      Store it in the cell   
      Get >15 FMP.  
   }

   else
   {
      if(cnt > MAX)
      {
        end task A
      }
   }

   Get random num
   Find occuped cell (Random num)
   if(not occuped)
   {
     find((Random num - 1) -> 0)  
     if(not found)
     find((Random num + 1) -> MAX) 
   }  
   if(found)
   {
      Check contents
      free()
      cell-> NULL // Under CS
      cnt++;
      if(cnt > MAX)
      {
         end task B
      }   
   }  

 B
   Get random num
   Find occuped cell (Random num)
   if(not occuped)
   {
     find((Random num - 1) -> 0)  
     if(not found)
     find((Random num + 1) -> MAX) 
   }  
   if(found)
   {
      Check contents
      free()
      cell-> NULL // Under CS
      cnt++;
      if(cnt > MAX)
      {
         end task B
      }   
   }  

N5  
---
Done

N6
---

#endif
