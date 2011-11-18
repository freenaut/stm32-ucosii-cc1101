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
File        : LISTVIEW_SetSort.c
Purpose     : Implementation of LISTVIEW_SetSort
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>
#include "LISTVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  LISTVIEW_Obj  * pObj;
  LISTVIEW_SORT * pSort;
  SORT_TYPE     * paSortArray;
} SORT_OBJECT;

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _Free
*
* Purpose:
*   Frees the memory used by the attached LISTVIEW_SORT object.
*
* Parameter:
*   hObj - Handle of LISTVIEW_SORT object
*/
static void _Free(WM_HMEM hObj) {
  LISTVIEW_SORT * pSort;
  pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(hObj);
  GUI_ALLOC_Free(pSort->hSortArray);
  GUI_ALLOC_Free(hObj);
}

/*********************************************************************
*
*       _GetSelUnsorted
*/
static int _GetSelUnsorted(LISTVIEW_Obj * pObj) {
  int r;
  if ((LISTVIEW__GetSel(pObj) >= 0) && (pObj->hSort)) {
    LISTVIEW_SORT * pSort;
    pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(pObj->hSort);
    if (!pSort->SortArrayNumItems) {
      r = LISTVIEW__GetSel(pObj);
    } else {
      SORT_TYPE * paSortArray;
      paSortArray = (SORT_TYPE *)GUI_ALLOC_h2p(pSort->hSortArray);
      r = *(paSortArray + LISTVIEW__GetSel(pObj));
    }
  } else {
    r = LISTVIEW__GetSel(pObj);
  }
  return r;
}

/*********************************************************************
*
*       _SetSelUnsorted
*/
static void _SetSelUnsorted(LISTVIEW_Handle hObj, LISTVIEW_Obj * pObj, int Sel) {
  int i, NumItems;
  if (pObj->hSort) {
    LISTVIEW_SORT * pSort;
    SORT_TYPE * paSortArray;
    pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(pObj->hSort);
    NumItems = pSort->SortArrayNumItems;
    paSortArray = (SORT_TYPE *)GUI_ALLOC_h2p(pSort->hSortArray);
    for (i = 0; i < NumItems; i++) {
      if (*(paSortArray + i) == Sel) {
        LISTVIEW__SetSel(hObj, pObj, i);
        break;
      }
    }
  } else {
    LISTVIEW__SetSel(hObj, pObj, Sel);
  }
}

/*********************************************************************
*
*       _Compare
*/
static int _Compare(unsigned lb, unsigned ub, SORT_OBJECT * pSortObject) {
  char * p0, * p1;
  LISTVIEW_ROW  * pRow0, * pRow1;
  LISTVIEW_COLUMN * pColumn;
  SORT_TYPE i0, i1;
  int Result;
  i0 = *(pSortObject->paSortArray + lb);
  i1 = *(pSortObject->paSortArray + ub);
  pRow0   = (LISTVIEW_ROW *)   GUI_ARRAY_GetpItem(&pSortObject->pObj->RowArray,  i0);
  pRow1   = (LISTVIEW_ROW *)   GUI_ARRAY_GetpItem(&pSortObject->pObj->RowArray,  i1);
  p0      = ((LISTVIEW_CELL *) GUI_ARRAY_GetpItem(&pRow0->CellArray, pSortObject->pObj->SortIndex))->acText;
  p1      = ((LISTVIEW_CELL *) GUI_ARRAY_GetpItem(&pRow1->CellArray, pSortObject->pObj->SortIndex))->acText;
  pColumn = (LISTVIEW_COLUMN *)GUI_ARRAY_GetpItem(&pSortObject->pObj->ColumnArray, pSortObject->pObj->SortIndex);
  if (!pColumn) {
    return 0;
  }
  Result = pColumn->fpCompare(p0, p1);
  if (pSortObject->pSort->Reverse) {
    Result *= -1;
  }
  return Result;
}

/*********************************************************************
*
*       _Swap
*/
static void _Swap(unsigned lb, unsigned ub, SORT_OBJECT * pSortObject) {
  int Temp;
  Temp = *(pSortObject->paSortArray + lb);
  *(pSortObject->paSortArray + lb) = *(pSortObject->paSortArray + ub);
  *(pSortObject->paSortArray + ub) = Temp;
}

/*********************************************************************
*
*       _BubbleSort
*
* Purpose:
*   Bubble sort algorithm.
*/
static void _BubbleSort(unsigned lb, unsigned ub, SORT_OBJECT * pSortObject) {
  int Swapped;
  do {
    unsigned i;
    Swapped = 0;
    for (i = ub; i > lb; i--) {
      if (_Compare(i - 1, i, pSortObject) > 0) {
        _Swap(i - 1, i, pSortObject);
        Swapped = 1;
      }
    }
  } while (Swapped);
}

