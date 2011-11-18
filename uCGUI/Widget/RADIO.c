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
File        : RADIO.c
Purpose     : Implementation of radio button widget
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include "GUI_Protected.h"
#include "RADIO_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

/* Define default image inactiv */
#ifndef   RADIO_IMAGE0_DEFAULT
  #define RADIO_IMAGE0_DEFAULT        &RADIO__abmRadio[0]
#endif

/* Define default image activ */
#ifndef   RADIO_IMAGE1_DEFAULT
  #define RADIO_IMAGE1_DEFAULT        &RADIO__abmRadio[1]
#endif

/* Define default image check */
#ifndef   RADIO_IMAGE_CHECK_DEFAULT
  #define RADIO_IMAGE_CHECK_DEFAULT   &RADIO__bmCheck
#endif

/* Define default font */
#ifndef   RADIO_FONT_DEFAULT
  #if   WIDGET_USE_SCHEME_SMALL
    #define RADIO_SPACING_DEFAULT 20
    #define RADIO_FONT_DEFAULT &GUI_Font13_1
  #elif WIDGET_USE_SCHEME_MEDIUM
    #define RADIO_SPACING_DEFAULT 24
    #define RADIO_FONT_DEFAULT &GUI_Font16_1
  #elif WIDGET_USE_SCHEME_LARGE
    #define RADIO_SPACING_DEFAULT 30
    #define RADIO_FONT_DEFAULT &GUI_Font24_1
  #endif
#endif

/* Define default text color */
#ifndef   RADIO_DEFAULT_TEXT_COLOR
  #define RADIO_DEFAULT_TEXT_COLOR    GUI_BLACK
#endif

/* Define default background color */
#ifndef   RADIO_DEFAULT_BKCOLOR
  #define RADIO_DEFAULT_BKCOLOR       0xC0C0C0
#endif

#ifndef   RADIO_FOCUSCOLOR_DEFAULT
  #define RADIO_FOCUSCOLOR_DEFAULT    GUI_BLACK
#endif

#define RADIO_BORDER                  2

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

RADIO_PROPS RADIO__DefaultProps = {
  RADIO_DEFAULT_BKCOLOR,
  RADIO_DEFAULT_TEXT_COLOR,
  RADIO_FOCUSCOLOR_DEFAULT,
  RADIO_FONT_DEFAULT,
  RADIO_IMAGE0_DEFAULT,
  RADIO_IMAGE1_DEFAULT,
  RADIO_IMAGE_CHECK_DEFAULT,
};

tRADIO_SetValue* RADIO__pfHandleSetValue;

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/

/*********************************************************************
*
*       _OnPaint
*
* Purpose:
*   Paints the RADIO button.
*   The button can actually consist of multiple buttons (NumItems).
*   The focus rectangle will be drawn on top of the text if any text is set,
*   otherwise around the entire buttons.
*/
static void _OnPaint(RADIO_Handle hObj, RADIO_Obj* pObj) {
  const GUI_BITMAP* pBmRadio;
  const GUI_BITMAP* pBmCheck;
  const char* pText;
  GUI_FONTINFO FontInfo;
  GUI_RECT Rect, r, rFocus = {0};
  int i, y, HasFocus, FontDistY;
  U8 SpaceAbove, CHeight, FocusBorder;

  /* Init some data */
  WIDGET__GetClientRect(&pObj->Widget, &rFocus);
  HasFocus  = (pObj->Widget.State & WIDGET_STATE_FOCUS) ? 1 : 0;
  pBmRadio  = pObj->Props.apBmRadio[WM__IsEnabled(hObj)];
  pBmCheck  = pObj->Props.pBmCheck;
  rFocus.x1 = pBmRadio->XSize + RADIO_BORDER * 2 - 1;
  rFocus.y1 = pObj->Height + ((pObj->NumItems - 1) * pObj->Spacing) - 1;

  /* Select font and text color */
  LCD_SetColor(pObj->Props.TextColor);
  GUI_SetFont(pObj->Props.pFont);
  GUI_SetTextMode(GUI_TM_TRANS);

  /* Get font infos */
  GUI_GetFontInfo(pObj->Props.pFont, &FontInfo);
  FontDistY   = GUI_GetFontDistY();
  CHeight     = FontInfo.CHeight;
  SpaceAbove  = FontInfo.Baseline - CHeight;
  Rect.x0     = pBmRadio->XSize + RADIO_BORDER * 2 + 2;
  Rect.y0     = (CHeight <= pObj->Height) ? ((pObj->Height - CHeight) / 2) : 0;
  Rect.y1     = Rect.y0 + CHeight - 1;
  FocusBorder = (FontDistY <= 12) ? 2 : 3;
  if (Rect.y0 < FocusBorder) {
    FocusBorder = Rect.y0;
  }

  /* Clear inside ... Just in case      */
  /* Fill with parents background color */
#if WM_SUPPORT_TRANSPARENCY
  if (!WM_GetHasTrans(hObj))
#endif
  {
    if (pObj->Props.BkColor != GUI_INVALID_COLOR) {
      LCD_SetBkColor(pObj->Props.BkColor);
    } else {
      LCD_SetBkColor(RADIO_DEFAULT_BKCOLOR);
    }
    GUI_Clear();
  }

  /* Iterate over all items */
  for (i = 0; i < pObj->NumItems; i++) {
    y = i * pObj->Spacing;
    /* Draw the radio button bitmap */
    GUI_DrawBitmap(pBmRadio, RADIO_BORDER, RADIO_BORDER + y);
    /* Draw the check bitmap */
    if (pObj->Sel == i) {
      GUI_DrawBitmap(pBmCheck, RADIO_BORDER +  (pBmRadio->XSize - pBmCheck->XSize) / 2, 
                               RADIO_BORDER + ((pBmRadio->YSize - pBmCheck->YSize) / 2) + y);
    }
    /* Draw text if available */
    pText = (const char*)GUI_ARRAY_GetpItem(&pObj->TextArray, i);
    if (pText) {
      if (*pText) {
        r = Rect;
        r.x1 = r.x0 + GUI_GetStringDistX(pText) - 2;
        GUI_MoveRect(&r, 0, y);
        GUI_DispStringAt(pText, r.x0, r.y0 - SpaceAbove);
        /* Calculate focus rect */
        if (HasFocus && (pObj->Sel == i)) {
          GUI__ReduceRect(&rFocus, &r, -FocusBorder);
        }
      }
    }
  }

  /* Draw the focus rect */
  if (HasFocus) {
    LCD_SetColor(pObj->Props.FocusColor);
    GUI_DrawFocusRect(&rFocus, 0);
  }
}

