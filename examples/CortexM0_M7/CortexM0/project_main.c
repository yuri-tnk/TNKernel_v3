/**
*
*  Copyright (c) 2013,2016 Yuri Tiomkin
*  All Rights Reserved
*
*
*  Permission to use, copy, modify, and distribute this software in source
*  and binary forms and its documentation for any purpose and without fee
*  is hereby granted, provided that the above copyright notice appear
*  in all copies and that both that copyright notice and this permission
*  notice appear in supporting documentation.
*
*
*  THIS SOFTWARE IS PROVIDED BY YURI TIOMKIN "AS IS" AND ANY EXPRESSED OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL YURI TIOMKIN OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
*  THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"
#include "shell.h"

/*
   This project is for the  NUCLEO-F070RB board (CPU STM32F070RBT6)

   The CPU uses HSE osc - the 8 MHz crystal was populated to the board.
   You can use 8 MHz clock source from ST-LINK 2.1 (just not forget to add 
   RCC_CR_HSEBYP in the function SetSysClock() )

    The console UART is UART2 (it has on-board connection to the ST-LINK 2.1
   virtual USB serial port)

*/

//----------- Tasks ----------------------------------------------------------

//-- The OS ticks task - must

#define  TASK_OS_TICK_PRIORITY        0
#define  OS_TICK_TASK_STACK_SIZE    128

static TN_TCB tn_os_tick_task;

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned int tn_os_tick_task_stack[OS_TICK_TASK_STACK_SIZE] TN_ALIGN_ATTR_END;

void tn_os_tick_task_func(void * param);

/*
*/

#define  TASK_UART2_RX_PRIORITY      3
#define  TASK_MAIN_PRIORITY          6
#define  TASK_IO_PRIORITY            8

#define  TASK_UART2_RX_STK_SIZE    256
#define  TASK_MAIN_STK_SIZE        512
#define  TASK_IO_STK_SIZE          256

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned int task_uart2_rx_stack[TASK_UART2_RX_STK_SIZE] TN_ALIGN_ATTR_END;

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned int task_main_stack[TASK_MAIN_STK_SIZE] TN_ALIGN_ATTR_END;

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned int task_io_stack[TASK_IO_STK_SIZE] TN_ALIGN_ATTR_END;

static TN_TCB  task_uart2_rx;
static TN_TCB  task_main;
static TN_TCB  task_io;

void task_uart2_rx_func(void * par);
void task_main_func(void * par);
void task_io_func(void * par);

void idle_hook_func(void * par);

//-------- Semaphores -----------------------

//-------- Queues -----------------------

#define QUEUE_ACTIONS_SIZE           8
#define QUEUE_ACTIONS_MEM_BUF_SIZE  (DQUEUE_ENTRY_SIZE * QUEUE_ACTIONS_SIZE)

TN_DQUEUE queueShellEvents;

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned char queueShellEventsMem[QUEUE_ACTIONS_MEM_BUF_SIZE]  TN_ALIGN_ATTR_END;

//-------- Misc -------

#define UART2_RX_BUF_SIZE  256

#if defined (__ICCARM__)    // IAR ARM
#pragma data_alignment=8
#endif
static unsigned char g_uart2_rx_buf[UART2_RX_BUF_SIZE] TN_ALIGN_ATTR_END;


UARTINFO g_uart2_info;
//static USART_InitTypeDef USART_InitStructure;


//--- Not OS globals

static BSP_SHELL_DS  g_shell_ds;
static BSP_SHELL_DS * g_p_shell_ds = &g_shell_ds;

static GPIO_TypeDef*  LED_PORT[LEDn] = {LED2_GPIO_PORT};
static const uint16_t LED_PIN[LEDn] = {LED2_PIN};


