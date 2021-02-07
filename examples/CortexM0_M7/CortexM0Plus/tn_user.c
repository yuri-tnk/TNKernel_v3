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

#include "stm32l0xx.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"

#define  VECTACTIVE_M0   (0x0000003FU)   /* Bits[5:0]*/

void SysTick_Handler(void);
void DMA1_Channel4_5_6_7_IRQHandler(void);
void Clear_BSS(void);

//----------------------------------------------------------------------------
void tn_cpu_int_enable(void)
{
   // Enable a peripheral Int (Sys_Tick etc.)
   // The core Int is still disabled here

}

#if 0
//----------------------------------------------------------------------------
int tn_inside_int(void)
{
   int rc = 0;
   unsigned int ipsr;
   ipsr = __get_IPSR();
   if((ipsr & VECTACTIVE_M0) != 0UL)
   {
      rc = 1;
   }
   return rc;
}
#endif
//----------------------------------------------------------------------------
void SysTick_Handler(void)
{
   tn_tick_int_processing();

  //-- !!!    For the Cortex CPU, this function is a last func in the
  //--     any user's interrupt handler

   tn_int_exit();  
}

//----------------------------------------------------------------------------
void DMA1_Channel4_5_6_7_IRQHandler(void)
{
 //  DMA channel 7 - USART2_TX 

   if((DMA1->ISR & DMA_ISR_TCIF7) == DMA_ISR_TCIF7) // Transfer complete
   {
      DMA1->IFCR = DMA_IFCR_CTCIF7;  // Clear TC flag 

     //-- Signal to TX routine

      (void)tn_sem_signal(&g_uart2_info.tx_rdy_sem);
   }
   else if((DMA1->ISR & DMA_ISR_TEIF7) == DMA_ISR_TEIF7)  // Transfer Error 
   {
      DMA1->IFCR = DMA_IFCR_CTEIF7;  // Clear TE flag
      // ToDo 
   }
   else // Unexpected
   {
      NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
   }

  //-- !!!    For the Cortex CPU, this function is a last func in the
  //--     any user's interrupt handler
   tn_int_exit();  
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
