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

#include "io430.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "types.h"
#include "uart.h"
#include "shell.h"


//----------- Tasks ----------------------------------------------------------

//-- The OS ticks task - must

#define  TASK_OS_SV_PRIORITY       0
#define  TASK_OS_SV_STACK_SIZE    256
TN_TCB *task_os_sv;
void task_os_sv_func(void * param);

/*
*/

#define  TASK_UART1_RX_PRIORITY     3
#define  TASK_SHELL_EXEC_PRIORITY   4
#define  TASK_A_PRIORITY            6
#define  TASK_B_PRIORITY            8


#define  TASK_UART1_RX_STK_SIZE    512
#define  TASK_SHELL_EXEC_STK_SIZE  512
#define  TASK_A_STK_SIZE           256
#define  TASK_B_STK_SIZE           256 

TN_TCB * task_uart1_rx;
TN_TCB * task_shell_exec;
TN_TCB * task_A;
TN_TCB * task_B;

void task_uart1_rx_func(void * par);
void task_shell_exec_func(void * par);
void task_A_func(void * par);
void task_B_func(void * par);

void idle_hook_func(void * par);

//-------- Semaphores -----------------------

TN_SEM * semTxUart1;             //!< UART1 semaphore 
TN_SEM * semTxBufUart1;

TN_SEM * semTestPrint;  // for TestPrint() func

//-------- Queues -----------------------

#define QUEUE_SHELL_EVENTS_SIZE  8 //-- Capacity of data queue(num entries).
TN_DQUEUE * queueShellEvents;

//-------- Mailboxes -----------------------

TN_MAILBOX * svMailbox;
TN_MAILBOX * taskAMailbox;
TN_MAILBOX * taskBMailbox;

//-------- Misc -------
extern idle_task_hook_func  tn_idle_task_hook_func;

//--- Not OS globals

__no_init BTNINFO_ST  g_btn_info_arr[NUM_BUTTONS];
BTNINFO_ST * p_g_btn_info_arr = &g_btn_info_arr[0];

__no_init char g_test_print_buf[TESTPRINT_BUF_LEN];

__no_init BSP_UART_DS  g_uart_ds;
BSP_UART_DS * g_p_uart_ds = &g_uart_ds;     //!< global UART data

__no_init BSP_SHELL_DS  g_shell_ds;
BSP_SHELL_DS * g_p_shell_ds = &g_shell_ds;

#if defined  USE_DYN_OBJ

#define SYS_OBJ_MEM_SIZE  8192 //4096
#pragma data_alignment = 4
__no_init unsigned char sys_obj_mem_arr[SYS_OBJ_MEM_SIZE] @ 0x2400;

#endif

volatile BOOL do_blinking_A = FALSE;
volatile BOOL do_blinking_B = FALSE;

BOOL bsp_check_deep_sleep_cond(void);
void bsp_switch_to_deep_sleep(void);
void bsp_switch_to_op_mode(void);

