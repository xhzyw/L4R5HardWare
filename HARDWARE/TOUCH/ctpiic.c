//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103C8T6,F103C8T6核心开发板,主频72MHZ，晶振8MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    	MOSI         接          PA7          //液晶屏SPI总线数据写信号
//      MISO         接          PA6          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//      LCD_BL       接          PB6          //液晶屏背光控制信号（如果不需要控制，可以不接）
//       SCK         接          PA5          //液晶屏SPI总线时钟信号
//      LCD_RS       接          PB7          //液晶屏数据/命令控制信号
//      RESET        接          PB8         //液晶屏复位控制信号
//      LCD_CS       接          PB9          //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//     CTP_INT       接          PA1          //电容触摸屏触摸中断信号
//     CTP_SDA       接          PA9          //电容触摸屏IIC总线数据信号
//     CTP_RST       接          PA10         //电容触摸屏触摸复位信号
//     CTP_SCL       接          PB5          //电容触摸屏IIC总线时钟信号
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
#include "ctpiic.h"
#include "delay.h"	 

/*****************************************************************************
 * @name       :void CTP_Delay(void)
 * @date       :2020-05-13 
 * @function   :Delay in controlling IIC speed
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_Delay(void)
{
	delay_us(1);
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Init(void)
 * @date       :2020-05-13 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Init(void)
{	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);
	
}

/*****************************************************************************
 * @name       :void CTP_IIC_Start(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Start(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 最大输出速度50MHz
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);	  	  
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	CTP_Delay();
 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,0);//START:when CLK is high,DATA change form high to low 
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);//钳住I2C总线，准备发送或接收数据 
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Stop(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/   
void CTP_IIC_Stop(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 最大输出速度50MHz
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,0);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);//STOP:when CLK is high DATA change form low to high 
}

/*****************************************************************************
 * @name       :uint8_t CTP_IIC_Wait_Ack(void)
 * @date       :2020-05-13 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
uint8_t CTP_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // 输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;      // 无上下拉
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);delay_us(1);	   
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);delay_us(1);	 
	while(CTP_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CTP_IIC_Stop();
			return 1;
		} 
	}
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);//时钟输出0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Ack(void)
 * @date       :2020-05-13 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void CTP_IIC_Ack(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 最大输出速度50MHz
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,0);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
}

/*****************************************************************************
 * @name       :void CTP_IIC_NAck(void)
 * @date       :2020-05-13 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	    
void CTP_IIC_NAck(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 最大输出速度50MHz
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	CTP_Delay();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Send_Byte(uint8_t txd)
 * @date       :2020-05-13 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/					 				     		  
void CTP_IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 最大输出速度50MHz
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    CTP_IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	      
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
		CTP_Delay();
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);	
		CTP_Delay();
  }	 
} 	

/*****************************************************************************
 * @name       :uint8_t CTP_IIC_Read_Byte(unsigned char ack)
 * @date       :2020-05-13 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/	    
uint8_t CTP_IIC_Read_Byte(unsigned char ack)
{
	uint8_t i,receive=0;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // 输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;      // 无上下拉
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0); 	    	   
		delay_us(3);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);  
		receive<<=1;
		if(CTP_READ_SDA)receive++;   
	}	  				 
	if (!ack)CTP_IIC_NAck();//发送nACK
	else CTP_IIC_Ack(); //发送ACK   
 	return receive;
}




























