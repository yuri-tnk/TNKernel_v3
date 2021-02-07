/*
TNKernel real-time kernel

Copyright © 2006, 2016 Yuri Tiomkin
All rights reserved.

Permission to use, copy, modify, and distribute this software in source
and binary forms and its documentation for any purpose and without fee
is hereby granted, provided that the above copyright notice appear
in all copies and that both that copyright notice and this permission
notice appear in supporting documentation.

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
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

#include <stddef.h>
#include "LPC176x.h"

#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "LPC176x_uart.h"
#include "project_main.h"
#include "shell.h"


/* -- Project specific pinout description -

       LPC1766, SK_MLPC1768 board

Port            Pin desc           Pin N   Name in prj   Using as    I/O dir
------   -----------------------   -----   -----------   --------   ---------

 P0.0    RD1/TXD3/SDA1               46
 P0.1    TD1/RXD3/SCL1               47
 P0.2    TXD0/AD0.7                  98     TXD0           TXD
 P0.3    RXD0/AD0.6                  99     RXD0           RXD
 P0.4    I2SRX_CLK/RD2/CAP2.0        81
 P0.5    I2SRX_WS/TD2/CAP2.1         80
 P0.6    I2SRX_SDA/SSEL1/MAT2.0      79
 P0.7    I2STX_CLK/SCK1/MAT2.1       78
 P0.8    I2STX_WS/MISO1/MAT2.2       77
 P0.9    I2STX_SDA/MOSI1/MAT2.3      76     Test 1(LED)    I/O         Out
 P0.10   TXD2/SDA2/MAT3.0            48
 P0.11   RXD2/SCL2/MAT3.1            49
 P0.15   TXD1/SCK0/SCK               62     SCK            SCK
 P0.16   RXD1/SSEL0/SSEL             63     Auto
 P0.17   CTS1/MISO0/MISO             61     MISO           MISO
 P0.18   DCD1/MOSI0/MOSI             60     MOSI           MOSI
 P0.19   DSR1/SDA1                   59
 P0.20   DTR1/SCL1                   58
 P0.21   RI1/RD1                     57
 P0.22   RTS1/TD1                    56
 P0.23   AD0.0 /I2SRX_CLK/CAP3.0      9
 P0.24   AD0.1 /I2SRX_WS/CAP3.1       8
 P0.25   AD0.2 /I2SRX_SDA/TXD3        7
 P0.26   AD0.3 /AOUT/RXD3             6
 P0.27   SDA0/USB_SDA                25
 P0.28   SCL0/USB_SCL                24
 P0.29   USB_D+                      29
 P0.30   USB_D-                      30

 P1.0    ENET_TXD0                   95
 P1.1    ENET_TXD1                   94
 P1.4    ENET_TX_EN                  93
 P1.8    ENET_CRS                    92
 P1.9    ENET_RXD0                   91
 P1.10   ENET_RXD1                   90
 P1.14   ENET_RX_ER                  89
 P1.15   ENET_REF_CLK                88
 P1.16   ENET_MDC                    87
 P1.17   ENET_MDIO                   86
 P1.18   USB_UP_LED/PWM1.1 /CAP1.0   32
 P1.19   MCOA0/USB_PPWR/CAP1.1       33
 P1.20   MCI0/PWM1.2 /SCK0           34
 P1.21   MCABORT/PWM1.3 /SSEL0       35
 P1.22   MCOB0/USB_PWRD/MAT1.0       36
 P1.23   MCI1/PWM1.4 /MISO0          37
 P1.24   MCI2/PWM1.5 /MOSI0          38
 P1.25   MCOA1/MAT1.1                39     ANT-CTRL       MAT1.1
 P1.26   MCOB1/PWM1.6 /CAP0.0        40   LED
 P1.27   CLKOUT/USB_OVRCR/CAP0.1     43
 P1.28   MCOA2/PCAP1.0 /MAT0.0       44
 P1.29   MCOB2/PCAP1.1 /MAT0.1       45
 P1.30   VBUS/AD0.4                  21
 P1.31   SCK1/AD0.5                  20

 P2.0    PWM1.1 /TXD1                75
 P2.1    PWM1.2 /RXD1                74
 P2.2    PWM1.3 /CTS1/TRACEDATA.3    73
 P2.3    PWM1.4 /DCD1/TRACEDATA.2    70     SP1            I/O        Out
 P2.4    PWM1.5 /DSR1/TRACEDATA.1    69     SP2            I/O        Out
 P2.5    PWM1.6 /DTR1/TRACEDATA.0    68
 P2.6    PCAP1.0 /RI1/TRACECLK       67     CURR-CH        I/O         In
 P2.7    RD2/RTS1                    66     SR-OE          I/O         Out
 P2.8    TD2/TXD2/ENET_MDC           65
 P2.9    USB_CONNECT/RXD2/ENET_MDIO  64
 P2.10   EINT0/NMI                   53     ISPreq         I/O        In
 P2.11   EINT1/I2STX_CLK             52
 P2.12   EINT2/I2STX_WS              51     SSEL           I/O        Out
 P2.13   EINT3/I2STX_SDA             50

 P3.26   STCLK/MAT0.1 /PWM1.3        26     TB             PWM1.3
 P3.25   MAT0.0 /PWM1.2              27

 P4.28   RX_MCLK/MAT2.0 /TXD3        82
 P4.29   TX_MCLK/MAT2.1 /RXD3        85
------------------------------------------------------------------------------*/


