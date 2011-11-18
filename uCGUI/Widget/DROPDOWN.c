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
File        : DROPDOWN.c
Purpose     : Implementation of dropdown widget
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_ARRAY.h"
#include <stdlib.h>
#include <string.h>
#include "DROPDOWN.h"
#include "DROPDOWN_Private.h"
#include "SCROLLBAR.h"
#include "WIDGET.h"
#include "GUIDebug.h"
#include "GUI_Protected.h"
#include "WM_Intern.h"
#include "LISTBOX.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

/* Define default fonts */
#ifndef   DROPDOWN_FONT_DEFAULT
  #if   WIDGET_USE_SCHEME_SMALL
    #define DROPDOWN_FONT_DEFAULT &GUI_Font13_1
  #elif WIDGET_USE_SCHEME_MEDIUM
    #define DROPDOWN_FONT_DEFAULT &GUI_Font16_1
  #elif WIDGET_USE_SCHEME_LARGE
    #define DROPDOWN_FONT_DEFAULT &GUI_Font24_1
  #endif
#endif

/* Define colors */
#ifndef   DROPDOWN_BKCOLOR0_DEFAULT
  #define DROPDOWN_BKCOLOR0_DEFAULT GUI_WHITE     /* Not selected */
#endif

#ifndef   DROPDOWN_BKCOLOR1_DEFAULT
  #define DROPDOWN_BKCOLOR1_DEFAULT GUI_GRAY      /* Selected, no focus */
#endif

#ifndef   DROPDOWN_BKCOLOR2_DEFAULT
  #define DROPDOWN_BKCOLOR2_DEFAULT GUI_BLUE      /* Selected, focus */
#endif

#ifndef   DROPDOWN_TEXTCOLOR0_DEFAULT
  #define DROPDOWN_TEXTCOLOR0_DEFAULT GUI_BLACK   /* Not selected */
#endif

#ifndef   DROPDOWN_TEXTCOLOR1_DEFAULT
  #define DROPDOWN_TEXTCOLOR1_DEFAULT GUI_WHITE   /* Selected, no focus */
#endif

#ifndef   DROPDOWN_TEXTCOLOR2_DEFAULT
  #define DROPDOWN_TEXTCOLOR2_DEFAULT GUI_WHITE   /* Selected, focus */
#endif

#ifndef   SCROLLBAR_COLOR_ARROW_DEFAULT
  #define SCROLLBAR_COLOR_ARROW_DEFAULT GUI_BLACK /* Arrow color */
#endif

#ifndef   SCROLLBAR_COLOR_BUTTON_DEFAULT
  #define SCROLLBAR_COLOR_BUTTON_DEFAULT 0xc0c0c0 /* Button color */
#endif

#ifndef   DROPDOWN_BORDER_DEFAULT
  #define DROPDOWN_BORDER_DEFAULT 2
#endif

#ifndef   DROPDOWN_ALIGN_DEFAULT
  #define DROPDOWN_ALIGN_DEFAULT GUI_TA_LEFT      /* Default text alignment */
#endif

#ifndef   DROPDOWN_KEY_EXPAND
  #define DROPDOWN_KEY_EXPAND GUI_KEY_SPACE
#endif

#ifndef   DROPDOWN_KEY_SELECT
  #define DROPDOWN_KEY_SELECT GUI_KEY_ENTER
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

DROPDOWN_PROPS DROPDOWN__DefaultProps = {
  DROPDOWN_FONT_DEFAULT,
  DROPDOWN_BKCOLOR0_DEFAULT,
  DROPDOWN_BKCOLOR1_DEFAULT,
  DROPDOWN_BKCOLOR2_DEFAULT,
  DROPDOWN_TEXTCOLOR0_DEFAULT,
  DROPDOWN_TEXTCOLOR1_DEFAULT,
  DROPDOWN_TEXTCOLOR2_DEFAULT,
  SCROLLBAR_COLOR_ARROW_DEFAULT,
  SCROLLBAR_COLOR_BUTTON_DEFAULT,
  GUI_INVALID_COLOR,
  GUI_INVALID_COLOR,
  GUI_INVALID_COLOR,
  DROPDOWN_BORDER_DEFAULT,
  DROPDOWN_ALIGN_DEFAULT
};

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN__GetNumItems

  Returns:
    Number of fully or partially visible items
