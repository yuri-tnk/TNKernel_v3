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
//#include <stdarg.h>

#include "tn_config.h"
#include "tn.h"
#include "pmm.h"

/*--- 

   !!! --- Board - Protector 05_035_ELl-02.pdf  (CPU - MSP430F5359) -------- !!!

 
Port    Pin     Net            Net description                         Dir     State after Reset Init
-----  -----  -----------    ----------------------------------     -------   -----------------------
---01--P6.4     1    FREE_1          Not used                                OUT           '0'
P6.4     1    M_AN_PLUNG      ADC input for plunger status recogn      IN          no pull resistor
---01--P6.5     2    FREE_2          Not used                                OUT           '0'
P6.5     2    M_CHARG_SLOW    Charge via 100 Ohm (active high)        OUT           '0'
P6.6     3    M_WD_DONE       Ext WDT reset                           OUT           '0'
P6.7     4    M_18V_EN        18V DC/DC enable  1 - en                OUT           '0'
P7.4     5    M_LOC_DSBL      Meas Solenoid-Kernel In/Out 0-meas      OUT           '1'
---01--P7.5     6    M_CHARG_EN      Charge Capacitor Enable 1-en            OUT           '0'
P7.5     6    M_CHARG_FAST    Charge via 50 Ohm (active high)         OUT           '0'
P7.6     7    M_CHARG_UP      Charge Up  1-active                     OUT           '0' 
P7.7     8    M_SU_EN2        Charge DC/DC  1-en                      OUT           '0' 
P5.0     9    M_CHARG_MESUR   Enable Meas Capacitor Voltage 1-en      OUT           '0' 
P5.1    10    M_SU_EN1        Enable Solenoid DC/DC(reserved)  1-en   OUT           '0' 
---01--P5.6    16    N/C             Not connected                           OUT           '0' 
P5.6    16    M_PLUNG_POW     Vbat to drv for p stat recog(1-active)  OUT           '0' 
P2.0    17    M_SW_0          Button SW0 input                         IN            pull-up
P2.1    18    M_SW_1          Button SW2 input                         IN            pull-up
---01--P2.2    19    N/C             Not connected                           OUT           '0' 
P2.2    19    M_RS485_EN      RS485 drv IC power ON (active high)     OUT           '0' 
P2.3    20    M_DRV_IN_1      Solenoid drv IC control IN1             OUT           '0' 
P2.4    21    M_DRV_IN_2      Solenoid drv IC control IN2             OUT           '0' 
P2.5    22    M_DRV_EN        Solenoid drv IC enable pin              OUT           '0' 
P2.6    23    M_LED_BL        Backlight LEDs control  1-on            OUT           '0' 
P2.7    24    M_HALLSW_PWR    Switch hall sensor power                OUT           '1'
P5.2    28    M_REn           RE ena for RS485 drv IC  0-en 1-dis     OUT           '1'
P5.3    31    FREE_3          Not used                                OUT           '0'  
P5.4    32    M_DE            DE ena for RS485 drv IC  0-Tx to Z-st   OUT           '0'
P5.5    33    M_BAT_MESUR     Enable Battery Voltage meas 1-en        OUT           '0'
P1.0    34    M_HALL_SIG_0    Water stream Hall sensor 1 output        IN        No pull-up resistor
P1.1    35    M_HALL_SIG_1    Water stream Hall sensor 2 output        IN        No pull-up resistor 
P1.2    36    M_HALLSW_SIG    Switch hall sensor input                 IN        No pull-up resistor 
P1.3    37    M_TA0CC2_OUT    Timer A0 CC2 out-connect to Tmr 1       OUT           '0'
P1.4    38    M_BLE_IO2       Bluetooth BLE interface                 OUT           '0'
P1.5    39    M_BLE_IO3       Bluetooth BLE interface                 OUT           '0'
P1.6    40    M_BLE_IO4       Bluetooth BLE interface                 OUT           '0'
P1.7    41    M_BLE_IO7       Bluetooth BLE interface                 OUT           '0'
P3.0    42    M_TA1CLK        Timer A1 input - conn to TA0CC2 out      IN        No pull-up resistor
P3.1    43    M_BLE_IO8       Bluetooth BLE interface                 OUT           '0'
P3.2    44    M_BLE_IO9       Bluetooth BLE interface                 OUT           '0'
P3.3    45    M_BLE_IO10      Bluetooth BLE interface                 OUT           '0'
P3.4    46    M_BLE_IO11      Bluetooth BLE interface                 OUT           '0'
P3.5    47    FREE_4          Not used                                OUT           '0'
P3.6    48    M_HALL_PWM_0    PWM for water Stream Hall sensor 1      OUT           '1' 
---01--P3.7    49    M_HALL_PWM_1    PWM for water Stream Hall sensor 2      OUT           '1'
P3.7    49    M_LED_11        LED11 ctrl 1-on                         OUT           '0'
P4.0    50    M_LED_0         LED0 ctrl 1-on                          OUT           '0'
P4.1    51    M_LED_1         LED1 ctrl 1-on                          OUT           '0'
P4.2    52    M_LED_2         LED2 ctrl 1-on                          OUT           '0'
P4.3    53    M_LED_3         LED3 ctrl 1-on                          OUT           '0'
P4.4    54    M_LED_4         LED4 ctrl 1-on                          OUT           '0'
P4.5    55    M_LED_5         LED5 ctrl 1-on                          OUT           '0'
P4.6    56    M_LED_6         LED6 ctrl 1-on                          OUT           '0'
P4.7    57    M_LED_7         LED7 ctrl 1-on                          OUT           '0'
P8.0    58    M_FLS_VDD_EN    NOR flash power  1-off                  OUT           '1'
---01--P8.1    59    N/C             Not connected                           OUT           '0' 
P8.1    59    M_LED_8         LED8 ctrl 1-on                          OUT           '0'
P8.2    60    M_UART1_TX      RS485 UART TxD                          OUT           '1'
P8.3    61    M_UART1_RX      RS485 UART RxD                          IN            No pull-up resistor
P8.4    62    M_FLS_CLK       Flash SPI CLK                           OUT           '0' 
P8.5    65    M_FLS_SIMO      Flash SPI MOSI                          OUT           '0' 
P8.6    66    M_FLS_SOMI      Flash SPI MISO                          IN            pull-down resistor
P8.7    67    M_FLS_CSn       Flash SPI CS                            OUT           '0'
P9.0    68    M_SENS_EN       Enable 4..20 mA sensors meas (1-en)     OUT           '0'
P9.1    69    M_BLE_RST       Bluetooth BLE reset                     OUT           '0'
P9.2    70    M_UART2_TX      Bluetooth BLE TxD                       OUT           '0'
P9.3    71    M_UART2_RX      Bluetooth BLE RxD                       IN            pull-down resistor
P9.4    72    M_BLE_EN        Bluetooth BLE enable                    OUT           '0'
---01--P9.5    73    N/C             Not connected                           OUT           '0' 
P9.5    73    M_LED_9         LED9 ctrl 1-on                          OUT           '0'
---01--P9.6    74    N/C             Not connected                           OUT           '0' 
P9.6    74    MLED_10         LED10 ctrl 1-on                         OUT           '0'
P9.7    75    M_HALL_PWR      Power for the Water stream Hall sensor  OUT           '1' 
PU.0    77    N/C             Not connected                           OUT           '0' 
PU.1    79    N/C             Not connected                           OUT           '0' 
P5.7    88    M_RTCCLK                                                     Default
P6.0    97    M_AN_DRV_C      ADCA0 - the solenoid current meas       IN             no pull-up resistor     
P6.1    98    SEN_AN          ADCA1 - Sensors 4..20 mA ADC            IN             no pull-up resistor     
P6.2    99    M_AN_CHARG      ADCA2 - Capacitor voltage               IN             no pull-up resistor     
P6.3   100    M_AN_INBAT_V    ADCA3 - the battery voltage meas        IN             no pull-up resistor     
*/                



