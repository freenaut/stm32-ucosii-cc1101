#include <includes.h>
#include "cc1101.h"

#define   Dummy_Byte			0xff

#define 	WRITE_BURST     		0x40		/* 连续写入							*/
#define 	READ_SINGLE     		0x80		/* 读 									*/
#define 	READ_BURST      		0xC0		/* 连续读							*/
#define 	BYTES_IN_RXFIFO     	0x7F  		/* 接收缓冲区的有效字节数	*/
#define 	CRC_OK              		0x80 		/* CRC校验通过位标志			*/

// CC1100 STROBE, CONTROL AND STATUS REGSITER
#define CCxxx0_IOCFG2       	0x00        	/* GDO2 output pin configuration 					*/
#define CCxxx0_IOCFG1       	0x01        	/* GDO1 output pin configuration 					*/
#define CCxxx0_IOCFG0       	0x02        	/* GDO0 output pin configuration 					*/
#define CCxxx0_FIFOTHR     	0x03       	/* RX FIFO and TX FIFO thresholds				*/
#define CCxxx0_SYNC1        	0x04        	/* Sync word, high INT8U						*/
#define CCxxx0_SYNC0        	0x05        	/* Sync word, low INT8U						*/
#define CCxxx0_PKTLEN       	0x06        	/* Packet length								*/
#define CCxxx0_PKTCTRL1     	0x07       	/* Packet automation control						*/
#define CCxxx0_PKTCTRL0    	0x08       	/* Packet automation control						*/
#define CCxxx0_ADDR         	0x09        	/* Device address								*/
#define CCxxx0_CHANNR       	0x0A        	/* Channel number							*/	
#define CCxxx0_FSCTRL1      	0x0B        	/* Frequency synthesizer control					*/	
#define CCxxx0_FSCTRL0      	0x0C        	/* Frequency synthesizer control					*/
#define CCxxx0_FREQ2        	0x0D        	/* Frequency control word, high INT8U				*/
#define CCxxx0_FREQ1        	0x0E        	/* Frequency control word, middle INT8U			*/	
#define CCxxx0_FREQ0        	0x0F        	/* Frequency control word, low INT8U				*/
#define CCxxx0_MDMCFG4      	0x10          	/* Modem configuration							*/	
#define CCxxx0_MDMCFG3      	0x11        	/* Modem configuration							*/
#define CCxxx0_MDMCFG2      	0x12        	/* Modem configuration							*/
#define CCxxx0_MDMCFG1      	0x13        	/* Modem configuration							*/
#define CCxxx0_MDMCFG0      	0x14        	/* Modem configuration							*/
#define CCxxx0_DEVIATN      	0x15        	/* Modem deviation setting						*/
#define CCxxx0_MCSM2        	0x16        	/* Main Radio Control State Machine configuration	*/
#define CCxxx0_MCSM1        	0x17        	/* Main Radio Control State Machine configuration	*/
#define CCxxx0_MCSM0        	0x18        	/* Main Radio Control State Machine configuration	*/
#define CCxxx0_FOCCFG       	0x19        	/* Frequency Offset Compensation configuration		*/
#define CCxxx0_BSCFG        	0x1A        	/* Bit Synchronization configuration				*/
#define CCxxx0_AGCCTRL2     	0x1B        	/* AGC control								*/
#define CCxxx0_AGCCTRL1     	0x1C        	/* AGC control								*/
#define CCxxx0_AGCCTRL0     	0x1D        	/* AGC control								*/
#define CCxxx0_WOREVT1      	0x1E        	/* High INT8U Event 0 timeout					*/
#define CCxxx0_WOREVT0      	0x1F        	/* Low INT8U Event 0 timeout					*/
#define CCxxx0_WORCTRL      	0x20        	/* Wake On Radio control						*/
#define CCxxx0_FREND1       	0x21        	/* Front end RX configuration					*/
#define CCxxx0_FREND0       	0x22       	/* Front end TX configuration					*/
#define CCxxx0_FSCAL3       	0x23        	/* Frequency synthesizer calibration				*/
#define CCxxx0_FSCAL2       	0x24        	/* Frequency synthesizer calibration				*/
#define CCxxx0_FSCAL1       	0x25        	/* Frequency synthesizer calibration				*/
#define CCxxx0_FSCAL0       	0x26        	/* Frequency synthesizer calibration				*/
#define CCxxx0_RCCTRL1      	0x27        	/* RC oscillator configuration					*/
#define CCxxx0_RCCTRL0      	0x28        	/* RC oscillator configuration					*/
#define CCxxx0_FSTEST       	0x29        	/* Frequency synthesizer calibration control		*/
#define CCxxx0_PTEST        	0x2A        	/* Production test								*/
#define CCxxx0_AGCTEST      	0x2B        	/* AGC test									*/
#define CCxxx0_TEST2        	0x2C        	/* Various test settings							*/
#define CCxxx0_TEST1        	0x2D        	/* Various test settings							*/
#define CCxxx0_TEST0        	0x2E        	/* Various test settings							*/

