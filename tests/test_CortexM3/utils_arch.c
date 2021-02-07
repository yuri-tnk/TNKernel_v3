/*

  TNKernel real-time kernel

  Copyright © 2004,2016 Yuri Tiomkin

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

 /* ver 3.0 */

#include "lpc176x.h"
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "lpc176x_uart.h"
#include "project_main.h"
#include "test.h"

extern CURRTEST g_curr_test; 

// We use LPC Timer1 (32-bit)  here

#define TIMER1_VALUE  96000UL

//----------------------------------------------------------------------------
void  test_arch_set_int_timer(CURRTEST * ct, unsigned long timeout)
{
   rPCONP |= (1U<<2U);         //-- PCTIM1(bit 2) =1 Timer/Counter 1 Clock enabled

   rPCLKSEL0 &= ~(0x3U << 4U); //-- bit5:4 = 00 
   rPCLKSEL0 |= (0x1U << 4U);  //-- bit5:4 = 01-> Clock div = 1 for Timer 1

   rT1TCR = 0U;                //-- Timer 1 - stop

   rT1PR = 0U;                 //-- Prscaler = 0
   rT1PC = 0U;

   rT1MR0 = TIMER1_VALUE;      // 1 ms
   rT1MCR = 3U;                //-- bit 0=1 -int on MR0 , bit 1=1 - Reset on MR0

   //--  Set interrupt for Timer 1 --

   NVIC_SetPriority(TIMER1_IRQn, 0);
   NVIC_EnableIRQ(TIMER1_IRQn);

   rT1TCR = 1U;          //-- Timer 1 - run
}

//----------------------------------------------------------------------------
void test_arch_restart_int_timer(CURRTEST * ct)
{
   rT1TCR = 0U;          //-- Timer 1 - stop

   rT1PR = 0U;           //-- Prscaler = 0
   rT1PC = 0U;

   rT1MR0 = TIMER1_VALUE;
   rT1MCR = 3U;          //-- bit 0=1 -int on MR0 , bit 1=1 - Reset on MR0

   NVIC_SetPriority(TIMER1_IRQn, 0);
   NVIC_EnableIRQ(TIMER1_IRQn);

   rT1TCR = 1U;          //-- Timer 1 - run
}

//----------------------------------------------------------------------------
void  test_arch_disable_timer_int(void)
{
   rT1TCR = 0U;          //-- Timer 1 - stop

   NVIC_DisableIRQ(TIMER1_IRQn);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
