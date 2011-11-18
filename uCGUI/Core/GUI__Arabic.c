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
File        : GUI__Arabic.c
Purpose     : Implementation of arabic routines
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"

#if GUI_SUPPORT_ARABIC

/*********************************************************************
*
*       
*
**********************************************************************
*/
typedef struct {
  U16 Isolated;
  U16 Final;
  U16 Initial;
  U16 Medial;
} KEY_INFO;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const KEY_INFO _aKeyInfo[] = {

/*        Base      Isol.   Final   Initial Medial */

  { /* 0  0x0621 */ 0xFE80, 0x0000, 0x0000, 0x0000 /* Arabic letter Hamza                 */ },
  { /* 1  0x0622 */ 0xFE81, 0xFE82, 0x0000, 0x0000 /* Arabic letter Alef with Madda above */ },
  { /* 2  0x0623 */ 0xFE83, 0xFE84, 0x0000, 0x0000 /* Arabic letter Alef with Hamza above */ },
  { /* 3  0x0624 */ 0xFE85, 0xFE86, 0x0000, 0x0000 /* Arabic letter Waw with Hamza above  */ },
  { /* 4  0x0625 */ 0xFE87, 0xFE88, 0x0000, 0x0000 /* Arabic letter Alef with Hamza below */ },
  { /* 5  0x0626 */ 0xFE89, 0xFE8A, 0xFE8B, 0xFE8C /* Arabic letter Yeh with Hamza above  */ },
  { /* 6  0x0627 */ 0xFE8D, 0xFE8E, 0x0000, 0x0000 /* Arabic letter Alef                  */ },
  { /* 7  0x0628 */ 0xFE8F, 0xFE90, 0xFE91, 0xFE92 /* Arabic letter Beh                   */ },
  { /* 8  0x0629 */ 0xFE93, 0xFE94, 0x0000, 0x0000 /* Arabic letter Teh Marbuta           */ },
  { /* 9  0x062A */ 0xFE95, 0xFE96, 0xFE97, 0xFE98 /* Arabic letter Teh                   */ },
  { /* 10 0x062B */ 0xFE99, 0xFE9A, 0xFE9B, 0xFE9C /* Arabic letter Theh                  */ },
  { /* 11 0x062C */ 0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0 /* Arabic letter Jeem                  */ },
  { /* 12 0x062D */ 0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4 /* Arabic letter Hah                   */ },
  { /* 13 0x062E */ 0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8 /* Arabic letter Khah                  */ },
  { /* 14 0x062F */ 0xFEA9, 0xFEAA, 0x0000, 0x0000 /* Arabic letter Dal                   */ },
  { /* 15 0x0630 */ 0xFEAB, 0xFEAC, 0x0000, 0x0000 /* Arabic letter Thal                  */ },
  { /* 16 0x0631 */ 0xFEAD, 0xFEAE, 0x0000, 0x0000 /* Arabic letter Reh                   */ },
  { /* 17 0x0632 */ 0xFEAF, 0xFEB0, 0x0000, 0x0000 /* Arabic letter Zain                  */ },
  { /* 18 0x0633 */ 0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4 /* Arabic letter Seen                  */ },
  { /* 19 0x0634 */ 0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8 /* Arabic letter Sheen                 */ },
  { /* 20 0x0635 */ 0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC /* Arabic letter Sad                   */ },
  { /* 21 0x0636 */ 0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0 /* Arabic letter Dad                   */ },
  { /* 22 0x0637 */ 0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4 /* Arabic letter Tah                   */ },
  { /* 23 0x0638 */ 0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8 /* Arabic letter Zah                   */ },
  { /* 24 0x0639 */ 0xFEC9, 0xFECA, 0xFECB, 0xFECC /* Arabic letter Ain                   */ },
  { /* 25 0x063A */ 0xFECD, 0xFECE, 0xFECF, 0xFED0 /* Arabic letter Ghain                 */ },
  { /* 26 0x0641 */ 0xFED1, 0xFED2, 0xFED3, 0xFED4 /* Arabic letter Feh                   */ },
  { /* 27 0x0642 */ 0xFED5, 0xFED6, 0xFED7, 0xFED8 /* Arabic letter Qaf                   */ },
  { /* 28 0x0643 */ 0xFED9, 0xFEDA, 0xFEDB, 0xFEDC /* Arabic letter Kaf                   */ },
  { /* 29 0x0644 */ 0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0 /* Arabic letter Lam                   */ },
  { /* 30 0x0645 */ 0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4 /* Arabic letter Meem                  */ },
  { /* 31 0x0646 */ 0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8 /* Arabic letter Noon                  */ },
  { /* 32 0x0647 */ 0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC /* Arabic letter Heh                   */ },
  { /* 33 0x0648 */ 0xFEED, 0xFEEE, 0x0000, 0x0000 /* Arabic letter Waw                   */ },
  { /* 34 0x0649 */ 0xFEEF, 0xFEF0, 0x0000, 0x0000 /* Arabic letter Alef Maksura          */ },
  { /* 35 0x064A */ 0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4 /* Arabic letter Yeh                   */ },
  { /* 36 0x067E */ 0xFB56, 0xFB57, 0xFB58, 0xFB59 /* Eastern arabic letter Peh           */ },
  { /* 37 0x0686 */ 0xFB7A, 0xFB7B, 0xFB7C, 0xFB7D /* Eastern arabic letter Tcheh         */ },
  { /* 38 0x0698 */ 0xFB8A, 0xFB8B, 0x0000, 0x0000 /* Eastern arabic letter Jeh           */ },
  { /* 39 0x06A9 */ 0xFB8E, 0xFB8F, 0xFB90, 0xFB91 /* Eastern arabic letter Keheh         */ },
  { /* 40 0x06AF */ 0xFB92, 0xFB93, 0xFB94, 0xFB95 /* Eastern arabic letter Gaf           */ },
  { /* 41 0x06CC */ 0xFBFC, 0xFBFD, 0x0000, 0x0000 /* Eastern arabic letter Farsi Yeh     */ },
};

