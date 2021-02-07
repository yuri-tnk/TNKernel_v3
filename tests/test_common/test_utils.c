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


//  Copyright (C) 2006 Bob Jenkins (bob_jenkins@burtleburtle.net)
// 
//  A small noncryptographic PRNG
// 
//  http://burtleburtle.net/bob/rand/smallprng.html
// 
//  You can use this free for any purpose.  It's in the public domain.  
//  It has no warranty.


#include <string.h>
#include "prj_conf.h"
#include "tn_config.h"
#include "tn.h"
#include "tn_utils.h"
#include "test.h"

#define MAX_TEST_BUF_SIZE   32

#define rot(x,k) (((x)<<(k))|((x)>>(32-(k))))

typedef struct ranctx 
{ 
   unsigned long a; 
   unsigned long b; 
   unsigned long c; 
   unsigned long d; 
}ranctx;

ranctx g_ranctx;

#define rand_lock()
#define rand_unlock()


const static unsigned long crc32_ref_table[] =
{
   0x00000000UL,0x77073096UL,0xEE0E612CUL,0x990951BAUL,0x076DC419UL,0x706AF48FUL,
   0xE963A535UL,0x9E6495A3UL,0x0EDB8832UL,0x79DCB8A4UL,0xE0D5E91EUL,0x97D2D988UL,
   0x09B64C2BUL,0x7EB17CBDUL,0xE7B82D07UL,0x90BF1D91UL,0x1DB71064UL,0x6AB020F2UL,
   0xF3B97148UL,0x84BE41DEUL,0x1ADAD47DUL,0x6DDDE4EBUL,0xF4D4B551UL,0x83D385C7UL,
   0x136C9856UL,0x646BA8C0UL,0xFD62F97AUL,0x8A65C9ECUL,0x14015C4FUL,0x63066CD9UL,
   0xFA0F3D63UL,0x8D080DF5UL,0x3B6E20C8UL,0x4C69105EUL,0xD56041E4UL,0xA2677172UL,
   0x3C03E4D1UL,0x4B04D447UL,0xD20D85FDUL,0xA50AB56BUL,0x35B5A8FAUL,0x42B2986CUL,
   0xDBBBC9D6UL,0xACBCF940UL,0x32D86CE3UL,0x45DF5C75UL,0xDCD60DCFUL,0xABD13D59UL,
   0x26D930ACUL,0x51DE003AUL,0xC8D75180UL,0xBFD06116UL,0x21B4F4B5UL,0x56B3C423UL,
   0xCFBA9599UL,0xB8BDA50FUL,0x2802B89EUL,0x5F058808UL,0xC60CD9B2UL,0xB10BE924UL,
   0x2F6F7C87UL,0x58684C11UL,0xC1611DABUL,0xB6662D3DUL,0x76DC4190UL,0x01DB7106UL,
   0x98D220BCUL,0xEFD5102AUL,0x71B18589UL,0x06B6B51FUL,0x9FBFE4A5UL,0xE8B8D433UL,
   0x7807C9A2UL,0x0F00F934UL,0x9609A88EUL,0xE10E9818UL,0x7F6A0DBBUL,0x086D3D2DUL,
   0x91646C97UL,0xE6635C01UL,0x6B6B51F4UL,0x1C6C6162UL,0x856530D8UL,0xF262004EUL,
   0x6C0695EDUL,0x1B01A57BUL,0x8208F4C1UL,0xF50FC457UL,0x65B0D9C6UL,0x12B7E950UL,
   0x8BBEB8EAUL,0xFCB9887CUL,0x62DD1DDFUL,0x15DA2D49UL,0x8CD37CF3UL,0xFBD44C65UL,
   0x4DB26158UL,0x3AB551CEUL,0xA3BC0074UL,0xD4BB30E2UL,0x4ADFA541UL,0x3DD895D7UL,
   0xA4D1C46DUL,0xD3D6F4FBUL,0x4369E96AUL,0x346ED9FCUL,0xAD678846UL,0xDA60B8D0UL,
   0x44042D73UL,0x33031DE5UL,0xAA0A4C5FUL,0xDD0D7CC9UL,0x5005713CUL,0x270241AAUL,
   0xBE0B1010UL,0xC90C2086UL,0x5768B525UL,0x206F85B3UL,0xB966D409UL,0xCE61E49FUL,
   0x5EDEF90EUL,0x29D9C998UL,0xB0D09822UL,0xC7D7A8B4UL,0x59B33D17UL,0x2EB40D81UL,
   0xB7BD5C3BUL,0xC0BA6CADUL,0xEDB88320UL,0x9ABFB3B6UL,0x03B6E20CUL,0x74B1D29AUL,
   0xEAD54739UL,0x9DD277AFUL,0x04DB2615UL,0x73DC1683UL,0xE3630B12UL,0x94643B84UL,
   0x0D6D6A3EUL,0x7A6A5AA8UL,0xE40ECF0BUL,0x9309FF9DUL,0x0A00AE27UL,0x7D079EB1UL,
   0xF00F9344UL,0x8708A3D2UL,0x1E01F268UL,0x6906C2FEUL,0xF762575DUL,0x806567CBUL,
   0x196C3671UL,0x6E6B06E7UL,0xFED41B76UL,0x89D32BE0UL,0x10DA7A5AUL,0x67DD4ACCUL,
   0xF9B9DF6FUL,0x8EBEEFF9UL,0x17B7BE43UL,0x60B08ED5UL,0xD6D6A3E8UL,0xA1D1937EUL,
   0x38D8C2C4UL,0x4FDFF252UL,0xD1BB67F1UL,0xA6BC5767UL,0x3FB506DDUL,0x48B2364BUL,
   0xD80D2BDAUL,0xAF0A1B4CUL,0x36034AF6UL,0x41047A60UL,0xDF60EFC3UL,0xA867DF55UL,
   0x316E8EEFUL,0x4669BE79UL,0xCB61B38CUL,0xBC66831AUL,0x256FD2A0UL,0x5268E236UL,
   0xCC0C7795UL,0xBB0B4703UL,0x220216B9UL,0x5505262FUL,0xC5BA3BBEUL,0xB2BD0B28UL,
   0x2BB45A92UL,0x5CB36A04UL,0xC2D7FFA7UL,0xB5D0CF31UL,0x2CD99E8BUL,0x5BDEAE1DUL,
   0x9B64C2B0UL,0xEC63F226UL,0x756AA39CUL,0x026D930AUL,0x9C0906A9UL,0xEB0E363FUL,
   0x72076785UL,0x05005713UL,0x95BF4A82UL,0xE2B87A14UL,0x7BB12BAEUL,0x0CB61B38UL,
   0x92D28E9BUL,0xE5D5BE0DUL,0x7CDCEFB7UL,0x0BDBDF21UL,0x86D3D2D4UL,0xF1D4E242UL,
   0x68DDB3F8UL,0x1FDA836EUL,0x81BE16CDUL,0xF6B9265BUL,0x6FB077E1UL,0x18B74777UL,
   0x88085AE6UL,0xFF0F6A70UL,0x66063BCAUL,0x11010B5CUL,0x8F659EFFUL,0xF862AE69UL,
   0x616BFFD3UL,0x166CCF45UL,0xA00AE278UL,0xD70DD2EEUL,0x4E048354UL,0x3903B3C2UL,
   0xA7672661UL,0xD06016F7UL,0x4969474DUL,0x3E6E77DBUL,0xAED16A4AUL,0xD9D65ADCUL,
   0x40DF0B66UL,0x37D83BF0UL,0xA9BCAE53UL,0xDEBB9EC5UL,0x47B2CF7FUL,0x30B5FFE9UL,
   0xBDBDF21CUL,0xCABAC28AUL,0x53B39330UL,0x24B4A3A6UL,0xBAD03605UL,0xCDD70693UL,
   0x54DE5729UL,0x23D967BFUL,0xB3667A2EUL,0xC4614AB8UL,0x5D681B02UL,0x2A6F2B94UL,
   0xB40BBE37UL,0xC30C8EA1UL,0x5A05DF1BUL,0x2D02EF8DUL
};

