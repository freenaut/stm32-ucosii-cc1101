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
File        : ProgBar.c
Purpose     : Implementation of progress bar
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include "PROGBAR_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

#ifndef PROGBAR_DEFAULT_FONT
  #if   WIDGET_USE_SCHEME_SMALL
    #define PROGBAR_DEFAULT_FONT &GUI_Font6x8
  #elif WIDGET_USE_SCHEME_MEDIUM
    #define PROGBAR_DEFAULT_FONT &GUI_Font8x12_ASCII
  #elif WIDGET_USE_SCHEME_LARGE
    #define PROGBAR_DEFAULT_FONT &GUI_Font8x16
  #endif
#endif

#ifndef PROGBAR_DEFAULT_BARCOLOR0
  #define PROGBAR_DEFAULT_BARCOLOR0 0x555555
#endif

#ifndef PROGBAR_DEFAULT_BARCOLOR1
  #define PROGBAR_DEFAULT_BARCOLOR1 0xAAAAAA
#endif

#ifndef PROGBAR_DEFAULT_TEXTCOLOR0
  #define PROGBAR_DEFAULT_TEXTCOLOR0 0xFFFFFF
#endif

#ifndef PROGBAR_DEFAULT_TEXTCOLOR1
  #define PROGBAR_DEFAULT_TEXTCOLOR1 0x000000
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
PROGBAR_PROPS PROGBAR_DefaultProps = {
  PROGBAR_DEFAULT_FONT,
  PROGBAR_DEFAULT_BARCOLOR0,
  PROGBAR_DEFAULT_BARCOLOR1,
  PROGBAR_DEFAULT_TEXTCOLOR0,
  PROGBAR_DEFAULT_TEXTCOLOR1
};

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _Value2Pos
*/
static int _Value2Pos(const PROGBAR_Obj* pObj, int v) {
  int IsVertical = pObj->Flags & PROGBAR_CF_VERTICAL;
  int EffectSize = pObj->Widget.pEffect->EffectSize;
  int Size  = IsVertical ? pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0 + 1 : pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0 + 1;
  int Min   = pObj->Min;
  int Max   = pObj->Max;
  if (v < Min) {
	  v = Min;
  }
  if (v > Max) {
	  v = Max;
  }
  return EffectSize + ((Size - 2 * EffectSize) * (I32)(IsVertical ? Max - v : v - Min)) / (Max - Min);
}

/*********************************************************************
*
*       _DrawPart
*/
static void _DrawPart(const PROGBAR_Obj* pObj, int Index,
										  int xText, int yText, const char* pText) {
  LCD_SetBkColor(pObj->Props.aBarColor[Index]);
  LCD_SetColor(pObj->Props.aTextColor[Index]);
  GUI_Clear();
  if (pText) {
    GUI_GotoXY(xText, yText);
    GUI_DispString(pText);
  }
}

/*********************************************************************
*
*       _Paint
*/
static void _Paint(PROGBAR_Obj * pObj) {
  GUI_RECT r, rInside, rClient, rText;
  const char* pText;
  char ac[5];
  int tm, Pos;
  WM_GetClientRect(&rClient);
  GUI__ReduceRect(&rInside, &rClient, pObj->Widget.pEffect->EffectSize);
  Pos  = _Value2Pos(pObj, pObj->v);
  if (pObj->Flags & PROGBAR_CF_VERTICAL) {
    /* Draw lower part */
    r    = rInside;
    r.y0 = Pos;
    WM_SetUserClipArea(&r);
    _DrawPart(pObj, 0, 0, 0, 0);
    /* Draw upper part */
    r    = rInside;
    r.y1 = Pos - 1;
    WM_SetUserClipArea(&r);
    _DrawPart(pObj, 1, 0, 0, 0);
  } else {
    pText = PROGBAR__GetText(pObj, ac);
    GUI_SetFont(pObj->Props.pFont);
    PROGBAR__GetTextRect(pObj, &rText, pText);
    tm = GUI_SetTextMode(GUI_TM_TRANS);
    /* Draw left bar */
    r    = rInside;
    r.x1 = Pos - 1;
    WM_SetUserClipArea(&r);
    _DrawPart(pObj, 0, rText.x0, rText.y0, pText);
    /* Draw right bar */
    r    = rInside;
    r.x0 = Pos;
    WM_SetUserClipArea(&r);
    _DrawPart(pObj, 1, rText.x0, rText.y0, pText);
  }
  WM_SetUserClipArea(NULL);
  GUI_SetTextMode(tm);
  WIDGET__EFFECT_DrawDownRect(&pObj->Widget, &rClient);
}

/*********************************************************************
*
*       _Delete
*/
static void _Delete(PROGBAR_Handle hObj) {
  PROGBAR__FreeText(hObj);
}

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       PROGBAR_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
PROGBAR_Obj * PROGBAR_h2p(PROGBAR_Handle h) {
  PROGBAR_Obj * p = (PROGBAR_Obj *)GUI_ALLOC_h2p(h);
  if (p) {
    if (p->DebugId != PROGBAR_ID) {
      GUI_DEBUG_ERROROUT("PROGBAR.c: Wrong handle type or Object not init'ed");
      return 0;
    }
  }
  return p;
}
#endif

/*********************************************************************
*
*       PROGBAR__FreeText
*/
void PROGBAR__FreeText(PROGBAR_Handle hObj) {
  PROGBAR_Obj* pObj = PROGBAR_H2P(hObj);
  GUI_ALLOC_FreePtr(&pObj->hpText);
}

