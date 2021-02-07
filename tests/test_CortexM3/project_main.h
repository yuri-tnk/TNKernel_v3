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


#ifndef  PROJECT_MAIN_H
#define  PROJECT_MAIN_H

//-- project_main.c

extern TN_DQUEUE queueShellEvents;

//-- prj_shell_func.c

extern const int g_shell_cmd_arr_size;
   

void HardwareInit(void);

   //--- I/O pins masks

#define  MASK_SSEL       (1UL<<12UL)
#define  MASK_SP1        (1UL<<3UL)
#define  MASK_SP2        (1UL<<4UL)
#define  MASK_LED1       (1UL<<9UL)
#define  MASK_SR_OE      (1UL<<7UL)
#define  MASK_RADIO_TX   (1UL<<25UL)
#define  MASK_CH_NUM     (1UL<<6UL)

#define  MASK_LED        (1UL<<26UL)
#define  LED_MASK        (1UL<<26UL)

#define  MAX_TST_DUMP_STR_LEN   96
#define  MAX_DUMP_STR_LEN       (MAX_TST_DUMP_STR_LEN - 1) 


extern UARTINFO g_uart0_info;
extern TN_DQUEUE  queueRxUART0;
extern TN_FMP   RxUART0MemPool;
extern TN_SEM   semTxUart0;
extern TN_SEM   semMeasPrf;
extern TN_SEM   semFifoEmptyTxUART0;
extern TN_SEM   semCmdExec;

   //--- tn_user.c

 //-- Shell - use UART0

#define  uart_tx_str(str)    uart0_tx_str(str)
#define  uart_tx_char(ch)    uart0_tx_char(ch)

#define  uart_tx_str_func    uart0_tx_str
#define  uart_tx_char_func   uart0_tx_char



#endif /* #ifndef  PROJECT_MAIN_H */


