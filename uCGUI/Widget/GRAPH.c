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
File        : GRAPH.c
Purpose     : Implementation of GRAPH widget
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "GRAPH_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
#ifndef   GRAPH_BKCOLOR_DEFAULT
  #define GRAPH_BKCOLOR_DEFAULT GUI_BLACK
#endif
#ifndef   GRAPH_BORDERCOLOR_DEFAULT
  #define GRAPH_BORDERCOLOR_DEFAULT 0xc0c0c0
#endif
#ifndef   GRAPH_FRAMECOLOR_DEFAULT
  #define GRAPH_FRAMECOLOR_DEFAULT GUI_WHITE
#endif
#ifndef   GRAPH_GRIDCOLOR_DEFAULT
  #define GRAPH_GRIDCOLOR_DEFAULT GUI_DARKGRAY
#endif
#ifndef   GRAPH_GRIDSPACING_X_DEFAULT
  #define GRAPH_GRIDSPACING_X_DEFAULT 50
#endif
#ifndef   GRAPH_GRIDSPACING_Y_DEFAULT
  #define GRAPH_GRIDSPACING_Y_DEFAULT 50
#endif
#ifndef   GRAPH_BORDER_L_DEFAULT
  #define GRAPH_BORDER_L_DEFAULT 0
#endif
#ifndef   GRAPH_BORDER_T_DEFAULT
  #define GRAPH_BORDER_T_DEFAULT 0
#endif
#ifndef   GRAPH_BORDER_R_DEFAULT
  #define GRAPH_BORDER_R_DEFAULT 0
#endif
#ifndef   GRAPH_BORDER_B_DEFAULT
  #define GRAPH_BORDER_B_DEFAULT 0
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
GRAPH_PROPS GRAPH__DefaultProps = {
  GRAPH_BKCOLOR_DEFAULT,
  GRAPH_BORDERCOLOR_DEFAULT,
  GRAPH_FRAMECOLOR_DEFAULT,
  GRAPH_GRIDCOLOR_DEFAULT,
  GRAPH_GRIDSPACING_X_DEFAULT,
  GRAPH_GRIDSPACING_Y_DEFAULT,
  GRAPH_BORDER_L_DEFAULT,
  GRAPH_BORDER_T_DEFAULT,
  GRAPH_BORDER_R_DEFAULT,
  GRAPH_BORDER_B_DEFAULT,
};

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawGrid
*/
static void _DrawGrid(GRAPH_OBJ * pObj, GRAPH_Handle hObj, unsigned BorderL, unsigned BorderT, unsigned BorderR, unsigned BorderB) {
  GUI_RECT Rect;
  U8 LineStyleOld;
  int i, xSize, ySize, xSpace, ySpace, EffectSize;
  WM_GetInsideRectExScrollbar(hObj, &Rect);
  EffectSize = pObj->Widget.pEffect->EffectSize;
  xSpace = pObj->Props.GridSpacingX;
  ySpace = pObj->Props.GridSpacingY;
  xSize = Rect.x1 - Rect.x0 + 1 - BorderL - BorderR;
  ySize = Rect.y1 - Rect.y0 + 1 - BorderT - BorderB;
  GUI_SetColor(pObj->Props.aColor[GRAPH_CI_GRID]);
  LineStyleOld = GUI_GetLineStyle();
  if (xSpace) {
    int FirstPosX;
    FirstPosX = xSpace + pObj->Props.GridOffX + ((pObj->Flags & GRAPH_CF_GRID_FIXED_X) ? 0 : pObj->ScrollStateH.v);
    while (FirstPosX >= xSpace) FirstPosX -= xSpace;
    GUI_SetLineStyle(pObj->LineStyleV);
    for (i = FirstPosX; i < xSize; i += xSpace) {
      int x, y0, y1;
      x  = i + BorderL + EffectSize;
      y0 = Rect.y0 + BorderT;
      y1 = Rect.y1 - BorderB;
      if (pObj->LineStyleV != GUI_LS_SOLID) {
        GUI_DrawLine(x, y0, x, y1);
      } else {
        GUI_DrawVLine(x, y0, y1);
      }
    }
  }
  if (ySpace) {
    int FirstPosY;
    FirstPosY = ySpace + pObj->Props.GridOffY - pObj->ScrollStateV.v;
    while (FirstPosY >= ySpace) FirstPosY -= ySpace;
    GUI_SetLineStyle(pObj->LineStyleH);
    for (i = FirstPosY; i < ySize; i += ySpace) {
      int y, x0, x1;
      y  = Rect.y1 - BorderB - i;
      x0 = Rect.x0 + BorderL;
      x1 = Rect.x1 - BorderR;
      if (pObj->LineStyleH != GUI_LS_SOLID) {
        GUI_DrawLine(x0, y, x1, y);
      } else {
        GUI_DrawHLine(y, x0, x1);
      }
    }
  }
  GUI_SetLineStyle(LineStyleOld);
}

