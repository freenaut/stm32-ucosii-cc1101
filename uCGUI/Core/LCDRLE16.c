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
File        : LCDRLE16.c
Purpose     : Drawing routines for run length encoded bitmaps
              with 8 bpp
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "LCD_Private.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static struct {
  int x, y;
  const U8 GUI_UNI_PTR * pPixel;
  const U8 GUI_UNI_PTR * pPixelStart;
} _Cache;

static int _DisplayMode;
static U8  _DisplaySwapRB;
static U8  _BitmapSwapRB;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _Read16
*/
static U16 _Read16(const U8 GUI_UNI_PTR ** ppData) {
  const U8 GUI_UNI_PTR * pData;
  U16  Value;
  pData = *ppData;
  Value = *pData;
  Value |= (U16)(*(pData + 1) << 8);
  pData += 2;
  *ppData = pData;
  return Value;
}

/*********************************************************************
*
*       _DrawBitmap_RLE16
*/
static LCD_PIXELINDEX _GetPixelIndex(const U8 GUI_UNI_PTR ** ppPixel) {
  U16 Data = _Read16(ppPixel);
  if ((_DisplayMode == 565) && (_DisplaySwapRB == _BitmapSwapRB)) {
    return (LCD_PIXELINDEX)Data;
  }
  if (_BitmapSwapRB) {
    return LCD_Color2Index(LCD_Index2Color_M565(Data));
  }
  return LCD_Color2Index(LCD_Index2Color_565(Data));
}

/*********************************************************************
*
*       _DrawBitmap_RLE
*/
static void _DrawBitmap_RLE(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, int xMag, int yMag) {
  LCD_PIXELINDEX OldColorIndex, PixelIndex;
  int xi, y, xL, yL;
  const U8 GUI_UNI_PTR * pPixelOrg;
  char NoTrans, IsMagnified;
  pPixelOrg     = pPixel;
  NoTrans       = !(GUI_Context.DrawMode & LCD_DRAWMODE_TRANS);
  IsMagnified   = ((yMag | xMag) != 1);
  OldColorIndex = LCD_ACOLORINDEX[1];
  _DisplayMode   = LCD_GetFixedPaletteEx(GUI_Context.SelLayer);
  _DisplaySwapRB = LCD_GetSwapRBEx(GUI_Context.SelLayer);
 /* Check if we can limit the number of lines due to clipping) */
  if (yMag == 1) {
    if (ysize > GUI_Context.ClipRect.y1 - y0 + 1) {
      ysize = GUI_Context.ClipRect.y1 - y0 + 1;
    }
  }
  /* Init variables for looping */
  xi = y = 0;
  /* Check if we can use the cache to save some unnecessary iterations */
  if (!IsMagnified) {
    int yDiff = GUI_Context.ClipRect.y0 - y0;
    if ((_Cache.pPixelStart == pPixel) && (yDiff > _Cache.y)) {
      /* Accept cache values */
      y      = _Cache.y;
      xi     = _Cache.x;
      pPixel = _Cache.pPixel;
    }
  }
  /* Init values for caching */
  _Cache.pPixel = _Cache.pPixelStart = pPixelOrg;
  _Cache.x = _Cache.y = 0;
  /* Repeat until we have reached bottom */
  for (; y < ysize; ) {
    U8 NumCompressedPixels = *pPixel++;
    if (NumCompressedPixels) {
      PixelIndex = _GetPixelIndex(&pPixel);
      /* Save cache info */
      _Cache.pPixel = pPixel - 3;
      _Cache.x      = xi;
      _Cache.y      = y;
      LCD_ACOLORINDEX[1] = PixelIndex;
      while (NumCompressedPixels) {
        int xi1 = xi + NumCompressedPixels;
        if (xi1 >= xsize) {
          xi1 = xsize;
        }
        NumCompressedPixels -= (xi1 - xi);
        if (PixelIndex || NoTrans) {  /* Skip transparent pixels */
          if (IsMagnified) {
            xL = xMag * xi + x0;
            yL = yMag * y + y0;
            LCD_FillRect(xL, yL, xL + xMag * (xi1 - xi) -1 , yL + yMag - 1);
          } else {
            LCD_DrawHLine(x0 + xi, y + y0, xi1 + x0 - 1);
          }
        }
        xi = xi1;
        if (xi1 == xsize) {
          y++;
          xi = 0;
        }
      }
    } else {
      U8 NumPixels = *pPixel++;
      do {
        PixelIndex = _GetPixelIndex(&pPixel);
        if (PixelIndex || NoTrans) {  /* Skip transparent pixels */
          int x = x0 + xi;
          if (IsMagnified) {
            LCD_SetColorIndex(PixelIndex);
            xL = xMag * xi + x0;
            yL = yMag * y + y0;
            LCD_FillRect(xL, yL, xL + xMag - 1 , yL + yMag - 1);
          } else {
            if ((y + y0) >= GUI_Context.ClipRect.y0) {
              if (x >= GUI_Context.ClipRect.x0) {
                if (x <= GUI_Context.ClipRect.x1) {
                  LCDDEV_L0_SetPixelIndex(x, y + y0, PixelIndex);
                }
              }
            }
          }
        }
        if (++xi >= xsize) {
          y++;
          xi = 0;
          if (y >= ysize) {
            break;
          }
        }
      } while (--NumPixels);
    }
  }
  LCD_ACOLORINDEX[1] = OldColorIndex;
}

/*********************************************************************
*
*       _DrawBitmap_RLE16
*/
static void _DrawBitmap_RLE16(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  GUI_USE_PARA(pLogPal);
  _BitmapSwapRB = 0;
  _DrawBitmap_RLE(x0, y0, xsize, ysize, pPixel, xMag, yMag);
}

/*********************************************************************
*
*       _DrawBitmap_RLEM16
*/
static void _DrawBitmap_RLEM16(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  GUI_USE_PARA(pLogPal);
  _BitmapSwapRB = 1;
  _DrawBitmap_RLE(x0, y0, xsize, ysize, pPixel, xMag, yMag);
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_BitmapMethodsRLE16
*/
const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE16 = {
  _DrawBitmap_RLE16,
  NULL
};

/*********************************************************************
*
*       GUI_BitmapMethodsRLEM16
*/
const GUI_BITMAP_METHODS GUI_BitmapMethodsRLEM16 = {
  _DrawBitmap_RLEM16,
  NULL
};

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
