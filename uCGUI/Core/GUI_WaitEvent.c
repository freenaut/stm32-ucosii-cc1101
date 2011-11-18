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
File        : GUI_WaitEvent.c
Purpose     : Implementation of said function
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_WaitEvent
*/
void GUI_WaitEvent(void) {
#if GUI_OS
  unsigned LockCnt;
  unsigned i;

  LockCnt = GUITASK__EntranceCnt;
  /*
   * Reset lock count
   */
  GUI_ALLOC_Lock();     /* Memory lock */
  for (i = LockCnt; i; i--) {
    GUI_Unlock();       /* Dec task lock */
  }
#endif
  GUI_X_WAIT_EVENT();
#if GUI_OS
  /*
   * Restore lock count
   */
  for (i = LockCnt; i; i--) {
    GUI_Lock();         /* Inc task lock */
  }
  GUI_ALLOC_Unlock()  ; /* Memory unlock */
#endif
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
