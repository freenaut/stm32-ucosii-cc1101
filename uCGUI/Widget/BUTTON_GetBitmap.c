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
File        : BUTTON_GetBitmap.c
Purpose     : Implementation of BUTTON_GetBitmap()
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>
#include "BUTTON_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/
/*********************************************************************
*
*       BUTTON_GetBitmap
*/
const GUI_BITMAP GUI_UNI_PTR * BUTTON_GetBitmap(BUTTON_Handle hObj,unsigned int Index) {
  const GUI_BITMAP GUI_UNI_PTR * pBM;
  pBM = NULL;
  if (hObj) {
    BUTTON_Obj * pObj;
    WM_LOCK();
    pObj = BUTTON_H2P(hObj);
    if (Index <= GUI_COUNTOF(pObj->ahDrawObj)) {
      WM_HMEM hDrawObj;
      hDrawObj = pObj->ahDrawObj[Index];
      if (hDrawObj) {
        GUI_DRAW * pDrawObj;
        pDrawObj = (GUI_DRAW *)GUI_ALLOC_h2p(hDrawObj);
        pBM = (const GUI_BITMAP GUI_UNI_PTR *)pDrawObj->Data.pData;
      }
    }
    WM_UNLOCK();
  }
  return pBM;
}

#else                            /* Avoid problems with empty object modules */
  void BUTTON_GetBitmap_C(void);
  void BUTTON_GetBitmap_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
