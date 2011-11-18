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
File        : WM_InvalidateArea.c
Purpose     : Implementation of WM_InvalidateArea
----------------------------------------------------------------------
*/

#include "WM_Intern.h"

#if GUI_WINSUPPORT    /* If 0, WM will not generate any code */

/*********************************************************************
*
*      Public code
*
**********************************************************************
*/
/*********************************************************************
*
*        WM_InvalidateArea
*
* Purpose:
*   Invalidate a certain section of the display. One main reason for this is
*   that the top window has been moved or destroyed.
*   The coordinates given are absolute coordinates (desktop coordinates)
*/
void WM_InvalidateArea(const GUI_RECT* pRect) {
  WM_HWIN   hWin;
  WM_LOCK();
  /* Iterate over all windows */
  for (hWin = WM__FirstWin; hWin; hWin = WM_H2P(hWin)->hNextLin) {
    WM__Invalidate1Abs(hWin, pRect);
  }
  WM_UNLOCK();
}

#else
  void WM_InvalidateArea_C(void);
  void WM_InvalidateArea_C(void) {} /* avoid empty object files */
#endif   /* GUI_WINSUPPORT */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
