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

#include <string.h>
#include "prj_conf.h"
#include "tn_config.h"

#if defined TN_CORTEX_M0
 //#include "stm32f0xx.h"
 #include "stm32f0xx_conf.h"
#elif defined TN_CORTEX_M0P
 #include "stm32l0xx.h"
#elif defined TN_CORTEX_M4
 #include "stm32f4xx.h"
#elif defined TN_CORTEX_M7
 #include "stm32f7xx.h"
#endif

#include "tn.h"
#include "project_main.h"

//----------------------------------------------------------------------------
//  Converting short/integer to string according to the format options in
//  'fmt' string (snprintf like, but for the single short/integer argument 
//----------------------------------------------------------------------------



#ifndef NULL
#define NULL (void*)0
#endif


#define  NO_OPTION       0x00U   // No options specified.
#define  MINUS_SIGN      0x01U   // Should we print a minus sign?
#define  RIGHT_JUSTIFY   0x02U   // Should field be right justified?
#define  ZERO_PAD        0x04U   // Should field be zero padded?
#define  CAPITAL_HEX     0x08U   // Did we encounter %X?

#define  USE_WIDTH_FROM_ARG         (-2)
#define  USE_PRECISION_FROM_ARG     (-3)

typedef int (*dprintf_func)(void *outParm, int ch);

/**
*  Operating status structure
*
*/
typedef struct _PARAMS
{
   int base;
   int longArg;
   int precision;
   int is_neg;
   int numOutputChars;   //!< number output characters 
   unsigned int options;          //!< options
   int minFieldWidth;    //!< minimal field width 
   int editedStringLen;  //!< edited string length 
   int leadingZeros;     //!< Leading zeroes 
   dprintf_func outFunc;  //!< Pointer to the actual dprintf function
   void * outParm;        //!< Output parameters
}PARAMS;

/**
*  String parameters structure
*
*/
typedef struct _STRPRINTFPR
{
   unsigned char * str;     //!<  pointer to string
   int   maxLen;   //!<  maximum length
}STRPRINTFPR;


static int StrPrintfFunc(void * outParm, int ch);
static int do_conv(PARAMS * par, 
                   unsigned long x);
static int vv_StrXPrintf(PARAMS * par, 
                         const char * fmt,
                         dprintf_func outFunc, 
                         void * outParm);
static void OutputChar(PARAMS * p, int c);

//----------------------------------------------------------------------------

static TN_INLINE int IsOptionSet(PARAMS * p, unsigned int x)     
{
   int rc = 0;
   if((p->options & x) != 0U)
   {
      rc = 1;  
   }
   return rc;
}

static TN_INLINE int IsOptionClear(PARAMS * p, unsigned int x)   
{
   int rc = 0;
   if((p->options & x) == 0U)
   {
      rc = 1;  
   }
   return rc;
}

static TN_INLINE void SetOption(PARAMS * p, unsigned int x)       
{
   p->options = p->options | x;
}

static TN_INLINE void ClearOption(PARAMS * p, unsigned int x)     
{
   p->options = p->options & (~x);
}

