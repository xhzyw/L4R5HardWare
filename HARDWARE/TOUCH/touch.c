//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=========================================触摸屏触接线=========================================//
//触摸功能默认数据总线类型为硬件IIC总线
//	   LCD模块                STM32单片机 
//     CTP_INT       接          PA10          //电容触摸屏触摸中断信号
//     CTP_SDA       接          PG13          //电容触摸屏IIC总线数据信号
//     CTP_RST       接          PA8         //电容触摸屏触摸复位信号
//     CTP_SCL       接          PG14          //电容触摸屏IIC总线时钟信号
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
//默认为touchtype=0的数据.
 
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
	tp_dev.scan=FT6336_Scan;	//扫描函数指向GT911触摸屏扫描
	return 0;
}


