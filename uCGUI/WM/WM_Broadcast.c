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
File        : WM_Broadcast.C
Purpose     : Windows manager, add. module
----------------------------------------------------------------------
*/

#include "WM_Intern.h"

#if GUI_WINSUPPORT    /* If 0, WM will not generate any code */

/*********************************************************************
*
*                   Broadcast message
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_BroadcastMessage
*
* Note
*   (1) Rather than sending the same message to all windows, we send
*       a copy of the message.
*       This is so because the callback may modify the message, so
*       that without copy only the first window would receive the
*       real, unmodified message.
*/
int WM_BroadcastMessage( WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  WM_LOCK();
  for (hWin = WM__FirstWin; hWin; ) {
    WM_MESSAGE Msg;
    Msg = *pMsg;                  /* Note (1) */
    WM__SendMessage(hWin, &Msg);
    hWin = WM_H2P(hWin)->hNextLin;
  }
  WM_UNLOCK();
  return 0;
}

#else
  void WM_Broadcast(void) {} /* avoid empty object files */
#endif   /* GUI_WINSUPPORT */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
