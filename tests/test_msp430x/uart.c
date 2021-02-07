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

 /* ver 3.0 */

#include "io430.h"
#include <string.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "project_main.h"
#include "uart.h"


UARTSINFO * bsp_get_uart_config_info_addr(void);
UARTDS * bsp_get_uart_data_struct_addr(uint32_t num_uart);
unsigned char * bsp_get_uart_rx_buf(int32_t num_uart);

extern TN_SEM * semTxUart1;
extern TN_SEM * semTxBufUart1;

const UARTSINFO g_uart_config_info[] =
{
  {
     CFG_UART1_38400_8N1_20MHZ,          // uint16_t uartConfigNum;   --  Num of the CONFI
     USCI_A1_BASE,                       // uint32_t baseAddress;
     UART_CLOCKSOURCE_SMCLK,             // uint8_t  selectClockSource;
     UART_NO_PARITY,                     // uint8_t  parity;
     UART_LSB_FIRST,                     // uint8_t  msborLsbFirst;
     UART_ONE_STOP_BIT,                  // uint8_t  numberofStopBits;
      32,                                // uint8_t  baudRateReg_BR0;
       0,                                // uint8_t  baudRateReg_BR1;
     145,                                // uint16_t ModReg_MCTL;    -- included overSampling(if any)
     UART_USE_RX_INT | UART_USE_TX_INT   // uint16_t configFlags;
  },
  
  //-- Terminator
  {
     0,  // uint16_t uartConfigNum;
     0,  // uint32_t baseAddress;
     0,  // uint8_t  selectClockSource;
     0,  // uint8_t  parity;
     0,  // uint8_t  msborLsbFirst;
     0,  // uint8_t  numberofStopBits;
     0,  // uint8_t  baudRateReg_BR0;
     0,  // uint8_t  baudRateReg_BR1;
     0,  // uint16_t ModReg_MCTL;
     0   // uint16_t configFlags;
  }
};

//--- Global variables for UART

extern BSP_UART_DS * g_p_uart_ds;

//--- Local functions prototypes

static int bsp_sys_uart_close(UARTDS * par);
static int bsp_sys_uart_setup(UARTSINFO * par, int uart_config_num);
static int bsp_sys_uart_set_int(UARTSINFO * par);
static void set_uart_rx_timeout(UARTDS * fd, int32_t req, void * arg);
static void set_uart_tx_timeout(UARTDS * fd, int32_t req, void * arg);

/**
*
*    @brief {Function Name} 
*    
*    @details {Detailed Desctiption}           
*
*    @param [in] <parameter-name> { parameter description } 
*
*    @param [out] <parameter-name> { parameter description } 
*    
*    @return { description of the return value } 
*
*    @pre { description of the precondition } 
*
*    @post { description of the postcondition } 
*/
//----------------------------------------------------------------------------
int uart1_tx_buf(char * buf, int len)
{
   int rc;// = TERR_NO_ERR;
   static int fFirst = 0;


   if(buf == NULL || len <= 0)
   {
      rc = -ENOENT;
   }
   else
   {
      if(len > 255)
      {
         len = 255;
      }
      (void)tn_sem_acquire(g_p_uart_ds->g_uart1_ds.semTxBufUart, TN_WAIT_INFINITE);


     //!< Set I/O pins as "RS485 TX"

      P5OUT |= (unsigned char)BIT2; //!< New /RE = 1  RX out - 3-State
      P5OUT |= (unsigned char)BIT4; //!<  DE = 1  TX out - Enable

      if(fFirst == 0)
      {
         fFirst = 1;
         (void)tn_task_sleep(10);
      }
      else
      {
         (void)tn_task_sleep(1);
      }
      rc = bsp_uart_write(&g_p_uart_ds->g_uart1_ds, buf, len);

      (void)tn_task_sleep(1);

     //!< Set I/O pins as "RS485 RX"

      P5OUT &= ~((unsigned char)BIT2);  //!< New /RE = 0  RX out - Enable
      P5OUT &= ~((unsigned char)BIT4);  //!<  DE = 0  TX out - Disable

      (void)tn_sem_signal(g_p_uart_ds->g_uart1_ds.semTxBufUart);
   }

   return rc;
}