//----------------------------------------------------------------------------
void HardwareInit(void)
{

   //--- Sys clock

#define  PLL0CFG_VAL              11  /* 11+1  */
#define  CCLKCFG_VAL               2  /* 2+1  */


    //-- Fosc = 14.7456 MHz, Fcco = 2 * 12 * 14.7456 = 353.8944 MHz
    //-- cclk = 353.8944 / 4 = 88.4736 MHz
    //-- cclk/2 = 44.2368 MHz

     //-- Also was tested with

     //--   Fosc = 14.7456 MHz, Fcco = 2 * 10 * 14.7456 = 294.912 MHz,
     //--   cclk = 294.912 / 3 = 98.304 MHz


     //--   Fosc = 12.0000 MHz, Fcco = 2 * (11 + 1) * 12.0000 = 288.000 MHz,
     //--   cclk = 288.000 / (2 + 1) = 96 MHz

  rSCS = (1U<<5U);                //-- Start main oscillator,  freq < 20MHz
  while((rSCS & (1U<<6U)) == 0U)  //-- Wait for Oscillator to be ready
  {
  }
  rCCLKCFG = CCLKCFG_VAL;       //-- Setup Clock Divider

  rCLKSRCSEL = 1U;               //-- Select Clock Source for PLL0 - Main oscillator

  rPLL0CFG   = PLL0CFG_VAL;

  rPLL0CON   = 0x01;             //--- PLL0 Enable
  rPLL0FEED  = 0xAA;
  rPLL0FEED  = 0x55;
  while( (rPLL0STAT & (1U<<26U)) == 0U) //-- Wait for PLOCK0
  {
  }

  rFLASHCFG  &= 0x00000FFFU;  // dont change bits 11:0
  rFLASHCFG  |= 0x4U << 12U;  //4+1 clk - up to 100 MHz   FLASHCFG_VAL;     //-- Flash speed

  rPLL0CON   = 0x03U;             //--- PLL0 Enable & Connect

  rPLL0FEED  = 0xAAU;
  rPLL0FEED  = 0x55U;

 //--- !!! Peripheral Clock Selection - before PLL operations (see Errata file)

 //--  UART0, SPI, Timer 1, PWM1 - Enabled by default

 // rPCONP     = ();        // Power Control for Peripherals


//-------------- I/O pins ---------------------------

// P0.0    RD1/TXD3/SDA1               46
// P0.1    TD1/RXD3/SCL1               47

#define MASK_P0_0_1  (1U | (1U<<1U))

   rFIO0DIR |= MASK_P0_0_1;   //-- Set as Output
   rFIO0SET |= MASK_P0_0_1;   //-- Set to '1'

// P0.2    TXD0/AD0.7                  98     TXD0           TXD
// P0.3    RXD0/AD0.6                  99     RXD0           RXD

   // InitUART0();

// P0.4    I2SRX_CLK/RD2/CAP2.0        81
// P0.5    I2SRX_WS/TD2/CAP2.1         80
// P0.6    I2SRX_SDA/SSEL1/MAT2.0      79
// P0.7    I2STX_CLK/SCK1/MAT2.1       78
// P0.8    I2STX_WS/MISO1/MAT2.2       77

#define MASK_P0_4_8  ((1U<<4U) | (1U<<5U) | (1U<<6U) | (1U<<7U) | (1U<<8U))

   rFIO0DIR |= MASK_P0_4_8;   //-- Set as Output
   rFIO0SET |= MASK_P0_4_8;   //-- Set to '1'

// P0.9    I2STX_SDA/MOSI1/MAT2.3      76     Test 1(LED)    I/O         Out

   rFIO0DIR |= MASK_LED1;   //-- Pin - Set as Output
   rFIO0CLR |= MASK_LED1;   //-- Set to '0'

// P0.10   TXD2/SDA2/MAT3.0            48
// P0.11   RXD2/SCL2/MAT3.1            49

#define MASK_P0_10_11  ((1U<<10U) | (1U<<11U))

   rFIO0DIR |= MASK_P0_10_11;   //-- Set as Output
   rFIO0SET |= MASK_P0_10_11;   //-- Set to '1'

// P0.15   TXD1/SCK0/SCK               62     SCK            SCK
// P0.16   RXD1/SSEL0/SSEL             63     Auto
// P0.17   CTS1/MISO0/MISO             61     MISO           MISO
// P0.18   DCD1/MOSI0/MOSI             60     MOSI           MOSI

//   InitSPI();

// P0.19   DSR1/SDA1                   59
// P0.20   DTR1/SCL1                   58
// P0.21   RI1/RD1                     57
// P0.22   RTS1/TD1                    56
// P0.23   AD0.0 /I2SRX_CLK/CAP3.0      9
// P0.24   AD0.1 /I2SRX_WS/CAP3.1       8
// P0.25   AD0.2 /I2SRX_SDA/TXD3        7
// P0.26   AD0.3 /AOUT/RXD3             6
// P0.27   SDA0/USB_SDA                25
// P0.28   SCL0/USB_SCL                24

#define MASK_P0_19_28  ((1U<<19U) | (1U<<20U) | (1U<<21U) | (1U<<22U) | (1U<<23U) | (1U<<24U) | \
                        (1U<<25U) | (1U<<26U) | (1U<<27U) | (1U<<28U))

   rFIO0DIR |= MASK_P0_19_28;   //-- Set as Output
   rFIO0SET |= MASK_P0_19_28;   //-- Set to '1'

