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
File        : LISTVIEW_GetItemText.c
Purpose     : Implementation of LISTVIEW_GetItemText
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
*       LISTVIEW_GetItemText
*/
void LISTVIEW_GetItemText(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, char* pBuffer, unsigned MaxSize) {
  memset(pBuffer, 0, MaxSize);
  if (hObj && MaxSize) {
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if ((Column < LISTVIEW__GetNumColumns(pObj)) && (Row < LISTVIEW__GetNumRows(pObj))) {
      LISTVIEW_ROW*  pRow;
      LISTVIEW_CELL* pCell;
      pRow  = (LISTVIEW_ROW*)  GUI_ARRAY_GetpItem(&pObj->RowArray,  Row);
      pCell = (LISTVIEW_CELL*) GUI_ARRAY_GetpItem(&pRow->CellArray, Column);
      strncpy(pBuffer, pCell->acText, MaxSize);
      pBuffer[MaxSize - 1] = 0;
    }
    WM_UNLOCK();
  }
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_GetItemText_C(void);
  void LISTVIEW_GetItemText_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
