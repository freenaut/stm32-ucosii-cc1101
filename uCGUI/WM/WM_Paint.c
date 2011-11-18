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
File        : WM_Paint.c
Purpose     : Windows manager, add. module
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "WM_Intern.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_Update
*/
void WM_Update(WM_HWIN hWin) {
  GUI_CONTEXT Context;
  WM_ASSERT_NOT_IN_PAINT();
  if (hWin) {
    WM_Obj* pWin;
    WM_LOCK();
    GUI_SaveContext(&Context);
    pWin = WM_H2P(hWin);
    WM__Paint(hWin, pWin);
    GUI_RestoreContext(&Context);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       WM_Paint
*/
void WM_Paint(WM_HWIN hWin) {
  WM_InvalidateWindow(hWin);
  WM_Update(hWin);
}

#else
  void WM_Paint(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
