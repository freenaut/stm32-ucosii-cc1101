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
File        : SLIDER_SetFocusColor.c
Purpose     : Implementation of SLIDER_SetFocusColor
---------------------------END-OF-HEADER------------------------------
*/

#include "SLIDER_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       SLIDER_SetFocusColor
*/
GUI_COLOR SLIDER_SetFocusColor(SLIDER_Handle hObj, GUI_COLOR Color) {
  GUI_COLOR OldColor = 0;
  if (hObj) {
    SLIDER_Obj* pObj;
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    if (Color != pObj->Props.FocusColor) {
      OldColor = pObj->Props.FocusColor;
      pObj->Props.FocusColor = Color;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldColor;
}

#else                            /* Avoid problems with empty object modules */
  void SLIDER_SetFocusColor_C(void);
  void SLIDER_SetFocusColor_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
