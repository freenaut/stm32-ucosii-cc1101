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
File        : WM__SendMessage.c
Purpose     : Implementation of WM__SendMessage
----------------------------------------------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "WM_Intern.h"

#if GUI_WINSUPPORT    /* If 0, WM will not generate any code */

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#ifndef   GUI_MAX_MESSAGE_NESTING
  #define GUI_MAX_MESSAGE_NESTING 20
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       WM__SendMessage
*/
void WM__SendMessage(WM_HWIN hWin, WM_MESSAGE* pMsg) {
  static int _EntranceCnt;
  WM_Obj* pWin;
  if (_EntranceCnt < GUI_MAX_MESSAGE_NESTING) {
    pWin = WM_HANDLE2PTR(hWin);
    pMsg->hWin = hWin;
    if (pWin->cb != NULL) {
      _EntranceCnt++;
      (*pWin->cb)(pMsg);
      _EntranceCnt--;
    } else {
      WM_DefaultProc(pMsg);
    }
  }
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_PARA
  else {
    GUI_DEBUG_ERROROUT("Max. message nesting exceeded, Message skipped."); 
  }
  #endif
}

#else
  void WM__SendMessage_c(void);
  void WM__SendMessage_c(void) {} /* avoid empty object files */
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