*/
int DROPDOWN__GetNumItems(DROPDOWN_Obj* pObj) {
  return pObj->Handles.NumItems;
}

/*********************************************************************
*
*       _GethItem

  Returns:
    Handle of the specified item
*/
static WM_HMEM _GethItem(DROPDOWN_Obj* pObj, int Index) {
  return GUI_ARRAY_GethItem(&pObj->Handles, Index);
}

/*********************************************************************
*
*       _DrawTriangleDown
*/
static void _DrawTriangleDown(int x, int y, int Size) {
  for (; Size >= 0; Size--, y++ ) {
    GUI_DrawHLine(y, x - Size, x + Size);
  }
}
/*********************************************************************
*
*       _GetpItem

  Returns:
    Pointer to the specified item
*/
static const char* _GetpItem(DROPDOWN_Obj* pObj, int Index) {
  const char* s = NULL;
  WM_HMEM h = _GethItem(pObj, Index);
  if (h) {
    s = (const char*) GUI_ALLOC_h2p(h);
  }
  return s;
}

/*********************************************************************
*
*       _Tolower
*/
static int _Tolower(int Key) {
  if ((Key >= 0x41) && (Key <= 0x5a)) {
    Key += 0x20;
  }
  return Key;
}

/*********************************************************************
*
*       _SelectByKey
*/
static void _SelectByKey(DROPDOWN_Handle hObj, int Key) {
  int i;
  DROPDOWN_Obj* pObj;
  pObj = DROPDOWN_H2P(hObj);
  Key = _Tolower(Key);
  for (i = 0; i < DROPDOWN__GetNumItems(pObj); i++) {
    char c = _Tolower(*_GetpItem(pObj, i));
    if (c == Key) {
      DROPDOWN_SetSel(hObj, i);
      break;
    }
  }
}

/*********************************************************************
*
*       _FreeAttached
*/
static void _FreeAttached(DROPDOWN_Obj* pObj) {
  GUI_ARRAY_Delete(&pObj->Handles);
  WM_DeleteWindow(pObj->hListWin);
}

/*********************************************************************
*
*       _Paint
*/
static void _Paint(DROPDOWN_Handle hObj) {
  int Border;
  GUI_RECT r;
  const char* s;
  int InnerSize, ColorIndex;
  DROPDOWN_Obj* pObj;
  int TextBorderSize;
  /* Do some initial calculations */
  pObj = DROPDOWN_H2P(hObj);
  Border = pObj->Widget.pEffect->EffectSize;
  TextBorderSize = pObj->Props.TextBorderSize;
  GUI_SetFont(pObj->Props.pFont);
  ColorIndex = (pObj->Widget.State & WIDGET_STATE_FOCUS) ? 2 : 1;
  s = _GetpItem(pObj, pObj->Sel);
  WM_GetClientRect(&r);
  GUI__ReduceRect(&r, &r, Border);
  InnerSize = r.y1 - r.y0 + 1;
  /* Draw the 3D effect (if configured) */
  WIDGET__EFFECT_DrawDown(&pObj->Widget);
  /* Draw the outer text frames */
  r.x1 -= InnerSize;     /* Spare square area to the right */
  LCD_SetColor(pObj->Props.aBackColor[ColorIndex]);
  /* Draw the text */
  LCD_SetBkColor(pObj->Props.aBackColor[ColorIndex]);
  GUI_FillRectEx(&r);
  r.x0 += TextBorderSize;
  r.x1 -= TextBorderSize;
  LCD_SetColor  (pObj->Props.aTextColor[ColorIndex]);
  GUI_DispStringInRect(s, &r, pObj->Props.Align);
  /* Draw arrow */
  WM_GetClientRect(&r);
  GUI__ReduceRect(&r, &r, Border);
  r.x0 = r.x1 + 1 - InnerSize;
  LCD_SetColor(pObj->Props.aColor[DROPDOWN_CI_BUTTON]);
  GUI_FillRectEx(&r);
  LCD_SetColor(pObj->Props.aColor[DROPDOWN_CI_ARROW]);
  _DrawTriangleDown((r.x1 + r.x0) / 2, r.y0 + 5, (r.y1 - r.y0 - 8) / 2);
  WIDGET__EFFECT_DrawUpRect(&pObj->Widget, &r);
}

