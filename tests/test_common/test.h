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


#ifndef TEST_H__
#define TEST_H__

#include <stdarg.h>

   //--- tn_sprintf.c

int tn_snprintf( char *outStr, int maxLen, const char *fmt, ... );
int vStrPrintf( char *outStr, int maxLen, const char *fmt, va_list args );

//==== Test data types

typedef void (*test_func)(void * par);
typedef int (*int_test_func)(void * par);

#define RC_ARR_SIZE  10

typedef struct _CURRTEST
{
   unsigned char * test_res_data_1;
   int_test_func  test_int_handler;
   void * test_data_1; 

   int rc_base;
   int rc_int;
   int rc_A;
   int rc_B;
   int rc_C;
   int rc_D;
   int rc_E;
   int rc_F;

   int rc_arr[RC_ARR_SIZE];

   int int_state;
   int int_cnt;
   unsigned long timer_val; // value to re-arm int timer
   unsigned long prescaler_val; // value to re-arm int timer prescaler

}CURRTEST;

#define INTERRUPT_RDY_FLAG  1U
#define TASK_A_RDY_FLAG     2U
#define TASK_B_RDY_FLAG     4U
#define TASK_C_RDY_FLAG     8U
#define TASK_D_RDY_FLAG    16U
#define TASK_E_RDY_FLAG    32U


   //--- utils_arch.c

void  test_arch_set_int_timer(CURRTEST * ct, unsigned long timeout);
void  test_arch_disable_timer_int(void);
void test_arch_restart_int_timer(CURRTEST * ct);

   //--- test_common.c

void setup_test_int_timer(unsigned long timeout, // in OS ticks; for MSP430X - max 511
                          int_test_func test_int_func);
void do_tests(const test_func main_test_func_arr[]);

void test_end_signal_create(void);
void test_end_signal_delete(void);
void test_end_do_signal(unsigned int pattern);
int test_end_wait(unsigned int test_end_pattern);

TN_TCB * create_test_task(unsigned int priority, 
                          test_func task_fn);   

   //--- test_utils.c

unsigned long update_crc32(unsigned long crc, unsigned char ch);
void sfsi_randomize(unsigned long seed);    
unsigned long sfsi_rand(void); 
void fill_random_buf(unsigned char * buf, unsigned long buf_size);
int chk_random_buf(unsigned char * buf);
void long_job_delay(int duration);
unsigned long async_read_sys_tick(void);
int find_free_cell(unsigned char ** cell_arr, unsigned long max_cells);
int find_occupied_cell(unsigned char ** cell_arr, unsigned long max_cells);
BOOL tick_val_inside(unsigned long val, 
                     unsigned long min_val, 
                     unsigned long max_val);



extern TN_SEM * semTestPrint;
extern char g_test_print_buf[];

void TestPrint(const char * fmt, ...);
void tdump(MEMINFO * mi);

#define TESTPRINT_BUF_LEN  256

void do_itoa(int val, char * buf, int buf_len);

#endif /* #ifndef TEST_H__ */
