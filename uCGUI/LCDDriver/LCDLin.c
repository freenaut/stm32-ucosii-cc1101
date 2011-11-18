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
File        : LCDLin.C
Purpose     : Driver for LCDs using a single Seiko Epson SED13XX controllers
              This version supports a single LCD controller
              in (almost) any hardware configuration. The following
              derivatives are currently supported:
              SED1352
              SED1354
              SED1356     (With/Without BitBlt engine support)
              SED1374
              SED1375
              SED13806
              Other Epson LCD controllers are very similar and could
              be covered by this driver as well, but have not been
              tested.
----------------------------------------------------------------------
Version-Date---Author-Explanation
----------------------------------------------------------------------
3.97    050810 JE     a) LCD_L0_ReadRect() optimized for LCD_MIRROR_X = 1, LCD_MIRROR_Y = 1
3.95d   050704 JE     a) LCD_L0_ReadRect() added
3.92    041021 JE     a) Optimized version of for LCD_L0_DrawVLine 2bpp, 16bit bus, LCD_SWAP_XY
                         and LCD_MIRROR_X now works with and without LCD_SWAP_BYTE_ORDER
3.90a   040824 JE     a) Optimization added for 2bpp, 16bit bus, LCD_SWAP_XY and LCD_MIRROR_X:
                         LCD_L0_DrawVLine and DrawBitLine1BPP_Swap
3.81    040510 JE     a) Support for bitBLT engine of S1D13A03-S1D13A05 added
3.76    040128 JE     a) Use of macros LCD_COPY_MEM16_THRESHOLD and LCD_COPY_MEM16 added
3.72    031209 JE     a) LUT initialisation for S1D13704 changed
                      b) No call of SwapBytes if buswidth = 8
3.70e   031103 JE     a) _Swap renamed to SwapBytes and changed from static to a non static
                         function to avoid compiler warnings.
3.70b   031103 SR/RS  a) Support for PC hardware, mode X (320 * 200) added. Controller Id: 1302
                         Tested with Watcom compiler
3.70a   031030 JE     a) Optimization added: DrawBitLine1BPP for 16 bit bus and 1bpp mode
3.54a   030617 JE     a) Driver renamed LCD13XX.c -> LCDLin.c
3.48    030504 RS     a) BITBLT_SET_DESTINATION modified to always use U32 in computation.
                         This is necessary in cases where a 16 bit CPU is used with a
                         display with more than 64k pixels if LCD_USE_32BIT_OFF not used.
3.00w   030422 JE     a) Capricorn 2 support in LCD_L0_SetLUTEntry changed
3.00v   030401 JE     a) Support for Capricorn 2 added
3.00U   030311 JE     a) Optimization added for 8bpp, 16bit bus, LCD_MIRROR_X:
                         DrawBitLine1BPP, DrawBitLine8BPP
3.00t   030123 JE     a) Superfluous variable from DrawBitLine1BPP removed
3.00s   020724 JE     a) LCD_L0_Init changed
3.00r   020708 JE     a) Changed to work with 2bpp DDP bitmaps
3.00q   020705 JE     a) Changed to work with 4bpp DDP bitmaps
3.00p   020705 JE     a) Optimizations changed: LCD_L0_DrawVLine
3.00o   020703 JE     a) Optimizations changed: LCD_L0_DrawVLine
3.00n   020624 JE     a) Optimizations added for 4bpp, 16bit bus, LCD_SWAPXY:
                         LCD_L0_DrawVLine, DrawBitLine1BPP_Swap, DrawBitLine4BPP_Swap
3.00m   020506 JE     a) LCD_FillRectBB changed to work with large
                         rectangles (width > 1024, heigth > 1024)
3.00l   020327 JE     a) DrawBitLine1BPP for 16bit bus, 4bpp:
                         Compiler problem solved
3.00k   020319 JE     a) DrawBitLine1BPP for 16bit bus, 4bpp optimized
3.00j   020222 JE     a) Optimized version DrawBitLine16BPP added
                      b) Macro mapping LCD_SWAP_BYTE_ORDER changed
3.00i   020222 JE     a) DrawBitLine1BPP_Swap, 16bit bus, 4bpp changed
                      b) LCD_L0_DrawVLine, 16bit bus, 4bpp changed
3.00h   020221 JE     a) LCD_USE_32BIT_PNT added to support 32Bit offsets
                      b) XY2OFF changed to work with 32Bit offsets
                      c) BITBLT_SET_DESTINATION changed to work with 32Bit offsets
3.00g   020221 JE     a) LCD_MSB_SHIFT & LCD_LSB_SHIFT removed
3.00f   020221 JE     a) Remaining LCD_READ_MEM removed
                      b) WRITE_MEM changed for swapped mode
3.00e   020218 JE     a) Use of READ_MEM, WRITE_MEM instead of
                         LCD_READ_MEM, LCD_WRITE_MEM
3.00d9  020129 JE     a) DrawBitLine4BPP: Non optimized version used if
                         defined LCD_LUT_SEG
                      b) LCD_L0_DrawVLine, LCD_L0_FillRect:
                         BitBLT-versions not used if
                         defined LCD_LUT_SEG or LCD_LUT_COM
                      c) LCD_DrawBitmap1BPPBB: Not called if
                         defined LCD_LUT_SEG or LCD_LUT_COM
                      d) BitBLT support not used if
                         defined LCD_LUT_SEG or LCD_LUT_COM
3.00d8  020129 JE     a) Bugfix: LCD_LUT_COM with 'space' now works
3.00d7  020124 JE     a) DrawBitLine1BPP, 16bit bus, 1bpp optimized
3.00df  020123 JE     a) LCD_L0_DrawHLine, 16bit bus, 1bpp,
                         LCD_SWAP_BYTE_ORDER added
3.00de  020123 JE     a) LCD_L0_DrawHLine, 16bit bus, 1bpp optimized
3.00d4  020104 JE     a) DrawBitLine8BPP, 16Bit bus, 4bpp optimized
3.00d3  011204 JE     a) BitBLT support changed
3.00d2  011129 RS     a) 4bpp opt. enabled also with LCD_LUT_COM
3.00d1  011115 JE     a) Some casts inserted to avoid warnings
3.00d   011109 JE     a) BitBLT support changed
3.00c9  011107 JE     a) LCD_L0_DrawHLine 16bpp, 16bit changed
                      b) BPP mapping 15->16bpp removed to keep sure
                         LCD_NUMCOLORS works right
3.00c8  011016 JE     a) Optimized DrawBitLine4BPP for 16 bit bus, 4 bpp mode
                         now supports LCD_SWAP_BYTE_ORDER
3.00c7  011016 RS     a) DrawBitLine4BPP for 16 bit bus, 4 bpp mode changed
3.00c6  011016 JE     a) Optimized version DrawBitLine4BPP for
                         16 bit bus, 4 bpp mode added
3.00c5  010926 JE     a) Dummy read from BITBLT memory area inserted
                         to shutdown BITBLT-engine after operation
                         is finished
3.00c4  010925 JE     a) #define SETNEXTPIXEL(c) SETNEXTPIXEL(c) deleted
                      b) LCD_L0_DrawVLine for 8bit BYW mode changed
                      c) GetPixelIndex changed
3.00c3  010925 JE     a) Bugfixes: 8bit bus, LCD_MIRROR_Y, LCD_L0_Init
3.00c2  010925 JE     a) Bugfixes: 16bit bus, LCD_MIRROR_Y
3.00c1  010914 RS     a) LCDInt.h always included
3.00c   010905 RS     a) DrawBitLine16BPP taken out via preprocessor
                         for non 16 bpp modes
3.00b3  010824 JE     a) DrawBitLine16BPP ok
3.00b2  010824 JE     a) DrawBitLine16BPP (no opt) added, not tested
3.00b1  010820 JE     a) Bugfixes for LCD_MIRROR_Y, 16 bit bus, 4 bpp
3.00b   010820 RS     a) Changes for 16bpp mode and non-EPSON controllers
3.00a4  010723 JE     a) Optimization added for LCD_L0_DrawVLine for
                         16 bit bus, 4 bpp, SWAP_XY, MIRROR_Y
3.00a3  010718 JE     a) Bugfix in LCD_L0_GetPixelIndex, LCD_SWAP_XY,
                         LCD_MIRROR_X and LCD_MIRROR_Y was not
                         supported
                      b) Bugfix: DrawBitLine8BPP, DrawBitLine4BPP:
                         LCD_SWAP_XY was not supported
3.00a2  010718 JE     a) Bugfix in LCD_L0_GetPixelIndex, optimized
                         version for LCD_BUSWIDTH == 8
3.00a1  010515 RS     a) SUperfluos compiler warnings eliminated
3.00a   010509 RS     a) LUT init for 1374 fixed
3.00    010430 RS     a) Optimizaion for 4bpp bitmaps in regular 8bpp
                         mode
2.02    010402 RS     a) LCD_GetDevCaps removed from driver
                         (now LCD.c)
2.00f   001030 RS     a) Optimization for 8bpp bitmaps without palette
                         in 16bit bus mode now also used if
                         COM LUT is being used -> mayor acceleration
                         if this configuration is used (VDO/BMW)
2.00e   001019 RS     a) Optimization for 8bpp bitmaps without palette
                         in 16bit bus mode now also used if
                         COM LUT is being used -> mayor acceleration
                         if this configuration is used (VDO/BMW)
2.00d   001017 RS     a) Video memory init removed from this driver
                         (normally done by LCD_Init in LCD.c)
2.00c   001013 RS     a) 8bbp dd-bitmaps in 16bit bus, 4bpp configuration
                         accelerated (for use with memory devices)
2.00b   000925 RS     a) Source cleaned up (no effect on code)
2.00a   000911 RS     a) LCD_DATAADR introduced
2.00    000525 RS     a) New interface supported.
                      b) Support for LCD_LUT_COM added
1.40a   000524 RS/JE  a) 1 BPP bitmap draw fix: Misbehaved if
                         Backgroundcolor == Color
1.40    000516 RS     a) LCD_GetColorIndex implemented
                      b) Support for simulation added
1.30l   000505 RS/JE  a) 8 bit DDB fix
1.30k   000428 JE     a) Optimized version of DrawBitLine8BPP (16 bit)
                         now draws transparent bitmaps
1.30j   000426 RS     a) Support for V1.30 interface
						             1) LCD_SetColorIndex, LCD_SetBkColorIndex
						                added
                         2) LCD_InitLUT,LCD_SetColor, LCD_SetBkColor
                            removed (now in LCD_COLOR)
                      b) Cleanup
1.30i   000416 RS     a) Small cleanups
1.30h   000411 RS     a) Optimized DrawHLine 8bpp, 8 bit bus
1.30g   000411 JE     a) Now all textmodes supported in
                         DrawBitLine1BPP for 1bpp mode
1.30f   000411 JE     a) Support for 1bpp mode added to LCD_L0_GetPixelIndex
                      b) Clipping in DrawBitLine1BPP for 1bpp mode changed
1.30e   000408 RS     a) 1BPP mode, 8bit bus accelerated
1.30d   000408 RS     a) LCD_SetNormalDispMode, LCD_SetReverseDispMode
                         eliminated
1.30c   000406 JE     a) Code generation: 133x controller excluded
1.30b   000404 RS     a) LCD_DrawHLine accelerated for 1BPP/8bitbus
1.30a   000403 RS     a) Support for 1bpp mode added with 16 bit bus
1.30    000402 RS     a) Interface changed: LCD_GetDevCaps
1.14j   000401 RS     a) Offset type changed to unsigned
1.14i   000327 RS     a) LCD_SetPaletteEntry renamed LCD_L0_SetLUTEntry
                         LCD_InitPalette renamed LCD_InitLUT
1.14h   000324 JE     a) DrawBitLine8BPP_DDP & DrawBitLine8BPP merged
1.14g   000323 RS     a) offets types changed from int to
                         unsigned int in order for 16-bit CPUs
                         to cope with bigger vmem areas
                      b) COLOR2INDEX macro created (simplifying
                         driver source)
                      c) LCD_SetBkColorIndex, LCD_SetColorIndex
                         created
1.14f   000322 RS     a) 1BPP mde supported
1.14e   000321 RS     a) 4BPP mode, 8 bit bus accelerated
1.14d	  000320 RS     a) Support for 1375 palette init added
1.14c   000306 JE     a) Support for SED13806 added
1.14b   991213 JE     a) LCD_DrawBitMap changed: If LCD_MIRROR_X==1 the
                         standard routine DrawBitLine1BPP has to be used
1.14a   991211 RS     a) 8-bit mode Swapped, y-mirror accelerated
                         (using SETNEXTPIXEL)
1.14    991206 RS     a) SWAP_XY modes supported, 8-bit bus modes
                         accelerated
1.12b   991201 RS     a) XY2OFF macro clean up: parameterorder changed
1.12a   991129 RS     a) Additional defaults for config switches defined
                         in order to keep config files simple
1.12    991128 RS     a) PIXELCOLOR definition moved to top in order to
                         avoid compiler warnings
                      b) Optimizations for 8 bit modes added
1.10.25 991113 RS     a) Comments brushed up
                      b) Cleanup in LCD_DrawBitmap1BPPBB
                      c) LCD_DrawBitLine1BPPBB eliminated, as its
                         functionality has been completely replaced
                         by LCD_DrawBitmap1BPPBB
                      d) Default for LCD_OPTIMIZE set to (1) for all
                         configurations
                      e) LCD_DrawBitmap1BPPBB would not necessarily
                         work correctly if the drawing area had been
                         reduced because of clipping to less than one
                         line. Fixed.
1.10.24 991022 JE     a) Support for LCD_COMTRANS & LCD_SEGTRANS completed
                      b) LUT init for SED1356 changed, reverse mode supported
1.10.23 991021 RS     a) Illegal optimization were used with LCD_COMTRANS
                         Fixed.
1.10.22 991021 JE     a) BitBLT support for SED1356 accellerated,
                         drawing 1 bitmap instead of bitlines
                      b) LCD_TYPE_COMTRANS & LCD_TYPE_SEGTRANS (re)implemented
                      c) LCD_ENABLE_REG_ACCESS implemented to accellerate
                         BitBLT routines with register access
1.10.21 991020 JE     a) BitBLT support for SED1356 finished and cleand up
1.10.20 991019 JE/RS  a) LCD_SetPalette (re)implemented for 1354 controllers
1.10.19 991019 JE     a) SetBkColor/SetColor fix
                      b) LCD_DrawPixelAA added
1.10.18 991015 JE     a) BitBLT Support added for 8bpp and 15bpp color mode
1.10.17 991012 JE     a) LCD_DrawBitLine8BPP changed
1.10.16 991012 JE     a) LCD_DrawHLine changed
                      b) Color translation in 8bpp mode supported
1.10.15 991011 RS     a) LCD_DrawBitLine8BPP optimized for 16 bit bus, 8bpp
                      b) LCD_DrawHLine optimized for 16 bit bus, 8bpp
                      c) Color translation changed, PhysColors only used
                         when LCD_FIXEDPALETTE == 0
1.10.14 991011 JE     a) Optimizations in LCD_DrawHLine changed in 8 & 15b pp mode
1.10.13 991009 RS     a) LCD_DrawHLine cleaned up for easier readability
                      b) Support for 8 bit bus added
                      c) XY2OFF macro cleaned up, accelerated
                      d) Palettes reversed for all gray modes as default
                         (since most displays require current (or set bi ts)
                         in order to make a pixel dark
                      e) DrawBitLine2BPP: Bug with transparent bitmaps
                         fixed: The left 1-4 pixel were not drawn or not
                         drawn correctly
1.10.12 991008 JE     a) LCD_DrawHLine changed
                      b) renamed LCD_USE_OPT -> LCD_OPTIMIZE
                      c) LCD_LSB_SHIFT introduced in order to support
                         both big and little endian systems.
                      d) Result of XY2OFF changed from byte to word
1.10.11 991007 JE     a) Configuration switch LCD_USE_OPT included
                      b) X-clipping in DrawBitLine1BPP & DrawBitLine2BPP changed
                      c) LCD_DrawVLine changed, the last pixel of a line was not drawn
1.10.10 991007 JE     a) Support for clipping and XOR modes for 4bpp and 8bpp included
                      b) LCD_DrawVLine optimized
                      c) X-clipping in DrawBitLine8BPP changed
1.10.09 991005 JE     a) Support 4bpp and 8bpp included
                      b) Optimization for LCD_DrawHLine 8bpp and 4bpp
                      c) New LCD_SHIFT_DATA to shift data bits
                      d) Offset calculation in dependence of LCD_BUSWIDTH included
1.10.08 990928 RS     a) Optimization for LCD_DrawHLine
                      b) Various compile warnings eliminated
                      c) LCD_Color2Color15 renamed LCD_Color2Index15
1.10.07 990927 RS     a) Defaults for config switches added
                      b) LCD_SWAPXY --> LCD_SWAP_XY for compatibility
                      c) LCD_SetPalette eliminated
1.10.06 990924 JE/RS  a) BYTESPERLINE changed to work for a normal
                         system (BMW might require Config change)
1.10.05 990831 RS     a) Full support for transparent 16 color
                         bitmaps (bugfix)
1.10.04               Font support for true color
1.10.03
1.10.02 990713 RS     LCD_MEMSIZE calculation changed
1.10.01 990713 RS     Fix for Memsize in colormode
1.10    990711 RS     Support for Segment/Com Lookup tables added
1.00    990118 RS     First release.
---------------------------LIST OF CONFIG SWITCHES--------------------
The following is a list of additional configuration switches for this
driver. These switches might not be listed in the manual, because
the manual mainly covers the general config switches which are
supported by all drivers.
----------------------------------------------------------------------
define ----------------------Explanation------------------------------
LCD_SWAP_BYTE_ORDER          Activate if high low bytes are swapped
                             Default: 0
