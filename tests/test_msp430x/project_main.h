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


#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

//!< This is a part of the standard MSP430 CPU definitions
  
#define STATUS_SUCCESS  0x01   /**< op ok */
#define STATUS_FAIL     0x00   /**< op failed */
  
//!< Access to H/W registers
  
#define HWREG8(x) \
              (*((volatile uint8_t*)((uint16_t)x)))     /**<  8 bit access */ 
#define HWREG16(x) \
              (*((volatile uint16_t*)((uint16_t)x)))    /**< 16 bit access */ 
#define HWREG32(x) \
              (*((volatile uint32_t*)((uint16_t)x)))    /**< 32 bit access */ 


   //--- bsp_hardware_init.c

void bsp_init_CPU(void);


   //--- tn_sprintf.c

int tn_snprintf( char *outStr, int maxLen, const char *fmt, ... );


extern TN_DQUEUE * queueShellEvents;


 //-- Shell - use UART1

#define  uart_tx_str(str)    uart1_tx_str(str)
#define  uart_tx_char(ch)    uart1_tx_char(ch)

#define  uart_tx_str_func    uart1_tx_str
#define  uart_tx_char_func   uart1_tx_char

#endif /* #ifndef PROJECT_MAIN_H */


