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
File        : PROGBAR_SetMinMax.c
Purpose     : Implementation of progbar widget
---------------------------END-OF-HEADER------------------------------
*/

#include "PROGBAR_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       PROGBAR_SetMinMax
*/
void PROGBAR_SetMinMax(PROGBAR_Handle hObj, int Min, int Max) {
  PROGBAR_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = PROGBAR_H2P(hObj);
    if (Max > Min) {
      if ((Max != pObj->Max) || (Min != pObj->Min)) {
        pObj->Min = Min;
        pObj->Max = Max;
        WM_InvalidateWindow(hObj);
      }
    }
    WM_UNLOCK();
  }
}

#else  /* avoid empty object files */
  void PROGBAR_SetMinMax_C(void);
  void PROGBAR_SetMinMax_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
