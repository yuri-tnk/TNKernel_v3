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

TN_MAILBOX * gp_A_mbox;
TN_MAILBOX * gp_B_mbox;
TN_MAILBOX * gp_C_mbox;

TN_MAILBOX * g_tst_mailbox_A;
TN_MAILBOX g_tst_mailbox_B;

#define MBOX_ELEM_SIZE_TEST_7  120

unsigned char g_tx_data[MBOX_ELEM_SIZE_TEST_7]; 
unsigned char g_rx_data[MBOX_ELEM_SIZE_TEST_7]; 


#define  QUE_DATA_1     0x77073096UL
#define  QUE_DATA_2     0xEE0E612CUL
#define  QUE_DATA_3     0x990951BAUL
#define  QUE_DATA_4     0x076DC419UL
#define  QUE_DATA_5     0xF3B97148UL
#define  QUE_DATA_6     0x84BE41DEUL
#define  QUE_DATA_7     0x1ADAD47DUL
#define  QUE_DATA_8     0x6DDDE4EBUL
#define  QUE_DATA_9     0xF4D4B551UL
#define  QUE_DATA_10    0x83D385C7UL

//==== Mailbox Test 1

static void test_base_func_test_mailbox_1(void * par);
static void test_task_A_func_test_mailbox_1(void * par); 
static  int check_test_mailbox_1_result(CURRTEST * ct); 

#define  MBOX_ELEM_SIZE_TEST_1    12

#define  TEST_DQUEUE_A_1   1
#define  TEST_DQUEUE_A_2   2
#define  TEST_DQUEUE_A_3   3


#define  TEST_DQUEUE_A_EXIT     100

//==== Mailbox Test 2

static void test_base_func_test_mailbox_2(void * par);
static int int_timer_func_test_mailbox_2(void * par);
static  int check_test_mailbox_2_result(CURRTEST * ct); 

#define TEST_DQUEUE_INT_1      1
#define TEST_DQUEUE_INT_2      2


//==== Mailbox Test 3

static void test_base_func_test_mailbox_3(void * par);
static int int_timer_func_test_mailbox_3(void * par);
static  int check_test_mailbox_3_result(CURRTEST * ct); 


//==== Mailbox Test 4

static void test_base_func_test_mailbox_4(void * par);
static void test_task_A_func_test_mailbox_4(void * par);
static void test_task_B_func_test_mailbox_4(void * par);
static void test_task_C_func_test_mailbox_4(void * par);
static  int check_test_mailbox_4_result(CURRTEST * ct); 

//==== Mailbox Test 5

static void test_base_func_test_mailbox_5(void * par);
static void test_task_A_func_test_mailbox_5(void * par);
static void test_task_B_func_test_mailbox_5(void * par);
static void test_task_C_func_test_mailbox_5(void * par);
static  int check_test_mailbox_5_result(CURRTEST * ct); 

//==== Mailbox Test 6

static void test_base_func_test_mailbox_6(void * par);
static  int check_test_mailbox_6_result(CURRTEST * ct); 


//==== Mailbox Test 7

static void test_base_func_test_mailbox_7(void * par);
static void test_task_A_func_test_mailbox_7(void * par);
static  int check_test_mailbox_7_result(CURRTEST * ct); 

#define  TEST7_EXIT     121

//================ All Mailbox tests to run ============

const test_func g_mailbox_tests_arr[] =
{
   test_base_func_test_mailbox_1,
   test_base_func_test_mailbox_2,
   test_base_func_test_mailbox_3,
   test_base_func_test_mailbox_4,
   test_base_func_test_mailbox_5,
   test_base_func_test_mailbox_6,
   test_base_func_test_mailbox_7,

   NULL // Terminator
};

//==== Mailbox Test 1

