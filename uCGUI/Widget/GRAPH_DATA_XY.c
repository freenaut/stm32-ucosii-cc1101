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
File        : GRAPH_DATA_XY.c
Purpose     : Implementation of YT graph
---------------------------END-OF-HEADER------------------------------
*/

#include <string.h>

#include "GRAPH_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  GRAPH_DATA_OBJ GraphDataObj;
  WM_HMEM        hData;
  U8             PenSize;
  U8             LineStyle;
} GRAPH_DATA_XY_OBJ;

/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawPolyLine
*/
static void _DrawPolyLine(const GUI_POINT * pPoints, int NumPoints, int x0, int y0) {
  unsigned PixelCnt;
  PixelCnt = 0;
  WM_ADDORG(x0,y0);
  WM_ITERATE_START(NULL); {
    GL_MoveTo(pPoints->x + x0, y0 - pPoints->y);
    while (--NumPoints >0) {
      pPoints++;
      GL_DrawLineToEx(pPoints->x + x0, y0 - pPoints->y, &PixelCnt);
    }
  } WM_ITERATE_END();
}

/*********************************************************************
*
*       _OnPaint
*/
static void _OnPaint(GRAPH_DATA_Handle hDataObj, GUI_RECT * pRect) {
  GUI_POINT         * pData;
  GRAPH_DATA_XY_OBJ * pDataXYObj;
  GUI_USE_PARA(pRect);
  pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
  pData = (GUI_POINT *)GUI_ALLOC_h2p(pDataXYObj->hData);
  if (pData) {
    GUI_RECT Rect;
    GRAPH_OBJ * pObj;
    int xPos, yPos;
    int BorderL, BorderT, BorderR, BorderB;
    pObj = GRAPH_H2P(pDataXYObj->GraphDataObj.PaintObj.hGraph);
    BorderL = pObj->Props.BorderL;
    BorderT = pObj->Props.BorderT;
    BorderR = pObj->Props.BorderR;
    BorderB = pObj->Props.BorderB;
    WM_GetInsideRectExScrollbar(pDataXYObj->GraphDataObj.PaintObj.hGraph, &Rect);
    Rect.x0 += BorderL;
    Rect.y0 += BorderT;
    Rect.x1 -= BorderR;
    Rect.y1 -= BorderB;
    xPos     = Rect.x0 + pDataXYObj->GraphDataObj.OffX;
    yPos     = Rect.y1 - pDataXYObj->GraphDataObj.OffY;
    pObj = GRAPH_H2P(pDataXYObj->GraphDataObj.PaintObj.hGraph);
    GUI_SetColor(pDataXYObj->GraphDataObj.Color);
    GUI_SetPenSize(pDataXYObj->PenSize);
    GUI_SetLineStyle(pDataXYObj->LineStyle);
    _DrawPolyLine(pData, pDataXYObj->GraphDataObj.NumItems, xPos + pObj->ScrollStateH.v, yPos + pObj->ScrollStateV.v);
  }
}

/*********************************************************************
*
*       _OnDelete
*/
static void _OnDelete(GRAPH_DATA_Handle hDataObj) {
  GRAPH_DATA_XY_OBJ * pDataXYObj;
  pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
  if (pDataXYObj->hData) {
    GUI_ALLOC_Free(pDataXYObj->hData);
  }
}

/*********************************************************************
*
*       _SetOffX
*/
static void _SetOffX(GRAPH_DATA_OBJ * pDataObj, int Value) {
  if (pDataObj) {
    if (pDataObj->OffX != Value) {
      pDataObj->OffX = Value;
      WM_Invalidate(pDataObj->PaintObj.hGraph);
    }
  }
}

