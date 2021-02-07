//#include "stm32f4xx.h"


#include <time.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>
#include <stdint.h>

#include "tn_config.h"
#include "tn.h"
#include "tn_port.h"

#ifdef __cplusplus
 extern "C" {
#endif 

void SysTick_Handler(void);
void SystemInit(void);

#ifdef __cplusplus
}
#endif


volatile unsigned int g_cnt = 0;

//-- The OS ticks task - must

#define  TASK_OS_TICK_PRIORITY       0
#define OS_TICK_TASK_STACK_SIZE    128
TN_TCB tn_os_tick_task __attribute__((aligned(0x8)));
unsigned int tn_os_tick_task_stack[OS_TICK_TASK_STACK_SIZE] __attribute__((aligned(0x8)));//TN_ALIGN_ATTR_END;
void tn_os_tick_task_func(void * param);

/*
*/

#define __no_init 

#define  TASK_UART1_RX_PRIORITY      3
#define  TASK_MAIN_PRIORITY          6
#define  TASK_IO_PRIORITY            8


#define  TASK_UART1_RX_STK_SIZE    512
#define  TASK_MAIN_STK_SIZE       1024
#define  TASK_IO_STK_SIZE          512

__no_init unsigned int task_uart1_rx_stack[TASK_UART1_RX_STK_SIZE];
__no_init unsigned int task_main_stack[TASK_MAIN_STK_SIZE];
__no_init unsigned int task_io_stack[TASK_IO_STK_SIZE];

__no_init TN_TCB  task_uart1_rx;
__no_init TN_TCB  task_main;
__no_init TN_TCB  task_io;

void task_uart1_rx_func(void * par);
void task_main_func(void * par);
void task_io_func(void * par);

void idle_hook_func(void * par);

#define UInt8  unsigned char
#define UInt16 unsigned short

typedef union 
{
  struct 
  { 
      UInt8 l; 
      UInt8 h; 
  } B;
  unsigned short W;
} RegisterPair;

typedef struct {
    RegisterPair AF;
    RegisterPair BC;
    RegisterPair DE;
    RegisterPair HL;
    RegisterPair IX;
    RegisterPair IY;
    RegisterPair PC;
    RegisterPair SP;
    RegisterPair AF1;
    RegisterPair BC1;
    RegisterPair DE1;
    RegisterPair HL1;
    RegisterPair SH;
    UInt8 I;
    UInt8 R;
    UInt8 R2;

    UInt8 iff1;
    UInt8 iff2;
    UInt8 im;
    UInt8 halt;	
    UInt8 ei_mode;
} CpuRegs;
  
static UInt8  ZSXYTable[256] = {0}; 

typedef struct
{
    CpuRegs       regs;             /* Active register bank            */ 
} R800;

//-------- Semaphores -----------------------

TN_SEM * semTxUart1;             //!< UART1 semaphore 
TN_SEM  *semTxBufUart1;

//__no_init TN_SEM semRS485;      // for RS485 serial port
TN_SEM * semTestPrint;  // for TestPrint() func

//-------- Queues -----------------------

#define QUEUE_ACTIONS_SIZE  8

TN_DQUEUE  * queueShellEvents;

//-------- Misc -------
extern idle_task_hook_func  tn_idle_task_hook_func;

//--- Not OS globals

//__no_init char g_test_print_buf[TESTPRINT_BUF_LEN];

//__no_init BSP_UART_DS  g_uart_ds;
//BSP_UART_DS * g_p_uart_ds = &g_uart_ds;     //!< global UART data

//__no_init BSP_SHELL_DS  g_shell_ds;
//BSP_SHELL_DS * g_p_shell_ds = &g_shell_ds;

#if defined  USE_DYN_OBJ

#define SYS_OBJ_MEM_SIZE  4096
//#pragma data_alignment = 8
 __align(8) __no_init unsigned char sys_obj_mem_arr[SYS_OBJ_MEM_SIZE];

#endif


#define  TASK_EXIT_PRIORITY      3
#define  TASK_EXIT_STK_SIZE    512

//#pragma data_alignment = 8
 __align(8) __no_init unsigned int task_exit_stack[TASK_EXIT_STK_SIZE];
__no_init TN_TCB  task_exit;

void task_exit_func(void * par);

#define  TASK_DLY_PRIORITY      4
#define  TASK_DLY_STK_SIZE    256

//#pragma data_alignment = 8
 __align(8) __no_init unsigned int task_dly_stack[TASK_DLY_STK_SIZE];
__no_init TN_TCB  task_dly;

void task_dly_func(void * par);

//extern MEMINFO  g_test_mem;
//extern __no_init unsigned char g_test_mem_buf[TEST_MEM_POOL_SIZE];
//extern TN_SEM   g_test_rdy_sem;
volatile int g_ssd = 0;
   //--- NVIC

#define  ISRPENDING      (1u<<22)
#define  VECTACTIVE      (0x000001FF)
#define  rNVIC_ICSR      (*((volatile unsigned int*)0xE000ED04))

#define C_FLAG      0x01
#define N_FLAG      0x02
#define P_FLAG      0x04
#define V_FLAG      0x04
#define X_FLAG      0x08
#define H_FLAG      0x10
#define Y_FLAG      0x20
#define Z_FLAG      0x40
#define S_FLAG      0x80 

R800 g_R800;