// Strobe commands
#define CCxxx0_SRES         0x30        	/* Reset chip.										*/
#define CCxxx0_SFSTXON      0x31        	/* 	Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        					If in RX/TX: Go to a wait state where only the synthesizer is
                                        					running (for quick RX / TX turnaround).                                              */
#define CCxxx0_SXOFF        0x32        	/* 	Turn off crystal oscillator.                                                */
#define CCxxx0_SCAL         0x33        	/* 	Calibrate frequency synthesizer and turn it off			*/
                                        				/* 	(enables quick start).								*/
#define CCxxx0_SRX          0x34        	/* 	Enable RX. Perform calibration first if coming from IDLE and
                                        					MCSM0.FS_AUTOCAL=1.							*/
#define CCxxx0_STX          0x35        	/* 	In IDLE state: Enable TX. Perform calibration first if
                                        					MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        					Only go to TX if channel is clear.							*/
#define CCxxx0_SIDLE        0x36        	/*	Exit RX / TX, turn off frequency synthesizer and exit
                                        					Wake-On-Radio mode if applicable.					*/
#define CCxxx0_SAFC         0x37        	/*	Perform AFC adjustment of the frequency synthesizer	*/
#define CCxxx0_SWOR         0x38        	/*	Start automatic RX polling sequence (Wake-on-Radio)	*/
#define CCxxx0_SPWD         0x39        	/*	Enter power down mode when CSn goes high.			*/
#define CCxxx0_SFRX         0x3A        	/*	Flush the RX FIFO buffer.							*/
#define CCxxx0_SFTX         0x3B        	/*	Flush the TX FIFO buffer.							*/
#define CCxxx0_SWORRST      0x3C        	/*	Reset real time clock.								*/
#define CCxxx0_SNOP         0x3D        	/*	No operation. May be used to pad strobe commands to two
                                        					INT8Us for simpler software.							*/

#define CCxxx0_PARTNUM      0x30
#define CCxxx0_VERSION      0x31
#define CCxxx0_FREQEST      0x32
#define CCxxx0_LQI          0x33
#define CCxxx0_RSSI         0x34
#define CCxxx0_MARCSTATE    0x35
#define CCxxx0_WORTIME1     0x36
#define CCxxx0_WORTIME0     0x37
#define CCxxx0_PKTSTATUS    0x38
#define CCxxx0_VCO_VC_DAC   0x39
#define CCxxx0_TXBYTES      0x3A
#define CCxxx0_RXBYTES      0x3B

#define CCxxx0_PATABLE      0x3E
#define CCxxx0_TXFIFO       0x3F
#define CCxxx0_RXFIFO       0x3F

INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};
INT8U paTableLen = 4;                                    /*	对应功率值，默认4则为0dbm发送*/

INT8U txBuffer[20]={15,1,3,4//,5,6,7,8,9,4,5,5,6,2,1,3,4,5,6,7
                  //,3,4,5,6,7,8,9,4,5,5,6,2,1,3,4,5,6,7,8,9
                  ,7,8,9,4,5,5,6,2,1,3,4,5,6,7,8,6,};

//RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
typedef struct S_RF_SETTINGS
{
	INT8U FSCTRL2;			/*	Frequency synthesizer control 2.				*/
	INT8U FSCTRL1;   			/*	Frequency synthesizer control 1.				*/
	INT8U FSCTRL0;   			/*	Frequency synthesizer control.				*/
	INT8U FREQ2;     			/*	Frequency control word, high INT8U.			*/
	INT8U FREQ1;     			/*	Frequency control word, middle INT8U.			*/
	INT8U FREQ0;     			/*	Frequency control word, low INT8U.			*/
	INT8U MDMCFG4;   		/*	Modem configuration.						*/
	INT8U MDMCFG3;   		/*	Modem configuration.						*/
	INT8U MDMCFG2;   		/*	Modem configuration.						*/
	INT8U MDMCFG1;   		/*	Modem configuration.						*/
	INT8U MDMCFG0;   		/*	Modem configuration.						*/
	INT8U CHANNR;    		/*	Channel number.							*/
	INT8U DEVIATN;   		/*	Modem deviation setting (when FSK modulation is enabled).	*/
	INT8U FREND1;    			/*	Front end RX configuration.					*/
	INT8U FREND0;    			/*	Front end RX configuration.					*/
	INT8U MCSM0;     			/*	Main Radio Control State Machine configuration.	*/
	INT8U FOCCFG;    			/*	Frequency Offset Compensation Configuration.	*/
	INT8U BSCFG;     			/*	Bit synchronization Configuration.				*/
	INT8U AGCCTRL2;  		/*	AGC control.								*/
	INT8U AGCCTRL1;  		/*	AGC control.								*/
	INT8U AGCCTRL0;  		/*	AGC control.								*/
	INT8U FSCAL3;    			/*	Frequency synthesizer calibration.				*/
	INT8U FSCAL2;    			/*	Frequency synthesizer calibration.				*/
	INT8U FSCAL1;    			/*	Frequency synthesizer calibration.				*/
	INT8U FSCAL0;    			/*	Frequency synthesizer calibration.				*/
	INT8U FSTEST;    			/*	Frequency synthesizer calibration control		*/
	INT8U TEST2;     			/*	Various test settings.						*/
	INT8U TEST1;     			/*	Various test settings.						*/
	INT8U TEST0;     			/*	Various test settings.						*/
	INT8U IOCFG2;    			/*	GDO2 output pin configuration				*/
	INT8U IOCFG0;    			/*	GDO0 output pin configuration				*/
	INT8U PKTCTRL1;  		/*	Packet automation control.					*/
	INT8U PKTCTRL0;  		/*	Packet automation control.					*/
	INT8U ADDR;      			/*	Device address.							*/
	INT8U PKTLEN;    			/*	Packet length.								*/
} RF_SETTINGS;

const RF_SETTINGS rfSettings = {
	0x00,
	0x08, // FSCTRL1 Frequency synthesizer control.
	0x00, // FSCTRL0 Frequency synthesizer control.
	0x10, // FREQ2 Frequency control word, high byte.
	0xA7, // FREQ1 Frequency control word, middle byte.
	0x62, // FREQ0 Frequency control word, low byte.
	0x5B, //0x2D, // MDMCFG4 Modem configuration.
	0xF8, //0x3B, // MDMCFG3 Modem configuration.
	0x02, //0x73, // MDMCFG2 Modem configuration.
	0x22, // MDMCFG1 Modem configuration.
	0xF8, // MDMCFG0 Modem configuration.
	
	0x00, // CHANNR Channel number.
	0x47, // DEVIATN Modem deviation setting (when FSK modulation is enabled).
	0xB6, // FREND1 Front end RX configuration.
	0x10, // FREND0 Front end RX configuration.
	0x18, // MCSM0 Main Radio Control State Machine configuration.
	0x1D, // FOCCFG Frequency Offset Compensation Configuration.
	0x1C, // BSCFG Bit synchronization Configuration.
	0x07, // AGCCTRL2 AGC control.
	0x00, //0x00, // AGCCTRL1 AGC control.
	0xB2, // AGCCTRL0 AGC control.
	
	0xEA, // FSCAL3 Frequency synthesizer calibration.
	0x2A, // FSCAL2 Frequency synthesizer calibration.
	0x00, // FSCAL1 Frequency synthesizer calibration.
	0x1F, // FSCAL0 Frequency synthesizer calibration.
	0x59, // FSTEST Frequency synthesizer calibration.
	0x88, // TEST2 Various test settings.
	0x31, // TEST1 Various test settings.
	0x0B, // TEST0 Various test settings.
	0x0B, // IOCFG2 GDO2 output pin configuration.
	0x06, // IOCFG0D GDO0 output pin configuration.
	
	0x05, // PKTCTRL1 Packet automation control.
	0x05, // PKTCTRL0 Packet automation control.
	0x22, // ADDR Device address.
	0xff // PKTLEN Packet length.
};

