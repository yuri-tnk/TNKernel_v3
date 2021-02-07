/**
*
*  Copyright (c) 2004,2016 Yuri Tiomkin
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
#include "LPC236x.h" 
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"
#include "system_lpc236x.h" 
#include "lpc236x_uart.h"

#define UART_STATE_CLOSE          0
#define UART_STATE_OPEN           1
#define UART_STATE_IN_PROGRESS    2

#define UART_TER_TXEN			(1U<<7U) 		/*!< Transmit enable bit */

#define UART_TER_BITMASK		(0x80U)        /**< UART Transmit Enable Register bit mask */

#define UART0_IRQ_NUM    6

void int_func_uart0(void);

//----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------
int bsp_uart0_open(UARTINFO * ui, 
                  unsigned char * rx_buf,
                  unsigned int rx_buf_size)
{
   TN_INTSAVE_DATA

//   unsigned int div_val;
//   unsigned int cclk;
//   unsigned int baudrate;
   int rc = TERR_ILUSE;
   int state = UART_STATE_IN_PROGRESS;

   if(ui == NULL || rx_buf == NULL || rx_buf_size < 16U)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      tn_disable_interrupt();
      if(ui->state == UART_STATE_CLOSE)
      {
         ui->state = UART_STATE_IN_PROGRESS; //!< as a flag for 'open in progress'
         state = UART_STATE_CLOSE;
      } 
      tn_enable_interrupt();

      if(state == UART_STATE_CLOSE)
      { 
         ui->rx_tail     = 0UL;
         ui->rx_head     = 0UL;
         ui->rx_buf      = rx_buf;
         ui->rx_buf_size = rx_buf_size;
         ui->rx_timeout  = TN_WAIT_INFINITE;  // default value
         ui->rx_timeout_cnt = ui->rx_timeout;  

         // Create semaphores
         (void)memset(&ui->tx_sem, 0, sizeof(TN_SEM));
         (void)memset(&ui->tx_rdy_sem, 0, sizeof(TN_SEM));
         rc = tn_sem_create(&ui->tx_sem , 1, 1);
         if(rc == TERR_NO_ERR)
         {
            rc = tn_sem_create(&ui->tx_rdy_sem, 0, 1);
            if(rc == TERR_NO_ERR)
            {
               //-- Pins
               rPINSEL0 |= (0x01U<<4U) |  //-- P0.2  TXD0
                           (0x01U<<6U);   //-- P0.3  RXD0

               rPCLKSEL0 |= (0x01U << 6U); //-- bit 7:6 =01-> Clock div = 1 for UART0

               //-- enable access to divisor latch regs
               rU0LCR = LCR_ENABLE_LATCH_ACCESS;

               //-- set divisor for desired baud rate

	               // F_in = 57 600 000 Hz

	            //-- U0DLM =0, U0DLL =3, DIVADDVAL =3, MULVAL =10 UART0baudrate=923076.923077
	            //-- err = 0.160256 %

	            //-- U0DLM =0, U0DLL =25, DIVADDVAL =1, MULVAL =4, UART0baudrate=115200.000000
	            //-- err = 0.000000 %

               //-- set divisor for desired baud
               rU0FDR =  (0x04U<<4U)  //-- MULVAL
                               | 1U;  //-- DIVADDVAL

               rU0DLM = 0U;
               rU0DLL = 25U;

               //-- disable access to divisor latch regs (enable access to xmit/rcv fifos
               //-- and int enable regs)

               //-- disable access to divisor latch regs (enable access to xmit/rcv fifos
               //-- and int enable regs)

               rU0LCR = LCR_DISABLE_LATCH_ACCESS;

               rU0IER = 1U; //-- Enable Rx int

               rU0FCR = (0x3U<<6U) | 1U;  //-- Int Trigger - 14 bytes, Enable FIFO,Reset Tx FIFO & Rx FIFO

               //-- setup line control reg - disable break transmittion, even parity,
               //-- 1 stop bit, 8 bit chars

               rU0LCR = 0x13;//-- 0b00010011

               //---

//               NVIC_EnableIRQ(UART0_IRQn);  //-- It is a safe case global interrupts are not yet enabled
               (void)install_irq(UART0_IRQ_NUM,                //-- IntNumber for UART0
                                tn_func_to_ul_2(int_func_uart0),  //-- Handler
                                2);                            //-- Priority

               tn_disable_interrupt();
               ui->state = UART_STATE_OPEN;
               tn_enable_interrupt();
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
int bsp_uart0_close(UARTINFO *ui)
{
   TN_INTSAVE_DATA

   int rc = TERR_NO_ERR;
   int state = UART_STATE_IN_PROGRESS;

   if(ui == NULL)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      tn_disable_interrupt();

      if(ui->state == UART_STATE_OPEN)
      {
         ui->state = UART_STATE_IN_PROGRESS; 
         state = UART_STATE_OPEN;
      }
      tn_enable_interrupt();

      if(state == UART_STATE_OPEN) //
      { 
         DisableIRQ(UART0_IRQ_NUM);

         rU0IER = 0U; 
         rU0TER &= (~UART_TER_TXEN) & UART_TER_BITMASK;

         tn_disable_interrupt();
         ui->state = UART_STATE_CLOSE;
         tn_enable_interrupt();
      }
   }

   return rc;
} 

//----------------------------------------------------------------------------
int bsp_uart0_read(UARTINFO * ui, unsigned char * buf, unsigned int max_len)
{
   unsigned int nbytes;
   int ind   = 0;
   
   BOOL fExit = FALSE;

   if(ui == NULL || buf == NULL || max_len == 0U)
   {  
      ind = TERR_WPARAM;
   } 
   else
   {  
      ui->rx_timeout_cnt = ui->rx_timeout;  

      if(max_len < ui->rx_buf_size)
      {
         nbytes = max_len;
      }  
      else
      {
         nbytes = ui->rx_buf_size;
      }

      while(fExit == FALSE)
      {
         // tn_disable_interrupt();
         rU0IER &= ~1U; //-- Disable Rx int

         if(ui->rx_tail == ui->rx_head) // No char
         {  
            // tn_enable_interrupt();
            rU0IER |= 1U; //-- Enable Rx int

            if(ind > 0) //!< We received some character(s) before
            {  
               fExit = TRUE;
            }   
            else // no chars in buf
            {  
               if(ui->rx_timeout == TN_NO_WAIT)
               {  
                  ind = TERR_TIMEOUT;
                  fExit = TRUE;
               }   
               else if(ui->state != UART_STATE_OPEN) // check without critical section
               { 
                  ind = TERR_ILUSE;
                  fExit = TRUE;
               }
               else
               {  
                  (void)tn_task_sleep(1); //!< Sleep (here 1 mS)
                  if(ui->rx_timeout != TN_WAIT_INFINITE)
                  {
                     ui->rx_timeout_cnt--;
                     if(ui->rx_timeout_cnt == 0U)
                     {  
                        ind = TERR_TIMEOUT;
                        fExit =TRUE;
                     } 
                  }
               }   
            }
         }   
         else // Got char(s)
         {
            buf[ind] = ui->rx_buf[ui->rx_tail];  //!< rd data

            ui->rx_tail++;
            if(ui->rx_tail >= ui->rx_buf_size)
            {  
               ui->rx_tail = 0U;
            }   

          //  tn_enable_interrupt();
            rU0IER |= 1U; //-- Enable Rx int

            ind++;
            if(ind >= (int)nbytes)
            {  
               fExit = TRUE;
            }   
         }
      }
   } //!< while(!fExit)

   return ind;
}

//----------------------------------------------------------------------------
int uart0_tx_str(char * str)
{
   int rc = TERR_ILUSE;
   unsigned int len;
   
   if(str != NULL) 
   {   
      len = (unsigned int)strlen(str);
      rc = bsp_uart0_transmit(&g_uart0_info, 
                             (unsigned char *)str, 
                              len);
   }

   return rc;
}   

//----------------------------------------------------------------------------
void uart0_tx_char(unsigned char ch)
{
   unsigned char buf[4];
   buf[0] = ch;
   
   (void)bsp_uart0_transmit(&g_uart0_info, 
                            buf, 
                            1U);
}

//-----------------------------------------------------------------------------
int bsp_uart0_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size)
{
   int rc;
   unsigned int tx_bytes;
   unsigned char * ptr;
   unsigned int nbytes;

   if(ui == NULL || data == NULL || data_size == 0U)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      rc = tn_sem_acquire(&ui->tx_sem, 20000U); // 20 sec 
      if(rc == TERR_NO_ERR)
      {
         ptr = data;
         nbytes = data_size;

         while(nbytes > 0U)
         {
            //-- here - UART Tx FIFO is empty

	         if(nbytes > (unsigned int)UART_FIFO_SIZE)
            {  
	            tx_bytes = (unsigned int)UART_FIFO_SIZE;
            }
	         else
            {
	            tx_bytes = nbytes;
            }
            nbytes -= tx_bytes;

            while(tx_bytes > 0U)
            {
               rU0THR = *ptr;
               ptr++;
               tx_bytes--;
            }

            rU0IER |= 2U;  //-- Enable Tx int

         // Wait end of Tx
            rc = tn_sem_acquire(&ui->tx_rdy_sem, 5000U); // 5 s (its OK for 115.2 kbaud and 16 bytes
            if(rc < 0) // Err
            {  
               break;  // Somthing goes wrong
            }
         }
      }
      (void)tn_sem_signal(&ui->tx_sem); // Release UART sem any case
   }

   return rc;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