static void test_base_func_test_mailbox_1(void * par)
{
   int rc;
   int err;
   unsigned long tx_data;
   unsigned long rx_data;
   unsigned char mb_data_arr[MBOX_ELEM_SIZE_TEST_1];

   TestPrint("Mailbox Test 1 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   gp_A_mbox = tn_mailbox_create_dyn(8UL, 
                                     MBOX_ELEM_SIZE_TEST_1,
                                     &err);
   if(gp_A_mbox == NULL)
   {
      TestPrint("gp_A_mbox == NULL\r\n");
      goto exit_l;  
   }

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mailbox_1);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(1UL, //num_elem,
                                           MBOX_ELEM_SIZE_TEST_1, // elem_size,  
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
//   tn_task_activate(task_B);

//--- Base task test operating

    rc = tn_mailbox_receive(g_tst_mailbox_A,
                            mb_data_arr,
                            MBOX_ELEM_SIZE_TEST_1, //max_len
                            TN_NO_WAIT);
    if(rc == TERR_UNDERFLOW)
       g_curr_test.rc_base++;

    if(tn_mailbox_is_empty(g_tst_mailbox_A) == TRUE)
       g_curr_test.rc_base++;

    rc = tn_mailbox_receive(g_tst_mailbox_A,
                            mb_data_arr,
                            MBOX_ELEM_SIZE_TEST_1, //max_len
                            2);
    if(rc == TERR_TIMEOUT)
       g_curr_test.rc_base++;


// A - send  QUE_DATA_1, TN_WAIT_INFINITE
   tx_data = (unsigned long)TEST_DQUEUE_A_1;
   tn_mailbox_send(gp_A_mbox, 
                   &tx_data, 
                   sizeof(tx_data),
                   TN_WAIT_INFINITE);

   rc = tn_mailbox_receive(g_tst_mailbox_A,
                           &rx_data,
                           sizeof(tx_data), 
                           TN_WAIT_INFINITE);
   if(rc == (int)sizeof(tx_data))
   {
      if(rx_data == QUE_DATA_1)
       g_curr_test.rc_base++;
   }

//----
   tx_data = QUE_DATA_2;
   rc = tn_mailbox_send(g_tst_mailbox_A,
                        &tx_data,
                        sizeof(tx_data),
                        TN_NO_WAIT);
   if(rc == TERR_NO_ERR)
      g_curr_test.rc_base++;

   tx_data = (unsigned long)TEST_DQUEUE_A_2;
   tn_mailbox_send(gp_A_mbox,  
                   &tx_data, 
                   sizeof(tx_data),
                   TN_WAIT_INFINITE);
   tn_task_sleep(5);

   rc = tn_mailbox_receive(g_tst_mailbox_A,
                           &rx_data,
                           sizeof(rx_data),
                           TN_WAIT_INFINITE);
   if(rc == sizeof(rx_data))
   {
      if(rx_data == QUE_DATA_2)
         g_curr_test.rc_base++;
   }

//-------------------------------------------------

   tx_data = (unsigned long)TEST_DQUEUE_A_3;
   tn_mailbox_send(gp_A_mbox,  
                  &tx_data, 
                  sizeof(tx_data),
                  TN_WAIT_INFINITE);
   tn_task_sleep(2);

//--- Signal to task A to terminate

   tx_data = (unsigned long)TEST_DQUEUE_A_EXIT;
   tn_mailbox_send(gp_A_mbox,
                  &tx_data, 
                  sizeof(tx_data),
                  TN_WAIT_INFINITE);
   tn_task_sleep(2);

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_1_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished

exit_l:

   tn_mailbox_delete(g_tst_mailbox_A);

   tn_mailbox_delete(gp_A_mbox);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mailbox Test 1 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mailbox_1(void * par)
{
   unsigned long rx_data;
   unsigned long tx_data;
   int val;
//   int i;
   int rc;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(;;)
   {
      rc = tn_mailbox_receive(gp_A_mbox,
                              &rx_data,
                              sizeof(rx_data),
                              TN_WAIT_INFINITE);
      if(rc == sizeof(rx_data)) // OK
      {
         val = (int)((long)rx_data);
         if(val == TEST_DQUEUE_A_1)
         {
// A - send  QUE_DATA_1, TN_WAIT_INFINITE
            tx_data = QUE_DATA_1;
            rc = tn_mailbox_send(g_tst_mailbox_A,
                                 &tx_data,
                                 sizeof(tx_data),
                                 TN_WAIT_INFINITE);
            if(rc == TERR_NO_ERR)
            {
               ct->rc_A++;
            }  
         }
         else if(val == TEST_DQUEUE_A_2)
         {

// send_to_mailbox(TN_NO_WAIT)     rc = OVERFLOW
//   send_to_mailbox(2)             rc = TIMEOUT
            tx_data = QUE_DATA_2;
            rc = tn_mailbox_send(g_tst_mailbox_A,
                                 &tx_data,
                                 sizeof(tx_data),
                                 TN_NO_WAIT);
            if(rc == TERR_OVERFLOW)
               ct->rc_A++;

            rc = tn_mailbox_send(g_tst_mailbox_A,
                                 &tx_data,
                                 sizeof(tx_data),
                                 2);
            if(rc == TERR_TIMEOUT)
               ct->rc_A++;
         }
         else if(val == TEST_DQUEUE_A_3)
         {

            tx_data = QUE_DATA_3;
            rc = tn_mailbox_send(g_tst_mailbox_A,
                                 &tx_data,
                                 sizeof(tx_data),
                                 2);
            if(rc == TERR_NO_ERR)
               ct->rc_A++;
         }
         else if(val == TEST_DQUEUE_A_EXIT)
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

static  int check_test_mailbox_1_result(CURRTEST * ct)
{
   if(ct->rc_base == 6 && ct->rc_A == 4)
   {
      TestPrint("Mailbox Test 1 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 1 failed. Base: %d A: %d\r\n", 
                                ct->rc_base, ct->rc_A);
   }

   return 0;
} 

//==== Mailbox Test 2

static void test_base_func_test_mailbox_2(void * par)
{
   int rc;
   int err;
   int i;
   unsigned long tx_data;
   unsigned long rx_data;
   unsigned long num_entries;

   TestPrint("Mailbox Test 2 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;
   g_curr_test.rc_int  = 0;

   //--- Create tasks

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(5UL,
                                           MBOX_ELEM_SIZE_TEST_1, 
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test
  
   for(i=0;i < 5;i++) 
   {
      switch(i)
      {
         case 0:
            tx_data = QUE_DATA_1;
            break;
         case 1:
            tx_data = QUE_DATA_2;
            break;
         case 2:
            tx_data = QUE_DATA_3;
            num_entries = tn_mailbox_num_free_entries(g_tst_mailbox_A);
            if(num_entries == 3UL)
               g_curr_test.rc_base++;
            break;
         case 3:
            tx_data = QUE_DATA_4;
            break;
         case 4:
            tx_data = QUE_DATA_5;
            break;
         default:
            break;
      } 
      rc = tn_mailbox_send(g_tst_mailbox_A,
                          &tx_data,
                          sizeof(tx_data),
                          TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR)
      {
         g_curr_test.rc_base++;
      }
   }

   rc = tn_mailbox_send(g_tst_mailbox_A,
                        &tx_data,
                        sizeof(tx_data),
                        2);
   if(rc == TERR_TIMEOUT)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_mailbox_send(g_tst_mailbox_A,
                        &tx_data,
                        sizeof(tx_data),
                        TN_NO_WAIT);
   if(rc == TERR_OVERFLOW)
   {
      g_curr_test.rc_base++;
   }

   g_curr_test.int_state = TEST_DQUEUE_INT_1;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_mailbox_2);
   tn_task_sleep(10);

//------------------------------------------------
   g_curr_test.int_state = TEST_DQUEUE_INT_2;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_mailbox_2);
   tn_task_sleep(10);
//---------------------
   rc = tn_mailbox_receive(g_tst_mailbox_A,
                           &rx_data,
                           sizeof(rx_data),
                           TN_WAIT_INFINITE);
   if(rc == sizeof(rx_data))
   {
      if(rx_data == QUE_DATA_1)
         g_curr_test.rc_base++;
   }       

   for(i=1;i < 6;i++) 
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data),
                              TN_NO_WAIT);
      if(rc == sizeof(rx_data))
      {
         switch(i)
         {
            case 1:
               if(rx_data == QUE_DATA_2)
                  g_curr_test.rc_base++;
               break;
            case 2:
               if(rx_data == QUE_DATA_3)
                  g_curr_test.rc_base++;
               break;
            case 3:
               if(rx_data == QUE_DATA_4)
                  g_curr_test.rc_base++;
               break;
            case 4:
               if(rx_data == QUE_DATA_5)
                  g_curr_test.rc_base++;
               break;
            default:
               break;
         } 
      }
      else
      {
         if(rc == TERR_UNDERFLOW)
            g_curr_test.rc_base++;
         break;
      }
   }

   //--- Wait until all test participants finished
   rc = test_end_wait(INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_2_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }


   tn_mailbox_delete(g_tst_mailbox_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mailbox Test 2 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);

}

static int int_timer_func_test_mailbox_2(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long tx_data;
   unsigned long rx_data;
   int cnt = 0;
   int rc;
   int i;

   if(ct->int_state == TEST_DQUEUE_INT_1)
   {
//  until not empty  read_from_mailbox(TN_NO_WAIT) 
      for(i=0;;i++) 
      {
         rc = tn_mailbox_receive(g_tst_mailbox_A,
                                 &rx_data,
                                 sizeof(rx_data),
                                 TN_NO_WAIT);
         if(rc == sizeof(rx_data))
         {
            switch(i)
            {
               case 0:
                  if(rx_data == QUE_DATA_1)
                     cnt++;
                  break;
               case 1:
                  if(rx_data == QUE_DATA_2)
                     cnt++;
                  break;
               case 2:
                  if(rx_data == QUE_DATA_3)
                     cnt++;
                  break;
               case 3:
                  if(rx_data == QUE_DATA_4)
                     cnt++;
                  break;
               case 4:
                  if(rx_data == QUE_DATA_5)
                     cnt++;
                  break;
               default:
                  break;
            } 
         }
         else
         {
            if(rc == TERR_UNDERFLOW)
               cnt++;
            break;
         }
      }
   }
   else if(ct->int_state == TEST_DQUEUE_INT_2)
   {
//     until TERR_OVERFLOW   send_to_mailbox(TN_NO_WAIT) (5 times)

      for(i=0;;i++) 
      {
         switch(i)
         {
            case 0:
               tx_data = QUE_DATA_1;
               break;
            case 1:
               tx_data = QUE_DATA_2;
               break;
            case 2:
               tx_data = QUE_DATA_3;
               break;
            case 3:
               tx_data = QUE_DATA_4;
               break;
            case 4:
               tx_data = QUE_DATA_5;
               break;
            default:
               break;
         } 
         rc = tn_mailbox_send(g_tst_mailbox_A,
                             &tx_data,
                             sizeof(tx_data),
                             TN_NO_WAIT);
         if(rc == TERR_NO_ERR)
         {
            cnt++;
         }
         else
         {
            if(rc == TERR_OVERFLOW && i == 5)
               cnt++;
            break;
         }
      }
   }

   ct->rc_int += cnt;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;  // Stop int
}

static  int check_test_mailbox_2_result(CURRTEST * ct)
{
   if(ct->rc_base == 14 && ct->rc_int == 12)
   {
      TestPrint("Mailbox Test 2 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 2 failed. Base: %d int: %d\r\n", 
                                       ct->rc_base, ct->rc_int);
   }

   return 0;
} 


//==== Mailbox Test 3

static void test_base_func_test_mailbox_3(void * par)
{
   int rc;
   int err;
   TN_MAILBOX * tst_mailbox;
#define DATA_BUF_SIZE 16
   unsigned char  data_buf[DATA_BUF_SIZE];

   TestPrint("Mailbox Test 3 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;
   g_curr_test.rc_int  = 0;

   //--- Create tasks

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(5UL,
                                         MBOX_ELEM_SIZE_TEST_1, 
                                         &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test
  
//Create/delete  dyn/static

   tst_mailbox = tn_mailbox_create_dyn(2UL, 
                                       MBOX_ELEM_SIZE_TEST_1, 
                                       &err);
   if(tst_mailbox != NULL && err == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_mailbox_delete(tst_mailbox);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   tst_mailbox = tn_mailbox_create_dyn(2UL, 
                                       MBOX_ELEM_SIZE_TEST_1, 
                                       NULL);
   if(tst_mailbox != NULL)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_mailbox_delete((TN_MAILBOX*)((void*)data_buf));
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

   g_tst_mailbox_B.id_mailbox = 11UL;
   rc = tn_mailbox_create(&g_tst_mailbox_B,
                          1UL,              // num_elem,
                          7UL,              // elem_size,  
                          data_buf,
                          (unsigned long)DATA_BUF_SIZE);            // data_buf_size
   if(rc == TERR_ILUSE)
   {
      g_curr_test.rc_base++;
   }

   g_tst_mailbox_B.id_mailbox = 0UL;
   rc = tn_mailbox_create(NULL,
                          1UL,              // num_elem,
                          7UL,              // elem_size,  
                          data_buf,
                          (unsigned long)DATA_BUF_SIZE);            // data_buf_size
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   g_tst_mailbox_B.id_mailbox = 0UL;
   rc = tn_mailbox_create(&g_tst_mailbox_B,
                          0UL,              // num_elem,
                          7UL,              // elem_size,  
                          data_buf,
                          (unsigned long)DATA_BUF_SIZE);            // data_buf_size
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_mailbox_create(&g_tst_mailbox_B,
                          1UL,              // num_elem,
                          7UL,              // elem_size,  
                          NULL,
                          (unsigned long)DATA_BUF_SIZE);            // data_buf_size
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_mailbox_create(&g_tst_mailbox_B,
                          1UL,              // num_elem,
                          7UL,              // elem_size,  
                          data_buf,
                          2UL);            // data_buf_size
   if(rc == TERR_WPARAM)
   {
      g_curr_test.rc_base++;
   }

   g_tst_mailbox_B.id_mailbox = 0UL;
   rc = tn_mailbox_create(&g_tst_mailbox_B,
                          1UL,              // num_elem,
                          7UL,              // elem_size,  
                          data_buf,
                          (unsigned long)DATA_BUF_SIZE);            // data_buf_size
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }

   rc = tn_mailbox_delete(&g_tst_mailbox_B);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_mailbox_delete(tst_mailbox);
   if(rc == TERR_NO_ERR)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_mailbox_delete(&g_tst_mailbox_B);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }
   rc = tn_mailbox_delete(tst_mailbox);
   if(rc == TERR_NOEXS)
   {
      g_curr_test.rc_base++;
   }

   g_curr_test.int_state = TEST_DQUEUE_INT_1;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_mailbox_3);
   tn_task_sleep(10);
//------------------------------------------------
   g_curr_test.int_state = TEST_DQUEUE_INT_2;
   setup_test_int_timer(2UL,   // start timeout, 
                        int_timer_func_test_mailbox_3);
   tn_task_sleep(10);
//---------------------

   //--- Wait until all test participants finished
   rc = test_end_wait(INTERRUPT_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_3_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }


   tn_mailbox_delete(g_tst_mailbox_A);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mailbox Test 3 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);

}