static unsigned char send_num=0;

/*******************************************************************************
* Function Name  : CC1101_Init()
* Description    : Initializes the peripherals .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CC1101_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC , ENABLE);

	/* Configure SPI2 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_SEND_Pin_SCLK | GPIO_SEND_Pin_SO | GPIO_SEND_Pin_SI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure SPI1 pins: SCK, MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_RECEIVE_Pin_SCLK | GPIO_RECEIVE_Pin_SI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure SPI1 pins: MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_RECEIVE_Pin_SO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure I/O for CC1101 SEND Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_SEND_Pin_CS ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SEND_CS, &GPIO_InitStructure);

	/* Configure I/O for CC1101 RECEIVE Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_RECEIVE_Pin_CS ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_RECEIVE_CS, &GPIO_InitStructure);
	
	/* Configure I/O for PC4,PC5,PC6,PC7  */
	GPIO_InitStructure.GPIO_Pin = GPIO_SEND_Pin_GD2| GPIO_SEND_Pin_GD0 | GPIO_RECEIVE_Pin_GD2| GPIO_RECEIVE_Pin_GD0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Enable SPI1 && SPI2 && GPIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* SPI2 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI_Direction_2Lines_RxOnly;//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI_Direction_2Lines_RxOnly;//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_FLASH_ReadByte(CC1101_TYPE type)
{
	if(type == CC1101_TYPE_SEND){
		return (SPI_FLASH_SendByte(Dummy_Byte, CC1101_TYPE_SEND));
	}
	else
	if(type == CC1101_TYPE_RECEIVE){
		return (SPI_FLASH_SendByte(Dummy_Byte, CC1101_TYPE_RECEIVE));
	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte, CC1101_TYPE type)
{
	if(type == CC1101_TYPE_SEND){
		/* Loop while DR register in not emplty */
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

		/* Send byte through the SPI2 peripheral */
		SPI_I2S_SendData(SPI2, byte);

		/* Wait to receive a byte */
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

		/* Return the byte read from the SPI bus */
		return SPI_I2S_ReceiveData(SPI2);
	}
	else 
	if(type == CC1101_TYPE_RECEIVE)
	{
		/* Loop while DR register in not emplty */
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

		/* Send byte through the SPI2 peripheral */
		SPI_I2S_SendData(SPI1, byte);

		/* Wait to receive a byte */
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

		/* Return the byte read from the SPI bus */
		return SPI_I2S_ReceiveData(SPI1);
	}
}

/**********************************CC1101********************/

void Delay_1(vu32 nCount)
{
  int i,j;
  for(j=0;j<nCount;j++)
  {
     for(i=0;i<10;i++);
  }
}


INT8U SPI_CC1101_ReadID(void)
{
	 INT8U id;
	 SPI_SEND_CS_LOW();
	 
//	 SPI_FLASH_SendByte(0x30|0xc0);
//	 id = SPI_FLASH_SendByte(0xff);
//	 SPI_FLASH_SendByte(0x31|0xc0);
	 
	 SPI_FLASH_SendByte(0x31, CC1101_TYPE_SEND);
	 id = SPI_FLASH_SendByte(0xff, CC1101_TYPE_SEND);
	 SPI_SEND_CS_HIGH();

	 return id;
}

void CC1101_POWER_RESET(void)
{
	/* Deselect the FLASH: Chip Select high */
	SPI_SEND_CS_HIGH();     
	GPIO_SetBits(GPIOB, GPIO_SEND_Pin_SCLK); //SCLK=1
	GPIO_ResetBits(GPIOB, GPIO_SEND_Pin_SI); //SI=0
	GPIO_SetBits(GPIOA, GPIO_RECEIVE_Pin_SCLK); //SCLK=1
	GPIO_ResetBits(GPIOA, GPIO_RECEIVE_Pin_SI); //SI=0
	/*  Chip Select low */
	Delay_1(5000);
	SPI_SEND_CS_HIGH();
	SPI_RECEIVE_CS_HIGH();
	Delay_1(1);
	SPI_SEND_CS_LOW();
	SPI_RECEIVE_CS_LOW();
	Delay_1(1);
	SPI_SEND_CS_HIGH();
	SPI_RECEIVE_CS_HIGH();
	Delay_1(41);
	SPI_SEND_CS_LOW();
	SPI_RECEIVE_CS_LOW();
	while (GPIO_ReadInputDataBit(GPIOB,GPIO_SEND_Pin_SO)  || GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO));//waite SO =0
	SPI_FLASH_SendByte(CCxxx0_SRES, CC1101_TYPE_SEND);
	SPI_FLASH_SendByte(CCxxx0_SRES, CC1101_TYPE_RECEIVE);
	while (GPIO_ReadInputDataBit(GPIOB,GPIO_SEND_Pin_SO) || GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO));//waite SO =0 again 
	SPI_SEND_CS_HIGH(); 
	SPI_RECEIVE_CS_HIGH();
}