/*********************************************************************
*
*       _SetOffY
*/
static void _SetOffY(GRAPH_DATA_OBJ * pDataObj, int Value) {
  if (pDataObj) {
    if (pDataObj->OffY != Value) {
      pDataObj->OffY = Value;
      WM_Invalidate(pDataObj->PaintObj.hGraph);
    }
  }
}

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       GRAPH_DATA_XY_Create
*/
GRAPH_DATA_Handle GRAPH_DATA_XY_Create(GUI_COLOR Color, unsigned MaxNumItems, GUI_POINT * pItems, unsigned NumItems) {
  GRAPH_DATA_Handle  hDataObj;
  WM_LOCK();
  hDataObj = GUI_ALLOC_AllocZero(sizeof(GRAPH_DATA_XY_OBJ));
  if (hDataObj) {
    GRAPH_DATA_XY_OBJ * pDataXYObj;
    WM_HMEM             hData;
    int                 BufferSize;
    BufferSize = sizeof(GUI_POINT) * MaxNumItems;
    hData = GUI_ALLOC_AllocZero(BufferSize);
    if (hData && pItems && NumItems) {
      void * p;
      p = GUI_ALLOC_h2p(hData);
      memcpy(p, pItems, sizeof(GUI_POINT) * NumItems);
    }
    pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
    pDataXYObj->hData = hData;
    pDataXYObj->PenSize = 1;
    pDataXYObj->GraphDataObj.Color               = Color;
    pDataXYObj->GraphDataObj.MaxNumItems         = MaxNumItems;
    pDataXYObj->GraphDataObj.NumItems            = NumItems;
    pDataXYObj->GraphDataObj.PaintObj.pfOnPaint  = _OnPaint;
    pDataXYObj->GraphDataObj.PaintObj.pfOnDelete = _OnDelete;
  }
  WM_UNLOCK();
  return hDataObj;
}

/*********************************************************************
*
*       GRAPH_DATA_XY_AddPoint
*/
void GRAPH_DATA_XY_AddPoint(GRAPH_DATA_Handle hDataObj, GUI_POINT * pPoint) {
  GRAPH_DATA_XY_OBJ * pDataXYObj;
  WM_LOCK();
  pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
  if (pDataXYObj) {
    GUI_POINT * pData;
    pData = (GUI_POINT *)GUI_ALLOC_h2p(pDataXYObj->hData);
    GRAPH__AddValue(&pDataXYObj->GraphDataObj, pData, pPoint, sizeof(GUI_POINT));
  }
  WM_UNLOCK();
}

/*********************************************************************
*
*       GRAPH_DATA_XY_SetOffX
*/
void GRAPH_DATA_XY_SetOffX(GRAPH_DATA_Handle hDataObj, int Off) {
  WM_LOCK();
  _SetOffX((GRAPH_DATA_OBJ *)GUI_ALLOC_h2p(hDataObj), Off);
  WM_UNLOCK();
}

/*********************************************************************
*
*       GRAPH_DATA_XY_SetOffY
*/
void GRAPH_DATA_XY_SetOffY(GRAPH_DATA_Handle hDataObj, int Off) {
  WM_LOCK();
  _SetOffY((GRAPH_DATA_OBJ *)GUI_ALLOC_h2p(hDataObj), Off);
  WM_UNLOCK();
}

/*********************************************************************
*
*       GRAPH_DATA_XY_SetPenSize
*/
void GRAPH_DATA_XY_SetPenSize(GRAPH_DATA_Handle hDataObj, U8 PenSize) {
  GRAPH_DATA_XY_OBJ * pDataXYObj;
  WM_LOCK();
  pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
  if (pDataXYObj) {
    if (pDataXYObj->PenSize != PenSize) {
      pDataXYObj->PenSize = PenSize;
      WM_InvalidateWindow(pDataXYObj->GraphDataObj.PaintObj.hGraph);
    }
  }
  WM_UNLOCK();
}

/*********************************************************************
*
*       GRAPH_DATA_XY_SetLineStyle
*/
void GRAPH_DATA_XY_SetLineStyle(GRAPH_DATA_Handle hDataObj, U8 LineStyle) {
  GRAPH_DATA_XY_OBJ * pDataXYObj;
  WM_LOCK();
  pDataXYObj = (GRAPH_DATA_XY_OBJ *)GUI_ALLOC_h2p(hDataObj);
  if (pDataXYObj) {
    if (pDataXYObj->LineStyle != LineStyle) {
      pDataXYObj->LineStyle = LineStyle;
      WM_InvalidateWindow(pDataXYObj->GraphDataObj.PaintObj.hGraph);
    }
  }
  WM_UNLOCK();
}

#else                            /* Avoid problems with empty object modules */
  void GRAPH_DATA_XY_C(void);
  void GRAPH_DATA_XY_C(void) {}
#endif  /* #if GUI_WINSUPPORT */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