// P0.29   USB_D+                      29
// P0.30   USB_D-                      30

   //-- Default

// P1.0    ENET_TXD0                   95
// P1.1    ENET_TXD1                   94
// P1.4    ENET_TX_EN                  93
// P1.8    ENET_CRS                    92
// P1.9    ENET_RXD0                   91
// P1.10   ENET_RXD1                   90
// P1.14   ENET_RX_ER                  89
// P1.15   ENET_REF_CLK                88
// P1.16   ENET_MDC                    87
// P1.17   ENET_MDIO                   86
// P1.18   USB_UP_LED/PWM1.1 /CAP1.0   32
// P1.19   MCOA0/USB_PPWR/CAP1.1       33
// P1.20   MCI0/PWM1.2 /SCK0           34
// P1.21   MCABORT/PWM1.3 /SSEL0       35
// P1.22   MCOB0/USB_PWRD/MAT1.0       36
// P1.23   MCI1/PWM1.4 /MISO0          37
// P1.24   MCI2/PWM1.5 /MOSI0          38

#define MASK_P1_0_24  (1U | (1U<<1U) | (1U<<4U) | (1U<<1U) | (1U<<4U) | (1U<<8U) | \
                 (1U<<9U) | (1U<<20U) | (1U<<21U) | (1U<<22U) | (1U<<23U) | (1U<<24U))

   rFIO1DIR |= MASK_P1_0_24;   //-- Set as Output
   rFIO1SET |= MASK_P1_0_24;   //-- Set to '1'

// P1.25   MCOA1/MAT1.1                39     ANT-CTRL       MAT1.1

  // InitTimer1();

// P1.26   MCOB1/PWM1.6 /CAP0.0        40

   rFIO1DIR |= (1U<<26U);   //-- Set as Output
   rFIO1SET |= (1U<<26U);   //-- Set to '1'