//*****************************************************************************************
//函数名：void halSpiWriteReg(INT8U addr, INT8U value, CC1101_TYPE type) 
//输入：地址和配置字
//输出：无
//功能描述：SPI写寄存器
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value, CC1101_TYPE type) 
{
	if(type == CC1101_TYPE_SEND){
		SPI_SEND_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOB,GPIO_SEND_Pin_SO) );
		SPI_FLASH_SendByte(addr, CC1101_TYPE_SEND);		//写地址
		SPI_FLASH_SendByte(value, CC1101_TYPE_SEND);		//写入配置
		SPI_SEND_CS_HIGH(); 
	}
	else
	if(type == CC1101_TYPE_RECEIVE){
		SPI_RECEIVE_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_RECEIVE_Pin_SO) );
		SPI_FLASH_SendByte(addr, CC1101_TYPE_RECEIVE);		//写地址
		SPI_FLASH_SendByte(value, CC1101_TYPE_RECEIVE);		//写入配置
		SPI_RECEIVE_CS_HIGH();
	}
}

//*****************************************************************************************
//函数名：void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count, CC1101_TYPE type) 
{
	if(type == CC1101_TYPE_SEND){
		INT8U i, temp;
		temp = addr | WRITE_BURST;
		SPI_SEND_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOB,GPIO_SEND_Pin_SO) );
		SPI_FLASH_SendByte(temp, CC1101_TYPE_SEND);
		for (i = 0; i < count; i++)
		{
			SPI_FLASH_SendByte(buffer[i], CC1101_TYPE_SEND);
		}
		SPI_SEND_CS_HIGH(); 
	}
	else
	if(type == CC1101_TYPE_RECEIVE){
		INT8U i, temp;
		temp = addr | WRITE_BURST;
		SPI_RECEIVE_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO) );
		SPI_FLASH_SendByte(temp, CC1101_TYPE_RECEIVE);
		for (i = 0; i < count; i++)
		{
			SPI_FLASH_SendByte(buffer[i], CC1101_TYPE_RECEIVE);
		}
		SPI_RECEIVE_CS_HIGH(); 
	}
}

//*****************************************************************************************
//函数名：void halSpiStrobe(INT8U strobe)
//输入：命令
//输出：无
//功能描述：SPI写命令
//*****************************************************************************************
void halSpiStrobe(INT8U strobe, CC1101_TYPE type) 
{
	if(type == CC1101_TYPE_SEND){
		SPI_SEND_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOB,GPIO_SEND_Pin_SO) );
		SPI_FLASH_SendByte(strobe, CC1101_TYPE_SEND);		//写入命令
		SPI_SEND_CS_HIGH();
	}
	else
	if(type == CC1101_TYPE_RECEIVE){
		SPI_RECEIVE_CS_LOW();
		while (GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO) );
		SPI_FLASH_SendByte(strobe, CC1101_TYPE_RECEIVE);		//写入命令
		SPI_RECEIVE_CS_HIGH();
	}
}

