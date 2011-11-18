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
File        : WM_SetScrollPos.c
Purpose     : Implementation of WM_SetScrollPos
----------------------------------------------------------------------
*/

#include "WM_Intern.h"

#if GUI_WINSUPPORT    /* If 0, WM will not generate any code */

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetScrollPos
*/
static void _SetScrollPos(WM_HWIN hWin, unsigned ScrollPos, int Id) {
  WM_HWIN hVScroll;
  hVScroll = WM_GetDialogItem(hWin, Id);
  if (hVScroll) {
    int OldValue;
    WM_SCROLL_STATE ScrollState = {0};
    WM_GetScrollState(hVScroll, &ScrollState);
    OldValue = ScrollState.v;
    ScrollState.v = ScrollPos;
    WM_CheckScrollBounds(&ScrollState);
    if (ScrollState.v != OldValue) {
      WM_SetScrollState(hVScroll, &ScrollState);
    }
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_SetScrollPosH
*/
void WM_SetScrollPosH(WM_HWIN hWin, unsigned ScrollPos) {
  WM_LOCK();
  _SetScrollPos(hWin, ScrollPos, GUI_ID_HSCROLL);
  WM_UNLOCK();
}

/*********************************************************************
*
*       WM_SetScrollPosV
*/
void WM_SetScrollPosV(WM_HWIN hWin, unsigned ScrollPos) {
  WM_LOCK();
  _SetScrollPos(hWin, ScrollPos, GUI_ID_VSCROLL);
  WM_UNLOCK();
}

#else
  void WM_SetScrollPos_C(void) {} /* avoid empty object files */
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