//----------------------------------------------------------------------------
static int vv_StrXPrintf(PARAMS * par, 
                         const char * fmt,
                         dprintf_func outFunc, 
                         void * outParm)
{
   char controlChar;
   int rc = -1;

   par->numOutputChars  = 0;
   par->base = 0; // Err
   par->precision = -1;
   par->longArg   = 0;

   par->outFunc   = outFunc;
   par->outParm   = outParm;

   controlChar = (char)*fmt++;

   while(controlChar != '\0')
   {
      if(controlChar == '%')
      {
       //  int base      = 0;

         controlChar = (char)*fmt++;
         par->minFieldWidth = 0;
         par->leadingZeros  = 0;
         par->options       = NO_OPTION;

         SetOption(par, RIGHT_JUSTIFY);

          // Process [flags]

         if(controlChar == '-')
         {
            ClearOption(par, RIGHT_JUSTIFY);
            controlChar = (char)*fmt++;
         }

         if(controlChar == '0')
         {
            SetOption(par, ZERO_PAD);
            controlChar = (char)*fmt++;
         }

         // Process [width]

         if(controlChar == '*')
         {
            par->minFieldWidth = USE_WIDTH_FROM_ARG;
            controlChar = (char)*fmt++;
         }
         else
         {
            while(('0' <= controlChar) && (controlChar <= '9'))
            {
               par->minFieldWidth = par->minFieldWidth * 10 + controlChar - '0';
               controlChar = (char)*fmt++;
            }
         }

          // Process [.precision]

         if(controlChar == '.')
         {
            controlChar = (char)*fmt++; 

            if(controlChar == '*')
            {
               par->precision = USE_PRECISION_FROM_ARG;
               controlChar = (char)*fmt++;
            }
            else
            {
               par->precision = 0;
               while(('0' <= controlChar) && (controlChar <= '9'))
               {
                  par->precision = par->precision * 10 + controlChar - '0';
                  controlChar = (char)*fmt++;
               }
            }
         }

         //  Process [l]

         if(controlChar == 'l')
         {
            par->longArg = 1;
            controlChar = (char)*fmt++;
         }

         //  Process type.

         if(controlChar == 'd')
         {
            par->base = 10;
            rc = 0; // O.K
         }
         else if(controlChar == 'x')
         {
            par->base = 16;
            rc = 0; // O.K
         }
         else if(controlChar == 'X')
         {
            par->base = 16;
            SetOption(par, CAPITAL_HEX );
            rc = 0; // O.K
         }
         else if(controlChar == 'u')
         {
            par->base = 10;
            rc = 0; // O.K
         }
         else if(controlChar == 'o')
         {
            par->base = 8;
            rc = 0; // O.K
         }
         else if(controlChar == 'b')
         {
            par->base = 2;
            rc = 0; // O.K
         }
         else if(controlChar == 'c')
         {
            par->base = -1;
            ClearOption(par, ZERO_PAD);
            rc = 0; // O.K
         }
         else 
         {
            if(controlChar == 's')
            {
               par->base = -2;
               ClearOption(par, ZERO_PAD);
               rc = 0; // O.K
            }
         }
         if(rc == 0) // finished
         {
            break;
         }
      }
      else
      {
          // We're not processing a % output. Just output the character that
          // was encountered.

         OutputChar(par, controlChar);
         controlChar = (char)*fmt++;
      }
   }

   return rc;
}


//-----------------------------------------------------------------------------
static void OutputChar(PARAMS * p, int c)
{
   if(p->numOutputChars >= 0)
   {
      int n = (*p->outFunc)(p->outParm, c);

      if(n >= 0)
      { 
         p->numOutputChars++;
      }
      else
      {
         p->numOutputChars = n;
      }
   }
}

//-----------------------------------------------------------------------------
static void OutputField(PARAMS * p, unsigned char * str )
{
   unsigned char * ptr;

   int padLen = p->minFieldWidth - p->leadingZeros - p->editedStringLen;

 //  padLen = p->minFieldWidth - p->leadingZeros - p->editedStringLen;

   if(IsOptionSet(p,MINUS_SIGN) != 0)
   {
      if(IsOptionSet(p,ZERO_PAD) != 0)
      {
          // Since we're zero padding, output the minus sign now. If we're space
          // padding, we wait until we've output the spaces.
         OutputChar( p, (int)'-');
      }
      // Account for the minus sign now, even if we are going to output it
      // later. Otherwise we'll output too much space padding.

      padLen--;
   }

   if(IsOptionSet(p, RIGHT_JUSTIFY) != 0)
   {
      int par;
      if((p->options & ZERO_PAD) != 0U) 
      {  
         par = (int)'0'; 
      }
      else 
      {
         par = (int)' '; 
      }
       // Right justified: Output the spaces then the field.
      while( --padLen >= 0)
      {
         OutputChar(p, par);
      }
   }
   if(IsOptionSet(p, MINUS_SIGN) != 0 && IsOptionClear(p, ZERO_PAD) != 0)
   {
       // We're not zero padding, which means we haven't output the minus
       // sign yet. Do it now.
      OutputChar( p, (int)'-' );
   }

   while(--p->leadingZeros >= 0)  // Output any leading zeros.
   {
      OutputChar(p, (int)'0');
   }

   ptr = str;
   while(--p->editedStringLen >= 0) // Output the field itself.
   {
      OutputChar(p, (int)(*ptr));
      ptr += 1;
   }
   // Output any trailing space padding. Note that if we output leading
   // padding, then padLen will already have been decremented to zero.

   while(--padLen >= 0)
   {
      OutputChar(p, (int)' ');
   }
}

