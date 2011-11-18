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
File        : EDIT_SetFocussable.c
Purpose     : Implementation of EDIT_SetFocussable
---------------------------END-OF-HEADER------------------------------
*/

#include "EDIT.h"
#include "EDIT_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       EDIT_SetFocussable
*/
void EDIT_SetFocussable(EDIT_Handle hObj, int State) {
  if (State) {
    WIDGET_OrState(hObj, WIDGET_STATE_FOCUSSABLE);
  } else {
    WIDGET_AndState(hObj, WIDGET_STATE_FOCUSSABLE);
  }
}

#else  /* avoid empty object files */

void EDIT_SetFocussable_C(void);
void EDIT_SetFocussable_C(void){}

#endif /* GUI_WINSUPPORT */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
