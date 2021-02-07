/*

Copyright © 2004, 2016 Yuri Tiomkin
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

#include "LPC236x.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"
#include "lpc236x_uart.h"


//----------------------------------------------------------------------------
//   User routine to processing IRQ (for LPC2XX)
//----------------------------------------------------------------------------
void tn_cpu_irq_handler(void)
{
   int_func pf;
   pf = tn_ul_to_int_func(rVICVectAddr);
   if(pf != NULL)
   {
      pf();
   }
}

//----------------------------------------------------------------------------
void int_func_timer0(void)
{
   rT0IR = 0xFF;                //-- clear int source

   tn_tick_int_processing();    //-- OS timer ticks

   rVICVectAddr = 0xFF;
}

//----------------------------------------------------------------------------
void int_func_default(void)
{
   rVICVectAddr = 0xFF;
}

//----------------------------------------------------------------------------
void int_func_uart0(void)
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
      rU0IER &= ~2U;  //-- Disable Tx int
      (void)tn_sem_signal(&ui->tx_rdy_sem);
   }

   rVICVectAddr = 0xFF;
}

//----------------------------------------------------------------------------
void tn_cpu_int_enable(void)
{
   // Enable a peripheral Int (Sys_Tick etc.)here (if required),
   // The core Int is still disabled here

}