//----------------------------------------------------------------------------
unsigned long update_crc32(unsigned long crc, unsigned char ch)
{
   crc = (crc >> 8) ^ crc32_ref_table[((unsigned char)(crc & 0xFFUL)) ^ ch];
   return crc;
}

//----------------------------------------------------------------------------
void fill_random_buf(unsigned char * buf, unsigned long buf_size)
{
   unsigned long len = 0UL;
   unsigned int tlen;
   unsigned long crc;
   unsigned int i;

   //--- Obtain buf data len

   while(len == 0UL)
   {
     len = sfsi_rand() % buf_size;
   }
   if(len < 9UL)
   {
      len = 9UL;
   }

   tlen = (unsigned int)len;
   len -= 8UL;
   crc = 0xFFFFFFFFUL;

   for(i=8; i < tlen; i++)
   {
      buf[i] =  /* 0x30 +i; */ (unsigned char)(sfsi_rand() % 256UL);
      crc = update_crc32(crc, buf[i]);
   }
 
   crc ^= 0xFFFFFFFFUL;

   (void)memcpy(buf,     (void*)&len, 4);  // crc
   (void)memcpy(buf + 4, (void*)&crc, 4);  // crc
}

//----------------------------------------------------------------------------
int chk_random_buf(unsigned char * buf)
{
   long len;
   unsigned long crc;
   unsigned long calc_crc;
   int i;
   int rc = 0;  // OK

   (void)memcpy((void*)&len, buf,      4);  // crc
   (void)memcpy((void*)&crc, buf + 4,  4);  // crc

   len += 8L;
   if(len < 9L || len > (long)65535)
   {
      rc = -1;
   }
   else
   {
      calc_crc = 0xFFFFFFFFUL;
      for(i=8; i < (int)len; i++)
      {
         calc_crc = update_crc32(calc_crc, buf[i]);
      }
      calc_crc ^= 0xFFFFFFFFUL;

      if(calc_crc != crc)
      { 
         rc = -2; 
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
unsigned long sfsi_rand(void) 
{
   ranctx * x = &g_ranctx;
   unsigned long e;
   unsigned long rc;

   rand_lock();

   e = x->a - rot(x->b, 27);
    
   x->a = x->b ^ rot(x->c, 17);
   x->b = x->c + x->d;
   x->c = x->d + e;
   x->d = e + x->a;

   rc = x->d;

   rand_unlock();
   
   return rc;
}

//----------------------------------------------------------------------------
void sfsi_randomize(unsigned long seed) 
{
   int i;
   ranctx * x = &g_ranctx;
   
   x->a = 0xf1ea5eedUL;
   x->b = x->c = x->d = seed;
   
   for(i=0; i < 20; i++) 
   {
      (void)sfsi_rand();
   }
}

//----------------------------------------------------------------------------
unsigned long async_read_sys_tick(void)
{
   unsigned long sys_tick_0;
   unsigned long sys_tick_1;
   unsigned long sys_tick_2;
   unsigned long res;

   sys_tick_0 = tn_sys_tick_count;
   sys_tick_1 = tn_sys_tick_count;
   sys_tick_2 = tn_sys_tick_count;

   if(sys_tick_0 == sys_tick_1)
   {
      res = sys_tick_0;
   }
   else
   {  
      if(sys_tick_0 == sys_tick_2)
      {
         res = sys_tick_0;
      }
      else //!< any case - includes variant sys_tick_1 == sys_tick_2
      {  
         res = sys_tick_2;    //!< if 0 != 1 != 2, it seems that transition
                              //!< was happened in 1; we take 2
      }   
   }
   return res;
}


//----------------------------------------------------------------------------
void long_job_delay(int duration)
{
   unsigned long prev;
   unsigned long curr;
   int i;

   prev = async_read_sys_tick();

   for(i = 0; i < duration; i++)
   {
      for(;;)
      {
         curr = async_read_sys_tick();
         if(curr != prev)
         {
            break;
         } 
      }
      prev = curr;
   }
}

//----------------------------------------------------------------------------
// returns - if found - index in 'cell_arr', otherwise - '-1'
//----------------------------------------------------------------------------
int find_free_cell(unsigned char ** cell_arr, unsigned long arr_size)
{
   int idx;
   int i;
   long pos;
   int rc = -1;

   pos =  (long)(sfsi_rand() % arr_size);
   idx = (int)pos;
   if(cell_arr[idx] == NULL)
   {
      rc = idx;
   }
   else
   {
    // find((Random num - 1) -> 0)  
      for(i = idx - 1; i >= 0; i--)
      {
         if(cell_arr[i] == NULL)
         {
            rc = i;
            break;    
         }
      }

      if(rc == -1) // Not found
      {
          //  find((Random num + 1) -> MAX) 
         for(i = idx + 1; i < arr_size; i++)
         {
            if(cell_arr[i] == NULL)
            {
               rc = i;
               break;    
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
// returns - if found - index in 'cell_arr', otherwise - '-1'
//----------------------------------------------------------------------------
int find_occupied_cell(unsigned char ** cell_arr, unsigned long arr_size)
{
   int idx;
   int i;
   long pos;
   int rc = -1;

   pos =  (long)(sfsi_rand() % arr_size);
   idx = (int)pos;
   if(cell_arr[idx] != NULL)
   {
      rc = idx;
   }
   else
   {
    // find((Random num - 1) -> 0)  
      for(i = idx - 1; i >= 0; i--)
      {
         if(cell_arr[i] != NULL)
         {
            rc = i;
            break;    
         }
      }

      if(rc == -1) // Not found
      {
          //  find((Random num + 1) -> MAX) 
         for(i = idx + 1; i < arr_size; i++)
         {
            if(cell_arr[i] != NULL)
            {
               rc = i;
               break;    
            }
         }
      }
   }
   return rc;
}

//----------------------------------------------------------------------------
BOOL tick_val_inside(unsigned long val, 
                     unsigned long min_val, 
                     unsigned long max_val)
{
   BOOL rc = FALSE;

   if(val >= min_val &&  val <= max_val)
   {
      rc = TRUE;
   }
   return rc;
} 

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
