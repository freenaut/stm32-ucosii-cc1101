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
File        : LISTBOX_GetTextAlign.c
Purpose     : Implementation of LISTBOX_GetTextAlign
---------------------------END-OF-HEADER------------------------------
*/

#include "LISTBOX_Private.h"
#include "GUI_Protected.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTBOX_SetTextAlign
*/
int LISTBOX_GetTextAlign(LISTBOX_Handle hObj) {
  int Align = 0;
  if (hObj) {
    LISTBOX_Obj* pObj;
    WM_LOCK();
    pObj = LISTBOX_H2P(hObj);
    Align = pObj->Props.Align;
    WM_Invalidate(hObj);
    WM_UNLOCK();
  }
  return Align;
}

#else  /* avoid empty object files */

void LISTBOX_GetTextAlign_c(void);
void LISTBOX_GetTextAlign_c(void){}

#endif  /* #if GUI_WINSUPPORT */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
