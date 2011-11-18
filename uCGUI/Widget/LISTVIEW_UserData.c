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
File        : LISTVIEW_UserData.c
Purpose     : Implementation of LISTVIEW_SetUserData() and LISTVIEW_GetUserData()
---------------------------END-OF-HEADER------------------------------
*/

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
*       LISTVIEW_SetUserData
*/
void LISTVIEW_SetUserData(LISTVIEW_Handle hObj, unsigned Row, U32 UserData) {
  if (hObj) {
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if (Row < LISTVIEW__GetNumRows(pObj)) {
      LISTVIEW_ROW * pRow;
      pRow = (LISTVIEW_ROW *)GUI_ARRAY_GetpItem(&pObj->RowArray, Row);
      pRow->UserData = UserData;
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       LISTVIEW_GetUserData
*/
U32 LISTVIEW_GetUserData(LISTVIEW_Handle hObj, unsigned Row) {
  U32 UserData;
  UserData = 0;
  if (hObj) {
    LISTVIEW_Obj* pObj;
    WM_LOCK();
    pObj = LISTVIEW_H2P(hObj);
    if (Row < LISTVIEW__GetNumRows(pObj)) {
      LISTVIEW_ROW * pRow;
      pRow = (LISTVIEW_ROW *)GUI_ARRAY_GetpItem(&pObj->RowArray, Row);
      UserData = pRow->UserData;
    }
    WM_UNLOCK();
  }
  return UserData;
}

#else                            /* Avoid problems with empty object modules */
  void LISTVIEW_UserData_C(void);
  void LISTVIEW_UserData_C(void) {}
#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
