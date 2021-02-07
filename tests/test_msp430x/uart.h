/*

  TNKernel real-time kernel

  Copyright © 2004, 2016 Yuri Tiomkin
  All rights reserved.

  Permission to use, copy, modify, and distribute this software in source
  and binary forms and its documentation for any purpose and without fee
  is hereby granted, provided that the above copyright notice appear
  in all copies and that both that copyright notice and this permission
  notice appear in supporting documentation.

  THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.

*/

  /* ver 3  */



#ifndef  MSP430X_UART_H
#define  MSP430X_UART_H

#ifdef __cplusplus
extern "C" 
{
#endif

/**  
* Its assumed that 'ui' is a global array of possible UARTs(all) configuration
* parameters and it is placed in CODE (CPU flash)
*/

#define  UART_USE_RX_DMA      0x0001U   /**< use DMA for RX */
#define  UART_USE_TX_DMA      0x0002U   /**< use DMA for TX */
#define  UART_USE_RX_INT      0x0004U   /**< use RX interrupts */
#define  UART_USE_TX_INT      0x0008U   /**< use TX interrupts */


#define  SET_UART_RX_TIMEOUT       1   /**< parameter for ioctl */
#define  SET_UART_TX_TIMEOUT       2   /**< parameter for ioctl */

#define  UART_STOP_TX_END          1   /**< use checking TX end */
#define  UART_STOP_TX_CTS          2   /**< use CTS  */

/**  
*   Predefined configuration structure (located in FLASH)
*  
*/  
typedef struct _UARTSINFO
{
   uint16_t uartConfigNum;      //!< Num of the CONFI
   uint32_t baseAddress;        //!< current UART base address
   uint8_t  selectClockSource;  //!< clock source
   uint8_t  parity;             //!< parity
   uint8_t  msborLsbFirst;      //!< send MSB first 
   uint8_t  numberofStopBits;   //!< stop bits (1,2 etc)
   uint8_t  baudRateReg_BR0;    //!< baud rate - register BR0 value
   uint8_t  baudRateReg_BR1;    //!< baud rate - register BR1 value
   uint16_t ModReg_MCTL;        //!< included overSampling
   uint16_t configFlags;        //!< configuration flags 
}UARTSINFO;


/**  
*
*   UART data structure (located in RAM)
*/  
typedef struct _UARTDS
{
   uint32_t baseAddress;              //!< UART base address (0 if UART not open)
   uint32_t fd_flags;                 //!< reserved

   TN_SEM * semTxUart;                  //!< TX semaphore for end of TX
   TN_SEM * semTxBufUart;               //!< TX semaphore for sharing UART protection
   unsigned char * tx_buf;            //!< TX data buffer
   long tx_timeout;                   //!< TX timeout value 
   long tx_timeout_cnt;               //!< TX timeout counter 
   int max_tx_buf_size;               //!< current TX buffer size
   int tx_idx;                        //!< TX buffer current index
   int tx_cnt;                        //!< TX bytes counter
   int tx_stop_reason;                //!< Actual only when CTS is in use
   int use_cts;                       //!< To use or not CTS  signal  

   unsigned char * rx_buf;            //!< RX data buffer
   long rx_timeout;                   //!< RX timeout value 
   long rx_timeout_cnt;               //!< RX timeout counter 
   int rx_buf_size;                   //!< current RX buffer size
   int rx_tail;                       //!< tail counter of RX
   int rx_head;                       //!< head counter of RX
   int rx_cnt;                        //!< num RXed bytes
}UARTDS;


#define  OFS_UCAxCTL0          (0x0001u)      /**< UCAxCTL0 offset */ 
#define  OFS_UCAxCTL1          (0x0000u)      /**< UCAxCTL1 offset */ 
#define  UCAxCTL1              UCAxCTLW0_L    /**< USCI Ax Control Register 1 */
#define  UCAxCTL0              UCAxCTLW0_H    /**< USCI Ax Control Register 0 */
          
#define  OFS_UCAxBRW           (0x0006u)      /**< USCI Ax Baud Word Rate 0 */
#define  OFS_UCAxBRW_L         OFS_UCAxBRW    /**< UCAxBRW low byte */ 
#define  OFS_UCAxBRW_H         OFS_UCAxBRW+1  /**< UCAxBRW high byte */ 

#define  OFS_UCAxBR0           (0x0006u)      /**< UCAxBR0 offset */ 
#define  OFS_UCAxBR1           (0x0007u)      /**< UCAxBR1 offset */  
#define  OFS_UCAxMCTL          (0x0008u)      /**< USCI Ax Modulation Control */

#define  OFS_UCAxRXBUF         (0x000Cu)      /**< USCI Ax Receive Buffer */
#define  OFS_UCAxTXBUF         (0x000Eu)      /**< USCI Ax Transmit Buffer */
 
#define  OFS_UCAxIE            (0x001Cu)      /**<  UCAxIE offset */ 


/**
*
*  The following are values that can be passed to the UART_init() API
*  as the parity parameter.
*/
#define UART_NO_PARITY   0x00    /**< no parity */ 
#define UART_ODD_PARITY  0x01    /**< odd parity */ 
#define UART_EVEN_PARITY 0x02    /**< even parity */ 

/**
*
*  The following are values that can be passed to the UART_init() API
*  as the selectClockSource parameter.
*/
#define UART_CLOCKSOURCE_ACLK    UCSSEL__ACLK     /**< the UART clock source is ACLK */ 
#define UART_CLOCKSOURCE_SMCLK   UCSSEL__SMCLK    /**< the UART clock source is SMCLK */ 

/*
*
*  The following are values that can be passed to the UART_init() API
*  as the numberofStopBits parameter.
*/
#define UART_ONE_STOP_BIT    0x00      /**< use 1 stop bit */ 
#define UART_TWO_STOP_BITS   UCSPB     /**< use 2 stop bits */ 

/*
*
*  The following are values that can be passed to the UART_init() API
*  as the msborLsbFirst parameter.
*/
#define UART_MSB_FIRST    UCMSB   /**< send MSB bit first */ 
#define UART_LSB_FIRST    0x00    /**< send LSB bit first */ 

/**
*
* The following are values that can be passed to the parity parameter for
* functions: USCI_A_UART_initAdvance().
*/
#define USCI_A_UART_NO_PARITY        0x00   /**<  not use parity */ 
#define USCI_A_UART_ODD_PARITY       0x01   /**<  use odd parity */ 
#define USCI_A_UART_EVEN_PARITY      0x02   /**<  use even parity */ 

/**
*
*   The following are values that can be passed to the uartMode parameter for
*   functions: USCI_A_UART_initAdvance().
*/
#define USCI_A_UART_MODE         UCMODE_0   /**< set USCI_A mode as UART */ 

    //!< bsp_uart.h

int bsp_uart_open(UARTDS * fd,  char* name, int32_t flags, int32_t mode);
int bsp_uart_close(UARTDS * fd);
int bsp_uart_getc(UARTDS * fd);
int bsp_uart_read(UARTDS * fd, char * buf, int len);
int bsp_uart_write(UARTDS * fd,  char* buf, int len);
int bsp_uart_write_ex(UARTDS * fd, char* buf, int len); // check CTS
int bsp_uart_ioctl(UARTDS * fd, int32_t req, void * arg);

/**
*    UART 0 / UART 1/ UART 2 
*/

/**
*  UARTs pre-defined configurations
* 
*/
typedef enum UART_CGF_E
{
  CFG_UART0_57600_8N1_20MHZ = 1,  //!< UART0, 57600 bauds, F = 20 MHz
  CFG_UART0_38400_8N1_20MHZ,      //!< UART0, 38400 bauds, F = 20 MHz
  CFG_UART1_38400_8N1_20MHZ,      //!< UART1, 38400 bauds, F = 20 MHz
  CFG_UART2_38400_8N1_20MHZ,      //!< UART2, 38400 bauds, F = 20 MHz
  CFG_UART1_38400_8N1_1MHZ,       //!< UART1, 38400 bauds, F = 1 MHz
  CFG_UART2_38400_8N1_1MHZ,       //!< UART2, 38400 bauds, F = 1 MHz
  CFG_UART1_38400_8N1_4MHZ,       //!< UART1, 38400 bauds, F = 4 MHz
  CFG_UART2_38400_8N1_4MHZ,       //!< UART2, 38400 bauds, F = 4 MHz
  CFG_UART1_38400_8N1_2MHZ,       //!< UART1, 38400 bauds, F = 2 MHz
  CFG_UART2_38400_8N1_2MHZ        //!< UART2, 38400 bauds, F = 2 MHz
}UART_CFG_ET;

#define  UART0_TX_MAX_BUFF_SIZE        96  /**< for terminal(RS232) */
#define  UART0_RX_BUF_SIZE             64  /**< for terminal(RS232) */
#define  UART0_TX_BUF_SIZE             64  /**< for shell */

#define  UART1_TX_MAX_BUFF_SIZE       256  /**< for terminal(RS485) */
#define  UART1_RX_BUF_SIZE            256  /**< for terminal(RS485) */

#define  UART2_TX_MAX_BUFF_SIZE      (512 + 128) 
#define  UART2_RX_BUF_SIZE           (512 + 512) 

/**
*   The data structure for all UARTs in the system
*
*/

// #define USE_UART_0

typedef struct _BSP_UART_GLOBALS
{
#if defined USE_UART_0  
//   UARTDS g_uart0_ds;                             //!< UART0 data
//   unsigned char g_uart0_rx_buf[UART0_RX_BUF_SIZE];  //!< UART0 RX buffer
#endif  
#if defined USE_UART_1  
   UARTDS g_uart1_ds;                             //!< UART1 data
   unsigned char g_uart1_rx_buf[UART1_RX_BUF_SIZE];  //!< UART1 RX buffer
#endif  
#if defined USE_UART_2  
//   UARTDS g_uart2_ds;                             //!< UART2 data
//   unsigned char g_uart2_rx_buf[UART2_RX_BUF_SIZE];  //!< UART2 RX buffer
#endif  
}BSP_UART_DS;

//!< UART 0

//!< UART 1

int uart1_tx_str(char * str);
void uart1_tx_char(unsigned char ch);
int uart1_tx_buf(char * buf, int len);
int uart1_tx_buf_ex(char * buf, int len);

//!< UART 2

//-------------------------------------------

#define  EEXIST      (100)
#define  EBUSY       (101)
#define  ENOENT      (102)
#define  ENODEV      (103)
#define  ENXIO       (104)
#define  EAGAIN      (105)
#define  ETIMEDOUT   (110)

#ifndef  MIN
#define  MIN(a,b)    (((a)<(b))?(a):(b))
#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef  __BSP_UART_H__ */