void set_sys_clock(void);
void set_XT2_osc(void);
void set_32khz_osc(void);
uint16_t SetVCore (uint8_t level);
void bsp_init_CPU(void);
void stop_XT2_osc(void);
void do_rtc_init(void);


  //!< Local prototypes


//extern __no_init TN_DQUE timers_mb_dque;
//extern __no_init TN_FMP timers_mb_dque_MemPool;

//static void init_ws_hall_sensor_hardware(void);
void init_cpu_pins_after_reset(void);

//void InitLPM_DORG_2(void);
void bsp_set_storage_mode_pins_test(void);
void bsp_set_deep_sleep_pins(void);

//#define NO_WS_HALL_SENSORS


//BOOL PMM_setVCore(uint32_t baseAddress, uint8_t level);
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


   
   P2OUT |= (unsigned char)BIT7;   //!< Enable power for HALL switch sensor

//---------------------------------------
   
   set_32khz_osc();
   set_XT2_osc();  

   do_rtc_init();

   P7OUT |= (unsigned char)BIT5;          //!< Open switch to Vbat

   
   //P3DIR |= BIT4;                            // P3.4 pin 46 SMCLK set out to pins
   //P3SEL |= BIT4;
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
   //UCSCTL6 |= XCAP_0;                      // Internal load cap 
   //UCSCTL6 |= XCAP_1;                      // Internal load cap 
   //UCSCTL6 |= XCAP_2;                      // Internal load cap 
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
//   unsigned short val_ush = 1U;
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

