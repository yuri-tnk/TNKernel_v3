/**
*
*  Copyright (c) 2004, 2016 Yuri Tiomkin
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

/*
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:/ /www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include <stdint.h>
#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"

#define UART_STATE_CLOSE          0
#define UART_STATE_OPEN           1
#define UART_STATE_IN_PROGRESS    2

//volatile unsigned int g_dbg = 0;
//----------------------------------------------------------------------------
// rx_buf should be aligned  as int*
//
// DMA Channel 5  USART2_RX
//----------------------------------------------------------------------------
int bsp_uart2_open(UARTINFO *ui, 
                 // USART_InitTypeDef * init_info,
                  unsigned char *rx_buf,
                  unsigned int rx_buf_size)
{
   TN_INTSAVE_DATA
   unsigned int tmp;

   int rc = TERR_ILUSE;
   int state = UART_STATE_IN_PROGRESS;

   if(ui == NULL)
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
         ui->rx_buf_size = rx_buf_size;
         ui->rx_buf      = rx_buf;
         ui->rx_timeout  = TN_WAIT_INFINITE;  
         ui->rx_timeout_cnt = ui->rx_timeout;
         ui->rx_tail     =  0U;

            // Create semaphores
         (void)memset(&ui->tx_sem, 0, sizeof(TN_SEM));
         (void)memset(&ui->tx_rdy_sem, 0, sizeof(TN_SEM));
         rc = tn_sem_create(&ui->tx_sem , 1, 1);
         if(rc == TERR_NO_ERR)
         {
            rc = tn_sem_create(&ui->tx_rdy_sem, 0, 1);
            if(rc == TERR_NO_ERR)
            {
              /* Enable the peripheral clock of GPIOA */
               RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

              /* Enable USART clock */
               RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

              /* Enable the DMA periph clock */
               RCC->AHBENR |= RCC_AHBENR_DMA1EN;
              
  	           /* Select AF mode (10) on PA2 and PA3 */
               GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER2 |GPIO_MODER_MODER3))\
                              | (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1); 

	           /* AF1 for USART2 pins */
#define AF1 1UL
	            GPIOA->AFR[0] = (GPIOA->AFR[0] &~ (0x0000FF00UL)) |
	                            (AF1 << (2UL * 4UL)) | // PA2 
	                            (AF1 << (3UL * 4UL));  // PA3

               USART2->CR1 &= ~USART_CR1_UE;
               USART2->BRR =  417; // 48 000 000 / 115200 = 416.666.. 
               USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR; // En Rx & Tx DMA
               USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; /* (3) */
  
               while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)/* polling idle frame Transmission */
               { 
                    /* add time out here for a robust application */
               }
               USART2->ICR = USART_ICR_TCCF;/* Clear TC flag */
           
               DMA1_Channel5->CCR  &= ~DMA_CCR_EN; // Disable DMA channel

               DMA1_Channel5->CCR   = DMA_CCR_MINC |     // Memory increment mode 
                                      DMA_CCR_CIRC; 

               DMA1_Channel5->CNDTR = (uint32_t)rx_buf_size; 
               tmp = (uint32_t)&(USART2->RDR);
               DMA1_Channel5->CPAR  = tmp; 
               DMA1_Channel5->CMAR  = (uint32_t)rx_buf; 
               DMA1_Channel5->CCR  |= DMA_CCR_EN; // Enable DMA

               //USART2->CR3 |= USART_CR3_DMAR;  // Enable Uart Rx DMA request
               //USART2->CR1 |= USART_CR1_UE;    // Enable UART

               tn_disable_interrupt();
               ui->state = UART_STATE_OPEN;
               tn_enable_interrupt();

               rc = TERR_NO_ERR;
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
int bsp_uart2_close(UARTINFO *ui)
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
         //--- Disable "DMATransferComplete" interrupt
         NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);

         DMA_DeInit(DMA1_Channel5); // RX
         DMA_DeInit(DMA1_Channel4); // TX

          /* Disable the USART Rx/Tx DMA request */
         USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
         USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
          
          /* Disable the DMA channel */
         DMA_Cmd(DMA1_Channel5, DISABLE); // RX
         DMA_Cmd(DMA1_Channel4, DISABLE); // TX
          
        /* Disable USART */
         USART_Cmd(USART2, DISABLE);

         tn_disable_interrupt();
         ui->state = UART_STATE_CLOSE;
         tn_enable_interrupt();
      }
   }

   return rc;
} 