static int int_timer_func_test_mailbox_3(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long rx_data;
   unsigned long tx_data = 1;
   int cnt = 0;
   int rc;
   int err;
   TN_MAILBOX * tst_mailbox;
   unsigned char data_buf[16];

   if(ct->int_state == TEST_DQUEUE_INT_1)
   {
// send/receive  int bad context

      rc = tn_mailbox_receive(g_tst_mailbox_A,
                             &rx_data,
                             sizeof(rx_data),
                             TN_WAIT_INFINITE);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                             &rx_data,
                             sizeof(rx_data),
                             2);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }

      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &tx_data,
                           sizeof(tx_data),
                           TN_WAIT_INFINITE);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &tx_data,
                           sizeof(tx_data),
                           2UL);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
   }
   else if(ct->int_state == TEST_DQUEUE_INT_2)
   {
// Create/delete  int bad context

      tst_mailbox = tn_mailbox_create_dyn(2UL, 
                                          5UL,
                                          &err);
      if(tst_mailbox == NULL && err == TERR_WCONTEXT)
      {
         cnt++;
      }

      g_tst_mailbox_B.id_mailbox = 0UL;
      rc = tn_mailbox_create(&g_tst_mailbox_B,
                             1UL,              // num_elem,
                             7UL,              // elem_size,  
                             data_buf,
                             16UL);            // data_buf_size
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
   
      rc = tn_mailbox_delete(g_tst_mailbox_A);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }

      rc = tn_mailbox_delete(tst_mailbox);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }

      rc = tn_mailbox_delete(&g_tst_mailbox_B);
      if(rc == TERR_WCONTEXT)
      {
         cnt++;
      }
   }

   ct->rc_int += cnt;

   test_arch_disable_timer_int();

   return TERR_NO_ERR;  // Stop int
}

