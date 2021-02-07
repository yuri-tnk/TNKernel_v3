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



#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"

#define UART_STATE_CLOSE          0
#define UART_STATE_OPEN           1
#define UART_STATE_IN_PROGRESS    2

//----------------------------------------------------------------------------
// rx_buf should be aligned  as int*
//----------------------------------------------------------------------------
int bsp_uart2_open(UARTINFO *ui, 
                  USART_InitTypeDef * init_info,
                  unsigned char *rx_buf,
                  unsigned int rx_buf_size)
{
   TN_INTSAVE_DATA

   int rc = TERR_ILUSE;
   int state = UART_STATE_IN_PROGRESS;
   GPIO_InitTypeDef  GPIO_InitStructure;

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
        
         ui->rx_tail        = 0UL;
         ui->rx_buf         = rx_buf;
         ui->rx_buf_size    = rx_buf_size;
         ui->rx_timeout     = TN_WAIT_INFINITE;  // default value
         ui->rx_timeout_cnt = ui->rx_timeout;  

         // Create semaphores
         memset(&ui->tx_sem, 0, sizeof(TN_SEM));
         memset(&ui->tx_rdy_sem, 0, sizeof(TN_SEM));
         rc = tn_sem_create(&ui->tx_sem , 1, 1);
         if(rc == TERR_NO_ERR)
         {
            rc = tn_sem_create(&ui->tx_rdy_sem, 0, 1);
            if(rc == TERR_NO_ERR)
            {
              /* Enable GPIO clock */
               RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
              
              /* Enable USART clock */
               RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
              
              /* Enable the DMA periph */
               RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
              
              /* Connect PXx to USARTx_Tx */
               GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
              
              /* Connect PXx to USARTx_Rx */
               GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
              
              /* Configure USART Tx and Rx as alternate function push-pull */
               GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
               GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
               GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
               GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
               GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; //USARTx_TX_PIN;

               GPIO_Init(GPIOA, &GPIO_InitStructure);
              
               GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3; //USARTx_RX_PIN;
               GPIO_Init(GPIOA, &GPIO_InitStructure);
              
               USART_Init(USART2, init_info);
              
              //-- DMA Configuration - Rx, Ch 4 DMA Stream 5
               ui->DMA_InitStructure.DMA_Channel            = DMA_Channel_4; // DMA_CHANNELx;  
               ui->DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
               ui->DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)rx_buf;
               ui->DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
               ui->DMA_InitStructure.DMA_BufferSize         = (uint16_t)rx_buf_size; //2;
               ui->DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
               ui->DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
               ui->DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
               ui->DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
               ui->DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular; //DMA_Mode_Normal;
               ui->DMA_InitStructure.DMA_Priority           = DMA_Priority_Low; //DMA_Priority_High;
               ui->DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
               ui->DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
               ui->DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
               ui->DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

               DMA_Init(DMA1_Stream5, &ui->DMA_InitStructure);

                /* Enable the USART Rx DMA request */
               USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
                
                /* Enable the DMA channel */

               DMA_Cmd(DMA1_Stream5, ENABLE);
                
              /* Enable USART */

               USART_Cmd(USART2, ENABLE);

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

         NVIC_DisableIRQ(DMA1_Stream6_IRQn);

         DMA_DeInit(DMA1_Stream5); //USART2 RX
         DMA_DeInit(DMA1_Stream6); //USART2 TX

          /* Disable the USART Rx/Tx DMA request */
         USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
         USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
          
          /* Disable the DMA channel */
         DMA_Cmd(DMA1_Stream5, DISABLE);  // Rx
         DMA_Cmd(DMA1_Stream6, DISABLE);  // Tx
          
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

         if(DMA1_Stream5->NDTR + ui->rx_tail == ui->rx_buf_size)
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
int bsp_uart2_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size)
{
   int rc;

   if(ui == NULL || data == NULL || data_size == 0U)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      rc = tn_sem_acquire(&ui->tx_sem, TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR)
      {
            //-- DMA Configuration - Tx, Ch 4 DMA Stream 6

         ui->DMA_InitStructure.DMA_Channel            = DMA_Channel_4; 
         ui->DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
         ui->DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)data;
         ui->DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
         ui->DMA_InitStructure.DMA_BufferSize         = (uint16_t)data_size; //2;
         ui->DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
         ui->DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
         ui->DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
         ui->DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
         ui->DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
         ui->DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
         ui->DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
         ui->DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
         ui->DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
         ui->DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

         DMA_Init(DMA1_Stream6, &ui->DMA_InitStructure);
            
         USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

         DMA_ITConfig(DMA1_Stream6, DMA_IT_TC | DMA_IT_TE, ENABLE);
         DMA_Cmd(DMA1_Stream6, ENABLE); // Enable DMA1 USART Tx Channel

         // Enable 'TransferComplete' DMA interrupt

         NVIC_SetPriority(DMA1_Stream6_IRQn, 0);
         NVIC_EnableIRQ(DMA1_Stream6_IRQn);

         // Wait end of Tx
         rc = tn_sem_acquire(&ui->tx_rdy_sem, 2000 ); // 2 s (at 115200 baud, this is ~23kbytes)

         DMA_Cmd(DMA1_Stream6, DISABLE); 
         NVIC_DisableIRQ(DMA1_Stream6_IRQn);

         if(rc < 0) // Error - timeout etc.
         {
            // Error Handler
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

