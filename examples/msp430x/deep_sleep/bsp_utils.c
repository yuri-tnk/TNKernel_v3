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
#include <string.h>
#include <stdint.h>
#include "tn_config.h"
#include "tn.h"
#include "prj_conf.h"
#include "types.h"
#include "uart.h"


extern BTNINFO_ST * p_g_btn_info_arr;   //!< pointer to the global button data structure
extern TN_MAILBOX * taskAMailbox;
extern TN_MAILBOX * taskBMailbox;

static void bsp_btn_init(BTNINFO_ST * bi,
                         unsigned short port_addr,
                         unsigned int pin,
                         int num,
                         int on_level,
                         send_btn_state_func send_btn_state);
static BTNINFO_ST * get_btn_arr_ptr(void);
static void btn_proc(BTNINFO_ST * bi);
static void btn_is_released_proc(BTNINFO_ST * bi);
static void btn_is_pressed_proc(BTNINFO_ST * bi);
static int send_btn_state(int btn, int state);


//----------------------------------------------------------------------------
static int send_btn_state(int btn, int state)
{
   TN_MAILBOX * dst = NULL;
   int rc = TERR_ILUSE;
   SYS_MSG msg;
   if(btn == BTN_ID_1)
   {
      dst = taskAMailbox;
      if(state == BTN_STATE_BTN_PRESSED)
      {
         msg.data = (void*)((long)MSG_BTN0_PRESSED);
      }
      else if(state == BTN_STATE_BTN_RELEASED)
      {
         msg.data = (void*)((long)MSG_BTN0_RELEASED);
      }
      else
      {
         dst = NULL;
      }
   }
   else if(btn == BTN_ID_2)
   {
      dst = taskBMailbox;
      if(state == BTN_STATE_BTN_PRESSED)
      {
         msg.data = (void*)((long)MSG_BTN1_PRESSED);
      }
      else if(state == BTN_STATE_BTN_RELEASED)
      {
         msg.data = (void*)((long)MSG_BTN1_RELEASED);
      }
      else
      {
         dst = NULL;
      }
   }

   if(dst != NULL)
   {
      rc = tn_mailbox_send(dst,
                           &msg,
                           sizeof(SYS_MSG),
                           TN_NO_WAIT); 
   }
   
   return rc;
}

//----------------------------------------------------------------------------
static BTNINFO_ST * get_btn_arr_ptr(void)
{
   return p_g_btn_info_arr;
}

//----------------------------------------------------------------------------
void bsp_button_processing(void)
{
   BTNINFO_ST * bi;
   int i;
   
   bi = get_btn_arr_ptr();
      
   for(i = 0; i < NUM_BUTTONS; i++)
   {  
       btn_proc(bi);
       bi++;
   }
}

//----------------------------------------------------------------------------
void bsp_buttons_init(void)
{
   BTNINFO_ST * bi;
   
   bi = get_btn_arr_ptr();
   
   //!< SW2 P2.1 BTN_ID_1
   
   bsp_btn_init(bi,
                BTN_PORT_ADDR,         //!< port_addr
                BIT1,                  //!< pin
                BTN_ID_1,        //!< num
                BTN_IS_PRESSED_AT_0,
                (send_btn_state_func)send_btn_state);
   bi++;      
   
   //!< SW1 P2.0 BTN_ID_2
  
   bsp_btn_init(bi,
                BTN_PORT_ADDR,         //!< port_addr
                BIT0,                  //!< pin
                BTN_ID_2,     //!< num
                BTN_IS_PRESSED_AT_0,
                (send_btn_state_func)send_btn_state);
}

//----------------------------------------------------------------------------
static void btn_is_pressed_proc(BTNINFO_ST * bi)
{
   //!< Button is pressed
   
   if(bi->bounce_cnt > 0)
   {  
      bi->bounce_cnt--;
      if(bi->bounce_cnt == 0)
      {
         if(bi->status != BTN_STATE_BTN_UNKNOWN)
         { 
            bi->send_btn_state_handler(bi->num, BTN_STATE_BTN_PRESSED);
         }   
         bi->status = BTN_STATE_BTN_ON;
      }
   }   
}

//----------------------------------------------------------------------------
static void btn_is_released_proc(BTNINFO_ST * bi)
{
   //!< Button is released
  
   if(bi->bounce_cnt < bi->max_bounce_cnt)
   {
      bi->bounce_cnt++;
      if(bi->bounce_cnt == bi->max_bounce_cnt)
      {
         if(bi->status != BTN_STATE_BTN_UNKNOWN)
         { 
            bi->send_btn_state_handler(bi->num, BTN_STATE_BTN_RELEASED);
         }   
         bi->status = BTN_STATE_BTN_OFF;
      } 
   }
}

//----------------------------------------------------------------------------
static void btn_proc(BTNINFO_ST * bi)
{
   volatile unsigned char port_val;

   port_val = *bi->port;
   if((port_val & bi->pin) == 0) //!< Button is pressed
   {
      if(bi->on_level == BTN_IS_PRESSED_AT_0)
         btn_is_pressed_proc(bi);
      else if(bi->on_level == BTN_IS_PRESSED_AT_1)
         btn_is_released_proc(bi);
   }
   else
   {
      if(bi->on_level == BTN_IS_PRESSED_AT_0)
         btn_is_released_proc(bi);
      else if(bi->on_level == BTN_IS_PRESSED_AT_1)
         btn_is_pressed_proc(bi);
   }
}

//----------------------------------------------------------------------------
static void bsp_btn_init(BTNINFO_ST * bi,
                         unsigned short port_addr,
                         unsigned int pin,
                         int num,
                         int on_level,
                         send_btn_state_func send_btn_state)
{
  
   bi->bounce_cnt     = MAX_BTN_BOUNCE_SUPPR_DLY >> 1;
   bi->max_bounce_cnt = MAX_BTN_BOUNCE_SUPPR_DLY;
   
   bi->port = (unsigned char *) port_addr;
   bi->pin  = pin;
   bi->num  = num;
   bi->on_level = on_level;
     
   bi->status = BTN_STATE_BTN_UNKNOWN;
   bi->send_btn_state_handler = send_btn_state;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