static  int check_test_mailbox_3_result(CURRTEST * ct)
{
   if(ct->rc_base == 14 && ct->rc_int == 9)
   {
      TestPrint("Mailbox Test 3 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 3 failed. Base: %d int: %d\r\n", 
                                       ct->rc_base, ct->rc_int);
   }

   return 0;
} 

//==== Mailbox Test 4 =========================================================

static void test_base_func_test_mailbox_4(void * par)
{

   int rc;
   int err;
   unsigned long tx_data;
   //unsigned long rx_data;

   TestPrint("Mailbox Test 4 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mailbox_4);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }
   task_B = create_test_task(11, test_task_B_func_test_mailbox_4);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }
   task_C = create_test_task(12, test_task_C_func_test_mailbox_4);
   if(task_C == NULL)
   {
      TestPrint("task_C == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(1UL, 
                                           MBOX_ELEM_SIZE_TEST_1,
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tx_data = 1UL;
   rc = tn_mailbox_send(g_tst_mailbox_A,
                        &tx_data,
                        sizeof(tx_data), 
                        TN_WAIT_INFINITE);
   if(rc == TERR_NO_ERR)
      g_curr_test.rc_base++;

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Base task test operating

   tn_task_sleep(10);

   rc = tn_mailbox_delete(g_tst_mailbox_A);
   if(rc == TERR_NO_ERR)
       g_curr_test.rc_base++;

   //-- Check something ????

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_4_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources
      //-- Tasks A already finished

exit_l:

//   tn_mailbox_delete(g_tst_mailbox_A);
//   tn_mailbox_delete(gp_A_mbox);

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mailbox Test 4 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mailbox_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long tx_data = 1UL;
   int rc;

   ct->rc_A = 0;

   for(;;)
   {
      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &tx_data,
                           sizeof(tx_data),
                           TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_A++;
         break;   
      } 
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_mailbox_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long tx_data = 1UL;
   int rc;

   ct->rc_B = 0;

   for(;;)
   {
      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &tx_data,
                           sizeof(tx_data),
                           TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_B++;
         break;   
      } 
   }

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_C_func_test_mailbox_4(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long tx_data = 1UL;
   int rc;

   ct->rc_C = 0;

   for(;;)
   {
      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &tx_data,
                           sizeof(tx_data),
                           TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_C++;
         break;   
      } 
   }

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_mailbox_4_result(CURRTEST * ct) 
{
   if(ct->rc_base == 2 && ct->rc_A == 1 && ct->rc_B == 1 &&
       ct->rc_C == 1)
   {
      TestPrint("Mailbox Test 4 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 4 failed. Base: %d A: %d B: %d C: %d\r\n", 
                 ct->rc_base, ct->rc_A, ct->rc_B, ct->rc_C);
   }

   return 0;
} 

//==== Mailbox Test 5 =========================================================

static void test_base_func_test_mailbox_5(void * par)
{
   int rc;
   int err;

   TestPrint("Mailbox Test 5 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mailbox_5);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }
   task_B = create_test_task(11, test_task_B_func_test_mailbox_5);
   if(task_B == NULL)
   {
      TestPrint("task_B == NULL\r\n");
      goto exit_l;  
   }
   task_C = create_test_task(12, test_task_C_func_test_mailbox_5);
   if(task_C == NULL)
   {
      TestPrint("task_C == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(1UL, 
                                           MBOX_ELEM_SIZE_TEST_1,
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);
   tn_task_activate(task_B);
   tn_task_activate(task_C);

   //--- Base task test operating

   tn_task_sleep(10);

   rc = tn_mailbox_delete(g_tst_mailbox_A);
   if(rc == TERR_NO_ERR)
       g_curr_test.rc_base++;

   //-- Check something ????

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG | TASK_B_RDY_FLAG | TASK_C_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_5_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources

exit_l:

   test_end_signal_delete();

   //---- Notify parent task about the test end

   TestPrint("Mailbox Test 5 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static void test_task_A_func_test_mailbox_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long rx_data;
   int rc;

   ct->rc_A = 0;

   for(;;)
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data),  
                              TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_A++;
         break;   
      } 
   }

   test_end_do_signal(TASK_A_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_B_func_test_mailbox_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long rx_data;
   int rc;

   ct->rc_B = 0;

   for(;;)
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data),
                              TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_B++;
         break;   
      } 
   }

   test_end_do_signal(TASK_B_RDY_FLAG);
   tn_task_sleep(2);
}

