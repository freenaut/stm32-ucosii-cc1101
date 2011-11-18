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
File        : TEXT_SetWrapMode.c
Purpose     : Implementation of TEXT_SetWrapMode
---------------------------END-OF-HEADER------------------------------
*/

#include "TEXT_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       TEXT_SetWrapMode
*/
void TEXT_SetWrapMode(TEXT_Handle hObj, GUI_WRAPMODE WrapMode) {
  if (hObj) {
    TEXT_Obj * pObj;
    WM_LOCK();
    pObj = TEXT_H2P(hObj);
    if (pObj->Props.WrapMode != WrapMode) {
      pObj->Props.WrapMode = WrapMode;
      WM_Invalidate(hObj);
    }
    WM_UNLOCK();
  }
}

#else  /* avoid empty object files */

void TEXT_SetWrapMode_C(void);
void TEXT_SetWrapMode_C(void){}

#endif  /* #if GUI_WINSUPPORT */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
