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
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 
#include "spi.h"

	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
uint16_t POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;	 
extern DMA_HandleTypeDef hdma_spi1_tx;
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure = {0};
	
 	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = RES_PIN|CS_PIN|DC_PIN;	 
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 //�������
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//�ٶ�50MHz
 	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOB
 	HAL_GPIO_WritePin(GPIOC, RES_PIN|CS_PIN|DC_PIN, GPIO_PIN_SET);
}


uint8_t SPI_WriteByte(uint8_t Byte)
{
	  uint8_t data;
		HAL_SPI_Transmit(&hspi1,&Byte,1,1);
		return data; 
} 
/*****************************************************************************
 * @name       :void LCD_WR_REG(uint8_t data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(uint8_t data)
{ 
   LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPI_WriteByte(data);
   LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(uint8_t data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(uint8_t data)
{
   LCD_CS_CLR;
	 LCD_RS_SET;
   SPI_WriteByte(data);
   LCD_CS_SET;
}

uint8_t LCD_RD_DATA(void)
{
	 uint8_t data;
//	 LCD_CS_CLR;
//	 LCD_RS_SET;
//	 //SPI_SetSpeed(0);
//	 data = SPI_WriteByte(0xFF);
//	 //SPI_SetSpeed(1);
//	 LCD_CS_SET;
	 return data;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

uint8_t LCD_ReadReg(uint8_t LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);
  return LCD_RD_DATA();
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 


void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(uint16_t Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(uint16_t Data)
{	
   LCD_CS_CLR;
   LCD_RS_SET;  
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(uint16_t x,uint16_t y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}



/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(uint16_t Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			SPI_WriteByte(Color>>8);
			SPI_WriteByte(Color);
		}
	}
	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	LCD_LED=1;  //��������
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	//LCD_GPIOInit();//LCD GPIO��ʼ��			
//  LCD_GPIO_Init();	
//	LCD_RESET(); //LCD ��λ
////*************3.5 ST7796S IPS��ʼ��**********//	
//	LCD_CS_SET;
//	LCD_RS_SET;
//	//
//	LCD_WR_REG(0x11);     

//	delay_ms(120);                //Delay 120ms

//	LCD_WR_REG(0x36);     // Memory Data Access Control MY,MX~~

//	LCD_WR_DATA(0x48);   

//	LCD_WR_REG(0x3A);     

//	LCD_WR_DATA(0x55);   //LCD_WR_DATA(0x66);

//	LCD_WR_REG(0xF0);     // Command Set Control

//	LCD_WR_DATA(0xC3);   

//	LCD_WR_REG(0xF0);     

//	LCD_WR_DATA(0x96);   

//	LCD_WR_REG(0xB4);  

//	LCD_WR_DATA(0x01);   

//	LCD_WR_REG(0xB7);   
// 
//	LCD_WR_DATA(0xC6);   

//	//LCD_WR_REG(0xB9);     
//	//LCD_WR_DATA(0x02);
//	//LCD_WR_DATA(0xE0);

//	LCD_WR_REG(0xC0);   
// 
//	LCD_WR_DATA(0x80);  

//	LCD_WR_DATA(0x45);   

//	LCD_WR_REG(0xC1);  

//	LCD_WR_DATA(0x13);   //18  //00

//	LCD_WR_REG(0xC2);    

//	LCD_WR_DATA(0xA7);   

//	LCD_WR_REG(0xC5);    
//	
//	LCD_WR_DATA(0x0A);   

//	LCD_WR_REG(0xE8);   
// 
//	LCD_WR_DATA(0x40);
//	LCD_WR_DATA(0x8A);
//	LCD_WR_DATA(0x00);
//	LCD_WR_DATA(0x00);
//	LCD_WR_DATA(0x29);
//	LCD_WR_DATA(0x19);
//	LCD_WR_DATA(0xA5);
//	LCD_WR_DATA(0x33);