LCD_WRITE_MEM32(Off,Data32)  This macro accelerates display access
                             if defined by allowing the CPU to write
                             32 bits at a time to the controller.
                             (For 32 bits CPUs only and only if the
                             BUS interface unit can automatically
                             convert this to 2 16 bit accesses
LCD_OPTIMIZE                 Controls the use of optimized routines.
                             If 1, several (speed) optimizations are used.
                             Default: ON (1)
LCD_USE_BITBLT               This switch controls the use of optimized routines
                              with SED1356 bitblt engine.
                             If 1, the optimized routines with bitblt access are used.
                             The default value depends of LCD_BITSPERPIXEL: (4  ) -> 0, (8,15) -> 1
LCD_ENABLE_REG_ACCESS()
LCD_ENABLE_MEM_ACCESS()      In most systems (and with most LCD-controllers)
                             registers / memory can be accessed at
                             different addresses. However, in some
                             systems, it could be necessary to exec
                             code in order to be able to access the
                             registers or memory. This code should
                             then be placed in these macros (rather
                             than the actual access macros, which
                             would be slowed down)
LCD_DATAADR                  define adress if video memory can be treated
                             like regular memory
                             (will speed up driver)
LCD_USE_32BIT_OFF            required if a 16 bit CPU has to control more
                             video memory than can be handled with 16 bit offsets.
                             Forces 32 bit computations, which will make driver
                             bigger & slower.
LCD_COPY_MEM16(Dest, pSrc, NumItems) This macro defines a routine which is used
                             when displaying device dependend 8bpp bitmaps using a 16 bit bus.
                             It should copy 16 bit items and return 0 if successfull
                             and a value != 0 if it fails.
                             Parameter:
                             Dest     - Destination address
                             pSrc     - Void pointer to source data
                             NumItems - Number of 16 bit items to be copied
LCD_COPY_MEM16_THRESHOLD     Used to decide if the function defined by the macro
                             LCD_COPY_MEM16 should be used to copy the bitmap data.
                             It defines the minimum number of bytes to be copied
                             for using the routine.

----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
none
----------------------------------------------------------------------
Open issues
----------------------------------------------------------------------
None
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"


#if (LCD_CONTROLLER / 100 == 13) \
      && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

#if defined(WIN32) && (LCD_CONTROLLER == 1301)
  #include "LCDSIM.h"
#endif

#if defined (__WATCOMC__) && (LCD_CONTROLLER == 1302)
  #include <conio.h>
  #include <i86.h>
#endif

/*
        *********************************************************
        *                                                       *
        *           Compiler specific settings                  *
        *                                                       *
        *********************************************************

*/
#ifdef WIN32   /* Avoid warnings in MS-compiler */
  #pragma warning(disable : 4244)  /* warning C4244: '=' : conversion from 'long ' to 'unsigned char ', possible loss of data */
  #pragma warning(disable : 4761)  /* warning C4761: integral size mismatch in argument; conversion supplied */
#endif

/*
*********************************************************
*
*           Controller renumbering
*
*********************************************************

  EPSON decided to rename all of their controllers. In order to
  be able to work with old and new numbers, we simply map the old ones
  to the new ones.
*/
#if LCD_CONTROLLER == 1386
  #undef LCD_CONTROLLER
  #define LCD_CONTROLLER 13806
#endif


/*
        *********************************************************
        *                                                       *
        *           Defaults for configuration                  *
        *                                                       *
        *********************************************************

*/

/* Switch for support of multiple pages.
 Only available with certain LCD-controllers */
#ifndef   LCD_SUPPORT_PAGING
  #define LCD_SUPPORT_PAGING            (0)
#endif

#ifndef   LCD_NUM_CONTROLLERS
  #define LCD_NUM_CONTROLLERS           (1)
#endif

#ifndef   LCD_BUSWIDTH
  #define LCD_BUSWIDTH                  (16)
#endif

#ifndef   LCD_OPTIMIZE
  #define LCD_OPTIMIZE                (1)
#endif

#if (LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806) || (LCD_CONTROLLER == 1304)
  #ifndef LCD_USE_BITBLT
    #if ((LCD_BITSPERPIXEL == 16) || (LCD_BITSPERPIXEL == 8 )) && (LCD_MIRROR_Y == 0) && (LCD_SWAP_XY  == 0)
      #define LCD_USE_BITBLT            (1)
    #else
      #define LCD_USE_BITBLT            (0)
    #endif
  #else
    #if (LCD_MIRROR_Y)
      #error BITBLT engine does not support LCD_MIRROR_Y = 1!
    #endif
    #if (LCD_SWAP_XY)
      #error BITBLT engine does not support LCD_SWAP_XY = 1!
    #endif
  #endif
#else
  #define LCD_USE_BITBLT                (0)
#endif

#ifndef   LCD_ENABLE_REG_ACCESS
  #define LCD_ENABLE_REG_ACCESS()
#endif

#ifndef   LCD_ENABLE_MEM_ACCESS
  #define LCD_ENABLE_MEM_ACCESS()
#endif

#ifndef   STATIC
  #define STATIC static
#endif

#ifndef   LCD_DISPLAY_INDEX
  #define LCD_DISPLAY_INDEX 0
#endif

#ifndef   LCD_COPY_MEM16_THRESHOLD
  #define LCD_COPY_MEM16_THRESHOLD 50
#endif

#ifndef   LCD_CNF4
  #define LCD_CNF4 0
#endif

#ifndef   LCD_SET_ORG
  #define LCD_SET_ORG(x, y) GUI_USE_PARA(x); GUI_USE_PARA(y)
#endif


/*********************************************************************
*
*       Rename SwapBytes
*
* SwapBytes has been changed from static to a non static function,
* because it can not be evaluated easily if the routine will be used
* or not. To avoid compiler warnings it has been changed to a
* non static function. (JE)
*/
#if   (LCD_DISPLAY_INDEX == 0)
  #define SwapBytes SwapBytes_0
#elif (LCD_DISPLAY_INDEX == 1)
  #define SwapBytes SwapBytes_1
#elif (LCD_DISPLAY_INDEX == 2)
  #define SwapBytes SwapBytes_2
#elif (LCD_DISPLAY_INDEX == 3)
  #define SwapBytes SwapBytes_3
#elif (LCD_DISPLAY_INDEX == 4)
  #define SwapBytes SwapBytes_4
#else
  LCD_DISPLAY_INDEX > 4 not supported!
#endif

/*
        *********************************************************
        *                                                       *
        *           Defines for configuration simulation        *
        *                                                       *
        *********************************************************

*/
#if defined(WIN32)
#if !defined(USE_PC_HARDWARE)

#if   LCD_DISPLAY_INDEX == 1
  #define SIM_WriteMem8  SIM_WriteMem8_1
  #define SIM_WriteReg8  SIM_WriteReg8_1
  #define SIM_ReadMem8   SIM_ReadMem8_1
  #define SIM_ReadReg8   SIM_ReadReg8_1
  #define SIM_WriteMem16 SIM_WriteMem16_1
  #define SIM_WriteReg16 SIM_WriteReg16_1
  #define SIM_ReadMem16  SIM_ReadMem16_1
  #define SIM_ReadReg16  SIM_ReadReg16_1
#elif LCD_DISPLAY_INDEX == 2
  #define SIM_WriteMem8  SIM_WriteMem8_2
  #define SIM_WriteReg8  SIM_WriteReg8_2
  #define SIM_ReadMem8   SIM_ReadMem8_2
  #define SIM_ReadReg8   SIM_ReadReg8_2
  #define SIM_WriteMem16 SIM_WriteMem16_2
  #define SIM_WriteReg16 SIM_WriteReg16_2
  #define SIM_ReadMem16  SIM_ReadMem16_2
  #define SIM_ReadReg16  SIM_ReadReg16_2
#elif LCD_DISPLAY_INDEX == 3
  #define SIM_WriteMem8  SIM_WriteMem8_3
  #define SIM_WriteReg8  SIM_WriteReg8_3
  #define SIM_ReadMem8   SIM_ReadMem8_3
  #define SIM_ReadReg8   SIM_ReadReg8_3
  #define SIM_WriteMem16 SIM_WriteMem16_3
  #define SIM_WriteReg16 SIM_WriteReg16_3
  #define SIM_ReadMem16  SIM_ReadMem16_3
  #define SIM_ReadReg16  SIM_ReadReg16_3
#elif LCD_DISPLAY_INDEX == 4
  #define SIM_WriteMem8  SIM_WriteMem8_4
  #define SIM_WriteReg8  SIM_WriteReg8_4
  #define SIM_ReadMem8   SIM_ReadMem8_4
  #define SIM_ReadReg8   SIM_ReadReg8_4
  #define SIM_WriteMem16 SIM_WriteMem16_4
  #define SIM_WriteReg16 SIM_WriteReg16_4
  #define SIM_ReadMem16  SIM_ReadMem16_4
  #define SIM_ReadReg16  SIM_ReadReg16_4
#endif

  void SIM_WriteMem8(unsigned int Off, int Data);
  void SIM_WriteReg8(unsigned int Off, int Data);
  int  SIM_ReadMem8(unsigned int Off);
  int  SIM_ReadReg8(unsigned int Off);
  void SIM_WriteMem16(unsigned int Off, int Data);
  void SIM_WriteReg16(unsigned int Off, int Data);
  int  SIM_ReadMem16(unsigned int Off);
  int  SIM_ReadReg16(unsigned int Off);

  #undef  LCD_READ_MEM
  #undef  LCD_READ_REG
  #undef  LCD_WRITE_MEM
  #undef  LCD_WRITE_REG
  #undef  LCD_OFF
  #undef  LCD_ON
  #undef  LCD_ENABLE_REG_ACCESS
  #undef  LCD_ENABLE_MEM_ACCESS
  #undef  LCD_SET_ORG
  #define LCD_ENABLE_REG_ACCESS()
  #define LCD_ENABLE_MEM_ACCESS()
  #define LCD_OFF()
  #define LCD_ON()
  #define LCD_SET_ORG(x, y) GUI_USE_PARA(x); GUI_USE_PARA(y)

  #if LCD_BUSWIDTH==8
    #define LCD_READ_MEM(Off)       SIM_ReadMem8(Off)
    #define LCD_WRITE_MEM(Off,Data) SIM_WriteMem8(Off, Data)
    #define LCD_READ_REG(Off)       SIM_ReadReg8(Off)
    #define LCD_WRITE_REG(Off,Data) SIM_WriteReg8(Off, Data)
  #elif LCD_BUSWIDTH==16
    #define LCD_READ_MEM(Off)       SIM_ReadMem16(Off)
    #define LCD_WRITE_MEM(Off,Data) SIM_WriteMem16(Off, Data)
    #define LCD_READ_REG(Off)       SIM_ReadReg16(Off)
    #define LCD_WRITE_REG(Off,Data) SIM_WriteReg16(Off, Data)
  #endif
#elif defined(USE_PC_HARDWARE)
  void PC_WriteMem8(unsigned int Off, int Data);
  void PC_WriteReg8(unsigned int Off, int Data);
  int  PC_ReadMem8(unsigned int Off);
  int  PC_ReadReg8(unsigned int Off);
  void PC_WriteMem16(unsigned int Off, int Data);
  void PC_WriteReg16(unsigned int Off, int Data);
  int  PC_ReadMem16(unsigned int Off);
  int  PC_ReadReg16(unsigned int Off);

  #undef  LCD_READ_MEM
  #undef  LCD_READ_REG
  #undef  LCD_WRITE_MEM
  #undef  LCD_WRITE_REG
  #if LCD_BUSWIDTH==8
    #define LCD_READ_MEM(Off)       PC_ReadMem8(Off)
    #define LCD_WRITE_MEM(Off,Data) PC_WriteMem8(Off, Data)
    #define LCD_READ_REG(Off)       PC_ReadReg8(Off)
    #define LCD_WRITE_REG(Off,Data) PC_WriteReg8(Off, Data)
  #else
    #define LCD_READ_MEM(Off)       PC_ReadMem16(Off)
    #define LCD_WRITE_MEM(Off,Data) PC_WriteMem16(Off, Data)
    #define LCD_READ_REG(Off)       PC_ReadReg16(Off)
    #define LCD_WRITE_REG(Off,Data) PC_WriteReg16(Off, Data)
  #endif
#else
  #error Should not happen ...
#endif
#endif

/********************************************************
*
*           Remap Hardware macros
*
*********************************************************
*/

#ifndef WIN32
  #if GUI_NUM_LAYERS > 1 /* Use single display macros */
    #if LCD_DISPLAY_INDEX == 0       /* 1. display in a multi-display configuration */
      #define LCD_READ_MEM(Off)       LCD_READ_MEM_0(Off)
      #define LCD_WRITE_MEM(Off,Data) LCD_WRITE_MEM_0(Off,Data)
      #define LCD_READ_REG(Off)       LCD_READ_REG_0(Off)
      #define LCD_WRITE_REG(Off,Data) LCD_WRITE_REG_0(Off,Data)
    #elif LCD_DISPLAY_INDEX == 1     /* 2. display in a multi-display configuration */
      #define LCD_READ_MEM(Off)       LCD_READ_MEM_1(Off)
      #define LCD_WRITE_MEM(Off,Data) LCD_WRITE_MEM_1(Off,Data)
      #define LCD_READ_REG(Off)       LCD_READ_REG_1(Off)
      #define LCD_WRITE_REG(Off,Data) LCD_WRITE_REG_1(Off,Data)
    #elif LCD_DISPLAY_INDEX == 2     /* 3. display in a multi-display configuration */
      #define LCD_READ_MEM(Off)       LCD_READ_MEM_2(Off)
      #define LCD_WRITE_MEM(Off,Data) LCD_WRITE_MEM_2(Off,Data)
      #define LCD_READ_REG(Off)       LCD_READ_REG_2(Off)
      #define LCD_WRITE_REG(Off,Data) LCD_WRITE_REG_2(Off,Data)
    #elif LCD_DISPLAY_INDEX == 3     /* 4. display in a multi-display configuration */
      #define LCD_READ_MEM(Off)       LCD_READ_MEM_3(Off)
      #define LCD_WRITE_MEM(Off,Data) LCD_WRITE_MEM_3(Off,Data)
      #define LCD_READ_REG(Off)       LCD_READ_REG_3(Off)
      #define LCD_WRITE_REG(Off,Data) LCD_WRITE_REG_3(Off,Data)
    #elif LCD_DISPLAY_INDEX == 4     /* 5. display in a multi-display configuration */
      #define LCD_READ_MEM(Off)       LCD_READ_MEM_4(Off)
      #define LCD_WRITE_MEM(Off,Data) LCD_WRITE_MEM_4(Off,Data)
      #define LCD_READ_REG(Off)       LCD_READ_REG_4(Off)
      #define LCD_WRITE_REG(Off,Data) LCD_WRITE_REG_4(Off,Data)
    #else
      #error Not supported yet!
    #endif
  #endif
#endif

/*
        *********************************************************
        *                                                       *
        *           Macro calculations                          *
        *                                                       *
        *********************************************************
*/

/* To make life easier, assign physical x/y size */

#if !LCD_SWAP_XY
  #define LCD_XSIZE_P LCD_XSIZE
  #define LCD_YSIZE_P LCD_YSIZE
  #define LCD_VXSIZE_P LCD_VXSIZE
  #define LCD_VYSIZE_P LCD_VYSIZE
#else
  #define LCD_XSIZE_P LCD_YSIZE
  #define LCD_YSIZE_P LCD_XSIZE
  #define LCD_VXSIZE_P LCD_VYSIZE
  #define LCD_VYSIZE_P LCD_VXSIZE
#endif

#if   LCD_BITSPERPIXEL == 1
  #define BYTESPERLINE (LCD_VXSIZE_P/8)
  #define WORDSPERLINE (LCD_VXSIZE_P/16)
#elif   LCD_BITSPERPIXEL == 2
  #define BYTESPERLINE (LCD_VXSIZE_P/4)
  #define WORDSPERLINE (LCD_VXSIZE_P/8)
#elif   LCD_BITSPERPIXEL == 4
  #define BYTESPERLINE (LCD_VXSIZE_P/2)
  #define WORDSPERLINE (LCD_VXSIZE_P/4)
#elif LCD_BITSPERPIXEL == 8
  #define BYTESPERLINE (LCD_VXSIZE_P)
  #define WORDSPERLINE (LCD_VXSIZE_P/2)
#elif  (LCD_BITSPERPIXEL == 15)
  #define BYTESPERLINE (LCD_VXSIZE_P*2)
  #define WORDSPERLINE (LCD_VXSIZE_P)
#elif  (LCD_BITSPERPIXEL == 16)
  #define BYTESPERLINE (LCD_VXSIZE_P*2)
  #define WORDSPERLINE (LCD_VXSIZE_P)
#else
  #error This colordepth is not supported !!!
#endif

#ifndef LCD_USE_32BIT_OFF
  #if ((WORDSPERLINE * LCD_YSIZE) > 0xFFFF)
    #define LCD_USE_32BIT_OFF 1
  #else
    #define LCD_USE_32BIT_OFF 0
  #endif
#endif

#if LCD_USE_32BIT_OFF
  typedef unsigned long tOff;
#else
  typedef unsigned int  tOff;
#endif

/*
        *********************************************************
        *                                                       *
        *              Macros, standard                         *
        *                                                       *
        *********************************************************

These macros can be found in any LCD-driver as they serve purposes
that can be found in any class of LCD-driver (Like clipping).

*/

#define BKCOLOR LCD_BKCOLORINDEX
#define   COLOR LCD_COLORINDEX

/*
        *********************************************************
        *                                                       *
        *      Configuration switch checking                    *
        *                                                       *
        *********************************************************

Please be aware that not all configuration errors can be captured !

*/

/* Check number of controllers */
#if ((LCD_NUM_CONTROLLERS >1) || (LCD_NUM_CONTROLLERS <0))
  #error "More than 1 controller not supported by this driver"
#endif

#if ((LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806) || (LCD_CONTROLLER == 1304)) && (LCD_BUSWIDTH != 16)
  #error This controller does not work with 8-bit bus
#endif

#if (((LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806) || (LCD_CONTROLLER == 1304)) && LCD_USE_BITBLT && ((LCD_BITSPERPIXEL != 8) && (LCD_BITSPERPIXEL != 16)))
  #error BitBlt-Access only available for 8bpp and 16bpp mode
#endif

#if (LCD_CONTROLLER==1374)
  #if (LCD_BITSPERPIXEL == 8)
    #if (LCD_FIXEDPALETTE != 233)
      #error This controller supports only 233 palette in 8 bpp mode !
    #endif
  #endif
#endif





/*
        *********************************************************
        *                                                       *
        *       Macros for internal use                         *
        *                                                       *
        *********************************************************
*/

#if !defined (LCD_LUT_COM)
  #define LINE2COM(y) y
#else
  #define LINE2COM(y) LCD__aLine2Com0[y]
#endif

#if LCD_BUSWIDTH == 16
  #if   (LCD_BITSPERPIXEL == 16)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+(x))
  #elif (LCD_BITSPERPIXEL == 15)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+(x))
  #elif (LCD_BITSPERPIXEL ==  8)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+(x>>1))
  #elif (LCD_BITSPERPIXEL ==  4)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+(x>>2))
  #elif (LCD_BITSPERPIXEL ==  2)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+((x)>>3))
  #elif (LCD_BITSPERPIXEL ==  1)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)WORDSPERLINE+((x)>>4))
  #endif
#else
  #if   (LCD_BITSPERPIXEL == 16)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+((x)<<1))
  #elif (LCD_BITSPERPIXEL == 15)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+((x)<<1))
  #elif (LCD_BITSPERPIXEL ==  8)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+(x))
  #elif (LCD_BITSPERPIXEL ==  4)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+((x)>>1))
  #elif (LCD_BITSPERPIXEL ==  2)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+((x)>>2))
  #elif (LCD_BITSPERPIXEL ==  1)
    #define XY2OFF(x,y)    (tOff)((tOff)LINE2COM(y)*(tOff)BYTESPERLINE+((x)>>3))
  #endif
#endif

#if ((LCD_SWAP_BYTE_ORDER) && (LCD_BITSPERPIXEL <=8)) | ((LCD_SWAP_BYTE_ORDER==0) && (LCD_BITSPERPIXEL  > 8))
  #define READ_MEM(Off, Data)  Data = LCD_READ_MEM(Off); Data = ((Data & 0xff) << 8) + (Data >> 8)
  #define WRITE_MEM(Off, Data) LCD_WRITE_MEM(Off, (U16)(((Data & 0xff) << 8) + ((Data & 0xff00) >> 8)))
#else
  #define READ_MEM(Off, Data)  Data = LCD_READ_MEM(Off)
  #define WRITE_MEM(Off, Data) LCD_WRITE_MEM(Off, Data)
#endif

#if (LCD_SWAP_BYTE_ORDER | (LCD_BUSWIDTH == 8))
  #define READ_REG(Off, Data)  Data = LCD_READ_REG(Off)
  #define WRITE_REG(Off, Data) LCD_WRITE_REG(Off, Data)
#else
  #define READ_REG(Off, Data)  { Data = SwapBytes(LCD_READ_REG(Off)); }
  #define WRITE_REG(Off, Data) { LCD_WRITE_REG(Off, SwapBytes(Data)); }
#endif

/* No use of LCD_WRITE_MEM, LCD_READ_MEM, LCD_WRITE_REG, LCD_READ_REG from this point */

#if defined (LCD_LUT_COM)
  #if (LCD_MIRROR_Y)
    #error LCD_MIRROR_Y not supported with COMTrans !
  #endif
  #if (LCD_MIRROR_X)
    #error LCD_MIRROR_X not supported with COMTrans !
  #endif
#endif

#if (!defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define PHYS2LOG(x, y) x, y
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define PHYS2LOG(x, y) y, x
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define PHYS2LOG(x, y) x, LCD_YSIZE - 1 - (y)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define PHYS2LOG(x, y) y, LCD_XSIZE - 1 - (x)
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define PHYS2LOG(x, y) LCD_XSIZE - 1 - (x), y
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #define PHYS2LOG(x, y) LCD_YSIZE - 1 - (y), x
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY)
    #define PHYS2LOG(x, y) LCD_XSIZE - 1 - (x), LCD_YSIZE - 1 - (y)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY)
    #error This combination of mirroring/swapping not yet supported
  #endif
#else
  #define PHYS2LOG(x, y) LCD__aCol2Seg0[x], y
#endif

#define SETPIXEL(x, y, c)  _SetPixel    (PHYS2LOG(x, y), c)
#define GETPIXEL(x, y)     _GetPixelIndex(PHYS2LOG(x, y))
#define XORPIXEL(x, y)     XorPixel     (PHYS2LOG(x, y))

/*
        *********************************************************
        *                                                       *
        *       Register access routines                        *
        *                                                       *
        *********************************************************
*/
#if (LCD_CONTROLLER == 1374)
#if defined(LCD_READ_REG)
#if (LCD_BUSWIDTH == 8)
  #define READ_REG_BYTE(Off) LCD_READ_REG(Off)
#else
  #define READ_REG_BYTE(Off) _ReadRegByte(Off)
static U8 _ReadRegByte(int Off) {
  U16 Data = LCD_READ_REG((Off>>1));
  #if LCD_SWAP_BYTE_ORDER
    return (Off&1) ? Data>>8 : Data&255;
  #else
    return (Off&1) ? Data&255 : Data>>8;
  #endif
}
#endif
#endif
#endif


/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/

