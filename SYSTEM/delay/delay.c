#include "delay.h"
#include "tim.h"
//΢����ʱ
void delay_us(int n)//100us
{
    //���ü�����ֵΪ0 ---��0��ʼ����
	  __HAL_TIM_SET_COUNTER(&htim2, 0);
    //������ʱ��2��ʼ����
	  __HAL_TIM_ENABLE(&htim2) ;
	  //��ȡ��ǰ��������������Ҫ��ʱ��ʱ�������бȽ�----�ȴ���ʱʱ�䵽��
	  while(__HAL_TIM_GET_COUNTER(&htim2) < n)
		{
		    
		}
		//�رն�ʱ��
		 __HAL_TIM_DISABLE(&htim2) ;
}   
//������ʱ
void delay_ms(int n)//
{
	   while(n--)
		 {
		     delay_us(1000);
		 }
   
}   




































