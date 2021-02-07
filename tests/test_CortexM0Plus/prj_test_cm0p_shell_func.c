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
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#if defined  TNKERNEL_PORT_MSP430X
  #include "types.h"
  #include "uart.h"
#else
  #include "test.h"
#endif
#include "shell.h"
#include "project_main.h"


//extern const test_func g_sem_tests_arr[];
//extern const test_func g_fmp_tests_arr[];
//extern const test_func g_mutex_tests_arr[];
//extern const test_func g_dqueue_tests_arr[];
//extern const test_func g_mailbox_tests_arr[];
//extern const test_func g_evf_tests_arr[];
//extern const test_func g_task_tests_arr[];
extern const test_func g_timer_tests_arr[];

int sh_exec_cmd_psw(SHELLINFO * sh_info);
int sh_exec_cmd_ver(SHELLINFO * sh_info);
int sh_exec_cmd_1(SHELLINFO * sh_info);
int sh_exec_cmd_2(SHELLINFO * sh_info);
int sh_exec_cmd_3(SHELLINFO * sh_info);
int sh_exec_dump_stk(SHELLINFO * sh_info);
int sh_exec_test_sem(SHELLINFO * sh_info);
int sh_exec_test_fmp(SHELLINFO * sh_info);
int sh_exec_test_mutex(SHELLINFO * sh_info);
int sh_exec_test_dqueue(SHELLINFO * sh_info);
int sh_exec_test_mailbox(SHELLINFO * sh_info);
int sh_exec_test_evf(SHELLINFO * sh_info);
int sh_exec_test_task(SHELLINFO * sh_info);
int sh_exec_test_timer(SHELLINFO * sh_info);
int sh_exec_test_all(SHELLINFO * sh_info);

const SHELLCMD g_shell_cmd_arr[] =
{
   {"psw",            sh_exec_cmd_psw},
   {"ver",            sh_exec_cmd_ver},
   {"cmd1",           sh_exec_cmd_1},
   {"cmd2",           sh_exec_cmd_2},
   {"cmd3",           sh_exec_cmd_3},
   {"dump_stk",       sh_exec_dump_stk},
   {"test_sem",       sh_exec_test_sem},
   {"test_fmp",       sh_exec_test_fmp},
   {"test_mutex",     sh_exec_test_mutex},
   {"test_dque",      sh_exec_test_dqueue},
   {"test_mbox",      sh_exec_test_mailbox},
   {"test_evf",       sh_exec_test_evf},
   {"test_task",      sh_exec_test_task},
   {"test_timer",     sh_exec_test_timer},
   {"test_all",       sh_exec_test_all},
};

const int g_shell_cmd_arr_size = sizeof(g_shell_cmd_arr)/sizeof(SHELLCMD);

static const char g_msg1[] = "Not implemented for STM32L053R8\r\n";
//----------------------------------------------------------------------------
int sh_exec_cmd_psw(SHELLINFO * sh_info)
{
   TestPrint("This is a PSW cmd.\r\n");
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_ver(SHELLINFO * sh_info)
{
   TestPrint("Ver - 3.0\r\n");
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_1(SHELLINFO * sh_info)
{
   TestPrint("This is a CMD1.\r\n");
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_2(SHELLINFO * sh_info)
{
   TestPrint("This is a CMD2.\r\n");
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_cmd_3(SHELLINFO * sh_info)
{
   TestPrint("This is a CMD3.\r\n");
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_dump_stk(SHELLINFO * sh_info)
{
   CDLL_QUEUE * que;
   CDLL_QUEUE * que_next;
   TN_TCB * task;
   unsigned int * ptr;
   unsigned long  end_stack_ul;

   TestPrint("The stack related values are in the \'int\'(not bytes)\r\n");

   for(que = tn_create_queue.next; que != &tn_create_queue; que = que_next)
   {
      que_next = que->next;

      task = get_task_by_create_queue(que);
      end_stack_ul = (unsigned long)task->stk_start - (unsigned long)(task->stk_size << (sizeof(int)/2U)) + sizeof(int); // Stack End - Minimal Addr
      ptr = (unsigned int *)end_stack_ul;
      for(;;)
      {
         if(*ptr != TN_FILL_STACK_VAL)
            break;
         ptr++;
      }

      TestPrint("Task: %2d  Usage: %4d Size: %4d\r\n",
                task->base_priority,
                (int)(task->stk_start - ptr + 1), // case ptr was ++
                task->stk_size);
   }
   
   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_sem(SHELLINFO * sh_info)
{
//   do_tests(g_sem_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_fmp(SHELLINFO * sh_info)
{

//   do_tests(g_fmp_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_mutex(SHELLINFO * sh_info)
{
//   do_tests(g_mutex_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_dqueue(SHELLINFO * sh_info)
{
 //  do_tests(g_dqueue_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}
//----------------------------------------------------------------------------
int sh_exec_test_mailbox(SHELLINFO * sh_info)
{
//   do_tests(g_mailbox_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_evf(SHELLINFO * sh_info)
{
//   do_tests(g_evf_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_task(SHELLINFO * sh_info)
{
//   do_tests(g_task_tests_arr);
   (void)uart2_tx_str((char*)g_msg1);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_timer(SHELLINFO * sh_info)
{
   do_tests(g_timer_tests_arr);

   return 0;
}

//----------------------------------------------------------------------------
int sh_exec_test_all(SHELLINFO * sh_info)
{
//   do_tests(g_sem_tests_arr);
//   do_tests(g_fmp_tests_arr);
//   do_tests(g_mutex_tests_arr);
//   do_tests(g_dqueue_tests_arr);
//   do_tests(g_mailbox_tests_arr);
//   do_tests(g_evf_tests_arr);
//   do_tests(g_task_tests_arr);
   do_tests(g_timer_tests_arr);

   return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

