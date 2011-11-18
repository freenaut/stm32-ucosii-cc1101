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
File        : EDIT_SetTextMode
Purpose     : Implementation of EDIT_SetTextMode
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "EDIT.h"
#include "GUIDebug.h"
#include "GUI_Protected.h"
#include "EDIT_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*             Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       EDIT_SetTextMode
*/
void EDIT_SetTextMode(EDIT_Handle hEdit) {
  EDIT_Obj* pObj;
  WM_LOCK();
  if (hEdit) {
    pObj = EDIT_H2P(hEdit);
    pObj->pfAddKeyEx    = NULL;
    pObj->pfUpdateBuffer= NULL;
    pObj->CurrentValue  = 0;
    pObj->CursorPos     = 0;
    pObj->Min           = 0;
    pObj->Max           = 0;
    pObj->NumDecs       = 0;
    pObj->Flags         = 0;
    pObj->EditMode      = GUI_EDIT_MODE_INSERT;
    EDIT_SetText(hEdit, "");
    EDIT__SetCursorPos(pObj, pObj->CursorPos);
    WM_Invalidate(hEdit);
  }
  WM_UNLOCK();
}

#else  /* avoid empty object files */

void EDIT_SetTextMode_C(void);
void EDIT_SetTextMode_C(void){}

#endif /* GUI_WINSUPPORT */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
