/*

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


#include "io430.h"
  
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "uart.h"
#include "project_main.h"
#include "shell.h"
#include "test.h"

/*
   CPU - MSP430F5359, 20MHz,512K Flash, 66K RAM

   UART1 is used (38400 bauds, 1 stop bit, 8 bit, No parity)

   All shell commands - in the file \common\test_shell_func.c 
*/


//----------- Tasks ----------------------------------------------------------

//-- The OS ticks task - must

#define  TASK_OS_TICK_PRIORITY        0
#define  OS_TICK_TASK_STACK_SIZE    128

TN_TCB tn_os_tick_task;
unsigned int tn_os_tick_task_stack[OS_TICK_TASK_STACK_SIZE];
void tn_os_tick_task_func(void * param);

#define  TASK_UART1_RX_PRIORITY       3
#define  TASK_MAIN_PRIORITY           6
#define  TASK_IO_PRIORITY             8

#define  TASK_UART1_RX_STK_SIZE     512
#define  TASK_MAIN_STK_SIZE         512
#define  TASK_IO_STK_SIZE           128

__no_init unsigned int task_uart1_rx_stack[TASK_UART1_RX_STK_SIZE];
__no_init unsigned int task_main_stack[TASK_MAIN_STK_SIZE];
__no_init unsigned int task_io_stack[TASK_IO_STK_SIZE];

__no_init TN_TCB  task_uart1_rx;
__no_init TN_TCB  task_main;
__no_init TN_TCB  task_io;

void task_uart1_rx_func(void * par);
void task_main_func(void * par);
void task_io_func(void * par);

void idle_hook_func(void * par);

//-------- Semaphores -----------------------

TN_SEM * semTxUart1;             //!< UART1 semaphore 
TN_SEM  *semTxBufUart1;

//__no_init TN_SEM semRS485;      // for RS485 serial port
TN_SEM * semTestPrint;  // for TestPrint() func

//-------- Queues -----------------------

#define QUEUE_ACTIONS_SIZE  8

TN_DQUEUE  * queueShellEvents;

//-------- Misc -------
extern idle_task_hook_func  tn_idle_task_hook_func;

//--- Not OS globals

__no_init char g_test_print_buf[TESTPRINT_BUF_LEN];

__no_init BSP_UART_DS  g_uart_ds;
BSP_UART_DS * g_p_uart_ds = &g_uart_ds;     //!< global UART data

__no_init BSP_SHELL_DS  g_shell_ds;
BSP_SHELL_DS * g_p_shell_ds = &g_shell_ds;

#if defined  USE_DYN_OBJ

#define SYS_OBJ_MEM_SIZE  4096
#pragma data_alignment = 4
__no_init unsigned char sys_obj_mem_arr[SYS_OBJ_MEM_SIZE];

#endif

extern MEMINFO  g_test_mem;
extern __no_init unsigned char g_test_mem_buf[TEST_MEM_POOL_SIZE];
extern TN_SEM   g_test_rdy_sem;

//----------------------------------------------------------------------------
int tn_app_init(void)
{
   int rc;
   int err;

   tn_idle_task_hook_func = idle_hook_func;

   rc = tn_alloc_init(&g_test_mem, 
                       g_test_mem_buf, 
                       (unsigned long)TEST_MEM_POOL_SIZE);
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }

   //--- OS ticks task - must

   tn_os_tick_task.id_task = 0UL;
   rc = tn_task_create(&tn_os_tick_task,          //-- task TCB
                        tn_os_tick_task_func,      //-- task function
                        0,                         //-- task priority
                        &(tn_os_tick_task_stack    //-- task stack first addr in memory
                        [OS_TICK_TASK_STACK_SIZE - 1]),
                        OS_TICK_TASK_STACK_SIZE,   //-- task stack size (in int,not bytes)
                        NULL,                      //-- task function parameter
                        TN_TASK_OS_TICK);          //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }
  //--- Task Rx UART1

   task_uart1_rx.id_task = 0UL;
   rc = tn_task_create(&task_uart1_rx,               //-- task TCB
                 task_uart1_rx_func,            //-- task function
                 TASK_UART1_RX_PRIORITY,        //-- task priority
                 &(task_uart1_rx_stack          //-- task stack first addr in memory
                    [TASK_UART1_RX_STK_SIZE-1]),
                 TASK_UART1_RX_STK_SIZE,        //-- task stack size (in int,not bytes)
                 NULL,                          //-- task function parameter
                 TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }
   //--- Task Main

   task_main.id_task = 0;
   rc = tn_task_create(&task_main,                      //-- task TCB
                 task_main_func,                   //-- task function
                 TASK_MAIN_PRIORITY,               //-- task priority
                 &(task_main_stack                 //-- task stack first addr in memory
                    [TASK_MAIN_STK_SIZE-1]),
                 TASK_MAIN_STK_SIZE,               //-- task stack size (in int,not bytes)
                 NULL,                             //-- task function parameter
                 TN_TASK_START_ON_CREATION);       //-- Creation option
   if(rc != TERR_NO_ERR)
   { 
      goto err_exit;
   }
   //--- Task IO

   task_io.id_task = 0;
   rc = tn_task_create(&task_io,                      //-- task TCB
                 task_io_func,                   //-- task function
                 TASK_IO_PRIORITY,               //-- task priority
                 &(task_io_stack                 //-- task stack first addr in memory
                    [TASK_IO_STK_SIZE-1]),
                 TASK_IO_STK_SIZE,               //-- task stack size (in int,not bytes)
                 NULL,                           //-- task function parameter
                 TN_TASK_START_ON_CREATION);     //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }

   //--- Semaphores

   semTxUart1 = tn_sem_create_dyn(0,      //-- Start value
                                  1,     //-- Max value
                                  &err); 
   if(semTxUart1 == NULL)
   {
      rc = err;
      goto err_exit;
   }

   semTxBufUart1 = tn_sem_create_dyn(1,   //-- Start value
                                     1,  //-- Max value
                                     &err);
   if(semTxBufUart1 == NULL)
   {
      rc = err;
      goto err_exit;
   }

   semTestPrint = tn_sem_create_dyn(1,  //-- Start value
                                    1,
                                    &err); //-- Max value
   if(semTestPrint == NULL)
   {
      rc = err;
      goto err_exit;
   }

   g_test_rdy_sem.id_sem = 0;
   rc = tn_sem_create(&g_test_rdy_sem,
                             0,  //-- Start value
                             1); //-- Max value
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }

   //--- Queues
   queueShellEvents =  tn_dqueue_create_dyn(QUEUE_ACTIONS_SIZE,        //-- Capacity of data queue(num entries).
                                            &err);
   if(queueShellEvents == NULL)
   {
      rc = err;
      goto err_exit;
   }

