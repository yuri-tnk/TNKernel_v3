/*
TNKernel real-time kernel

Copyright © 2006, 2013 Yuri Tiomkin
All rights reserved.

Permission to use, copy, modify, and distribute this software in source
and binary forms and its documentation for any purpose and without fee
is hereby granted, provided that the above copyright notice appear
in all copies and that both that copyright notice and this permission
notice appear in supporting documentation.

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
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

//#include <stdint.h>
//#include "core_cmFunc.h"
#include "LPC176x.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "lpc176x_uart.h"
#include "project_main.h"
#include "test.h"

void SysTick_Handler(void);
void UART0_IRQHandler(void);
void TMR1_IRQHandler(void);
     
extern CURRTEST g_curr_test;

#define VECTACTIVE  0x1FFUL
//----------------------------------------------------------------------------
//  SysTick timer (1 ms)
//----------------------------------------------------------------------------
void SysTick_Handler(void)
{
   tn_tick_int_processing();

  //-- !!!    For the Cortex CPU, this function always MUST be a last func in 
  //--     any user's interrupt handler

   tn_int_exit();  
}

//----------------------------------------------------------------------------
void UART0_IRQHandler(void)
{
   register unsigned int data;
   register int i;
   unsigned int status;

   UARTINFO * ui = &g_uart0_info;

   status = rU0IIR;  // Reset Int Request Source

   status &= (0x7U <<1U);

  //----- RX --------------------------------

   if(status == RX_INT || status == TO_INT)
   {
      i = UART_FIFO_SIZE;
      while(((rU0LSR & 0x01U) == 0x01U) && (i > 0)) //-- Rx FIFO not empty
      {
         data = rU0RBR;

         //!< check 'fifo is full'
         if((ui->rx_head == ui->rx_tail - 1U) ||
              (ui->rx_head == ui->rx_buf_size - 1U && ui->rx_tail == 0U))
         {
              //!< overflow
            ui->rx_buf[ui->rx_head] = (unsigned char)data;
            ui->rx_head++;
            if(ui->rx_head >= ui->rx_buf_size)
            {  
               ui->rx_head = 0U;
            }
            ui->rx_tail = ui->rx_head + 1U;
            if(ui->rx_tail >= ui->rx_buf_size)
            { 
               ui->rx_tail = 0U;
            }
         }
         else //!< no overflow
         {
            ui->rx_buf[ui->rx_head] = (unsigned char)data;
            ui->rx_head++;
            if(ui->rx_head >= ui->rx_buf_size)
            { 
               ui->rx_head = 0U;
            }
         }

         i--;
      }
   }

    //----- TX --------------------------------

   if(status == TX_INT)
   {
      rU0IER &= ~2U;  //-- Diable Tx int
      (void)tn_sem_signal(&ui->tx_rdy_sem);
   }

  //-- !!!    For the Cortex CPU, this function always MUST be a last func in 
  //--     any user's interrupt handler
   tn_int_exit();
}

//----------------------------------------------------------------------------
void TMR1_IRQHandler(void)
{
   int rc;
   rT1IR = 0xFF;                //-- clear int source

   test_arch_disable_timer_int();  //  LPC ARM 

   if(g_curr_test.test_int_handler != NULL)
   {
      rc = g_curr_test.test_int_handler(&g_curr_test);
      if(rc == TERR_NO_ERR) // Interrupt overall job was finished
      { 
         test_end_do_signal(INTERRUPT_RDY_FLAG);
      }
   }


  //-- !!!    For the Cortex CPU, this function always MUST be a last func in 
  //--     any user's interrupt handler
   tn_int_exit();
}

#if 0
static inline unsigned int __get_IPSR(void)
{
  register unsigned int __regIPSR = {r0};   
  asm("msr r0, ipsr");
  return(__regIPSR);
}
#endif

//----------------------------------------------------------------------------
// CPU specific routines for Cortex-M3
//
//----------------------------------------------------------------------------
void tn_cpu_int_enable(void)
{
   // For the project, do nothing here
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------