//      UCSCTL6 &= ~XT2DRIVE_3;          // Clear XT2drive field
//      UCSCTL6 |= XT2DRIVE_1;           // XT2DRIVE_1  ----------  8  - 16 MHz

     
//         f_DCOCLK = D * (N + 1) * (f_FLLREFCLK / n);
//        
//         where n =  FLLREFDIV Bits 2:0 UCSCTL3
//               D =  FLLD Bits 14:12    UCSCTL2
//               N  = FLLN Bits 9:0      UCSCTL2
     

/*

 Select source for MCLK and SMCLK e.g. SELECT_MCLK_SMCLK(SELM__DCOCLK + SELS__DCOCLK) 

#define st(x)      do { x } while (__LINE__ == -1)
#define SELECT_MCLK_SMCLK(sources) st(UCSCTL4 = (UCSCTL4 & ~(SELM_7 + SELS_7)) | (sources);)

   SELECT_MCLK_SMCLK(SELS__XT2CLK | SELM__XT2CLK);
*/

   UCSCTL4 =   (unsigned short)(0x5U << 4) |     //!< SELS Bits 6:4=101 - XT2OSC (Selects the SMCLK source)
               (unsigned short)5;       //!< SELM Bits 2:0=101 - XT2OSC (Selects the MCLK source)

    // P3DIR |= BIT4;
    // P3SEL |= BIT4;

}

//----------------------------------------------------------------------------
void stop_XT2_osc(void)
{
  // UCSCTL4 = 0; //-- ACLK, SMCLK, MCLK = XT1CLK

   UCSCTL6 |=  (unsigned short)XT2OFF;                //!< Set XT2 Off
   
   P7SEL   &= ~((unsigned char)BIT2 + (unsigned char)BIT3);         //!< Port select XT2
   P7DIR   |=   (unsigned char)BIT2 + (unsigned char)BIT3;          //!< P7.2, P7.3 -outputs
   P7OUT   &= ~((unsigned char)BIT2 + (unsigned char)BIT3);         //!< P7.2, P7.3 - set to 0
}