/*********************************************************************
*
*       _GetTableIndex
*/
static int _GetTableIndex(U16 c) {
  if (c < 0x621) {
    return 0;
  }
  if (c > 0x6cc) {
    return 0;
  }
  if ((c >= 0x621) && (c <= 0x63a)) {
    return c - 0x621;
  }
  if ((c >= 0x641) && (c <= 0x64a)) {
    return c - 0x641 + 26;
  }
  if (c == 0x67e) {
    return 36;
  }
  if (c == 0x686) {
    return 37;
  }
  if (c == 0x698) {
    return 38;
  }
  if (c == 0x6a9) {
    return 39;
  }
  if (c == 0x6af) {
    return 40;
  }
  if (c == 0x6cc) {
    return 41;
  }
  return 0;
}

/*********************************************************************
*
*       _IsTransparent
*/
static int _IsTransparent(U16 c) {
  if (c >= 0x064b) {
    return 1;
  }
  if (c == 0x670) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       _GetLigature
*/
static int _GetLigature(U16 Char, U16 Next, int PrevAffectsJoining) {
  if (((Next != 0x622) && (Next != 0x623) && (Next != 0x625) && (Next != 0x627)) || (Char != 0x644)) {
    return 0;
  }
  if (PrevAffectsJoining) {
    switch (Next) {
    case 0x622:
      return 0xfef6;
    case 0x623:
      return 0xfef8;
    case 0x625:
      return 0xfefa;
    case 0x627:
      return 0xfefc;
    }
  } else {
    switch (Next) {
    case 0x622:
      return 0xfef5;
    case 0x623:
      return 0xfef7;
    case 0x625:
      return 0xfef9;
    case 0x627:
      return 0xfefb;
    }
  }
  return 0;
}

/*********************************************************************
*
*       _GetStringDistXArabic
*/
static int _GetStringDistXArabic(const char GUI_UNI_PTR *s, int MaxNumChars) {
  U16 Glyph, Char, PrevChar = 0, NextChar;
  int CharIsArabic, IgnoreNextCharacter = 0, xSizeArabic = 0;
  while (--MaxNumChars >= 0) {
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
        xSizeArabic += GUI_GetCharDistX(Glyph);
      } else {
        break;
      }
    }
    PrevChar = Char;
  }
  return xSizeArabic;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI__GetCursorCharacterArabic