/*********************************************************************
*
*       _Reverse
*
* Purpose:
*   Reverse the current sorting order
*/
static void _Reverse(SORT_OBJECT * pSortObject) {
  int i, NumItems;
  NumItems = pSortObject->pSort->SortArrayNumItems;
  for (i = 0; i < NumItems / 2; i++) {
    int Temp;
    Temp                                           = *(pSortObject->paSortArray + i);
    *(pSortObject->paSortArray + i)                = *(pSortObject->paSortArray + NumItems - i - 1);
    *(pSortObject->paSortArray + NumItems - i - 1) = Temp;
  }
}

/*********************************************************************
*
*       _Sort
*
* Purpose:
*   Sorts the contents of the LISTVIEW by using the qsort algorithm.
*   The compare function is called for each compare operation with valid
*   pointers to cell data of the specified column.
*/
static int _Sort(LISTVIEW_Handle hObj) {
  WM_HMEM hSortArray;
  SORT_OBJECT SortObject;
  int NumItems, NumItemsReq, i, Sel;
  SortObject.pObj = LISTVIEW_H2P(hObj);
  if (((SortObject.pObj->IsPresorted) && (SortObject.pObj->IsSorted)) || (SortObject.pObj->hSort == 0)) {
    return 0;
  }
  SortObject.pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(SortObject.pObj->hSort);
  NumItemsReq = SortObject.pObj->RowArray.NumItems;
  NumItems    = SortObject.pSort->SortArrayNumItems;
  Sel = _GetSelUnsorted(SortObject.pObj);
  /* Adjust number of items in sort array */
  if (NumItems != NumItemsReq) {
    if (!SortObject.pSort->hSortArray) {
      hSortArray = GUI_ALLOC_AllocZero(sizeof(SORT_TYPE) * NumItemsReq);
    } else {
      hSortArray = GUI_ALLOC_Realloc(SortObject.pSort->hSortArray, sizeof(SORT_TYPE) * NumItemsReq);
    }
    if (!hSortArray) {
      return 1;
    }
    SortObject.pObj = LISTVIEW_H2P(hObj);
    SortObject.pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(SortObject.pObj->hSort);
    SortObject.pSort->hSortArray = hSortArray;
  }
  SortObject.paSortArray = (SORT_TYPE *)GUI_ALLOC_h2p(SortObject.pSort->hSortArray);
  if (SortObject.pObj->IsPresorted) {
    /* Add new indices */
    if (NumItems < NumItemsReq) {
      SortObject.pObj->ReverseSort = 0; /* Reverse sort only allowed if listview is presorted and no rows are added */
      for (i = NumItems; i < NumItemsReq; i++) {
        *(SortObject.paSortArray + i) = i;
      }
    }
  } else {
    SortObject.pObj->ReverseSort = 0; /* Reverse sort only allowed if listview is presorted */
    /* Fill with indices if not presorted */
    for (i = 0; i < NumItemsReq; i++) {
      *(SortObject.paSortArray + i) = i;
    }
  }
  SortObject.pSort->SortArrayNumItems = NumItemsReq;
  /* Sort only if more than one item is available */
  if (NumItemsReq > 1) {
    if (SortObject.pObj->ReverseSort) {
      _Reverse(&SortObject);
    } else {
      _BubbleSort(0, NumItemsReq - 1, &SortObject);
    }
    _SetSelUnsorted(hObj, SortObject.pObj, Sel);
  }
  SortObject.pObj->IsPresorted = 1;
  SortObject.pObj->IsSorted    = 1;
  HEADER_SetDirIndicator(SortObject.pObj->hHeader, SortObject.pObj->SortIndex, SortObject.pSort->Reverse);
  return 0;
}

/*********************************************************************
*
*       _CreateSortObject
*/
static LISTVIEW_SORT * _CreateSortObject(LISTVIEW_Handle hObj, LISTVIEW_Obj * pObj) {
  LISTVIEW_SORT * pSort;
  if (!pObj->hSort) {
    pObj->hSort = GUI_ALLOC_AllocZero(sizeof(LISTVIEW_SORT));
    pObj = LISTVIEW_H2P(hObj); /* Restore after allocating memory */
    if (!pObj->hSort) {
      return NULL;
    }
  }
  pSort = (LISTVIEW_SORT *)GUI_ALLOC_h2p(pObj->hSort);
  pObj->SortIndex      = -1;
  pSort->fpFree        = _Free;
  pSort->fpSort        = _Sort;
  return pSort;
}

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       LISTVIEW_CompareText
*/
int LISTVIEW_CompareText(const void * p0, const void * p1) {
  return strcmp((const char *)p1, (const char *)p0);
}

