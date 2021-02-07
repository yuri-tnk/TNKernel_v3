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


#ifndef TH_CONFIG_H
#define TH_CONFIG_H


#define  TN_CORTEX_M7     1
#define  TN_SUPPORT_FPU   1

   //--- The system configuration (change it for your particular project)


#define  TN_STACK_CHECK              1
#define  TN_CHECK_PARAM              1
#define  TN_RESET_IDLE_STACK         1

//#define  TN_MEAS_PERFORMANCE  1

#define  USE_MUTEXES                 1

#define  USE_EVENTFLAGS              1

//#define  USE_DYN_OBJ                 1


//#define  TN_TEST                     1 


#define  USER_TIMERS                 1
    //--- User Timers

#if defined USER_TIMERS

#define  TN_USER_TICK_LISTS_CNT        8
#define  TN_USER_TICK_LISTS_MASK       (TN_USER_TICK_LISTS_CNT - 1)

#endif

#define  TN_IDLE_STACK_SIZE   96  // CPU & project depended

    //--- OS Timers

#define  TN_OS_TICK_LISTS_CNT        8
#define  TN_OS_TICK_LISTS_MASK       (TN_OS_TICK_LISTS_CNT - 1)

//-- Do not remove or change this line

#define  TN_PRJ_CONFIG               1

#endif