*/
U16 GUI__GetCursorCharacterArabic(const char GUI_UNI_PTR * s, int Index, int MaxNumChars, int * pIsRTL) {
  int Ligature = 0, PrevIndex, PrevAffectsJoining;
  U16 Prev = 0, Char, Next = 0, Glyph;
  if (Index) {
    int NumChars = Index;
    while (--NumChars) {
      s += GUI_UC_GetCharSize(s);
    }
    Prev = GUI_UC__GetCharCodeInc(&s);
    Char = GUI_UC__GetCharCodeInc(&s);
  } else {
    int NumChars = Index;
    while (NumChars--) {
      s += GUI_UC_GetCharSize(s);
    }
    Char = GUI_UC__GetCharCodeInc(&s);
  }
  if (Index < (MaxNumChars - 1)) {
    Next = GUI_UC_GetCharCode(s);
  }
  PrevIndex = _GetTableIndex(Prev);
  PrevAffectsJoining = (_GetTableIndex(Prev) || _IsTransparent(Prev)) && (_aKeyInfo[PrevIndex].Medial);
  Ligature = _GetLigature(Prev, Char, PrevAffectsJoining);
  if (Ligature) {
    *pIsRTL = 1;
    return Ligature;
  }
  Glyph = GUI__GetPresentationForm(Char, Next, Prev, 0);
  if (Char != 0x20) {
    *pIsRTL = GUI__IsArabicCharacter(Char) ? 1 : 0;
  } else {
    *pIsRTL = GUI__IsArabicCharacter(Prev) ? 1 : 0;
  }
  return Glyph;
}

/*********************************************************************
*
*       GUI__IsArabicCharacter
*/
int GUI__IsArabicCharacter(U16 c) {
  return ((c >= 0x600) && (c <= 0x6ff)) ? 1 : 0;
}

/*********************************************************************
*
*       GUI__GetPresentationForm
*/
U16 GUI__GetPresentationForm(U16 Char, U16 Next, U16 Prev, int * pIgnoreNext) {
  int CharIndex, PrevIndex, NextAffectsJoining, PrevAffectsJoining, Final, Initial, Medial, Ligature;
  CharIndex = _GetTableIndex(Char);
  if (!CharIndex) {
    return Char;
  }
  PrevIndex = _GetTableIndex(Prev);
  PrevAffectsJoining = (_GetTableIndex(Prev) || _IsTransparent(Prev)) && (_aKeyInfo[PrevIndex].Medial);
  Ligature = _GetLigature(Char, Next, PrevAffectsJoining);
  if (!Ligature) {
    Ligature = _GetLigature(Prev, Char, PrevAffectsJoining);
  }
  if (Ligature) {
    if (pIgnoreNext) {
      *pIgnoreNext = 1;
    }
    return Ligature;
  } else {
    if (pIgnoreNext) {
     *pIgnoreNext = 0;
    }
  }
  NextAffectsJoining = (_GetTableIndex(Next) || _IsTransparent(Next)) && (_aKeyInfo[CharIndex].Medial);
  if ((!PrevAffectsJoining) && (!NextAffectsJoining)) {
    return _aKeyInfo[CharIndex].Isolated;
  } else if ((!PrevAffectsJoining) && (NextAffectsJoining)) {
    Initial = _aKeyInfo[CharIndex].Initial;
    if (Initial) {
      return Initial;
    } else {
      return _aKeyInfo[CharIndex].Isolated;
    }
  } else if ((PrevAffectsJoining) && (NextAffectsJoining)) {
    Medial = _aKeyInfo[CharIndex].Medial;
    if (Medial) {
      return Medial;
    } else {
      return _aKeyInfo[CharIndex].Isolated;
    }
  } else if ((PrevAffectsJoining) && (!NextAffectsJoining)) {
    Final = _aKeyInfo[CharIndex].Final;
    if (Final) {
      return Final;
    } else {
      return _aKeyInfo[CharIndex].Isolated;
    }
  }
  return Char;
}

