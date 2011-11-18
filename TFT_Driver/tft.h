#ifndef __TFT_H
#define __TFT_H
/* Define to prevent recursive inclusion -------------------------------------*/
//#define RED	  0XF800
//#define GREEN 0X07E0
//#define BLUE  0X001F  
//#define BRED  0XF81F
//#define GRED  0XFFE0
//#define GBLUE 0X07FF;
//#define BLACK 0;  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"

/*硬件相关的宏定义*/
#define Set_Cs  GPIOD->BSRR  = 0x00001000;
#define Clr_Cs  GPIOD->BRR   = 0x00001000;

#define Set_Rs  GPIOD->BSRR  = 0x00002000;
#define Clr_Rs  GPIOD->BRR   = 0x00002000;

#define Set_nWr GPIOD->BSRR  = 0x00004000;
#define Clr_nWr GPIOD->BRR   = 0x00004000;

#define Set_nRd GPIOD->BSRR  = 0x00008000;
#define Clr_nRd GPIOD->BRR   = 0x00008000;

/*TFT颜色宏定义*/
#define TFT_BLACK	0X0000	//纯黑
#define TFT_NAVY	0X0010	//深蓝 
#define TFT_GREEN	0X0400	//深绿
#define TFT_TEAL	0X0410	//靛青
#define TFT_MAROON	0X8000	//深红
#define TFT_PURPLE	0X8010	//深紫
#define TFT_OLIVE	0X8400	//褐黄
#define TFT_GRAY	0X8410	//深灰
#define TFT_SILVER	0XC618	//浅灰
#define TFT_BLUE	0X001F	//浅蓝
#define TFT_LIME	0X07E0	//浅绿
#define TFT_AQUA	0X07FF	//天蓝
#define TFT_RED		0XF800	//大红
#define TFT_FUCHSIA	0XF81F	//品红
#define TFT_YELLOW	0XFFE0	//明黄
#define TFT_WHITE	0XFFFF	//亮白
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
__inline void WriteRegister(u16 index,u16 dat);

void TFT_Configuration(void);
void LCD_Initialization(void);

void TFT_Touch_Init(void);
unsigned char SPI_WriteByte(unsigned char data);
void SpiDelay(unsigned int DelayCnt);
u16 TPReadX(void);
u16 TPReadY(void);

/****************************************************************************
* 名    称：void TFT_DrawPoint(int x, int y, int PixelIndex)
* 功    能：画点函数
* 入口参数：x，y	坐标
*         ：PixelIndex 16位颜色值
* 出口参数：无
****************************************************************************/
void TFT_DrawPoint(int x, int y, int PixelIndex);

/****************************************************************************
* 名    称：void TFT_DrawHLine(int x0, int y,  int x1, int color)
* 功    能：画水平线
* 入口参数：x0，x1	水平起点和终点坐标
*         ：y  		垂直坐标
*           color  线颜色
* 出口参数：无
****************************************************************************/
void TFT_DrawHLine(int x0, int y,  int x1, int color);

/****************************************************************************
* 名    称：void TFT_DrawVLine(int x, int y0,  int y1, int color)
* 功    能：画垂直线
* 入口参数：y0，y1	垂直起点和终点坐标
*         ：x  		水平坐标
*           color  线颜色
* 出口参数：无
****************************************************************************/
void TFT_DrawVLine(int x, int y0,  int y1, int color);

/****************************************************************************
* 名    称：void TFT_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
* 功    能：在指定座标画直线
* 入口参数：x0     A点行座标
*           y0     A点列座标
*           x1     B点行座标
*           y1     B点列座标
*           color  线颜色
* 出口参数：无
* 调用方法：TFT_DrawLine(0,0,240,320,0x0000);
****************************************************************************/
void TFT_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);

/****************************************************************************
* 名    称：void TFT_FillRect(int x0, int y0, int x1, int y1)
* 功    能：填充矩形
* 入口参数：x0，y0	起点坐标
*         ：x1，y1  终点坐标
*           color  线颜色
* 出口参数：无
****************************************************************************/
void TFT_FillRect(int x0, int y0, int x1, int y1, int color);

/****************************************************************************
* 名    称：void TFT_DispCharAt(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：TFT_DispCharAt(10,10,'a',TFT_BLACK,TFT_WHITE);
****************************************************************************/
void TFT_DispCharAt(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);

/****************************************************************************
* 名    称：void TFT_DispString(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
* 功    能：在指定座标显示字符串
* 入口参数：x      行座标
*           y      列座标
*           *str   字符串
*           len    字符串长度
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 调用方法：TFT_DispString(0,0,"0123456789",10,TFT_BLACK,TFT_WHITE);
****************************************************************************/
void TFT_DispString(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);

/****************************************************************************
* 名    称：TFT_ShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
* 功    能：在指定座标显示数字
* 入口参数：x      行座标
*           y      列座标
*			num	   数字
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 调用方法：TFT_ShowNum(0,0,16,TFT_BLACK,TFT_WHITE);
****************************************************************************/
void TFT_ShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor);

/****************************************************************************
* 名    称：TFT_RightShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
* 功    能：在指定座标显示数字 (右对齐)
* 入口参数：x      行座标
*           y      列座标
*			num	   数字
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 调用方法：TFT_RightShowNum(320,0,16,TFT_BLACK,TFT_WHITE);
****************************************************************************/
void TFT_RightShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor);

/****************************************************************************
* 名    称：u16 TFT_Color565(u32 RGB)
* 功    能：将RGB颜色转换为16位颜色。
* 入口参数：RGB  颜色值
* 出口参数：返回16位RGB颜色值。
* 调用方法：i=TFT_Color565(0xafafaf);
****************************************************************************/
u16 TFT_Color565(u32 RGB);

/****************************************************************************
* 名    称：void TFT_Test(void)
* 功    能：测试。
****************************************************************************/
void TFT_Test(void);


#endif /* __TFT_H */