/*********************************************************************
*
*       _OnTouch
*/
static void _OnTouch(RADIO_Handle hObj, RADIO_Obj* pObj, WM_MESSAGE*pMsg) {
  int Notification;
  int Hit = 0;
  GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
  if (pMsg->Data.p) {  /* Something happened in our area (pressed or released) */
    if (pState->Pressed) {
      int y, Sel;
      y   = pState->y;
      Sel = y   / pObj->Spacing;
      y  -= Sel * pObj->Spacing;
      if (y <= pObj->Height) {
        RADIO_SetValue(hObj, Sel);
      }
      if (WM_IsFocussable(hObj)) {
        WM_SetFocus(hObj);
      }
      Notification = WM_NOTIFICATION_CLICKED;
    } else {
      Hit = 1;
      Notification = WM_NOTIFICATION_RELEASED;
    }
  } else {
    Notification = WM_NOTIFICATION_MOVED_OUT;
  }
  WM_NotifyParent(hObj, Notification);
  if (Hit == 1) {
    GUI_DEBUG_LOG("RADIO: Hit\n");
    GUI_StoreKey(pObj->Widget.Id);
  }
}

/*********************************************************************
*
*       _OnKey
*/
static void  _OnKey(RADIO_Handle hObj, WM_MESSAGE* pMsg) {
  WM_KEY_INFO* pKeyInfo;
  pKeyInfo = (WM_KEY_INFO*)(pMsg->Data.p);
  if (pKeyInfo->PressedCnt > 0) {
    switch (pKeyInfo->Key) {
    case GUI_KEY_RIGHT:
    case GUI_KEY_DOWN:
      RADIO_Inc(hObj);
      break;                    /* Send to parent by not doing anything */
    case GUI_KEY_LEFT:
    case GUI_KEY_UP:
      RADIO_Dec(hObj);
      break;                    /* Send to parent by not doing anything */
    default:
      return;
    }
  }
}

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       RADIO_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
RADIO_Obj * RADIO_h2p(RADIO_Handle h) {
  RADIO_Obj * p = (RADIO_Obj *)GUI_ALLOC_h2p(h);
  if (p) {
    if (p->DebugId != RADIO_ID) {
      GUI_DEBUG_ERROROUT("RADIO.c: Wrong handle type or Object not init'ed");
      return 0;
    }
  }
  return p;
}
#endif

/*********************************************************************
*
*       Exported routines, modul internal
*
**********************************************************************
*/
/*********************************************************************
*
*       RADIO__SetValue
*/
void RADIO__SetValue(RADIO_Handle hObj, RADIO_Obj* pObj, int v) {
  if (v >= (int)pObj->NumItems) {
    v = pObj->NumItems - 1;
  }
  if (v != pObj->Sel) {
    pObj->Sel = v;
    WM_InvalidateWindow(hObj);
    WM_NotifyParent(hObj, WM_NOTIFICATION_VALUE_CHANGED);
  }
}