/*********************************************************************
*
*       GUI__GetLineDistXArabic
*/
int GUI__GetLineDistXArabic(const char GUI_UNI_PTR *s, int MaxNumChars) {
  int Dist;
  Dist = 0;
  if (s) {
    U16 Glyph, Char, PrevChar = 0, NextChar;
    int CharIsArabic, IgnoreNextCharacter = 0;
    #if GUI_COMPILER_SUPPORTS_FP
      if (GUI_Context.pAFont->pafEncode) {
        return GUI_Context.pAFont->pafEncode->pfGetLineDistX(s, MaxNumChars);
      }
    #endif
    while (--MaxNumChars >= 0) {
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
        Dist += GUI_GetCharDistX(Glyph);
      }
      PrevChar = Char;
    }
  }
  return Dist;
}

/*********************************************************************
*
*       GUI__DispLineArabic
*/
void GUI__DispLineArabic(const char GUI_UNI_PTR * s, int MaxNumChars, const GUI_RECT * pRect) {
  if (GUI_Context.pClipRect_HL) {
    if (GUI_RectsIntersect(GUI_Context.pClipRect_HL, pRect) == 0) {
      return;
    }
  }
  #if GUI_COMPILER_SUPPORTS_FP
  if (GUI_Context.pAFont->pafEncode) {
    GUI_Context.pAFont->pafEncode->pfDispLine(s, MaxNumChars);
  } else {
  #else
  {
  #endif
    U16 Glyph, Char, PrevChar = 0, NextChar;
    int CharIsArabic, xPosArabicStart, IgnoreNextCharacter = 0, xSizeArabic;
    const char GUI_UNI_PTR * p;
    const char GUI_UNI_PTR * pNext;
    while (--MaxNumChars >= 0) {
      if (!PrevChar) {
        p        = s;
        Char     = GUI_UC__GetCharCodeInc(&s);
        if (Char) {
          pNext    = s;
          NextChar = GUI_UC__GetCharCodeInc(&s);
        }
      } else {
        p        = pNext;
        Char     = NextChar;
        if (Char) {
          pNext    = s;
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
        if ((!GUI_MoveRTL) && (CharIsArabic)) {
          /* Set GUI_MoveRTL to 1 and adjust cursor to the right position */
          xSizeArabic = _GetStringDistXArabic(p, MaxNumChars + 1);
          xPosArabicStart = GUI_GetDispPosX() + xSizeArabic;
          GUI_GotoX(xPosArabicStart);
          GUI_MoveRTL = 1;
        } else if ((GUI_MoveRTL) && (!CharIsArabic)) {
          /* Set GUI_MoveRTL to 0 and adjust cursor to the right position */
          GUI_GotoX(xPosArabicStart);
          GUI_MoveRTL  = 0;
        }
        /* Please leave the following lines for test purpose */
        #if 0
          GUI_SetBkColorIndex(GUI_GetBkColorIndex() + 3);
          GUI_SetTextMode(GUI_TM_NORMAL);
        #endif
        GUI_Context.pAFont->pfDispChar(Glyph);
        if (GUI_pfDispCharStyle) {
          GUI_pfDispCharStyle(Glyph);
        }
      }
      PrevChar = Char;
    }
    GUI_MoveRTL = 0;
  }
}

/*********************************************************************
*
*       GUI__GetCursorPosXArabic
*/
int GUI__GetCursorPosXArabic(const char GUI_UNI_PTR * s, int Index, int MaxNumChars) {
  U16 Glyph, Char, PrevChar = 0, NextChar, Cnt = 0;
  int CharIsArabic, ArabicMode = 0, IgnoreNextCharacter = 0, xSizeArabic, CursorPosXArabic = 0, LigaturPosition;
  const char GUI_UNI_PTR * p;
  const char GUI_UNI_PTR * pNext;
  while (--MaxNumChars >= 0) {
    if (!PrevChar) {
      p        = s;
      Char     = GUI_UC__GetCharCodeInc(&s);
      if (Char) {
        pNext    = s;
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    } else {
      p        = pNext;
      Char     = NextChar;
      if (Char) {
        pNext    = s;
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    }
    if (IgnoreNextCharacter) {
      IgnoreNextCharacter = 0;
      if (Cnt == Index) {
        return LigaturPosition;
      }
    } else {
      if (Char != 0x20) {
        CharIsArabic = GUI__IsArabicCharacter(Char);
      }
      if (CharIsArabic) {
        Glyph = GUI__GetPresentationForm(Char, NextChar, PrevChar, &IgnoreNextCharacter);
      } else {
        Glyph = Char;
      }
      if ((!ArabicMode) && (CharIsArabic)) {
        /* Set GUI_MoveRTL to 1 and adjust cursor to the right position */
        xSizeArabic = _GetStringDistXArabic(p, MaxNumChars + 1);
        CursorPosXArabic += xSizeArabic;
        ArabicMode = 1;
      } else if ((ArabicMode) && (!CharIsArabic)) {
        /* Set GUI_MoveRTL to 0 and adjust cursor to the right position */
        CursorPosXArabic += xSizeArabic;
        ArabicMode  = 0;
      }
      if (Cnt >= Index) {
        break;
      }
      if (IgnoreNextCharacter) {
        LigaturPosition = CursorPosXArabic;
      }
      if (CharIsArabic) {
        CursorPosXArabic -= GUI_GetCharDistX(Glyph);
      } else {
        CursorPosXArabic += GUI_GetCharDistX(Glyph);
      }
    }
    Cnt++;
    PrevChar = Char;
  }
  return CursorPosXArabic;
}

/*********************************************************************
*
*       GUI__GetCursorPosCharArabic
*/
int GUI__GetCursorPosCharArabic(const char GUI_UNI_PTR * s, int x, int MaxNumChars) {
  U16 Glyph, Char, PrevChar = 0, NextChar, Cnt = 0, NumChars;
  int CharIsArabic, ArabicMode = 0, IgnoreNextCharacter = 0, xSizeArabic, CursorPosXArabic = 0, CharWidth, NewPosX;
  const char GUI_UNI_PTR * p;
  const char GUI_UNI_PTR * pNext;
  NumChars = MaxNumChars;
  while (--MaxNumChars >= 0) {
    if (!PrevChar) {
      p        = s;
      Char     = GUI_UC__GetCharCodeInc(&s);
      if (Char) {
        pNext    = s;
        NextChar = GUI_UC__GetCharCodeInc(&s);
      }
    } else {
      p        = pNext;
      Char     = NextChar;
      if (Char) {
        pNext    = s;
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
      if ((!ArabicMode) && (CharIsArabic)) {
        /* Set GUI_MoveRTL to 1 and adjust cursor to the right position */
        xSizeArabic = _GetStringDistXArabic(p, MaxNumChars + 1);
        CursorPosXArabic += xSizeArabic;
        ArabicMode = 1;
      } else if ((ArabicMode) && (!CharIsArabic)) {
        /* Set GUI_MoveRTL to 0 and adjust cursor to the right position */
        CursorPosXArabic += xSizeArabic;
        ArabicMode  = 0;
      }
      CharWidth = GUI_GetCharDistX(Glyph);
      if (CharIsArabic) {
        NewPosX = CursorPosXArabic - CharWidth;
        if ((NewPosX <= x) && (CursorPosXArabic >= x)) {
          break;
        }
        CursorPosXArabic = NewPosX;
      } else {
        NewPosX = CursorPosXArabic + CharWidth;
        if ((NewPosX >= x) && (CursorPosXArabic <= x)) {
          break;
        }
        CursorPosXArabic = NewPosX;
      }
    }
    Cnt++;
    PrevChar = Char;
  }
  if (Cnt >= NumChars) {
    Cnt = NumChars - 1;
  }
  return Cnt;
}

#else

void GUI__Arabic_c(void);
void GUI__Arabic_c(void) { } /* avoid empty object files */

#endif

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