static void test_task_C_func_test_mailbox_5(void * par)
{
   CURRTEST * ct = (CURRTEST *)par;
   unsigned long rx_data;
   int rc;

   ct->rc_C = 0;

   for(;;)
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data),
                              TN_WAIT_INFINITE);
      if(rc == TERR_DLT)
      {
         ct->rc_C++;
         break;   
      } 
   }

   test_end_do_signal(TASK_C_RDY_FLAG);
   tn_task_sleep(2);
}

static  int check_test_mailbox_5_result(CURRTEST * ct) 
{
   if(ct->rc_base == 1 && ct->rc_A == 1 && ct->rc_B == 1 && ct->rc_C == 1)
   {
      TestPrint("Mailbox Test 5 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 5 failed. Base: %d A: %d B: %d C: %d\r\n", 
                               ct->rc_base, ct->rc_A, ct->rc_B, ct->rc_C);
   }

   return 0;
} 


//==== Mailbox Test 6 =========================================================

static void test_base_func_test_mailbox_6(void * par)
{
   int err;
   int i;
   int rc;
   unsigned long rx_data;
   unsigned long tx_data;

   TestPrint("Mailbox Test 6 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(5UL, 
                                           MBOX_ELEM_SIZE_TEST_1,
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
   }

   //--- Start test

   // Send reg 1
   tx_data = QUE_DATA_1;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_NORMAL);
   // Send reg 2
   tx_data = QUE_DATA_2;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_NORMAL);
   // Send urgent 3
   tx_data = QUE_DATA_3;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send urgent 4
   tx_data = QUE_DATA_4;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);

     //-- Read sequence 4312

   for(i=0; i < 5; i++) 
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data), 
                              3);
      if(rc == sizeof(rx_data))
      {
         switch(i)
         {
            case 0:
               if(rx_data == QUE_DATA_4)
                  g_curr_test.rc_base++;
               break;
            case 1:
               if(rx_data == QUE_DATA_3)
                  g_curr_test.rc_base++;
               break;
            case 2:
               if(rx_data == QUE_DATA_1)
                  g_curr_test.rc_base++;
               break;
            case 3:
               if(rx_data == QUE_DATA_2)
                  g_curr_test.rc_base++;
               break;
            default:
               break;
         } 
      }
      else
      {
         if(rc == TERR_TIMEOUT && i== 4)
            g_curr_test.rc_base++;
      }
   }

   // Send urgent 1
   tx_data = QUE_DATA_1;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send reg 2                                
   tx_data = QUE_DATA_2;                        
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_NORMAL);
   // Send urgent 3                             
   tx_data = QUE_DATA_3;                        
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send reg 4                                
   tx_data = QUE_DATA_4;                        
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_NORMAL);

     //-- Read sequence 3124

   for(i=0; i < 5; i++) 
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data), 
                              3);
      if(rc == sizeof(rx_data))
      {
         switch(i)
         {
            case 0:
               if(rx_data == QUE_DATA_3)
                  g_curr_test.rc_base++;
               break;
            case 1:
               if(rx_data == QUE_DATA_1)
                  g_curr_test.rc_base++;
               break;
            case 2:
               if(rx_data == QUE_DATA_2)
                  g_curr_test.rc_base++;
               break;
            case 3:
               if(rx_data == QUE_DATA_4)
                  g_curr_test.rc_base++;
               break;
            default:
               break;
         } 
      }
      else
      {
         if(rc == TERR_TIMEOUT && i== 4)
            g_curr_test.rc_base++;
      }
   }

   // Send urgent 1
   tx_data = QUE_DATA_1;
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send urgent 2                            
   tx_data = QUE_DATA_2;                       
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send urgent 3                            
   tx_data = QUE_DATA_3;                       
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);
   // Send urgent 4                            
   tx_data = QUE_DATA_4;                       
   tn_mailbox_send_ex(g_tst_mailbox_A, &tx_data, sizeof(tx_data), TN_NO_WAIT, TN_SEND_URGENT);

     //-- Read sequence 4321
   for(i=0; i < 5; i++) 
   {
      rc = tn_mailbox_receive(g_tst_mailbox_A,
                              &rx_data,
                              sizeof(rx_data),
                              TN_NO_WAIT);
      if(rc == sizeof(rx_data))
      {
         switch(i)
         {
            case 0:
               if(rx_data == QUE_DATA_4)
                  g_curr_test.rc_base++;
               break;
            case 1:
               if(rx_data == QUE_DATA_3)
                  g_curr_test.rc_base++;
               break;
            case 2:
               if(rx_data == QUE_DATA_2)
                  g_curr_test.rc_base++;
               break;
            case 3:
               if(rx_data == QUE_DATA_1)
                  g_curr_test.rc_base++;
               break;
            default:
               break;
         } 
      }
      else
      {
         if(rc == TERR_UNDERFLOW && i== 4)
            g_curr_test.rc_base++;
      }
   }

   tn_task_sleep(10);

   check_test_mailbox_6_result(&g_curr_test);  //--- Check test results

   tn_mailbox_delete(g_tst_mailbox_A);

  //---- Notify parent task about the test end

   TestPrint("Mailbox Test 6 - end.\r\n");