/*********************************************************************
*
*       SwapBytes
*
* Changed from static to a non static function to avoid compiler warnings.
*/
U16 SwapBytes(U16 Data);
U16 SwapBytes(U16 Data) {
  return (Data<<8) | (Data>>8);
}

#if !defined(LCD_SET_LUT_ENTRY)

#if   (LCD_CONTROLLER == 1301) /* Capricorn 2 */

/*********************************************************************
*
*       _SetLUTEntry_1301
*/
static void _SetLUTEntry_1301(U8 Pos, LCD_COLOR color) {
  U8 r = color & 0xff;
  U8 g = (color & 0xff00) >> 8;
  U8 b = (color & 0xff0000) >> 16;
  U32 LUTEntry = ((r >> 3) << 27) | ((g >> 3) << 22) | ((b >> 2) << 17);
  #ifndef WIN32
    #if   (LCD_DISPLAY_INDEX == 0)
      *(U32*)(LCD_LUT_0_VADR + 4 * Pos) = LUTEntry;
    #elif (LCD_DISPLAY_INDEX == 1)
      *(U32*)(LCD_LUT_1_VADR + 4 * Pos) = LUTEntry;
    #elif (LCD_DISPLAY_INDEX == 2)
      *(U32*)(LCD_LUT_2_VADR + 4 * Pos) = LUTEntry;
    #elif (LCD_DISPLAY_INDEX == 3)
      *(U32*)(LCD_LUT_3_VADR + 4 * Pos) = LUTEntry;
    #else
      #error Illegal Layer!
    #endif
  #else
    LCDSIM_SetLUTEntry(Pos, color, LCD_DISPLAY_INDEX);
  #endif
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1301(Pos, Color)

#elif (LCD_CONTROLLER == 1302) && defined (__WATCOMC__)

/*********************************************************************
*
*       _SetLUTEntry_1302
*/
static void _SetLUTEntry_1302(U8 Pos, LCD_COLOR color) {
  U8 r = color & 0xff;
  U8 g = (color & 0xff00) >> 8;
  U8 b = (color & 0xff0000) >> 16;
  outp(0x3c8, Pos);
  outp(0x3c9, r);
  outp(0x3c9, g);
  outp(0x3c9, b);
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1302(Pos, Color)

#elif (LCD_CONTROLLER == 1374)

/*********************************************************************
*
*       _SetLUTEntry_1374
*/
static void _SetLUTEntry_1374(U8 Pos, LCD_COLOR color) {
  int i;
  U16 aColorSep[3];
  for (i = 0; i < 3; i++) {
    aColorSep[i] = color &0xff;
    color >>= 8;
  }
  /* Convert 8 bit color seperation into index */
  for (i = 0; i < 3; i++) {
    aColorSep[i] = (U16)(aColorSep[i] + 8) / 17;
  }
  /* Write into palette register */
  LCD_ENABLE_REG_ACCESS();
  /* 4 bit palette */
  #if (LCD_BUSWIDTH == 16)
    WRITE_REG((0x14 >> 1), Pos * 0x101);   /* Select position */
    for (i = 0; i < 3; i++) {
      WRITE_REG((0x16 >> 1), ((U16)aColorSep[i] * 17) << 8);  /* 1375 expects high nibble, 1374 low nibble (so this works on both) */
    }
  #else
    WRITE_REG((0x15), Pos);   /* Select position */
    for (i = 0; i < 3; i++) {
      WRITE_REG((0x17), aColorSep[i]);
    }
  #endif
  LCD_ENABLE_MEM_ACCESS();
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1374(Pos, Color)

#elif (LCD_CONTROLLER == 1375)

/*********************************************************************
*
*       _SetLUTEntry_1375
*/
static void _SetLUTEntry_1375(U8 Pos, LCD_COLOR color) {
  int i;
  U16 aColorSep[3];
  for (i = 0; i < 3; i++) {
    aColorSep[i] = color &0xff;
    color >>= 8;
  }
  /* Convert 8 bit color seperation into index */
  for (i = 0; i < 3; i++) {
    aColorSep[i] = (U16)(aColorSep[i] + 8) / 17;
  }
  /* Write into palette register */
  LCD_ENABLE_REG_ACCESS();
  /* 4 bit palette */
  #if (LCD_BUSWIDTH == 16)
    WRITE_REG((0x14 >> 1), Pos * 0x101);   /* Select position */
    for (i = 0; i < 3; i++) {
      WRITE_REG((0x16 >> 1), ((U16)aColorSep[i] * 17) << 8);  /* 1375 expects high nibble, 1374 low nibble (so this works on both) */
    }
  #else
    WRITE_REG((0x15), Pos);   /* Select position */
    for (i = 0; i < 3; i++) {
      WRITE_REG((0x17), aColorSep[i] << 4);
    }
  #endif
  LCD_ENABLE_MEM_ACCESS();
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1375(Pos, Color)

#elif (LCD_CONTROLLER == 1376)

/*********************************************************************
*
*       _SetLUTEntry_1376
*/
static void _SetLUTEntry_1376(U8 Pos, LCD_COLOR color) {
  int i;
  U16 aColorSep[3];
  for (i = 0; i < 3; i++) {
    aColorSep[i] = color &0xff;
    color >>= 8;
  }
  /* Convert 8 bit color seperation into index */
  for (i = 0; i < 3; i++) {
    aColorSep[i] = (aColorSep[i]) / 4;
  }
  /* Write into palette register */
  LCD_ENABLE_REG_ACCESS();
  /* Write Green and Blue into regs 8/9 */
  WRITE_REG((0x8 >> 1), (aColorSep[1] << (8 + 2))| (aColorSep[2] << 2));
  /* Write Red into regs 0xa, index into 0xb */
  WRITE_REG(0xa >> 1, (Pos << 8)  | (aColorSep[0] << 2));   /* Select position */
  LCD_ENABLE_MEM_ACCESS();
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1376(Pos, Color)

#elif (LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13506) || \
      (LCD_CONTROLLER == 1386) || (LCD_CONTROLLER == 13806)

/*********************************************************************
*
*       _SetLUTEntry_1386_1356
*/
static void _SetLUTEntry_1386_1356(U8 Pos, LCD_COLOR color) {
  int i;
  U16 aColorSep[3];
  for (i = 0; i < 3; i++) {
    aColorSep[i] = color &0xff;
    color >>= 8;
  }
  /* Convert 8 bit color seperation into index */
  for (i = 0; i < 3; i++) {
    aColorSep[i] = (U16)(aColorSep[i] + 8) / 17;
  }
  /* Write into palette register */
  LCD_ENABLE_REG_ACCESS();
  #if (LCD_BUSWIDTH == 16)
    WRITE_REG(0x01e2 / 2, Pos);   /* Select position */
    for (i = 0; i < 3; i++) {
      WRITE_REG(0x01e4 / 2, aColorSep[i] << 4);
    }
  #else
    #error Not yet defined
  #endif
  LCD_ENABLE_MEM_ACCESS();
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1386_1356(Pos, Color)

#elif (LCD_CONTROLLER == 1304)

/*********************************************************************
*
*       _WriteReg16L
*/
static void _WriteReg16L(U32 Reg, U16 Value) {
  #if (LCD_CNF4 == 0)
    WRITE_REG( Reg      >> 1, Value);
  #else
    WRITE_REG((Reg + 2) >> 1, Value);
  #endif
}

/*********************************************************************
*
*       _WriteReg16H
*/
static void _WriteReg16H(U32 Reg, U16 Value) {
  #if (LCD_CNF4 == 0)
    WRITE_REG((Reg + 2) >> 1, Value);
  #else
    WRITE_REG( Reg      >> 1, Value);
  #endif
}

/*********************************************************************
*
*       _WriteReg32
*/
static void _WriteReg32(U32 Reg, U32 Value) {
  _WriteReg16L(Reg, (U16)Value);
  _WriteReg16H(Reg, (U16)(Value >> 16));
}

/*********************************************************************
*
*       _ReadReg16L
*/
static U16 _ReadReg16L(U32 Reg) {
  U16 Value;
  #if (LCD_CNF4 == 0)
    READ_REG( Reg      >> 1, Value);
  #else
    READ_REG((Reg + 2) >> 1, Value);
  #endif
  return Value;
}

/*********************************************************************
*
*       _SetLUTEntry_1304
*/
static void _SetLUTEntry_1304(U8 Pos, LCD_COLOR color) {
  int i;
  U16 RegL, RegH;
  U16 aColorSep[3];
  for (i = 0; i < 3; i++) {
    aColorSep[i] = color & 0xfc;
    color >>= 8;
  }
  /* Write into palette register */
  RegL = aColorSep[2] | (aColorSep[1] << 8);
  RegH = aColorSep[0] | (Pos << 8);
  LCD_ENABLE_REG_ACCESS();
  #if (LCD_CNF4 == 0)
    _WriteReg16L(0x18, RegL);
    _WriteReg16H(0x18, RegH); /* If pin CNF4 = 0 (little endian mode) the LUT is updated after writing bits 24-31 */
  #else
    _WriteReg16H(0x18, RegH);
    _WriteReg16L(0x18, RegL); /* If pin CNF4 = 1 (big endian mode) the LUT is updated after writing bits 2-7 */
  #endif
  LCD_ENABLE_MEM_ACCESS();
}
#define LCD_SET_LUT_ENTRY(Pos, Color) _SetLUTEntry_1304(Pos, Color)

#endif

#endif /* !defined(LCD_SET_LUT_ENTRY) */

/*
        *********************************************************
        *                                                       *
        *       Next pixel routines                             *
        *                                                       *
        *********************************************************
*/

#if      (LCD_OPTIMIZE)             \
      && (LCD_BUSWIDTH == 8)        \
      && (!LCD_MIRROR_X)             \
      && (!LCD_MIRROR_Y)            \
      && (LCD_SWAP_XY)              \
      && (!defined (LCD_LUT_COM))    \
      && (!defined (LCD_LUT_SEG))    \
      && (LCD_BITSPERPIXEL == 4)

static int CurPosY;    /* Physical x position !!! */
static tOff CurOff;

static void SetPosXY(int x, int y) {
  y = LCD_YSIZE-1-y;
  CurPosY = y;
  CurOff = XY2OFF(y,x);
}

static void SetNextPixel(LCD_PIXELINDEX c) {
  U8 Data;
  READ_MEM(CurOff, Data);
  if (CurPosY&1) {
    Data = (Data & ~(15<<0)) | (c<<0);
    CurOff++;
  } else {
    Data = (Data & ~(15<<4)) | (c<<4);
  }
  WRITE_MEM(CurOff, Data);
  CurPosY++;
}

#elif      (LCD_OPTIMIZE)           \
      && (LCD_BUSWIDTH == 8)        \
      && (!LCD_MIRROR_X)            \
      && (!LCD_MIRROR_Y)            \
      && (!LCD_SWAP_XY)              \
      && (!defined (LCD_LUT_COM))    \
      && (!defined (LCD_LUT_SEG))    \
      && (LCD_BITSPERPIXEL == 4)

static int CurPosX;    /* Physical x position !!! */
static tOff CurOff;
static U8  CurData;

static void SetPosXY(int x, int y) {
  CurPosX = x;
  CurOff = XY2OFF(x,y);
  CurData = LCD_READ_MEM(CurOff);
}

#define SETNEXTPIXEL(c) {                           \
  if (CurPosX&1) {																	\
    CurData = (CurData & ~(15<<0)) | (c<<0);				\
    WRITE_MEM(CurOff, CurData);			    						\
    CurOff++;																				\
    READ_MEM(CurOff, CurData);				    					\
  } else {																					\
    CurData = (CurData & ~(15<<4)) | (c<<4);				\
  }																									\
  CurPosX++;                                        \
}

void SetNextPixel(int c) {
  SETNEXTPIXEL(c);
}

#define END_SETNEXTPIXEL() if (CurPosX&1) WRITE_MEM(CurOff, CurData);

#else
  #define END_SETNEXTPIXEL()
#endif

/*********************************************************************
*
*       Static code: BitBlt access for SED1356/SED13806
*
**********************************************************************
*/

#if LCD_USE_BITBLT             \
    && (LCD_BUSWIDTH==16)      \
    && (!defined (LCD_LUT_COM))   \
    && (!defined (LCD_LUT_SEG))   \
    && ((LCD_CONTROLLER == 1356)||(LCD_CONTROLLER == 13806))

#if LCD_BITSPERPIXEL == 8
  #define BITBLT_SET_DESTINATION(x,y)     {                                                                            \
                                            U32 DestOff = (U32)y * BYTESPERLINE + x;                                   \
                                            WRITE_REG(0x108 / 2, DestOff);                                             \
                                            WRITE_REG(0x10a / 2, DestOff >> 16);                                       \
                                          }
  #define BITBLT_SET_ACTIVE()             WRITE_REG(0x100 / 2, 0x0000); WRITE_REG(0x100 / 2, 0x0080)
#elif LCD_BITSPERPIXEL == 16
  #define BITBLT_SET_DESTINATION(x,y)     {                                                                            \
                                            U32 DestOff = (U32)y * BYTESPERLINE + (x <<1);                                   \
                                            WRITE_REG(0x108 / 2, DestOff);                                             \
                                            WRITE_REG(0x10a / 2, DestOff >> 16);                                       \
                                          }
  #define BITBLT_SET_ACTIVE()             WRITE_REG(0x100 / 2, 0x0100); WRITE_REG(0x100 / 2, 0x0180)
#endif

/*********************************************************************
*
*       _WaitForBltEnd
*/
static void _WaitForBltEnd(void) {
  volatile U16 tmp;
  do {
    READ_REG(0x100 / 2, tmp);
  } while (tmp & 0x80);
  READ_REG(0x100000 / 2, tmp);                                            /* dummy read */
}

/*********************************************************************
*
*       _FillRectBB
*/
static void _FillRectBB(int x0, int y0, int x1, int y1) {
  LCD_ENABLE_REG_ACCESS(); {
    for (;x0 <= x1; x0 += 1024) {
      int _y0 = y0;
      int _x1 = x1;
      if (_x1 > (x0 + 1023)) {
        _x1 = x0 + 1023;
      }
      for (;_y0 <= y1; _y0 += 1024) {
        int _y1 = y1;
        if (_y1 > (_y0 + 1023)) {
          _y1 = _y0 + 1023;
        }
        BITBLT_SET_DESTINATION(x0, _y0);                                  /* set destination start address */
        WRITE_REG(0x110 / 2, (_x1 - x0));                                 /* set width */
        WRITE_REG(0x112 / 2, (_y1 - _y0));                                /* set height */
        WRITE_REG(0x118 / 2, (COLOR));                                    /* set foreground color  */
        if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
          WRITE_REG(0x102 / 2, 0x0605);                                   /* pattern fill, ~D   */
        } else {
          WRITE_REG(0x102 / 2, 0x0c00);                                   /* solid fill, no ROP */
        }
        BITBLT_SET_ACTIVE();                                              /* engage bitblt engine */
        _WaitForBltEnd();                                                 /* wait for pending blit to end */
      }
    }
  } LCD_ENABLE_MEM_ACCESS();
}

/*********************************************************************
*
*       _DrawBitmap1BPPBB
*/
static void _DrawBitmap1BPPBB(int x, int y, U8 const*p, int Diff, int xsize, int ysize, int BytesPerLine, const LCD_PIXELINDEX*pTrans) {
  volatile U16 tmp;
  x+= Diff;
  LCD_ENABLE_REG_ACCESS(); {
    U16 StartBit = 7 - (Diff & 7);
    U16 Data = StartBit | ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) ? 0x900 : 0x800);
    int NumWords = ((Diff & 7) + xsize + 15) >> 4;
    WRITE_REG(0x102 / 2, Data);                                           /* set start bit and operation */
    WRITE_REG(0x104 / 2, 0);                                              /* set source start address */
    BITBLT_SET_DESTINATION(x, y);                                         /* set destination start address */
    WRITE_REG(0x110 / 2, (xsize - 1));                                    /* set width */
    WRITE_REG(0x112 / 2, (ysize - 1));                                    /* set height */
    WRITE_REG(0x114 / 2, (*(pTrans + 0)));                                /* set background color */
    WRITE_REG(0x118 / 2, (*(pTrans + 1)));                                /* set foreground color  */
    BITBLT_SET_ACTIVE();                                                  /* engage bitblt engine */
    do {
      READ_REG(0x100 / 2, tmp);
    } while ((tmp & 0x80) == 0);
    for (;ysize; ysize--, p += BytesPerLine) {
      U8 const *pLine= p;
      int i;
      for (i = NumWords; i; i--, pLine += 2) {
        do {
          READ_REG(0x100 / 2, tmp);
        } while ((tmp & 0x40) == 0x40);
        WRITE_REG(0x100000 / 2, ((*pLine) | ((*(pLine + 1)) << 8)));      /* write data into FIFO */
      }
    }
    _WaitForBltEnd();                                                     /* wait for pending blit to end */
  } LCD_ENABLE_MEM_ACCESS();
}

#endif

/*********************************************************************
*
*       Static code: BitBlt access for S1D13A03 - S1D13A05
*
**********************************************************************
*/
#if LCD_USE_BITBLT              \
    && (LCD_BUSWIDTH==16)       \
    && (!defined (LCD_LUT_COM)) \
    && (!defined (LCD_LUT_SEG)) \
    && (LCD_CONTROLLER == 1304)

#if LCD_BITSPERPIXEL == 8
  #define BITBLT_SET_DESTINATION(x,y)     _WriteReg32(0x8010, (U32)y * BYTESPERLINE + x)
  #define BITBLT_SET_ACTIVE()             _WriteReg16H(0x8000, 0x0000); /* 8bpp mode */ \
                                          _WriteReg16L(0x8000, 0x0001)  /* BitBLT enable */
#elif LCD_BITSPERPIXEL == 16
  #define BITBLT_SET_DESTINATION(x,y)     _WriteReg32(0x8010, (U32)y * BYTESPERLINE + (x << 1))
  #define BITBLT_SET_ACTIVE()             _WriteReg16H(0x8000, 0x0004); /* 16bpp mode */ \
                                          _WriteReg16L(0x8000, 0x0001)  /* BitBLT enable */
#endif

/*********************************************************************
*
*       _WaitForBltEnd
*/
static void _WaitForBltEnd(void) {
  volatile U32 tmp;
  do {
    tmp = _ReadReg16L(0x8004);
  } while (tmp & 1);
}

/*********************************************************************
*
*       _FillRectBB
*/
static void _FillRectBB(int x0, int y0, int x1, int y1) {
  LCD_ENABLE_REG_ACCESS(); {
    for (;x0 <= x1; x0 += 1024) {
      int _y0 = y0;
      int _x1 = x1;
      if (_x1 > (x0 + 1023)) {
        _x1 = x0 + 1023;
      }
      for (;_y0 <= y1; _y0 += 1024) {
        int _y1 = y1;
        if (_y1 > (_y0 + 1023)) {
          _y1 = _y0 + 1023;
        }
        BITBLT_SET_DESTINATION(x0, _y0);                                  /* set destination start address */
        _WriteReg16L(0x8018, (_x1 - x0));                                 /* set width */
        _WriteReg16L(0x801C, (_y1 - _y0));                                /* set height */
        _WriteReg16L(0x8024, (COLOR));                                    /* set foreground color  */
        if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
          _WriteReg16L(0x8008, 0x0006);                                   /* pattern fill */
          _WriteReg16H(0x8008, 0x0005);                                   /* ~D */
        } else {
          _WriteReg16L(0x8008, 0x000C);                                   /* solid fill */
          _WriteReg16H(0x8008, 0x0000);                                   /* no ROP */
        }
        BITBLT_SET_ACTIVE();                                              /* engage bitblt engine */
        _WaitForBltEnd();                                                 /* wait for pending blit to end */
      }
    }
  } LCD_ENABLE_MEM_ACCESS();
}