/*********************************************************************
*
*       _OnTouch
*/
static int _OnTouch(DROPDOWN_Handle hObj, WM_MESSAGE*pMsg) {
  const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
  if (pMsg->Data.p) {  /* Something happened in our area (pressed or released) */
    if (pState->Pressed) {
      WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
    } else {
      WM_NotifyParent(hObj, WM_NOTIFICATION_RELEASED);
    }
  } else {     /* Mouse moved out */
    WM_NotifyParent(hObj, WM_NOTIFICATION_MOVED_OUT);
  }
  return 0; /* Message handled */
}

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
DROPDOWN_Obj * DROPDOWN_h2p(DROPDOWN_Handle h) {
  DROPDOWN_Obj * p = (DROPDOWN_Obj *)GUI_ALLOC_h2p(h);
  if (p) {
    if (p->DebugId != DROPDOWN_ID) {
      GUI_DEBUG_ERROROUT("DROPDOWN.c: Wrong handle type or Object not init'ed");
      return 0;
    }
  }
  return p;
}
#endif

/*********************************************************************
*
*       DROPDOWN__AdjustHeight
*/
void DROPDOWN__AdjustHeight(DROPDOWN_Handle hObj, DROPDOWN_Obj* pObj) {
  int Height;
  Height = pObj->TextHeight;
  if (!Height) {
    Height = GUI_GetYDistOfFont(pObj->Props.pFont);
  }
  Height += pObj->Widget.pEffect->EffectSize + 2 *  pObj->Props.TextBorderSize;
  WM_SetSize(hObj, WM__GetWindowSizeX(&pObj->Widget.Win), Height);
}

