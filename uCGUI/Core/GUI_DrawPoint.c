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
File        : GUI_DrawPoint.c
Purpose     : Implementation of point drawing routines
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.h"
#include "GUIDebug.h"

/*********************************************************************
*
*       GL_DrawPoint
*/
void GL_DrawPoint(int x, int y) {
  if (GUI_Context.PenSize == 1) {
    LCD_HL_DrawPixel(x, y);
  } else {
    GL_FillCircle(x, y, (GUI_Context.PenSize - 1) / 2);
  }
}

/*********************************************************************
*
*       GUI_DrawPoint
*/
void GUI_DrawPoint(int x, int y) {
  GUI_LOCK();
  #if (GUI_WINSUPPORT)
    WM_ADDORG(x, y);
    WM_ITERATE_START(NULL); {
  #endif
  GL_DrawPoint(x, y);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
  #endif
  GUI_UNLOCK();
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
