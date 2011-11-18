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
File        : LISTVIEW_SetItemColor.c
Purpose     : Implementation of LISTVIEW_SetItemTextColor and LISTVIEW_SetItemBkColor
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "LISTVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       _GetpCellInfo
*/
static LISTVIEW_CELL_INFO* _GetpCellInfo(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, unsigned int Index) {
  LISTVIEW_CELL_INFO* pCellInfo = 0;
  if (hObj) {
    LISTVIEW_Obj* pObj;
    pObj = LISTVIEW_H2P(hObj);
    if (Index < GUI_COUNTOF(pCellInfo->aTextColor)) {
      if ((Column < LISTVIEW__GetNumColumns(pObj)) && (Row < LISTVIEW__GetNumRows(pObj))) {
        LISTVIEW_ROW*  pRow;
        LISTVIEW_CELL* pCell;
        pRow  = (LISTVIEW_ROW*)  GUI_ARRAY_GetpItem(&pObj->RowArray,  Row);
        pCell = (LISTVIEW_CELL*) GUI_ARRAY_GetpItem(&pRow->CellArray, Column);
        if (!pCell->hCellInfo) {
          int i;
          pCell->hCellInfo = GUI_ALLOC_AllocZero(sizeof(LISTVIEW_CELL_INFO));
          pCellInfo = (LISTVIEW_CELL_INFO*) GUI_ALLOC_h2p(pCell->hCellInfo);
          for (i = 0; i < GUI_COUNTOF(pCellInfo->aTextColor); i++) {
            pCellInfo->aTextColor[i] = LISTVIEW_GetTextColor(hObj, i);
            pCellInfo->aBkColor[i]   = LISTVIEW_GetBkColor  (hObj, i);
          }
        } else {
          pCellInfo = (LISTVIEW_CELL_INFO*) GUI_ALLOC_h2p(pCell->hCellInfo);
        }
      }
    }
  }
  return pCellInfo;
}

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW_SetItemTextColor
*/
void LISTVIEW_SetItemTextColor(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, unsigned int Index, GUI_COLOR Color) {
  LISTVIEW_CELL_INFO* pCellInfo;
  WM_LOCK();
  pCellInfo = _GetpCellInfo(hObj, Column, Row, Index);
  if (pCellInfo) {
    pCellInfo->aTextColor[Index] = Color;
  }
  WM_UNLOCK();
}

/*********************************************************************
*
*       LISTVIEW_SetItemBkColor
*/
void LISTVIEW_SetItemBkColor(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, unsigned int Index, GUI_COLOR Color) {
  LISTVIEW_CELL_INFO* pCellInfo;
  WM_LOCK();
  pCellInfo = _GetpCellInfo(hObj, Column, Row, Index);
  if (pCellInfo) {
    pCellInfo->aBkColor[Index] = Color;
  }
  WM_UNLOCK();
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_SetItemColor_C(void);
  void LISTVIEW_SetItemColor_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