//	LCD_WR_REG(0xE0);
//	LCD_WR_DATA(0xD0);
//	LCD_WR_DATA(0x08);
//	LCD_WR_DATA(0x0F);
//	LCD_WR_DATA(0x06);
//	LCD_WR_DATA(0x06);
//	LCD_WR_DATA(0x33);
//	LCD_WR_DATA(0x30);
//	LCD_WR_DATA(0x33);
//	LCD_WR_DATA(0x47);
//	LCD_WR_DATA(0x17);
//	LCD_WR_DATA(0x13);
//	LCD_WR_DATA(0x13);
//	LCD_WR_DATA(0x2B);
//	LCD_WR_DATA(0x31);

//	LCD_WR_REG(0xE1);
//	LCD_WR_DATA(0xD0);
//	LCD_WR_DATA(0x0A);
//	LCD_WR_DATA(0x11);
//	LCD_WR_DATA(0x0B);
//	LCD_WR_DATA(0x09);
//	LCD_WR_DATA(0x07);
//	LCD_WR_DATA(0x2F);
//	LCD_WR_DATA(0x33);
//	LCD_WR_DATA(0x47);
//	LCD_WR_DATA(0x38);
//	LCD_WR_DATA(0x15);
//	LCD_WR_DATA(0x16);
//	LCD_WR_DATA(0x2C);
//	LCD_WR_DATA(0x32);


//	LCD_WR_REG(0xF0);     
//	LCD_WR_DATA(0x3C);   

//	LCD_WR_REG(0xF0);     
//	LCD_WR_DATA(0x69);   

//	delay_ms(120);                

//	LCD_WR_REG(0x21);     

//	LCD_WR_REG(0x29); 
	
	
	
	LCD_GPIO_Init();//��ʼ��GPIO
	LCD_RESET(); //LCD ��λ
//*************3.5 ST7796S IPS��ʼ��**********//	
	LCD_CS_SET;
	LCD_RS_SET;
	
	LCD_WR_REG(0x11); 
	delay_ms(120); 
	LCD_WR_REG(0x36); 
	if(USE_HORIZONTAL==0)LCD_WR_DATA(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA(0x70);
	else LCD_WR_DATA(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x33); 

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA(0x35);  

	LCD_WR_REG(0xBB);
	LCD_WR_DATA(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0x4C);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29); 

  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ���� 
	LCD_Clear(WHITE);//��ȫ����ɫ
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	LCD_WR_REG(0x2a);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(0x2b);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);
	LCD_WR_REG(0x2c);//������д

	  //LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   


void LCD_UpdatePart(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t* colorBuffer) {
	
		uint16_t i,j,width,height; 
		width = xEnd-xStart+1;
		height = yEnd-yStart+1;
		uint32_t size = width * height;
    // Step 1: ����LCD����ʾ����Ϊ��Ҫ���µ�����
    LCD_SetWindows(xStart, yStart, xEnd, yEnd);

    // Step 2: д����µ����ݵ�LCD (д����ɫ���ݵ�GRAM)
    LCD_CS_CLR;      // ѡ��LCD��Ƭѡ���ͣ�
    LCD_RS_SET;      // ����Ϊд����ģʽ

    // ���������ڵ�ÿһ�����ز����и���
//    for(uint16_t y = yStart; y <= yEnd; y++) {
//        for(uint16_t x = xStart; x <= xEnd; x++) {
//            // �ӻ����л�ȡ��Ӧλ�õ���ɫֵ
//            uint16_t color = *colorBuffer++;
//            // ������д��16λ��ɫֵ
//            SPI_WriteByte(color >> 8); // ���ֽ�
//            SPI_WriteByte(color);      // ���ֽ�
//        }
//    }
		hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
		//hspi1.Instance->CR1|=SPI_CR1_DFF;
		HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)colorBuffer,size);
		while(__HAL_DMA_GET_COUNTER(&hdma_spi1_tx)!=0);

		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
		//hspi1.Instance->CR1&=~SPI_CR1_DFF;
    LCD_CS_SET;      // ȡ��ѡ��LCD��Ƭѡ���ߣ�
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(uint8_t direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(uint8_t direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
			lcddev.rramcmd=0x2E;
			lcddev.dir = direction%4;
	switch(lcddev.dir){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(1<<6));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
		break;	
		default:break;
	}	
}	 
