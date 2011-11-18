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
File        : LISTVIEW_DisableRow.c
Purpose     : Implementation of LISTVIEW_DisableRow
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "LISTVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetRowDisabled
*/
static void _SetRowDisabled(LISTVIEW_Handle hObj, unsigned Row, int OnOff) {
  if (hObj) {
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if (Row < LISTVIEW__GetNumRows(pObj)) {
      LISTVIEW_ROW* pRow;
      pRow = (LISTVIEW_ROW*) GUI_ARRAY_GetpItem(&pObj->RowArray, Row);
      pRow->Disabled = OnOff;
      LISTVIEW__InvalidateRow(hObj, pObj, Row);
    }
    WM_UNLOCK();
  }
}
/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW_DisableRow
*/
void LISTVIEW_DisableRow(LISTVIEW_Handle hObj, unsigned Row) {
  _SetRowDisabled(hObj, Row, 1);
}

/*********************************************************************
*
*       LISTVIEW_EnableRow
*/
void LISTVIEW_EnableRow(LISTVIEW_Handle hObj, unsigned Row) {
  _SetRowDisabled(hObj, Row, 0);
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_DisableRow_C(void);
  void LISTVIEW_DisableRow_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
