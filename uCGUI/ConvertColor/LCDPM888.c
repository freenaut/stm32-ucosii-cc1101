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
File        : LCDPM888.C
Purpose     : Color conversion routines for 888 mode
---------------------------END-OF-HEADER------------------------------
*/

#include "LCD_Protected.h"    /* inter modul definitions */

/*********************************************************************
*
*       Public code,
*
*       LCD_FIXEDPALETTE == 888, 4294967296 colors, RRRRRRRRGGGGGGGGBBBBBBBB
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_Color2Index_M888
*/
unsigned LCD_Color2Index_M888(LCD_COLOR Color) {
  U32 r, g, b;
  b = Color & 0xff;
  g = (Color & 0xff00) >> 8;
  r = (Color & 0xff0000) >> 16;
  return (unsigned)(b << 16) | (g << 8) | r;
}

/*********************************************************************
*
*       LCD_GetIndexMask_M888
*/
unsigned LCD_GetIndexMask_M888(void) {
  return (unsigned)0xffffff;
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
