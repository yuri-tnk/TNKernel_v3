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

#ifndef  PROJECT_MAIN_H
#define  PROJECT_MAIN_H

//-- project_main.c

extern TN_DQUEUE queueShellEvents;

//-- prj_shell_func.c

extern const int g_shell_cmd_arr_size;
//-- stm32_uart.c

typedef struct _UARTINFO
{
   TN_SEM tx_sem;
   TN_SEM tx_rdy_sem;
   unsigned int rx_buf_size;
   unsigned char * rx_buf;
   unsigned int rx_timeout_cnt;
   unsigned int rx_timeout;  
   unsigned int rx_tail;
   int state;
#if 0
   DMA_InitTypeDef  DMA_InitStructure;   
   USART_TypeDef huart;
#endif
}UARTINFO;

extern UARTINFO g_uart2_info;



int bsp_uart2_open(UARTINFO *ui, 
//                  USART_InitTypeDef * init_info,
                  unsigned char *rx_buf,
                  unsigned int rx_buf_size);
int bsp_uart2_close(UARTINFO *ui);
int bsp_uart2_read(UARTINFO * ui, unsigned char * buf, unsigned int max_len);
int bsp_uart2_transmit(UARTINFO * ui, 
                      unsigned char * data, 
                      unsigned int data_size);
int uart2_tx_str(char * str);
void uart2_tx_char(unsigned char ch);


void do_itoa(int val, char * buf, int buf_len);

//----------------------------------------------------------------------------
int tn_dump_int_ex(unsigned char * buf, // output buffer
                   int buf_len,      // total output buffer length
                   const char * fmt, // format string
                   int val,          // value to dump
                   int width,        // valid only if 'fmp' contains '.*' in the width field
                   int precision);    // valid only if 'fmp' contains '.*' in the precision field

static TN_INLINE int tn_dump_int(unsigned char * buf,
                                  int buf_len,
                                  const char * fmt,
                                  int val)
{
   return tn_dump_int_ex(buf,
                         buf_len,
                         fmt,
                         val,
                         0,
                         0);
}

//----------------------------------------------------------------------------
//  Converting unsigned short/integer to string according to the format options in
//  'fmt' string (snprintf like, but for the single unsigned short/integer argument 
//----------------------------------------------------------------------------
int tn_dump_uint_ex(const char * fmt, // format string
                    unsigned char * buf, // output buffer
                    int buf_len,      // total output buffer length
                    unsigned int val,          // value to dump
                    int width,        // valid only if 'fmp' contains '.*' in the width field
                    int precision);   // valid only if 'fmp' contains '.*' in the precision field

static TN_INLINE int tn_dump_uint(const char * fmt,
                                  unsigned char * buf,
                                  int buf_len,
                                  unsigned int val)
{
   return tn_dump_uint_ex(fmt,
                          buf,
                          buf_len,
                          val,
                          0,
                          0);
}

//----------------------------------------------------------------------------
//  Converting long to string according to the format options in
//  'fmt' string (snprintf like, but for the single long argument 
//----------------------------------------------------------------------------
int tn_dump_long_ex(const char * fmt, // format string
                    unsigned char * buf, // output buffer
                    int buf_len,      // total output buffer length
                    long val,         // value to dump
                    int width,        // valid only if 'fmp' contains '.*' in the width field
                    int precision);   // valid only if 'fmp' contains '.*' in the precision field

static TN_INLINE int tn_dump_long(const char * fmt,
                                  unsigned char * buf,
                                  int buf_len,
                                  long val)
{
   return tn_dump_long_ex(fmt,
                          buf,
                          buf_len,
                          val,
                          0,
                          0);
}

//----------------------------------------------------------------------------
//  Converting unsigned long to string according to the format options in
//  'fmt' string (snprintf like, but for the single unsigned long argument 
//----------------------------------------------------------------------------
int tn_dump_ulong_ex(const char * fmt, // format string
                     unsigned char * buf, // output buffer
                     int buf_len,      // total output buffer length
                     unsigned long val,         // value to dump
                     int width,        // valid only if 'fmp' contains '.*' in the width field
                     int precision);   // valid only if 'fmp' contains '.*' in the precision field

static TN_INLINE int tn_dump_ulong(const char * fmt,
                                   unsigned char * buf,
                                   int buf_len,
                                   unsigned long val)
{
   return tn_dump_ulong_ex(fmt,
                           buf,
                           buf_len,
                           val,
                           0,
                           0);
}

//----------------------------------------------------------------------------
//  Converting (unsigned) char to string according to the format options in
//  'fmt' string (snprintf like, but for the single (unsigned) char argument 
//----------------------------------------------------------------------------
int tn_dump_uchar(const char * fmt, // format string
                  unsigned char * buf,  // output buffer
                  int buf_len,       // total output buffer length
                  unsigned char val); // value to dump


 //-- Shell - use UART2

#define  uart_tx_str(str)    uart2_tx_str(str)
#define  uart_tx_char(ch)    uart2_tx_char(ch)

#define  uart_tx_str_func    uart2_tx_str
#define  uart_tx_char_func   uart2_tx_char


#endif /* #ifndef  PROJECT_MAIN_H */


