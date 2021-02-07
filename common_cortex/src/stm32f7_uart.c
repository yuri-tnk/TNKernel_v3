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
#include "stm32f7xx.h"
#include "stm32f7xx_conf.h"   /* YVT */
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "project_main.h"
#include "system_stm32f746.h"

#define UART_STATE_CLOSE          0
#define UART_STATE_OPEN           1
#define UART_STATE_IN_PROGRESS    2

//----------------------------------------------------------------------------
int bsp_uart3_open(UARTINFO *ui, 
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
               __HAL_RCC_GPIOD_CLK_ENABLE();
          
              /* Enable USART clock */
               __HAL_RCC_USART3_CLK_ENABLE();
          
              /* DMA controller clock enable */
               __HAL_RCC_DMA1_CLK_ENABLE();  // We use DMA1 Ch2 & Ch3
          
              /* Connect PXx to USARTx_Tx */
               GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
              
              /* Connect PXx to USARTx_Rx */
               GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
              
              /* Configure USART Tx and Rx as alternate function push-pull */
               GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
               GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
               GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
               GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
               GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8; //USARTx_TX_PIN;
               GPIO_Init(GPIOD, &GPIO_InitStructure);
             
               GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //USARTx_RX_PIN;
               GPIO_Init(GPIOD, &GPIO_InitStructure);

               USART_Init(USART3, init_info);
              
              //-- DMA Configuration - Rx, DMA1  Ch 4 Stream 1

               ui->DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
               ui->DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART3->RDR));
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

               DMA_Init(DMA1_Stream1, &ui->DMA_InitStructure);

                /* Enable the USART Rx DMA request */
               USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
                
                /* Enable the DMA channel */

               DMA_Cmd(DMA1_Stream1, ENABLE);
                
              /* Enable USART */
               USART_Cmd(USART3, ENABLE);

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
int bsp_uart3_close(UARTINFO *ui)
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
         NVIC_DisableIRQ(DMA1_Stream3_IRQn);

         DMA_DeInit(DMA1_Stream1); //USARTx_RX_DMA_CHANNEL);
         DMA_DeInit(DMA1_Stream3); //USARTx_TX_DMA_CHANNEL);

          /* Disable the USART Rx/Tx DMA request */
         USART_DMACmd(USART3, USART_DMAReq_Rx, DISABLE);
         USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
          
          /* Disable the DMA channel */
         DMA_Cmd(DMA1_Stream1, DISABLE);  // Rx
         DMA_Cmd(DMA1_Stream3, DISABLE);  // Tx
          
        /* Disable USART */
         USART_Cmd(USART3, DISABLE);

         tn_disable_interrupt();
         ui->state = UART_STATE_CLOSE;
         tn_enable_interrupt();
      }
   }

   return rc;
} 

//----------------------------------------------------------------------------
int bsp_uart3_read(UARTINFO * ui, unsigned char * buf, unsigned int max_len)
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

         if(DMA1_Stream1->NDTR + ui->rx_tail == ui->rx_buf_size)
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
int uart3_tx_str(char * str)
{
   int rc = TERR_ILUSE;
   unsigned int len;
   
   if(str != NULL) 
   {   
      len = (unsigned int)strlen(str);
      rc = bsp_uart3_transmit(&g_uart3_info, 
                             (uint8_t *)str, 
                             (uint16_t) len);
   }

   return rc;
}   

//----------------------------------------------------------------------------
void uart3_tx_char(unsigned char ch)
{
   unsigned char buf[4];
   buf[0] = ch;
   
   (void)bsp_uart3_transmit(&g_uart3_info, 
                           (uint8_t *)buf, 
                           1U);
}


//-----------------------------------------------------------------------------
static void bsp_uart3_init_transmit(UARTINFO * ui, 
                                    unsigned char * data, 
                                    unsigned int data_size)
{
  // DMA UART Tx is safe only from the non-cacheable RAM ( in the project
  // 'g_uart3_tx_buf' is placed at DTCM)

   (void)memcpy(g_uart3_tx_buf, data, data_size);

    //-- DMA Configuration - Tx, DMA1  Ch 4 Stream 3

   ui->DMA_InitStructure.DMA_Channel            = DMA_Channel_4; 
   ui->DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART3->TDR));
   ui->DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_uart3_tx_buf;
   ui->DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
   ui->DMA_InitStructure.DMA_BufferSize         = (uint16_t)data_size;
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

   DMA_Init(DMA1_Stream3, &ui->DMA_InitStructure);
      
   USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
 
   DMA_ITConfig(DMA1_Stream3, DMA_IT_TC | DMA_IT_TE, ENABLE);            
      
   // Enable DMA1 USART Tx Channel
   DMA_Cmd(DMA1_Stream3, ENABLE);

   // Enable 'TransferComplete' DMA interrupt
   NVIC_SetPriority(DMA1_Stream3_IRQn, 0);
   NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}

//-----------------------------------------------------------------------------
int bsp_uart3_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size)
{
   int rc;
   unsigned int nbytes;
   unsigned int nb;
   unsigned char * ptr; 

   if(ui == NULL || data == NULL || data_size == 0U)
   {  
      rc = TERR_WPARAM;
   } 
   else
   {  
      rc = tn_sem_acquire(&ui->tx_sem, TN_WAIT_INFINITE);
      if(rc == TERR_NO_ERR)
      {
         nbytes = data_size;
         ptr = data; 
         while(nbytes > 0UL)
         { 
            if(nbytes > UART3_TX_BUF_SIZE)
            {
               nb = UART3_TX_BUF_SIZE;
            }
            else
            {
               nb = nbytes;
            }

            bsp_uart3_init_transmit(ui, ptr, nb); 

            // Wait end of Tx
            rc = tn_sem_acquire(&ui->tx_rdy_sem, 5000 ); // 5 s

            // Any case, stop Tx DMA & disable 'TransferComplete' DMA interrupt
            USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
            NVIC_DisableIRQ(DMA1_Stream3_IRQn);

            if(rc >= 0) // OK
            {
               ptr    += nb;
               nbytes -= nb;
            }
            else
            {
               break; 
            }
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

