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

#include "io430.h"
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "pmm.h"

/*--- 

   !!! --- Board - 05_035_ELl-02.pdf  (CPU - MSP430F5359) -------- !!!
*/                

void set_sys_clock(void);
void set_XT2_osc(void);
void set_32khz_osc(void);
uint16_t SetVCore (uint8_t level);
void bsp_init_CPU(void);
void stop_XT2_osc(void);
void do_rtc_init(void);

  //!< Local prototypes

void init_cpu_pins_after_reset(void);
void bsp_set_storage_mode_pins_test(void);
void bsp_set_deep_sleep_pins(void);

/**
*
*
*/
void bsp_init_CPU(void)
{
   //-- First, make sure the watchdog is disabled.

   //WDTCTL = WDTPW + WDTHOLD;           // Stop WDT

   (void)PMM_setVCore(PMM_BASE,      // Set VCore = 1.9V for 20MHz clock
                      PMMCOREV_3);

   init_cpu_pins_after_reset();
   
   P2OUT |= (unsigned char)BIT7;   //!< Board specific

//---------------------------------------
   
   set_32khz_osc();
   set_XT2_osc();  

   do_rtc_init();

   P7OUT |= (unsigned char)BIT5;   //!< Board specific
}

//----------------------------------------------------------------------------
void set_32khz_osc(void)
{
   //!< Enable XT1
  
   while((BAKCTL & (unsigned short)LOCKIO) == (unsigned short)LOCKIO)                   //!< Unlock XT1 pins for operation
   {
      BAKCTL &= (unsigned int)(~((unsigned short)LOCKIO));
   }

   UCSCTL6 &= ~((unsigned short)XT1OFF);                     //!< XT1 On
   UCSCTL6 |= XCAP_3;                      //!< Internal load cap
  
   do
   {
      UCSCTL7 &= (unsigned int)(~((unsigned short)XT2OFFG + 
                                  (unsigned short)XT1LFOFFG /*+ XT1HFOFFG*/ + 
                                  (unsigned short)DCOFFG));
                                             //!< Clear XT2,XT1,DCO fault flags
      SFRIFG1 &= (unsigned int)(~OFIFG);     //!< Clear fault flags
   }while ((SFRIFG1 & (unsigned int)OFIFG) == (unsigned int)OFIFG);       //!< Test oscillator fault flag

   UCSCTL6 &= ~(XT1DRIVE_3);                 //!< Xtal is now stable, reduce drive
                                             //!< strength
//--- Fault 32 khz osc int flags 

   RTCCTL0 &= ~((unsigned char)RTCOFIFG);  // Clear int flag
   RTCCTL0 |= (unsigned char)RTCOFIE;  // Enable int

   UCSCTL4 &= (unsigned int)(~((unsigned int)SELA0 + 
                               (unsigned int)SELA1 + 
                               (unsigned int)SELA2));      //!< Ensure XT1 is ACLK source
}

//----------------------------------------------------------------------------
void do_rtc_init(void)
{
   unsigned char  val_uch = 1U;
//--------------------------------------------------------

      //---- Disable Alarm --------------------
     
      RTCCTL0 &= (unsigned char)(~((unsigned char)RTCAIE | 
                                   (unsigned char)RTCAIFG)); //-- disable Alarm interrupt
      RTCAMIN  = 0x00;
      RTCAHOUR = 0x00;
      RTCADAY  = 0x00;
      RTCADOW  = 0x00;
      
      //---------------------------------------
      
   // Configure RTC_B - BCD calendar mode
   
      RTCCTL01 |= /*(unsigned short)RTCRDYIE |*/ // enable RTC read ready interrupt
                  (unsigned short)RTCBCD |   // BCD mode
                  (unsigned short)RTCHOLD;   // RTC hold 

      RTCSEC  = val_uch;
      RTCMIN  = val_uch;
      RTCHOUR = val_uch;
      RTCDOW  = val_uch;
      RTCDAY  = val_uch;
      RTCMON  = val_uch;
      RTCYEAR = (unsigned short)2000U;

      RTCCTL01 &= ~((unsigned short)RTCHOLD);  // Start RTC calendar mode
}

//----------------------------------------------------------------------------
//   Now it is set for the crystal 20 MHz
//----------------------------------------------------------------------------
void set_XT2_osc(void)  
{
   P7DIR &= ~((unsigned char)BIT2 + (unsigned char)BIT3);     //!< P7.2,P7.3 -inputs

   P7SEL |= (unsigned char)BIT2 + (unsigned char)BIT3;        //!< Port select XT2
  
   UCSCTL6 &= ~(unsigned short)XT2OFF;          //!< Start XT2
  
   while((SFRIFG1 & (unsigned short)OFIFG) != 0U)
   {                 // Check OFIFG fault flag
      UCSCTL7 &= (unsigned int)(~((unsigned int)DCOFFG + 
                                  (unsigned int)XT1LFOFFG +
                                  (unsigned int)XT1HFOFFG + 
                                  (unsigned int)XT2OFFG)); // Clear OSC flaut Flags
      SFRIFG1 &= ~((unsigned int)OFIFG);                    // Clear OFIFG fault flag
   }

   UCSCTL4 =   (unsigned short)(0x5U << 4) |     //!< SELS Bits 6:4=101 - XT2OSC (Selects the SMCLK source)
               (unsigned short)5;       //!< SELM Bits 2:0=101 - XT2OSC (Selects the MCLK source)
}

