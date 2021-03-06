/*

  TNKernel real-time kernel

  Copyright � 2004, 2016 Yuri Tiomkin

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

#include <stdint.h>
#include <string.h>
#include "stm32l0xx.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"
#include "test.h"


// TIM2 (16-bit), TIM2_IRQHandler

#define  TIM_DIER_IRQ_MASK      (TIM_DIER_UIE |   \
                                 TIM_DIER_CC1IE | \
                                 TIM_DIER_CC2IE | \
                                 TIM_DIER_CC3IE | \
                                 TIM_DIER_CC4IE | \
                                 TIM_DIER_TIE)

//----------------------------------------------------------------------------
void  test_arch_set_int_timer(CURRTEST * ct, unsigned long timeout)
{
   unsigned long prescaler_val;

   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  //APB1 Clk = PCLK = 16MHz
 
   prescaler_val     = (16000000UL) / 1000UL; // 1 OS tick - 1 ms
   ct->prescaler_val = prescaler_val;
   ct->timer_val     = timeout;

   test_arch_restart_int_timer(ct);
}

//----------------------------------------------------------------------------
void test_arch_restart_int_timer(CURRTEST * ct)
{

  TIM2->PSC   = ct->prescaler_val;         //  prescaler
  TIM2->ARR   = (uint32_t)(ct->timer_val -1UL);  /* Time constant.           */
  TIM2->EGR   = TIM_EGR_UG;                /* Update Generation (prescaler also) */
  TIM2->CNT   = 0;                         /* Reset counter.           */

  TIM2->SR    = 0;                         /* Clear pending IRQs.      */
  TIM2->DIER |=  TIM_DIER_UIE;             /* Update Event IRQ enabled.*/

  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM2->CR1  = TIM_CR1_CEN |               /* Counter enable        */ 
               TIM_CR1_OPM |               /* One pulse mode        */ 
               TIM_CR1_URS;                /* Update request source */ 
}

//----------------------------------------------------------------------------
void  test_arch_disable_timer_int(void)
{
   TIM2->CR1   = 0UL; 
   TIM2->SR    = 0UL; 

  // All interrupts disabled.
   TIM2->DIER &= ~TIM_DIER_IRQ_MASK;

   NVIC_DisableIRQ(TIM2_IRQn);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
