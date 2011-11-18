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
File        : LCD_DrawBitmap_565.c
Purpose     : Methods for drawing a 16bpp bitmap
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Private.h"

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
static void _DrawBitmap(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  GUI__DrawBitmap16bpp(x0, y0, xsize, ysize, pPixel, pLogPal, xMag, yMag, GUI_BitmapMethods565.pfIndex2Color);
}

/*********************************************************************
*
*       _DrawBitmapSwapRB
*/
static void _DrawBitmapSwapRB(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag) {
  GUI__DrawBitmap16bpp(x0, y0, xsize, ysize, pPixel, pLogPal, xMag, yMag, GUI_BitmapMethodsM565.pfIndex2Color);
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_BitmapMethods565
*/
const GUI_BITMAP_METHODS GUI_BitmapMethods565 = {
  _DrawBitmap,
  LCD_Index2Color_565
};

/*********************************************************************
*
*       GUI_BitmapMethodsM565
*/
const GUI_BITMAP_METHODS GUI_BitmapMethodsM565 = {
  _DrawBitmapSwapRB,
  LCD_Index2Color_M565
};

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