/*********************************************************************
*
*       Exported routines:  Callback
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN_Callback
*/
void DROPDOWN_Callback (WM_MESSAGE*pMsg) {
  DROPDOWN_Handle hObj;
  DROPDOWN_Obj* pObj;
  char IsExpandedBeforeMsg;
  hObj = pMsg->hWin;
  pObj = (DROPDOWN_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
  IsExpandedBeforeMsg = pObj->hListWin ? 1 : 0;
  /* Let widget handle the standard messages */
  if (WIDGET_HandleActive(hObj, pMsg) == 0) {
    return;
  }
  switch (pMsg->MsgId) {
  case WM_NOTIFY_OWNER_KEY:
    /* Close the listbox if DROPDOWN_KEY_SELECT has been pressed */
    if (((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt > 0) {
      int Key;
      Key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
      if (Key == DROPDOWN_KEY_SELECT) {
        DROPDOWN_SetSel(hObj, LISTBOX_GetSel(pObj->hListWin));
        DROPDOWN_Collapse(hObj);
        WM_SetFocus(hObj);
      }
    }
    break;
  case WM_NOTIFY_PARENT:
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_SCROLL_CHANGED:
      WM_NotifyParent(hObj, WM_NOTIFICATION_SCROLL_CHANGED);
      break;
    case WM_NOTIFICATION_CLICKED:
      DROPDOWN_SetSel(hObj, LISTBOX_GetSel(pObj->hListWin));
      WM_SetFocus(hObj);
      break;
    case LISTBOX_NOTIFICATION_LOST_FOCUS:
      DROPDOWN_Collapse(hObj);
      break;
    }
    break;
  case WM_PID_STATE_CHANGED:
    if (IsExpandedBeforeMsg == 0) {    /* Make sure we do not react a second time */
      const WM_PID_STATE_CHANGED_INFO * pInfo = (const WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p;
      if (pInfo->State) {
        DROPDOWN_Expand(hObj);
      }
    }
    break;
  case WM_TOUCH:
    if (_OnTouch(hObj, pMsg) == 0) {
      return;
    }
    break;
  case WM_PAINT:
    _Paint(hObj);
    break;
  case WM_DELETE:
    _FreeAttached(pObj);
    break;       /* No return here ... WM_DefaultProc needs to be called */
  case WM_KEY:
    if ( ((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt >0) {
      int Key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
      switch (Key) {
        case DROPDOWN_KEY_EXPAND:
          DROPDOWN_Expand(hObj);
          break;
        case GUI_KEY_BACKTAB:
          break;
        case GUI_KEY_TAB:
          if (WM_GetFocussedWindow() == pObj->hListWin) {
            WM_SetFocus(hObj);      /* Make sure, WM_SetFocusOnNextChild() works right when calling WM_DefaultProc() */
          }
          break;                    /* Send to parent by not doing anything */
        default:
          DROPDOWN_AddKey(hObj, Key);
          return;
      }
    }
    break;
  }
  WM_DefaultProc(pMsg);
}

/*********************************************************************
*
*       Exported routines:  Create
*
**********************************************************************
*/
/*********************************************************************
*
*       DROPDOWN_CreateEx
*/
DROPDOWN_Handle DROPDOWN_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                  int WinFlags, int ExFlags, int Id)
{
  DROPDOWN_Handle hObj;
  WM_LOCK();
  hObj = WM_CreateWindowAsChild(x0, y0, xsize, -1, hParent, WinFlags, DROPDOWN_Callback,
                                sizeof(DROPDOWN_Obj) - sizeof(WM_Obj));
  if (hObj) {
    DROPDOWN_Obj* pObj;
    pObj = (DROPDOWN_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
    /* Init sub-classes */
    GUI_ARRAY_CREATE(&pObj->Handles);
    /* init widget specific variables */
    WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
    pObj->Flags          = ExFlags;
    pObj->Props          = DROPDOWN__DefaultProps;
    pObj->ScrollbarWidth = 0;
    DROPDOWN_INIT_ID(pObj);
    pObj->ySizeEx = ysize;
    DROPDOWN__AdjustHeight(hObj, pObj);
  }
  WM_UNLOCK();
  return hObj;
}

/*********************************************************************
*
*       Exported routines:  Various methods
*
**********************************************************************
*/

/*********************************************************************
*
*       DROPDOWN_Collapse
*/
void DROPDOWN_Collapse(DROPDOWN_Handle hObj) {
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    if (pObj->hListWin) {
      WM_DeleteWindow(pObj->hListWin);
      pObj->hListWin = 0;
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       DROPDOWN_Expand
*/
void DROPDOWN_Expand(DROPDOWN_Handle hObj) {
  int xSize, ySize, i, NumItems;
  WM_HWIN hLst;
  GUI_RECT r;
  WM_HWIN hParent;
  WM_Obj* pParent;
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    if  (pObj->hListWin == 0) {
      hParent = WM_GetParent(hObj);
      pParent = WM_H2P(hParent);
      xSize = WM__GetWindowSizeX(&pObj->Widget.Win);
      ySize = pObj->ySizeEx;
      NumItems = DROPDOWN__GetNumItems(pObj);
      /* Get coordinates of window in client coordiantes of parent */
      r = pObj->Widget.Win.Rect;
      GUI_MoveRect(&r, -pParent->Rect.x0, -pParent->Rect.y0);
      if (pObj->Flags & DROPDOWN_CF_UP) {
        r.y0 -= ySize;
      } else {
        r.y0 = r.y1;
      }
      hLst = LISTBOX_CreateAsChild(NULL, WM_GetParent(hObj), r.x0, r.y0
                         , xSize, ySize, WM_CF_SHOW);
      #if WIDGET_USE_PARENT_EFFECT
        WIDGET_SetEffect(hLst, pObj->Widget.pEffect);
      #endif
      if (pObj->Flags & DROPDOWN_SF_AUTOSCROLLBAR) {
        LISTBOX_SetScrollbarWidth(hLst, pObj->ScrollbarWidth);
        LISTBOX_SetAutoScrollV(hLst, 1);
      }
      for (i = 0; i< NumItems; i++) {
        LISTBOX_AddString(hLst, _GetpItem(pObj, i));
      }
      for (i = 0; i < GUI_COUNTOF(pObj->Props.aBackColor); i++) {
        LISTBOX_SetBkColor(hLst, i, pObj->Props.aBackColor[i]);
      }
      for (i = 0; i < GUI_COUNTOF(pObj->Props.aTextColor); i++) {
        LISTBOX_SetTextColor(hLst, i, pObj->Props.aTextColor[i]);
      }
      for (i = 0; i < GUI_COUNTOF(pObj->Props.aScrollbarColor); i++) {
        LISTBOX_SetScrollbarColor(hLst, i, pObj->Props.aScrollbarColor[i]);
      }
      LISTBOX_SetItemSpacing(hLst, pObj->ItemSpacing);
      LISTBOX_SetFont(hLst, pObj->Props.pFont);
      LISTBOX_SetTextAlign(hLst, pObj->Props.Align);
      WM_SetFocus(hLst);
      pObj->hListWin = hLst;
      LISTBOX_SetOwner(hLst, hObj);
      LISTBOX_SetSel(hLst, pObj->Sel);
      WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       DROPDOWN_AddKey
*/
void DROPDOWN_AddKey(DROPDOWN_Handle hObj, int Key) {
  if (hObj) {
    WM_LOCK();
    switch (Key) {
      case GUI_KEY_DOWN:
        DROPDOWN_IncSel(hObj);
        break;
      case GUI_KEY_UP:
        DROPDOWN_DecSel(hObj);
        break;
      default:
        _SelectByKey(hObj, Key);
        break;
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       DROPDOWN_SetSel
*/
void DROPDOWN_SetSel(DROPDOWN_Handle hObj, int Sel) {
  int NumItems, MaxSel;
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    NumItems = DROPDOWN__GetNumItems(pObj);
    MaxSel = NumItems ? NumItems-1 : 0;
    if (Sel > MaxSel) {
      Sel = MaxSel;
    }
    if (Sel != pObj->Sel) {
      pObj->Sel = Sel;
      DROPDOWN_Invalidate(hObj);
      WM_NotifyParent(hObj, WM_NOTIFICATION_SEL_CHANGED);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       DROPDOWN_IncSel
*/
void DROPDOWN_IncSel(DROPDOWN_Handle hObj) {
  int Sel = DROPDOWN_GetSel(hObj);
  DROPDOWN_SetSel(hObj, Sel+1);
}

/*********************************************************************
*
*       DROPDOWN_DecSel
*/
void DROPDOWN_DecSel(DROPDOWN_Handle hObj) {
  int Sel = DROPDOWN_GetSel(hObj);
  if (Sel)
	  Sel--;
  DROPDOWN_SetSel(hObj, Sel);
}

/*********************************************************************
*
*       DROPDOWN_GetSel
*/
int  DROPDOWN_GetSel (DROPDOWN_Handle hObj) {
  int r = 0;
  DROPDOWN_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DROPDOWN_H2P(hObj);
    r = pObj->Sel;
    WM_UNLOCK();
  }
  return r;
}

#else                            /* Avoid problems with empty object modules */
  void DROPDOWN_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