static void ADC(R800* r800, UInt8 reg) 
{
    int rv = r800->regs.AF.B.h + reg + (r800->regs.AF.B.l & C_FLAG);
    r800->regs.AF.B.l = ZSXYTable[rv & 0xff] | ((rv >> 8) & C_FLAG) |
        ((r800->regs.AF.B.h ^ rv ^ reg) & H_FLAG) |
        ((((reg ^ r800->regs.AF.B.h ^ 0x80) & (reg ^ rv)) >> 5) & V_FLAG);
    r800->regs.AF.B.h = rv;
}

static void SBCW(R800* r800, UInt16 reg) 
{
    int regVal = r800->regs.HL.W;
    int rv = regVal - reg - (r800->regs.AF.B.l & C_FLAG);
    r800->regs.SH.W   = regVal + 1;
    r800->regs.AF.B.l = (((regVal ^ reg ^ rv) >> 8) & H_FLAG) | N_FLAG |
        ((rv >> 16) & C_FLAG) | ((rv & 0xffff) ? 0 : Z_FLAG) | 
        ((((reg ^ regVal) & (regVal ^ rv)) >> 13) & V_FLAG) |
        ((rv >> 8) & (S_FLAG | X_FLAG | Y_FLAG));
    r800->regs.HL.W = rv;
    //delayAdd16(r800);
}
 
void SystemInit(void)
{

}
/*
void tn_disable_interrupt_nsc(void)
{
  (void)tn_cpu_save_sr();
}
*/

void tn_cpu_int_enable(void)
{
   // Inable a peripheral Int (Sys_Tick etc.)
   // The core Int is disabled here

   g_ssd++;
}


//----------------------------------------------------------------------------
//void tn_int_stack_init(INTSTACKINFO * si, unsigned int * int_stack_mem)
//{
   //return TERR_NO_ERR;
//}

#if 0
int tn_inside_int(void)
{
   if(rNVIC_ICSR & VECTACTIVE)
      return 1;
   return 0;
}
#endif
//----------------------------------------------------------------------------
int tn_app_init(void)
{
   int rc;
   //int err;


   SBCW(&g_R800, 0x1235);
   ADC(&g_R800, 0x11); 

   tn_idle_task_hook_func = NULL;  //idle_hook_func;

   //--- OS ticks task - must

   tn_os_tick_task.id_task = 0UL;
   rc = tn_task_create(&tn_os_tick_task,          //-- task TCB
                        tn_os_tick_task_func,      //-- task function
                        0,                         //-- task priority
                        &(tn_os_tick_task_stack    //-- task stack first addr in memory
                        [OS_TICK_TASK_STACK_SIZE - 1]),
                        OS_TICK_TASK_STACK_SIZE,   //-- task stack size (in int,not bytes)
                        NULL,                      //-- task function parameter
                        TN_TASK_OS_TICK);          //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      return -1;
   }

   //--- Task Exit

   task_exit.id_task = 0;
   rc = tn_task_create(&task_exit,                      //-- task TCB
                 task_exit_func,                   //-- task function
                 TASK_EXIT_PRIORITY,               //-- task priority
                 &(task_exit_stack                 //-- task stack first addr in memory
                    [TASK_EXIT_STK_SIZE-1]),
                 TASK_EXIT_STK_SIZE,               //-- task stack size (in int,not bytes)
                 NULL,                           //-- task function parameter
                 TN_TASK_START_ON_CREATION);     //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      return -1;
   }

   //--- Task Dly

   task_dly.id_task = 0;
   rc = tn_task_create(&task_dly,                      //-- task TCB
                 task_dly_func,                   //-- task function
                 TASK_DLY_PRIORITY,               //-- task priority
                 &(task_dly_stack                 //-- task stack first addr in memory
                    [TASK_DLY_STK_SIZE-1]),
                 TASK_DLY_STK_SIZE,               //-- task stack size (in int,not bytes)
                 NULL,                           //-- task function parameter
                 TN_TASK_START_ON_CREATION);     //-- Creation option
   if(rc != TERR_NO_ERR)
   {
      return -1;
   }

   return 0;
}



//----------------------------------------------------------------------------
int main(void)
{
   int rc;

   ADC(&g_R800, 0x11); 

   //-- if no err, should never return
 
   rc = tn_start_system(sys_obj_mem_arr,
                        (unsigned long) SYS_OBJ_MEM_SIZE,
                        NULL); // NULL is OK for the MSP430X  (int_stack_mem);

   // if we are here, something goes wrong

   if(rc != TERR_NO_ERR)
   {
      // Add your own error handler here, (if you are really need it)
      for(;;)
      {
      }
   }
   return 0;
}

//----------------------------------------------------------------------------
void tn_os_tick_task_func(void * param) // Must
{
   int rc;

   for(;;)
   {
      rc = tn_sem_acquire(&tn_sys_tick_sem, TN_WAIT_INFINITE);
      if(rc >= 0) // OK
      {
         (void)tn_os_timers_tick_proc();
#if defined USER_TIMERS
         (void)tn_user_timers_tick_proc();
#endif
      }
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void task_exit_func(void * par)
{
   int i;
   for(i=0; i< 10; i++);
   //tn_task_sleep(10);
}
 
void task_dly_func(void * par)
{
   for(;;)
   {
     tn_task_sleep(10);
   } 
}


//----------------------------------------------------------------------------
void SysTick_Handler(void)
{
   tn_tick_int_processing();

  //-- !!!    For the Cortex CPU, this function is a last func in 
  //--     any user's interrupt handler

   tn_int_exit();  

}