/*********************************************************************
*
*       _DrawBitmap1BPPBB
*/
static void _DrawBitmap1BPPBB(int x, int y, U8 const*p, int Diff, int xsize, int ysize, int BytesPerLine, const LCD_PIXELINDEX*pTrans) {
  volatile U16 tmp;
  x+= Diff;
  LCD_ENABLE_REG_ACCESS(); {
    U16 StartBit  = 7 - (Diff & 7);
    U16 Operation = (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) ? 0x0009 : 0x0008;
    int NumWords = ((Diff & 7) + xsize + 15) >> 4;
    _WriteReg16L(0x8008, Operation);                                      /* set operation */
    _WriteReg16H(0x8008, StartBit);                                       /* set start bit */
    _WriteReg32 (0x800C, 0);                                              /* set source start address */
    BITBLT_SET_DESTINATION(x, y);                                         /* set destination start address */
    _WriteReg16L(0x8018, (xsize - 1));                                    /* set width */
    _WriteReg16L(0x801C, (ysize - 1));                                    /* set height */
    _WriteReg16L(0x8020, (*(pTrans + 0)));                                /* set background color */
    _WriteReg16L(0x8024, (*(pTrans + 1)));                                /* set foreground color  */
    BITBLT_SET_ACTIVE();                                                  /* engage bitblt engine */
    do {
      tmp = _ReadReg16L(0x8004);
    } while ((tmp & 1) == 0);
    for (;ysize; ysize--, p += BytesPerLine) {
      U8 const *pLine= p;
      int i;
      for (i = NumWords; i; i--, pLine += 2) {
        do {
          tmp = _ReadReg16L(0x8004);
        } while ((tmp & 0x4) == 0x4);
        _WriteReg16L(0x10000, ((*pLine) | ((*(pLine + 1)) << 8)));        /* write data into FIFO */
      }
    }
    _WaitForBltEnd();                                                     /* wait for pending blit to end */
  } LCD_ENABLE_MEM_ACCESS();
}

#endif

/*
        *********************************************************
        *                                                       *
        *       Internal set pixel routines                     *
        *                                                       *
        *********************************************************
*/

static void _SetPixel(int x, int y, LCD_PIXELINDEX c) {
  tOff Off = XY2OFF(x,y);
#if LCD_BUSWIDTH == 16
  #if LCD_BITSPERPIXEL == 1
    U8 BitNo = (~x)&15;
    U16 Data;
    READ_MEM(Off, Data);
    if (c)
      Data |= c<<BitNo;
    else
      Data &= ~(1<<BitNo);
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 2
    U16 Data;
    U8 Shift = 14 - ((x & 7) << 1);
    READ_MEM(Off, Data);
    Data = (Data & ~(3 << Shift)) | (c << Shift);
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 4
    U8 Shift = ((~x)&3)<<2;         /* 12,8,4 or 0 */
    U16 Data;
    READ_MEM(Off, Data);
    Data &= ~(15<<Shift);
		Data |= c<<Shift;
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 8
    U16 Data;
    READ_MEM(Off, Data);
    switch (x&1) {
    case 1:
      Data = (Data & ~(0xff   )) | (c   );
      break;
    case 0:
      Data = (Data & ~(0xff<<8)) | (c<<8);
      break;
    }
    WRITE_MEM(Off, Data);
  #elif (LCD_BITSPERPIXEL == 15) | (LCD_BITSPERPIXEL == 16)
    WRITE_MEM(Off, c);
  #else
    #error unsupported LCD_BITSPERPIXEL
  #endif
#elif LCD_BUSWIDTH == 8
  #if LCD_BITSPERPIXEL == 1
    U8 Data;
    U8 BitNo;
    READ_MEM(Off, Data);
    BitNo = 7-(x&7);
    if (c)
      Data |= c<<BitNo;
    else
      Data &= ~(1<<BitNo);
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 2
    U8 Data;
    READ_MEM(Off, Data);
    switch (x&3) {
    case 3:
      Data = (Data & ~(3<<0)) | (c<<0);
      break;
    case 2:
      Data = (Data & ~(3<<2)) | (c<<2);
      break;
    case 1:
      Data = (Data & ~(3<<4)) | (c<<4);
      break;
    case 0:
      Data = (Data & ~(3<<6)) | (c<<6);
      break;
    }
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 4
    U8 Data;
    READ_MEM(Off, Data);
    switch (x&1) {
    case 1:
      Data = (Data & ~(15<<0)) | (c<<0);
      break;
    case 0:
      Data = (Data & ~(15<<4)) | (c<<4);
      break;
    }
    WRITE_MEM(Off, Data);
  #elif LCD_BITSPERPIXEL == 8
    WRITE_MEM(Off, c);
  #else
    #error TBD
  #endif
#else
  #error unsupported LCD_BUSWIDTH
#endif
}

static unsigned int _GetPixelIndex(int x, int y) {
  LCD_PIXELINDEX col;
  tOff Off = XY2OFF(x,y);
#if LCD_BUSWIDTH == 16
  U16 Data;
  READ_MEM(Off,Data);
  #if LCD_BITSPERPIXEL == 1
    col = (Data >> (15-(x&15))) &1;
  #elif LCD_BITSPERPIXEL == 2
    col = (Data >> (14-((x&7)<<1))) &3;
  #elif LCD_BITSPERPIXEL == 4
    col = (Data >> (12-((x&3)<<2))) &15;
  #elif LCD_BITSPERPIXEL == 8
    col = ((x&1) ==0) ? Data>>8 : Data&255;
  #elif LCD_BITSPERPIXEL == 15
    col = Data;
  #elif LCD_BITSPERPIXEL == 16
    col = Data;
  #endif
#else
  U8 Data;
  READ_MEM(Off,Data);
  #if LCD_BITSPERPIXEL == 1
    col = (Data >> (7-(x&7))) &1;
  #elif LCD_BITSPERPIXEL == 2
    col = (Data >> (6-((x&3)<<1))) &3;
  #elif LCD_BITSPERPIXEL == 4
    col = (x&1) ? Data&15 : Data>>4;
  #elif LCD_BITSPERPIXEL == 8
    col = Data;
  #endif
#endif
  return col;
}

static void XorPixel   (int x, int y) {
  LCD_PIXELINDEX Index = _GetPixelIndex(x,y);
  _SetPixel(x,y,LCD_NUM_COLORS-1-Index);
}



/*
        *********************************************************
        *                                                       *
        *       LCD_L0_XorPixel                                 *
        *                                                       *
        *********************************************************

Purpose:  Inverts 1 pixel of the display.

*/

void LCD_L0_XorPixel(int x, int y) {
  XORPIXEL(x, y);
}

/*
        *********************************************************
        *                                                       *
        *       LCD_L0_SetPixelIndex                            *
        *                                                       *
        *********************************************************

Purpose:  Writes 1 pixel into the display.

*/
void LCD_L0_SetPixelIndex(int x, int y, int ColorIndex) {
  SETPIXEL(x, y, ColorIndex);
}


/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                      *
        *                                                       *
        *          16 bit bus, Using BITBLT                     *
        *                                                       *
        *********************************************************
*/

#if (LCD_USE_BITBLT)           \
    && (!LCD_SWAP_XY)          \
    && (LCD_BUSWIDTH==16)      \
    && (!defined (LCD_LUT_COM))   \
    && (!defined (LCD_LUT_SEG))   \
    && ((LCD_CONTROLLER == 1304) || (LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806))

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
    #define X1 (LCD_XSIZE-1-(x0))
  #else
    #define X0 x0
    #define X1 x1
  #endif
  #if LCD_MIRROR_Y
    #define Y0 (LCD_YSIZE-1-(y))
  #else
    #define Y0 y
  #endif
  if (x0>x1) return;
  _FillRectBB(X0,Y0,X1,Y0);
  #undef X0
  #undef X1
  #undef Y0
}


/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                      *
        *                                                       *
        *          16 bit bus, 16 bpp                           *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_Y)         \
      && (LCD_BUSWIDTH==16)      \
      && (!defined (LCD_LUT_COM))   \
      && (!defined (LCD_LUT_SEG))   \
      && ((LCD_BITSPERPIXEL == 16) || (LCD_BITSPERPIXEL == 15))

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
  #else
    #define X0 x0
  #endif
  if (x0>x1) return;
  {
    register tOff Off = XY2OFF(X0,y);
    register int Rem = x1-x0+1;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      for (; Rem--; Off++) {
        U16 Data;
        READ_MEM(Off, Data);
        Data = LCD_NUM_COLORS-1-Data;
        WRITE_MEM(Off,Data);
      }
    } else {
      for (; Rem >= 8; Off += 8) {
        WRITE_MEM(Off,COLOR);
        WRITE_MEM(Off+1,COLOR);
        WRITE_MEM(Off+2,COLOR);
        WRITE_MEM(Off+3,COLOR);
        WRITE_MEM(Off+4,COLOR);
        WRITE_MEM(Off+5,COLOR);
        WRITE_MEM(Off+6,COLOR);
        WRITE_MEM(Off+7,COLOR);
        Rem -=8;
      }
      for (; Rem--; Off++) {
        WRITE_MEM(Off,COLOR);
      }
    }
  }
  #undef X0
}


/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                   *
        *                                                       *
        *          16 bit bus, 8 bpp                            *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (LCD_BUSWIDTH==16)      \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 8)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
  {
    int t = (LCD_XSIZE-1-(x0));
    x0 = (LCD_XSIZE-1-(x1));
    x1 = t;
  }
  #endif
  #if LCD_MIRROR_Y
    y = (LCD_YSIZE-1-(y));
  #endif
  {
    register tOff Off = XY2OFF(x0,y);
    /* register */ U16 Data;
/* XOR mode */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      if (x0&1) {
        READ_MEM(Off, Data);
        Data ^= ((unsigned int)255);
        WRITE_MEM(Off,Data);
        x0++;
        Off++;
      }
      for (; x0<x1; x0+=2, Off++) {
        READ_MEM(Off,Data);
        Data ^= 0xffff;
        WRITE_MEM(Off,Data);
      }
      if ((x1&1)==0) {
        READ_MEM(Off,Data);
        Data ^=  (U16)255<<8;
        WRITE_MEM(Off,Data);
      }
/* WRITE mode */
    } else {
      if (x0&1) {
        READ_MEM(Off,Data);
        Data = (Data&((U16)0xff<<8)) | (COLOR);
        WRITE_MEM(Off,Data);
        x0++;
        Off++;
      }
      Data = COLOR|((U16)COLOR<<8);
      /* Optimization for longer lines ... */
      #ifdef LCD_WRITE_MEM32
      if ((x0<x1-4) && (x0&2)) { /* Write 16 bits if necessary */
        WRITE_MEM(Off,Data);
        x0+=2;
        Off++;
      }
      /* write 4 pixels at a time, loop unrolled  */
      { register U32 Data32 = Data| (Data<<16);
        for (;x0<x1-32-2; x0+=32, Off+=16) {
          LCD_WRITE_MEM32(Off+0,Data32);
          LCD_WRITE_MEM32(Off+2,Data32);
          LCD_WRITE_MEM32(Off+4,Data32);
          LCD_WRITE_MEM32(Off+6,Data32);
          LCD_WRITE_MEM32(Off+8,Data32);
          LCD_WRITE_MEM32(Off+10,Data32);
          LCD_WRITE_MEM32(Off+12,Data32);
          LCD_WRITE_MEM32(Off+14,Data32);
        }
      }
      { register U32 Data32 = Data| (Data<<16);
        for (;x0<x1-16-2; x0+=16, Off+=8) {
          LCD_WRITE_MEM32(Off+0,Data32);
          LCD_WRITE_MEM32(Off+2,Data32);
          LCD_WRITE_MEM32(Off+4,Data32);
          LCD_WRITE_MEM32(Off+6,Data32);
        }
      }
      { register U32 Data32 = Data| (Data<<16);
        for (;x0<x1-8-2; x0+=8, Off+=4) {
          LCD_WRITE_MEM32(Off+0,Data32);
          LCD_WRITE_MEM32(Off+2,Data32);
        }
      }
      #else
      for (;x0<x1-10; x0+=12, Off+=6) {
        /* swapping can be ignored, so use faster LCD_WRITE_MEM */
        LCD_WRITE_MEM(Off,Data);
        LCD_WRITE_MEM(Off+1,Data);
        LCD_WRITE_MEM(Off+2,Data);
        LCD_WRITE_MEM(Off+3,Data);
        LCD_WRITE_MEM(Off+4,Data);
        LCD_WRITE_MEM(Off+5,Data);
      }
      #endif
      /* write the last pixels 2 at a time */
      for (; x0<x1; x0+=2, Off++) {
        /* swapping can be ignored, so use faster LCD_WRITE_MEM */
        LCD_WRITE_MEM(Off,Data);
      }
      if ((x1&1)==0) {
        READ_MEM(Off, Data);
        Data = (Data&((unsigned int)0xff)) | ((U16)COLOR<<8);
        WRITE_MEM(Off,Data);
      }
    }
  }
}



/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                   *
        *                                                       *
        *          16 bit bus, 4 bpp                            *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (LCD_BUSWIDTH==16)      \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 4)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
  #else
    #define X0 x0
  #endif
  #if LCD_MIRROR_Y
    #define Y (LCD_YSIZE - 1 - y)
  #else
    #define Y y
  #endif
  if (x0>x1) return;
  {
    register tOff Off;
    register int Rem;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      for (; (x0&3    )&&(x0<=x1); x0++) XORPIXEL(x0, y);
      for (; ((x1+1)&3)&&(x0<=x1); x1--) XORPIXEL(x1, y);
      Off = XY2OFF(X0,Y);
      Rem = (x1-x0+4)>>2;
      for (; Rem--; Off++) {
        U16 c;
        READ_MEM(Off, c);
        c ^= 0xffff;
        WRITE_MEM(Off,c);
      }
    } else {
      U16 col = ((U16)(COLOR)) * 0x1111;
      for (; (x0&3    )&&(x0<=x1); x0++)
        SETPIXEL(x0, y, COLOR);
      for (; ((x1+1)&3)&&(x0<=x1); x1--)
        SETPIXEL(x1, y, COLOR);
      Off = XY2OFF(X0,Y);
      Rem = (x1-x0+4)>>2;
      for (; Rem--; Off++) {
        /* swapping can be ignored, so use faster LCD_WRITE_MEM */
        LCD_WRITE_MEM(Off,col);
      }
    }
  }
  #undef X0
}

/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                   *
        *                                                       *
        *          16 bit bus, 1 bpp                            *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (LCD_BUSWIDTH==16)      \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 1)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
  {
    int temp = x0;
    x0 = (LCD_XSIZE-1-(x1));
    x1 = (LCD_XSIZE-1-(temp));
  }
  #endif
  #if LCD_MIRROR_Y
  {
    y = (LCD_YSIZE - 1 - y);
  }
  #endif
  {
    register tOff Off = XY2OFF(x0,y);
    int iWord = x0>>4;
    int Word1 = x1>>4;
    U16 Mask   =  0xffff   >> (x0&15);
    U16 EndMask = 0xffff8000 >> (x1&15);
    U16 Data;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      for (; iWord<Word1; iWord++) {
        READ_MEM(Off,Data);
        Data ^= Mask;
        WRITE_MEM(Off++,Data);
        Mask = 0xffff;
      }
      Mask &= EndMask;
      READ_MEM(Off,Data);
      Data ^= Mask;
      WRITE_MEM(Off++,Data);
    } else {                                  /* Clear pixels in line */
      int NumWords = Word1-iWord;
      if (COLOR==0) {
        if (NumWords) {
          READ_MEM(Off,Data);
          Data &= ~Mask;
          WRITE_MEM(Off++,Data);
          NumWords--;
          /* Fill Words in 2 loops for speed reasons ... */
          for (; NumWords>=4; NumWords-=4) {
            /* swapping can be ignored, so use faster LCD_WRITE_MEM */
            LCD_WRITE_MEM(Off++,0);
            LCD_WRITE_MEM(Off++,0);
            LCD_WRITE_MEM(Off++,0);
            LCD_WRITE_MEM(Off++,0);
          }
          for (; NumWords; NumWords--) {
            /* swapping can be ignored, so use faster LCD_WRITE_MEM */
            LCD_WRITE_MEM(Off++,0);
          }
          Mask = 0xffff;
        }
        Mask &= EndMask;
        READ_MEM(Off, Data);
        Data &= ~Mask;
        WRITE_MEM(Off++,Data);
      } else {                                  /* Set pixels in line */
        if (NumWords) {
          READ_MEM(Off, Data);
          Data |= Mask;
          WRITE_MEM(Off++,Data);
          NumWords--;
        /* Fill Words in 2 loops for speed reasons ... */
          for (; NumWords>=4; NumWords-=4) {
            /* swapping can be ignored, so use faster LCD_WRITE_MEM */
            LCD_WRITE_MEM(Off++,0xffff);
            LCD_WRITE_MEM(Off++,0xffff);
            LCD_WRITE_MEM(Off++,0xffff);
            LCD_WRITE_MEM(Off++,0xffff);
          }
          for (; NumWords; NumWords--) {
            /* swapping can be ignored, so use faster LCD_WRITE_MEM */
            LCD_WRITE_MEM(Off++,0xffff);
          }
          Mask = 0xffff;
        }
        Mask &= EndMask;
        READ_MEM(Off, Data);
        Data |= Mask;
        WRITE_MEM(Off++,Data);
      }
    }
  }
}

/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                      *
        *                                                       *
        *          8 bit bus, 8 bpp                             *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (LCD_BUSWIDTH==8)      \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 8)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
  #else
    #define X0 x0
  #endif
  #if LCD_MIRROR_Y
    y = LCD_YSIZE - 1 - y;
  #endif
  if (x0>x1) return;
  {
    register tOff Off = XY2OFF(X0,y);
    register int Rem = x1-x0+1;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      for (; (--Rem)>=0; Off++) {
        U16 c;
        READ_MEM(Off,c);
        c ^= 0xff;
        WRITE_MEM(Off,c);
      }
    } else {
      for (;Rem>=4;Off+=4,Rem-=4) {
        WRITE_MEM(Off,COLOR);
        WRITE_MEM(Off+1,COLOR);
        WRITE_MEM(Off+2,COLOR);
        WRITE_MEM(Off+3,COLOR);
	  }
      for (; (--Rem)>=0; Off++) {
        WRITE_MEM(Off,COLOR);
      }
    }
  }
  #undef X0
}


/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                      *
        *                                                       *
        *          8 bit bus, 4 bpp                            *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_Y)         \
      && (LCD_BUSWIDTH==8)       \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 4)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
  #else
    #define X0 x0
  #endif
  if (x0>x1) return;
  {
    register tOff Off;
    register int Rem;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      for (;  (x0&1)      &&(x0<=x1); x0++) XORPIXEL(x0, y);
      for (; ((x1&1)!=1)  &&(x0<=x1); x1--) XORPIXEL(x1, y);
      Off = XY2OFF(X0,y);
      Rem = (x1-x0+2)>>1;
      for (; Rem--; Off++) {
        U16 c;
        READ_MEM(Off,c);
        c ^= 0xff;
        WRITE_MEM(Off,c);
      }
    } else {
      U8 col = COLOR+(COLOR<<4);
      /* Draw pixels left of fill area */
      for (; (x0&1) && (x0<=x1); x0++)   SETPIXEL(x0, y, COLOR);
      /* Draw pixels right of fill area */
      for (; ((x1+1)&1)&&(x0<=x1); x1--) SETPIXEL(x1, y, COLOR);
      Off = XY2OFF(X0,y);
      Rem = (x1-x0+2)>>1;
      for (; Rem--; Off++) {
        WRITE_MEM(Off,col);
      }
    }
  }
  #undef X0
}


/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine optimized                      *
        *                                                       *
        *          8 bit bus, 1 bpp                             *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_Y)         \
      && (!LCD_MIRROR_X)         \
      && (LCD_BUSWIDTH==8)       \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 1)

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  register tOff Off = XY2OFF(x0,y);
  int iByte = x0>>3;
  int Byte1 = x1>>3;
  U8 Mask =    0xff   >> (x0&7);
  U8 EndMask = 0xff80 >> (x1&7);
  U8 Data;
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; iByte<Byte1; iByte++) {
      READ_MEM(Off,Data);
      Data ^= Mask;
      WRITE_MEM(Off,Data);
      Mask = 0xff;
      Off++;
    }
    Mask &= EndMask;
    READ_MEM(Off,Data);
    Data ^= Mask;
    WRITE_MEM(Off,Data);
  } else {                                  /* Clear pixels in line */
    int NumBytes = Byte1-iByte;
    if (COLOR==0) {
      if (NumBytes) {
        READ_MEM(Off,Data);
        Data &= (~Mask);
        WRITE_MEM(Off,Data);
        Off++; NumBytes--;
      /* Fill bytes in 2 loops for speed reasons ... */
        for (; NumBytes>=4; NumBytes-=4, Off+=4) {
          WRITE_MEM(Off,0);
          WRITE_MEM(Off+1,0);
          WRITE_MEM(Off+2,0);
          WRITE_MEM(Off+3,0);
        }
        for (; NumBytes; NumBytes--, Off++) {
          WRITE_MEM(Off,0);
        }
        Mask = 0xff;
      }
      Mask &= EndMask;
      READ_MEM(Off,Data);
      Data &= ~Mask;
      WRITE_MEM(Off,Data);
    } else {                                  /* Set pixels in line */
      if (NumBytes) {
        READ_MEM(Off,Data);
        Data |= Mask;
        WRITE_MEM(Off,Data);
        Off++; NumBytes--;
      /* Fill bytes in 2 loops for speed reasons ... */
        for (; NumBytes>=4; NumBytes-=4, Off+=4) {
          WRITE_MEM(Off,  0xff);
          WRITE_MEM(Off+1,0xff);
          WRITE_MEM(Off+2,0xff);
          WRITE_MEM(Off+3,0xff);
        }
        for (; NumBytes; NumBytes--, Off++) {
          WRITE_MEM(Off,0xff);
        }
        Mask = 0xff;
      }
      Mask &= EndMask;
      READ_MEM(Off,Data);
      Data |= Mask;
      WRITE_MEM(Off,Data);
    }
  }
}




