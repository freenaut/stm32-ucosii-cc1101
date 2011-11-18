/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H


#define GPIO_SEND_CS                  			GPIOB
#define RCC_APB2Periph_GPIO_CS   		RCC_APB2Periph_GPIOB
#define GPIO_SEND_Pin_CS              		GPIO_Pin_12 

#define GPIO_SEND_Pin_SCLK			  	GPIO_Pin_13
#define GPIO_SEND_Pin_SO			  	GPIO_Pin_14
#define GPIO_SEND_Pin_SI			  	GPIO_Pin_15
 #define GPIO_SEND_Pin_GD0			  	GPIO_Pin_6
 #define GPIO_SEND_Pin_GD2			  	GPIO_Pin_7

 #define GPIO_RECEIVE_CS                  		GPIOA
 
#define GPIO_RECEIVE_Pin_CS              		GPIO_Pin_4
#define GPIO_RECEIVE_Pin_SCLK			  	GPIO_Pin_5
#define GPIO_RECEIVE_Pin_SO			  		GPIO_Pin_6
#define GPIO_RECEIVE_Pin_SI			  		GPIO_Pin_7
 #define GPIO_RECEIVE_Pin_GD0			  	GPIO_Pin_4
 #define GPIO_RECEIVE_Pin_GD2			  	GPIO_Pin_5


 #define GPIO_PIN_LED					GPIO_Pin_14

 typedef enum
{
	CC1101_TYPE_SEND,
	CC1101_TYPE_RECEIVE,
}CC1101_TYPE;

/* Exported macro ------------------------------------------------------------*/
/* Select SPI : Chip Select pin low  */
#define SPI_SEND_CS_LOW()       GPIO_ResetBits(GPIO_SEND_CS, GPIO_SEND_Pin_CS)
/* Deselect SPI : Chip Select pin high */
#define SPI_SEND_CS_HIGH()      GPIO_SetBits(GPIO_SEND_CS, GPIO_SEND_Pin_CS)

#define SPI_RECEIVE_CS_LOW()       GPIO_ResetBits(GPIO_RECEIVE_CS, GPIO_RECEIVE_Pin_CS)
/* Deselect SPI : Chip Select pin high */
#define SPI_RECEIVE_CS_HIGH()      GPIO_SetBits(GPIO_RECEIVE_CS, GPIO_RECEIVE_Pin_CS)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_PageErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(CC1101_TYPE type);
u8 SPI_FLASH_SendByte(u8 byte, CC1101_TYPE type);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WaitForWriteEnd(void);

void CC1101_Init(void);
void	CC1101_POWER_RESET(void);
void CC1101_Send_Main(void);
void CC1101_Receive_Main(void);
#endif