/*********************************************************************
*
*       _DrawBorder
*/
static void _DrawBorder(GRAPH_OBJ * pObj, GRAPH_Handle hObj, unsigned BorderL, unsigned BorderT, unsigned BorderR, unsigned BorderB, GUI_RECT * pRectInvalid) {
  if (BorderL || BorderT || BorderR || BorderB) {
    int OrgX, OrgY, x0, y0, x1, y1, x0_Clear, y0_Clear, x1_Clear, y1_Clear;
    GUI_RECT Rect;
    int EffectSize;
    EffectSize = pObj->Widget.pEffect->EffectSize;
    WM_GetInsideRectExScrollbar(hObj, &Rect);
    OrgX = WM_GetOrgX();
    OrgY = WM_GetOrgY();
    x0 = Rect.x0 + BorderL;
    y0 = Rect.y0 + BorderT;
    x1 = Rect.x1 - BorderR;
    y1 = Rect.y1 - BorderB;
    GUI_SetBkColor(pObj->Props.aColor[GRAPH_CI_BORDER]);
    GUI_SetColor(pObj->Props.aColor[GRAPH_CI_FRAME]);
    if (BorderL) {
      x1_Clear = BorderL - 1 + EffectSize;
      if ((pRectInvalid->x0 - OrgX) <= x1_Clear) {
        x0_Clear = Rect.x0;
        y0_Clear = BorderT + EffectSize;
        y1_Clear = Rect.y1 - BorderB + 1;
        GUI_ClearRect(x0_Clear, y0_Clear, x1_Clear, y1_Clear);
        GUI_DrawVLine(x0 - 1, y0 - 1, y1 + 1);
      }
    }
    if (BorderT) {
      y1_Clear = BorderT - 1 + EffectSize;
      if ((pRectInvalid->y0 - OrgY) <= y1_Clear) {
        x0_Clear = Rect.x0;
        y0_Clear = Rect.y0;
        x1_Clear = Rect.x1;
        GUI_ClearRect(x0_Clear, y0_Clear, x1_Clear, y1_Clear);
        GUI_DrawHLine(y0 - 1, x0 - 1, x1 + 1);
      }
    }
    if (BorderR) {
      x0_Clear = Rect.x1 - BorderR + 1;
      if ((pRectInvalid->x1 - OrgX) >= x0_Clear) {
        y0_Clear = BorderT + EffectSize;
        x1_Clear = Rect.x1;
        y1_Clear = Rect.y1 - BorderB + 1;
        GUI_ClearRect(x0_Clear, y0_Clear, x1_Clear, y1_Clear);
        GUI_DrawVLine(x1 + 1, y0 - 1, y1 + 1);
      }
    }
    if (BorderB) {
      y0_Clear = Rect.y1 - BorderB + 1;
      if ((pRectInvalid->y1 - OrgY) >= y0_Clear) {
        x0_Clear = Rect.x0;
        x1_Clear = Rect.x1;
        y1_Clear = Rect.y1;
        GUI_ClearRect(x0_Clear, y0_Clear, x1_Clear, y1_Clear);
        GUI_DrawHLine(y1 + 1, x0 - 1, x1 + 1);
      }
    }
  }
}

/*********************************************************************
*
*       _GetParams
*/
static void _GetParams(GRAPH_OBJ * pObj, unsigned * pBorderL, unsigned * pBorderT, unsigned * pBorderR, unsigned * pBorderB, unsigned * pEffectSize) {
  *pBorderL    = pObj->Props.BorderL;
  *pBorderT    = pObj->Props.BorderT;
  *pBorderR    = pObj->Props.BorderR;
  *pBorderB    = pObj->Props.BorderB;
  *pEffectSize = pObj->Widget.pEffect->EffectSize;
}