//-----------------------------------------------------------------------------
static int StrPrintfFunc(void * outParm, int ch)
{
   STRPRINTFPR * strParm;
   int rc = -1;

   strParm = (STRPRINTFPR *)(outParm);
   if(strParm != NULL)
   {
      if(strParm->maxLen > 0)
      {
         *strParm->str++ = (unsigned char)ch;
         *strParm->str   = (unsigned char)'\0';
         strParm->maxLen--;

         rc = 1;
      }
   }
   return rc;
}


//----------------------------------------------------------------------------
//  Converting short/integer to string according to the format options in
//  'fmt' string (snprintf like, but for the single short/integer argument 
//----------------------------------------------------------------------------
int tn_dump_int_ex(unsigned char * buf, // output buffer
                   int buf_len,      // total output buffer length
                   const char * fmt, // format string
                   int val,          // value to dump
                   int width,        // valid only if 'fmp' contains '.*' in the width field
                   int precision)    // valid only if 'fmp' contains '.*' in the precision field
                
{
   int rc;
   PARAMS par;
   STRPRINTFPR    strParm;

   strParm.str    = buf;
   strParm.maxLen = buf_len - 1; // Leave space for temrinating null char

   rc =  vv_StrXPrintf(&par, fmt, StrPrintfFunc, &strParm);
   if(rc ==  0) // O.K
   {
      if(val < 0)
      {
         par.is_neg = -1;
      }
      else
      {
         par.is_neg = 0;
      }
      rc = do_conv(&par, 
                  (unsigned long) val);
   }
   return rc;
}

