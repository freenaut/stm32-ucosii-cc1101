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
File        : GUI_DispStringInRectWrap.c
Purpose     : Implementation of GUI_DispStringInRectWrap
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"
 
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_DispStringInRectWrap
*/
void GUI_DispStringInRectWrap(const char GUI_UNI_PTR * s, GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode) {
  const GUI_RECT * pOldClipRect;
  GUI_RECT Rect;
  int xSize, ySizeFont, NumBytesToNextLine, NumCharsDisp;
  if (!s) {
    return;
  }
  GUI_LOCK();
  Rect      = *pRect;
  xSize     = Rect.x1 - Rect.x0 + 1;
  ySizeFont = GUI_GetFontDistY();
  /* Vertical alignment */
  if ((TextAlign & GUI_TA_VERTICAL) != GUI_TA_TOP) {
    int NumLines = 0;
    const char GUI_UNI_PTR * pText;
    pText = s;
    /* Count the number of lines */
    do {
      NumBytesToNextLine = GUI__WrapGetNumBytesToNextLine(pText, xSize, WrapMode);
      pText += NumBytesToNextLine;
      if (NumBytesToNextLine) {
        NumLines++;
      }
    } while (NumBytesToNextLine);
    /* Do the vertical alignment */
    switch (TextAlign & GUI_TA_VERTICAL) {
	  case GUI_TA_BASELINE:
	  case GUI_TA_BOTTOM:
      Rect.y0 = Rect.y1 - NumLines * ySizeFont + 1;
      break;
	  case GUI_TA_VCENTER:
		  Rect.y0 = Rect.y0 + (Rect.y1 - Rect.y0 + 1 - NumLines * ySizeFont) / 2;
      break;
	  }
  }
  pOldClipRect = GUI_SetClipRect(pRect);
  do {
    int xLineSize;
    GUI_RECT RectText;
    NumCharsDisp = GUI__WrapGetNumCharsDisp(s, xSize, WrapMode);
    Rect.y1      = Rect.y0 + ySizeFont - 1;
    RectText     = Rect;
    /* Horizontal alignment */
    xLineSize = GUI__GetLineDistX(s, NumCharsDisp);
    switch (TextAlign & GUI_TA_HORIZONTAL) {
    case GUI_TA_HCENTER:
      RectText.x0 = pRect->x0 + (pRect->x1 - pRect->x0 - xLineSize) / 2;
      break;
    case GUI_TA_LEFT:
      RectText.x0 = pRect->x0;
      break;
    case GUI_TA_RIGHT:
      RectText.x0 = pRect->x1 - xLineSize + 1;
      break;
    }
    GUI__DispLine(s, NumCharsDisp, &RectText);
    s += GUI__WrapGetNumBytesToNextLine(s, xSize, WrapMode);
    Rect.y0 += ySizeFont;
  } while (*s);
  GUI_SetClipRect(pOldClipRect);
  GUI_UNLOCK();
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
