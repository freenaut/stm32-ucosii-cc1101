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
File        : GUI2DLib.C
Purpose     : Main part of the 2D graphics library
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.h"
#include "GUIDebug.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_MoveRel
*/
void GUI_MoveRel(int dx, int dy) { /*tbd: GL_LinePos. */
  GUI_LOCK();
  GUI_Context.DrawPosX += dx;
  GUI_Context.DrawPosY += dy;
  GUI_UNLOCK();
}

/*********************************************************************
*
*       GL_MoveTo
*/
void GL_MoveTo(int x, int y) {
  GUI_Context.DrawPosX = x;
  GUI_Context.DrawPosY = y;
}

/*********************************************************************
*
*       GUI_MoveTo
*/
void GUI_MoveTo(int x, int y) {
  GUI_LOCK();
  #if (GUI_WINSUPPORT)
    WM_ADDORG(x,y);
  #endif
  GL_MoveTo(x,y);
  GUI_UNLOCK();
}

/*********************************************************************
*
*       Rectangle filling / inverting
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawRect
*/
static void _DrawRect(int x0, int y0, int x1, int y1) {
  LCD_DrawHLine(x0, y0, x1);
  LCD_DrawHLine(x0, y1, x1);
  LCD_DrawVLine(x0, y0 + 1, y1 - 1);
  LCD_DrawVLine(x1, y0 + 1, y1 - 1);
}

/*********************************************************************
*
*       GUI_DrawRect
*/
void GUI_DrawRect(int x0, int y0, int x1, int y1) {
  #if (GUI_WINSUPPORT)
    int Off;
    GUI_RECT r;
  #endif
  GUI_LOCK();
  #if (GUI_WINSUPPORT)
    Off = GUI_Context.PenSize -1;
    WM_ADDORG(x0,y0);
    WM_ADDORG(x1,y1);
    r.x0 = x0 - Off;
    r.x1 = x1 + Off;
    r.y0 = y0 - Off;
    r.y1 = y1 + Off;
    WM_ITERATE_START(&r); {
  #endif
  _DrawRect(x0, y0, x1, y1);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
  #endif
  GUI_UNLOCK();
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