/*********************************************************************
*
*       LISTVIEW_CompareDec
*/
int LISTVIEW_CompareDec(const void * p0, const void * p1) {
  int v[2] = {0}, i, d;
  const char * ps[2];
  char c;
  ps[0] = (const char *)p0;
  ps[1] = (const char *)p1;
  for (i = 0; i < 2; i++) {
    int Neg = 0;
    if (*ps[i] == '-') {
      Neg = 1;
      ps[i]++;
    }
    while ((c = *ps[i]++) != 0) {
      d = c - '0';
      if ((d < 0) || (d > 9)) {
        break;
      }
      v[i] *= 10;
      v[i] += d;
    }
    if (Neg) {
      v[i] *= -1;
    }
  }
  if (v[0] > v[1]) {
    return -1;
  } else if (v[0] < v[1]) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       LISTVIEW_GetSelUnsorted
*
* Purpose:
*   If sorting is disabled, the behaviour is the same as LISTVIEW_GetSel().
*   It returns the 'visible' selection index.
*   If sorting is enabled, the function returns the 'unselected' index of
*   the selected row.
*/
int LISTVIEW_GetSelUnsorted(LISTVIEW_Handle hObj) {
  int r = -1;
  if (hObj) {
    LISTVIEW_Obj * pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    _Sort(hObj);
    r = _GetSelUnsorted(pObj);
    WM_UNLOCK();
  }
  return r;
}

/*********************************************************************
*
*       LISTVIEW_SetSelUnsorted
*
* Purpose:
*   If sorting is disabled, the behaviour is the same as LISTVIEW_SetSel().
*   It sets the 'visible' selection index.
*   If sorting is enabled, the function sets the 'unselected' index of the selected
*   the selected row.
*/
void LISTVIEW_SetSelUnsorted(LISTVIEW_Handle hObj, int Sel) {
  if (hObj) {
    LISTVIEW_Obj * pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    _Sort(hObj);
    _SetSelUnsorted(hObj, pObj, Sel);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_SetCompareFunc
*
* Purpose:
*   Sets the compare function for the given column, which is called by
*   the sorting algorithm if sorting is enabled.
*/
void LISTVIEW_SetCompareFunc(LISTVIEW_Handle hObj, unsigned Column, int (* fpCompare)(const void * p0, const void * p1)) {
  int NumColumns;
  LISTVIEW_Obj    * pObj;
  LISTVIEW_COLUMN * pColumn;
  if (hObj) {
    WM_LOCK();
    pObj       = LISTVIEW_H2P(hObj);
    NumColumns = LISTVIEW_GetNumColumns(hObj);
    if ((int)Column <= NumColumns) {
      pColumn = (LISTVIEW_COLUMN *)GUI_ARRAY_GetpItem(&pObj->ColumnArray, Column);
      pColumn->fpCompare = fpCompare;
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_DisableSort
*
* Purpose:
*   Disables sorting for the given listview object.
*/
void LISTVIEW_DisableSort(LISTVIEW_Handle hObj) {
  if (hObj) {
    LISTVIEW_Obj * pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if (pObj->hSort) {
      int Sel = _GetSelUnsorted(pObj);
      _Free(pObj->hSort);
      pObj->hSort       = 0;
      pObj->SortIndex   = -1;
      pObj->IsPresorted = 0;
      pObj->IsSorted    = 0;
      HEADER_SetDirIndicator(pObj->hHeader, -1, 0);
      LISTVIEW_SetSel(hObj, Sel);
      LISTVIEW__InvalidateInsideArea(hObj, pObj);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_EnableSort
*
* Purpose:
*   Enables sorting for the given listview object.
*/
void LISTVIEW_EnableSort(LISTVIEW_Handle hObj) {
  if (hObj) {
    LISTVIEW_Obj * pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if (!pObj->hSort) {
      _CreateSortObject(hObj, pObj);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_SetSort
*
* Purpose:
*   Enables sorting for the given listview object. The function returns,
*   if the object is 0, the column index is >= number of columns or if
*   no compare function has been set for the desired column
*
* Return value:
*   0 if succeed
*   1 if not succeed
*/
unsigned LISTVIEW_SetSort(LISTVIEW_Handle hObj, unsigned Column, unsigned Reverse) {
  LISTVIEW_Obj    * pObj;
  LISTVIEW_SORT   * pSort;
  LISTVIEW_COLUMN * pColumn;
  unsigned NumColumns, Error = 0;
  if (!hObj) {
    return 1;
  }
  NumColumns = LISTVIEW_GetNumColumns(hObj);
  if (Column >= NumColumns) {
    return 1;
  }
  WM_LOCK();
  pObj    = LISTVIEW_H2P(hObj);
  pColumn = (LISTVIEW_COLUMN *)GUI_ARRAY_GetpItem(&pObj->ColumnArray, Column);
  if (pColumn->fpCompare) {
    pSort = _CreateSortObject(hObj, pObj);
    pObj  = LISTVIEW_H2P(hObj); /* Restore after allocating memory */
    if (pSort) {
      pObj->SortIndex = Column;
      pSort->Reverse  = Reverse;
      LISTVIEW__InvalidateInsideArea(hObj, pObj);
    } else {
      Error = 1;
    }
  } else {
    Error = 1;
  }
  WM_UNLOCK();
  return Error;
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_SetSort_C(void);
  void LISTVIEW_SetSort_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
