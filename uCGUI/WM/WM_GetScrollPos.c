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
File        : WM_GetScrollPos.c
Purpose     : Implementation of WM_GetScrollPosH and WM_GetScrollPosV
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
*       _GetScrollPos
*/
static int _GetScrollPos(WM_HWIN hWin, int Id) {
  WM_SCROLL_STATE ScrollState = {0};
  WM_HWIN hVScroll;
  hVScroll = WM_GetDialogItem(hWin, Id);
  if (hVScroll) {
    WM_GetScrollState(hVScroll, &ScrollState);
  }
  return ScrollState.v;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_GetScrollPosH
*/
int WM_GetScrollPosH(WM_HWIN hWin) {
  int ScrollPos;
  WM_LOCK();
  ScrollPos = _GetScrollPos(hWin, GUI_ID_HSCROLL);
  WM_UNLOCK();
  return ScrollPos;
}

/*********************************************************************
*
*       WM_GetScrollPosV
*/
int WM_GetScrollPosV(WM_HWIN hWin) {
  int ScrollPos;
  WM_LOCK();
  ScrollPos = _GetScrollPos(hWin, GUI_ID_VSCROLL);
  WM_UNLOCK();
  return ScrollPos;
}

#else
  void WM_GetScrollPos_C(void) {} /* avoid empty object files */
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