/*********************************************************************
*
*       PROGBAR__GetText
*/
const char * PROGBAR__GetText(const PROGBAR_Obj* pObj, char* pBuffer) {
  char* pText;
  if (pObj->hpText) {
    pText = (char*) GUI_ALLOC_h2p(pObj->hpText);
  } else {
    pText = pBuffer;
    GUI_AddDecMin((100 * (I32)(pObj->v - pObj->Min)) / (pObj->Max - pObj->Min), &pBuffer);
    *pBuffer++ = '%';
		*pBuffer   = 0;
	}
  return (const char*)pText;
}

/*********************************************************************
*
*       PROGBAR__GetTextRect
*/
void PROGBAR__GetTextRect(const PROGBAR_Obj* pObj, GUI_RECT* pRect, const char* pText) {
  int xSize      = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0 + 1;
  int ySize      = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0 + 1;
  int TextWidth  = GUI_GetStringDistX(pText);
  int TextHeight = GUI_GetFontSizeY();
  int EffectSize = pObj->Widget.pEffect->EffectSize;
  switch (pObj->TextAlign & GUI_TA_HORIZONTAL) {
  case GUI_TA_CENTER:
    pRect->x0 = (xSize - TextWidth) / 2;
		break;
  case GUI_TA_RIGHT:
    pRect->x0 = xSize - TextWidth - 1 - EffectSize;
		break;
  default:
    pRect->x0 = EffectSize;
	}
  pRect->y0  = (ySize - TextHeight) / 2;
  pRect->x0 += pObj->XOff;
  pRect->y0 += pObj->YOff;
  pRect->x1  = pRect->x0 + TextWidth  - 1;
  pRect->y1  = pRect->y0 + TextHeight - 1;
}

/*********************************************************************
*
*       Exported routines:  Callback
*
**********************************************************************
*/
/*********************************************************************
*
*       PROGBAR_Callback
*/
void PROGBAR_Callback(WM_MESSAGE * pMsg) {
  PROGBAR_Handle hObj;
  PROGBAR_Obj * pObj;
  hObj = pMsg->hWin;
  pObj = (PROGBAR_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
  /* Let widget handle the standard messages */
  if (WIDGET_HandleActive(hObj, pMsg) == 0) {
    return;
  }
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _Paint(pObj);
    return;
  case WM_DELETE:
    _Delete(hObj);
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
*       PROGBAR_CreateEx
*/
PROGBAR_Handle PROGBAR_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                int WinFlags, int ExFlags, int Id)
{
  PROGBAR_Handle hObj;
  WM_LOCK();
  hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, PROGBAR_Callback,
                                sizeof(PROGBAR_Obj) - sizeof(WM_Obj));
  if (hObj) {
    PROGBAR_Obj* pObj;
    pObj = (PROGBAR_Obj *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
    /* init widget specific variables */
    WIDGET__Init(&pObj->Widget, Id, 0);
    WIDGET_SetEffect(hObj, &WIDGET_Effect_None); /* Standard effect for progbar: None */
    PROGBAR_INIT_ID(pObj);
    /* init member variables */
    pObj->Props        = PROGBAR_DefaultProps;
    pObj->TextAlign    = GUI_TA_CENTER;
    pObj->Max          = 100;
    pObj->Min          = 0;
    pObj->Flags        = ExFlags;
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
*       PROGBAR_SetValue
*/
void PROGBAR_SetValue(PROGBAR_Handle hObj, int v) {
  if (hObj) {
    PROGBAR_Obj* pObj;
    WM_LOCK();
    pObj= PROGBAR_H2P(hObj);
    /* Put v into legal range */
    if (v < pObj->Min) {
	    v = pObj->Min;
    }
    if (v > pObj->Max) {
	    v = pObj->Max;
    }
    if (pObj->v != v) {
      GUI_RECT r;
      if (pObj->Flags & PROGBAR_CF_VERTICAL) {
        /* Get y values */
        if (v < pObj->v) {
          r.y1 = _Value2Pos(pObj, v);
          r.y0 = _Value2Pos(pObj, pObj->v);
        } else {
          r.y1 = _Value2Pos(pObj, pObj->v);
          r.y0 = _Value2Pos(pObj, v);
        }
		    r.x0    = 0;
		    r.x1    = 4095;
        pObj->v = v;
      } else {
        /* Get x values */
        if (v < pObj->v) {
          r.x0 = _Value2Pos(pObj, v);
          r.x1 = _Value2Pos(pObj, pObj->v);
        } else {
          r.x0 = _Value2Pos(pObj, pObj->v);
          r.x1 = _Value2Pos(pObj, v);
        }
		    r.y0    = 0;
		    r.y1    = 4095;
        if (pObj->hpText == 0) {
          const GUI_FONT GUI_UNI_PTR * pOldFont;
          char acBuffer[5];
          GUI_RECT rText;
          pOldFont = GUI_SetFont(pObj->Props.pFont);
          PROGBAR__GetTextRect(pObj, &rText, PROGBAR__GetText(pObj, acBuffer));
          GUI_MergeRect(&r, &r, &rText);
          pObj->v = v;
          PROGBAR__GetTextRect(pObj, &rText, PROGBAR__GetText(pObj, acBuffer));
          GUI_MergeRect(&r, &r, &rText);
          GUI_SetFont(pOldFont);
        } else {
          pObj->v = v;
        }
      }
      WM_InvalidateRect(hObj, &r);
    }
    WM_UNLOCK();
  }
}

#else

void WIDGET_Progbar(void) {} /* avoid empty object files */

#endif /* GUI_WINSUPPORT */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
