/* Includes ------------------------------------------------------------------*/
#include "usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*串口1配置*/
void USART1_Configuration()
{  	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*使能外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	/*把GPIO_InitStruct中的每一个参数按缺省值填入*/
	GPIO_StructInit(&GPIO_InitStructure);
	/*设置PA9为推挽输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	/*设置PA10为浮空输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*把USART_InitStruct的每一个参数按缺省值写入*/
	USART_StructInit(&USART_InitStructure);
	/*设置串口波特率为115200，一帧8位，一个停止位，没有奇偶位，发送、接收使能，硬件流控制失能*/
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_Init(USART1 , &USART_InitStructure);
	/*使能USART1外设*/
	USART_Cmd(USART1 , ENABLE);
	/*使能USART1中断*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
/*******************************
名称：串口发送函数
参数1：USART1或USART2或USART3
参数2：字符串，以\n结尾
输出：无
例：USART_SendString(USART1,"欢迎使用stm32系统！这是串口测试程序\n")
*******************************/
void USART_SendString(USART_TypeDef *  USARTx, char * format)
{
    while(*format)
	{
	   USART_SendData(USARTx,*format);//发送一个字节
	   while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);//检测是否发送成功  
	   format++;
	}
}
/*****************************************************************************/
//	USART1_Configuration();
//	USART_SendString(USART1,"欢迎使用stm32系统！这是串口测试程序\n");
//while (1)
//{
//	Delay(10000);
//	USART_SendData(USART1 , ADC1ConvertedValue>>8);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	USART_SendData(USART1 , ADC1ConvertedValue);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//}
