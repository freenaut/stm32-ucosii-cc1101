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
File        : GUI_DrawPolygon.C
Purpose     : Implementation of polygon drawing routines
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.h"
#include "GUIDebug.h"

/*********************************************************************
*
*       GL_DrawPolygon
*/
void GL_DrawPolygon(const GUI_POINT*pPoints, int NumPoints, int x0, int y0) {
  const GUI_POINT* pPoint = pPoints;
  GL_MoveTo(pPoint->x+x0, pPoint->y+y0);
  while (--NumPoints >0) {
    pPoint++;
    GL_DrawLineTo(pPoint->x+x0, pPoint->y+y0);
  }
  /* Now draw closing line unless it has already been closed */
  if ( (pPoint->x != pPoints->x)
     ||(pPoint->y != pPoints->y))
  {
    GL_DrawLineTo(pPoints->x+x0, pPoints->y+y0);
  }
}

/*********************************************************************
*
*       GUI_DrawPolygon
*/
void GUI_DrawPolygon(const GUI_POINT* pPoints, int NumPoints, int x0, int y0) {
  GUI_LOCK();
  #if (GUI_WINSUPPORT)
    WM_ADDORG(x0,y0);
    WM_ITERATE_START(NULL); {
  #endif
  GL_DrawPolygon (pPoints, NumPoints, x0, y0);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
  #endif
  GUI_UNLOCK();
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
