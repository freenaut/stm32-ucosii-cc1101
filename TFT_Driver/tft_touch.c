/* Includes ------------------------------------------------------------------*/
#include "tft.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//´¥Ãþ³õÊ¼»¯
void TFT_Touch_Init(void) 
{ 
	GPIO_InitTypeDef  GPIO_InitStructure; 
	SPI_InitTypeDef   SPI_InitStructure; 
	
	//SPI1 Periph clock enable 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// SPI1 Config  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI1,&SPI_InitStructure); 
	
	// SPI1 enable  
	SPI_Cmd(SPI1,ENABLE);  
}
unsigned char SPI_WriteByte(unsigned char data) 
{ 
// unsigned char Data = 0; 

   //Wait until the transmit buffer is empty 
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
	while(SPI1->SR & SPI_I2S_FLAG_TXE==0);
  // Send the byte  
//  SPI_I2S_SendData(SPI1,data); 
	SPI1->DR = data;

   //Wait until a data is received 
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
//	while(SPI1->SR & SPI_I2S_FLAG_RXNE==0); 
//	while((SPI1->SR &SPI_I2S_FLAG_RXNE)==RESET);
  // Get the received data 
//  Data = SPI_I2S_ReceiveData(SPI1); 

  // Return the shifted data 
//  return Data; 
	return  SPI1->DR;
}  


void SpiDelay(unsigned int DelayCnt)
{
	unsigned int i;
	for(i=0;i<DelayCnt;i++);
}

u16 TPReadX(void)
{ 
	u16 x=0;
	GPIOB->BRR = GPIO_Pin_7;
//	SpiDelay(10);
	SPI_WriteByte(0x90);
//	SpiDelay(10);
	x=SPI_WriteByte(0x00);
	x<<=8;
	x+=SPI_WriteByte(0x00);
//	SpiDelay(10);
	GPIOB->BSRR = GPIO_Pin_7; 
	x = x>>3;
	return (x);
}

u16 TPReadY(void)
{
	u16 y=0;
	GPIOB->BRR = GPIO_Pin_7;
//	SpiDelay(10);
	SPI_WriteByte(0xD0);
//	SpiDelay(10);
	y=SPI_WriteByte(0x00);
	y<<=8;
	y+=SPI_WriteByte(0x00);
//	SpiDelay(10);
	GPIOB->BSRR = GPIO_Pin_7;
	y = y>>3; 
	return (y);
}

