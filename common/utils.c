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

#include <string.h>
#include <stdint.h>

#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"

#if defined  TNKERNEL_PORT_MSP430X
  #include <stdarg.h>
  #include "uart.h"
#else
  #if defined TN_CORTEX_M0
    #include "stm32f0xx_conf.h"
  #elif defined TN_CORTEX_M3
    #include "lpc176x_uart.h"
  #elif defined (TN_CORTEX_M4) || defined (TN_CORTEX_M7)
    #include "stm32f4xx_conf.h"
  #elif defined (TNKERNEL_PORT_ARM)
    #include "lpc236x_uart.h"
  #endif
#endif

#if defined TN_TEST
  #include "test.h"
#endif

#include "project_main.h"
#include "shell.h"


#if defined TN_TEST

#if defined  TNKERNEL_PORT_MSP430X

int vStrPrintf( char *outStr, int maxLen, const char *fmt, va_list args );
extern TN_SEM * semTestPrint;
extern __no_init char g_test_print_buf[];

#endif

//----------------------------------------------------------------------------
void TestPrint(const char * fmt, ...)
{
   va_list  args;

 // Sem here protects 'g_test_print_buf' 

   tn_sem_acquire(semTestPrint, TN_WAIT_INFINITE); // (RTOS_TICKS_PER_SECOND) * 2UL); // sleep until TX finished or stoped

   g_test_print_buf[0] = 0;

   va_start( args, fmt);                          
   vStrPrintf(g_test_print_buf, TESTPRINT_BUF_LEN - 1, fmt, args); 

   uart_tx_str(g_test_print_buf); // defined in "project_main.h"

   tn_sem_signal(semTestPrint);
}

#endif

void do_itoa(int val, char * buf, int buf_len);

//----------------------------------------------------------------------------
void do_itoa(int val, char * buf, int buf_len)
{
   char *p ;       
   char *fd;       
   char tmp;      
   int digval; 

   unsigned int len = 0;  

//   if(buf == NULL)
//   {
//      return;
//   }
   p = buf;
   //if(p != NULL)
   {
      fd = p;           /* save pointer to first digit */

#if defined (__ICCARM__) || defined (__ICC430__)
#pragma cstat_suppress="MISRAC2012-Rule-14.3_b"
      if(val < 0) 
#else
      if(val < 0) 
#endif
      {
         *p = '-';
         p++;

         len++;
         val = -val;
      }

      do 
      {
         digval = val % 10;
         val /= 10;

       //  if(digval > 9)
       //  {
       //     *p++ = (char) (digval - 10 + 'a');  // a letter 
       //  }
       //  else
         {  
            digval += 0x30L;
            tmp = (char)digval;
            *p = tmp;
            p++;
         } 
         len++;
      }while (val > 0 && len < (unsigned int)buf_len);

      /* Check for buffer overrun */
      if(len >= (unsigned int)buf_len)
      {
         buf[0] = '\0';
      }

     // reverse 

      *p = '\0'; 
      p--;  

      do 
      {
         tmp = *p;
         *p = *fd;
         *fd = tmp;  
         --p;
         ++fd;        
      }while (fd < p); 
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