//----------------------------------------------------------------------------
int tn_app_init(void)
{
   int rc = TERR_NO_ERR;
   int err = 0;

   tn_idle_task_hook_func = idle_hook_func;

   task_os_sv = tn_task_create_dyn(task_os_sv_func,
                                   0U, // priority,
                                   TASK_OS_SV_STACK_SIZE, //task_stack_size, in sizeof(int)
                                   NULL, //param,
                                   TN_TASK_OS_TICK, // option 
                                   &err);
   if(task_os_sv == NULL)
   {
      goto err_exit;
   }

  //--- Task Rx UART1

   task_uart1_rx = tn_task_create_dyn(task_uart1_rx_func,
                                      TASK_UART1_RX_PRIORITY, // priority,
                                      TASK_UART1_RX_STK_SIZE, //task_stack_size, in sizeof(int)
                                      NULL, //param,
                                      TN_TASK_START_ON_CREATION, // option 
                                      &err);
   if(task_uart1_rx == NULL)
   {
      goto err_exit;
   }

  //--- Task Shell exec

   task_shell_exec = tn_task_create_dyn(task_shell_exec_func,
                                        TASK_SHELL_EXEC_PRIORITY, // priority,
                                        TASK_SHELL_EXEC_STK_SIZE, //task_stack_size, in sizeof(int)
                                        NULL, //param,
                                        TN_TASK_START_ON_CREATION, // option 
                                        &err);
   if(task_shell_exec == NULL)
   {
      goto err_exit;
   }

   //--- Task A

   task_A = tn_task_create_dyn(task_A_func,
                               TASK_A_PRIORITY, // priority,
                               TASK_A_STK_SIZE, //task_stack_size, in sizeof(int)
                               NULL, //param,
                               TN_TASK_START_ON_CREATION, // option 
                               &err);
   if(task_A == NULL)
   {
      goto err_exit;
   }

   //--- Task B

   task_B = tn_task_create_dyn(task_B_func,
                               TASK_B_PRIORITY, // priority,
                               TASK_B_STK_SIZE, //task_stack_size, in sizeof(int)
                               NULL, //param,
                               TN_TASK_START_ON_CREATION, // option 
                               &err);
   if(task_B == NULL)
   {
      goto err_exit;
   }

   //--- Semaphores

   semTxUart1 = tn_sem_create_dyn(0,      //-- Start value
                                  1,     //-- Max value
                                  &err);
   if(semTxUart1 == NULL)
   {
      goto err_exit;
   }

   semTxBufUart1 = tn_sem_create_dyn(1,      //-- Start value
                                     1,     //-- Max value
                                     &err);
   if(semTxBufUart1 == NULL)
   {
      goto err_exit;
   }

   //--- Queues

   queueShellEvents = tn_dqueue_create_dyn(QUEUE_SHELL_EVENTS_SIZE, //-- Capacity of data queue(num entries).
                                           &err);
   if(queueShellEvents == NULL)
   {
      goto err_exit;
   }

   //--- Mailboxex

   svMailbox = tn_mailbox_create_dyn(8UL,             //unsigned long num_elem,
                                     sizeof(SYS_MSG), //unsigned long elem_size,  
                                     &err);   // [OUT]   
   if(svMailbox == NULL)
   {
      goto err_exit;
   }

   taskAMailbox = tn_mailbox_create_dyn(8UL,             //unsigned long num_elem,
                                        sizeof(SYS_MSG), //unsigned long elem_size,  
                                        &err);   // [OUT]   
   if(taskAMailbox == NULL)
   {
      goto err_exit;
   }

   taskBMailbox = tn_mailbox_create_dyn(8UL,             //unsigned long num_elem,
                                        sizeof(SYS_MSG), //unsigned long elem_size,  
                                        &err);   // [OUT]   
   if(taskBMailbox == NULL)
   {
      goto err_exit;
   }

   return rc;

err_exit:

   rc = err;
   return rc;
}

//----------------------------------------------------------------------------
int main(void)
{
   int rc;

   WDTCTL = WDTPW + (unsigned short)WDTHOLD;

   bsp_init_CPU();

   bsp_buttons_init();
   sh_init(g_p_shell_ds);

   (void)memset(g_p_uart_ds, 0, sizeof(BSP_UART_DS));

   //-- if no err, should never return
 
   rc = tn_start_system(sys_obj_mem_arr,
                        (unsigned long) SYS_OBJ_MEM_SIZE,
                        NULL); // NULL is OK for the MSP430X  (int_stack_mem);

   // if we are here, something goes wrong

   if(rc != TERR_NO_ERR)
   {
      // Add your own error handler here
      for(;;)
      {
      }
   }
   return 0;
}

//----------------------------------------------------------------------------
void task_os_sv_func(void * param) // Must
{
   long res;
   SYS_MSG msg;

   for(;;)
   {
      res = tn_mailbox_receive(svMailbox,
                               &msg,                       /* [OUT] */
                               sizeof(SYS_MSG),            //max_len,
                               (unsigned long)(256 * 8));  // 8 sec
      if(res == (long)((unsigned long)sizeof(SYS_MSG)))
      {
         switch(msg.op_code)
         {
            case MSG_TICK_INT:
              (void)tn_os_timers_tick_proc();
               bsp_button_processing();
               break;
            case MSG_INT_BTN0:
               bsp_switch_to_op_mode(); //!< may come as a deep sleep wake-up interrupt
               break;
            case MSG_INT_BTN1:
               bsp_switch_to_op_mode(); //!< may come as a deep sleep wake-up interrupt
               break;
            default:
               break;
         }
      }
      else
      {
         if(res == TERR_TIMEOUT)
         {
         } 
         else // Fatal error, something goes wrong
         {
         }
      }
   }
}

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
      nbytes =  bsp_uart_read(p_uart, 
                              &rx_buf[0], 
                              DRV_UART1_RX_BUF_SIZE);    
      if(nbytes > 0)
      {
         for(i = 0; i < nbytes; i++)
         {
            sh_input(&g_p_shell_ds->shell_info, rx_buf[i]);
         }
      }
      else // Not open etc.
      {
         (void)tn_task_sleep(2); 
      }  
   }
}

