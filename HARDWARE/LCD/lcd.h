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
#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				  //LCD ID
	uint8_t  dir;			  //���������������ƣ�0��������1��������	
	uint16_t	 wramcmd;		//��ʼдgramָ��
	uint16_t  rramcmd;   //��ʼ��gramָ��
	uint16_t  setxcmd;		//����x����ָ��
	uint16_t  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
/////////////////////////////////////�û�������///////////////////////////////////	 
#define USE_HORIZONTAL  	 1//����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
#define USE_VERTICAL       0

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 320
#define LCD_H 480

//TFTLCD������Ҫ���õĺ���		   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define GPIO_TYPE  GPIOC  //GPIO������
#define LED      6        //�����������        PB6
#define LCD_CS   9       //Ƭѡ����            PB9
#define LCD_RS   7       //�Ĵ���/����ѡ������ PB7 
#define LCD_RST  8       //��λ����            PB8


//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define	LCD_LED PBout(LED) //LCD����    		 PB6
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 

//GPIO��λ�����ߣ�
#define	LCD_CS_SET  GPIO_TYPE->BSRR=1<<LCD_CS    //Ƭѡ�˿�  	
#define	LCD_RS_SET	GPIO_TYPE->BSRR=1<<LCD_RS    //����/����  	  
#define	LCD_RST_SET	GPIO_TYPE->BSRR=1<<LCD_RST   //��λ			  

//GPIO��λ�����ͣ�							    
#define	LCD_CS_CLR  GPIO_TYPE->BRR=1<<LCD_CS     //Ƭѡ�˿�  
#define	LCD_RS_CLR	GPIO_TYPE->BRR=1<<LCD_RS     //����/���� 	 
#define	LCD_RST_CLR	GPIO_TYPE->BRR=1<<LCD_RST    //��λ	


#define RES_PORT			GPIOC
#define RES_PIN				GPIO_PIN_8

#define DC_PORT				GPIOC
#define DC_PIN				GPIO_PIN_7

#define CS_PORT				GPIOC
#define CS_PIN				GPIO_PIN_9

#define BLK_PORT			GPIOC
#define BLK_PIN				GPIO_PIN_6


//������ɫ
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void LCD_Init(void);
void LCD_Clear(uint16_t Color);	    
void LCD_direction(uint8_t direction );
#endif  
	 
	 



