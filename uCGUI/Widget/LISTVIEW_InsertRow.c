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
File        : LISTVIEW_InsertRow.c
Purpose     : Implementation of LISTVIEW_InsertRow
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
*       LISTVIEW_InsertRow
*/
int LISTVIEW_InsertRow(LISTVIEW_Handle hObj, unsigned Index, const GUI_ConstString * ppText) {
  int r = 0;
  if (hObj) {
    unsigned NumItems;
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    NumItems = GUI_ARRAY_GetNumItems(&pObj->RowArray);
    if (Index < NumItems) {
      /* Insert new row */
      if (GUI_ARRAY_InsertItem(&pObj->RowArray, Index, sizeof(LISTVIEW_ROW))) {
        LISTVIEW_ROW* pRow;
        int i, NumColumns, NumBytes;
        const char* s;
        pObj       = LISTVIEW_H2P(hObj); /* Restore after allocating memory */
        NumColumns = LISTVIEW__GetNumColumns(pObj);
        pRow       = (LISTVIEW_ROW*) GUI_ARRAY_GetpItem(&pObj->RowArray, Index);
        GUI_ARRAY_CREATE(&pRow->CellArray);
        /* Add columns for the new row */
        for (i = 0; i < NumColumns; i++) {
          LISTVIEW_CELL* pCell;
          s = (ppText) ? *ppText++ : 0;
          if (s == 0) {
            ppText = 0;
          }
          NumBytes = GUI__strlen(s) + 1;
          if (GUI_ARRAY_AddItem(&pRow->CellArray, NULL, sizeof(LISTVIEW_CELL) + NumBytes)) {
            r = 1;
            break;
          }
          pRow  = (LISTVIEW_ROW*) GUI_ARRAY_GetpItem(&pObj->RowArray, Index); /* Restore after allocating memory */
          pCell = (LISTVIEW_CELL*) GUI_ARRAY_GetpItem(&pRow->CellArray, i);
          if (NumBytes > 1) {
            strcpy(pCell->acText, s);
          }
        }
        pObj = LISTVIEW_H2P(hObj); /* Restore after allocating memory */
        if (pObj->SortIndex < 0) {
          if ((int)Index < LISTVIEW__GetSel(pObj)) {
            LISTVIEW_IncSel(hObj);
          }
        }
        pObj->IsPresorted = 0;
        if (LISTVIEW__UpdateScrollParas(hObj, pObj)) {
          LISTVIEW__InvalidateInsideArea(hObj, pObj);
        } else {
          if (pObj->hSort && (pObj->SortIndex >= 0)) {
            LISTVIEW__InvalidateInsideArea(hObj, pObj);
          } else {
            LISTVIEW__InvalidateRowAndBelow(hObj, pObj, Index);
          }
        }
      } else {
        r = 1;
      }
    } else {
      /* Append new row */
      r = LISTVIEW_AddRow(hObj, ppText);
    }
    WM_UNLOCK();
  }
  return r;
}


#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_InsertRow_C(void);
  void LISTVIEW_InsertRow_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