/*
        *********************************************************
        *                                                       *
        *          LCD_L0_DrawHLine unoptimized                    *
        *                                                       *
        *********************************************************
*/

#else  /* Unoptimized variant */

#if (LCD_ALLOW_NON_OPTIMIZED_MODE == 0) && (LCD_SWAP_XY == 0)
  #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
#endif

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (x0 <= x1) {
      XORPIXEL(x0, y);
      x0++;
    }
  } else {
    while (x0 <= x1) {
      SETPIXEL(x0, y, COLOR);
      x0++;
    }
  }
}

#endif


/********************************************************
*
*          LCD_L0_DrawVLine optimized
*
*          16 bit bus, using BITBLT
*
*********************************************************
*/

#if (LCD_USE_BITBLT)           \
    && (!LCD_SWAP_XY)          \
    && (LCD_BUSWIDTH==16)      \
    && (!defined (LCD_LUT_COM))   \
    && (!defined (LCD_LUT_SEG))   \
    && ((LCD_CONTROLLER == 1356)||(LCD_CONTROLLER == 13806))

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x))
  #else
    #define X0 x
  #endif
  #if LCD_MIRROR_Y
    #define Y0 (LCD_YSIZE-1-(y1))
    #define Y1 (LCD_YSIZE-1-(y0))
  #else
    #define Y0 y0
    #define Y1 y1
  #endif
  if (y0>y1) return;
  _FillRectBB(X0,Y0,X0,Y1);
  #undef X0
  #undef Y0
  #undef Y1
}

/********************************************************
*
*          LCD_L0_DrawVLine optimized
*
*          8 bit bus, 4 bpp, SWAP_XY, MIRROR_Y
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (LCD_SWAP_XY)          \
      && (LCD_MIRROR_Y)         \
      && (LCD_BUSWIDTH==8)       \
      && (!defined (LCD_LUT_COM))   \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 4)

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  #if !LCD_MIRROR_Y
    int y_P = x;
  #else
    int y_P = (LCD_XSIZE-1-x);
  #endif
  #if !LCD_MIRROR_X
    int x0_P = y0;
    int x1_P = y1;
  #else
    int x0_P = (LCD_YSIZE-1-(y1));
    int x1_P = (LCD_YSIZE-1-(y0));
  #endif
  register tOff Off = XY2OFF(x0_P, y_P);
  register int Rem;
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    if (x0_P & 3)
      while (x0_P <= x1_P)
        XorPixel(x0_P++, y_P);
    if ((x1_P + 1) & 3)
      while (x1_P >= x0_P)
        XorPixel(x1_P--, y_P);
    Rem = (x1_P - x0_P + 1) >> 1;
    for (; Rem-- > 0; Off++) {
      U8 Contents;
      READ_MEM(Off,Contents);
      Contents ^= 0xff;
      WRITE_MEM(Off,Contents);
    }
  } else {
    U8 col = COLOR+(COLOR<<4);
    if (x0_P & 3)
      while (x0_P <= x1_P)
        _SetPixel(x0_P++, y_P, COLOR);
    if ((x1_P + 1) & 3)
      while (x1_P >= x0_P)
        _SetPixel(x1_P--, y_P, COLOR);
    Rem = (x1_P - x0_P + 1) >> 1;
    for (; Rem-- > 0; Off++)
      WRITE_MEM(Off,col);
  }
  #if 0
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
//      for (;  y0<=y1; y0++) XORPIXEL(x0, y0);
  } else {
    U8 col = COLOR+(COLOR<<4);
    /* Draw pixels left of fill area */
    for (; (x0_P&1) && (x0_P<=x1_P); x0_P++)
      _SetPixel(x0_P, y_P, COLOR);
    /* Draw pixels right of fill area */
    for (; ((x1_P+1)&1)&&(x0_P<=x1_P); x1_P--)
      _SetPixel(x0_P, y_P, COLOR);
    Rem = (x1_P+1-x0_P)>>1;
    for (; Rem--; Off++) {
      WRITE_MEM(Off,col);
    }
  }
  #endif
}

/*
*********************************************************
*
*          LCD_L0_DrawVLine optimized
*
*          16 bit bus, 2bpp, LCD_SWAP_XY = 1, LCD_MIRROR_X = 1
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)              \
      && (LCD_SWAP_XY)            \
      && (LCD_MIRROR_X)           \
      && (!LCD_MIRROR_Y)          \
      && (!defined (LCD_LUT_COM)) \
      && (!defined (LCD_LUT_SEG)) \
      && (LCD_BITSPERPIXEL == 2)  \
      && (LCD_BUSWIDTH == 16)

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  int x0_P = (LCD_YSIZE - 1 - y1);
  int x1_P = (LCD_YSIZE - 1 - y0);
  int y_P  = x;
  tOff Off = XY2OFF(x1_P, y_P);
  U16 Data;
  U16 Color = LCD_COLORINDEX * 0x5555;
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      XORPIXEL(x, y0);
      y0++;
    }
  } else {
    int Shift, RemPixels;
    U16 OrMask, AndMask;
    /* First pixels */
    RemPixels = x1_P - x0_P + 1;
    Shift = (7 - (x1_P & 7)) * 2;
    if (Shift) {
      AndMask = ~(0xffff << Shift);
      OrMask  = Color << Shift;
      READ_MEM(Off, Data);
      if (XY2OFF(x0_P, y_P) == Off) {
        /* Only one word is affected */
        Shift = (x0_P & 7) * 2;
        OrMask &= 0xffff >> Shift;
        AndMask |= ~(0xffff >> Shift);
        Data &= AndMask;
        Data |= OrMask;
        WRITE_MEM(Off, Data);
        return;
      }
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
      RemPixels -= 8 - (Shift >> 1);
      Off--;
    }
    /* Complete words */
    while (RemPixels >= 8) {
      WRITE_MEM(Off, Color);
      RemPixels -= 8;
      Off--;
    }
    /* Last pixels */
    if (RemPixels) {
      Shift = (x0_P & 7) * 2;
      OrMask = Color >> Shift;
      AndMask = ~(0xffff >> Shift);
      READ_MEM(Off, Data);
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*
*********************************************************
*
*          LCD_L0_DrawVLine optimized
*
*          16 bit bus, 4 bpp, SWAP_XY
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)               \
      && (LCD_SWAP_XY)             \
      && (!defined (LCD_LUT_COM))  \
      && (!defined (LCD_LUT_SEG))  \
      && (LCD_BITSPERPIXEL == 4)   \
      && (LCD_BUSWIDTH==16)

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  #if !LCD_MIRROR_Y
    int y_P = x;
  #else
    int y_P = (LCD_XSIZE-1-x);
  #endif
  #if !LCD_MIRROR_X
    int x0_P = y0;
    int x1_P = y1;
  #else
    int x0_P = (LCD_YSIZE-1-(y1));
    int x1_P = (LCD_YSIZE-1-(y0));
  #endif
  register tOff Off;
  register int Rem;
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    if (x0_P & 3)
      while ((x0_P <= x1_P) && (x0_P & 3))
        XorPixel(x0_P++, y_P);
    if ((x1_P + 1) & 3)
      while ((x1_P >= x0_P) && ((x1_P + 1) & 3))
        XorPixel(x1_P--, y_P);
    Off = XY2OFF(x0_P, y_P);
    Rem = (x1_P - x0_P + 3) >> 2;
    for (; Rem-- > 0; Off++) {
      U16 Contents;
      Contents = LCD_READ_MEM(Off);
      Contents ^= 0xffff;
      LCD_WRITE_MEM(Off,Contents);
    }
  } else {
    U16 col = COLOR * 0x1111;
    if (x0_P & 3)
      while ((x0_P <= x1_P) && (x0_P & 3))
        _SetPixel(x0_P++, y_P, COLOR);
    if ((x1_P + 1) & 3)
      while ((x1_P >= x0_P) && ((x1_P + 1) & 3))
        _SetPixel(x1_P--, y_P, COLOR);
    Off = XY2OFF(x0_P, y_P);
    Rem = (x1_P - x0_P + 3) >> 2;
    for (; Rem-- > 0; Off++) {
      LCD_WRITE_MEM(Off,col);
    }
  }
}

/*
*********************************************************
*
*          LCD_L0_DrawVLine optimized
*
*          16 bit bus, 8 bpp
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)              \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_Y)          \
      && (!defined (LCD_LUT_COM))  \
      && (!defined (LCD_LUT_SEG))  \
      && (LCD_BITSPERPIXEL == 8)  \
      && (LCD_BUSWIDTH==16)

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x))
  #else
    #define X0 x
  #endif
  {
    register int shift = 0;
    register U16 AndMask, OrMask;
    register tOff Off = XY2OFF((X0),y0);
  #if LCD_MIRROR_X
    switch (x&1) {
  #else
    switch (1-(x&1)) {
  #endif
    case 0:
      shift = 0;
      break;
    case 1:
      shift = 8;
    }
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      AndMask = 0xffff;
      OrMask =  (0xff << shift);
    } else {
      AndMask = ~(0xff << shift);
      OrMask = ((U16)COLOR << shift);
    }
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
      register U16 Mask = (0xff << shift);
      for (;y0<=y1;y0++) {
        U16 Data;
        READ_MEM(Off,Data);
        Data ^= Mask;
        WRITE_MEM(Off,Data);
        Off+= WORDSPERLINE;
      }
    } else {
      for (;y0<=y1;y0++) {
        U16 Data;
        READ_MEM(Off,Data);
        Data &= AndMask;
        Data |= OrMask;
        WRITE_MEM(Off,Data);
        Off+= WORDSPERLINE;
      }
    }
  }
  #undef X0
}

/*
*********************************************************
*
*          LCD_L0_DrawVLine no optimization
*
*          8 bit bus, 1 bpp
*
*********************************************************
*/

#else  /* No optimization */

#if (LCD_ALLOW_NON_OPTIMIZED_MODE == 0) && (LCD_SWAP_XY == 1)
  #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
#endif

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      XORPIXEL(x, y0);
      y0++;
    }
  } else {
    while (y0 <= y1) {
      SETPIXEL(x, y0, COLOR);
      y0++;
    }
  }
}

#endif


/*
        *********************************************************
        *                                                       *
        *          LCD_FillRect Optimized                       *
        *                                                       *
        *          16 bit bus, Using BITBLT                     *
        *                                                       *
        *********************************************************
*/

#if (LCD_USE_BITBLT)           \
    && (!LCD_SWAP_XY)          \
    && (LCD_BUSWIDTH==16)      \
    && (!defined (LCD_LUT_COM))   \
    && (!defined (LCD_LUT_SEG))   \
    && ((LCD_CONTROLLER == 1304) || (LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806))

void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
  #if LCD_MIRROR_X
    #define X0 (LCD_XSIZE-1-(x1))
    #define X1 (LCD_XSIZE-1-(x0))
  #else
    #define X0 x0
    #define X1 x1
  #endif
  #if LCD_MIRROR_Y
    #define Y0 (LCD_YSIZE-1-(y1))
    #define Y1 (LCD_YSIZE-1-(y0))
  #else
    #define Y0 y0
    #define Y1 y1
  #endif
  if ((x0>x1) | (y0>y1)) return;
  _FillRectBB(X0,Y0,X1,Y1);
  #undef X0
  #undef X1
  #undef Y0
  #undef Y1
}

#else


/*
        *********************************************************
        *                                                       *
        *          LCD_FillRect                                 *
        *                                                       *
        *********************************************************
*/

void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
#if !LCD_SWAP_XY
  for (; y0 <= y1; y0++) {
    LCD_L0_DrawHLine(x0,y0, x1);
  }
#else
  for (; x0 <= x1; x0++) {
    LCD_L0_DrawVLine(x0,y0, y1);
  }
#endif
}

#endif


/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          8 bit bus, 1 bpp
*
*********************************************************
*/

#if (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_Y)         \
      && (!LCD_MIRROR_X)         \
      && (LCD_BUSWIDTH==8)       \
      && (!defined (LCD_LUT_COM))   \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 1)


static void  DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
/* only normal mode optimized */
  #if (GUI_USE_MEMDEV_1BPP_FOR_SCREEN == 1)
    const LCD_PIXELINDEX aTrans[2] = {0, 1};
    if (!pTrans) {
      pTrans = aTrans;
    }
  #endif
  if (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR))
    goto DrawBitLine1BPP_NoOpt;
  {
    LCD_PIXELINDEX Index0 = *(pTrans+0);
    LCD_PIXELINDEX Index1 = *(pTrans+1);
    x+=Diff;
  /* Check if filling will do ... */
    if (Index0==Index1) {
      LCD_PIXELINDEX ColorIndexOld= COLOR;  /* Save forground color */
      COLOR = Index0;              /* Set foreground color */
      LCD_L0_DrawHLine(x,y,x+xsize-1);
      COLOR = ColorIndexOld;
      return;
    }
    {
  /* O.K., we have to draw ... */
      tOff Off  = XY2OFF(x,y);
      int x1 = x+xsize-1;
      U8 Mask =    0xff   >> (x &7);
      U8 EndMask = 0xff80 >> (x1&7);
      U8 Data;
      U16 XorData = Index1 ? 0 : 0xffff;
      U16 PixelData;
      int NumBytes = (x1>>3) - (x>>3);
      if (NumBytes) {
  /* Write first byte (Needs to be masked) */
        READ_MEM(Off,Data);
        Data &= (~Mask);
        PixelData   = (*(p+1) | ((*p)<<8)) ^ XorData;
        PixelData >>= (8+(x&7)-(Diff&7));
        Data |= PixelData&Mask;
        WRITE_MEM(Off,Data);
        { int DiffOld = Diff;
          Diff+= 8-(x&7);
          if ((DiffOld&~7) != (Diff&~7))
            p++;
        }
        x=0;
        Off++; NumBytes--;
  /* Write full byte */
        for (; NumBytes; NumBytes--, Off++) {
          PixelData   = (*(p+1) | ((*p)<<8))^XorData;
          PixelData >>= (8-(Diff&7));
          p++;
          WRITE_MEM(Off, PixelData);
        }
        Mask = 0xff;
      }
      PixelData   = (*(p+1) | ((*p)<<8))^XorData;
      PixelData >>= (8+(x&7)-(Diff&7));
      Mask &= EndMask;
      READ_MEM(Off,Data);
      Data &= (~Mask);
      Data |= PixelData&Mask;
      WRITE_MEM(Off,Data);
    }
    return;
  }
/* no optimization */
DrawBitLine1BPP_NoOpt:
  {
    LCD_PIXELINDEX pixels;
    LCD_PIXELINDEX Index1 = *(pTrans+1);
  /*
  // Jump to right entry point
  */
    pixels = *p;
    switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
    case LCD_DRAWMODE_TRANS:
      switch (Diff&7) {
      case 0:
        goto WriteTBit0;
      case 1:
        goto WriteTBit1;
      case 2:
        goto WriteTBit2;
      case 3:
        goto WriteTBit3;
      case 4:
        goto WriteTBit4;
      case 5:
        goto WriteTBit5;
      case 6:
        goto WriteTBit6;
      case 7:
        goto WriteTBit7;
      }
      break;
    case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
    case LCD_DRAWMODE_XOR:
      switch (Diff&7) {
      case 0:
        goto WriteXBit0;
      case 1:
        goto WriteXBit1;
      case 2:
        goto WriteXBit2;
      case 3:
        goto WriteXBit3;
      case 4:
        goto WriteXBit4;
      case 5:
        goto WriteXBit5;
      case 6:
        goto WriteXBit6;
      case 7:
        goto WriteXBit7;
      }
    }
  /*
          Write with transparency
  */
    WriteTBit0:
      if (pixels&(1<<7)) SETPIXEL(x+0, y, Index1);
      if (!--xsize)
        return;
    WriteTBit1:
      if (pixels&(1<<6)) SETPIXEL(x+1, y, Index1);
      if (!--xsize)
        return;
    WriteTBit2:
      if (pixels&(1<<5)) SETPIXEL(x+2, y, Index1);
      if (!--xsize)
        return;
    WriteTBit3:
      if (pixels&(1<<4)) SETPIXEL(x+3, y, Index1);
      if (!--xsize)
        return;
    WriteTBit4:
      if (pixels&(1<<3)) SETPIXEL(x+4, y, Index1);
      if (!--xsize)
        return;
    WriteTBit5:
      if (pixels&(1<<2)) SETPIXEL(x+5, y, Index1);
      if (!--xsize)
        return;
    WriteTBit6:
      if (pixels&(1<<1)) SETPIXEL(x+6, y, Index1);
      if (!--xsize)
        return;
    WriteTBit7:
      if (pixels&(1<<0)) SETPIXEL(x+7, y, Index1);
      if (!--xsize)
        return;
      x+=8;
      pixels = *(++p);
      goto WriteTBit0;
  /*
          Write XOR mode
  */
    WriteXBit0:
      if (pixels&(1<<7))
        XORPIXEL(x+0, y);
      if (!--xsize)
        return;
    WriteXBit1:
      if (pixels&(1<<6))
        XORPIXEL(x+1, y);
      if (!--xsize)
        return;
    WriteXBit2:
      if (pixels&(1<<5))
        XORPIXEL(x+2, y);
      if (!--xsize)
        return;
    WriteXBit3:
      if (pixels&(1<<4))
        XORPIXEL(x+3, y);
      if (!--xsize)
        return;
    WriteXBit4:
      if (pixels&(1<<3))
        XORPIXEL(x+4, y);
      if (!--xsize)
        return;
    WriteXBit5:
      if (pixels&(1<<2))
        XORPIXEL(x+5, y);
      if (!--xsize)
        return;
    WriteXBit6:
      if (pixels&(1<<1))
        XORPIXEL(x+6, y);
      if (!--xsize)
        return;
    WriteXBit7:
      if (pixels&(1<<0))
        XORPIXEL(x+7, y);
      if (!--xsize)
        return;
      x+=8;
      pixels = *(++p);
      goto WriteXBit0;
  }
}

/*********************************************************************
*
*       Draw Bitmap 1 BPP, 16 bit bus, 1 bpp mode, optimzed
*/
#elif (LCD_OPTIMIZE)                \
      && (!LCD_SWAP_XY)           \
      && (!LCD_MIRROR_Y)          \
      && (!LCD_MIRROR_X)          \
      && (LCD_BUSWIDTH == 16)     \
      && (!defined (LCD_LUT_COM)) \
      && (!defined (LCD_LUT_SEG)) \
      && (LCD_BITSPERPIXEL == 1)