// P1.27   CLKOUT/USB_OVRCR/CAP0.1     43

       //-- Clock Output Configuration - CPU CLP div by 2 and enabled

   rPINSEL3    |= (1U<<22U);                   //-- Set pin P1.27 as clock output
   rCLKOUTCFG  |= (1U<<8U) | (1U<<4U);

// P1.28   MCOA2/PCAP1.0 /MAT0.0       44
// P1.29   MCOB2/PCAP1.1 /MAT0.1       45
// P1.30   VBUS/AD0.4                  21
// P1.31   SCK1/AD0.5                  20

#define MASK_P1_28_31  ((1U<<28U) | (1U<<29U) | (1U<<30U) | (1U<<31U))

   rFIO1DIR |= MASK_P1_28_31;   //-- Set as Output
   rFIO1SET |= MASK_P1_28_31;   //-- Set to '1'

// P2.0    PWM1.1 /TXD1                75
// P2.1    PWM1.2 /RXD1                74
// P2.2    PWM1.3 /CTS1/TRACEDATA.3    73

#define MASK_P2_0_2  ( 1U |(1U<<1U) | (1U<<2U))

   rFIO2DIR |= MASK_P2_0_2;   //-- Set as Output
   rFIO2SET |= MASK_P2_0_2;   //-- Set to '1'

// P2.3    PWM1.4 /DCD1/TRACEDATA.2    70     SP1            I/O        Out

   rFIO2DIR |= MASK_SP1;   //-- Set as Output
   rFIO2SET |= MASK_SP1;   //-- Set to '1'

// P2.4    PWM1.5 /DSR1/TRACEDATA.1    69     SP2            I/O        Out

   rFIO2DIR |= MASK_SP2;   //-- Set as Output
   rFIO2SET |= MASK_SP2;   //-- Set to '1'

// P2.5    PWM1.6 /DTR1/TRACEDATA.0    68

   rFIO2DIR |= (1U<<5U);   //-- Set as Output
   rFIO2SET |= (1U<<5U);   //-- Set to '1'

// P2.6    PCAP1.0 /RI1/TRACECLK       67         CURR-CH        I/O         In

     //-- Input

// P2.7    RD2/RTS1                    66         SR-OE

   rFIO2DIR |= MASK_SR_OE;   //-- Set as Output
   rFIO2SET |= MASK_SR_OE;   //-- Set to '1'

// P2.8    TD2/TXD2/ENET_MDC           65
// P2.9    USB_CONNECT/RXD2/ENET_MDIO  64

#define MASK_P2_8_9  ((1U<<8U) | (1U<<9U))

   rFIO2DIR |= MASK_P2_8_9;   //-- Set as Output
   rFIO2SET |= MASK_P2_8_9;   //-- Set to '1'

// P2.10   EINT0/NMI                   53     ISPreq         I/O        In

     //-- Input

// P2.11   EINT1/I2STX_CLK             52

   rFIO2DIR |= (1U<<11U);   //-- Set as Output
   rFIO2SET |= (1U<<11U);   //-- Set to '1'

// P2.12   EINT2/I2STX_WS              51     SSEL           I/O        Out

   rFIO2DIR |= MASK_SSEL;   //-- Set as Output
   rFIO2SET |= MASK_SSEL;   //-- Set to '1'

// P2.13   EINT3/I2STX_SDA             50

   rFIO2DIR |= (1U<<13U);   //-- Set as Output
   rFIO2SET |= (1U<<13U);   //-- Set to '1'

// P3.26   STCLK/MAT0.1 /PWM1.3        26     TB             PWM1.3

  // InitPWM();

// P3.25   MAT0.0 /PWM1.2              27

   rFIO3DIR |= (1U<<25U);   //-- Set as Output
   rFIO3SET |= (1U<<25U);   //-- Set to '1'

// P4.28   RX_MCLK/MAT2.0 /TXD3        82
// P4.29   TX_MCLK/MAT2.1 /RXD3        85

#define MASK_P4_28_29  ((1U<<28U) | (1U<<29U))

   rFIO4DIR |= MASK_P4_28_29;   //-- Set as Output
   rFIO4SET |= MASK_P4_28_29;   //-- Set to '1'

//------ OS tick timer - SysTick = 1 ms -----

   (void)SysTick_Config(96000U); //88000);//88473);  //-- cpu clk/1000
}

#ifndef TN_TEST

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

#if defined (__ICCARM__) 
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

#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