/**
*
*    @brief {Function Name} 
*    
*    @details {Detailed Desctiption}           
*
*    @param [in] <parameter-name> { parameter description } 
*
*    @param [out] <parameter-name> { parameter description } 
*    
*    @return { description of the return value } 
*
*    @pre { description of the precondition } 
*
*    @post { description of the postcondition } 
*/
//----------------------------------------------------------------------------
void uart1_tx_char(unsigned char ch)
{
   char buf[4];
   buf[0] = ch;
   
   (void)uart1_tx_buf(buf, 1);
}

/**
*
*    @brief {Function Name} 
*    
*    @details {Detailed Desctiption}           
*
*    @param [in] <parameter-name> { parameter description } 
*
*    @param [out] <parameter-name> { parameter description } 
*    
*    @return { description of the return value } 
*
*    @pre { description of the precondition } 
*
*    @post { description of the postcondition } 
*/
//----------------------------------------------------------------------------
int uart1_tx_str(char * str)
{
   int rc = -ENOENT;
   int len;
   
   if(str != NULL) 
   {   
      len = (int)strlen(str);
      rc = uart1_tx_buf(str, len);
   }

   return rc;
}   

/**
*
*    @brief    bsp_get_uart_config_info_addr
*    
*    @details  Get pointer to global variable 'g_uart_config_info'
*
*    @param [in] Nothing
*
*    
*    @return  Address of 'g_uart_config_info'
*
*/
//----------------------------------------------------------------------------
UARTSINFO * bsp_get_uart_config_info_addr(void)
{
   return (UARTSINFO *)conv_const_ptr_to_ptr((const void*) &g_uart_config_info[0]);
}

/**
*
*    @brief    bsp_get_uart_data_struct_addr
*    
*    @details  Get pointer to global variable 'g_uartXX_data'
*              according to 'num_uart'     
*
*    @param [in] num_uart (USCI_A0_BASE etc)
*
*    
*    @return  Address of 'g_uartXX_data'
*
*/
//----------------------------------------------------------------------------
UARTDS * bsp_get_uart_data_struct_addr(uint32_t num_uart)
{
   UARTDS * ptr; // = NULL;
  
//   if(num_uart == (uint32_t)USCI_A1_BASE)
   {
      ptr = &g_p_uart_ds->g_uart1_ds;
   }

   return ptr;
}

/**
*
*    @brief    bsp_get_uart_rx_buf
*    
*    @details  Get pointer to global variable 'g_uartXX_rx_buf'
*              according to 'num_uart'     
*
*    @param [in] num_uart (USCI_A0_BASE etc)
*
*    
*    @return  Address of 'g_uartXX_rx_buf'
*
*/
//----------------------------------------------------------------------------
unsigned char * bsp_get_uart_rx_buf(int32_t num_uart)
{
   unsigned char * ptr; // = NULL;
#if 0   
   if(num_uart == (int32_t)USCI_A0_BASE)
   {
#if defined USE_UART_0     
      ptr = g_p_uart_ds->g_uart0_rx_buf;
#endif      
   } 
   else if(num_uart == (int32_t)USCI_A1_BASE)
#endif
   {
      ptr = g_p_uart_ds->g_uart1_rx_buf;
   }   
#if 0
   else 
   {
#if defined USE_UART_2     
      if(num_uart == (int32_t)USCI_A2_BASE)
      {
         ptr = g_p_uart_ds->g_uart2_rx_buf;
      }
#endif      
   }
#endif
   return ptr;
}

