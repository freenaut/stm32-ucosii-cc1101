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
File        : LCD_ReadRect.c
Purpose     : Implementation of LCD_ReadRect
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Private.h"
#include "LCD_Private.h"      /* Required for configuration, APIList */

#if GUI_SUPPORT_DEVICES

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_ReadRect
*/
void LCD_ReadRect(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer, const tLCDDEV_APIList * pAPI) {
  void (* pfReadRect)(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer);
  pfReadRect = (void (*)(int, int, int, int, LCD_PIXELINDEX *))pAPI->pfGetDevFunc(LCD_DEVFUNC_READRECT);
  if (!pfReadRect) {
    while (y0 <= y1) {
      int x;
      for (x = x0; x <= x1; x++) {
        *pBuffer++ = pAPI->pfGetPixelIndex(x, y0);
      }
      y0++;
    }
  } else {
    pfReadRect(x0, y0, x1, y1, pBuffer);
  }
}

#else
  void LCD_ReadRect_C(void);
  void LCD_ReadRect_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
