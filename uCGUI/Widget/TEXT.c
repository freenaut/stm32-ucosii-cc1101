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
File        : TEXT.c
Purpose     : Implementation of text widget
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include "TEXT_Private.h"
#include "WIDGET.h"
#include "GUIDebug.h"
#include "GUI_Protected.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

/* Define default fonts */
#ifndef TEXT_FONT_DEFAULT
  #if   WIDGET_USE_SCHEME_SMALL
    #define TEXT_FONT_DEFAULT &GUI_Font13_1
  #elif WIDGET_USE_SCHEME_MEDIUM
    #define TEXT_FONT_DEFAULT &GUI_Font16_1
  #elif WIDGET_USE_SCHEME_LARGE
    #define TEXT_FONT_DEFAULT &GUI_Font24_1
  #endif
#endif

#ifndef   TEXT_DEFAULT_TEXT_COLOR
  #define TEXT_DEFAULT_TEXT_COLOR GUI_BLACK
#endif
#ifndef   TEXT_DEFAULT_BK_COLOR
  #define TEXT_DEFAULT_BK_COLOR   GUI_INVALID_COLOR
#endif
#ifndef   TEXT_DEFAULT_WRAPMODE
  #define TEXT_DEFAULT_WRAPMODE   GUI_WRAPMODE_NONE
#endif

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
TEXT_PROPS TEXT__DefaultProps = {
  TEXT_FONT_DEFAULT,
  TEXT_DEFAULT_TEXT_COLOR,
  TEXT_DEFAULT_BK_COLOR,
  TEXT_DEFAULT_WRAPMODE
};

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _FreeAttached
*/
static void _FreeAttached(TEXT_Obj* pObj) {
  GUI_ALLOC_FreePtr(&pObj->hpText);
}

/*********************************************************************
*
*       _Paint
*/
static void _Paint(TEXT_Handle hObj, TEXT_Obj* pObj) {
  const char * s;
  GUI_RECT Rect;
  GUI_USE_PARA(hObj);
  LCD_SetColor(pObj->Props.TextColor);
  GUI_SetFont    (pObj->Props.pFont);
  /* Fill with parents background color */
  #if !TEXT_SUPPORT_TRANSPARENCY   /* Not needed any more, since window is transparent*/
    if (pObj->Props.BkColor == GUI_INVALID_COLOR) {
      LCD_SetBkColor(WIDGET__GetBkColor(hObj));
    } else {
      LCD_SetBkColor(pObj->Props.BkColor);
    }
    GUI_Clear();
  #else
    if (!WM_GetHasTrans(hObj)) {
      LCD_SetBkColor(pObj->Props.BkColor);
      GUI_Clear();
    }
  #endif
  /* Show the text */
  if (pObj->hpText) {
    s = (const char*) GUI_ALLOC_h2p(pObj->hpText);
    GUI_SetTextMode(GUI_TM_TRANS);
    WM_GetClientRect(&Rect);
    GUI_DispStringInRectWrap(s, &Rect, pObj->Align, pObj->Props.WrapMode);
  }
}

/*********************************************************************
*
*       _Delete
*/
static void _Delete(TEXT_Obj* pObj) {
  /* Delete attached objects (if any) */
  GUI_DEBUG_LOG("TEXT: Delete() Deleting attached items");
  _FreeAttached(pObj);
}

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       TEXT_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
TEXT_Obj * TEXT_h2p(TEXT_Handle h) {
  TEXT_Obj * p = (TEXT_Obj *)GUI_ALLOC_h2p(h);
  if (p) {
    if (p->DebugId != TEXT_ID) {
      GUI_DEBUG_ERROROUT("TEXT.c: Wrong handle type or Object not init'ed");
      return 0;
    }
  }
  return p;
}
#endif

/*********************************************************************
*
*       Exported routines:  Callback
*
**********************************************************************
*/
/*********************************************************************
*
*       TEXT_Callback
*/
void TEXT_Callback (WM_MESSAGE*pMsg) {
  TEXT_Handle hObj;
  TEXT_Obj * pObj;
  hObj = pMsg->hWin;
  pObj = (TEXT_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
  /* Let widget handle the standard messages */
  if (WIDGET_HandleActive(hObj, pMsg) == 0) {
    return;
  }
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_DEBUG_LOG("TEXT: _Callback(WM_PAINT)\n");
    _Paint(hObj, pObj);
    return;
  case WM_DELETE:
    GUI_DEBUG_LOG("TEXT: _Callback(WM_DELETE)\n");
    _Delete(pObj);
    break;       /* No return here ... WM_DefaultProc needs to be called */
  }
  WM_DefaultProc(pMsg);
}

/*********************************************************************
*
*       Exported routines:  Create
*
**********************************************************************
*/

/* Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions */

/*********************************************************************
*
*       TEXT_CreateEx
*/
TEXT_Handle TEXT_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                          int WinFlags, int ExFlags, int Id, const char* pText)
{
  TEXT_Handle hObj;
  /* Create the window */
  #if TEXT_SUPPORT_TRANSPARENCY
    WinFlags |= WM_CF_HASTRANS;
  #endif
  WM_LOCK();
  hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, TEXT_Callback,
                                sizeof(TEXT_Obj) - sizeof(WM_Obj));
  if (hObj) {
    TEXT_Obj* pObj;
    WM_HMEM hMem = 0;
    pObj = (TEXT_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
    /* init widget specific variables */
    WIDGET__Init(&pObj->Widget, Id, 0);
    /* init member variables */
    TEXT_INIT_ID(pObj);
    if (pText) {
      hMem = GUI_ALLOC_AllocZero(strlen(pText) + 1);
      if (hMem) {
        strcpy((char*) GUI_ALLOC_h2p(hMem), pText);
      }
    }
    pObj->hpText = hMem;
    pObj->Align  = ExFlags;
    pObj->Props  = TEXT__DefaultProps;
  } else {
    GUI_DEBUG_ERROROUT_IF(hObj==0, "TEXT_Create failed")
  }
  WM_UNLOCK();
  return hObj;
}

#else /* avoid empty object files */

void TEXT_C(void);
void TEXT_C(void){}

#endif  /* #if GUI_WINSUPPORT */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