/**
*
*    @brief    bsp_uart_open
*    
*    @details  open uart for operating
*
*    @param [in]  fd     - driver descriptor 
*                 name   - NULL here (not used)
*                 flags  - 0 (not used)
*                 mode  - uart operating mode (predefined constant)
*
*    @return  E_NO_ERR if operation complete, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
int bsp_uart_open(UARTDS * fd, char* name, int32_t flags, int32_t mode)
{
   TN_INTSAVE_DATA
     
   UARTSINFO * par;
   UARTDS * pdata; // = NULL;
   int rc;
//   int i;

   if(fd == NULL)
   {  
      rc = -ENOENT;
   } 
   else
   {  
      if(fd->baseAddress != 0UL)       //!< prevent re-open
      {
         rc = -EEXIST;
      }
      else
      {  
         tn_disable_interrupt();

         if(fd->fd_flags == 0xFFFFFFFFUL) //!<  open is in progress
         {
            rc = -EBUSY;
            tn_enable_interrupt();
         }
         else
         {  
            fd->fd_flags = 0xFFFFFFFFUL;  //!< as flag for open in progress

            tn_enable_interrupt();

            par = bsp_get_uart_config_info_addr();
            while(par->uartConfigNum != 0U)
            {
               if(par->uartConfigNum == (unsigned short)mode)
               {
                  break;
               }
               par++;
            }

            if(par->uartConfigNum == 0U) // configuration not found
            {  
               rc = -ENOENT;
            }
            else
            {  
               rc = -ENOENT;


               pdata = bsp_get_uart_data_struct_addr(USCI_A1_BASE);

               if(pdata != NULL)
               {
                  (void)memset(pdata, 0, sizeof(UARTDS));
                  
                  pdata->baseAddress     = par->baseAddress;
                  pdata->rx_buf          = bsp_get_uart_rx_buf(USCI_A1_BASE);
                  pdata->rx_buf_size     = UART1_RX_BUF_SIZE;
                  pdata->max_tx_buf_size = UART1_TX_MAX_BUFF_SIZE;
                  pdata->semTxUart       = semTxUart1;
                  pdata->semTxBufUart    = semTxBufUart1;
                 
                  pdata->rx_timeout      = (long)TN_WAIT_INFINITE;
                  pdata->rx_timeout_cnt  = pdata->rx_timeout;  
                 
                  pdata->use_cts         = FALSE;
                  pdata->rx_cnt          = 0;
                  pdata->rx_tail         = 0;
                  pdata->rx_head         = 0;  

                  rc = TERR_NO_ERR;
               }

               //!< Power to RS485 driver

               P2OUT |=  (unsigned char)BIT2;

                 //!< Set I/O pins as "RS485 RX"
              
               P5DIR |= (unsigned char)BIT2;
               P5OUT &= (unsigned char)(~BIT2);  //!< New /RE = 0  RX out - Enable
              
               P5DIR |= (unsigned char)BIT4;
               P5OUT &= (unsigned char)(~BIT4);  //!<  DE = 0  TX out - Disable
              
              //!< set UART_A1 Rx/Tx I/O pins

               P8OUT &= (unsigned char)(~BIT3);          //!< RXD - to input
               P8REN |= (unsigned char)BIT3;          //!< Pullup/pulldown resistor enabled  
               P8OUT |= (unsigned char)BIT3;          //!< 1= pullup resistor
                
               P8OUT |= (unsigned char)BIT2;           //!< TXD - to 1
               P8DIR |= (unsigned char)BIT2;           //!< TXD - Output
               P8SEL |= (unsigned char)(BIT2 + BIT3);    //!< TXD & RXD - Peripheral module function is selected for the pin
                  
                  //UART_op_inc();
               if(rc == TERR_NO_ERR)
               {  
                  (void)bsp_sys_uart_setup(par, (int)mode);
                  (void)bsp_sys_uart_set_int(par);
               }
            }
         }   
      }  
   }
   
   return rc;
}

/**
*
*    @brief  bsp_uart_close
*    
*    @details  Close uart driver
*
*    @param [in]  fd  Driver descriptor
*
*    
*    @return E_NO_ERR if operation complete, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
int bsp_uart_close(UARTDS * fd)
{
   TN_INTSAVE_DATA
     
   int rc = TERR_NO_ERR; 

   tn_disable_interrupt();

   if(fd == NULL)
   {  
      rc = -ENOENT;
   }
   else
   {  
      if(fd->baseAddress == 0UL)      //!< not open
      {
         rc = -ENXIO;
      }
      else
      {  
         (void)bsp_sys_uart_close(fd);

         fd->baseAddress = 0UL; 
         fd->fd_flags = 0;

         //UART_op_dec();
      }   
   }   
   
   tn_enable_interrupt();

   return rc;
}

/**
*
*    @brief   bsp_sys_uart_close
*    
*    @details do real job to close uart driver
*
*    @param [in] par Pointer to  UARTSINFO_ST structure
*
*    @return E_NO_ERR if operation complete, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
static int bsp_sys_uart_close(UARTDS * par)
{
   int rc = TERR_NO_ERR;
   
      //!< Disable RX & TX interrupt

   HWREG8(par->baseAddress + OFS_UCAxIE) &= (unsigned char)(~((unsigned char)UCTXIE | (unsigned char)UCRXIE));

   //!< USART - to reset

   HWREG8(par->baseAddress + OFS_UCAxCTL1) |= (unsigned char)UCSWRST;

   //!< USART - to dormant(sleep)

   HWREG8(par->baseAddress + OFS_UCAxCTL1) |= (unsigned char)UCDORM;

   //!< set I/O pins as regular I/O

   P8SEL &= (unsigned char)(~((unsigned char)BIT2 + (unsigned char)BIT3)); 
   
   par->baseAddress = 0UL; //!< Set port as 'not open' 
         
   return rc;
}


/**
*
*    @brief  bsp_uart_getc
*    
*    @details Receive a single character from uart
*
*    @param [in]  fd   Pointer to device descriptor 
*
*    
*    @return  >= 0 - received character, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
int bsp_uart_getc(UARTDS * fd)
{
   TN_INTSAVE_DATA

   int rc; // = TERR_NO_ERR;

   if(fd == NULL)
   {  
      rc = -ENOENT;
   }
   else
   {  
      if(fd->baseAddress != (uint32_t)USCI_A1_BASE)
      {  
         rc = -ENXIO;
      }
      else
      {  
         tn_disable_interrupt();
      
         if(fd->rx_tail == fd->rx_head) //!< no rx ch
         {
            rc = -EAGAIN;
         }
         else
         {  
            //!< rd data

            rc = (int)fd->rx_buf[fd->rx_tail];

            fd->rx_tail++;
            if(fd->rx_tail >= fd->rx_buf_size)
            { 
               fd->rx_tail = 0;
            }
         } 
         
         tn_enable_interrupt();
      }
   }   
   return rc;
}

/**
*
*    @brief bsp_uart_read
*    
*    @details  Read data from the uart
*
*    @param [in]  fd   - Pointer to device descriptor
*                 buf  - buffer to receive 
*                 len  - length of the 'buf'
*
*    @return  >=0 - number of received bytes, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
int bsp_uart_read(UARTDS * fd, char * buf, int len)
{
   TN_INTSAVE_DATA
   
   int nbytes;
   int rc; //    = -EAGAIN;
   int ind   = 0;
   int fExit = FALSE;
   int fChar;

   if(fd == NULL || buf == NULL || len <= 0)
   {  
      ind = -ENOENT;
   }
   else
   {  
      if(fd->baseAddress != (uint32_t)USCI_A1_BASE)
      { 
         ind = -ENXIO;
      }
      else
      {  
         nbytes = MIN(len, fd->rx_buf_size);
         fd->rx_timeout_cnt = fd->rx_timeout;  

         while(fExit == FALSE)
         {
      //!< ----- rx char ----------------------------

            tn_disable_interrupt();

            if(fd->rx_tail == fd->rx_head)
            {  
               rc = -EAGAIN;
               fChar = FALSE;
            }   
            else
            {
               rc = (int)fd->rx_buf[fd->rx_tail];  //!< rd data
               fChar = TRUE;

               fd->rx_tail++;
               if(fd->rx_tail >= fd->rx_buf_size)
               {  
                  fd->rx_tail = 0;
               }   
               fd->rx_cnt--;
            }

            tn_enable_interrupt();

      //---------------------------------

//            if(rc >= 0) //!< char, not err
            if(fChar == TRUE)
            {
               buf[ind++] = (unsigned char)rc;
               if(ind >= nbytes)
               {  
                  fExit = TRUE;
               }   
            }
            else //!< now -no char
            {
               if(ind > 0) //!< We received some character(s) before
               {  
                  fExit = TRUE;
               }   
               else // no chars in buf
               {  
                  if(fd->rx_timeout == (long)TN_NO_WAIT)
                  {  
                     ind = -EAGAIN;
                     fExit = TRUE;
                  }   
                  else if(fd->baseAddress != (uint32_t)USCI_A0_BASE &&
                            fd->baseAddress != (uint32_t)USCI_A1_BASE &&
                               fd->baseAddress != (uint32_t)USCI_A2_BASE)      //!< not open
                  { 
                     ind = -ENXIO;
                     fExit = TRUE;
                  }
                  else
                  {  
                     (void)tn_task_sleep(1); //!< Sleep (here ~3.91 mS)
                     if(fd->rx_timeout != (long)TN_WAIT_INFINITE)
                     {
                        fd->rx_timeout_cnt--;
                        if(fd->rx_timeout_cnt == 0)
                        {  
                           ind = -ETIMEDOUT;
                           fExit =TRUE;
                        }   
                     }
                  }   
               }   
            }
         } //!< while(!fExit)
      }
   }   
   return ind;
}


/**
*
*    @brief    bsp_uart_write
*    
*    @details Writes bytes to uart
*
*    @param [in]  fd   - Pointer to device descriptor
*                 buf  - buffer with bytes to send
*                 len  - number of bytes to send
*    
*    @return  >=0 - number of sent bytes, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
int bsp_uart_write(UARTDS * fd, char * buf, int len)
{
   TN_INTSAVE_DATA
   int rc; // = TERR_NO_ERR;
 
   if(fd == NULL || buf == NULL || len <= 0)
   {  
      rc = -ENOENT;
   }   
   else
   {  
      if(fd->baseAddress != (uint32_t)USCI_A1_BASE)
      {  
         rc = -ENXIO;
      }
      else
      {  
         fd->tx_buf = (unsigned char *)((void*)buf);
         fd->tx_idx = 0;
         fd->tx_cnt = MIN(len, fd->max_tx_buf_size);

         tn_disable_interrupt();
    
         HWREG8(fd->baseAddress + OFS_UCAxTXBUF) =
                    fd->tx_buf[fd->tx_idx++];             //!< send char out UART transmitter
         HWREG8(fd->baseAddress + OFS_UCAxIE) |=  (unsigned char)UCTXIE;  //!< Enable UART TX interrupts

         tn_enable_interrupt();

         (void)tn_sem_acquire(fd->semTxUart, TN_WAIT_INFINITE); //TOUT_200_MSEC); //!< sleep until TX finished
        
         rc = fd->tx_idx;
      }   
   }
   return rc;
}


/**
*
*    @brief bsp_uart_ioctl
*    
*    @details   All changings will be applied at the next call wr/rd func
*
*    @param [in]  Any
*    
*    @return  E_NO_ERR
*/
//----------------------------------------------------------------------------
int bsp_uart_ioctl(UARTDS * fd, int32_t req, void * arg)
{
   int rc = TERR_NO_ERR;
   
   if(fd == NULL)
   {  
      rc = -ENOENT;
   }
   else
   {  
      if(fd->baseAddress != (uint32_t)USCI_A1_BASE)
      {  
         rc = -ENXIO;
      }
      else
      {  
         switch(req) 
         {  
            case (int32_t)SET_UART_RX_TIMEOUT:

               set_uart_rx_timeout(fd, req, arg);

               break;

            case (int32_t)SET_UART_TX_TIMEOUT:

               set_uart_tx_timeout(fd, req, arg);
       
               break;

            default:   
              
               rc = -ENOENT;
               
               break;
         }  
      }
   }   

   return rc;   
}