//*****************************************************************************************
//函数名：void halRfSendPacket(INT8U *txBuffer, INT8U size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1100发送一组数据
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
	halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size, CC1101_TYPE_SEND);	//写入要发送的数据
	halSpiStrobe(CCxxx0_STX, CC1101_TYPE_SEND);		//进入发送模式发送数据	

	// Wait for GDO0 to be set -> sync transmitted
	while (!GPIO_ReadInputDataBit(GPIOC,GPIO_SEND_Pin_GD0) );/* while (!GDO0);*/

	// Wait for GDO0 to be cleared -> end of packet
	while (GPIO_ReadInputDataBit(GPIOC,GPIO_SEND_Pin_GD0) ); /* while (GDO0);*/

	halSpiStrobe(CCxxx0_SFTX, CC1101_TYPE_SEND);
}

//*****************************************************************************************
//函数名：void halRfSendData(INT8U txData)
//输入：发送的数据
//输出：无
//功能描述：CC1100发送一个数据
//*****************************************************************************************

void halRfSendData(INT8U txData)
{
    halSpiWriteReg(CCxxx0_TXFIFO, txData, CC1101_TYPE_SEND);	//写入要发送的数据

    halSpiStrobe(CCxxx0_STX, CC1101_TYPE_SEND);		//进入发送模式发送数据	

    // Wait for GDO0 to be set -> sync transmitted
    while (!GPIO_ReadInputDataBit(GPIOC,GPIO_SEND_Pin_GD0) );//while (!GDO0);
    // Wait for GDO0 to be cleared -> end of packet
    while (GPIO_ReadInputDataBit(GPIOC,GPIO_SEND_Pin_GD0) );// while (GDO0);
	halSpiStrobe(CCxxx0_SFTX, CC1101_TYPE_SEND);
}

//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该寄存器的配置字
//功能描述：SPI读寄存器
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
	INT8U temp, value;
	temp = addr|READ_SINGLE;//读寄存器命令
	SPI_RECEIVE_CS_LOW();
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO) );//MISO
	SPI_FLASH_SendByte(temp, CC1101_TYPE_RECEIVE);
	value = SPI_FLASH_SendByte(0, CC1101_TYPE_RECEIVE);
	 SPI_RECEIVE_CS_HIGH();
	return value;
}

//*****************************************************************************************
//函数名：void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
	INT8U i,temp;
	temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
	SPI_RECEIVE_CS_LOW();
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO) );//MISO
	SPI_FLASH_SendByte(temp, CC1101_TYPE_RECEIVE);   
	for (i = 0; i < count; i++) 
	{
		buffer[i] = SPI_FLASH_SendByte(0, CC1101_TYPE_RECEIVE);
	}
	SPI_RECEIVE_CS_HIGH();
}


INT8U halSpiReadStatus(INT8U addr) 
{
	INT8U value,temp;
	temp = addr | READ_BURST;		/*/写入要读的状态寄存器的地址同时写入读命令*/
	SPI_RECEIVE_CS_LOW();
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_RECEIVE_Pin_SO) );
	SPI_FLASH_SendByte(temp, CC1101_TYPE_RECEIVE);
	value = SPI_FLASH_SendByte(0, CC1101_TYPE_RECEIVE);
	SPI_RECEIVE_CS_HIGH();
	return value;
}


INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
	INT8U status[2];
	INT8U RxFifoData;
	INT8U packetLength;
	INT8U address;
	INT8U i=(*length)*4;  /*/ 具体多少要根据datarate和length来决定*/

	halSpiStrobe(CCxxx0_SRX, CC1101_TYPE_RECEIVE);		/*/进入接收状态*/
	OSTimeDlyHMSM(0, 0, 0, 10);
	while (!GPIO_ReadInputDataBit(GPIOC,GPIO_RECEIVE_Pin_GD0) )//while (GDO0)
	{
		OSTimeDlyHMSM(0, 0, 0, 1);
		--i;
		if(i<1){
		   return 0;
		}
	}

	RxFifoData =halSpiReadStatus(CCxxx0_RXBYTES);
	printf("Rx FIFO data %d \r\n", RxFifoData);
	if (RxFifoData & BYTES_IN_RXFIFO) /*/如果接的字节数不为0*/
	{
		halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, 9); /*/读出所有接收到的数据*/
		*length = rxBuffer[1];				/*/把接收数据长度的修改为当前数据的长度*/

		// Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
		halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	/*/读出CRC校验位*/
		halSpiStrobe(CCxxx0_SFRX, CC1101_TYPE_RECEIVE);		/*/清洗接收缓冲区*/
		printf("Check CRC %s\r\n", (status[1] & CRC_OK)==CRC_OK?"Ok":"Fail");
		return (status[1] & CRC_OK);			/*/如果校验成功返回接收成功*/
	} 
 	return 0;
}


