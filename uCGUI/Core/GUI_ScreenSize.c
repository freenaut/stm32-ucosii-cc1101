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
File        : GUI_ScreenSize.c
Purpose     : Implementation of screen size functions
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static int _ScreenSizeX;
static int _ScreenSizeY;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_SetScreenSizeX
*/
void GUI_SetScreenSizeX(int xSize) {
  _ScreenSizeX = xSize;
}

/*********************************************************************
*
*       GUI_SetScreenSizeY
*/
void GUI_SetScreenSizeY(int ySize) {
  _ScreenSizeY = ySize;
}

/*********************************************************************
*
*       GUI_GetScreenSizeX
*/
int GUI_GetScreenSizeX(void) {
  if (!_ScreenSizeX) {
    _ScreenSizeX = LCD_GetXSize();
  }
  return _ScreenSizeX;
}

/*********************************************************************
*
*       GUI_GetScreenSizeY
*/
int GUI_GetScreenSizeY(void) {
  if (!_ScreenSizeY) {
    _ScreenSizeY = LCD_GetYSize();
  }
  return _ScreenSizeY;
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
