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
File        : TEXT_Default.c
Purpose     : Implementation of TEXT widget
---------------------------END-OF-HEADER------------------------------
*/

#include "TEXT_Private.h"
#include "GUI_Protected.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       TEXT_SetDefaultFont
*/
void TEXT_SetDefaultFont(const GUI_FONT GUI_UNI_PTR * pFont) {
  TEXT__DefaultProps.pFont = pFont;
}

/*********************************************************************
*
*       TEXT_SetDefaultTextColor
*/
void TEXT_SetDefaultTextColor(GUI_COLOR Color) {
  TEXT__DefaultProps.TextColor = Color;
}

/*********************************************************************
*
*       TEXT_GetDefaultFont
*/
const GUI_FONT GUI_UNI_PTR * TEXT_GetDefaultFont(void) {
  return TEXT__DefaultProps.pFont;
}

/*********************************************************************
*
*       TEXT_SetDefaultWrapMode
*/
GUI_WRAPMODE TEXT_SetDefaultWrapMode(GUI_WRAPMODE WrapMode) {
  GUI_WRAPMODE OldWrapMode;
  OldWrapMode = TEXT__DefaultProps.WrapMode;
  TEXT__DefaultProps.WrapMode = WrapMode;
  return OldWrapMode;
}

#else  /* avoid empty object files */

void TEXT_Default_c(void);
void TEXT_Default_c(void){}

#endif  /* #if GUI_WINSUPPORT */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