//----------------------------------------------------------------------------
void init_cpu_pins_after_reset(void)
{
/*
P1.0    34    M_HALL_SIG_0    Water stream Hall sensor 1 output        IN        No pull-up resistor
P1.1    35    M_HALL_SIG_1    Water stream Hall sensor 2 output        IN        No pull-up resistor 
P1.2    36    M_HALLSW_SIG    Switch hall sensor input                 IN        No pull-up resistor 
P1.3    37    M_TA0CC2_OUT    Timer A0 CC2 out-connect to Tmr 1       OUT           '0'
P1.4    38    M_BLE_IO2       Bluetooth BLE interface                 OUT           '0'
P1.5    39    M_BLE_IO3       Bluetooth BLE interface                 OUT           '0'
P1.6    40    M_BLE_IO4       Bluetooth BLE interface                 OUT           '0'
P1.7    41    M_BLE_IO7       Bluetooth BLE interface                 OUT           '0'
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
P2.0    17    M_SW_0          Button SW0 input                         IN            pull-up
P2.1    18    M_SW_1          Button SW2 input                         IN            pull-up
-----P2.2    19    M_RS485_EN      RS485 drv IC power ON (active high)     OUT           '0' 
P2.3    20    M_DRV_IN_1      Solenoid drv IC control IN1             OUT           '0' 
P2.4    21    M_DRV_IN_2      Solenoid drv IC control IN2             OUT           '0' 
P2.5    22    M_DRV_EN        Solenoid drv IC enable pin              OUT           '0' 
P2.6    23    M_LED_BL        Backlight LEDs control  1-on            OUT           '0' 
P2.7    24    M_HALLSW_PWR    Switch hall sensor power                OUT           '1'
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
P3.0    42    M_TA1CLK        Timer A1 input - conn to TA0CC2 out      IN        No pull-up resistor
P3.1    43    M_BLE_IO8       Bluetooth BLE interface                 OUT           '0'
P3.2    44    M_BLE_IO9       Bluetooth BLE interface                 OUT           '0'
P3.3    45    M_BLE_IO10      Bluetooth BLE interface                 OUT           '0'
P3.4    46    M_BLE_IO11      Bluetooth BLE interface                 OUT           '0'
P3.5    47    FREE_4          Not used                                OUT           '0'
P3.6    48    M_HALL_PWM_0    PWM for water Stream Hall sensor 1      OUT           '1' 
-----P3.7    49    M_LED_11        LED11 ctrl 1-on                         OUT           '0'
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
 P4.0    50  M_LED0         LED0 ctrl  1- on 0 - off                                 OUT        '0'
 P4.1    51  M_LED1         LED1 ctrl  1- on 0 - off                                 OUT        '0'
 P4.2    52  M_LED2         LED2 ctrl  1- on 0 - off                                 OUT        '0'
 P4.3    53  M_LED3         LED3 ctrl  1- on 0 - off                                 OUT        '0'
 P4.4    54  M_LED4         LED4 ctrl  1- on 0 - off                                 OUT        '0'
 P4.5    55  M_LED5         LED5 ctrl  1- on 0 - off                                 OUT        '0'
 P4.6    56  M_LED6         LED6 ctrl  1- on 0 - off                                 OUT        '0'
 P4.7    57  M_LED7         LED7 ctrl  1- on 0 - off                                 OUT        '0'
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
P5.0     9    M_CHARG_MESUR   Enable Meas Capacitor Voltage 1-en      OUT           '0' 
P5.1    10    M_SU_EN1        Enable Solenoid DC/DC(reserved)  1-en   OUT           '0' 
P5.2    28    M_REn           RE ena for RS485 drv IC  0-en 1-dis     OUT           '1'
P5.3    31    FREE_3          Not used                                OUT           '0'  
P5.4    32    M_DE            DE ena for RS485 drv IC  0-Tx to Z-st   OUT           '0'
P5.5    33    M_BAT_MESUR     Enable Battery Voltage meas 1-en        OUT           '0'
-----P5.6    16    M_PLUNG_POW     Vbat to drv for p stat recog(1-active)  OUT           '0' 
P5.7    88    M_RTCCLK                                                     Default
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
P6.0    97    M_AN_DRV_C      ADCA0 - the solenoid current meas       IN             no pull-up resistor     
P6.1    98    SEN_AN          ADCA1 - Sensors 4..20 mA ADC            IN             no pull-up resistor     
P6.2    99    M_AN_CHARG      ADCA2 - Capacitor voltage               IN             no pull-up resistor     
P6.3   100    M_AN_INBAT_V    ADCA3 - the battery voltage meas        IN             no pull-up resistor     
----P6.4     1    M_AN_PLUNG      ADC input for plunger status recogn      IN          no pull resistor
----P6.5     2    M_CHARG_SLOW    Charge via 100 Ohm (active high)        OUT           '0'
P6.6     3    M_WD_DONE       Ext WDT reset                           OUT           '0'
P6.7     4    M_18V_EN        18V DC/DC enable  1 - en                OUT           '0'
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
P7.4     5    M_LOC_DSBL      Meas Solenoid-Kernel In/Out 0-meas      OUT           '1'
-----P7.5     6    M_CHARG_FAST    Charge via 50 Ohm (active high)         OUT           '0'
P7.6     7    M_CHARG_UP      Charge Up  1-active                     OUT           '0' 
P7.7     8    M_SU_EN2        Charge DC/DC  1-en                      OUT           '0' 
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
P8.0    58    M_FLS_VDD_EN    NOR flash power  1-off                  OUT           '1'
------P8.1    59    M_LED_8         LED8 ctrl 1-on                          OUT           '0'
P8.2    60    M_UART1_TX      RS485 UART TxD                          OUT           '1'
P8.3    61    M_UART1_RX      RS485 UART RxD                          IN            No pull-up resistor
P8.4    62    M_FLS_CLK       Flash SPI CLK                           OUT           '0' 
P8.5    65    M_FLS_SIMO      Flash SPI MOSI                          OUT           '0' 
P8.6    66    M_FLS_SOMI      Flash SPI MISO                          IN            pull-down resistor
P8.7    67    M_FLS_CSn       Flash SPI CS                            OUT           '0'
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
P9.0    68    M_SENS_EN       Enable 4..20 mA sensors meas (1-en)     OUT           '0'
P9.1    69    M_BLE_RST       Bluetooth BLE reset                     OUT           '0'
P9.2    70    M_UART2_TX      Bluetooth BLE TxD                       OUT           '0'
P9.3    71    M_UART2_RX      Bluetooth BLE RxD                       IN            pull-down resistor
P9.4    72    M_BLE_EN        Bluetooth BLE enable                    OUT           '0'
-----P9.5    73    M_LED_9         LED9 ctrl 1-on                          OUT           '0'
-----P9.6    74    MLED_10         LED10 ctrl 1-on                         OUT           '0'
P9.7    75    M_HALL_PWR      Power for the Water stream Hall sensor  OUT           '1' 
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


  // ND_DRV_STOP();
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
