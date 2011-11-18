/*
*********************************************************************************************************
*                                                uC/GUI
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
File        : GUIDEMO_Cursor.c
Purpose     : shows Cursor-API
----------------------------------------------------------------------
*/

#include "GUI.H"
#include "GUIDEMO.H"

#if (GUI_SUPPORT_CURSOR | GUI_SUPPORT_TOUCH)

/*********************************************************************
*
*       GUIDEMO_Cursor
*
**********************************************************************
*/

#define countof(Obj) (sizeof(Obj)/sizeof(Obj[0]))

static const GUI_CURSOR* _aCursor[] = { 
  &GUI_CursorArrowM,
  &GUI_CursorArrowMI,
  &GUI_CursorCrossM,
  &GUI_CursorCrossMI,
};

static void _MoveCursor(const GUI_CURSOR* Cursor){
  int x, y;
  int xMax, yMax;
  xMax = LCD_GetXSize()/2;
  yMax = LCD_GetYSize()/2;
  GUI_CURSOR_Select(Cursor);  
  for(x = y = 0; ((x != xMax)  && (y != yMax)  && !GUIDEMO_CheckCancel()); x += 4, y += 4) {
    if ( x == xMax) {
      x = xMax;
    }
    if ( y == yMax) {
      y = yMax;
    }
    GUI_CURSOR_SetPosition(x,y);
    GUIDEMO_Delay(75);
  }
}

static void _DispCursor(void) {
  GUI_CURSOR_Hide();
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontComic24B_1);  
  GUI_DispStringAt ("Available\n  cursors:"     ,  20,  30);
  GUI_SetFont(&GUI_FontComic18B_1);  
  /* Display the arrow cursor */
  GUI_DispStringAt ("arrow cursors:"         ,   0,  90);
  GUI_DrawBitmap   (&GUI_BitmapArrowS        ,  10, 110);
  GUI_DispStringAt ("S"                      ,  10, 140);
  GUI_DrawBitmap   (&GUI_BitmapArrowM        ,  40, 110);
  GUI_DispStringAt ("M"                      ,  40, 140);
  GUI_DrawBitmap   (&GUI_BitmapArrowL        ,  70, 110);
  GUI_DispStringAt ("L"                      ,  70, 140);
  /* Display the arrow cursor inverted */
  GUI_DispStringAt ("inverted arrow cursors:", 125,  90);
  GUI_DrawBitmap   (&GUI_BitmapArrowSI       , 145, 110);
  GUI_DispStringAt ("S"                      , 145, 140);
  GUI_DrawBitmap   (&GUI_BitmapArrowMI       , 175, 110);
  GUI_DispStringAt ("M"                      , 175, 140);
  GUI_DrawBitmap   (&GUI_BitmapArrowLI       , 205, 110);
  GUI_DispStringAt ("L"                      , 205, 140);
    /* Display the cross cursor */
  GUI_DispStringAt ("cross cursors:"         ,   0, 155);
  GUI_DrawBitmap   (&GUI_BitmapCrossS        ,  10, 185);
  GUI_DispStringAt ("S"                      ,  10, 200);
  GUI_DrawBitmap   (&GUI_BitmapCrossM        ,  40, 180);
  GUI_DispStringAt ("M"                      ,  40, 200);
  GUI_DrawBitmap   (&GUI_BitmapCrossL        ,  70, 175);
  GUI_DispStringAt ("L"                      ,  70, 200);
  /* Display the cross cursor inverted */
  GUI_DispStringAt ("inverted cross cursors:", 125, 155);
  GUI_DrawBitmap   (&GUI_BitmapCrossSI       , 145, 185);
  GUI_DispStringAt ("S"                      , 145, 200);
  GUI_DrawBitmap   (&GUI_BitmapCrossMI       , 175, 180);
  GUI_DispStringAt ("M"                      , 175, 200);
  GUI_DrawBitmap   (&GUI_BitmapCrossLI       , 205, 175);
  GUI_DispStringAt ("L"                      , 205, 200);
}

void GUIDEMO_Cursor(void) {
  int i = 0;
  GUIDEMO_ShowIntro("Cursor",
                    "\nuC/GUI supports"
                    "\nsoftware cursor");
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  _DispCursor();
  GUIDEMO_Delay(2000);
  GUIDEMO_NotifyStartNext();
  GUI_CURSOR_Show(); 
  for ( i = 0;(i < countof(_aCursor) && !GUIDEMO_CheckCancel()); i++) {
      _MoveCursor(_aCursor[i]);
  }
  GUI_CURSOR_SetPosition(0,0);
  GUI_CURSOR_Select(&GUI_CursorArrowM);  
}

#else

void GUIDEMO_Cursor(void) {}

#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
