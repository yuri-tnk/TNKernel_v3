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

#ifndef SHELL_H__
#define SHELL_H__

#define  EVT_EXEC_SHELL    1 

   //--- shell.c

struct _SHELLINFO; 

typedef int (*sh_exec_cmd_handler)(struct _SHELLINFO * sh_info);
typedef int (*sh_send_str_handler)(char * buf);
typedef void (*sh_send_char_handler)(unsigned char ch);

/**
*
* shell command defenition ( in FLASH )
*/
typedef struct _SHELLCMD
{
   const char * cmd_name;                 //!< command name 
   sh_exec_cmd_handler exec_cmd;    //!< command function
}SHELLCMD;

/**
* shell data structure
*
*/
struct _SHELLINFO
{
   int cmd_run;                 //!< command to run number
   int cl_pos;                  //!< parsing cmd line - position
   int cl_size;                 //!< parsing cmd line - size
   unsigned char * cl_buf;      //!< parsing cmd line - pointer to data buffer
   unsigned char * exec_buf;
   int stop_exec;               //!< stop executing flag 

   unsigned char ** argv;       //!< command lines arguments

   int num_cmd;                 //!< number command line parameters
   SHELLCMD * sh_cmd_arr;       //!< pointer to shell commands

   sh_send_str_handler  sh_send_str;   //!< handler to send string
   sh_send_char_handler sh_send_char;  //!< handler to send char
};

typedef struct _SHELLINFO SHELLINFO;


#define  SH_EXECUTE     8    /**< EXEC command code */
#define  SH_BREAK       3    /**< BREAK command code  */

#define  CTRLC_SYM      3    /**< CTRL+C (break) symbol code  */
#define  BS_SYM         8    /**< ASCII symbol code  */
#define  LF_SYM        10    /**< ASCII symbol code  */
#define  CR_SYM        13    /**< ASCII symbol code  */
#define  DC_1_SYM      17    /**< ASCII symbol code  */
#define  DC_2_SYM      18    /**< ASCII symbol code  */
#define  ESC_SYM       27    /**< ASCII symbol code  */
#define  SPACE_SYM     32    /**< ASCII symbol code  */
#define  DEL_SYM     0x7F    /**< DEL symbol code  */

#define  SH_MAX_ARG                        4    /**< maximum arguments in the shell command line */
#define  SH_CL_SIZE                       96    /**< maximum symbols in the shell command line */
#define  SHELL_UART_RX_PAYLOAD_BUF_SIZE   64    /**< buffer size for uart payload */


/**
*    shell UART driver
*
*/
typedef struct _UARTDRV
{
   unsigned char * buf; //!< pointer to RX buffer  
   int max_buf_size;    //!< buffer size
   int pos;             //!< position in buffer
}UARTDRV;


/**
*  Global data structure with overall shell data
*
*/
typedef struct _BSP_SHELL_DS
{
   SHELLINFO shell_info;                   //!< shell info
   char * shell_argv[SH_MAX_ARG];          //!< pointers to input arguments 
   unsigned char shell_cl_buf[SH_CL_SIZE]; //!< command line input buffer
   unsigned char shell_exec_buf[SH_CL_SIZE]; //!< command line exec buffer
   UARTDRV sh_uart_drv;                    //!< shell UART driver
   unsigned char sh_uart_drv_payload_buf[SHELL_UART_RX_PAYLOAD_BUF_SIZE]; //!< shell UART driver payload buffer
}BSP_SHELL_DS;


void sh_init(BSP_SHELL_DS * p_sh);
void sh_input(SHELLINFO * sh_info, unsigned char ch);
int sh_start_cmd_exec(SHELLINFO * sh_info);
int sh_do_cmd_exec(SHELLINFO * sh_info);
int sh_prompt(SHELLINFO * sh_info, int send_CR_LF);
int sh_stop_cmd_exec(SHELLINFO * sh_info);
int sh_echo(SHELLINFO * sh_info, unsigned char ch);
int sh_putchar(SHELLINFO * sh_info, unsigned char ch);

extern const SHELLCMD g_shell_cmd_arr[];
extern const int g_shell_cmd_arr_size;

#if defined TNKERNEL_PORT_MSP430X
extern TN_DQUEUE  * queueShellEvents;
#else
extern TN_DQUEUE queueShellEvents;
#endif

#endif