//----------------------------------------------------------------------------
int bsp_uart2_read(UARTINFO * ui, unsigned char * buf, unsigned int max_len)
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

//!< ----- rx char ----------------------------

         if(DMA1_Channel5->CNDTR + ui->rx_tail == ui->rx_buf_size)
         {
            //!< now -no char  

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
	            else if(ui->state != UART_STATE_OPEN)
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
         else // got char 
         {
            buf[ind] = ui->rx_buf[ui->rx_tail];  //!< rd data

            ind++;   
            if(ind >= (int)nbytes)
            {  
               fExit = TRUE;
            }   

            ui->rx_tail++;
            if(ui->rx_tail >= ui->rx_buf_size)
            {  
               ui->rx_tail = 0;
            }   

         }
      } //!< while(!fExit)
   }   

   return ind;
}


//----------------------------------------------------------------------------
int uart2_tx_str(char * str)
{
   int rc = TERR_ILUSE;
   unsigned int len;
   
   if(str != NULL) 
   {   
      len = (unsigned int)strlen(str);
      rc = bsp_uart2_transmit(&g_uart2_info, 
                             (uint8_t *)str, 
                             (uint16_t) len);
   }

   return rc;
}   

//----------------------------------------------------------------------------
void uart2_tx_char(unsigned char ch)
{
   unsigned char buf[4];
   buf[0] = ch;
   
   (void)bsp_uart2_transmit(&g_uart2_info, 
                           (uint8_t *)buf, 
                           1U);
}

//-----------------------------------------------------------------------------
//  USART2_TX  (DMA Channel 4)
//-----------------------------------------------------------------------------
int bsp_uart2_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size)
{
   int rc;
   unsigned int tmp;

   if(ui == NULL || data == NULL || data_size == 0U)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      rc = tn_sem_acquire(&ui->tx_sem, TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR)
      {
              //-- DMA Configuration - Tx, DMA1 Channel 4

         DMA1_Channel4->CCR  &= ~DMA_CCR_EN; // Disable DMA channel
         DMA1_Channel4->CCR   = DMA_CCR_MINC |     // Memory increment mode 
                                DMA_CCR_PL_1 |     // !!! Otherwise corrupts sended info !!!
                                DMA_CCR_DIR  |     // Read FROM memory
                                DMA_CCR_TCIE |     // Transfer complete interrupt enable
                                DMA_CCR_TEIE;      // Transfer error interrupt enable
         DMA1_Channel4->CNDTR = (uint32_t)data_size; 
         tmp = (uint32_t)&(USART2->TDR);
         DMA1_Channel4->CPAR  = tmp; 
         DMA1_Channel4->CMAR  = (uint32_t)data; 
         DMA1_Channel4->CCR  |= DMA_CCR_EN; // Enable DMA

         USART2->CR3 |= USART_CR3_DMAT;  // Enable Uart Tx DMA

         // Enable Tx DMA interrupt

         NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0);
         NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

         // Wait end of Tx

         rc = tn_sem_acquire(&ui->tx_rdy_sem, 5000 ); // 5 sec

         DMA_Cmd(DMA1_Channel4, DISABLE);
         NVIC_DisableIRQ(DMA1_Channel4_5_IRQn);

         if(rc < 0) // Error -timeout etc.
         {
            // Error handler
         }

         (void)tn_sem_signal(&ui->tx_sem); // Release UART sem any case
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