/*********************************************************************
*
*       _OnPaint
*/
static void _OnPaint(GRAPH_OBJ * pObj, GRAPH_Handle hObj, GUI_RECT * pRectInvalid) {
  unsigned BorderL, BorderT, BorderR, BorderB, EffectSize, i, NumItems;
  GUI_RECT RectClient, Rect;
  _GetParams(pObj, &BorderL, &BorderT, &BorderR, &BorderB, &EffectSize);
  WM_GetClientRect(&RectClient);
  WIDGET__EFFECT_DrawDownRect(&pObj->Widget, &RectClient);
  Rect = RectClient;
  Rect.y0 += BorderT + EffectSize;
  Rect.y1 -= BorderB + EffectSize;
  Rect.x0 += BorderL + EffectSize;
  Rect.x1 -= BorderR + EffectSize;
  WM_SetUserClipRect(&Rect);
  GUI_SetBkColor(pObj->Props.aColor[GRAPH_CI_BK]);
  GUI_Clear();
  if (pObj->pUserDraw) {
    pObj->pUserDraw(hObj, GRAPH_DRAW_FIRST);
  }
  if (pObj->ShowGrid) {
    _DrawGrid(pObj, hObj, BorderL, BorderT, BorderR, BorderB);
  }
  NumItems = pObj->GraphArray.NumItems;
  for (i = 0; i < NumItems; i++) {
    WM_HMEM hMem;
    WM_HMEM * phMem;
    hMem  = GUI_ARRAY_GethItem(&pObj->GraphArray, i); /* Handle of array item */
    phMem = (WM_HMEM *)GUI_ALLOC_h2p(hMem);                      /* Pointer to handle */
    if (phMem) {
      GRAPH_DATA_Handle hDataObj;
      GRAPH_DATA_OBJ *  pDataObj;
      hDataObj = *(WM_HMEM *)phMem;
      pDataObj = (GRAPH_DATA_OBJ *)GUI_ALLOC_h2p(hDataObj);
      if (pDataObj) {
        pDataObj->PaintObj.pfOnPaint(hDataObj, pRectInvalid);
      }
    }
  }
  GUI__ReduceRect(&Rect, &RectClient, EffectSize);
  WM_SetUserClipRect(&Rect);
  _DrawBorder(pObj, hObj, BorderL, BorderT, BorderR, BorderB, pRectInvalid);
  NumItems = pObj->ScaleArray.NumItems;
  for (i = 0; i < NumItems; i++) {
    WM_HMEM hMem;
    WM_HMEM * phMem;
    hMem  = GUI_ARRAY_GethItem(&pObj->ScaleArray, i); /* Handle of array item */
    phMem = (WM_HMEM *)GUI_ALLOC_h2p(hMem);                      /* Pointer to handle */
    if (phMem) {
      GRAPH_SCALE_Handle hScaleObj;
      GRAPH_SCALE_OBJ *  pScaleObj;
      hScaleObj = *(WM_HMEM *)phMem;
      pScaleObj = (GRAPH_SCALE_OBJ *)GUI_ALLOC_h2p(hScaleObj);
      if (pScaleObj) {
        pScaleObj->PaintObj.pfOnPaint(hScaleObj, pRectInvalid);
      }
    }
  }
  WM_SetUserClipRect(0);
  if (pObj->pUserDraw) {
    pObj->pUserDraw(hObj, GRAPH_DRAW_LAST);
  }
}

/*********************************************************************
*
*       _OnDelete
*/
static void _OnDelete(GRAPH_OBJ * pObj) {
  int i, NumItems;
  NumItems = pObj->GraphArray.NumItems;
  for (i = 0; i < NumItems; i++) {
    WM_HMEM hMem;
    WM_HMEM * phMem;
    hMem  = GUI_ARRAY_GethItem(&pObj->GraphArray, i); /* Handle of array item */
    phMem = (WM_HMEM *)GUI_ALLOC_h2p(hMem);                      /* Pointer to handle */
    if (phMem) {
      GRAPH_DATA_Handle hDataObj;
      GRAPH_DATA_OBJ *  pDataObj;
      hDataObj = *(WM_HMEM *)phMem;
      pDataObj = (GRAPH_DATA_OBJ *)GUI_ALLOC_h2p(hDataObj);
      if (pDataObj) {
        pDataObj->PaintObj.pfOnDelete(hDataObj);
      }
      GUI_ALLOC_Free(hDataObj);
    }
  }
  NumItems = pObj->ScaleArray.NumItems;
  for (i = 0; i < NumItems; i++) {
    WM_HMEM hMem;
    WM_HMEM * phMem;
    hMem  = GUI_ARRAY_GethItem(&pObj->ScaleArray, i); /* Handle of array item */
    phMem = (WM_HMEM *)GUI_ALLOC_h2p(hMem);                      /* Pointer to handle */
    if (phMem) {
      GRAPH_SCALE_Handle hScaleObj;
      hScaleObj = *(WM_HMEM *)phMem;
      GUI_ALLOC_Free(hScaleObj);
    }
  }
  GUI_ARRAY_Delete(&pObj->GraphArray);
  GUI_ARRAY_Delete(&pObj->ScaleArray);
}

