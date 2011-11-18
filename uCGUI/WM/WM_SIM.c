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
File        : WM_SIM.c
Purpose     : Windows manager, add. module
----------------------------------------------------------------------
*/

#include "WM_Intern.h"

#if GUI_WINSUPPORT    /* If 0, WM will not generate any code */

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

#ifdef WIN32
static int _OldSizeX;
static int _OldSizeY;
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       WM_SetScreenSize
*/
void WM_SetScreenSize(int xSize, int ySize) {
  GUI_SetScreenSizeX(xSize);
  GUI_SetScreenSizeY(ySize);
}

/*********************************************************************
*
*       WM_PollSimMsg
*/
#ifdef WIN32
int WM_PollSimMsg(void) {
  int xSize, ySize, r = 0;
  WM_LOCK();
  xSize = GUI_GetScreenSizeX();
  ySize = GUI_GetScreenSizeY();
  if ((xSize != _OldSizeX) || (ySize != _OldSizeY)) {
    WM_MESSAGE Msg;
    Msg.MsgId = WM_SCREENSIZE_CHANGED;
    WM_BroadcastMessage(&Msg);
    _OldSizeX = xSize;
    _OldSizeY = ySize;
    r = 1;
  }
  WM_UNLOCK();
  return r;
}
#endif

#else
  void WM_SIM(void) {} /* avoid empty object files */
#endif   /* GUI_WINSUPPORT */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