//*****************************************************************************************
//函数名：void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//输入：无
//输出：无
//功能描述：配置CC1100的寄存器
//*****************************************************************************************
void halRfWriteRfSettings(CC1101_TYPE type) 
{

	halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2, type);
	// Write register settings
	halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1, type);
	halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0, type);
	halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2, type);
	halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1, type);
	halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0, type);
	halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4, type);
	halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3, type);
	halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2, type);
	halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1, type);
	halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0, type);
	halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR, type);
	halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN, type);
	halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1, type);
	halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0, type);
	halSpiWriteReg(CCxxx0_MCSM0,   rfSettings.MCSM0, type);
	halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG, type);
	halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG, type);
	halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2, type);
	halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1, type);	 //
	halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0, type);
	halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3, type);
	halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2, type);
	halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1, type);	//
	halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0, type);
	halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST, type);
	halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2, type);
	halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1, type);
	halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0, type);
	halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2, type);
	if(type==CC1101_TYPE_RECEIVE)
	halSpiWriteReg(CCxxx0_IOCFG0,  0x01, type);  
	else
	halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0, type);    
	halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1, type);
	halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0, type);
	if(type==CC1101_TYPE_RECEIVE)
	halSpiWriteReg(CCxxx0_ADDR, 0xEF , type);
	else
	halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR, type);
	halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN, type);
}

INT8U length=8;
INT8U txBuf[9] = {0x08, 0xEF, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
INT8U rxBuf[9] = {0, 0, 0, 0, 0, 0, 0, 0};
void CC1101_Send_Main(void)
{
    	INT8U num=0;	
	INT8U tf =0;	
	halRfWriteRfSettings(CC1101_TYPE_SEND);
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8, CC1101_TYPE_SEND);
	while(1)
    {
//    	GPIO_SetBits(GPIOA, GPIO_PIN_LED);
//		OSTimeDlyHMSM(0, 0, 0, 100);
		halSpiStrobe(CCxxx0_SIDLE, CC1101_TYPE_SEND);            	/* 进入IDLE模式		*/
		txBuf[2] = num++;
		halRfSendPacket(txBuf, 9);
//		GPIO_ResetBits(GPIOA, GPIO_PIN_LED);
		printf("Send a packet !\r\n");
		OSTimeDlyHMSM(0, 0, 1, 0);
//		
//		Delay(180000);
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//		Delay(180000);
    } 	
}

void CC1101_Receive_Main(void)
{
	INT8U i;
	GPIO_SetBits(GPIOA, GPIO_PIN_LED);
	OSTimeDlyHMSM(0, 0, 2, 0);
	halRfWriteRfSettings(CC1101_TYPE_RECEIVE);
	GPIO_ResetBits(GPIOA, GPIO_PIN_LED);
	OSTimeDlyHMSM(0, 0, 2, 0);
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8, CC1101_TYPE_RECEIVE);
	GPIO_SetBits(GPIOA, GPIO_PIN_LED);
	OSTimeDlyHMSM(0, 0, 2, 0);
	while(1)
	{
		GPIO_SetBits(GPIOA, GPIO_PIN_LED);
		OSTimeDlyHMSM(0, 0, 0, 100);
		if(halRfReceivePacket(rxBuf, &length))
		{
			for(i = 0; i < 9; i++)
		 	{
		 		printf(" txBuf[%d]=%d , rxBuf[%d]=%d \r\n", i, txBuf[i], i, rxBuf[i]);
		 	}
			if(i == 8)
			{
		 		GPIO_SetBits(GPIOA, GPIO_PIN_LED);
				OSTimeDlyHMSM(0, 0, 1, 0);
			}
		}
		GPIO_ResetBits(GPIOA, GPIO_PIN_LED);
		OSTimeDlyHMSM(0, 0,0, 100);
	}
}

