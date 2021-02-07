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


#ifndef LPC176X_UART_H
#define LPC176X_UART_H

typedef struct _UARTINFO
{
   TN_SEM tx_sem;
   TN_SEM tx_rdy_sem;
   unsigned int rx_tail;
   unsigned int rx_head;

   unsigned int rx_buf_size;
   unsigned char * rx_buf;

   unsigned int rx_timeout_cnt;
   unsigned int rx_timeout;  

   int state;

}UARTINFO;

extern UARTINFO g_uart0_info;

int bsp_uart0_open(UARTINFO * ui, 
                  unsigned char * rx_buf,
                  unsigned int rx_buf_size);
int bsp_uart0_close(UARTINFO *ui);
int bsp_uart0_read(UARTINFO * ui, unsigned char * buf, unsigned int max_len);
int bsp_uart0_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size);

int uart0_tx_str(char * str);
void uart0_tx_char(unsigned char ch);

   //--- UART

#define  LCR_DISABLE_LATCH_ACCESS    0x00000000
#define  LCR_ENABLE_LATCH_ACCESS     0x00000080

  //-- UART status register bits

#define  RX_INT    (0x2U<<1U)
#define  TO_INT    (0x6U<<1U)
#define  TX_INT    (0x1U<<1U)

#define  UART_FIFO_SIZE              16
#define  UART0_RX_PAYLOAD_BUF_SIZE   64

#endif /* #ifndef LPC176X_UART_H */
