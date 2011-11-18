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
File        : GUI__SetText.C
Purpose     : Implementation of said function
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include "GUI_Protected.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI__SetText
*
*  Function description
*    GUI internal function.
*    Assigns a "C"-string to a memory handle.
*    Memory for the string is assigned, the string itself is copied to the allocated memory.
*    If the handle was valid before, the memory of the old string is released.
*
*  Parameters
*    phText     Pointer to the handle. The pointer must be valid, the handle may be 0 (invalid).
*    s          Pointer to the string to be copied.
*               A NULL pointer is not allowed.
*/
int GUI__SetText(GUI_HMEM* phText, const char* s) {
  int r;
  GUI_HMEM hOld;

  r = 0;
  hOld = *phText;
  if (GUI__strcmp_hp(hOld, s) != 0) {            /* Make sure we have a quick out if nothing changes */
    GUI_HMEM hMem;
    int NewLen;
    int OldLen;
       
    NewLen = GUI__strlen(s) + 1;
    OldLen = GUI_ALLOC_GetSize(hOld);
    if (NewLen == OldLen) {
      char* pMem;
      pMem = (char*) GUI_ALLOC_h2p(hOld);
      memcpy(pMem, s, NewLen);
      r = 1;
    } else {
      hMem = GUI_ALLOC_AllocNoInit(NewLen);
      if (hMem) {
        char* pMem;
        pMem = (char*) GUI_ALLOC_h2p(hMem);
        strcpy(pMem, s);
        GUI_ALLOC_FreePtr(phText);
        *phText = hMem;
        r = 1;
      }
    }
  }
  return r;
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