/*********************************************************************
*
*       Exported routines:  Callback
*
**********************************************************************
*/
/*********************************************************************
*
*       RADIO_Callback
*/
void RADIO_Callback (WM_MESSAGE* pMsg) {
  RADIO_Handle hObj;
  RADIO_Obj*   pObj;
  hObj = pMsg->hWin;
  pObj = (RADIO_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
  /* Let widget handle the standard messages */
  if (WIDGET_HandleActive(hObj, pMsg) == 0) {
    return;
  }
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_DEBUG_LOG("RADIO: _Callback(WM_PAINT)\n");
    _OnPaint(hObj, pObj);
    return;
  case WM_GET_RADIOGROUP:
    pMsg->Data.v = pObj->GroupId;
    return;
  case WM_TOUCH:
    _OnTouch(hObj, pObj, pMsg);
    break;
  case WM_KEY:
    _OnKey(hObj, pMsg);
    break;
  case WM_DELETE:
    GUI_ARRAY_Delete(&pObj->TextArray);
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

/* Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions */

/*********************************************************************
*
*       RADIO_CreateEx
*/
RADIO_Handle RADIO_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                            int WinFlags, int ExFlags, int Id, int NumItems, int Spacing)
{
  RADIO_Handle hObj;
  int Height, i;
  WM_LOCK();
  /* Calculate helper variables */
  Height   = RADIO__DefaultProps.apBmRadio[0]->YSize + RADIO_BORDER * 2;
  Spacing  = (Spacing  <= 0) ? RADIO_SPACING_DEFAULT : Spacing;
  NumItems = (NumItems <= 0) ? 2                     : NumItems;
  if (ySize == 0) {
    ySize  = Height + ((NumItems - 1) * Spacing);
  }
  if (xSize == 0) {
    xSize  = RADIO__DefaultProps.apBmRadio[0]->XSize + RADIO_BORDER * 2;
  }
#if WM_SUPPORT_TRANSPARENCY
  WinFlags |= WM_CF_HASTRANS;
#endif
  /* Create the window */
  hObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WinFlags, RADIO_Callback, sizeof(RADIO_Obj) - sizeof(WM_Obj));
  if (hObj) {
    RADIO_Obj* pObj;
    pObj = (RADIO_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
    /* Init sub-classes */
    GUI_ARRAY_CREATE(&pObj->TextArray);
    for (i = 0; i < NumItems; i++) {
      GUI_ARRAY_AddItem(&pObj->TextArray, NULL, 0);
    }
    /* Init widget specific variables */
    ExFlags &= RADIO_TEXTPOS_LEFT;
    WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE | ExFlags);
    /* Init member variables */
    RADIO_INIT_ID(pObj);
    pObj->Props = RADIO__DefaultProps;
    pObj->Props.BkColor= WM_GetBkColor(hParent);
    pObj->NumItems     = NumItems;
    pObj->Spacing      = Spacing;
    pObj->Height       = Height;
  } else {
    GUI_DEBUG_ERROROUT_IF(hObj==0, "RADIO_Create failed")
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
*       RADIO_AddValue
*/
void RADIO_AddValue(RADIO_Handle hObj, int Add) {
  if (hObj) {
    RADIO_Obj* pObj;
    WM_LOCK();
    pObj = RADIO_H2P(hObj);
    RADIO_SetValue(hObj, pObj->Sel + Add);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       RADIO_Dec
*/
void RADIO_Dec(RADIO_Handle hObj) {
  RADIO_AddValue(hObj, -1);
}

/*********************************************************************
*
*       RADIO_Inc
*/
void RADIO_Inc(RADIO_Handle hObj) {
  RADIO_AddValue(hObj,  1);
}

/*********************************************************************
*
*       RADIO_SetValue
*/
void RADIO_SetValue(RADIO_Handle hObj, int v) {
  if (hObj) {
    RADIO_Obj* pObj;
    WM_LOCK();
    pObj = RADIO_H2P(hObj);
    if (pObj->GroupId && RADIO__pfHandleSetValue) {
      (*RADIO__pfHandleSetValue)(hObj, pObj, v);
    } else {
      if (v < 0) {
        v = 0;
      }
      RADIO__SetValue(hObj, pObj, v);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       Exported routines:  Query state
*
**********************************************************************
*/
/*********************************************************************
*
*       RADIO_GetValue
*/
int RADIO_GetValue(RADIO_Handle hObj) {
  int r = 0;
  if (hObj) {
    RADIO_Obj* pObj;
    WM_LOCK();
    pObj = RADIO_H2P(hObj);
    r = pObj->Sel;
    WM_UNLOCK();
  }
  return r;
}

#else /* avoid empty object files */

void RADIO_C(void);
void RADIO_C(void){}

#endif  /* #if GUI_WINSUPPORT */

/************************* end of file ******************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
