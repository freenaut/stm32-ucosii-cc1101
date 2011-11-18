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
File        : DROPDOWN_SetScrollbarWidth.c
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
*       DROPDOWN_SetScrollbarWidth
*/
void DROPDOWN_SetScrollbarWidth(DROPDOWN_Handle hObj, unsigned Width) {
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    if (Width != (unsigned)pObj->ScrollbarWidth) {
      pObj->ScrollbarWidth = Width;
      if (pObj->hListWin) {
        LISTBOX_SetScrollbarWidth(pObj->hListWin, Width);
      }
    }
    WM_UNLOCK();
  }  
}

#else                            /* Avoid problems with empty object modules */
  void DROPDOWN_SetScrollbarWidth_C(void);
  void DROPDOWN_SetScrollbarWidth_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
