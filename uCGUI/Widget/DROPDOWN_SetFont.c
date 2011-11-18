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
File        : DROPDOWN_SetFont.c
Purpose     : Implementation of dropdown widget
---------------------------END-OF-HEADER------------------------------
*/

#include "LISTBOX.h"
#include "DROPDOWN_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN_SetFont
*/
void DROPDOWN_SetFont(DROPDOWN_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont) {
  int OldHeight;
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    OldHeight = GUI_GetYDistOfFont(pObj->Props.pFont);
    pObj->Props.pFont = pfont;
    DROPDOWN__AdjustHeight(hObj, pObj);
    DROPDOWN_Invalidate(hObj);
    if (pObj->hListWin) {
      if (OldHeight != GUI_GetYDistOfFont(pObj->Props.pFont)) {
        DROPDOWN_Collapse(hObj);
        DROPDOWN_Expand(hObj);
      }
      LISTBOX_SetFont(pObj->hListWin, pfont);
    }
    WM_UNLOCK();
  }
}

#else                            /* Avoid problems with empty object modules */
  void DROPDOWN_SetFont_C(void);
  void DROPDOWN_SetFont_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
