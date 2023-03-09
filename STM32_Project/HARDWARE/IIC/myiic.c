#include "myiic.h"
#include "delay.h"

void IIC_Start(void)
{
	IIC_SDA(1);
	IIC_SCL(1);
	IIC_SDA(0);
	IIC_SCL(0);
}

void IIC_Stop(void)
{
	IIC_SDA(0);
	IIC_SCL(1);
	IIC_SDA(1);
}

void IIC_SendByte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte&(0x80>>i))
		{
			IIC_SDA(1);
		}
		else
		{
			IIC_SDA(0);
		}
		IIC_SCL(1);
		IIC_SCL(0);
	}
}

u8 IIC_ReceiveByte(void)
{
	u8 i, receivedByte=0;
	IIC_SDA(1);
	
	for(i=0;i<8;i++)
	{
		IIC_SCL(1);
		if(IIC_READ_SDA==1)
		{
			receivedByte|=(0x80>>i);
		}
		IIC_SCL(0);
	}
	return receivedByte;
}

void IIC_SendAck(u8 ack)
{
	IIC_SDA(ack);
	IIC_SCL(1);
	IIC_SCL(0);
}

u8 IIC_ReceiveAck(void)
{
	u8 receivedAck;
	IIC_SDA(1);
	IIC_SCL(1);
	receivedAck = IIC_READ_SDA;
	IIC_SCL(0);
	
	return receivedAck;
}

//初始化IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	IIC_SCL(1);
	IIC_SDA(1);
}



