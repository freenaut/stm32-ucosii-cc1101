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
File        : LISTVIEW_DeleteRow.c
Purpose     : Implementation of LISTVIEW_DeleteRow
---------------------------END-OF-HEADER------------------------------
*/

#include "LISTVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW__InvalidateRowAndBelow
*/
void LISTVIEW__InvalidateRowAndBelow(LISTVIEW_Handle hObj, LISTVIEW_Obj* pObj, int Sel) {
  if (Sel >= 0) {
    GUI_RECT Rect;
    int HeaderHeight, RowDistY;
    HeaderHeight = HEADER_GetHeight(pObj->hHeader);
    RowDistY     = LISTVIEW__GetRowDistY(pObj);
    WM_GetInsideRectExScrollbar(hObj, &Rect);
    Rect.y0 += HeaderHeight + (Sel - pObj->ScrollStateV.v) * RowDistY;
    WM_InvalidateRect(hObj, &Rect);
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
*       LISTVIEW_DeleteRow
*/
void LISTVIEW_DeleteRow(LISTVIEW_Handle hObj, unsigned Index) {
  if (hObj) {
    LISTVIEW_Obj* pObj;
    unsigned NumRows;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    NumRows = LISTVIEW__GetNumRows(pObj);
    if (Index < NumRows) {
      unsigned NumColumns, i;
      LISTVIEW_ROW* pRow;
      pRow = (LISTVIEW_ROW*) GUI_ARRAY_GetpItem(&pObj->RowArray, Index);
      /* Delete attached info items */
      NumColumns = LISTVIEW__GetNumColumns(pObj);
      for (i = 0; i < NumColumns; i++) {
        LISTVIEW_CELL* pCell;
        pCell = (LISTVIEW_CELL*) GUI_ARRAY_GetpItem(&pRow->CellArray, i);
        if (pCell) {
          if (pCell->hCellInfo) {
            GUI_ALLOC_Free(pCell->hCellInfo);
          }
        }
      }
      /* Delete row */
      GUI_ARRAY_Delete(&pRow->CellArray);
      GUI_ARRAY_DeleteItem(&pObj->RowArray, Index);
      /* Adjust properties */
      if (pObj->Sel == (int)Index) {
        pObj->Sel = -1;
      } else if (pObj->SortIndex < 0) {
        if (pObj->Sel > (int)Index) {
          pObj->Sel--;
        }
      } else {
        while (pObj->Sel >= (int)LISTVIEW__GetNumRows(pObj)) {
          pObj->Sel = -1;
        }
      }
      pObj->IsPresorted = 0;
      if (LISTVIEW__UpdateScrollParas(hObj, pObj)) {
        LISTVIEW__InvalidateInsideArea(hObj, pObj);
      } else {
        LISTVIEW__InvalidateRowAndBelow(hObj, pObj, Index);
      }
    }
    WM_UNLOCK();
  }
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_DeleteRow_C(void);
  void LISTVIEW_DeleteRow_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
