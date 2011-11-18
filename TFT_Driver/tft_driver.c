/* Includes ------------------------------------------------------------------*/
#include "tft.h"
#include "tft_font.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//初始化函数
void LCD_Initialization(void)
{
  /*****************************
  **    硬件连接说明          **
  ** STM32         ili9320    **
  ** PE0~15 <----> DB0~15     **
  ** PD15   <----> nRD        **
  ** PD14   <----> RS         **
  ** PD13   <----> nWR        **
  ** PD12   <----> nCS        **
  ** PD11   <----> nReset     **
  ** PC0    <----> BK_LED     **
  ******************************/
 	u16 i;

	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = 0xffff;
	Clr_nWr;
	Set_nWr;
	Set_nWr;
	Set_Cs;
	Set_Rs;
	Set_nRd;
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	WriteRegister(0x0000,0x0001);
	Delay(10000);
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	WriteRegister(0x00,0x0000);
	WriteRegister(0x01,0x0100);	//Driver Output Contral.
	WriteRegister(0x02,0x0700);	//LCD Driver Waveform Contral.

	WriteRegister(0x03,0x1030);	//Entry Mode Set.
	
	WriteRegister(0x04,0x0000);	//Scalling Contral.
	WriteRegister(0x08,0x0202);	//Display Contral 2.(0x0207)
	WriteRegister(0x09,0x0000);	//Display Contral 3.(0x0000)
	WriteRegister(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
	WriteRegister(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
	WriteRegister(0x0d,0x0000);	//Frame Maker Position.
	WriteRegister(0x0f,0x0000);	//Extern Display Interface Contral 2.
	
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	WriteRegister(0x07,0x0101);	//Display Contral.
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	
	WriteRegister(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
	WriteRegister(0x11,0x0007);								//Power Control 2.(0x0001)
	WriteRegister(0x12,(1<<8)|(1<<4)|(0<<0));					//Power Control 3.(0x0138)
	WriteRegister(0x13,0x0b00);								//Power Control 4.
	WriteRegister(0x29,0x0000);								//Power Control 7.
	
	WriteRegister(0x2b,(1<<14)|(1<<4));
	
	WriteRegister(0x50,0);		//Set X Start.
	WriteRegister(0x51,239);	//Set X End.
	WriteRegister(0x52,0);		//Set Y Start.
	WriteRegister(0x53,319);	//Set Y End.
	
	WriteRegister(0x60,0xA700);	//Driver Output Control.
	WriteRegister(0x61,0x0001);	//Driver Output Control.
	WriteRegister(0x6a,0x0000);	//Vertical Srcoll Control.
	
	WriteRegister(0x80,0x0000);	//Display Position? Partial Display 1.
	WriteRegister(0x81,0x0000);	//RAM Address Start? Partial Display 1.
	WriteRegister(0x82,0x0000);	//RAM Address End-Partial Display 1.
	WriteRegister(0x83,0x0000);	//Displsy Position? Partial Display 2.
	WriteRegister(0x84,0x0000);	//RAM Address Start? Partial Display 2.
	WriteRegister(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
	WriteRegister(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
	WriteRegister(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
	WriteRegister(0x93,0x0001);	//Panel Interface Contral 3.
	WriteRegister(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
	WriteRegister(0x97,(0<<8));	//
	WriteRegister(0x98,0x0000);	//Frame Cycle Contral.
	
	WriteRegister(0x07,0x0173);	//(0x0173)
}
/****************************************************************************
* 名    称：void WriteRegister(u16 index,u16 dat)
* 功    能：写指定地址寄存器的值
* 入口参数：index    寄存器地址
*         ：dat      寄存器值
* 出口参数：无
* 说    明：内部函数
* 调用方法：WriteRegister(0x0000,0x0001);
****************************************************************************/
__inline void WriteRegister(u16 index,u16 dat)
{
 /************************************************************************
  **                                                                    **
  ** nCS       ----\__________________________________________/-------  **
  ** RS        ------\____________/-----------------------------------  **
  ** nRD       -------------------------------------------------------  **
  ** nWR       --------\_______/--------\_____/-----------------------  **
  ** DB[0:15]  ---------[index]----------[data]-----------------------  **
  **                                                                    **
  ************************************************************************/
    Clr_Cs;

	Clr_Rs;
	Set_nRd;
	GPIOE->ODR = index;
	Clr_nWr;
	Set_nWr;
    
	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = dat;
	Clr_nWr;
	Set_nWr;
    
	Set_Cs; 
}
/****************************************************************************
* 名    称：void TFT_DrawPoint(int x, int y, int PixelIndex)
* 功    能：画点函数
* 入口参数：x，y	坐标
*         ：PixelIndex 16位颜色值
* 出口参数：无
****************************************************************************/
void TFT_DrawPoint(int x, int y, int PixelIndex)
{
    Clr_Cs;

	Clr_Rs;
	Set_nRd;
	GPIOE->ODR = 0x0020;
	Clr_nWr;
	Set_nWr;
    
	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = y;
	Clr_nWr;
	Set_nWr;

	Clr_Rs;
	Set_nRd;
	GPIOE->ODR = 0x0021;
	Clr_nWr;
	Set_nWr;
    
	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = x;
	Clr_nWr;
	Set_nWr;

    Clr_Rs;
	Set_nRd;
	GPIOE->ODR = 0x0022;
	Clr_nWr;
	Set_nWr;

	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = PixelIndex;
	Clr_nWr;
	Set_nWr;
}
/****************************************************************************
* 名    称：void TFT_DrawHLine(int x0, int y,  int x1)
* 功    能：画水平线
* 入口参数：x0，x1	水平起点和终点坐标
*         ：y  		垂直坐标
* 出口参数：无
****************************************************************************/
void TFT_DrawHLine(int x0, int y,  int x1, int color)
{
    Clr_Cs;

	Clr_Rs;
	Set_nRd;
	GPIOE->ODR = 0x0020;
	Clr_nWr;
	Set_nWr;
    
	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = y;
	Clr_nWr;
	Set_nWr;
	
	for(;x0<=x1;x0++)
	{
		Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0021;
		Clr_nWr;
		Set_nWr;
	    
		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = x0;
		Clr_nWr;
		Set_nWr;
	    
	    Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0022;
		Clr_nWr;
		Set_nWr;

		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = color;
		Clr_nWr;
		Set_nWr;
	}	
}
/****************************************************************************
* 名    称：void TFT_DrawVLine(int x, int y0,  int y1)
* 功    能：画垂直线
* 入口参数：y0，y1	垂直起点和终点坐标
*         ：x  		水平坐标
* 出口参数：无
****************************************************************************/
void TFT_DrawVLine(int x, int y0,  int y1, int color)
{
    Clr_Cs;

	Clr_Rs;
	Set_nRd;
	GPIOE->ODR = 0x0021;
	Clr_nWr;
	Set_nWr;
    
	Set_Rs;
	Set_nRd;
  	GPIOE->ODR = x;
	Clr_nWr;
	Set_nWr;
    
	for(;y0<=y1;y0++)
	{
		Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0020;
		Clr_nWr;
		Set_nWr;
	    
		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = y0;
		Clr_nWr;
		Set_nWr;

	    Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0022;
		Clr_nWr;
		Set_nWr;

		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = color;
		Clr_nWr;
		Set_nWr;
	}	
}
/****************************************************************************
* 名    称：void TFT_FillRect(int x0, int y0, int x1, int y1)
* 功    能：填充矩形
* 入口参数：x0，y0	起点坐标
*         ：x1，y1  终点坐标
* 出口参数：无
****************************************************************************/
void TFT_FillRect(int x0, int y0, int x1, int y1 ,int color)
{
   for (; x0 <= x1; x0++)
    {
        int y;
        /* 填充x轴坐标和y轴坐标 */
	    Clr_Cs;
	
		Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0020;
		Clr_nWr;
		Set_nWr;
	    
		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = y0;
		Clr_nWr;
		Set_nWr;
	    
		Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0021;
		Clr_nWr;
		Set_nWr;
	    
		Set_Rs;
		Set_nRd;
	  	GPIOE->ODR = x0;
		Clr_nWr;
		Set_nWr;

	    Clr_Rs;
		Set_nRd;
		GPIOE->ODR = 0x0022;
		Clr_nWr;
		Set_nWr;

        /* 开始写入显存 */
        y=y0;
        for (;y0 <= y1; y0++)
        {
			Set_Rs;
			Set_nRd;
		  	GPIOE->ODR = color;
			Clr_nWr;
			Set_nWr;
        }
        y0=y;
    }

}
/****************************************************************************
* 名    称：void TFT_DispCharAt(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：TFT_DispCharAt(10,10,'a',0x0000,0xffff);
****************************************************************************/
void TFT_DispCharAt(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
	u16 i=0;
	u16 j=0;
	
	u8 tmp_char=0;
	
	for (i=0;i<16;i++)
	{
		tmp_char=ascii_8x16[((c-0x20)*16)+i];
		for (j=0;j<8;j++)
		{
			if ( (tmp_char >> 7-j) & 0x01 == 0x01)
			{
				TFT_DrawPoint(x+j,y+i,charColor); // 字符颜色
			}
			else
			{
				TFT_DrawPoint(x+j,y+i,bkColor); // 背景颜色
			}
		}
	}
}
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
void TFT_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			TFT_DrawPoint(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			TFT_DrawPoint(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			TFT_DrawPoint(x,y,color);
  		}
 	}
 	else
 	{
 		TFT_DrawPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				TFT_DrawPoint(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				TFT_DrawPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				TFT_DrawPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				TFT_DrawPoint(x, y,color);
    			}
   			}
  		}
 	}
}
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
* 调用方法：TFT_DispString(0,0,"0123456789",10,0x0000,0xffff);
****************************************************************************/
void TFT_DispString(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
{
	u8 i;
	
	for (i=0;i<len;i++)
	{
		TFT_DispCharAt((x+8*i),y,*str++,Color,bkColor);
	}
}
/****************************************************************************
* 名    称：TFT_ShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
* 功    能：在指定座标显示数字
* 入口参数：x      行座标
*           y      列座标
*			num	   数字
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 调用方法：TFT_ShowNum(0,0,16,0x0000,0xffff);
****************************************************************************/
void TFT_ShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
{      
	u32 res;   	   
	u8 t=0,t1=0,t2=0;   
	res=num;
	if(!num)TFT_DispCharAt(x,y,'0',Color,bkColor);//显示0
	while(res)  //得到数字长度
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)	//显示数字
	{	
		res=1;
		t2=t;
		while(t2-1)
		{
			res*=10;
			t2--;
		}
	    TFT_DispCharAt(x+(t1-t)*8,y,(num/res)%10+'0',Color,bkColor);
		t--;
	}				     
} 
/****************************************************************************
* 名    称：TFT_RightShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
* 功    能：在指定座标显示数字 (右对齐)
* 入口参数：x      行座标
*           y      列座标
*			num	   数字
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 调用方法：TFT_RightShowNum(320,0,16,0x0000,0xffff);
****************************************************************************/
void TFT_RightShowNum(u16 x,u16 y,u32 num,u16 Color, u16 bkColor)
{      
	u32 res;   	   
	u8 t=0,t2=0;   
	res=num;
	if(!num)TFT_DispCharAt(x-8,y,'0',Color,bkColor);//显示0
	while(res)  //得到数字长度
	{
		res/=10;
		t++;
	}
	while(t)	//显示数字
	{	
		res=1;
		t2=t;
		while(t2-1)
		{
			res*=10;
			t2--;
		}
	    TFT_DispCharAt(x-t*8,y,(num/res)%10+'0',Color,bkColor);
		t--;
	}				     
} 
/****************************************************************************
* 名    称：u16 TFT_Color565(u32 RGB)
* 功    能：将RGB颜色转换为16位颜色。
* 入口参数：RGB  颜色值
* 出口参数：返回16位RGB颜色值。
* 调用方法：i=TFT_Color565(0xafafaf);
****************************************************************************/
u16 TFT_Color565(u32 RGB)
{
  u8  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;		// 取B色的高5位
  g = ( RGB >> (8+2) ) & 0x3f;		// 取G色的高6位
  r = ( RGB >> (16+3)) & 0x1f;		// 取R色的高5位
   
  return( (r<<11) + (g<<5) + (b<<0) );		
}
/****************************************************************************
* 名    称：void TFT_Test(void)
* 功    能：测试。
****************************************************************************/
void TFT_Test(void)
{
	TFT_DrawPoint(100,100,0);
	TFT_DrawHLine(50,50,150,0);
	TFT_DrawVLine(120,50,200,0);
	TFT_DispCharAt(10,30,'a',0x0000,0xffff);
	TFT_DrawLine(0,0,320,240,0x0000);
	TFT_DispString(70,10,"0123456789",10,0x0000,0xffff);
	TFT_FillRect(130,130,170,170,TFT_Color565(0xFFFF00));
	TFT_ShowNum(10,0,TFT_Color565(0xFFFF00),0x0000,0xffff);
	TFT_RightShowNum(320,0,16,0x0000,0xffff);
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
