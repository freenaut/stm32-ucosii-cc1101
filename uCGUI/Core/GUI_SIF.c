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
File        : GUI_SIF.c
Purpose     : Implementation of system independend fonts
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Private.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetNumCharAreas
*/
static int _GetNumCharAreas(const GUI_FONT GUI_UNI_PTR * pFont) {
  U16 NumCharAreas;
  const U8 * pData;
  pData = (const U8 *)pFont->p.pFontData + 14 /* Skip ID, YSize, YDist, Baseline, LHeight and CHeight */;
  NumCharAreas = GUI__Read16(&pData);
  return NumCharAreas;
}

/*********************************************************************
*
*       Exported code (private)
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_SIF__GetpCharInfo
*/
const U8 * GUI_SIF__GetpCharInfo(const GUI_FONT GUI_UNI_PTR * pFont, U16P c) {
  const U8 * pCharArea, * pCharInfo;
  int NumCharAreas;
  NumCharAreas = _GetNumCharAreas(pFont);                 /* Get # of char areas */
  pCharArea = (const U8 *)pFont->p.pFontData 
              + sizeof(GUI_SI_FONT);                      /* Set char area pointer to first char area */
  pCharInfo = pCharArea
              + sizeof(GUI_SIF_CHAR_AREA) * NumCharAreas; /* Set char info pointer to first character info */
  do {
    GUI_SIF_CHAR_AREA CharArea;
    CharArea.First    = GUI__Read16(&pCharArea);
    CharArea.Last     = GUI__Read16(&pCharArea);
    if ((c >= CharArea.First) && (c <= CharArea.Last)) {
      pCharInfo += sizeof(GUI_SIF_CHARINFO) * (c - CharArea.First);
      return pCharInfo;
    }
    pCharInfo += sizeof(GUI_SIF_CHARINFO) * (CharArea.Last - CharArea.First + 1);
  } while(--NumCharAreas);
  return 0;
}

/*********************************************************************
*
*       GUI_SIF__GetCharDistX
*/
int GUI_SIF__GetCharDistX(U16P c) {
  const U8 * pCharInfo;
  U16 DistX = 0;
  pCharInfo = GUI_SIF__GetpCharInfo(GUI_Context.pAFont, c); /* Get pointer to char info data */
  if (pCharInfo) {
    pCharInfo += 2;
    DistX = GUI__Read16(&pCharInfo);
  }
  return DistX;
}

/*********************************************************************
*
*       GUI_SIF__GetFontInfo
*/
void GUI_SIF__GetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi) {
  const U8 * pData;
  pData = (const U8 *)pFont->p.pFontData + 4 /* Skip XSize and XDist */;
  pfi->Baseline = GUI__Read16(&pData);
  pfi->LHeight  = GUI__Read16(&pData);
  pfi->CHeight  = GUI__Read16(&pData);
  pfi->Flags    = GUI_FONTINFO_FLAG_PROP;
}

/*********************************************************************
*
*       GUI_SIF__IsInFont
*/
char GUI_SIF__IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c) {
  const U8 * pCharInfo;
  GUI_USE_PARA(pFont);
  pCharInfo = GUI_SIF__GetpCharInfo(GUI_Context.pAFont, c);
  return (pCharInfo) ? 1 : 0;
}

/*********************************************************************
*
*       Exported code (public)
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_SIF_CreateFont
*/
void GUI_SIF_CreateFont(void * pFontData, GUI_FONT * pFont, const GUI_SIF_TYPE * pFontType) {
  const U8 * pSrc = (const U8 *)pFontData;
  U32 ID;
  /* Set pFontData of GUI_FONT structure to the first byte */
  pFont->p.pFontData = pFontData;
  /* Check ID */
  ID = GUI__Read32(&pSrc);
  if (ID != 0x50495547) { /* 'GUIP' */
    GUI_DEBUG_ERROROUT("ID of downloaded font wrong!");
    return;
  }
  /* Read properties of the font */
  pFont->YSize    = GUI__Read16(&pSrc);
  pFont->YDist    = GUI__Read16(&pSrc);
  pFont->Baseline = GUI__Read16(&pSrc);
  pFont->LHeight  = GUI__Read16(&pSrc);
  pFont->CHeight  = GUI__Read16(&pSrc);
  /* Magnification is always 1 */
  pFont->XMag = 1;
  pFont->YMag = 1;
  /* Set function pointers */
  pFont->pfDispChar     = pFontType->pDispChar;
  pFont->pfGetCharDistX = pFontType->pGetCharDistX;
  pFont->pfGetFontInfo  = pFontType->pGetFontInfo;
  pFont->pfIsInFont     = pFontType->pIsInFont;
  pFont->pafEncode      = 0;
  /* Use the new font */
  GUI_SetFont(pFont);
}

/*********************************************************************
*
*       GUI_SIF_DeleteFont
*/
void GUI_SIF_DeleteFont(GUI_FONT * pFont) {
  GUI_USE_PARA(pFont);
}
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
