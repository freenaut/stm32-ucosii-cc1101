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
File        : CHECKBOX_Image.c
Purpose     : Contains the default bitmap used by the checkbox widget
---------------------------END-OF-HEADER------------------------------
*/

#include "CHECKBOX.h"
#include "CHECKBOX_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Static const data
*
**********************************************************************
*/
/* Colors */
static const GUI_COLOR _aColorDisabled[] = {CHECKBOX_BKCOLOR0_DEFAULT, CHECKBOX_FGCOLOR0_DEFAULT};
static const GUI_COLOR _aColorEnabled[]  = {CHECKBOX_BKCOLOR1_DEFAULT, CHECKBOX_FGCOLOR1_DEFAULT};

/* Palettes */
static const GUI_LOGPALETTE _PalCheckDisabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorDisabled
};

static const GUI_LOGPALETTE _PalCheckEnabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorEnabled
};

/* Pixel data */
#if   WIDGET_USE_SCHEME_SMALL
static const unsigned char _acCheck[] = {
  ________, ________,
  ________, ________,
  ________, X_______,
  _______X, X_______,
  __X___XX, X_______,
  __XX_XXX, ________,
  __XXXXX_, ________,
  ___XXX__, ________,
  ____X___, ________,
  ________, ________,
  ________, ________
};
#elif WIDGET_USE_SCHEME_MEDIUM
static const unsigned char _acCheck[] = {
  ________, ________,
  ________, ________,
  ________, ________,
  ________, _____X__,
  ________, ____XX__,
  ________, ___XXX__,
  ________, __XXX___,
  ________, _XXX____,
  __X_____, XXX_____,
  __XX___X, XX______,
  __XXX_XX, X_______,
  ___XXXXX, ________,
  ____XXX_, ________,
  _____X__, ________,
  ________, ________,
  ________, ________
};
#elif WIDGET_USE_SCHEME_LARGE
static const unsigned char _acCheck[] = {
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, ___X____,
  ________, ________, __XX____,
  ________, ________, _XXX____,
  ________, ________, XXXX____,
  ________, _______X, XXX_____,
  ________, ______XX, XX______,
  ________, _____XXX, X_______,
  __X_____, ____XXXX, ________,
  __XX____, ___XXXX_, ________,
  __XXX___, __XXXX__, ________,
  __XXXX__, _XXXX___, ________,
  ___XXXX_, XXXX____, ________,
  ____XXXX, XXX_____, ________,
  _____XXX, XX______, ________,
  ______XX, X_______, ________,
  _______X, ________, ________,
  ________, ________, ________,
  ________, ________, ________,
  ________, ________, ________
};
#endif

/*********************************************************************
*
*       Exported const data
*
**********************************************************************
*/
/* Bitmaps */
#if   WIDGET_USE_SCHEME_SMALL
const GUI_BITMAP CHECKBOX__abmCheck[2] = {
  { 11, 11, 2, 1, _acCheck,  &_PalCheckDisabled},
  { 11, 11, 2, 1, _acCheck,  &_PalCheckEnabled }
};
#elif WIDGET_USE_SCHEME_MEDIUM
const GUI_BITMAP CHECKBOX__abmCheck[2] = {
  { 16, 16, 2, 1, _acCheck,  &_PalCheckDisabled},
  { 16, 16, 2, 1, _acCheck,  &_PalCheckEnabled }
};
#elif WIDGET_USE_SCHEME_LARGE
const GUI_BITMAP CHECKBOX__abmCheck[2] = {
  { 22, 22, 3, 1, _acCheck,  &_PalCheckDisabled},
  { 22, 22, 3, 1, _acCheck,  &_PalCheckEnabled }
};
#endif

#else                            /* Avoid problems with empty object modules */
  void CHECKBOX_Image_C(void);
  void CHECKBOX_Image_C(void) {}
#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
