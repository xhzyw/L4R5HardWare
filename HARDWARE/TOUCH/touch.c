//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ���������������ҵ��;
//����Ӳ������Ƭ��STM32F103C8T6,F103C8T6���Ŀ�����,��Ƶ72MHZ������8MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//    	MOSI         ��          PA7          //Һ����SPI��������д�ź�
//      MISO         ��          PA6          //Һ����SPI�������ݶ��źţ��������Ҫ�������Բ�����
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//      LCD_BL       ��          PB6          //Һ������������źţ��������Ҫ���ƣ����Բ��ӣ�
//       SCK         ��          PA5          //Һ����SPI����ʱ���ź�
//      LCD_RS       ��          PB7          //Һ��������/��������ź�
//      RESET        ��          PB8         //Һ������λ�����ź�
//      LCD_CS       ��          PB9          //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//     CTP_INT       ��          PA1          //���ݴ����������ж��ź�
//     CTP_SDA       ��          PA9          //���ݴ�����IIC���������ź�
//     CTP_RST       ��          PA10         //���ݴ�����������λ�ź�
//     CTP_SCL       ��          PB5          //���ݴ�����IIC����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "gui.h"	    

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


