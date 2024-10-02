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




