//----------------------------------------------------------------------------
void stop_XT2_osc(void)
{
   UCSCTL6 |=  (unsigned short)XT2OFF;                //!< Set XT2 Off
   
   P7SEL   &= ~((unsigned char)BIT2 + (unsigned char)BIT3);         //!< Port select XT2
   P7DIR   |=   (unsigned char)BIT2 + (unsigned char)BIT3;          //!< P7.2, P7.3 -outputs
   P7OUT   &= ~((unsigned char)BIT2 + (unsigned char)BIT3);         //!< P7.2, P7.3 - set to 0
}


//----------------------------------------------------------------------------
void init_cpu_pins_after_reset(void) //!< Board specific
{
/*
*/
   P1DIR  =   (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7;  // Out
   P1OUT &= ~((unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7); // Out - to '0'
/*
*/

   P2DIR  =   (unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7;
   P2OUT &= ~((unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6); //!< Out-> to '0'
   P2OUT |=   (unsigned char)BIT7;  //!<  Switch hall sensor power- on

   P2REN  =   (unsigned char)BIT0 | (unsigned char)BIT1;     //!< Enable pull resistor 
   P2OUT |=  ((unsigned char)BIT0 | (unsigned char)BIT1);    //!< Here OUT=1 -pull-up resistor

/*
*/
   P3DIR =     (unsigned char)BIT1 | 
               (unsigned char)BIT2 | 
               (unsigned char)BIT3 | 
               (unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT6 | 
               (unsigned char)BIT7;
   P3OUT &=  ~((unsigned char)BIT1 | 
               (unsigned char)BIT2 | 
               (unsigned char)BIT3 | 
               (unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT7);   //-- Out-> to '0'
   P3OUT |=    (unsigned char)BIT6;                           //-- Out - to '1'

/*
*/
   P4DIR  =   (unsigned char)BIT0 | 
              (unsigned char)BIT1 | 
              (unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7;
   P4OUT &= ~((unsigned char)BIT0 | 
              (unsigned char)BIT1 | 
              (unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7);  //-- Out-> to '0'
  
/*
*/
   P5DIR =    (unsigned char)BIT0 | 
              (unsigned char)BIT1 | 
              (unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6; // Set as Out
   P5OUT &= ~((unsigned char)BIT0 | 
              (unsigned char)BIT1 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6);  //-- Out-> to '0'
   P5OUT |=   (unsigned char)BIT2;                        //-- Out-> to '1' 

/*
*/
   P6DIR &= ~((unsigned char)BIT0 | 
              (unsigned char)BIT1 | 
              (unsigned char)BIT2 | 
              (unsigned char)BIT3 | 
              (unsigned char)BIT4);   // Dir - In
   P6DIR |=   (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7;    // Dir - Out

   P6OUT &=  ~((unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT6 | 
               (unsigned char)BIT7);  // Out - to 0

/*
*/

   P7DIR =    (unsigned char)BIT4 | 
              (unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7;   // All - OUT
   P7OUT &= ~((unsigned char)BIT5 | 
              (unsigned char)BIT6 | 
              (unsigned char)BIT7);         // Out - to '0'
   P7OUT |=   (unsigned char)BIT4;                        // Out - to '1'
 
/*
*/

   P8SEL &= 0U;            // Set as regular I/O  
   UCB1CTL1 |=  (unsigned char)UCSWRST;  // Reset USCI state machine

   P8DIR  =    (unsigned char)BIT0 | 
               (unsigned char)BIT1 | 
               (unsigned char)BIT2 | 
               (unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT7; // OUT
   P8OUT &=  ~((unsigned char)BIT1 | 
               (unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT7);              // Out-> to '0'
   P8OUT |=    (unsigned char)BIT0 | (unsigned char)BIT2;                             // Out-> to '1' 

   P8REN  =   BIT6;       // Enable pull resistor 
   P8OUT &=  ~(unsigned char)BIT6;       // Here OUT=0 -pull-down resistor
   
/*
*/

   P9DIR  =    (unsigned char)BIT0 | 
               (unsigned char)BIT1 | 
               (unsigned char)BIT2 | 
               (unsigned char)BIT4 | 
               (unsigned char)BIT5 | 
               (unsigned char)BIT6 | 
               (unsigned char)BIT7;  // OUT
   P9OUT  = (unsigned char)(~((unsigned char)BIT0 | 
                              (unsigned char)BIT1 | 
                              (unsigned char)BIT2 | /* BIT4 |*/
                              (unsigned char)BIT5 | 
                              (unsigned char)BIT6));        // Out - to '0' 
   //P9OUT  |=   BIT4 | BIT7;                                            // Out - to '1' 

   P9REN  =   (unsigned char)BIT3;       // Enable pull resistor 
   P9OUT  &= ~(unsigned char)BIT3;       // Here OUT=0 -pull-down resistor

/*
PU.0    77    N/C             Not connected                           OUT           '0' 
PU.1    79    N/C             Not connected                           OUT           '0' 
*/  

  //!< Do nothing here case after reset port U is disabled for I/O 
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