/*********************************************************************
*
*       _InvalidateGraph
*/
static void _InvalidateGraph(GRAPH_Handle hObj) {
  unsigned BorderL, BorderT, BorderR, BorderB, EffectSize;
  GUI_RECT Rect;
  GRAPH_OBJ * pObj;
  pObj = GRAPH_H2P(hObj);
  _GetParams(pObj, &BorderL, &BorderT, &BorderR, &BorderB, &EffectSize);
  WM_GetInsideRectExScrollbar(hObj, &Rect);
  Rect.x0 += BorderL;
  Rect.y0 += BorderT;
  Rect.x1 -= BorderR;
  Rect.y1 -= BorderB;
  WM_InvalidateRect(hObj, &Rect);
}

/*********************************************************************
*
*       _ManageScrollBars
*/
static void _ManageScrollBars(GRAPH_OBJ * pObj, GRAPH_Handle hObj) {
  int IsRequired, PageSize;
  GUI_RECT Rect;
  unsigned BorderL, BorderT, BorderR, BorderB, EffectSize;
  WM_GetInsideRectExScrollbar(hObj, &Rect);
  _GetParams(pObj, &BorderL, &BorderT, &BorderR, &BorderB, &EffectSize);
  PageSize   = Rect.y1 - Rect.y0 - BorderT - BorderB;
  IsRequired = ((int)pObj->RangeY > PageSize) ? 1 : 0;
  WM_SetScrollbarV(hObj, IsRequired);
  pObj->ScrollStateV.NumItems = pObj->RangeY;
  pObj->ScrollStateV.PageSize = PageSize;
  PageSize   = Rect.x1 - Rect.x0 - BorderL - BorderR;
  IsRequired = ((int)pObj->RangeX > PageSize) ? 1 : 0;
  WM_SetScrollbarH(hObj, IsRequired);
  pObj->ScrollStateH.NumItems = pObj->RangeX;
  pObj->ScrollStateH.PageSize = PageSize;
  WM_CheckScrollBounds(&pObj->ScrollStateV);
  WM_CheckScrollBounds(&pObj->ScrollStateH);
  WIDGET__SetScrollState(hObj, &pObj->ScrollStateV, &pObj->ScrollStateH);
}

/*********************************************************************
*
*       _SetGridOffY
*/
static unsigned _SetGridOffY(GRAPH_OBJ * pObj, GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (pObj) {
    OldValue = pObj->Props.GridOffY;
    if (pObj->Props.GridOffY != Value) {
      pObj->Props.GridOffY = Value;
      _InvalidateGraph(hObj);
    }
  }
  return OldValue;
}

/*********************************************************************
*
*       _OnScrollValueChanged
*/
static void _OnScrollValueChanged(GRAPH_OBJ * pObj, GRAPH_Handle hObj, WM_HWIN hScroll) {
  int Value, Id;
  Value = SCROLLBAR_GetValue(hScroll);
  Id = WM_GetId(hScroll);
  if (Id == GUI_ID_VSCROLL) {
    pObj->ScrollStateV.v = Value;
  } else {
    pObj->ScrollStateH.v = Value;
  }
  WM_InvalidateWindow(hObj);
}

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       GRAPH_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
GRAPH_OBJ * GRAPH_h2p(GRAPH_Handle h) {
  GRAPH_OBJ * p = (GRAPH_OBJ *)GUI_ALLOC_h2p(h);
  if (p) {
    if (p->DebugId != GRAPH_ID) {
      GUI_DEBUG_ERROROUT("GRAPH.c: Wrong handle type or Object not init'ed");
      return 0;
    }
  }
  return p;
}
#endif