//tdump(&tn_objects_mem);

   tn_sem_signal(&g_test_rdy_sem);

   tn_task_sleep(2);
}

static  int check_test_mailbox_6_result(CURRTEST * ct) 
{
   if(ct->rc_base == 15)
   {
      TestPrint("Mailbox Test 6 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 6 failed. Base: %d\r\n", ct->rc_base);
   }

   return 0;
}

//==== Mailbox Test 7 =========================================================

static void test_base_func_test_mailbox_7(void * par)
{

   int rc;
   int i;
   int err;
   unsigned long tx_data;

   TestPrint("Mailbox Test 7 - start.\r\n");

   //--- Create test data

   g_curr_test.rc_base = 0;

   //--- Create tasks

   task_A = create_test_task(10, test_task_A_func_test_mailbox_7);
   if(task_A == NULL)
   {
      TestPrint("task_A == NULL\r\n");
      goto exit_l;  
   }

   //--- Create test object (here - mailbox)

   g_tst_mailbox_A = tn_mailbox_create_dyn(5UL, 
                                           MBOX_ELEM_SIZE_TEST_7,
                                           &err);
   if(g_tst_mailbox_A == NULL)
   {
      TestPrint("g_tst_mailbox_A == NULL\r\n");
//      goto exit_l;  
   }

   //--- Create end test signal event

   test_end_signal_create();

   //--- Start test

   tn_task_activate(task_A);

   //--- Base task test operating

   tn_task_sleep(10);

   for(i = 0; i < 5; i++)
   {
      fill_random_buf(g_tx_data, (unsigned long)MBOX_ELEM_SIZE_TEST_7);
      rc = tn_mailbox_send(g_tst_mailbox_A,
                           &g_tx_data,
                           (unsigned long)MBOX_ELEM_SIZE_TEST_7, 
                           TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR)
        g_curr_test.rc_base++;
   }
   tn_task_sleep(10);

   tx_data = TEST7_EXIT;
   rc = tn_mailbox_send(g_tst_mailbox_A,
                        &tx_data,
                        sizeof(tx_data), 
                        TN_WAIT_INFINITE);
   if(rc == TERR_NO_ERR)
      g_curr_test.rc_base++;

   //--- Wait until all test participants finished
   rc = test_end_wait(TASK_A_RDY_FLAG);
   if(rc >= 0) // OK 
   {
      check_test_mailbox_7_result(&g_curr_test);  //--- Check test results
   }
   else
   {
      // To Do
   }

   //--- Clear resources

      //-- Tasks A already finished

exit_l:
//

   tn_mailbox_delete(g_tst_mailbox_A);

   test_end_signal_delete();

   TestPrint("Mailbox Test 7 - end.\r\n");
//tdump(&tn_objects_mem);

   //---- Notify parent task about the test end

   tn_sem_signal(&g_test_rdy_sem);
   tn_task_sleep(2);
}