static void DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  U8 Mode;
  LCD_PIXELINDEX Index0, Index1;
  #if (GUI_USE_MEMDEV_1BPP_FOR_SCREEN == 1)
    const LCD_PIXELINDEX aTrans[2] = {0, 1};
    if (!pTrans) {
      pTrans = aTrans;
    }
  #endif
  Index0 = *(pTrans + 0);
  Index1 = *(pTrans + 1);
  x += Diff;
  Mode = GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR);
  if (Mode == 0) {
    /* Check if filling will do ... */
    if (Index0 == Index1) {
      LCD_PIXELINDEX ColorIndexOld = COLOR;  /* Save forground color */
      COLOR = Index0;              /* Set foreground color */
      LCD_L0_DrawHLine(x, y, x + xsize - 1);
      COLOR = ColorIndexOld;
    } else {
      /* O.K., we have to draw ... */
      tOff Off;
      int x1, NumWords;
      U16 Mask, EndMask, Data;
      U32 XorData, PixelData;
      Off      = XY2OFF(x, y);
      x1       = x + xsize - 1;
      Mask     = 0xffff     >> (x  & 15);
      EndMask  = 0xffff8000 >> (x1 & 15);
      XorData  = Index1 ? 0 : 0xffffffff;
      NumWords = (x1 >> 4) - (x >> 4);
      if (NumWords) {
        /* Write first byte (Needs to be masked) */
        READ_MEM(Off, Data);
        Data       &= (~Mask);
        PixelData   = ((((U32)(*p)) << 24) | (((U32)*(p + 1)) << 16) | (((U16)*(p + 2)) << 8) | *(p + 3)) ^ XorData;
        PixelData >>= (16 + (x & 15) - (Diff & 15));
        Data       |= PixelData&Mask;
        WRITE_MEM(Off,Data);
        {
          int DiffOld = Diff;
          Diff += 16 - (x & 15);
          if ((DiffOld & ~15) != (Diff & ~15)) {
            p += 2;
          }
        }
        x = 0;
        Off++;
        NumWords--;
        /* Write full byte */
        for (; NumWords; NumWords--, Off++) {
          PixelData   = ((((U32)(*p)) << 24) | (((U32)*(p + 1)) << 16) | (((U16)*(p + 2)) << 8) | *(p + 3)) ^ XorData;
          PixelData >>= (16 - (Diff & 15));
          p += 2;
          WRITE_MEM(Off, PixelData);
        }
        Mask = 0xffff;
      }
      PixelData   = ((((U32)(*p)) << 24) | (((U32)*(p + 1)) << 16) | (((U16)*(p + 2)) << 8) | *(p + 3)) ^ XorData;
      PixelData >>= (16 + (x & 15) - (Diff & 15));
      Mask       &= EndMask;
      READ_MEM(Off, Data);
      Data &= (~Mask);
      Data |= PixelData&Mask;
      WRITE_MEM(Off,Data);
    }
  } else {
    switch (Mode) {
      case LCD_DRAWMODE_TRANS:
        do {
  		    if (*p & (0x80 >> Diff))
            SETPIXEL(x, y, Index1);
          x++;
			    if (++Diff == 8) {
            Diff = 0;
				    p++;
			    }
		    } while (--xsize);
        break;
      case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
      case LCD_DRAWMODE_XOR:
        do {
  		    if (*p & (0x80 >> Diff)) {
            int Pixel = LCD_L0_GetPixelIndex(x, y);
            SETPIXEL(x, y, LCD_NUM_COLORS - 1 - Pixel);
          }
          x++;
			    if (++Diff == 8) {
            Diff = 0;
				    p++;
			    }
		    } while (--xsize);
        break;
	  }
  }
}

/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          16 bit bus, 2 bpp mode, LCD_SWAP_XY = 1, LCD_MIRROR_X = 1
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)            \
    && (LCD_SWAP_XY)            \
    && (LCD_MIRROR_X)           \
    && (!LCD_MIRROR_Y)          \
    && (!defined (LCD_LUT_COM)) \
    && (!defined (LCD_LUT_SEG)) \
    && (LCD_BITSPERPIXEL == 2)  \
    && (LCD_BUSWIDTH == 16)

static const U8 _aShiftNibble[] = {
  14, 12, 10, 8, 6, 4, 2, 0
};

static void DrawBitLine1BPP_Swap(int x, int y, U8 const GUI_UNI_PTR * pData, int ysize, const U8 * pTrans, int BytesPerLine, U8 Pos) {
  int x0 = (LCD_YSIZE - 1 - y);
  int y0 = x;
  tOff Off = XY2OFF(x0, y0);
  U16 Buffer, Data;
  U8 Index, Pixel;
  U8 BufferValid = 0;
  U8 ShiftPos = 7 - (Pos & 7);
  U16 DataMask = 0x80 >> (7 - ShiftPos);
  for (; ysize; ysize--, pData += BytesPerLine, x0--) {
    U8 Shift = _aShiftNibble[x0 & 7];
    Pixel = (*pData & DataMask) >> ShiftPos;
    if (!BufferValid) {
      READ_MEM(Off, Buffer);
      BufferValid = 1;
    }
    switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      Buffer &= ~(0x3 << Shift);
      Index = *(pTrans + Pixel);
      Data = Index << Shift;
      Buffer |= Data;
      break;
    case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
    case LCD_DRAWMODE_XOR:
      if (Pixel)
        Buffer ^= (0x3 << Shift);
      break;
    case LCD_DRAWMODE_TRANS:
      if (Pixel) {
        Buffer &= ~(0x3 << Shift);
        Index = *(pTrans + Pixel);
        Data = Index << Shift;
        Buffer |= Data;
      }
      break;
    }
    if (!(x0 & 7)) {
      BufferValid = 0;
      WRITE_MEM(Off--, Buffer);
    }
  }
  if (BufferValid) {
    WRITE_MEM(Off, Buffer);
  }
}

/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          16 bit bus, 4 bpp mode, SWAP_XY
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)               \
    && (LCD_SWAP_XY)             \
    && (!defined (LCD_LUT_COM))  \
    && (!defined (LCD_LUT_SEG))  \
    && (LCD_BITSPERPIXEL == 4)   \
    && (LCD_BUSWIDTH==16)

static const U8 ShiftNibble[] = {
  12, 8, 4, 0
};

static void DrawBitLine1BPP_Swap(
  int x,
  int y,
  U8 const GUI_UNI_PTR * pData,
  int ysize,
  const U8 * pTrans,
  int BytesPerLine,
  U8 Pos)
{
  #if LCD_MIRROR_Y
    int y0 = LCD_XSIZE - x - 1;
  #else
    int y0 = x;
  #endif
  #if LCD_MIRROR_X
    int x0 = LCD_YSIZE - y - 1;
  #else
    int x0 = y;
  #endif
  tOff Off = XY2OFF(x0,y0);
  U16 Buffer, Data;
  U8 Index, Pixel;
  U8 BufferValid = 0;
  U8 ShiftPos = 7 - (Pos & 7);
  U16 DataMask = 0x80 >> (7 -ShiftPos);
  #if /*LCD_MIRROR_Y || */LCD_MIRROR_X
    for (; ysize; ysize--, pData += BytesPerLine, x0--) {
  #else
    for (; ysize; ysize--, pData += BytesPerLine, x0++) {
  #endif
    U8 Shift = ShiftNibble[x0 & 3];
    Pixel = (*pData & DataMask) >> ShiftPos;
    if (!BufferValid) {
      READ_MEM(Off,Buffer);
      BufferValid = 1;
    }
    switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      Buffer &= ~(0xF << Shift);
      Index = *(pTrans + Pixel);
      Data = Index << Shift;
      Buffer |= Data;
      break;
    case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
    case LCD_DRAWMODE_XOR:
      if (Pixel)
        Buffer ^= (0xF << Shift);
      break;
    case LCD_DRAWMODE_TRANS:
      if (Pixel) {
        Buffer &= ~(0xF << Shift);
        Index = *(pTrans + Pixel);
        Data = Index << Shift;
        Buffer |= Data;
      }
      break;
    }
    #if /*LCD_MIRROR_Y || */LCD_MIRROR_X
      if (!(x0 & 3)) {
        BufferValid = 0;
        WRITE_MEM(Off--, Buffer);
      }
    #else
      if ((x0 & 3) == 3) {
        BufferValid = 0;
        WRITE_MEM(Off++, Buffer);
      }
    #endif
  }
  if (BufferValid)
    WRITE_MEM(Off, Buffer);
}

/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          16 bit bus, 8 bpp mode
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_X)         \
      && (LCD_BUSWIDTH==16)       \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 8)

static void  DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  U8 Pixels = *p;
  U8 RemPixels;
  x+= Diff;
  RemPixels = 8-Diff;
  Pixels <<= Diff;
  /* Handle transparent bitmaps */
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    do {
      if (RemPixels==0) {
        Pixels = *(++p);
        RemPixels =8;
      }
      if (Pixels & 0x80) {
        XORPIXEL(x, y);
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
    do {
      if (RemPixels==0) {
        Pixels = *(++p);
        RemPixels =8;
      }
      if (Pixels & 0x80) {
        SETPIXEL(x, y, *(pTrans+1));
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else {
    /* Handle solid bitmaps */
    tOff Off;
    #if LCD_MIRROR_Y
      y = (LCD_YSIZE-1-(y));
    #endif
    Off = XY2OFF(x,y);
    /* Handle left border (incomplete words) */
    if (x&1) {
      U16 Data, Pixel;
      READ_MEM(Off,Data);
      Pixel = *(pTrans+(Pixels>>7));
      Data = (Data &  ((unsigned)0xff<<8)) | (Pixel);
      WRITE_MEM(Off, Data);
      xsize--;
      RemPixels--;
      Pixels <<=1;
      Off++;
    }
    /* Handle solid middle area */
    while (xsize >=2) {
      U16 Data;
      U8 Data0,Data1;
      /* Load first pixel */
      if (RemPixels ==0) {
        Pixels = *++p;
        RemPixels =8;
      }
      Data0 = Pixels>>7;
      RemPixels--;
      Pixels <<= 1;
      /* Load second pixel */
      if (RemPixels ==0) {
        Pixels = *++p;
        RemPixels =8;
      }
      Data1 = Pixels>>7;
      RemPixels--;
      Pixels <<= 1;
      Data = ((*(pTrans+Data0)) <<8) | ((*(pTrans+Data1)));
      WRITE_MEM(Off, Data);
      xsize-=2;
      Off++;
    }
    /* Handle right border (single pixel) if necessary */
    if (xsize >0) {
      U16 Data;
      U16 Pixel;
      READ_MEM(Off,Data);
      if (RemPixels==0) {
        Pixels =*(p+1);
      }
      Pixel = *(pTrans+(Pixels>>7));
      Data = (Data & (U16)(0xff)) | (Pixel<<8);
      WRITE_MEM(Off, Data);
    }
  }
}

/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          16 bit bus, 8 bpp mode, LCD_MIRROR_X
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (LCD_MIRROR_X)         \
      && (LCD_BUSWIDTH==16)       \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 8)

static void  DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  U8 Pixels = *p;
  U8 RemPixels;
  x+= Diff;
  RemPixels = 8-Diff;
  Pixels <<= Diff;
  /* Handle transparent bitmaps */
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    do {
      if (RemPixels==0) {
        Pixels = *(++p);
        RemPixels =8;
      }
      if (Pixels & 0x80) {
        XORPIXEL(x, y);
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
    do {
      if (RemPixels==0) {
        Pixels = *(++p);
        RemPixels =8;
      }
      if (Pixels & 0x80) {
        SETPIXEL(x, y, *(pTrans+1));
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else {
    /* Handle solid bitmaps */
    tOff Off;
    x = LCD_XSIZE - x - 1;
    #if LCD_MIRROR_Y
      y = (LCD_YSIZE-1-(y));
    #endif
    Off = XY2OFF(x,y);
    /* Handle left border (incomplete words) */
    if (!(x&1)) {
      U16 Data, Pixel;
      READ_MEM(Off,Data);
      Pixel = *(pTrans+(Pixels>>7));
      Data = (Data &  ((unsigned)0xff)) | (Pixel<<8);
      WRITE_MEM(Off, Data);
      xsize--;
      RemPixels--;
      Pixels <<=1;
      Off--;
    }
    /* Handle solid middle area */
    while (xsize >=2) {
      U16 Data;
      U8 Data0,Data1;
      /* Load first pixel */
      if (RemPixels ==0) {
        Pixels = *++p;
        RemPixels =8;
      }
      Data0 = Pixels>>7;
      RemPixels--;
      Pixels <<= 1;
      /* Load second pixel */
      if (RemPixels ==0) {
        Pixels = *++p;
        RemPixels =8;
      }
      Data1 = Pixels>>7;
      RemPixels--;
      Pixels <<= 1;
      Data = (*(pTrans+Data0)) | ((*(pTrans+Data1))<<8);
      WRITE_MEM(Off, Data);
      xsize-=2;
      Off--;
    }
    /* Handle right border (single pixel) if necessary */
    if (xsize >0) {
      U16 Data;
      U16 Pixel;
      READ_MEM(Off,Data);
      if (RemPixels==0) {
        Pixels =*(p+1);
      }
      Pixel = *(pTrans+(Pixels>>7));
      Data = (Data & (U16)(0xff00)) | (Pixel);
      WRITE_MEM(Off, Data);
    }
  }
}

/********************************************************
*
*          Draw Bitmap 1 BPP Optimzed
*
*          16 bit bus, 4 bpp mode
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)             \
      && (!LCD_SWAP_XY)          \
      && (!LCD_MIRROR_X)         \
      && (LCD_BUSWIDTH==16)       \
      && (!defined (LCD_LUT_SEG))   \
      && (LCD_BITSPERPIXEL == 4)

static void  DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX aColor[2];
  U16 Pixels = ((*p) << 8) | (*(p + 1));
  U8 RemPixels;
  p++;
  aColor[0] = *pTrans;
  aColor[1] = *(pTrans + 1);
  x += Diff;
  RemPixels = 16 - Diff;
  Pixels <<= Diff;
  /* Handle transparent bitmaps */
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    do {
      if (RemPixels==0) {
        Pixels = ((*(p + 1)) << 8) | (*(p + 2));
        p += 2;
        RemPixels = 16;
      }
      if (Pixels & 0x8000) {
        XORPIXEL(x, y);
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
    do {
      if (RemPixels==0) {
        Pixels = ((*(p + 1)) << 8) | (*(p + 2));
        p += 2;
        RemPixels = 16;
      }
      if (Pixels & 0x8000) {
        SETPIXEL(x, y, *(pTrans+1));
      }
      RemPixels--;
      Pixels <<=1;
      x++;
    } while (--xsize);
  } else {
    /* Handle solid bitmaps */
    tOff Off;
    #if LCD_MIRROR_Y
      y = (LCD_YSIZE-1-(y));
    #endif
    Off = XY2OFF(x,y);
    /* Handle left border (incomplete words) */
    if (x & 3) {
      U16 Data;
      U8 LeftRem = 4 - (x & 3);
      READ_MEM(Off, Data);
      if (LeftRem == 1) {
        Data &= 0xfff0;
        Data |= (Pixels & (U16)(1<<15)) ? aColor[1] : aColor[0];
      } else if (LeftRem == 2) {
        Data &= 0xff0f;
        Data |= ((Pixels & (U16)(1<<15)) ? aColor[1] : aColor[0]) << 4;
        if (xsize > 1) {
          Data &= 0xfff0;
          Data |= (Pixels & (1<<14)) ? aColor[1] : aColor[0];
        }
      } else if (LeftRem == 3) {
        Data &= 0xf0ff;
        Data |= ((Pixels & (U16)(1<<15)) ? aColor[1] : aColor[0]) << 8;
        if (xsize > 1) {
          Data &= 0xff0f;
          Data |= ((Pixels & (1<<14)) ? aColor[1] : aColor[0]) << 4;
          if (xsize > 2) {
            Data &= 0xfff0;
            Data |= (Pixels & (1<<13)) ? aColor[1] : aColor[0];
          }
        }
      }
      WRITE_MEM(Off, Data);
      xsize -= LeftRem;
      Off++;
      RemPixels -= LeftRem;
      Pixels <<= LeftRem;
    }
    /* Handle solid middle area */
    while (xsize >= 4) {
      U16 Data;
      Data  = ((Pixels & (U16)(1<<15)) ? aColor[1] : aColor[0]) << 12;
      Data |= ((Pixels & (1<<14)) ? aColor[1] : aColor[0]) << 8;
      Data |= ((Pixels & (1<<13)) ? aColor[1] : aColor[0]) << 4;
      Data |= ((Pixels & (1<<12)) ? aColor[1] : aColor[0]);
      RemPixels -= 4;
      Pixels <<= 4;
      WRITE_MEM(Off, Data);
      xsize -= 4;
      Off++;
      if (RemPixels <= 3) {
        U16 NewPixels;
        p++;
        NewPixels = ((*p) << 8) | (*(p + 1));
        NewPixels >>= RemPixels ;
        Pixels |= NewPixels;
        RemPixels += 8;
      }
    }
    /* Handle right border (single pixel) if necessary */
    if (xsize > 0) {
      U16 Data;
      READ_MEM(Off, Data);
      Data &= 0x0fff;
      Data |= ((Pixels & (U16)(1<<15)) ? aColor[1] : aColor[0]) << 12;
      if (xsize > 1) {
        Data &= 0xf0ff;
        Data |= ((Pixels & (1<<14)) ? aColor[1] : aColor[0]) << 8;
        if (xsize > 2) {
          Data &= 0xff0f;
          Data |= ((Pixels & (1<<13)) ? aColor[1] : aColor[0]) << 4;
        }
      }
      WRITE_MEM(Off, Data);
    }
  }
}

/*
*********************************************************
*
*          Draw Bitmap 1 BPP unoptimzed
*
*********************************************************
*/

#else

static void  DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixels;
  LCD_PIXELINDEX Index0, Index1;
  #if (GUI_USE_MEMDEV_1BPP_FOR_SCREEN == 1)
    const LCD_PIXELINDEX aTrans[2] = {0, 1};
    if (!pTrans) {
      pTrans = aTrans;
    }
  #endif
  Index0 = *(pTrans + 0);
  Index1 = *(pTrans + 1);
/*
// Jump to right entry point
*/
  pixels = *p;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:
    #if defined (SETNEXTPIXEL)   /* Optimization ! */
      x+=Diff;
      SetPosXY(x,y);
    #endif
    switch (Diff&7) {
    case 0:
      goto WriteBit0;
    case 1:
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    case 3:
      goto WriteBit3;
    case 4:
      goto WriteBit4;
    case 5:
      goto WriteBit5;
    case 6:
      goto WriteBit6;
    case 7:
      goto WriteBit7;
    }
    break;
  case LCD_DRAWMODE_TRANS:
    switch (Diff&7) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    case 3:
      goto WriteTBit3;
    case 4:
      goto WriteTBit4;
    case 5:
      goto WriteTBit5;
    case 6:
      goto WriteTBit6;
    case 7:
      goto WriteTBit7;
    }
    break;
  case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
  case LCD_DRAWMODE_XOR:
    switch (Diff&7) {
    case 0:
      goto WriteXBit0;
    case 1:
      goto WriteXBit1;
    case 2:
      goto WriteXBit2;
    case 3:
      goto WriteXBit3;
    case 4:
      goto WriteXBit4;
    case 5:
      goto WriteXBit5;
    case 6:
      goto WriteXBit6;
    case 7:
      goto WriteXBit7;
    }
  }
/*
        Write with transparency
*/
  WriteTBit0:
    if (pixels&(1<<7)) SETPIXEL(x+0, y, Index1);
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(1<<6)) SETPIXEL(x+1, y, Index1);
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(1<<5)) SETPIXEL(x+2, y, Index1);
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(1<<4)) SETPIXEL(x+3, y, Index1);
    if (!--xsize)
      return;
  WriteTBit4:
    if (pixels&(1<<3)) SETPIXEL(x+4, y, Index1);
    if (!--xsize)
      return;
  WriteTBit5:
    if (pixels&(1<<2)) SETPIXEL(x+5, y, Index1);
    if (!--xsize)
      return;
  WriteTBit6:
    if (pixels&(1<<1)) SETPIXEL(x+6, y, Index1);
    if (!--xsize)
      return;
  WriteTBit7:
    if (pixels&(1<<0)) SETPIXEL(x+7, y, Index1);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteTBit0;