/*********************************************************************
*
*       Exported routines:  Private functions
*
**********************************************************************
*/
/*********************************************************************
*
*       GRAPH__AddValue
*/
void GRAPH__AddValue(GRAPH_DATA_OBJ * pDataObj, void * pData, void * pValue, int Size) {
  if (pDataObj->PaintObj.hGraph) {
    if (pDataObj->NumItems == pDataObj->MaxNumItems) {
      memmove((U8 *)pData, (U8 *)pData + Size, Size * (pDataObj->MaxNumItems - 1));
      memmove((U8 *)pData + Size * (pDataObj->NumItems - 1), pValue, Size);
    } else {
      memmove((U8 *)pData + Size * pDataObj->NumItems++, pValue, Size);
    }
    _InvalidateGraph(pDataObj->PaintObj.hGraph);
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
*       GRAPH_Callback
*/
void GRAPH_Callback (WM_MESSAGE * pMsg) {
  int Result, ManageScrollBars = 0;
  GRAPH_Handle hObj;
  GRAPH_OBJ  * pObj;
  hObj = pMsg->hWin;
  pObj = (GRAPH_OBJ *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
  /* Let widget handle the standard messages */
  if (pMsg->MsgId == WM_WIDGET_SET_EFFECT) {
    ManageScrollBars = 1;
  }
  Result = WIDGET_HandleActive(hObj, pMsg);
  if (ManageScrollBars) {
    _ManageScrollBars(pObj, hObj);
  }
  if (!Result) {
    return;
  }
  switch (pMsg->MsgId) {
  case WM_SIZE:
    _ManageScrollBars(pObj, hObj);
    break;
  case WM_PAINT:
    _OnPaint(pObj, hObj, (GUI_RECT *)pMsg->Data.p);
    return;
  case WM_DELETE:
    _OnDelete(pObj);
    return;
  case WM_NOTIFY_PARENT:
    switch (pMsg->Data.v) {
    case WM_NOTIFICATION_VALUE_CHANGED:
      if (pMsg->hWinSrc  == WM_GetScrollbarV(hObj)) {
        _OnScrollValueChanged(pObj, pMsg->hWin, pMsg->hWinSrc);
      } else if (pMsg->hWinSrc == WM_GetScrollbarH(hObj)) {
        _OnScrollValueChanged(pObj, pMsg->hWin, pMsg->hWinSrc);
      }
      break;
    case WM_NOTIFICATION_SCROLLBAR_ADDED:
      #if WIDGET_USE_PARENT_EFFECT
        WIDGET_SetEffect(pMsg->hWinSrc, pObj->Widget.pEffect);
      #endif
      break;
    }
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
*       GRAPH_CreateEx
*/
GRAPH_Handle GRAPH_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                            int WinFlags, int ExFlags, int Id)
{
  GRAPH_Handle hObj;
  WM_LOCK();
  hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, GRAPH_Callback,
                                sizeof(GRAPH_OBJ) - sizeof(WM_Obj));
  if (hObj) {
    GRAPH_OBJ * pObj;
    pObj = (GRAPH_OBJ *)GUI_ALLOC_h2p(hObj); /* Don't use use WIDGET_H2P because WIDGET_INIT_ID() has not be called at this point */
    /* Init sub-classes */
    GUI_ARRAY_CREATE(&pObj->GraphArray);
    GUI_ARRAY_CREATE(&pObj->ScaleArray);
    /* Init widget specific variables */
    WIDGET__Init(&pObj->Widget, Id, 0);
    /* Init member variables */
    GRAPH_INIT_ID(pObj);
    pObj->Props = GRAPH__DefaultProps;
    pObj->Flags = ExFlags;
  }
  WM_UNLOCK();
  return hObj;
}

/*********************************************************************
*
*       GRAPH_AttachData
*/
void GRAPH_AttachData(GRAPH_Handle hObj, GRAPH_DATA_Handle hData) {
  if (hObj) {
    GRAPH_OBJ * pObj;
    GRAPH_DATA_OBJ * pDataObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    pDataObj = (GRAPH_DATA_OBJ *)GUI_ALLOC_h2p(hData);
    pDataObj->PaintObj.hGraph = hObj;
    GUI_ARRAY_AddItem(&pObj->GraphArray, &hData, sizeof(GRAPH_DATA_Handle));
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       GRAPH_AttachScale
*/
void GRAPH_AttachScale(GRAPH_Handle hObj, GRAPH_SCALE_Handle hScale) {
  if (hObj) {
    GRAPH_OBJ * pObj;
    GRAPH_SCALE_OBJ * pScaleObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    pScaleObj = (GRAPH_SCALE_OBJ *)GUI_ALLOC_h2p(hScale);
    pScaleObj->PaintObj.hGraph = hObj;
    GUI_ARRAY_AddItem(&pObj->ScaleArray, &hScale, sizeof(GRAPH_SCALE_Handle));
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       GRAPH_SetColor
*/
GUI_COLOR GRAPH_SetColor(GRAPH_Handle hObj, GUI_COLOR Color, unsigned Index) {
  GUI_COLOR OldColor = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    if (Index < GUI_COUNTOF(pObj->Props.aColor)) {
      OldColor = pObj->Props.aColor[Index];
      if (pObj->Props.aColor[Index] != Color) {
        pObj->Props.aColor[Index] = Color;
        WM_InvalidateWindow(hObj);
      }
    }
    WM_UNLOCK();
  }
  return OldColor;
}

/*********************************************************************
*
*       GRAPH_SetGridVis
*/
unsigned GRAPH_SetGridVis(GRAPH_Handle hObj, unsigned OnOff) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->ShowGrid;
    if ((unsigned)pObj->ShowGrid != OnOff) {
      pObj->ShowGrid = OnOff ? 1 : 0;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetGridDistX
*/
unsigned GRAPH_SetGridDistX(GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->Props.GridSpacingX;
    if (pObj->Props.GridSpacingX != Value) {
      pObj->Props.GridSpacingX = Value;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetGridDistY
*/
unsigned GRAPH_SetGridDistY(GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->Props.GridSpacingY;
    if (pObj->Props.GridSpacingY != Value) {
      pObj->Props.GridSpacingY = Value;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetGridOffY
*/
unsigned GRAPH_SetGridOffY(GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (hObj) {
    WM_LOCK();
    OldValue = _SetGridOffY(GRAPH_H2P(hObj), hObj, Value);
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetBorder
*/
void GRAPH_SetBorder(GRAPH_Handle hObj, unsigned BorderL, unsigned BorderT, unsigned BorderR, unsigned BorderB) {
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    pObj->Props.BorderL = BorderL;
    pObj->Props.BorderT = BorderT;
    pObj->Props.BorderR = BorderR;
    pObj->Props.BorderB = BorderB;
    _ManageScrollBars(pObj, hObj);
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       GRAPH_SetUserDraw
*/
void GRAPH_SetUserDraw(GRAPH_Handle hObj, void (* pUserDraw)(WM_HWIN hObj, int Stage)) {
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    pObj->pUserDraw = pUserDraw;
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       GRAPH_SetVSizeX
*/
unsigned GRAPH_SetVSizeX(GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->RangeX;
    if (pObj->RangeX != Value) {
      pObj->RangeX = Value;
      _ManageScrollBars(pObj, hObj);
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetVSizeY
*/
unsigned GRAPH_SetVSizeY(GRAPH_Handle hObj, unsigned Value) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->RangeY;
    if (pObj->RangeY != Value) {
      pObj->RangeY = Value;
      _ManageScrollBars(pObj, hObj);
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetGridFixedX
*/
unsigned GRAPH_SetGridFixedX(GRAPH_Handle hObj, unsigned OnOff) {
  unsigned OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = (pObj->Flags & GRAPH_CF_GRID_FIXED_X) ? 1 : 0;
    if (OldValue & !OnOff) {
      pObj->Flags &= ~GRAPH_CF_GRID_FIXED_X;
      WM_InvalidateWindow(hObj);
    } else if (!OldValue & OnOff) {
      pObj->Flags |= GRAPH_CF_GRID_FIXED_X;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetLineStyleH
*/
U8 GRAPH_SetLineStyleH(GRAPH_Handle hObj, U8 Value) {
  U8 OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->LineStyleH;
    if (OldValue != Value) {
      pObj->LineStyleH = Value;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetLineStyleV
*/
U8 GRAPH_SetLineStyleV(GRAPH_Handle hObj, U8 Value) {
  U8 OldValue = 0;
  if (hObj) {
    GRAPH_OBJ * pObj;
    WM_LOCK();
    pObj = GRAPH_H2P(hObj);
    OldValue = pObj->LineStyleV;
    if (OldValue != Value) {
      pObj->LineStyleV = Value;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
  return OldValue;
}

/*********************************************************************
*
*       GRAPH_SetLineStyle
*/
void GRAPH_SetLineStyle(GRAPH_Handle hObj, U8 Value) {
  GRAPH_SetLineStyleV(hObj, Value);
  GRAPH_SetLineStyleH(hObj, Value);
}


#else                            /* Avoid problems with empty object modules */
  void GRAPH_C(void);
  void GRAPH_C(void) {}
#endif  /* #if GUI_WINSUPPORT */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