static void test_task_A_func_test_mailbox_7(void * par)
{
   int rc;
   long res;
   unsigned long rx_data;
   CURRTEST * ct = (CURRTEST *)par;

   ct->rc_A = 0;
 
   for(;;)
   {
      res = tn_mailbox_receive(g_tst_mailbox_A,
                               &g_rx_data,
                               MBOX_ELEM_SIZE_TEST_7,
                               TN_WAIT_INFINITE);
      if(res > 0L) // got something
      {
         if(res == 4L)
         {
            memcpy(&rx_data, g_rx_data, sizeof(unsigned long));
            if(rx_data == TEST7_EXIT)
            {
               ct->rc_A++;
               break;
            }  
         }   
   
         rc = chk_random_buf(g_rx_data);
         if(rc == TERR_NO_ERR)
         {
            ct->rc_A++;
         } 
      } 
   }

   test_end_do_signal(TASK_A_RDY_FLAG);

   tn_task_sleep(2);
}

static  int check_test_mailbox_7_result(CURRTEST * ct) 
{
   if(ct->rc_base == 6 && ct->rc_A == 6)
   {
      TestPrint("Mailbox Test 7 passed.\r\n");
   }
   else
   {
      TestPrint("Mailbox Test 7 failed. Base: %d A: %d\r\n", ct->rc_base, ct->rc_A);
   }

   return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

