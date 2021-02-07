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

#if defined TN_CORTEX_M0
#include "stm32f0xx_conf.h"
#elif defined TN_CORTEX_M3
#include "lpc176x_uart.h"
#elif defined TN_CORTEX_M4
#include "stm32f4xx_conf.h"
#elif defined TN_CORTEX_M7
#include "stm32f4xx_conf.h"
#elif defined TNKERNEL_PORT_MSP430X
#include "uart.h"
#elif defined TNKERNEL_PORT_ARM
#include "lpc236x_uart.h"
#endif
#include "project_main.h"
#include "shell.h"

int sh_get_command_line_args(unsigned char * cmd_line, unsigned char ** argv);
void sh_cmd_end(SHELLINFO * sh_info, int use_CRLF);
int sh_send(SHELLINFO * sh_info, char * str_to_send);
int sh_stcasecmp(char * s1, char * s2);

#ifdef __cplusplus
extern "C"  {
#endif

#if defined( __GNUC__ ) // __CROSSWORKS_ARM)
int strcasecmp(const char *s1, const char *s2);
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

//----------------------------------------------------------------------------
void sh_init(BSP_SHELL_DS * p_sh)
{
     //-- Shell
   unsigned long tmp;

   (void)memset(p_sh, 0, sizeof(BSP_SHELL_DS));

   p_sh->shell_info.cl_size      = SH_CL_SIZE;
   p_sh->shell_info.cl_buf       = &p_sh->shell_cl_buf[0];
   p_sh->shell_info.argv         = (unsigned char **)((void*)&p_sh->shell_argv[0]);
   p_sh->shell_info.exec_buf     = &p_sh->shell_exec_buf[0]; //!< command line exec buffer

   p_sh->shell_info.num_cmd      = g_shell_cmd_arr_size;

   tmp =  (unsigned long) &g_shell_cmd_arr[0];
   p_sh->shell_info.sh_cmd_arr = (SHELLCMD *)tmp;


   p_sh->shell_info.sh_send_str  = uart_tx_str_func;
   p_sh->shell_info.sh_send_char = uart_tx_char_func;

   (void)memset(p_sh->shell_info.cl_buf, 0, (size_t)p_sh->shell_info.cl_size);

     //-- UART rx drv
   p_sh->sh_uart_drv.buf          = &p_sh->sh_uart_drv_payload_buf[0];
   p_sh->sh_uart_drv.max_buf_size = SHELL_UART_RX_PAYLOAD_BUF_SIZE;
   p_sh->sh_uart_drv.pos          = 0;
}

//----------------------------------------------------------------------------
void sh_input(SHELLINFO * sh_info, unsigned char ch)
{

   if(sh_info->cmd_run == FALSE)
   {
      if(ch != (unsigned char)CR_SYM && sh_info->cl_pos < sh_info->cl_size) // CR or Buffer is full.
      {
         switch(ch)
         {
            case BS_SYM:
            case DEL_SYM:

              if(sh_info->cl_pos > 0)
              {
                 sh_info->cl_pos--;
                 (void)sh_echo(sh_info, BS_SYM);
                 (void)sh_putchar(sh_info, (unsigned char)' ');
                 (void)sh_putchar(sh_info, BS_SYM);
              }
              break;

            default:

              if(sh_info->cl_pos + 1 < sh_info->cl_size)
              {
                 // Only printable characters.
                 if(ch >= (unsigned char)SPACE_SYM && ch <= (unsigned char)DEL_SYM)
                 {
                    sh_info->cl_buf[sh_info->cl_pos++] = (unsigned char)ch;
                    (void)sh_echo(sh_info, ch);
                 }
              }
              break;
          }
      }
      else //-- cmd received - to process
      {
          sh_info->cl_buf[sh_info->cl_pos] = (unsigned char)'\0';
          strcpy((char*)sh_info->exec_buf, (char*)sh_info->cl_buf);
#if 0
          (void)memset(sh_info->cl_buf, 0, (size_t)sh_info->cl_size);
#endif
          sh_info->cl_pos = 0;
          sh_info->cl_buf[sh_info->cl_pos] = (unsigned char)'\0';

          sh_info->sh_send_str((char*)"\r\n");

          sh_info->cmd_run = TRUE;

          (void)sh_start_cmd_exec(sh_info); // Just signal on
      }
   }
   else // cmd_run == TRUE
   {
      if(ch == (unsigned char)CTRLC_SYM)
      { 
         (void)sh_stop_cmd_exec(sh_info); // Just signal on, inside ->set cmd_run = FALSE when actually finished
      }
   }
}

//----------------------------------------------------------------------------
int sh_start_cmd_exec(SHELLINFO * sh_info)
{
   void * data = (unsigned char*)((unsigned int)EVT_EXEC_SHELL);

#if defined TNKERNEL_PORT_MSP430X
   (void)tn_dqueue_send(queueShellEvents, (TN_DQUEUE_ELEMENT)data, TN_NO_WAIT);
#else
   (void)tn_dqueue_send(&queueShellEvents, (TN_DQUEUE_ELEMENT)data, TN_NO_WAIT);
#endif

   return 0;
}
//----------------------------------------------------------------------------
int sh_do_cmd_exec(SHELLINFO * sh_info)
{
   int i;
   int argc;
   SHELLCMD * cmd_ptr;
   int rc = TERR_NO_ERR;

   // Clear argv buffer
   (void)memset(sh_info->argv, 0, (size_t)(sizeof(unsigned char *) * (size_t)SH_MAX_ARG));

   argc = sh_get_command_line_args(sh_info->exec_buf, sh_info->argv);
   if(argc <= 0)
   {
      sh_cmd_end(sh_info, FALSE);
      rc = -1;
   }
   else
   {  
      for(i=0; i < sh_info->num_cmd; i++)
      {
         cmd_ptr = &sh_info->sh_cmd_arr[i];
         if(strcasecmp((char*)((void*)sh_info->argv[0]), cmd_ptr->cmd_name) == 0)
         {
              //-- recognized cmd

            cmd_ptr->exec_cmd(sh_info);
            sh_cmd_end(sh_info, TRUE);

            break;
         }
      }
      if(i == sh_info->num_cmd)  // If here - cmd not found
      {  
         (void)sh_send(sh_info, (char*)"\r\nWrong command!\r\n");
         sh_cmd_end(sh_info, TRUE);
         rc = -1;
      }
   }

   return rc;
}

//----------------------------------------------------------------------------
int sh_send(SHELLINFO * sh_info, char * str_to_send)
{
   int rc = -1;

   if(sh_info->sh_send_str != NULL)
   {
      sh_info->sh_send_str(str_to_send);
      rc = TERR_NO_ERR; 
   }
   return rc; 
}

//----------------------------------------------------------------------------
int sh_prompt(SHELLINFO * sh_info, int send_CR_LF)
{
   int rc = TERR_NO_ERR;

   if(sh_info->sh_send_str == NULL)
   {
      rc = -1;
   }
   else
   {
      if(send_CR_LF != 0)
      {
         sh_info->sh_send_str("\r\n");
      }
      sh_info->sh_send_str("SHELL>");
   }

   return rc; //-- OK
}

//----------------------------------------------------------------------------
int sh_stop_cmd_exec(SHELLINFO * sh_info)
{
   sh_info->stop_exec = TRUE;

   return 0;
}

//----------------------------------------------------------------------------
int sh_echo(SHELLINFO * sh_info, unsigned char ch)
{
   int rc = TERR_NO_ERR;

   if(sh_info->sh_send_char == NULL)
   {
      rc = -1;
   }
   else
   { 
      sh_info->sh_send_char(ch);
   }
   return rc;
}

//----------------------------------------------------------------------------
int sh_putchar(SHELLINFO * sh_info, unsigned char ch)
{
   int rc = TERR_NO_ERR;

   if(sh_info->sh_send_char == NULL)
   {
      rc = -1;
   }
   else
   { 
      sh_info->sh_send_char(ch);
   }
   return rc;
}

//----------------------------------------------------------------------------
void sh_cmd_end(SHELLINFO * sh_info, int use_CRLF)
{
   TN_INTSAVE_DATA

   (void)sh_prompt(sh_info, use_CRLF);

   tn_disable_interrupt();
   sh_info->cmd_run = FALSE;
   tn_enable_interrupt();
}
//----------------------------------------------------------------------------
//  Args with whitespaces does not supported !!!
//----------------------------------------------------------------------------
int sh_get_command_line_args(unsigned char * cmd_line, unsigned char ** argv)
{
   int ch;
   int rc = 0;
   int fExit = FALSE;
   int argc = 0;
   int now_arg = FALSE;
   unsigned char * ptr = cmd_line;
   unsigned char * start_ptr = cmd_line;

   for(ch =0; ch < SH_MAX_ARG; ch++)
   {
      argv[ch] = NULL;
   }

   for(;;)
   {
      ch = (int)*ptr;
      switch(ch)
      {
         case '\0':
         case ' ':
         case '\t':

            if(now_arg == TRUE)
            {
               *ptr = (unsigned char)'\0';
               argv[argc] = start_ptr;
               argc++;
               if(argc >= SH_MAX_ARG)
               { 
                  rc = argc;
                  fExit = TRUE;
               }
               else
               { 
                  now_arg = FALSE;
               }
            }

            if(fExit == FALSE)
            { 
               if(ch == (int)'\0')
               {
                  rc = argc;
                  fExit = TRUE;
               }
            }

            break;

         default:

            if(now_arg == FALSE)
            {
               now_arg = TRUE;
               start_ptr = ptr;
            }

            break;
      }
      if(fExit == TRUE)
      {
         break;
      }
      ptr++;
   }

   return rc;
}

//----------------------------------------------------------------------------
#if 0
int sh_strcasecmp(char * s1, char * s2)
{
    char ch1, ch2;

    for(;;)
    {
       ch1 = *s1++;
       if(ch1 >= 'A' && ch1 <= 'Z')
          ch1 += 0x20;

       ch2 = *s2++;
       if(ch2 >= 'A' && ch2 <= 'Z')
          ch2 += 0x20;

        if(ch1 < ch2)
           return -1;
        if(ch1 > ch2)
           return 1;
        if(ch1 == 0)
           return 0;
    }
}
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
