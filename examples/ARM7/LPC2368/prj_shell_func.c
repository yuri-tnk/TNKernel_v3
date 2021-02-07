/**
*
*  Copyright (c) 2013,2016 Yuri Tiomkin
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
#include "lpc236x_uart.h"
#include "project_main.h"
#include "shell.h"


extern const SHELLCMD g_shell_cmd_arr[];

int sh_exec_cmd_psw(SHELLINFO * sh_info);
int sh_exec_cmd_ver(SHELLINFO * sh_info);
int sh_exec_cmd_1(SHELLINFO * sh_info);
int sh_exec_cmd_2(SHELLINFO * sh_info);
int sh_exec_cmd_3(SHELLINFO * sh_info);
int sh_exec_dump_stk(SHELLINFO * sh_info);

const SHELLCMD g_shell_cmd_arr[] =
{
   {"psw",            sh_exec_cmd_psw},
   {"ver",            sh_exec_cmd_ver},
   {"cmd1",           sh_exec_cmd_1},
   {"cmd2",           sh_exec_cmd_2},
   {"cmd3",           sh_exec_cmd_3},
   {"dump_stk",       sh_exec_dump_stk}
};

const int g_shell_cmd_arr_size = sizeof(g_shell_cmd_arr)/sizeof(SHELLCMD);

//----------------------------------------------------------------------------
int sh_exec_cmd_psw(SHELLINFO * sh_info)
{
   (void)uart0_tx_str((char*)("This is a PSW cmd.\r\n"));
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_ver(SHELLINFO * sh_info)
{
   (void)uart0_tx_str((char*)("Ver - 3.0.\r\n"));
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_1(SHELLINFO * sh_info)
{
   (void)uart0_tx_str((char*)("This is a CMD1.\r\n"));
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_2(SHELLINFO * sh_info)
{
   (void)uart0_tx_str((char*)("This is a CMD2.\r\n"));
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_3(SHELLINFO * sh_info)
{
   (void)uart0_tx_str((char*)("This is a CMD3.\r\n"));
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_dump_stk(SHELLINFO * sh_info)
{
   CDLL_QUEUE * que;
   CDLL_QUEUE * que_next;
   TN_TCB * task;
   unsigned int * ptr;
   unsigned int * tmp_ptr;
   unsigned int tmp;
   unsigned int tmp1;
   unsigned long  end_stack_ul;
#define BUF_LEN  24
#define SBUF_LEN 16
   char buf[BUF_LEN];
   char sbuf[SBUF_LEN];

   (void)uart0_tx_str((char*)"The stack \"Usage\" and \"Size\" values are in the \'int\'(not bytes)\r\n");

   for(que = tn_create_queue.next; que != &tn_create_queue; que = que_next)
   {
      que_next = que->next;

      task = get_task_by_create_queue(que);
//Cortex
      end_stack_ul = (unsigned long)task->stk_start - (task->stk_size << 2U) + sizeof(int); // Stack End - Minimal Addr
//????? MSP430 - stack size in int (2 bytes)  ??????
//    end_stack_ul = (unsigned long)task->stk_start - (unsigned long)(task->stk_size << 1) + sizeof(int); // Stack End - Minimal Addr

      ptr = (unsigned int *)end_stack_ul;
      for(;;)
      {
         if(*ptr != TN_FILL_STACK_VAL)
            break;
         ptr++;
      }

      (void)strcpy(buf, "Task: ");
      do_itoa((int)task->base_priority, sbuf, SBUF_LEN);
      (void)strcat(buf, sbuf);
      (void)strcat(buf, "  ");
      (void)uart0_tx_str(buf);

      (void)strcpy(buf, "Usage: ");

      // To make MISRAC happy  - it does not likes a pointer sub/add
      tmp_ptr = task->stk_start;
      tmp_ptr++;
      tmp  = (unsigned long)tmp_ptr;
      tmp1 = (unsigned long)ptr;
      tmp -= tmp1;
      tmp >>= 2U; // Convert to int

      do_itoa((int)tmp, sbuf, SBUF_LEN);
      (void)strcat(buf, sbuf);
      (void)strcat(buf, "  ");
      (void)uart0_tx_str(buf);

      (void)strcpy(buf, "Size: ");
      do_itoa((int)task->stk_size, sbuf, SBUF_LEN);
      (void)strcat(buf, sbuf);
      (void)strcat(buf, "\r\n");
      (void)uart0_tx_str(buf);

#if 0
      TestPrint("Task: %2d  Usage: %4d Size: %4d\r\n",
                task->base_priority,
                (int)(task->stk_start - ptr + 1), // case ptr was ++
                task->stk_size);
#endif
   }
   
   return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