//----------------------------------------------------------------------------
//  Converting unsigned short/integer to string according to the format options in
//  'fmt' string (snprintf like, but for the single unsigned short/integer argument 
//----------------------------------------------------------------------------
int tn_dump_uint_ex(const char * fmt, // format string
                    unsigned char * buf, // output buffer
                    int buf_len,      // total output buffer length
                    unsigned int val,          // value to dump
                    int width,        // valid only if 'fmp' contains '.*' in the width field
                    int precision)   // valid only if 'fmp' contains '.*' in the precision field
{
   int rc;
   PARAMS par;
   STRPRINTFPR    strParm;

   strParm.str    = buf;
   strParm.maxLen = buf_len - 1; // Leave space for temrinating null char

   rc =  vv_StrXPrintf(&par, fmt, StrPrintfFunc, &strParm);
   if(rc ==  0) // O.K
   {
      par.is_neg = 0;
      rc = do_conv(&par, 
                   (unsigned long) val);
   }
   return rc;
}
//----------------------------------------------------------------------------
//  Converting long to string according to the format options in
//  'fmt' string (snprintf like, but for the single long argument 
//----------------------------------------------------------------------------
int tn_dump_long_ex(const char * fmt, // format string
                    unsigned char * buf, // output buffer
                    int buf_len,      // total output buffer length
                    long val,         // value to dump
                    int width,        // valid only if 'fmp' contains '.*' in the width field
                    int precision)   // valid only if 'fmp' contains '.*' in the precision field
{
   int rc;
   PARAMS par;
   STRPRINTFPR    strParm;

   strParm.str    = buf;
   strParm.maxLen = buf_len - 1; // Leave space for temrinating null char

   rc =  vv_StrXPrintf(&par, fmt, StrPrintfFunc, &strParm);
   if(rc ==  0) // O.K
   {
      if(val < 0L)
      {
         par.is_neg = -1;
      }
      else
      {
         par.is_neg = 0;
      }
      rc = do_conv(&par, 
                   (unsigned long) val);
   }
   return rc;
}
//----------------------------------------------------------------------------
//  Converting unsigned long to string according to the format options in
//  'fmt' string (snprintf like, but for the single unsigned long argument 
//----------------------------------------------------------------------------
int tn_dump_ulong_ex(const char * fmt, // format string
                     unsigned char * buf, // output buffer
                     int buf_len,      // total output buffer length
                     unsigned long val,         // value to dump
                     int width,        // valid only if 'fmp' contains '.*' in the width field
                     int precision)    // valid only if 'fmp' contains '.*' in the precision field
{
   int rc;
   PARAMS par;
   STRPRINTFPR    strParm;

   strParm.str    = buf;
   strParm.maxLen = buf_len - 1; // Leave space for temrinating null char

   rc =  vv_StrXPrintf(&par, fmt, StrPrintfFunc, &strParm);
   if(rc ==  0) // O.K
   {
      par.is_neg = 0;
      rc = do_conv(&par, 
                   (unsigned long) val);
   }
   return rc;
}
//----------------------------------------------------------------------------
//  Converting (unsigned) char to string according to the format options in
//  'fmt' string (snprintf like, but for the single (unsigned) char argument 
//----------------------------------------------------------------------------
int tn_dump_uchar(const char * fmt, // format string
                  unsigned char * buf,  // output buffer
                  int buf_len,       // total output buffer length
                  unsigned char val) // value to dump
{
   int rc;// = -1;
   PARAMS par;
   STRPRINTFPR    strParm;

   strParm.str    = buf;
   strParm.maxLen = buf_len - 1; // Leave space for temrinating null char

   rc =  vv_StrXPrintf(&par, fmt, StrPrintfFunc, &strParm);
   if(rc ==  0) // O.K
   {
      if(par.base == -1)            // conversion type c
      {
         unsigned char ch = val;
         par.editedStringLen = 1;

         OutputField(&par, &ch);
      }
      else
      {
         rc = -2; 
      } 
   }

   return rc;
}


// conversion type d, b, o or x

static int do_conv(PARAMS * par, 
                   unsigned long x)
{
   int rc = -1;
   long tmp_l;
   unsigned char buffer[ 8 * sizeof(unsigned long) + 4 ]; //-- 4 for align,need 1

   // Worst case buffer allocation is required for binary output,
   // which requires one character per bit of a long.

   switch(par->base)
   {
      case 10:  // 'd'
      case 16:  // 'x' or 'X'
      case 8:   //'o'
      case 2:   // 'b'
         rc = 0;
         break;
      default:
         break;
   }
   if(rc == 0) // OK
   {
      par->editedStringLen = 0;

      if(par->is_neg == -1) // the value is signed and less than 0
      { 
         tmp_l = (long)x;
         if(par->base == 10)
         {
            SetOption(par, MINUS_SIGN);
            tmp_l -= tmp_l;
            x = (unsigned long) tmp_l;
         }
      }

      for(;;)
      {
         int c;
         unsigned int idx;

         unsigned long tmp = (unsigned long)par->base;
         tmp = x % tmp;
         c = (int)((long)tmp);
         c += (int)'0';

         if(c > (int)'9')
         {
            if(IsOptionSet(par, CAPITAL_HEX) != 0)
            {
               c += 'A'-'9'-1;
            }
            else
            {
               c += 'a'-'9'-1;
            }
         }
         idx = (unsigned int)sizeof(buffer) - 1UL - (unsigned int)par->editedStringLen;
         par->editedStringLen++;
         buffer[idx] = (unsigned char)c;

         tmp = (unsigned long)par->base;
         if(tmp != 0UL)
         {
            x = x / tmp;
         } 
         if(x == 0UL)
         {
            break;
         } 
      }


      if((par->precision >= 0) && (par->precision > par->editedStringLen))
      {
         par->leadingZeros = par->precision - par->editedStringLen;
      }

      OutputField(par, buffer + sizeof(buffer) - par->editedStringLen);
   }

   return rc;
}