/**
*
*    @brief    bsp_sys_uart_setup
*    
*    @details  Sets uart registers for the desired configuration
*
*    @param [in] par                 pointer to the UARTSINFO_ST structure
*                uart_config_num     number of predefined uart configuration
*
*    @return E_NO_ERR if operation complete, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
static int bsp_sys_uart_setup(UARTSINFO * par,  int uart_config_num)
{
   int rc = TERR_NO_ERR ; 
   if(par == NULL)
   {  
      rc = -ENOENT;
   }
   else
   {  
       //!< Disable the USCI Module
      HWREG8(par->baseAddress + OFS_UCAxCTL1) |= (unsigned char)UCSWRST;

       //!< Clock source select
      HWREG8(par->baseAddress + OFS_UCAxCTL1) &= (unsigned char)(~UCSSEL_3);
      HWREG8(par->baseAddress + OFS_UCAxCTL1) |= par->selectClockSource;

       //!< MSB, LSB select
      HWREG8(par->baseAddress + OFS_UCAxCTL0) &= (unsigned char)(~UCMSB);
      HWREG8(par->baseAddress + OFS_UCAxCTL0) |= par->msborLsbFirst;

       //!< UCSPB = 0(1 stop bit) OR 1(2 stop bits)
      HWREG8(par->baseAddress + OFS_UCAxCTL0) &= (unsigned char)(~UCSPB);
      HWREG8(par->baseAddress + OFS_UCAxCTL0) |= par->numberofStopBits;

       //!< Parity
      switch (par->parity)
      {
         case USCI_A_UART_NO_PARITY:
              //!< No Parity
              HWREG8(par->baseAddress + OFS_UCAxCTL0) &= (unsigned char)(~UCPEN);
              break;
         case USCI_A_UART_ODD_PARITY:
              //!< Odd Parity
              HWREG8(par->baseAddress + OFS_UCAxCTL0) |= (unsigned char)UCPEN;
              HWREG8(par->baseAddress + OFS_UCAxCTL0) &= (unsigned char)(~UCPAR);
              break;
         case USCI_A_UART_EVEN_PARITY:
              //!< Even Parity
              HWREG8(par->baseAddress + OFS_UCAxCTL0) |= (unsigned char)UCPEN;
              HWREG8(par->baseAddress + OFS_UCAxCTL0) |= (unsigned char)UCPAR;
              break;
         default:
              break;  
      }

       //!< Modulation/BaudRate Control Registers

      HWREG8(par->baseAddress + OFS_UCAxBR0)  = par->baudRateReg_BR0;
      HWREG8(par->baseAddress + OFS_UCAxBR1)  = par->baudRateReg_BR1;
      HWREG8(par->baseAddress + OFS_UCAxMCTL) = (unsigned char)(par->ModReg_MCTL);


       //!< Asynchronous mode & 8 bit character select & clear mode
      HWREG8(par->baseAddress + OFS_UCAxCTL0) &=  (unsigned char)(~((unsigned char)UCSYNC +
                                                                    (unsigned char)UC7BIT +
                                                                    (unsigned char)UCMODE_3));

       //!< Configure  UART mode.
      HWREG8(par->baseAddress + OFS_UCAxCTL0) |= (unsigned char)USCI_A_UART_MODE;

       //!< Reset UCRXIE, UCBRKIE, UCDORM, UCTXADDR, UCTXBRK
      HWREG8(par->baseAddress + OFS_UCAxCTL1)  &= (unsigned char)(~((unsigned char)UCRXEIE + 
                                                    (unsigned char)UCBRKIE + 
                                                    (unsigned char)UCDORM +
                                                    (unsigned char)UCTXADDR + 
                                                    (unsigned char)UCTXBRK));
      //!< Enable the USCI Module

      HWREG8(par->baseAddress + OFS_UCAxCTL1) &= (unsigned char)(~UCSWRST);
   }
   return rc;
}

/**
*
*    @brief     bsp_sys_uart_set_int
*    
*    @details   Enabled(if desired) UART RX interrupt
*
*    @param [in]  par  Pointer to UARTSINFO_ST structure
*
*    @return E_NO_ERR if operation complete, otherwise - errorcode
*
*/
//----------------------------------------------------------------------------
static int bsp_sys_uart_set_int(UARTSINFO * par)
{
   int rc = TERR_NO_ERR;

   if(par == NULL)
   {  
      rc = -ENOENT;
   }
   else
   {  
      UCA1IE |=  (unsigned char)UCRXIE;
   }
   return rc;
}

//----------------------------------------------------------------------------
static void set_uart_rx_timeout(UARTDS * fd, int32_t req, void * arg)
{
   TN_INTSAVE_DATA
   unsigned long data;

   (void)memcpy((void *)&data, arg, sizeof(long)); 
               
   tn_disable_interrupt();

   fd->rx_timeout = (long)data;

   tn_enable_interrupt();
}

//----------------------------------------------------------------------------
static void set_uart_tx_timeout(UARTDS * fd, int32_t req, void * arg)
{
   TN_INTSAVE_DATA
   unsigned long data;

   (void)memcpy((void *)&data, arg, sizeof(long)); 
               
   tn_disable_interrupt();

   fd->tx_timeout = (long)data;

   tn_enable_interrupt();
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------










