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
File        : DROPDOWN_Default.c
Purpose     : Implementation of dropdown widget
---------------------------END-OF-HEADER------------------------------
*/

#include "DROPDOWN_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN_SetDefaultFont
*/
void DROPDOWN_SetDefaultFont(const GUI_FONT GUI_UNI_PTR * pFont) {
  DROPDOWN__DefaultProps.pFont = pFont;
}

/*********************************************************************
*
*       DROPDOWN_GetDefaultFont
*/
const GUI_FONT GUI_UNI_PTR * DROPDOWN_GetDefaultFont(void) {
  return DROPDOWN__DefaultProps.pFont;
}

/*********************************************************************
*
*       DROPDOWN_SetDefaultColor
*/
GUI_COLOR DROPDOWN_SetDefaultColor(int Index, GUI_COLOR Color) {
  GUI_COLOR OldColor = 0;
  if (Index < GUI_COUNTOF(DROPDOWN__DefaultProps.aColor)) {
    OldColor = DROPDOWN__DefaultProps.aColor[Index];
    DROPDOWN__DefaultProps.aColor[Index] = Color;
  }
  return OldColor;
}

/*********************************************************************
*
*       DROPDOWN_SetDefaultScrollbarColor
*/
GUI_COLOR DROPDOWN_SetDefaultScrollbarColor(int Index, GUI_COLOR Color) {
  GUI_COLOR OldColor = 0;
  if (Index < GUI_COUNTOF(DROPDOWN__DefaultProps.aScrollbarColor)) {
    OldColor = DROPDOWN__DefaultProps.aScrollbarColor[Index];
    DROPDOWN__DefaultProps.aScrollbarColor[Index] = Color;
  }
  return OldColor;
}

#else                            /* Avoid problems with empty object modules */
  void DROPDOWN_Default_C(void);
  void DROPDOWN_Default_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
