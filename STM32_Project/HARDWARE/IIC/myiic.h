#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 

//Attention, there must have 1us delay, because the max speed of IIC is 400kHz!
#define IIC_SCL(x)    {PBout(8)=x;delay_us(1);} //********************Please change the port according to your MCU**********************//
#define IIC_SDA(x)    {PBout(9)=x;delay_us(1);} //********************Please change the port according to your MCU**********************//	 
#define IIC_READ_SDA   PBin(9)

void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(u8 byte);
u8 IIC_ReceiveByte(void);
void IIC_SendAck(u8 ack);
u8 IIC_ReceiveAck(void);
void IIC_Init(void);

//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif


