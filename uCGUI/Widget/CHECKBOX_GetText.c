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
File        : CHECKBOX_GetText.c
Purpose     : Implementation of CHECKBOX_GetText
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>
#include "CHECKBOX_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CHECKBOX_GetText
*/
int CHECKBOX_GetText(CHECKBOX_Handle hObj, char * pBuffer, int MaxLen) {
  int Len = 0;
  if (hObj) {
    CHECKBOX_Obj  * pObj;
    WM_LOCK();
    pObj = CHECKBOX_H2P(hObj);
    if (pObj->hpText) {
      const char * pText;
      pText = (const char *)GUI_ALLOC_h2p(pObj->hpText);
      Len = strlen(pText);
      if (Len > (MaxLen - 1)) {
        Len = MaxLen - 1;
      }
      memcpy(pBuffer, pText, Len);
      *(pBuffer + Len) = 0;
    }
    WM_UNLOCK();
  }
  return Len;
}

#else  /* avoid empty object files */

void CHECKBOX_GetText_c(void);
void CHECKBOX_GetText_c(void) {}

#endif  /* #if GUI_WINSUPPORT */

/************************* end of file ******************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