//----------------------------------------------------------------------------
void task_shell_exec_func(void * par)
{
   int rc;
   unsigned long rx_data;
 
   for(;;)
   {
      rc = tn_dqueue_receive(queueShellEvents, 
                             &rx_data, 
                             TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         if(rx_data == (unsigned long)EVT_EXEC_SHELL)
         {
            (void)sh_do_cmd_exec(&g_p_shell_ds->shell_info);
         }  
         else if(rx_data == (unsigned long)EVT_REINIT_AFTER_DEEP_SL)
         {
            // Reopen UART after deep sleep - back to the full functionality

            UARTDS * p_uart = &g_p_uart_ds->g_uart1_ds;
            (void)bsp_uart_open(p_uart,
                                NULL, // const char* name,
                                0,    // int32_t flags,
                                (long)CFG_UART1_38400_8N1_20MHZ);
         } 
         else
         {
            (void)uart1_tx_str((char*)"Unknown Shell msg.\r\n");
         }
      }
   }
}

//----------------------------------------------------------------------------
void task_A_func(void * par)
{
   long res;
   SYS_MSG msg;
   unsigned int cnt = 0U;
   int op; 

   for(;;)
   {
      res = tn_mailbox_receive(taskAMailbox,
                               &msg,           
                               sizeof(SYS_MSG),
                               (unsigned long)(10));  // 39 msec
      if(res == (long)((unsigned long)sizeof(SYS_MSG)))
      {
         op = (int)((long)msg.data);  
         switch(op)
         {
            case MSG_BTN0_PRESSED:

               do_blinking_A = TRUE;
               break;

            case MSG_BTN0_RELEASED:

               do_blinking_A = FALSE;
               break;

            default:

               break;
         }
      }
      else
      {
         if(res == TERR_TIMEOUT)
         {
            if(do_blinking_A == TRUE)
            {
               if((cnt & 1U) == 0U)
               {
                  P4OUT |= (unsigned char)BIT0; 
               }
               else
               {
                  P4OUT &= ~((unsigned char)BIT0); 
               }
               cnt++;
            }
            else   
            { 
               P4OUT &= ~((unsigned char)BIT0); 
               cnt = 0U;
            }
         } 
         else // Fatal error, something goes wrong
         {
         }
      }
   }
}

//----------------------------------------------------------------------------
void task_B_func(void * par)
{
   long res;
   SYS_MSG msg;
   unsigned int cnt = 0U;
   int op; 

   for(;;)
   {
      res = tn_mailbox_receive(taskBMailbox,
                               &msg,           
                               sizeof(SYS_MSG),
                               (unsigned long)(10));  // 39 msec
      if(res == (long)((unsigned long)sizeof(SYS_MSG)))
      {
         op = (int)((long)msg.data);  
         switch(op)
         {
            case MSG_BTN1_PRESSED:

               do_blinking_B = TRUE;
               break;

            case MSG_BTN1_RELEASED:

               do_blinking_B = FALSE;
               break;

            default:

               break;
         }
      }
      else
      {
         if(res == TERR_TIMEOUT)
         {
            if(do_blinking_B == TRUE)
            {
               if((cnt & 1U) == 0U)
               {
                  P4OUT |= (unsigned char)BIT6; 
               }
               else
               {
                  P4OUT &= ~((unsigned char)BIT6); 
               }

               cnt++;
            }
            else   
            { 
               P4OUT &= ~((unsigned char)BIT6); 
               cnt = 0U;
            }
         } 
         else // Fatal error, something goes wrong
         {
         }
      }
   }
}


