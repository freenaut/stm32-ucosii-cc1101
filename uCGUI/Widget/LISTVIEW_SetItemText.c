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
File        : LISTVIEW_SetItemText.c
Purpose     : Implementation of LISTVIEW_SetItemText
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "LISTVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW_SetItemText
*/
void LISTVIEW_SetItemText(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, const char * s) {
  if (hObj && s) {
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if ((Column < LISTVIEW__GetNumColumns(pObj)) && (Row < LISTVIEW__GetNumRows(pObj))) {
      LISTVIEW_ROW*  pRow;
      LISTVIEW_CELL* pCell;
      int NumBytes;
      NumBytes = GUI__strlen(s) + 1;
      pRow  = (LISTVIEW_ROW*)  GUI_ARRAY_GetpItem(&pObj->RowArray, Row);
      pCell = (LISTVIEW_CELL*) GUI_ARRAY_ResizeItem(&pRow->CellArray, Column, sizeof(LISTVIEW_CELL) + NumBytes);
      strcpy(pCell->acText, s);
      pObj->IsPresorted = 0;
      LISTVIEW__InvalidateRow(hObj, pObj, Row);
    }
    WM_UNLOCK();
  }
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_SetItemText_C(void);
  void LISTVIEW_SetItemText_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