err_exit:
    
   return rc;
}

//----------------------------------------------------------------------------
int main(void)
{
   int rc;

   WDTCTL = WDTPW + (unsigned short)WDTHOLD;

   bsp_init_CPU();

   sh_init(g_p_shell_ds);

   (void)memset(g_p_uart_ds, 0, sizeof(BSP_UART_DS));

   sfsi_randomize(0x1234579FUL); 

   //-- if no err, should never return
 
   rc = tn_start_system(sys_obj_mem_arr,
                        (unsigned long) SYS_OBJ_MEM_SIZE,
                        NULL); // NULL is OK for the MSP430X  (int_stack_mem);

   // if we are here, something goes wrong

   if(rc != TERR_NO_ERR)
   {
      // Add your own error handler here, (if you are really need it)
      for(;;)
      {
      }
   }
   return 0;
}

//----------------------------------------------------------------------------
void tn_os_tick_task_func(void * param) // Must
{
   int rc;

   for(;;)
   {
      rc = tn_sem_acquire(&tn_sys_tick_sem, TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         (void)tn_os_timers_tick_proc();
#if defined USER_TIMERS
         (void)tn_user_timers_tick_proc();
#endif
      }
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void task_uart1_rx_func(void * par)
{
#define DRV_UART1_RX_BUF_SIZE 32  
 
   char rx_buf[DRV_UART1_RX_BUF_SIZE];
   int i;
   int nbytes;

   UARTDS * p_uart = &g_p_uart_ds->g_uart1_ds;

   (void)bsp_uart_open(p_uart,
                       NULL, // const char* name,
                       0,    // int32_t flags,
                       (long)CFG_UART1_38400_8N1_20MHZ);
   for(;;)
   {
      // blocking reading
      nbytes =  bsp_uart_read(p_uart, 
                              &rx_buf[0], 
                              DRV_UART1_RX_BUF_SIZE);    
      if(nbytes > 0)
      {
         for(i = 0; i < nbytes; i++)
         {
            sh_input(&g_p_shell_ds->shell_info,
                     rx_buf[i]);
         }
      }
   }
}

//----------------------------------------------------------------------------
void task_main_func(void * par)
{
   int rc;
   unsigned long rx_data;
 
   for(;;)
   {
      rc = tn_dqueue_receive(queueShellEvents, (TN_DQUEUE_ELEMENT *)&rx_data, TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         if(rx_data == (unsigned long)EVT_EXEC_SHELL)
         {
            (void)sh_do_cmd_exec(&g_p_shell_ds->shell_info);
         }  
         else
         {
            TestPrint("Unknown msg.\r\n");
         }
      }
   }
}

//----------------------------------------------------------------------------
void task_io_func(void * par)
{
   unsigned int cnt = 0;
   unsigned int cnt1 = 0;

   for(;;)
   {
      if((cnt & 1U) == 0U)
      {
        P4OUT |= (unsigned char)BIT0; 
      }
      else
      {
         P4OUT &= ~((unsigned char)BIT0); 
      }

      if((cnt % 4U) == 0U)
      {
      //   TestPrint("Hello %u\r\n", cnt1);
         cnt1++;
      }

      (void)tn_task_sleep(64); // ~ 256 ms
      
      cnt++;
   }
}

//----------------------------------------------------------------------------
void idle_hook_func(void * par)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

