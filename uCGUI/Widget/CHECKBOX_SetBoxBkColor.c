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
File        : CHECKBOX_SetBoxBkColor.c
Purpose     : Implementation of CHECKBOX_SetBoxBkColor
---------------------------END-OF-HEADER------------------------------
*/

#include "CHECKBOX_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       CHECKBOX_SetBoxBkColor
*/
GUI_COLOR CHECKBOX_SetBoxBkColor(CHECKBOX_Handle hObj, GUI_COLOR Color, int Index) {
  GUI_COLOR OldColor = 0;
  if (hObj) {
    CHECKBOX_Obj* pObj;
    WM_LOCK();
    pObj = CHECKBOX_H2P(hObj);
    if (Index < GUI_COUNTOF(pObj->Props.aBkColorBox)) {
      if (Color != pObj->Props.aBkColorBox[Index]) {
        OldColor = pObj->Props.aBkColorBox[Index];
        pObj->Props.aBkColorBox[Index] = Color;
        WM_InvalidateWindow(hObj);
      }
    }
    WM_UNLOCK();
  }
  return OldColor;
}

#else                            /* Avoid problems with empty object modules */
  void CHECKBOX_SetBoxBkColor_C(void);
  void CHECKBOX_SetBoxBkColor_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
