//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=========================================������������=========================================//
//��������Ĭ��������������ΪӲ��IIC����
//	   LCDģ��                STM32��Ƭ�� 
//     CTP_INT       ��          PA10          //���ݴ����������ж��ź�
//     CTP_SDA       ��          PG13          //���ݴ�����IIC���������ź�
//     CTP_RST       ��          PA8         //���ݴ�����������λ�ź�
//     CTP_SCL       ��          PG14          //���ݴ�����IIC����ʱ���ź�
#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"    

_m_tp_dev tp_dev=
{
	TP_Init,
	NULL,
	0,
	0,
 	0,	
};					
//Ĭ��Ϊtouchtype=0������.
 
/*****************************************************************************
 * @name       :uint8_t TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
uint8_t TP_Init(void)
{			    		   
	if(FT6336_Init())
	{
		return 1;
	}
	tp_dev.scan=FT6336_Scan;	//ɨ�躯��ָ��GT911������ɨ��
	return 0;
}


