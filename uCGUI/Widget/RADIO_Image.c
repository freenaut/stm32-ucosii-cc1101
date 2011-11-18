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
File        : RADIO_Image.c
Purpose     : Implementation of radio widget
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"
#include "RADIO_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/* Define colors */
#ifndef RADIO_BKCOLOR0_DEFAULT
  #define RADIO_BKCOLOR0_DEFAULT 0xc0c0c0           /* Inactive color */
#endif

#ifndef RADIO_BKCOLOR1_DEFAULT
  #define RADIO_BKCOLOR1_DEFAULT GUI_WHITE          /* Active color */
#endif

/*********************************************************************
*
*       Static const data
*
**********************************************************************
*/

/* Colors */
static const GUI_COLOR _aColorDisabled[] = {0xC0C0C0, 0x808080, 0x000000, RADIO_BKCOLOR0_DEFAULT};
static const GUI_COLOR _aColorEnabled[]  = {0xC0C0C0, 0x808080, 0x000000, RADIO_BKCOLOR1_DEFAULT};
static const GUI_COLOR _ColorsCheck[]    = {0xFFFFFF, 0x000000};

/* Palettes */
static const GUI_LOGPALETTE _PalRadioDisabled = {
  4,	/* number of entries */
  1, 	/* Transparency */
  _aColorDisabled
};

static const GUI_LOGPALETTE _PalRadioEnabled = {
  4,	/* number of entries */
  1, 	/* Transparency */
  _aColorEnabled
};

static const GUI_LOGPALETTE _PalCheck = {
  2,	/* number of entries */
  1, 	/* Transparency */
  &_ColorsCheck[0]
};

/* Pixel data */
#if   WIDGET_USE_SCHEME_SMALL
static const unsigned char _acRadio[] = {
  0x00, 0x55, 0x00,
  0x05, 0xAA, 0x50,
  0x1A, 0xFF, 0xAC,
  0x1B, 0xFF, 0xCC,
  0x6F, 0xFF, 0xF3,
  0x6F, 0xFF, 0xF3,
  0x6F, 0xFF, 0xF3,
  0x6F, 0xFF, 0xF3,
  0x1B, 0xFF, 0xCC,
  0x10, 0xFF, 0x0C,
  0x0F, 0x00, 0xF0,
  0x00, 0xFF, 0x00
};

static const unsigned char _acCheck[] = {
  _XX_____,
  XXXX____,
  XXXX____,
  _XX_____
};
#elif WIDGET_USE_SCHEME_MEDIUM
static const unsigned char _acRadio[] = {
  0x00, 0x15, 0x54, 0x00,
  0x01, 0x6A, 0xA9, 0x40,
  0x06, 0xBF, 0xFE, 0x90,
  0x1B, 0xFF, 0xFF, 0xCC,
  0x1B, 0xFF, 0xFF, 0xCC,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xF3,
  0x1B, 0xFF, 0xFF, 0xCC,
  0x1B, 0xFF, 0xFF, 0xCC,
  0x0C, 0x3F, 0xFC, 0x30,
  0x03, 0xC0, 0x03, 0xC0,
  0x00, 0x3F, 0xFC, 0x00,
};

static const unsigned char _acCheck[] = {
  _XXXX___,
  XXXXXX__,
  XXXXXX__,
  XXXXXX__,
  XXXXXX__,
  _XXXX___
};
#elif WIDGET_USE_SCHEME_LARGE
static const unsigned char _acRadio[] = {
  0x00, 0x00, 0x15, 0x54, 0x00, 0x00,
  0x00, 0x01, 0x6A, 0xA9, 0x40, 0x00,
  0x00, 0x16, 0xBF, 0xFE, 0x94, 0x00,
  0x00, 0x6B, 0xFF, 0xFF, 0xE9, 0x00,
  0x01, 0xBF, 0xFF, 0xFF, 0xFE, 0xC0,
  0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x30,
  0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x30,
  0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC,
  0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3,
  0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC,
  0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC,
  0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x30,
  0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x30,
  0x01, 0x3F, 0xFF, 0xFF, 0xFC, 0xC0,
  0x00, 0xC3, 0xFF, 0xFF, 0xC3, 0x00,
  0x00, 0x3C, 0x3F, 0xFC, 0x3C, 0x00,
  0x00, 0x03, 0xC0, 0x03, 0xC0, 0x00,
  0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00,
};

static const unsigned char _acCheck[] = {
  ___XXXX_, ________,
  __XXXXXX, ________,
  _XXXXXXX, X_______,
  XXXXXXXX, XX______,
  XXXXXXXX, XX______,
  XXXXXXXX, XX______,
  XXXXXXXX, XX______,
  _XXXXXXX, X_______,
  __XXXXXX, ________,
  ___XXXX_, ________,
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
const GUI_BITMAP RADIO__abmRadio[] = {
  { 12, 12, 3, 2, _acRadio, &_PalRadioDisabled},
  { 12, 12, 3, 2, _acRadio, &_PalRadioEnabled}
};

const GUI_BITMAP RADIO__bmCheck = {
  4, /* XSize */
  4, /* YSize */
  1, /* BytesPerLine */
  1, /* BitsPerPixel */
  _acCheck,   /* Pointer to picture data (indices) */
  &_PalCheck  /* Pointer to palette */
};
#elif WIDGET_USE_SCHEME_MEDIUM
const GUI_BITMAP RADIO__abmRadio[] = {
  { 16, 16, 4, 2, _acRadio, &_PalRadioDisabled},
  { 16, 16, 4, 2, _acRadio, &_PalRadioEnabled}
};

const GUI_BITMAP RADIO__bmCheck = {
  6, /* XSize */
  6, /* YSize */
  1, /* BytesPerLine */
  1, /* BitsPerPixel */
  _acCheck,   /* Pointer to picture data (indices) */
  &_PalCheck  /* Pointer to palette */
};
#elif WIDGET_USE_SCHEME_LARGE
const GUI_BITMAP RADIO__abmRadio[] = {
  { 24, 24, 6, 2, _acRadio, &_PalRadioDisabled},
  { 24, 24, 6, 2, _acRadio, &_PalRadioEnabled}
};

const GUI_BITMAP RADIO__bmCheck = {
  10, /* XSize */
  10, /* YSize */
  2,  /* BytesPerLine */
  1,  /* BitsPerPixel */
  _acCheck,   /* Pointer to picture data (indices) */
  &_PalCheck  /* Pointer to palette */
};
#endif

#else                            /* Avoid problems with empty object modules */
  void RADIO_Image_C(void);
  void RADIO_Image_C(void) {}
#endif

/************************* end of file ******************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
