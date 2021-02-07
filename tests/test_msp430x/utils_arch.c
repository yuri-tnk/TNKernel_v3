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

 /* ver 3.0 */

#include "io430.h"
#include <string.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "project_main.h"
#include "test.h"

//----------------------------------------------------------------------------
void  test_arch_set_int_timer(CURRTEST * ct, unsigned long timeout)
{
   unsigned long timer_val;

   TB0CTL = 
            (1 << 4) |     //!<  Bits 5-4 01b = Up mode
            (1 << 8);      //!<  Bits 9..8 = 01 - Use ACLK (32KHz) as timer clock


   TB0EX0 = 0;             //!<  Bits 2-0 = 000b TAIDEX is set div by 1

   TB0CTL |= (1 << 2);     //!<  Clear timer

   TB0R = 0;

   //!< Generate output signal for TB0CCR0 

       // In compare mode, any CCIFG flag is set when TBxR counts to 
       // the associated TBxCLn value.

   //Each OS tick is 32768/256 = 128 clocks of timer input freq

   timer_val =  timeout * 128UL;
   if(timer_val > 0xFFFEUL)
   {
      timer_val = 0xFFFEUL;
   }

   ct->timer_val = timer_val;

   TB0CCR0 = (unsigned short)timer_val;    

   TB0CCTL0 = (0x4 << 5) | //!< Bits 7-5  100b = Toggle
                    CCIE;  //!< Interrupt enable
}


//----------------------------------------------------------------------------
void test_arch_restart_int_timer(CURRTEST * ct)
{
   TB0CTL = (1 << 4) |     //!<  Bits 5-4 01b = Up mode
            (1 << 8);      //!<  Bits 9..8 = 01 - Use ACLK (32KHz) as timer clock

   TB0EX0 = 0;             //!<  Bits 2-0 = 000b TAIDEX is set div by 1

   TB0CTL |= (1 << 2);     //!<  Clear timer

   TB0R = 0;

   TB0CCR0 = (unsigned short)ct->timer_val;    
   TB0CCTL0 = (0x4 << 5) | //!< Bits 7-5  100b = Toggle
                    CCIE;  //!< Interrupt enable
}

//----------------------------------------------------------------------------
void  test_arch_disable_timer_int(void)
{
   TB0CCTL0 &= ~CCIE;  //!< Interrupt disabled
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

