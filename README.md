# 适用于单色LED屏幕灰度图像显示的抖动算法
* 5级灰度
* STM32F407
* 串口DMA、双缓冲
<div align="center"><img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/BadApple_raw.jpg" width=400>                <img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/BadApple_1bit.jpg" width=250></div>
<div align="center"><img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/Cxk_raw.jpg" width=450>     <img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/Cxk_1bit.jpg" width=250></div>
<div align="center"><img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/G.E.M_raw.jpg" width=450>     <img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/G.E.M_1bit.jpg" width=250></div>

## 抖动算法原理
[CSDN](https://blog.csdn.net/qq_42676511/article/details/120626723)图案法是指灰度可以用一定比例的黑白点组成的区域表示，从而达到整体图像的灰度感。简单来说，就是使用黑白点组成图案来表示像素的灰度。本文基于[另一位up主的视频](https://www.bilibili.com/video/BV1vg411n7LD/?spm_id_from=333.1007.top_right_bar_window_history.content.click)实现了5级灰度图像的绘制。
<div align="center"><img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/DitheringAlgotithm.jpg" width=1000></div>

## 串口DMA双缓冲配置
```c
void uart_init(u32 baudrate)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 and GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//WordLength: 8bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//StopBit number: 1
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_Init(USART1, &USART_InitStructure);
	
	USART1_DMA_Init((u32)USART1_RX_BUFF0, (u32)USART1_RX_BUFF1);
  
  	USART_Cmd(USART1, ENABLE);
}

void USART1_DMA_Init(u32 DMA_Memory0BaseAddr, u32 DMA_Memory1BaseAddr)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	DMA_DeInit(DMA2_Stream2);
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Peripheral pointer no increase
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//Peripheral size: Byte
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	
	DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//Memory pointer increase
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;	//Memory size: Byte
	
	DMA_InitStructure.DMA_BufferSize = 1024;	//counter
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//Circular Mode
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFO enable  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//Full FIFO 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC16;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
		
	//Double Buffer Mode
	DMA_MemoryTargetConfig(DMA2_Stream2, DMA_Memory1BaseAddr, DMA_Memory_1);
	DMA_DoubleBufferModeCmd(DMA2_Stream2,ENABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA2_Stream2, ENABLE);

}
```
## PC图像预处理
#### 需要安装配置的库
1. opencv
2. Windows Screenshot [原作者](https://www.cnblogs.com/zhiyiYo/p/14800601.html)
3. c++串口通信 [原作者Repository： LairdXavier/MyTool](https://github.com/LairdXavier/MyTool/tree/master/serial_win_cplus)
#### 实现功能
1. 截取视频图像并进行图像处理
2. 截取屏幕图像并进行图像处理
3. 获取摄像头图像并进行图像处理
4. 将图像流保存至`ditheringVideo.bin`，该文件可以使用串口助手直接发送至单片机
<div align="center"><img src="https://github.com/Potatotatotato/1bit-OLED-DitheringAlgorithm/blob/main/Images/SerialSoftware.jpg" width=600></div>


