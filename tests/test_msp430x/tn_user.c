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
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "uart.h"
#include "project_main.h"
#include "test.h"


extern TN_SEM * semTxUart1;             //!< UART1 semaphore 
extern BSP_UART_DS * g_p_uart_ds;     //!< global UART data structure
extern CURRTEST g_curr_test;

static volatile unsigned char * var_rtc;
static volatile unsigned char * var_uart1;
static volatile unsigned char * var_timer_B0;

extern volatile int tn_inside_int_cnt;

__raw __interrupt void USCI_A1_ISR(void);
__raw __interrupt void RTC_ISR(void);
__raw  __interrupt void TIMER0_B0_ISR(void);  // Int N 59

/**
*    @brief    hwi_USCI_A1_Fxn
*    
*    @details  Interrupt function UART 1(USCI_A1)
*
*              RX - Put the char to the internal UART RX buf, if no room - overflow
*              TX - Get char from the internal TX buf and put it to the UART TX register
*                   If TX buf is empty, disables TX interrupt and post the TX semaphore  
*
*    @param [in] arg  The SYS/BYOS default parameter
*    
*    @pre      May be used only like Interrupt Handler function for 
*              the UART 1(USCI_A1) hardware interrupt 
*/

__task  void do_USCI_A1_ISR(void);

#pragma vector = USCI_A1_VECTOR
__raw  __interrupt void USCI_A1_ISR(void)  // Int N 46
{
   tn_int_enter(var_uart1);

   do_USCI_A1_ISR();

   tn_int_exit(var_uart1);
}

__task  void do_USCI_A1_ISR(void)
{
   //!< UCRXIFG is automatically reset when UCAxRXBUF is read
   //!< UCTXIFG is automatically reset if a character is written to UCxTXBUF

   UARTDS * pdata;
   int rx_val;
   unsigned int rv;

   pdata = &g_p_uart_ds->g_uart1_ds;

   rv = __even_in_range(UCA1IV,4);
   switch(rv)
   {
      case 0: 
        
         break;                          //!< Vector 0 - no interrupt
         
      case 2:                            //!< Vector 2 - RXIFG
        
         rx_val = (int)UCA1RXBUF;    //!< read any case 
         if((UCA1STAT & ((unsigned char)UCFE | 
                         (unsigned char)UCOE | 
                         (unsigned char)UCRXERR)) != 0U) //!< if rx error
         {
            //!< do nothing here
         }
         else
         {
            //!< check 'fifo is full'
            if((pdata->rx_head == pdata->rx_tail - 1) ||
                 (pdata->rx_head == pdata->rx_buf_size - 1 && pdata->rx_tail == 0))
            {
                 //!< overflow
               pdata->rx_buf[pdata->rx_head] = (unsigned char)rx_val;
               pdata->rx_head++;
               if(pdata->rx_head >= pdata->rx_buf_size)
               {
                  pdata->rx_head = 0;
               }
               pdata->rx_tail = pdata->rx_head + 1;
               if(pdata->rx_tail >= pdata->rx_buf_size)
               {
                  pdata->rx_tail = 0;
               }
            }
            else //!< no overflow
            {
               pdata->rx_buf[pdata->rx_head] = (unsigned char)rx_val;
               pdata->rx_head++;
               if(pdata->rx_head >= pdata->rx_buf_size)
               { 
                  pdata->rx_head = 0;
               }
            }
         } 
         break;
         
      case 4:                                 //!< Vector 4 - TXIFG

         if(pdata->tx_idx >= pdata->tx_cnt)    //!< tx overall buf finished
         {  
            UCA1IE &=  (unsigned char)(~UCTXIE); 
            pdata->tx_buf = NULL;
            
            (void)tn_sem_signal(semTxUart1);
         }
         else
         {  
            if(pdata->tx_buf != NULL)
            {
               UCA1TXBUF = pdata->tx_buf[pdata->tx_idx++]; //!< send char out UART transmitter
            }
         }
         break;
         
      default: 
         break;
   }
}

/**
*    @brief    hwi_RTC_Fxn
*    
*    @details  Interrupt function  RTC (Real Time Clock)
*
*    @param  [in]  arg  The SYS/BYOS default parameter
*    
*    @pre      May be used only like Interrupt Handler function for 
*              the RTC hardware interrupts 
*/

__task  void do_RTC_ISR(void);

#pragma vector = RTC_VECTOR
__raw __interrupt void RTC_ISR(void)    // Int N 42
{
   tn_int_enter(var_rtc);

   do_RTC_ISR();

   tn_int_exit(var_rtc);
}

__task void do_RTC_ISR(void)
{
   unsigned int rv;

   while((BAKCTL & (unsigned int)LOCKBAK) == (unsigned int)LOCKBAK)            //!< Unlock backup system
   {
      BAKCTL &= (unsigned int)(~((unsigned int)LOCKBAK)); 
   }
//P4OUT |= BIT6;

   rv = __even_in_range(RTCIV, 14);
   switch(rv)
   {
      case  0:                            //!< Vector  0:  No interrupt
        
         break;                           
         
      case  2:                            //!< Vector  2:  RTCRDYIFG
         
         break;
         
      case  4:    //!< Vector  4:  RTCEVIFG
        
         break;
         
      case  6:    //!< Vector  6:  RTCAIFG - RTC alarm

         RTCCTL0 &= (unsigned char)(~RTCAIFG);        
  
         break;
         
      case  8:    //!< Vector  8:  RT0PSIFG - RTOS tick 256 Hz (3.91 ms)

         tn_tick_int_processing();

         break;
         
      case 10:    //!< Vector 10:  RT1PSIFG
        
         break;
         
      case 12:    //!< Vector 12:  RTCOFIFG    32-kHz crystal oscillator fault interrupt flag.

         break;
         
      case 14:    //!< Vector 14:  Reserved
        
         break; 
         
      default: 
        
         break;
   }
}

//----------------------------------------------------------------------------
__task  void do_TIMER0_B0_ISR(void);

//----------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__raw  __interrupt void TIMER0_B0_ISR(void)  // Int N 59
{
   tn_int_enter(var_timer_B0);

   do_TIMER0_B0_ISR();

   tn_int_exit(var_timer_B0);
}

//----------------------------------------------------------------------------
__task  void do_TIMER0_B0_ISR(void)
{
   int rc;
   if(g_curr_test.test_int_handler != NULL)
   {
      rc = g_curr_test.test_int_handler(&g_curr_test);
      if(rc == TERR_NO_ERR) // Interrupt overall job was finished
      { 
         test_end_do_signal(INTERRUPT_RDY_FLAG);
      }
   }
}

//----------------------------------------------------------------------------
// Processor specific routine
//
//----------------------------------------------------------------------------
void tn_cpu_int_enable()
{

 // The global MSP430X CPU interrupts (GIE) will be enable
 // after the finish of the function tn_start_exe() 


   //-- OS ticks interrupt source - RTC prescaler (div by 128)

   RTCPS0CTL &= (unsigned int)(~RT0PSIE);

     // RT0IP(bits 4..2) = 110b = Divide by 128; 32768Hz/128 = 256 Hz

   RTCPS0CTL |= (unsigned short)(6U<<2U);
   RTCPS0CTL |= (unsigned short)RT0PSIE;    //!< enable OS tick interrupt 

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

