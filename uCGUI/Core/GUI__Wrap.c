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
File        : GUI__Wrap.c
Purpose     : Implementation of wrap routines
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _IsLineEnd
*/
static int _IsLineEnd(U16 Char) {
  if (!Char || (Char == '\n')) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       _GetNoWrap
*/
static int _GetNoWrap(const char GUI_UNI_PTR * s) {
  return GUI__GetLineNumChars(s, 0x7FFF);
}

#if (GUI_SUPPORT_ARABIC)

/*********************************************************************
*
*       _GetWordWrap (with Arabic support)
*/
static int _GetWordWrap(const char GUI_UNI_PTR * s, int xSize, int * pxDist) {
  int xDist = 0, NumChars = 0, WordWrap = 0, CharDist = 0;
  U16 Char;
  int IgnoreNextCharacter = 0, CharIsArabic;
  U16 Glyph, NextChar, PrevChar = 0, FirstChar = 0;
  while (1) {
    if (!PrevChar) {
      Char     = GUI_UC__GetCharCodeInc(&s);
      if (Char) {
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    } else {
      Char     = NextChar;
      if (Char) {
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    }
    if (IgnoreNextCharacter) {
      IgnoreNextCharacter = 0;
    } else {
      if (Char != 0x20) {
        CharIsArabic = GUI__IsArabicCharacter(Char);
      }
      if (CharIsArabic) {
        Glyph = GUI__GetPresentationForm(Char, NextChar, PrevChar, &IgnoreNextCharacter);
      } else {
        Glyph = Char;
      }
      if (!FirstChar) {
        FirstChar = Glyph;
      }
      /* Let's first check if the line end is reached. In this case we are done. */
      if (_IsLineEnd(Glyph)) {
        WordWrap = NumChars;
        break;
      }
      /* If the current character is part of leading spaces the position can be used for wrapping */
      if ((Glyph != ' ') && (FirstChar == ' ')) {
        FirstChar = 0;
        WordWrap = NumChars;
      } else if (0) {
        WordWrap = NumChars;

      } else if ((Glyph != ' ') && (PrevChar == ' ') && (CharIsArabic)) {
        WordWrap = NumChars;

      /* If the current character is a space and the previous character not, we found a wrapping position */
      } else if ((Glyph == ' ') && (Glyph != PrevChar)) {
        WordWrap = NumChars;
      /* If the current character is a trailing space the position can be used for wrapping */
      } else if ((Glyph == ' ') && (PrevChar == ' ')) {
        WordWrap = NumChars;
      }
      CharDist = GUI_GetCharDistX(Glyph);
      xDist += CharDist;
    }
    if ((xDist <= xSize) || (NumChars == 0)) {
      NumChars++;
    } else {
      break;
    }
    PrevChar = Char;
  }
  if (!WordWrap) {
    WordWrap = NumChars;
  }
  if (pxDist) {
    xDist -= CharDist;
    *pxDist = xDist;
  }
  return WordWrap;
}

/*********************************************************************
*
*       _GetCharWrap (with Arabic support)
*/
static int _GetCharWrap(const char GUI_UNI_PTR * s, int xSize) {
  int xDist = 0, NumChars = 0;
  U16 Char;
  int IgnoreNextCharacter = 0, CharIsArabic;
  U16 Glyph, NextChar, PrevChar = 0;
  while (1) {
    if (!PrevChar) {
      Char     = GUI_UC__GetCharCodeInc(&s);
      if (Char) {
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    } else {
      Char     = NextChar;
      if (Char) {
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    }
    if (!Char) {
      break;
    }
    if (IgnoreNextCharacter) {
      IgnoreNextCharacter = 0;
    } else {
      if (Char != 0x20) {
        CharIsArabic = GUI__IsArabicCharacter(Char);
      }
      if (CharIsArabic) {
        Glyph = GUI__GetPresentationForm(Char, NextChar, PrevChar, &IgnoreNextCharacter);
      } else {
        Glyph = Char;
      }
      xDist += GUI_GetCharDistX(Glyph);
      if ((NumChars && (xDist > xSize)) || (Glyph == '\n')) {
        break;
      }
    }
    NumChars++;
    PrevChar = Char;
  }
  return NumChars;
}

#else

/*********************************************************************
*
*       _GetWordWrap (without Arabic support)
*/
static int _GetWordWrap(const char GUI_UNI_PTR * s, int xSize, int * pxDist) {
  int xDist = 0, NumChars = 0, WordWrap = 0, CharDist = 0;
  U16 Char, PrevChar = 0, FirstChar = 0;
  while (1) {
    Char = GUI_UC__GetCharCodeInc(&s);   /* Similar to:  *s++ */
    if (!FirstChar) {
      FirstChar = Char;
    }
    /* Let's first check if the line end is reached. In this case we are done. */
    if (_IsLineEnd(Char)) {
      WordWrap = NumChars;
      break;
    }
    /* If the current character is part of leading spaces the position can be used for wrapping */
    if ((Char != ' ') && (FirstChar == ' ')) {
      FirstChar = 0;
      WordWrap = NumChars;
    /* If the current character is a space and the previous character not, we found a wrapping position */
    } else if ((Char == ' ') && (Char != PrevChar)) {
      WordWrap = NumChars;
    /* If the current character is a trailing space the position can be used for wrapping */
    } else if ((Char == ' ') && (PrevChar == ' ')) {
      WordWrap = NumChars;
    }
    PrevChar = Char;
    CharDist = GUI_GetCharDistX(Char);
    xDist += CharDist;
    if ((xDist <= xSize) || (NumChars == 0)) {
      NumChars++;
    } else {
      break;
    }
  }
  if (!WordWrap) {
    WordWrap = NumChars;
  }
  if (pxDist) {
    xDist -= CharDist;
    *pxDist = xDist;
  }
  return WordWrap;
}

/*********************************************************************
*
*       _GetCharWrap (without Arabic support)
*/
static int _GetCharWrap(const char GUI_UNI_PTR * s, int xSize) {
  int xDist = 0, NumChars = 0;
  U16 Char;
  while ((Char = GUI_UC__GetCharCodeInc(&s)) != 0) {
    xDist += GUI_GetCharDistX(Char);
    if ((NumChars && (xDist > xSize)) || (Char == '\n')) {
      break;
    }
    NumChars++;
  }
  return NumChars;
}

#endif /* GUI_SUPPORT_ARABIC */

/*********************************************************************
*
*       _WrapGetNumCharsDisp
*/
static int _WrapGetNumCharsDisp(const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode, int * pxDist) {
  int r;
  switch (WrapMode) {
  case GUI_WRAPMODE_WORD:
    r = _GetWordWrap(pText, xSize, pxDist);
    break;
  case GUI_WRAPMODE_CHAR:
    r = _GetCharWrap(pText, xSize);
    break;
  default:
    r = _GetNoWrap(pText);
  }
  return r;
}

/*********************************************************************
*
*       puplic code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI__WrapGetNumCharsDisp
*
* Returns:
*  Number of characters to display in the line.
*  Trailing spaces and line end character are
*  not counted
*/
int GUI__WrapGetNumCharsDisp(const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode) {
  return _WrapGetNumCharsDisp(pText, xSize, WrapMode, 0);
}

/*********************************************************************
*
*       GUI__WrapGetNumCharsToNextLine
*/
int GUI__WrapGetNumCharsToNextLine(const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode) {
  int NumChars;
  int xDist;
  U16 Char;
  NumChars = _WrapGetNumCharsDisp(pText, xSize, WrapMode, &xDist);
  pText   += GUI_UC__NumChars2NumBytes(pText, NumChars);
  Char     = GUI_UC__GetCharCodeInc(&pText);
  if (Char == '\n') {
    NumChars++;
  } else {
    if ((WrapMode == GUI_WRAPMODE_WORD) && (Char == ' ')) {
      int xDistChar = GUI_GetCharDistX(Char);
      while ((Char == ' ') && (xSize >= xDist)) {
        NumChars++;
        xDist += xDistChar;
        Char = GUI_UC__GetCharCodeInc(&pText);
      }
    }
  }
  return NumChars;
}

/*********************************************************************
*
*       GUI__WrapGetNumBytesToNextLine
*/
int GUI__WrapGetNumBytesToNextLine(const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode) {
  int NumChars, NumBytes;
  NumChars = GUI__WrapGetNumCharsToNextLine(pText, xSize, WrapMode);
  NumBytes = GUI_UC__NumChars2NumBytes(pText, NumChars);
  return NumBytes;
}

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