//----------------------------------------------------------------------------
void idle_hook_func(void * par)
{
   //wdt_restart();

P4OUT |= (unsigned char)BIT5;    // Hardware profiling


   if(bsp_check_deep_sleep_cond() == TRUE)
   {
      bsp_switch_to_deep_sleep();
   }

P4OUT &= ~((unsigned char)BIT5);  // Hardware profiling

   for(;;)
   {
      __bis_SR_register(LPM3_bits + GIE);
      __no_operation();                         // For debugger 
   }
}


//----------------------------------------------------------------------------
BOOL bsp_check_deep_sleep_cond(void)
{
   BOOL rc = FALSE;

 //!< Deep sleep condition - the system hasn't a job(s) to do now

   if(g_p_uart_ds->g_uart1_ds.baseAddress == 0UL && // UART is closed 
      tn_mailbox_is_empty(svMailbox) == TRUE &&
        tn_mailbox_is_empty(taskAMailbox) == TRUE &&
          tn_mailbox_is_empty(taskBMailbox) == TRUE &&
            tn_dqueue_is_empty(queueShellEvents) == TRUE)
   {
      rc = TRUE; 
   }   

   return rc;
}

//----------------------------------------------------------------------------
void bsp_switch_to_deep_sleep(void)
{

   P4OUT = (unsigned char)0U; // Turn off all LEDS

  // bsp_set_deep_sleep_pins();

   //!< enable buttons interrupts
      
       //!< P2.0 - Button 0
        
   P2DIR &= ~((unsigned char)BIT0);    
   P2IES |=   (unsigned char)BIT0;  //!< Int on falling edge
   P2IFG &= ~((unsigned char)BIT0);  //!< Clear int flag        
   P2IE  |=   (unsigned char)BIT0;  //!< Enable P2.0 interrupt
      
       //!< P2.1 - Button 1
        
   P2DIR &= ~((unsigned char)BIT1);    
   P2IES |=   (unsigned char)BIT1;  //!< Int on falling edge
   P2IFG &= ~((unsigned char)BIT1);  //!< Clear int flag        
   P2IE  |=   (unsigned char)BIT1;   //!< Enable P2.1 interrupt
         

   //<! disable RTC 1 sec interrupt 
      
  // RTCCTL01 &= ~RTCRDYIE;
      
   //!< disable OS tick interrupt 
      
   RTCPS0CTL &= ~((unsigned int)RT0PSIE);


//   wdt_set_long();   //-- Also wdt reset
}

//----------------------------------------------------------------------------
void bsp_switch_to_op_mode(void)
{
   TN_DQUEUE_ELEMENT tx_data;

// Check if we are wakeup from deep sleep mode - In this case Int RTC 1 sec and
// RTOS tick are disabled
      
   if((RTCCTL01 & (unsigned char)RTCRDYIE) == 0U)  //<! RTC 1 sec interrupt is disabled
   {
      if((RTCPS0CTL & (unsigned char)RT0PSIE) == 0U)  //!<  OS tick interrupt is disabled
      { 
    //  wdt_set_regular();   //-- Also wdt reset

       // we are wakeup from deep sleep mode
    //  dslp_read_cnt_after_dslp(&g_wdt_info, &g_dslp_profiling);
      }
   }
      
       //!< P2.0 - Button 0
        
   P2IFG &= ~((unsigned char)BIT0);  //!< Clear int flag        
   P2IE  &= ~((unsigned char)BIT0);  //!< Disable P2.0 interrupt
      
       //!< P2.1 - Button 1
        
   P2IFG &= ~((unsigned char)BIT1);  //!< Clear int flag        
   P2IE  &= ~((unsigned char)BIT1);  //!< Disable P2.1 interrupt

   
 //  RTCCTL01  |= RTCRDYIE;   //!< enable RTC 1 sec interrupt 
   RTCPS0CTL |= (unsigned char)RT0PSIE;    //!< enable OS tick interrupt 

   //-- Long operation - not in the 0-priority task

   tx_data = EVT_REINIT_AFTER_DEEP_SL;
   (void)tn_dqueue_send(queueShellEvents, 
                        tx_data, 
                        TN_NO_WAIT);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

