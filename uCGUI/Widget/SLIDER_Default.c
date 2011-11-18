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
File        : SLIDER_Default.c
Purpose     : Implementation of SLIDER widget
---------------------------END-OF-HEADER------------------------------
*/

#include "SLIDER_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       SLIDER_SetDefaultFocusColor
*/
GUI_COLOR SLIDER_SetDefaultFocusColor(GUI_COLOR Color) {
  GUI_COLOR OldColor = SLIDER__DefaultProps.FocusColor;
  SLIDER__DefaultProps.FocusColor = Color;
  return OldColor;
}

#else                            /* Avoid problems with empty object modules */
  void SLIDER_Default_C(void);
  void SLIDER_Default_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
