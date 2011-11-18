/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCD_DrawBitmap_888.c
Purpose     : Drawing routine for drawing a 16bpp bitmap
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "LCD_Private.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawBitmap
*/
static void _DrawBitmap(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag, tLCDDEV_Index2Color * pfIndex2Color) {
  const U32 * pData;
  int y, OldIndex, Mag, BytesPerLine, UseDrawBitmap = 0;
  U32 PrevColor;
  GUI_USE_PARA(pLogPal);
  OldIndex = LCD_GetColorIndex();
  PrevColor = GUI_INVALID_COLOR;
  pData = (const U32 *)pPixel;
  Mag = (xMag | yMag);
  BytesPerLine = xsize * 4;
  /* Check if bitmaps color format fits to the current color conversion */
  if (Mag == 1) {
    #if GUI_SUPPORT_MEMDEV
    if (!GUI_Context.hDevData) {
    #else
    {
    #endif
      if (LCD_GetpfIndex2ColorEx(GUI_Context.SelLayer) == pfIndex2Color) {
        UseDrawBitmap = 1;
      }
    }
  }
  if (UseDrawBitmap) {
    LCD_DrawBitmap(x0, y0, xsize, ysize, 1, 1, 24, BytesPerLine, pPixel, 0);
  } else {
    if (Mag == 1) {
      /* Perform clipping */
      int x1, y1, Diff;
      y1 = y0 + ysize - 1;
      x1 = x0 + xsize - 1;
      /* Clip Y */
      if (y0 < GUI_Context.ClipRect.y0) {
        Diff = GUI_Context.ClipRect.y0 - y0;
        y0   = GUI_Context.ClipRect.y0;
        #if GUI_SUPPORT_LARGE_BITMAPS                       /* Required only for 16 bit CPUs if some bitmaps are >64kByte */
          pData += (U32)Diff * (U32)BytesPerLine / 4;
        #else
          pData += (unsigned)Diff * (unsigned)BytesPerLine / 4;
        #endif
        ysize -= Diff;
      }
      Diff = y1 - GUI_Context.ClipRect.y1;
      if (Diff > 0) {
        ysize -= Diff;
      }
      if (ysize <=0) {
		    return;
      }
      /* Clip right side */
      Diff = x1 - GUI_Context.ClipRect.x1;
      if (Diff > 0) {
        xsize -= Diff;
      }
      /* Clip left side */
      if (x0 < GUI_Context.ClipRect.x0) {
        Diff = GUI_Context.ClipRect.x0 - x0;
			  xsize -= Diff;
        pData += Diff; 
        x0 += Diff; 
      }
      if (xsize <=0) {
		    return;
      }
      /* Simple, unmagnified output using LCD_L0_SetPixel() */
      for (y = 0; y < ysize; y++) {
        int x;
        const U32 * p = pData;
        for (x = 0; x < xsize; x++) {
          U32 Color;
          Color = *p++;
          if (Color != PrevColor) {
            LCD_SetColor(pfIndex2Color(Color));
            PrevColor = Color;
          }
          LCDDEV_L0_SetPixelIndex(x + x0, y + y0, LCD_COLORINDEX);
        }
        pData += BytesPerLine / 4;
      }
    } else {
      /* Magnified output using LCD_FillRect() */
      int x,y;
      int yi;
      for (y = y0, yi = 0; yi < ysize; yi++, y += yMag, pPixel += BytesPerLine) {
        int yMax;
        yMax = y + yMag - 1;
        /* Draw if within clip area */
        if ((yMax >= GUI_Context.ClipRect.y0) && (y <= GUI_Context.ClipRect.y1)) {
          int xi;
          const U32 GUI_UNI_PTR * p = pData;
          for (x = x0, xi = 0; xi < xsize; xi++, x += xMag) {
            U32 Color;
            Color = *p++;
            if (Color != PrevColor) {
              LCD_SetColor(pfIndex2Color(Color));
              PrevColor = Color;
            }
            LCD_FillRect(x, y, x + xMag - 1, yMax);
          }
        }
        pData += BytesPerLine / 4;
      }
    }
    GUI_SetColorIndex(OldIndex);
  }
}

/*********************************************************************
*
*       _DrawBitmap_565
*/
static void _DrawBitmap_888(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  _DrawBitmap(x0, y0, xsize, ysize, pPixel, pLogPal, xMag, yMag, GUI_BitmapMethods888.pfIndex2Color);
}

/*********************************************************************
*
*       _DrawBitmap_M565
*/
static void _DrawBitmap_M888(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  _DrawBitmap(x0, y0, xsize, ysize, pPixel, pLogPal, xMag, yMag, GUI_BitmapMethodsM888.pfIndex2Color);
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_BitmapMethods888
*/
const GUI_BITMAP_METHODS GUI_BitmapMethods888 = {
  _DrawBitmap_888,
  LCD_Index2Color_888
};

/*********************************************************************
*
*       GUI_BitmapMethodsM888
*/
const GUI_BITMAP_METHODS GUI_BitmapMethodsM888 = {
  _DrawBitmap_M888,
  LCD_Index2Color_M888
};

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
