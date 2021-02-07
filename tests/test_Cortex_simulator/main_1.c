
#include "stm32f4xx.h"

#ifdef __cplusplus
 extern "C" {
#endif 

void SysTick_Handler(void);
void SystemInit(void);

#ifdef __cplusplus
}
#endif

volatile unsigned int g_cnt = 0;

void SysTick_Handler(void)
{

   g_cnt++; 
}

void SystemInit(void)
{

}

int main()
{
   for(;;)
   {
   }
   return 0;
}