/*
        Write without transparency
*/
#if defined (SETNEXTPIXEL)   /* Optimization ! */
  WriteBit0:
    SetNextPixel((pixels&(1<<7)) ? Index1 : Index0);
    if (!--xsize) {
End_WriteBit:
      END_SETNEXTPIXEL();
      return;
    }
  WriteBit1:
    SetNextPixel((pixels&(1<<6)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit2:
    SetNextPixel((pixels&(1<<5)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit3:
    SetNextPixel((pixels&(1<<4)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit4:
    SetNextPixel((pixels&(1<<3)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit5:
    SetNextPixel((pixels&(1<<2)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit6:
    SetNextPixel((pixels&(1<<1)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
  WriteBit7:
    SetNextPixel((pixels&(1<<0)) ? Index1 : Index0);
    if (!--xsize)
      goto End_WriteBit;
    x+=8;
    pixels = *(++p);
    goto WriteBit0;
#else
  WriteBit0:
    SETPIXEL(x+0, y, (pixels&(1<<7)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit1:
    SETPIXEL(x+1, y, (pixels&(1<<6)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit2:
    SETPIXEL(x+2, y, (pixels&(1<<5)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit3:
    SETPIXEL(x+3, y, (pixels&(1<<4)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit4:
    SETPIXEL(x+4, y, (pixels&(1<<3)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit5:
    SETPIXEL(x+5, y, (pixels&(1<<2)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit6:
    SETPIXEL(x+6, y, (pixels&(1<<1)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit7:
    SETPIXEL(x+7, y, (pixels&(1<<0)) ? Index1 : Index0);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteBit0;
#endif
/*
        Write XOR mode
*/
  WriteXBit0:
    if (pixels&(1<<7))
      XORPIXEL(x+0, y);
    if (!--xsize)
      return;
  WriteXBit1:
    if (pixels&(1<<6))
      XORPIXEL(x+1, y);
    if (!--xsize)
      return;
  WriteXBit2:
    if (pixels&(1<<5))
      XORPIXEL(x+2, y);
    if (!--xsize)
      return;
  WriteXBit3:
    if (pixels&(1<<4))
      XORPIXEL(x+3, y);
    if (!--xsize)
      return;
  WriteXBit4:
    if (pixels&(1<<3))
      XORPIXEL(x+4, y);
    if (!--xsize)
      return;
  WriteXBit5:
    if (pixels&(1<<2))
      XORPIXEL(x+5, y);
    if (!--xsize)
      return;
  WriteXBit6:
    if (pixels&(1<<1))
      XORPIXEL(x+6, y);
    if (!--xsize)
      return;
  WriteXBit7:
    if (pixels&(1<<0))
      XORPIXEL(x+7, y);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteXBit0;
}
#endif

/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 2 BPP                                    *
        *                                                       *
        *********************************************************
*/

#if (LCD_MAX_LOG_COLORS > 2)

#if (LCD_BITSPERPIXEL == 2)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  DrawBitLine2BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixels;
/*
// Jump to right entry point
*/
  pixels = *p;
  if (pTrans) {
    /*
      with palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    default:
      goto WriteTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteBit0;
    case 1:
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    default:
      goto WriteBit3;
    }
  /*
          Write without transparency
  */
  WriteBit0:
    SETPIXEL(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteBit1:
    SETPIXEL(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteBit2:
    SETPIXEL(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteBit3:
    SETPIXEL(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels&(3<<6))
      SETPIXEL(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(3<<4))
      SETPIXEL(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(3<<2))
      SETPIXEL(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(3<<0))
      SETPIXEL(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteTBit0;
  } else {
    /*
      without palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteDDPTBit0;
    case 1:
      goto WriteDDPTBit1;
    case 2:
      goto WriteDDPTBit2;
    default:
      goto WriteDDPTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteDDPBit0;
    case 1:
      goto WriteDDPBit1;
    case 2:
      goto WriteDDPBit2;
    default:
      goto WriteDDPBit3;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    SETPIXEL(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPBit1:
    SETPIXEL(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPBit2:
    SETPIXEL(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPBit3:
    SETPIXEL(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels&(3<<6))
      SETPIXEL(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&(3<<4))
      SETPIXEL(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPTBit2:
    if (pixels&(3<<2))
      SETPIXEL(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPTBit3:
    if (pixels&(3<<0))
      SETPIXEL(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPTBit0;
  }
}

#endif



#if (LCD_MAX_LOG_COLORS > 4)

/**********************************************************
*
*          Draw Bitmap 4 BPP, optimized
*
*          16 bit bus, 4 bpp mode
*
*********************************************************
*/
#if (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 4)  \
    && (LCD_BITSPERPIXEL == 4)   \
    && (LCD_MIRROR_X==0)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_SEG))


static void  DrawBitLine4BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  U16 Data;
  U16 DataOut;
  U16 Mask;
  U8  Phase = x&1;
  x += Diff;   /* Set x back to the orignal position before clipping */
  if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
    U8 Pixels;
    U8 RemPixels =0;
    if (Diff) {
      RemPixels = Diff;
      Pixels = (*p++) << (RemPixels<<2);
    }
    do {
      /* Load data if necessary */
      if (RemPixels == 0) {
        Pixels = *p++;
        RemPixels = 2;
      }
      /* Set pixel if index != 0 */
      if (Pixels & 0xf0) {
        if (pTrans) {
          SETPIXEL(x, y, *(pTrans + (Pixels >> 4)));
        } else {
          SETPIXEL(x, y, (Pixels >> 4));
        }
      }
      /* inc counters / ptrs etc. */
      RemPixels--;
      Pixels <<= 4;
      x++;
    } while (--xsize);
  } else { /* Handle non-transparent bitmaps */
    tOff Off;
    #if LCD_MIRROR_Y
      y = LCD_YSIZE - 1 - y;
    #endif
    Off = XY2OFF(x, y);
    /* Handle left border */
    if (x & 3) {
      U8 LeftRem = 4- (x & 3);
      /* Load data */
      if (Phase == 0) {
        Data = *p++ <<8;
        if (LeftRem >2) {
          Data |= *(p++);
        } else {
          Data >>= 8;
        }
      } else {
        Data = *p <<4;
        if (LeftRem >1) {
          Data |= (*++p >>4);
        } else {
          Data >>= 8;
        }
      }
      /* Convert logical indices to physical indices */
      if (pTrans) {
        DataOut  = *(pTrans + (Data & 0xf));
        if (LeftRem >1) {
          DataOut |= *(pTrans + ((Data>>4) & 0xf)) <<4;
          if (LeftRem >2) {
            DataOut |= *(pTrans + ((Data>>8) & 0xf)) <<8;
          }
        }
      } else {
        DataOut  = (Data & 0xf);
        if (LeftRem >1) {
          DataOut |= ((Data>>4) & 0xf) <<4;
          if (LeftRem >2) {
            DataOut |= ((Data>>8) & 0xf) <<8;
          }
        }
      }
      /* Mask output data */
      Mask =  0xffff >> (16-(LeftRem << 2));     /* Mask out pixels to the left */
      if (xsize < LeftRem) {
        Mask &= 0xffff << ((LeftRem-xsize) << 2);      /* Mask out pixels to the right */
      }
      DataOut &= Mask;
      /* Or unchanged pixels */
      READ_MEM(Off, Data);
      DataOut |= Data &~Mask;
      /* output data */
      WRITE_MEM(Off, DataOut);
      xsize -= LeftRem;
      Off++;
    }
    /* Handle solid middle area (4 pixels at a time)*/
    while (xsize >= 4) {
      /* Load data */
      if (Phase == 0) {
        Data = (*p <<8) | *(p+1) ;
      } else {
        Data = (*p <<12) | (*(p+1) <<4) | (*(p+2) >>4);
      }
      /* Convert logical indices to physical indices */
      if (pTrans) {
        DataOut  = *(pTrans + (Data & 0xf));
        DataOut |= *(pTrans + ((Data>>4) & 0xf)) <<4;
        DataOut |= *(pTrans + ((Data>>8) & 0xf)) <<8;
        DataOut |= *(pTrans +  (Data>>12))       <<12;
      } else {
        DataOut  = (Data & 0xf);
        DataOut |= ((Data>>4) & 0xf) <<4;
        DataOut |= ((Data>>8) & 0xf) <<8;
        DataOut |= (Data>>12)       <<12;
      }
      /* output data */
      WRITE_MEM(Off, DataOut);
      /* inc counters / ptrs etc. */
      p+=2;
      xsize -= 4;
      Off++;
    }
    /* Handle right border */
    if (xsize >0) {    /* Anything to do at all ? */
      /* Load data */
      if (Phase == 0) {
        Data = (*p <<8);
        if (xsize > 2) {
          Data |= (*(p+1));
        }
      } else {
        Data = (*p <<12);
        if (xsize > 1) {
          Data |= (*(p+1) <<4);
        }
      }
      /* Convert logical indices to physical indices */
      if (pTrans) {
        DataOut  = *(pTrans + (Data>>12)) <<12;
        if (xsize > 1) {
          DataOut |= *(pTrans + ((Data>>8) & 0xf)) <<8;
          if (xsize > 2) {
            DataOut |= *(pTrans + ((Data>>4) & 0xf)) <<4;
          }
        }
      } else {
        DataOut  = (Data>>12) <<12;
        if (xsize > 1) {
          DataOut |= ((Data>>8) & 0xf) <<8;
          if (xsize > 2) {
            DataOut |= ((Data>>4) & 0xf) <<4;
          }
        }
      }
      /* Mask output data */
      Mask = 0xffff << ((4-xsize)<<2);
      DataOut &= Mask;
      /* Or unchanged pixels */
      READ_MEM(Off, Data);
      DataOut |= Data &~Mask;
      /* output data */
      WRITE_MEM(Off, DataOut);
    }
  }
}

/********************************************************
*
*          Draw Bitmap 4 BPP Optimzed
*
*          16 bit bus, 4 bpp mode, SWAP_XY
*
*********************************************************
*/

#elif (LCD_OPTIMIZE)               \
    && (LCD_SWAP_XY)             \
    && (!defined (LCD_LUT_COM))  \
    && (!defined (LCD_LUT_SEG))  \
    && (LCD_BITSPERPIXEL == 4)   \
    && (LCD_BUSWIDTH==16)

static void DrawBitLine4BPP_Swap(
  int x,
  int y,
  U8 const GUI_UNI_PTR * pData,
  int ysize,
  const U8 * pTrans,
  int BytesPerLine,
  U8 Pos)
{
  #if LCD_MIRROR_Y
    int y0 = LCD_XSIZE - x - 1;
  #else
    int y0 = x;
  #endif
  #if LCD_MIRROR_X
    int x0 = LCD_YSIZE - y - 1;
  #else
    int x0 = y;
  #endif
  tOff Off = XY2OFF(x0,y0);
  U16 Buffer, Data;
  U8 Index, Pixel;
  U8 BufferValid = 0;
  U8 ShiftPos = (1 - (Pos & 1)) << 2;
  U16 DataMask = 0xf0 >> (4 - ShiftPos);
  #if /*LCD_MIRROR_Y || */LCD_MIRROR_X
    for (; ysize; ysize--, pData += BytesPerLine, x0--) {
  #else
    for (; ysize; ysize--, pData += BytesPerLine, x0++) {
  #endif
    U8 Shift = ShiftNibble[x0 & 3];
    Pixel = (*pData & DataMask) >> ShiftPos;
    if (!BufferValid) {
      READ_MEM(Off,Buffer);
      BufferValid = 1;
    }
    switch (GUI_Context.DrawMode) {
    case LCD_DRAWMODE_NORMAL:
    case LCD_DRAWMODE_REV:
      Buffer &= ~(0xF << Shift);
      if (pTrans) {
        Index = *(pTrans + Pixel);
      } else {
        Index = Pixel;
      }
      Data = Index << Shift;
      Buffer |= Data;
      break;
    case LCD_DRAWMODE_XOR:
      if (Pixel)
        Buffer ^= (0xF << Shift);
      break;
    case LCD_DRAWMODE_TRANS:
      if (Pixel) {
        Buffer &= ~(0xF << Shift);
        if (pTrans) {
          Index = *(pTrans + Pixel);
        } else {
          Index = Pixel;
        }
        Data = Index << Shift;
        Buffer |= Data;
      }
      break;
    }
    #if /*LCD_MIRROR_Y || */LCD_MIRROR_X
      if (!(x0 & 3)) {
        BufferValid = 0;
        WRITE_MEM(Off--, Buffer);
      }
    #else
      if ((x0 & 3) == 3) {
        BufferValid = 0;
        WRITE_MEM(Off++, Buffer);
      }
    #endif
  }
  if (BufferValid)
    WRITE_MEM(Off, Buffer);
}


/**********************************************************
*
*          Draw Bitmap 4 BPP, optimized
*
*          16 bit bus, 8 bpp mode
*
*********************************************************
*/
#elif (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 4) \
    && (LCD_BITSPERPIXEL == 8)   \
    && (LCD_MIRROR_X==0)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_COM))


static void  DrawBitLine4BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  U8 RemPixels;
  if (Diff&1) {
    x++;
    RemPixels = 1;
    Pixels <<= 4;
  } else {
    RemPixels = 2;
  }
  /* Handle transparent bitmaps */
  if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
    if (pTrans) {
      do {
        if (RemPixels==0) {
          Pixels = *(++p);
          RemPixels =2;
        }
        if (Pixels & 0xf0) {
          SETPIXEL(x, y, *(pTrans+((Pixels>>4)&0xf)));
        }
        RemPixels--;
        Pixels <<=4;
        x++;
      } while (--xsize);
    } else {
      do {
        if (RemPixels==0) {
          Pixels = *(++p);
          RemPixels =2;
        }
        if (Pixels & 0xf0) {
          SETPIXEL(x, y, ((Pixels>>4)&0xf));
        }
        RemPixels--;
        Pixels <<=4;
        x++;
      } while (--xsize);
    }
  } else {
    tOff Off;
    #if LCD_MIRROR_Y
      y = (LCD_YSIZE-1-(y));
    #endif
    /* Handle solid bitmaps */
    Off = XY2OFF(x,y);
    /* Handle left border (incomplete words) */
    if (x&1) {
      U16 Data, Pixel;
      READ_MEM(Off,Data);
      if (pTrans) {
        Pixel = *(pTrans + ((Pixels >> 4) & 0xf));
      } else {
        Pixel = (Pixels >> 4) & 0xf;
      }
      Data = (Data &  ((unsigned)0xff<<8)) | (Pixel);
      WRITE_MEM(Off, Data);
      xsize--;
      RemPixels--;
      Pixels <<=4;
      Off++;
    }
    /* Handle solid middle area */
    while (xsize >=2) {
      U16 Data;
      U8 Data0,Data1;
      if ((RemPixels&1) ==0) {
        if (RemPixels ==0) {
          Pixels = *++p;
        } else {
          RemPixels =0;
        }
        Data0 = Pixels>>4;
        Data1 = Pixels&15;
      } else {
        Data0 = (Pixels >> 4) & 0xf;
        Pixels = *++p;
        Data1 = Pixels >> 4;
        Pixels <<= 4;
      }
      Data = ((*(pTrans+Data0)) <<8) | ((*(pTrans+Data1)));
      WRITE_MEM(Off, Data);
      xsize-=2;
      Off++;
    }
    /* Handle right border (single pixel) if necessary */
    if (xsize >0) {
      U16 Data;
      U16 Pixel;
      READ_MEM(Off,Data);
      if (RemPixels==0) {
        Pixels =*(p+1);
      }
      if (pTrans) {
        Pixel = *(pTrans + ((Pixels >> 4) & 0xf));
      } else {
        Pixel = (Pixels >> 4) & 0xf;
      }
      Data = (Data & (U16)(0xff)) | (Pixel<<8);
      WRITE_MEM(Off, Data);
    }
  }
}

/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 4 BPP, unoptimized (default)             *
        *                                                       *
        *********************************************************
*/

#else

#if (LCD_BITSPERPIXEL == 4)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  DrawBitLine4BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixels;
/*
// Jump to right entry point
*/
  pixels = *p;
  if (pTrans) {
    /*
      with palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
      if ((Diff&1) ==0)
        goto WriteTBit0;
      goto WriteTBit1;
    } else {
      if ((Diff&1) ==0)
        goto WriteBit0;
      goto WriteBit1;
    }
  /*
          Write without transparency
  */
  WriteBit0:
    SETPIXEL(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteBit1:
    SETPIXEL(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels>>4)
      SETPIXEL(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&0xf)
      SETPIXEL(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteTBit0;
  } else {
    /*
      without palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
      if ((Diff&1) ==0)
        goto WriteDDPTBit0;
      goto WriteDDPTBit1;
    } else {
      if ((Diff&1) ==0)
        goto WriteDDPBit0;
      goto WriteDDPBit1;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    SETPIXEL(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPBit1:
    SETPIXEL(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels>>4)
      SETPIXEL(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&0xf)
      SETPIXEL(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPTBit0;
  }
}

#endif
#endif


/*
*********************************************************
*
*  Draw Bitmap 8 BPP  (256 colors)
*
*      16 bit bus, 4 bpp
*
*********************************************************
*/

#if (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 16) \
    && (LCD_BITSPERPIXEL == 4)   \
    && (LCD_MIRROR_X==0)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_SEG))

static void  DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR *p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  #if LCD_MIRROR_Y
    #define Y (LCD_YSIZE - 1 - y)
  #endif
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    register tOff Off;
    /* Handle bitmaps with palette */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    /* Handle bitmaps without palette */
    } else {
      #if 1 /* Optimized 020104 */
        U16 Data;
        U16 DataOut;
        U16 Mask;
        /* Offset */
        #if LCD_MIRROR_Y
          y = LCD_YSIZE - 1 - y;
        #endif
        Off = XY2OFF(x, y);
        /* Handle left border */
        if (x & 3) {
          /* Load data */
          U8 LeftRem = 4 - (x & 3);
          DataOut = (*(p + LeftRem - 1));
          if (LeftRem > 1) {
            DataOut |= (*(p + LeftRem - 2)) << 4;
            if (LeftRem > 2) {
              DataOut |= (*(p + LeftRem - 3)) << 8;
            }
          }
          /* Mask output data */
          Mask =  0xffff >> (16 - (LeftRem << 2));       /* Mask out pixels to the left */
          if (xsize < LeftRem) {
            Mask &= 0xffff << ((LeftRem - xsize) << 2);  /* Mask out pixels to the right */
          }
          DataOut &= Mask;
          /* Or unchanged pixels */
          READ_MEM(Off, Data);
          DataOut |= Data & ~Mask;
          /* output data */
          WRITE_MEM(Off, DataOut);
          xsize -= LeftRem;
          Off++;
          p += LeftRem;
        }
        /* Handle solid middle area (4 pixels at a time)*/
        while (xsize >= 4) {
          /* Load data */
          DataOut = (*p << 12) | (*(p + 1) << 8) | (*(p + 2) << 4) | *(p + 3);
          p += 4;
          /* output data */
          WRITE_MEM(Off, DataOut);
          /* inc counters / ptrs etc. */
          xsize -= 4;
          Off++;
        }
        /* Handle right border */
        if (xsize >0) {    /* Anything to do at all ? */
          /* Load data */
          DataOut = (*p++) << 12;
          if (xsize > 1) {
            DataOut |= (*p++) << 8;
            if (xsize > 2) {
              DataOut |= (*p) << 4;
            }
          }
          /* Mask output data */
          Mask = 0xffff << ((4 - xsize) << 2);
          DataOut &= Mask;
          /* Or unchanged pixels */
          READ_MEM(Off, Data);
          DataOut |= Data & ~Mask;
          /* output data */
          WRITE_MEM(Off, DataOut);
        }
      #else /* Old */
        for (;(x&3)&& (xsize>0); x++,xsize--) {
          SETPIXEL(x, y, *p++);
        }
        Off = XY2OFF(x,Y);
        /* Write 4 pixels at a time */
        if (xsize >=4) {
          int LoopRem = xsize>>2;
          x += xsize &~3;
          xsize &=3;
          for (;--LoopRem>=0; p+=4,Off++) {
            U16 Data = (*p<<(4+8)) | (*(p+1)<<(0+8))| (*(p+2)<<(4))| (*(p+3));
            LCD_WRITE_MEM(Off, Data);
          }
        }
        for (;xsize>0; x++,xsize--) {
          SETPIXEL(x, y, *p++);
        }
      #endif
    }
  } else {
    /* Handle transparent bitmap with palette */
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, *(pTrans+pixel));
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, pixel);
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 8 BPP  (256 colors)                      *
        *                                                       *
        *      16 bit bus, 8 bpp                                *
        *                                                       *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 16) \
    && (LCD_BITSPERPIXEL == 8)   \
    && (LCD_MIRROR_X==0)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_SEG))

static void  DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR *p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    register tOff Off;
    /* Handle bitmaps with palette */
    if (pTrans) {
      /* Handle first pixel seperatly if on odd position */
      if (x&1) {
        pixel = *p++;
        SETPIXEL(x+0, y, *(pTrans+pixel));
        x++;
        xsize--;
      }
      /* Copy the bulk of the data. */
      #if LCD_MIRROR_Y
        y = (LCD_YSIZE-1-(y));
      #endif
      Off = XY2OFF(x,y);
      x+= xsize&~1;
      /* Optimization for longer lines ... */
      for (;xsize>8; xsize-=8, p+=8, Off+=4) {
        WRITE_MEM(Off,   (*(pTrans+*p)     << 8) | (*(pTrans+*(p+1))));
        WRITE_MEM(Off+1, (*(pTrans+*(p+2)) << 8) | (*(pTrans+*(p+3))));
        WRITE_MEM(Off+2, (*(pTrans+*(p+4)) << 8) | (*(pTrans+*(p+5))));
        WRITE_MEM(Off+3, (*(pTrans+*(p+6)) << 8) | (*(pTrans+*(p+7))));
      }
      /* Handle 2 pixels at a time */
      for (; xsize >= 2; xsize-=2) {
        U16 Data = (*(pTrans+*p) << 8) | (*(pTrans+*(p+1)));
        WRITE_MEM(Off,Data);
        p+=2;
        Off++;
      }
      /* Handle last pixel (if one is left) */
      if (xsize > 0) {
        #if LCD_MIRROR_Y
          y = (LCD_YSIZE-1-(y));
        #endif
        pixel = *p;
        SETPIXEL(x+0, y, *(pTrans+pixel));
        xsize--;
        x++;
        p++;
      }
    /* Handle bitmaps without palette */
    } else {
      #if defined(LCD_DATAADR) & !defined(WIN32)
        memcpy((U16*)(LCD_DATAADR+x+((U32)y)*LCD_VXSIZE), p,xsize);
      #else
        if (x&1) {
          pixel = *p++;
          SETPIXEL(x+0, y, pixel);
          x++;
          xsize--;
        }
        #if LCD_MIRROR_Y
          y = (LCD_YSIZE-1-(y));
        #endif
        Off = XY2OFF(x,y);
        x+= xsize&~1;
        /*
          Copy the bulk of the data. If configured, a special (fast) copy  routine
          can be used, e.g. using DMA. If not available or it did not succeed, we
          do the manual copy
        */
        #ifdef LCD_COPY_MEM16
        if ((xsize > LCD_COPY_MEM16_THRESHOLD) && (LCD_COPY_MEM16(Off, (void *)p, xsize >> 1) == 0)) {
          p     += xsize & 0x7ffe;
          xsize &= 1;
        } else
        #endif
        {
          /* Optimization for longer lines ... */
          for (;xsize >= 8; xsize-=8, p+=8, Off+=4) {
            WRITE_MEM(Off,   (*(p)   << 8) | (*(p+1)));
            WRITE_MEM(Off+1, (*(p+2) << 8) | (*(p+3)));
            WRITE_MEM(Off+2, (*(p+4) << 8) | (*(p+5)));
            WRITE_MEM(Off+3, (*(p+6) << 8) | (*(p+7)));
          }
          /* Handle 2 pixels at a time */
          for (; xsize >= 2; xsize-=2) {
            U16 Data = (*(p) << 8) | (*(p+1));
            WRITE_MEM(Off,Data);
            p+=2;
            Off++;
          }
        }
        /* Handle last pixel (if one is left) */
        if (xsize > 0) {
          #if LCD_MIRROR_Y
            y = (LCD_YSIZE-1-(y));
          #endif
          pixel = *p;
          SETPIXEL(x+0, y, pixel);
          xsize--;
          x++;
          p++;
        }
      #endif
      }
  } else {
    /* Handle transparent bitmap with palette */
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, *(pTrans+pixel));
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, pixel);
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*
        *********************************************************
        *
        *  Draw Bitmap 8 BPP  (256 colors)
        *
        *      16 bit bus, 8 bpp, LCD_MIRROR_X
        *
        *********************************************************
*/

#elif (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 16) \
    && (LCD_BITSPERPIXEL == 8)   \
    && (LCD_MIRROR_X==1)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_SEG))

static void  DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR *p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    register tOff Off;
    /* Handle bitmaps with palette */
    if (pTrans) {
      /* Handle first pixel seperatly if on odd position */
      if (x&1) {
        pixel = *p++;
        SETPIXEL(x+0, y, *(pTrans+pixel));
        x++;
        xsize--;
      }
      #if LCD_MIRROR_Y
        y = (LCD_YSIZE-1-(y));
      #endif
      Off = XY2OFF((LCD_XSIZE - x - 1),y);
      x+= xsize&~1;
      /* Optimization for longer lines ... */
      for (;xsize>8; xsize-=8, p+=8, Off-=4) {
        WRITE_MEM(Off,   (*(pTrans+*p)    ) | ((*(pTrans+*(p+1))) << 8));
        WRITE_MEM(Off-1, (*(pTrans+*(p+2))) | ((*(pTrans+*(p+3))) << 8));
        WRITE_MEM(Off-2, (*(pTrans+*(p+4))) | ((*(pTrans+*(p+5))) << 8));
        WRITE_MEM(Off-3, (*(pTrans+*(p+6))) | ((*(pTrans+*(p+7))) << 8));
      }
      /* Handle 2 pixels at a time */
      for (; xsize >= 2; xsize-=2) {
        U16 Data = (*(pTrans+*p)) | ((*(pTrans+*(p+1))) << 8);
        WRITE_MEM(Off,Data);
        p+=2;
        Off--;
      }
      /* Handle last pixel (if one is left) */
      if (xsize > 0) {
        #if LCD_MIRROR_Y
          y = (LCD_YSIZE-1-(y));
        #endif
        pixel = *p;
        SETPIXEL(x, y, *(pTrans+pixel));
      }
    /* Handle bitmaps without palette */
    } else {
      #if defined(LCD_DATAADR) & !defined(WIN32)
        memcpy((U16*)(LCD_DATAADR+x+((U32)y)*LCD_VXSIZE), p,xsize);
      #else
        if (x&1) {
          pixel = *p++;
          SETPIXEL(x+0, y, pixel);
          x++;
          xsize--;
        }
        #if LCD_MIRROR_Y
          y = (LCD_YSIZE-1-(y));
        #endif
        Off = XY2OFF((LCD_XSIZE - x - 1),y);
        x+= xsize&~1;
        /* Optimization for longer lines ... */
        for (;xsize>8; xsize-=8, p+=8, Off-=4) {
          WRITE_MEM(Off,   (*(p)  ) | ((*(p+1)) << 8));
          WRITE_MEM(Off-1, (*(p+2)) | ((*(p+3)) << 8));
          WRITE_MEM(Off-2, (*(p+4)) | ((*(p+5)) << 8));
          WRITE_MEM(Off-3, (*(p+6)) | ((*(p+7)) << 8));
        }
        /* Handle 2 pixels at a time */
        for (; xsize >= 2; xsize-=2) {
          U16 Data = (*(p)) | ((*(p+1)) << 8);
          WRITE_MEM(Off,Data);
          p+=2;
          Off--;
        }
        /* Handle last pixel (if one is left) */
        if (xsize > 0) {
          #if LCD_MIRROR_Y
            y = (LCD_YSIZE-1-(y));
          #endif
          pixel = *p;
          SETPIXEL(x+0, y, pixel);
        }
      #endif
      }
  } else {
    /* Handle transparent bitmap with palette */
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, *(pTrans+pixel));
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0)
          SETPIXEL(x+0, y, pixel);
        xsize--;
        x++;
        p++;
      }
    }
  }
}

#else


/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 8 BPP  (256 colors)                      *
        *                                                       *
        *      Default (no optimization)                        *
        *                                                       *
        *********************************************************
*/

#if (LCD_BITSPERPIXEL == 8)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR *p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize > 0; xsize--,x++,p++) {
        pixel = *p;
        SETPIXEL(x, y, *(pTrans+pixel));
      }
    } else {
      for (;xsize > 0; xsize--,x++,p++) {
        SETPIXEL(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, pixel);
        }
      }
    }
  }
}

#endif

/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 8 BPP  (256 colors)                      *
        *                                                       *
        *      16 bit bus, 8 bpp                                *
        *                                                       *
        *********************************************************
*/

#if (LCD_BITSPERPIXEL > 8)
#if (LCD_OPTIMIZE)               \
    && (LCD_MAX_LOG_COLORS > 16) \
    && (LCD_BITSPERPIXEL == 16)   \
    && (LCD_MIRROR_X==0)         \
    && (LCD_BUSWIDTH==16)        \
    && (!LCD_SWAP_XY)            \
    && (!defined (LCD_LUT_SEG))

static void  DrawBitLine16BPP(int x, int y, U16 const GUI_UNI_PTR *p, int xsize) {
  LCD_PIXELINDEX Index;
  register tOff Off;
  #if LCD_MIRROR_Y
    y = (LCD_YSIZE-1-(y));
  #endif
  Off = XY2OFF(x,y);
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    /* Always write first pixel */
    Index = *p;
    WRITE_MEM(Off, Index);
    /* Unrolled loop */
    while ( xsize > 4 ) {
      WRITE_MEM(Off+1, *(p+1));
      WRITE_MEM(Off+2, *(p+2));
      WRITE_MEM(Off+3, *(p+3));
      WRITE_MEM(Off+4, *(p+4));
      Off += 4;
      p += 4;
      xsize -=4;
    }
    /* End loop */
    for (; --xsize; ) {
      Off++;
      Index = *++p;
      WRITE_MEM(Off, Index);
    }
  } else {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++) {
      Index = *p;
      if (Index) {
        SETPIXEL(x+0, y, Index);
      }
    }
  }
}

/*
        *********************************************************
        *                                                       *
        *  Draw Bitmap 16 BPP
        *                                                       *
        *      Default (no optimization)                        *
        *                                                       *
        *********************************************************
*/

#else

#if (LCD_BITSPERPIXEL == 16)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  DrawBitLine16BPP(int x, int y, U16 const GUI_UNI_PTR *p, int xsize) {
  LCD_PIXELINDEX Index;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    for (;xsize > 0; xsize--,x++,p++) {
      SETPIXEL(x, y, *p);
    }
  } else {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++) {
      Index = *p;
      if (Index) {
        SETPIXEL(x+0, y, Index);
      }
    }
  }
}
#endif
#endif
/*
        *********************************************************
        *                                                       *
        *         Universal draw Bitmap routine                 *
        *                                                       *
        *********************************************************
*/

