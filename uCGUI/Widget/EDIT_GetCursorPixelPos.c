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
File        : EDIT_GetCursorPixelPos.c
Purpose     : Implementation of edit widget
---------------------------END-OF-HEADER------------------------------
*/

#include "EDIT_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*             Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       EDIT_GetCursorPixelPos
*/
void EDIT_GetCursorPixelPos(EDIT_Handle hObj, int * pxPos, int * pyPos) {
  if (hObj) {
    int NumChars;
    EDIT_Obj * pObj;
    const char GUI_UNI_PTR * pText;
    GUI_RECT rInside, rText;
    WM_LOCK();
    pObj = EDIT_H2P(hObj);
    if (pObj->hpText) {
      GUI_SetFont(pObj->Props.pFont);
      WIDGET__GetInsideRect(&pObj->Widget, &rInside);
      pText       = (const char*) GUI_ALLOC_h2p(pObj->hpText);
      NumChars    = GUI__GetNumChars(pText);
      rInside.x0 += pObj->Props.Border + EDIT_XOFF;
      rInside.x1 -= pObj->Props.Border + EDIT_XOFF;
      GUI__CalcTextRect(pText, &rInside, &rText, pObj->Props.Align);
      rText.x0 += GUI__GetCursorPosX(pText, pObj->CursorPos, NumChars);
      *pxPos = rText.x0;
      *pyPos = rText.y0;
    }
    WM_UNLOCK();
  }
}

#else  /* avoid empty object files */

void EDIT_GetCursorPixelPos_C(void);
void EDIT_GetCursorPixelPos_C(void) {}

#endif /* GUI_WINSUPPORT */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
