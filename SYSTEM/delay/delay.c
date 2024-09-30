#include "delay.h"
#include "tim.h"
//微秒延时
void delay_us(int n)//100us
{
    //设置计数初值为0 ---从0开始数数
	  __HAL_TIM_SET_COUNTER(&htim2, 0);
    //启动定时器2开始计数
	  __HAL_TIM_ENABLE(&htim2) ;
	  //获取当前正在数的数跟想要延时的时间数进行比较----等待延时时间到达
	  while(__HAL_TIM_GET_COUNTER(&htim2) < n)
		{
		    
		}
		//关闭定时器
		 __HAL_TIM_DISABLE(&htim2) ;
}   
//豪秒延时
void delay_ms(int n)//
{
	   while(n--)
		 {
		     delay_us(1000);
		 }
   
}   




