void LCD_L0_DrawBitmap   (int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel,
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  /*
     Use BitBlt engine (if available)
  */
  #if LCD_USE_BITBLT              \
      && (LCD_BUSWIDTH==16)       \
      && ((LCD_CONTROLLER == 1304) || (LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806)) \
      && (LCD_MIRROR_X==0)         \
      && (!defined (LCD_LUT_COM))  \
      && (!defined (LCD_LUT_SEG))
    if ((BitsPerPixel==1) && !(GUI_Context.DrawMode & LCD_DRAWMODE_XOR)) {
      _DrawBitmap1BPPBB(x0, y0, pData, Diff, xsize, ysize, BytesPerLine, pTrans);
      return;
    }
  #endif
  /*
     Use DrawBitLineXBPP
  */
  switch (BitsPerPixel) {
  case 1:
    #if (LCD_OPTIMIZE)               \
        && (LCD_SWAP_XY)             \
        && (!defined (LCD_LUT_COM))  \
        && (!defined (LCD_LUT_SEG))  \
        && (LCD_BUSWIDTH==16)        \
        && ((LCD_BITSPERPIXEL == 4) || ((LCD_BITSPERPIXEL == 2) && (LCD_MIRROR_X == 1) && (LCD_MIRROR_Y == 0)))
      xsize += Diff & 7;
      for (i = Diff; i < xsize; i++) {
        DrawBitLine1BPP_Swap(x0+i, y0, pData + (i>>3), ysize, pTrans, BytesPerLine, i);
      }
    #else
      for (i=0; i<ysize; i++) {
        DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
        pData += BytesPerLine;
      }
    #endif
    break;
  #if (LCD_MAX_LOG_COLORS > 2)
    case 2:
      for (i=0; i<ysize; i++) {
        DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
        pData += BytesPerLine;
      }
      break;
  #endif
  #if (LCD_MAX_LOG_COLORS > 4)
    case 4:
      #if (LCD_OPTIMIZE)               \
          && (LCD_SWAP_XY)             \
          && (!defined (LCD_LUT_COM))  \
          && (!defined (LCD_LUT_SEG))  \
          && (LCD_BITSPERPIXEL == 4)   \
          && (LCD_BUSWIDTH==16)
        xsize += Diff & 1;
        for (i = Diff; i < xsize; i++) {
          DrawBitLine4BPP_Swap(x0+i, y0, pData + (i>>1), ysize, pTrans, BytesPerLine, i);
        }
      #else
        for (i=0; i<ysize; i++) {
          DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
          pData += BytesPerLine;
        }
      #endif
      break;
  #endif
  #if (LCD_MAX_LOG_COLORS > 16)
    case 8:
      for (i=0; i<ysize; i++) {
        DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
        pData += BytesPerLine;
      }
      break;
  #endif
  #if (LCD_BITSPERPIXEL > 8)
    case 16:
      for (i=0; i<ysize; i++) {
        DrawBitLine16BPP(x0, i+y0, (U16*)pData, xsize);
        pData += BytesPerLine;
      }
      break;
  #endif
  }
}


/********************************************************
*
*       LCD_L0_SetOrg
*
*********************************************************

Purpose:        Sets the original position of the virtual display.
                Has no function at this point with the PC-driver.
*/

void LCD_L0_SetOrg(int x, int y) {
  LCD_SET_ORG(x, y);
}


/*
        *********************************************************
        *                                                       *
        *       LCD_On                                          *
        *       LCD_Off                                         *
        *                                                       *
        *********************************************************
*/

void LCD_Off          (void) {
#ifdef LCD_OFF
  LCD_ENABLE_REG_ACCESS();
  LCD_OFF();
  LCD_ENABLE_MEM_ACCESS();
#endif
}
void LCD_On           (void) {
#ifdef LCD_ON
  LCD_ENABLE_REG_ACCESS();
  LCD_ON();
  LCD_ENABLE_MEM_ACCESS();
#endif
}


unsigned int LCD_L0_GetPixelIndex(int x, int y) {
  return GETPIXEL(x,y);
}


/*********************************************************************
*
*       LCD_L0_SetLUTEntry
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
  #if defined(LCD_SET_LUT_ENTRY)
    LCD_SET_LUT_ENTRY(Pos, Color);
  #else
    GUI_USE_PARA(Pos);
    GUI_USE_PARA(Color);
  #endif
}

/*
        *********************************************************
        *                                                       *
        *       LCD_L0_ReInit : Re-Init the display                *
        *                                                       *
        *********************************************************

ReInit contains all of the code that can be re-executed at any point without
changing or even disturbing what can be seen on the LCD.
Note that it is also used as a subroutine by LCD_Init().

*/


void  LCD_L0_ReInit(void) {
  LCD_ENABLE_REG_ACCESS();
  LCD_INIT_CONTROLLER();                     /* macro defined in config */
#if LCD_USE_BITBLT
  #if ((LCD_CONTROLLER == 1356) || (LCD_CONTROLLER == 13806))
    WRITE_REG(0x10c/2,WORDSPERLINE);       /* Write Bit blit engine line offset */
    WRITE_REG(0x100/2,0x0);                /* Stop engine (to be on the safe side ...) */
  #elif (LCD_CONTROLLER == 1304)
    _WriteReg16L(0x8014, WORDSPERLINE);
  #endif
#endif
  LCD_ENABLE_MEM_ACCESS();
}


/*
        *********************************************************
        *                                                       *
        *       LCD_Init : Init the display                     *
        *                                                       *
        *********************************************************
*/
int  LCD_L0_Init(void) {
  GUI_DEBUG_LOG("\nLCD_Init()");
/* check production code */
/*
#if LCD_CONTROLLER == 1374
  if ((READ_REG_BYTE(0) &0xfc) != (6<<2)) {
    GUI_DEBUG_ERROROUT("\nLCD13XX: 1374 controller not found");
    return LCD_ERR_CONTROLLER_NOT_FOUND;
	}
#elif LCD_CONTROLLER == 1375
  if ((READ_REG_BYTE(0) >>2) != 0x9) {
    GUI_DEBUG_ERROROUT("\nLCD13XX: 1375 controller not found");
    return LCD_ERR_CONTROLLER_NOT_FOUND;
	}
#endif
*/
#if LCD_CONTROLLER == 1374
  { int ProdCode;
    LCD_ENABLE_REG_ACCESS();
    ProdCode = READ_REG_BYTE(0) >>2;
    LCD_ENABLE_MEM_ACCESS();
    if ((ProdCode != 6)    /* 1374 */
      &&(ProdCode != 9))   /* 1375 */
    return LCD_ERR_CONTROLLER_NOT_FOUND;
  }
#endif
/* Init controllers (except lookup table) */
  LCD_L0_ReInit();
  LCD_Off();
/* Quick check video memory */
#if (LCD_BUSWIDTH == 16)
  {
    LCD_COLOR Color_0, Color_1;
    Color_0 = LCD_READ_MEM(0);
    Color_1 = LCD_READ_MEM(1);
    LCD_WRITE_MEM(0,0);
    LCD_WRITE_MEM(1,0x5253);
    if ((LCD_READ_MEM(0) != 0) || (LCD_READ_MEM(1) != 0x5253)) {
      GUI_DEBUG_ERROROUT("\nLCD13XX: video memory problem");
      return LCD_ERR_MEMORY;
    }
    LCD_WRITE_MEM(0, Color_0); /* Leave contents unchanged */
    LCD_WRITE_MEM(1, Color_1); /* Leave contents unchanged */
  }
#else
  {
    LCD_COLOR Color_0, Color_1;
    Color_0 = LCD_READ_MEM(0);
    Color_1 = LCD_READ_MEM(1);
    LCD_WRITE_MEM(0,0);
    LCD_WRITE_MEM(1,0x52);
    if ((LCD_READ_MEM(0) != 0) || (LCD_READ_MEM(1) != 0x52)) {
      GUI_DEBUG_ERROROUT("\nLCD13XX: video memory problem");
      return LCD_ERR_MEMORY;
    }
    LCD_WRITE_MEM(0, Color_0); /* Leave contents unchanged */
    LCD_WRITE_MEM(1, Color_1); /* Leave contents unchanged */
  }
#endif
  return 0;    /* Init successfull ! */
}

/*********************************************************************
*
*       LCD_L0_ReadRect, default orientation
*/
#if (LCD_OPTIMIZE)              \
    && (!LCD_SWAP_XY)           \
    && (!LCD_MIRROR_X)          \
    && (!LCD_MIRROR_Y)          \
    && (LCD_BUSWIDTH == 16)     \
    && (!defined (LCD_LUT_COM)) \
    && (!defined (LCD_LUT_SEG)) \
    && (LCD_BITSPERPIXEL == 8)
static void LCD_L0_ReadRect(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer) {
  register tOff Off = XY2OFF(x0, y0);
  U16 Data;
  int x;
  x = x0;
  do {
    Off = XY2OFF(x0, y0);
    if (x0 & 1) {
      READ_MEM(Off, Data);
      *(pBuffer++) = Data & 0xff;
      x0++;
      Off++;
    }
    for (;x0 < x1; x0 += 2, Off++, pBuffer += 2) {
      READ_MEM(Off, Data);
      *(pBuffer    ) = Data >> 8;
      *(pBuffer + 1) = Data & 0xff;
    }
    if ((x1 & 1) == 0) {
      READ_MEM(Off, Data);
      *(pBuffer++) = Data >> 8;
    }
    x0 = x;
  } while (y0++ < y1);
}

/*********************************************************************
*
*       LCD_L0_ReadRect, LCD_SWAP_XY = 0, LCD_MIRROR_X = 1, LCD_MIRROR_Y = 1
*/
#elif (LCD_OPTIMIZE)            \
    && (!LCD_SWAP_XY)           \
    && (LCD_MIRROR_X)           \
    && (LCD_MIRROR_Y)           \
    && (LCD_BUSWIDTH == 16)     \
    && (!defined (LCD_LUT_COM)) \
    && (!defined (LCD_LUT_SEG)) \
    && (LCD_BITSPERPIXEL == 8)
static void LCD_L0_ReadRect(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer) {
  register tOff Off = XY2OFF(x0, y0);
  U16 Data;
  int x, x0Phys, y0Phys, x1Phys, y1Phys;
  x0Phys = LCD_XSIZE - x1 - 1;
  x1Phys = LCD_XSIZE - x0 - 1;
  y0Phys = LCD_YSIZE - y1 - 1;
  y1Phys = LCD_YSIZE - y0 - 1;
  x = x1Phys;
  do {
    Off = XY2OFF(x1Phys, y1Phys);
    if ((x1Phys & 1) == 0) {
      READ_MEM(Off, Data);
      *(pBuffer++) = Data >> 8;
      x1Phys--;
      Off--;
    }
    for (;x1Phys > x0Phys; x1Phys -= 2, Off--, pBuffer += 2) {
      READ_MEM(Off, Data);
      *(pBuffer + 1) = Data >> 8;
      *(pBuffer    ) = Data & 0xff;
    }
    if (x0Phys & 1) {
      READ_MEM(Off, Data);
      *(pBuffer++) = Data & 0xff;
    }
    x1Phys = x;
  } while (y1Phys-- > y0Phys);
}

#else

#define LCD_L0_ReadRect NULL

#endif

/*********************************************************************
*
*       LCD_L0_GetDevFunc
*/
void * LCD_L0_GetDevFunc(int Index) {
  switch (Index) {
  case LCD_DEVFUNC_READRECT:
    return (void *)LCD_L0_ReadRect;
  }
  return NULL;
}

#else

void LCD13XX_c(void);
void LCD13XX_c(void) { } /* avoid empty object files */

#endif  /* (LCD_CONTROLLER/100 == 13) */

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
