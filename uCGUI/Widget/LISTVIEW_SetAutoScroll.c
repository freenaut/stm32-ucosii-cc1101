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
File        : LISTVIEW_SetAutoScroll.c
Purpose     : Implementation of LISTVIEW_SetAutoScrollH() and LISTVIEW_SetAutoScrollV()
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include "GUI_ARRAY.h"
#include "LISTVIEW_Private.h"
#include "SCROLLBAR.h"
#include "GUIDebug.h"
#include "GUI_Protected.h"
#include "WM_Intern.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported routines:  Various methods
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW_SetAutoScrollH
*/
void LISTVIEW_SetAutoScrollH(LISTVIEW_Handle hObj, int State) {
  if (hObj) {
    LISTVIEW_Obj* pObj;
    char Flags;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    Flags = pObj->Flags & (~LISTVIEW_SF_AUTOSCROLLBAR_H);
    if (State) {
      Flags |= LISTVIEW_SF_AUTOSCROLLBAR_H;
    }
    if (pObj->Flags != Flags) {
      pObj->Flags = Flags;
      LISTVIEW__UpdateScrollParas(hObj, pObj);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_SetAutoScrollV
*/
void LISTVIEW_SetAutoScrollV(LISTVIEW_Handle hObj, int State) {
  if (hObj) {
    LISTVIEW_Obj* pObj;
    char Flags;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    Flags = pObj->Flags & (~LISTVIEW_SF_AUTOSCROLLBAR_V);
    if (State) {
      Flags |= LISTVIEW_SF_AUTOSCROLLBAR_V;
    }
    if (pObj->Flags != Flags) {
      pObj->Flags = Flags;
      LISTVIEW__UpdateScrollParas(hObj, pObj);
    }
    WM_UNLOCK();
  }
}


#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_SetAutoScroll_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