int main(void);
//----------------------------------------------------------------------------
int main(void)
{
   int rc;

   BSP_LED_Init(LED2);

   sh_init(g_p_shell_ds);

//-- 1ms config, Enable SysTick IRQ and SysTick Timer -inside 

   (void)SysTick_Config(48000);

   rc = tn_start_system(NULL,
                        0UL,
                        NULL); // NULL is OK for the project

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
int tn_app_init(void)
{
   int rc;

   tn_idle_task_hook_func = NULL;

#if 0
   USART_InitStructure.USART_BaudRate            = 115200;
   USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits            = USART_StopBits_1;
   USART_InitStructure.USART_Parity              = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
#endif
   rc = bsp_uart2_open(&g_uart2_info, 
                       //&USART_InitStructure,
                       g_uart2_rx_buf,
                       (unsigned int)UART2_RX_BUF_SIZE);
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
  //--- Task Rx UART2

   task_uart2_rx.id_task = 0UL;
   rc = tn_task_create(&task_uart2_rx,               //-- task TCB
                 task_uart2_rx_func,            //-- task function
                 TASK_UART2_RX_PRIORITY,        //-- task priority
                 &(task_uart2_rx_stack          //-- task stack first addr in memory
                    [TASK_UART2_RX_STK_SIZE-1]),
                 TASK_UART2_RX_STK_SIZE,        //-- task stack size (in int,not bytes)
                 NULL,                          //-- task function parameter
                 TN_TASK_START_ON_CREATION);    //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      goto err_exit;
   }
   //--- Task Main

   task_main.id_task = 0U;
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

   task_io.id_task = 0U;
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
   
   //--- Queues

   queueShellEvents.id_dqueue = 0UL;
   rc = tn_dqueue_create(&queueShellEvents,
                         QUEUE_ACTIONS_SIZE,
                         &queueShellEventsMem[0],  //-- Ptr to already existing array of void * to store data queue entries.Can be NULL
                         QUEUE_ACTIONS_MEM_BUF_SIZE);
   if(rc == TERR_NO_ERR)
   {
      goto err_exit;
   }

err_exit:
    
   return rc;
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
void task_uart2_rx_func(void * par)
{
#define DRV_UART2_RX_BUF_SIZE 32  
 
   unsigned char rx_buf[DRV_UART2_RX_BUF_SIZE];
   int i;
   int nbytes;

   for(;;)
   {
      // blocking reading , rx_timeout is default(TN_WAIT_INFINITE)
      nbytes =  bsp_uart2_read(&g_uart2_info, 
                              &rx_buf[0], 
                              DRV_UART2_RX_BUF_SIZE);    
      if(nbytes > 0)
      {
         for(i = 0; i < nbytes; i++)
         {
            sh_input(&g_p_shell_ds->shell_info, rx_buf[i]);
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
      rc = tn_dqueue_receive(&queueShellEvents, 
                             (TN_DQUEUE_ELEMENT *)((void*)&rx_data), 
                             TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         if(rx_data == (unsigned long)EVT_EXEC_SHELL)
         {
            (void)sh_do_cmd_exec(&g_p_shell_ds->shell_info);
         }  
         else
         {
            (void)uart2_tx_str((char*)("Unknown msg.\r\n"));
         }
      }
   }
}

//----------------------------------------------------------------------------
void task_io_func(void * par)
{
#define SBUF_SIZE 20
   unsigned int cnt = 0;
   unsigned int tmp;
   char sbuf[SBUF_SIZE];

   for(;;)
   {
      BSP_LED_Toggle(LED2);
      (void)tn_task_sleep(100); // 100 ms
      cnt++;
      if(cnt % 10U == 0U)
      { 
         (void)strcpy(sbuf, "Cnt: ");
         tmp = cnt/10U;  // To make MISRA 2012 happy
         do_itoa((int)tmp, sbuf + strlen(sbuf), SBUF_SIZE);
         (void)strcat(sbuf, "\r\n");

         (void)uart2_tx_str(sbuf);
      } 
   }   
}

//----------------------------------------------------------------------------
void BSP_LED_Init(Led_TypeDef Led)
{
   GPIO_InitTypeDef GPIO_InitStructure;  

  /* Enable GPIO clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure the GPIO_LED pin */

   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin   = LED_PIN[Led];

   GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);
}

//----------------------------------------------------------------------------
void BSP_LED_Toggle(Led_TypeDef Led)
{
   LED_PORT[Led]->ODR ^= LED_PIN[Led];
}
 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
