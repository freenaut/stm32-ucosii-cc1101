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
File        : LCDLin32.C
Purpose     : Driver for accessing linear video memory with 32bit operations
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>
#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"

#if (LCD_CONTROLLER == 3200) \
    && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#ifndef LCD_ENDIAN_BIG
  #error Please define the endian mode of the video RAM!
#endif

#ifndef   LCD_INIT_CONTROLLER
  #define LCD_INIT_CONTROLLER()
#endif

#ifndef   LCD_SET_LUT_ENTRY
  #define LCD_SET_LUT_ENTRY(Pos, Color) GUI_USE_PARA(Pos); GUI_USE_PARA(Color)
#endif

#ifndef   LCD_SET_ORG
  #define LCD_SET_ORG(x, y) GUI_USE_PARA(x); GUI_USE_PARA(y)
#endif

#ifndef   LCD_LIN_SWAP
  #define LCD_LIN_SWAP 0
#endif

/* Error for unsupported modes */
#if (LCD_BITSPERPIXEL != 1) && \
    (LCD_BITSPERPIXEL != 2) && \
    (LCD_BITSPERPIXEL != 4) && \
    (LCD_BITSPERPIXEL != 8) && \
    (LCD_BITSPERPIXEL != 16)
#error This mode is currently not supported! Please contact support!
#endif

/* Mappings for multi layer configurations */
#if   LCD_DISPLAY_INDEX == 1
  #define SIM_WriteMem32 SIM_WriteMem32_1
  #define SIM_ReadMem32  SIM_ReadMem32_1
#elif LCD_DISPLAY_INDEX == 2
  #define SIM_WriteMem32 SIM_WriteMem32_2
  #define SIM_ReadMem32  SIM_ReadMem32_2
#elif LCD_DISPLAY_INDEX == 3
  #define SIM_WriteMem32 SIM_WriteMem32_3
  #define SIM_ReadMem32  SIM_ReadMem32_3
#elif LCD_DISPLAY_INDEX == 4
  #define SIM_WriteMem32 SIM_WriteMem32_4
  #define SIM_ReadMem32  SIM_ReadMem32_4
#endif

#ifdef WIN32
  U32  SIM_ReadMem32 (unsigned int Off);
  void SIM_WriteMem32(unsigned int Off, U32 Data);
  #undef  LCD_SET_ORG
  #ifdef   LCD_READ_MEM
    #undef LCD_READ_MEM
  #endif
  #ifdef   LCD_WRITE_MEM
    #undef LCD_WRITE_MEM
  #endif
  #define LCD_READ_MEM(Off)        SIM_ReadMem32(Off)
  #define LCD_WRITE_MEM(Off, Data) SIM_WriteMem32(Off, Data)
  #define LCD_SET_ORG(x, y)    GUI_USE_PARA(x); GUI_USE_PARA(y)
#else
  #ifndef   LCD_READ_MEM
    #define LCD_READ_MEM(Off)        (*((U32 *)LCD_VRAM_ADR + (U32)Off))
  #endif
  #ifndef   LCD_WRITE_MEM
    #define LCD_WRITE_MEM(Off, Data) *((U32 *)LCD_VRAM_ADR + (U32)Off) = Data
  #endif
#endif

#if LCD_LIN_SWAP
  #define WRITE_MEM(Off, Data) LCD_WRITE_MEM(Off, _Swap(Data))
  #define READ_MEM(Off)        _Swap(LCD_READ_MEM(Off))
#else
  #define WRITE_MEM(Off, Data) LCD_WRITE_MEM(Off, Data)
  #define READ_MEM(Off)        LCD_READ_MEM(Off)
#endif

#define PIXELS_PER_DWORD (32 / LCD_BITSPERPIXEL)
#define DWORDS_PER_LINE (LCD_VXSIZE_PHYS / PIXELS_PER_DWORD)

#if   (LCD_BITSPERPIXEL == 16)
  #define XY2OFF(x,y)    ((unsigned)y * DWORDS_PER_LINE + ((unsigned)x >> 1))
#elif (LCD_BITSPERPIXEL ==  8)
  #define XY2OFF(x,y)    ((unsigned)y * DWORDS_PER_LINE + ((unsigned)x >> 2))
#elif (LCD_BITSPERPIXEL ==  4)
  #define XY2OFF(x,y)    ((unsigned)y * DWORDS_PER_LINE + ((unsigned)x >> 3))
#elif (LCD_BITSPERPIXEL ==  2)
  #define XY2OFF(x,y)    ((unsigned)y * DWORDS_PER_LINE + ((unsigned)x >> 4))
#elif (LCD_BITSPERPIXEL ==  1)
  #define XY2OFF(x,y)    ((unsigned)y * DWORDS_PER_LINE + ((unsigned)x >> 5))
#endif

/*********************************************************************
*
*       Macros for MIRROR_, SWAP_ and LUT_
*/
#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) y
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) x
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) y
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #endif
#else
  #if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) y
  #elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #endif
#endif

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
#if LCD_LIN_SWAP
static U32 _Swap(U32 Data) {
  #if   LCD_LIN_SWAP == 1
    Data = ((Data & 0x01010101) << 7) | ((Data & 0x02020202) << 5) | ((Data & 0x04040404) << 3) | ((Data & 0x08080808) << 1) | ((Data & 0x10101010) >> 1) | ((Data & 0x20202020) >> 3) | ((Data & 0x40404040) >> 5) | ((Data & 0x80808080) >> 7);
  #elif LCD_LIN_SWAP == 2
    Data = ((Data & 0x03030303) << 6) | ((Data & 0x0C0C0C0C) << 2) | ((Data & 0x30303030) >> 2) | ((Data & 0xC0C0C0C0) >> 6);
  #elif LCD_LIN_SWAP == 4
    Data = ((Data & 0xF0F0F0F0) >> 4) | ((Data & 0x0F0F0F0F) << 4);
  #else
    #error Unsupported swapping mode!
  #endif
  return Data;
}
#endif

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 16
*/
#if (LCD_ENDIAN_BIG == 0)    && \
    (LCD_BITSPERPIXEL == 16) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Index0 = *(pTrans + 0);
  LCD_PIXELINDEX Index1 = *(pTrans + 1);
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    {
      U32 Data, Color;
      unsigned Pixels, PixelCnt, Off;
      PixelCnt = 8 - Diff;
      Pixels   = LCD_aMirror[*p] >> Diff;
      Off      = y * DWORDS_PER_LINE + (x >> 1);
      /* First DWORD */
      if (x & 1) {
        Data = READ_MEM(Off) & 0xFFFF;
        Data |= ((Pixels & 1) ? Index1 : Index0) << 16;
        WRITE_MEM(Off, Data);
        Off++;
        xsize--;
        PixelCnt--;
        Pixels >>= 1;
      }
      /* Complete DWORDS */
      if (xsize >= 2) {
        do {
          /* Make sure we have enough pixels loaded */
          if (PixelCnt < 2) {
            Pixels |= LCD_aMirror[*(++p)] << PixelCnt;
            PixelCnt += 8;
          }
          Color     =  Pixels & 1 ? Index1 : Index0;
          Color    |= (Pixels & 2 ? Index1 : Index0) << 16;
          Pixels  >>= 2;
          PixelCnt -= 2;
          WRITE_MEM(Off, Color);
          Off++;
        } while ((xsize -= 2) >= 2);
      }
      /* Last DWORD */
      if (xsize) {
        /* Make sure we have enough pixels loaded */
        if (PixelCnt == 0) {
          Pixels = LCD_aMirror[*(++p)];
        }
        Data = READ_MEM(Off) & 0xFFFF0000;
        Data |= (Pixels & 1) ? Index1 : Index0;
        WRITE_MEM(Off, Data);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    {
      U32 Data;
      unsigned PixelPos;
      unsigned Pixels, PixelCnt, Off;
      PixelCnt = 8 - Diff;
      Pixels   = LCD_aMirror[*p] >> Diff;
      do {
        /* Prepare loop */
        if ((int)PixelCnt > xsize) {
          PixelCnt = xsize;
        }
        xsize -= PixelCnt;
        while (Pixels && PixelCnt) {
          if ((PixelCnt >= 2) && !(x & 1)) {
            PixelPos = (Pixels & 3);
            if (PixelPos) {
              Off = y * DWORDS_PER_LINE + (x >> 1);
              if (PixelPos == 3) {
                WRITE_MEM(Off, Index1 * 0x00010001);
              } else {
                Data = READ_MEM(Off);
                if (PixelPos == 1) {
                  Data &= 0xFFFF0000;
                  Data |= Index1;
                } else {
                  Data &= 0xFFFF;
                  Data |= Index1 << 16;
                }
                WRITE_MEM(Off, Data);
              }
            }
            x += 2;
            Pixels  >>= 2;
            PixelCnt -= 2;
          } else {
            if ((Pixels & 1)) {
              Off = y * DWORDS_PER_LINE + (x >> 1);;
              Data = READ_MEM(Off);
              if (x & (PIXELS_PER_DWORD - 1)) {
                Data &= 0xFFFF;
                Data |= Index1 << 16;
              } else {
                Data &= 0xFFFF0000;
                Data |= Index1;
              }
              WRITE_MEM(Off, Data);
            }
            x++;
            Pixels >>= 1;
            PixelCnt--;
          }
        }
        /* Check if an other Source byte needs to be loaded */
        if (xsize == 0) {
          return;
        }
        x += PixelCnt;
        PixelCnt = 8;
        Pixels = LCD_aMirror[*(++p)];
      } while (1);
    }
  case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
  case LCD_DRAWMODE_XOR:
    do {
      if (*p & (0x80 >> Diff)) {
        int Pixel = LCD_L0_GetPixelIndex(x, y);
        LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 8) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  U8 Mode;
  LCD_PIXELINDEX Index0 = *(pTrans + 0);
  LCD_PIXELINDEX Index1 = *(pTrans + 1);
  x += Diff;
  Mode = GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR);
  if (Mode == 0) {
    /* Check if filling will do ... */
    if (Index0 == Index1) {
      LCD_PIXELINDEX ColorIndexOld = LCD_COLORINDEX;  /* Save forground color */
      LCD_COLORINDEX = Index0;                        /* Set foreground color */
      LCD_L0_DrawHLine(x, y, x + xsize - 1);
      LCD_COLORINDEX = ColorIndexOld;
    } else { 
      /* O.K., we have to draw ... */
      int Off, NumPixel_0, NumPixel_1, i;
      U32 Data, ColorMask, AndMask;
      Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
      NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
      NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
        if ((xsize < 3) && (NumPixel_1)) {
          AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 4) && xsize; i++, xsize--) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (8 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 4) {
        ColorMask = 0;
        for (i = 0; i < 4; i++) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (8 * i);
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 4;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
        while (xsize) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (8 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    switch (Mode) {
    case LCD_DRAWMODE_TRANS:
      do {
        if (*p & (0x80 >> Diff)) {
          LCD_L0_SetPixelIndex(x, y, Index1);
        }
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
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8, LCD_SWAP_XY == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 8) && \
    ( LCD_SWAP_XY)
static void _DrawBitLine1BPP_Swap(unsigned x, unsigned y, U8 const GUI_UNI_PTR * pData, int ysize, const U8 * pTrans, int BytesPerLine, U8 Pos) {
  unsigned x0, y0, Off;
  U32 Buffer, Data;
  U8 Index, Pixel, BufferValid, ShiftPos, DataMask;
  #if LCD_MIRROR_Y
    y0 = LCD_XSIZE - x - 1;
  #else
    y0 = x;
  #endif
  #if LCD_MIRROR_X
    x0 = LCD_YSIZE - y - 1;
  #else
    x0 = y;
  #endif
  Off         = XY2OFF(x0, y0);
  BufferValid = 0;
  ShiftPos    = 7 - (Pos & 7);
  DataMask    = 0x80 >> (7 - ShiftPos);
  #if LCD_MIRROR_X
    for (; ysize; ysize--, pData += BytesPerLine, x0--) {
  #else
    for (; ysize; ysize--, pData += BytesPerLine, x0++) {
  #endif
    U8 Shift = (x0 & 0x3) << 3;
    Pixel = (*pData & DataMask) >> ShiftPos;
    if (!BufferValid) {
      Buffer = READ_MEM(Off);
      BufferValid = 1;
    }
    switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      Buffer &= ~(0xff << Shift);
      Index = *(pTrans + Pixel);
      Data = Index << Shift;
      Buffer |= Data;
      break;
    case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
    case LCD_DRAWMODE_XOR:
      if (Pixel) {
        Buffer ^= (0xff << Shift);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (Pixel) {
        Buffer &= ~(0xff << Shift);
        Index = *(pTrans + Pixel);
        Data = Index << Shift;
        Buffer |= Data;
      }
      break;
    }
    #if LCD_MIRROR_X
      if (!(x0 & 0x3)) {
        BufferValid = 0;
        WRITE_MEM(Off--, Buffer);
      }
    #else
      if ((x0 & 0x3) == 0x3) {
        BufferValid = 0;
        WRITE_MEM(Off++, Buffer);
      }
    #endif
  }
  if (BufferValid) {
    WRITE_MEM(Off, Buffer);
  }
}

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 4
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 4) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  U8 Mode;
  LCD_PIXELINDEX Index0 = *(pTrans + 0);
  LCD_PIXELINDEX Index1 = *(pTrans + 1);
  x += Diff;
  Mode = GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR);
  if (Mode == 0) {
    /* Check if filling will do ... */
    if (Index0 == Index1) {
      LCD_PIXELINDEX ColorIndexOld = LCD_COLORINDEX;  /* Save forground color */
      LCD_COLORINDEX = Index0;                        /* Set foreground color */
      LCD_L0_DrawHLine(x, y, x + xsize - 1);
      LCD_COLORINDEX = ColorIndexOld;
    } else { 
      /* O.K., we have to draw ... */
      int Off, NumPixel_0, NumPixel_1, i;
      U32 Data, ColorMask, AndMask;
      Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
      NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
      NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
        if ((8 - NumPixel_0) > xsize) {
          AndMask |= ~(0xFFFFFFFF >> (4 * (7 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 8) && xsize; i++, xsize--) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (4 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 8) {
        ColorMask = 0;
        for (i = 0; i < 8; i++) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (4 * i);
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 8;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
        while (xsize) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (4 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    switch (Mode) {
    case LCD_DRAWMODE_TRANS:
      do {
        if (*p & (0x80 >> Diff)) {
          LCD_L0_SetPixelIndex(x, y, Index1);
        }
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
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 2
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 2) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  U8 Mode;
  LCD_PIXELINDEX Index0 = *(pTrans + 0);
  LCD_PIXELINDEX Index1 = *(pTrans + 1);
  x += Diff;
  Mode = GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR);
  if (Mode == 0) {
    /* Check if filling will do ... */
    if (Index0 == Index1) {
      LCD_PIXELINDEX ColorIndexOld = LCD_COLORINDEX;  /* Save forground color */
      LCD_COLORINDEX = Index0;                        /* Set foreground color */
      LCD_L0_DrawHLine(x, y, x + xsize - 1);
      LCD_COLORINDEX = ColorIndexOld;
    } else { 
      /* O.K., we have to draw ... */
      int Off, NumPixel_0, NumPixel_1, i;
      U32 Data, ColorMask, AndMask;
      Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
      NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
      NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (2 * NumPixel_0));
        if ((16 - NumPixel_0) > xsize) {
          AndMask |= ~(0xFFFFFFFF >> (2 * (15 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 16) && xsize; i++, xsize--) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (2 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 16) {
        ColorMask = 0;
        for (i = 0; i < 16; i++) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (2 * i);
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 16;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFC << (2 * NumPixel_1);
        while (xsize) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << (2 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    switch (Mode) {
    case LCD_DRAWMODE_TRANS:
      do {
        if (*p & (0x80 >> Diff)) {
          LCD_L0_SetPixelIndex(x, y, Index1);
        }
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
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 2, LCD_SWAP_XY == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 2) && \
    (LCD_SWAP_XY)
static void _DrawBitLine1BPP_Swap(unsigned x, unsigned y, U8 const GUI_UNI_PTR * pData, int ysize, const U8 * pTrans, int BytesPerLine, U8 Pos) {
  unsigned x0, y0, Off;
  U32 Buffer, Data;
  U8 Index, Pixel, BufferValid, ShiftPos, DataMask;
  #if LCD_MIRROR_Y
    y0 = LCD_XSIZE - x - 1;
  #else
    y0 = x;
  #endif
  #if LCD_MIRROR_X
    x0 = LCD_YSIZE - y - 1;
  #else
    x0 = y;
  #endif
  Off         = XY2OFF(x0, y0);
  BufferValid = 0;
  ShiftPos    = 7 - (Pos & 7);
  DataMask    = 0x80 >> (7 - ShiftPos);
  #if LCD_MIRROR_X
    for (; ysize; ysize--, pData += BytesPerLine, x0--) {
  #else
    for (; ysize; ysize--, pData += BytesPerLine, x0++) {
  #endif
    U8 Shift = (x0 & 0xf) << 1;
    Pixel = (*pData & DataMask) >> ShiftPos;
    if (!BufferValid) {
      Buffer = READ_MEM(Off);
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
      if (Pixel) {
        Buffer ^= (0x3 << Shift);
      }
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
    #if LCD_MIRROR_X
      if (!(x0 & 0xf)) {
        BufferValid = 0;
        WRITE_MEM(Off--, Buffer);
      }
    #else
      if ((x0 & 0xf) == 0xf) {
        BufferValid = 0;
        WRITE_MEM(Off++, Buffer);
      }
    #endif
  }
  if (BufferValid) {
    WRITE_MEM(Off, Buffer);
  }
}

/*********************************************************************
*
*       Draw Bitmap 1 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 1) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
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
      LCD_PIXELINDEX ColorIndexOld = LCD_COLORINDEX;  /* Save forground color */
      LCD_COLORINDEX = Index0;                        /* Set foreground color */
      LCD_L0_DrawHLine(x, y, x + xsize - 1);
      LCD_COLORINDEX = ColorIndexOld;
    } else { 
      /* O.K., we have to draw ... */
      int Off, NumPixel_0, NumPixel_1, i;
      U32 Data, ColorMask, AndMask;
      Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
      NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
      NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << NumPixel_0);
        if ((32 - NumPixel_0) > xsize) {
          AndMask |= ~(0xFFFFFFFF >> (31 - NumPixel_1));
        }
        for (i = NumPixel_0; (i < 32) && xsize; i++, xsize--) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << i;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 32) {
        ColorMask = 0;
        for (i = 0; i < 32; i++) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << i;
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 32;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFE << NumPixel_1;
        while (xsize) {
          U8 Index = *p & (0x80 >> Diff) ? Index1 : Index0;
          if (++Diff == 8) {
            Diff = 0;
            p++;
          }
          ColorMask |= Index << i++;
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    switch (Mode) {
    case LCD_DRAWMODE_TRANS:
      do {
        if (*p & (0x80 >> Diff)) {
          LCD_L0_SetPixelIndex(x, y, Index1);
        }
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
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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

/*********************************************************************
*
*       Draw Bitmap 1 BPP, not optimized
*/
#else

#if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
  #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
#endif

static void _DrawBitLine1BPP(unsigned x, unsigned y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
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
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    do {
      LCD_L0_SetPixelIndex(x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  case LCD_DRAWMODE_TRANS:
    do {
      if (*p & (0x80 >> Diff))
        LCD_L0_SetPixelIndex(x, y, Index1);
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
        LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
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
#endif

/*********************************************************************
*
*       Draw Bitmap 2 BPP
*/
#if (LCD_MAX_LOG_COLORS > 2)
static void  _DrawBitLine2BPP(int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          LCD_L0_SetPixelIndex(x++, y, PixelIndex);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
        } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          LCD_L0_SetPixelIndex(x++, y, Index);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
        } while (--xsize);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
            LCD_L0_SetPixelIndex(x, y, PixelIndex);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
        } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            LCD_L0_SetPixelIndex(x, y, Index);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
        } while (--xsize);
      }
      break;
  }
}
#endif

/*********************************************************************
*
*       Draw Bitmap 4 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 4
*/
#if (LCD_MAX_LOG_COLORS > 4)
#if (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 4) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine4BPP(int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    int Off, NumPixel_0, NumPixel_1, i;
    U32 Data, ColorMask, AndMask;
    Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
    NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
    if (pTrans) {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
        if ((8 - NumPixel_0) > xsize) {
          AndMask |= ~(0xFFFFFFFF >> (4 * (7 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 8) && xsize; i++, xsize--) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Pixel = (*p & (0xF << Shift)) >> Shift;
          U8 Index = *(pTrans + Pixel);
          ColorMask |= Index << (4 * i);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 8) {
        ColorMask = 0;
        for (i = 0; i < 8; i++) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Pixel = (*p & (0xF << Shift)) >> Shift;
          U8 Index = *(pTrans + Pixel);
          ColorMask |= Index << (4 * i);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 8;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
        while (xsize) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Pixel = (*p & (0xF << Shift)) >> Shift;
          U8 Index = *(pTrans + Pixel);
          ColorMask |= Index << (4 * i++);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
        if ((8 - NumPixel_0) > xsize) {
          AndMask |= ~(0xFFFFFFFF >> (4 * (7 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 8) && xsize; i++, xsize--) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Index = (*p & (0xF << Shift)) >> Shift;
          ColorMask |= Index << (4 * i);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 8) {
        ColorMask = 0;
        for (i = 0; i < 8; i++) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Index = (*p & (0xF << Shift)) >> Shift;
          ColorMask |= Index << (4 * i);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
        }
        WRITE_MEM(Off, ColorMask);
        Off++;
        xsize -= 8;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
        while (xsize) {
          int Shift = 4 * (1 - CurrentPixel);
          U8 Index = (*p & (0xF << Shift)) >> Shift;
          ColorMask |= Index << (4 * i++);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            p++;
          }
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    if (pTrans) {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          LCD_L0_SetPixelIndex(x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          LCD_L0_SetPixelIndex(x, y, Index);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 4 BPP, not optimized
*/
#else
static void  _DrawBitLine4BPP(int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
        LCD_L0_SetPixelIndex(x++, y, PixelIndex);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        LCD_L0_SetPixelIndex(x++, y, Index);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          LCD_L0_SetPixelIndex(x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        int Shift = (1 - CurrentPixel) << 2;
        int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          LCD_L0_SetPixelIndex(x, y, Index);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  }
}
#endif
#endif /* (LCD_MAX_LOG_COLORS > 4) */

/*********************************************************************
*
*       Draw Bitmap 8 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8
*/
#if (LCD_MAX_LOG_COLORS > 16)
#if (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 8) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    U32 Data, ColorMask, AndMask;
    int Off, NumPixel_0, NumPixel_1, i;
    Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
    NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
    if (pTrans) {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
        if ((xsize < 3) && (NumPixel_1)) {
          AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 4) && xsize; i++, xsize--) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (8 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 4) {
        ColorMask =  *(pTrans + *p) | 
                    (*(pTrans + *(p + 1)) << 8) | 
                    (*(pTrans + *(p + 2)) << 16) | 
                    (*(pTrans + *(p + 3)) << 24);
        WRITE_MEM(Off, ColorMask);
        p += 4;
        Off++;
        xsize -= 4;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
        while (xsize) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (8 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
        if ((xsize < 3) && (NumPixel_1)) {
          AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
        }
        for (i = NumPixel_0; (i < 4) && xsize; i++, xsize--) {
          U8 Index = *(p++);
          ColorMask |= Index << (8 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 4) {
        ColorMask =  *(p) | 
                    (*(p + 1) << 8) | 
                    (*(p + 2) << 16) | 
                    (*(p + 3) << 24);
        WRITE_MEM(Off, ColorMask);
        p += 4;
        Off++;
        xsize -= 4;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
        while (xsize) {
          U8 Index = *(p++);
          ColorMask |= Index << (8 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    /* Handle transparent bitmap with palette */
    LCD_PIXELINDEX pixel;
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, *(pTrans + pixel));
        }
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, pixel);
        }
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8, LCD_SWAP_XY == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 8) && \
    (LCD_MIRROR_X) && \
    (LCD_SWAP_XY)
static void _DrawBitLine8BPP_Swap(unsigned x, unsigned y, U8 const GUI_UNI_PTR * pData, int ysize, const U8 * pTrans, int BytesPerLine, U8 Pos) {
  unsigned x0, y0, Off, xOdd;
  U32 Data;
  U8 Pixel, BufferValid, Shift, Index;
  #if LCD_MIRROR_Y
    y0 = LCD_XSIZE - x - 1;
  #else
    y0 = x;
  #endif
  x0 = LCD_YSIZE - y - 1;
  Off = XY2OFF(x0, y0);
  BufferValid = 0;
  xOdd = x0 & 0x3;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    /* Handle non transparent bitmaps */
    if (pTrans) {
      /* First DWORD */
      if (xOdd < 3) {
        Data = READ_MEM(Off);
        do {
          Shift = xOdd << 3;
          Pixel = *(pTrans + *pData);
          pData += BytesPerLine;
          Data &= ~(0xff << Shift);
          Data |= Pixel << Shift;
          if (!--ysize) {
            break;
          }
          x0--;
        } while (xOdd--);
        WRITE_MEM(Off, Data);
        Off--;
      }
      /* Complete DWORDS */
      while (ysize >= 4) {
        Data  = (*(pTrans + *pData)) << 24;
        pData += BytesPerLine;
        Data |= (*(pTrans + *pData)) << 16;
        pData += BytesPerLine;
        Data |= (*(pTrans + *pData)) << 8;
        pData += BytesPerLine;
        Data |= *(pTrans + *pData);
        pData += BytesPerLine;
        WRITE_MEM(Off, Data);
        ysize -= 4;
        x0 -= 4;
        Off--;
      }
      /* Last DWORD */
      if (ysize) {
        Data = READ_MEM(Off);
        Data &= ~0xff000000;
        Data |= (*(pTrans + *pData)) << 24;
        if (ysize > 1) {
          pData += BytesPerLine;
          Data &= ~0xff0000;
          Data |= (*(pTrans + *pData)) << 16;
          if (ysize > 2) {
            pData += BytesPerLine;
            Data &= ~0xff00;
            Data |= (*(pTrans + *pData)) << 8;
          }
        }
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (xOdd < 3) {
        Data = READ_MEM(Off);
        do {
          Shift = xOdd << 3;
          Pixel = *pData;
          pData += BytesPerLine;
          Data &= ~(0xff << Shift);
          Data |= Pixel << Shift;
          if (!--ysize) {
            break;
          }
          x0--;
        } while (xOdd--);
        WRITE_MEM(Off, Data);
        Off--;
      }
      /* Complete DWORDS */
      while (ysize >= 4) {
        Data  = (*pData) << 24;
        pData += BytesPerLine;
        Data |= (*pData) << 16;
        pData += BytesPerLine;
        Data |= (*pData) << 8;
        pData += BytesPerLine;
        Data |= *pData;
        pData += BytesPerLine;
        WRITE_MEM(Off, Data);
        ysize -= 4;
        x0 -= 4;
        Off--;
      }
      /* Last DWORD */
      if (ysize) {
        Data = READ_MEM(Off);
        Data &= ~0xff000000;
        Data |= (*pData) << 24;
        if (ysize > 1) {
          pData += BytesPerLine;
          Data &= ~0xff0000;
          Data |= (*pData) << 16;
          if (ysize > 2) {
            pData += BytesPerLine;
            Data &= ~0xff00;
            Data |= (*pData) << 8;
          }
        }
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    /* Handle transparent bitmaps */
    if (pTrans) {
      /* First DWORD */
      if (xOdd < 3) {
        Data = READ_MEM(Off);
        do {
          Shift = xOdd << 3;
          Index = *pData;
          pData += BytesPerLine;
          if (Index) {
            Pixel = *(pTrans + Index);
            Data &= ~(0xff << Shift);
            Data |= Pixel << Shift;
          }
          if (!--ysize) {
            break;
          }
          x0--;
        } while (xOdd--);
        WRITE_MEM(Off, Data);
        Off--;
      }
      /* Complete DWORDS */
      while (ysize >= 4) {
        Data = READ_MEM(Off);
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 24);
          Data |= (*(pTrans + Index)) << 24;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 16);
          Data |= (*(pTrans + Index)) << 16;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 8);
          Data |= (*(pTrans + Index)) << 8;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff);
          Data |= (*(pTrans + Index));
        }
        WRITE_MEM(Off, Data);
        ysize -= 4;
        x0 -= 4;
        Off--;
      }
      /* Last DWORD */
      if (ysize) {
        Data = READ_MEM(Off);
        Index = *pData;
        if (Index) {
          Data &= ~0xff000000;
          Data |= (*(pTrans + Index)) << 24;
        }
        if (ysize > 1) {
          pData += BytesPerLine;
          Index = *pData;
          if (Index) {
            Data &= ~0xff0000;
            Data |= (*(pTrans + Index)) << 16;
          }
          if (ysize > 2) {
            pData += BytesPerLine;
            Index = *pData;
            if (Index) {
              Data &= ~0xff00;
              Data |= (*(pTrans + Index)) << 8;
            }
          }
        }
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (xOdd < 3) {
        Data = READ_MEM(Off);
        do {
          Shift = xOdd << 3;
          Index = *pData;
          pData += BytesPerLine;
          if (Index) {
            Data &= ~(0xff << Shift);
            Data |= Index << Shift;
          }
          if (!--ysize) {
            break;
          }
          x0--;
        } while (xOdd--);
        WRITE_MEM(Off, Data);
        Off--;
      }
      /* Complete DWORDS */
      while (ysize >= 4) {
        Data = READ_MEM(Off);
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 24);
          Data |= Index << 24;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 16);
          Data |= Index << 16;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff << 8);
          Data |= Index << 8;
        }
        Index = *pData;
        pData += BytesPerLine;
        if (Index) {
          Data &= ~(0xff);
          Data |= Index;
        }
        WRITE_MEM(Off, Data);
        ysize -= 4;
        x0 -= 4;
        Off--;
      }
      /* Last DWORD */
      if (ysize) {
        Data = READ_MEM(Off);
        Index = *pData;
        if (Index) {
          Data &= ~0xff000000;
          Data |= Index << 24;
        }
        if (ysize > 1) {
          pData += BytesPerLine;
          Index = *pData;
          if (Index) {
            Data &= ~0xff0000;
            Data |= Index << 16;
          }
          if (ysize > 2) {
            pData += BytesPerLine;
            Index = *pData;
            if (Index) {
              Data &= ~0xff00;
              Data |= Index << 8;
            }
          }
        }
        WRITE_MEM(Off, Data);
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 4
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 4) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    U32 Data, ColorMask, AndMask;
    int Off, NumPixel_0, NumPixel_1, i;
    Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
    NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
    if (pTrans) {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
        for (i = NumPixel_0; (i < 8) && xsize; i++, xsize--) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (4 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 8) {
        ColorMask =  *(pTrans + *p) | 
                    (*(pTrans + *(p + 1)) << 4) | 
                    (*(pTrans + *(p + 2)) << 8) | 
                    (*(pTrans + *(p + 3)) << 12) | 
                    (*(pTrans + *(p + 4)) << 16) | 
                    (*(pTrans + *(p + 5)) << 20) | 
                    (*(pTrans + *(p + 6)) << 24) | 
                    (*(pTrans + *(p + 7)) << 28);
        WRITE_MEM(Off, ColorMask);
        p += 8;
        Off++;
        xsize -= 8;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
        while (xsize) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (4 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
        for (i = NumPixel_0; (i < 8) && xsize; i++, xsize--) {
          U8 Index = *(p++);
          ColorMask |= Index << (4 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 8) {
        ColorMask =  *(p) | 
                    (*(p + 1) << 4) | 
                    (*(p + 2) << 8) | 
                    (*(p + 3) << 12) | 
                    (*(p + 4) << 16) | 
                    (*(p + 5) << 20) | 
                    (*(p + 6) << 24) | 
                    (*(p + 7) << 28);
        WRITE_MEM(Off, ColorMask);
        p += 8;
        Off++;
        xsize -= 8;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
        while (xsize) {
          U8 Index = *(p++);
          ColorMask |= Index << (4 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    /* Handle transparent bitmap with palette */
    LCD_PIXELINDEX pixel;
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, *(pTrans + pixel));
        }
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, pixel);
        }
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 2
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 2) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    U32 Data, ColorMask, AndMask;
    int Off, NumPixel_0, NumPixel_1, i;
    Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
    NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
    if (pTrans) {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (2 * NumPixel_0));
        for (i = NumPixel_0; (i < 16) && xsize; i++, xsize--) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (2 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 16) {
        ColorMask =  *(pTrans + *p) | 
                    (*(pTrans + *(p +  1)) <<  2) | 
                    (*(pTrans + *(p +  2)) <<  4) | 
                    (*(pTrans + *(p +  3)) <<  6) | 
                    (*(pTrans + *(p +  4)) <<  8) | 
                    (*(pTrans + *(p +  5)) << 10) | 
                    (*(pTrans + *(p +  6)) << 12) | 
                    (*(pTrans + *(p +  7)) << 14) | 
                    (*(pTrans + *(p +  8)) << 16) | 
                    (*(pTrans + *(p +  9)) << 18) | 
                    (*(pTrans + *(p + 10)) << 20) | 
                    (*(pTrans + *(p + 11)) << 22) | 
                    (*(pTrans + *(p + 12)) << 24) | 
                    (*(pTrans + *(p + 13)) << 26) | 
                    (*(pTrans + *(p + 14)) << 28) | 
                    (*(pTrans + *(p + 15)) << 30);
        WRITE_MEM(Off, ColorMask);
        p += 16;
        Off++;
        xsize -= 16;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFC << (2 * NumPixel_1);
        while (xsize) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << (2 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << (2 * NumPixel_0));
        for (i = NumPixel_0; (i < 16) && xsize; i++, xsize--) {
          U8 Index = *(p++);
          ColorMask |= Index << (2 * i);
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 16) {
        ColorMask =  *(p) | 
                    (*(p +  1) <<  2) | 
                    (*(p +  2) <<  4) | 
                    (*(p +  3) <<  6) | 
                    (*(p +  4) <<  8) | 
                    (*(p +  5) << 10) | 
                    (*(p +  6) << 12) | 
                    (*(p +  7) << 14) | 
                    (*(p +  8) << 16) | 
                    (*(p +  9) << 18) | 
                    (*(p + 10) << 20) | 
                    (*(p + 11) << 22) | 
                    (*(p + 12) << 24) | 
                    (*(p + 13) << 26) | 
                    (*(p + 14) << 28) | 
                    (*(p + 15) << 30);
        WRITE_MEM(Off, ColorMask);
        p += 16;
        Off++;
        xsize -= 16;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFC << (2 * NumPixel_1);
        while (xsize) {
          U8 Index = *(p++);
          ColorMask |= Index << (2 * i++);
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    /* Handle transparent bitmap with palette */
    LCD_PIXELINDEX pixel;
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, *(pTrans + pixel));
        }
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, pixel);
        }
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 1) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    U32 Data, ColorMask, AndMask;
    int Off, NumPixel_0, NumPixel_1, i;
    Off = y * DWORDS_PER_LINE + x / PIXELS_PER_DWORD;
    NumPixel_0 = x & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = (x + xsize - 1) & (PIXELS_PER_DWORD - 1);
    if (pTrans) {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << NumPixel_0);
        for (i = NumPixel_0; (i < 32) && xsize; i++, xsize--) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << i;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 32) {
        ColorMask =  *(pTrans + *p) | 
                    (*(pTrans + *(p +  1)) <<  1) | 
                    (*(pTrans + *(p +  2)) <<  2) | 
                    (*(pTrans + *(p +  3)) <<  3) | 
                    (*(pTrans + *(p +  4)) <<  4) | 
                    (*(pTrans + *(p +  5)) <<  5) | 
                    (*(pTrans + *(p +  6)) <<  6) | 
                    (*(pTrans + *(p +  7)) <<  7) | 
                    (*(pTrans + *(p +  8)) <<  8) | 
                    (*(pTrans + *(p +  9)) <<  9) | 
                    (*(pTrans + *(p + 10)) << 10) | 
                    (*(pTrans + *(p + 11)) << 11) | 
                    (*(pTrans + *(p + 12)) << 12) | 
                    (*(pTrans + *(p + 13)) << 13) | 
                    (*(pTrans + *(p + 14)) << 14) | 
                    (*(pTrans + *(p + 15)) << 15) | 
                    (*(pTrans + *(p + 16)) << 16) | 
                    (*(pTrans + *(p + 17)) << 17) | 
                    (*(pTrans + *(p + 18)) << 18) | 
                    (*(pTrans + *(p + 19)) << 19) | 
                    (*(pTrans + *(p + 20)) << 20) | 
                    (*(pTrans + *(p + 21)) << 21) | 
                    (*(pTrans + *(p + 22)) << 22) | 
                    (*(pTrans + *(p + 23)) << 23) | 
                    (*(pTrans + *(p + 24)) << 24) | 
                    (*(pTrans + *(p + 25)) << 25) | 
                    (*(pTrans + *(p + 26)) << 26) | 
                    (*(pTrans + *(p + 27)) << 27) | 
                    (*(pTrans + *(p + 28)) << 28) | 
                    (*(pTrans + *(p + 29)) << 29) | 
                    (*(pTrans + *(p + 30)) << 30) | 
                    (*(pTrans + *(p + 31)) << 31);
        WRITE_MEM(Off, ColorMask);
        p += 32;
        Off++;
        xsize -= 32;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFE << NumPixel_1;
        while (xsize) {
          U8 Index = *(pTrans + *p++);
          ColorMask |= Index << i++;
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    } else {
      /* First DWORD */
      if (NumPixel_0) {
        ColorMask = 0;
        AndMask = ~(0xFFFFFFFF << NumPixel_0);
        for (i = NumPixel_0; (i < 32) && xsize; i++, xsize--) {
          U8 Index = *(p++);
          ColorMask |= Index << i;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
        Off++;
      }
      /* Complete DWORDS */
      while (xsize >= 32) {
        ColorMask =  *(p) | 
                    (*(p +  1) <<  1) | 
                    (*(p +  2) <<  2) | 
                    (*(p +  3) <<  3) | 
                    (*(p +  4) <<  4) | 
                    (*(p +  5) <<  5) | 
                    (*(p +  6) <<  6) | 
                    (*(p +  7) <<  7) | 
                    (*(p +  8) <<  8) | 
                    (*(p +  9) <<  9) | 
                    (*(p + 10) << 10) | 
                    (*(p + 11) << 11) | 
                    (*(p + 12) << 12) | 
                    (*(p + 13) << 13) | 
                    (*(p + 14) << 14) | 
                    (*(p + 15) << 15) | 
                    (*(p + 16) << 16) | 
                    (*(p + 17) << 17) | 
                    (*(p + 18) << 18) | 
                    (*(p + 19) << 19) | 
                    (*(p + 20) << 20) | 
                    (*(p + 21) << 21) | 
                    (*(p + 22) << 22) | 
                    (*(p + 23) << 23) | 
                    (*(p + 24) << 24) | 
                    (*(p + 25) << 25) | 
                    (*(p + 26) << 26) | 
                    (*(p + 27) << 27) | 
                    (*(p + 28) << 28) | 
                    (*(p + 29) << 29) | 
                    (*(p + 30) << 30) | 
                    (*(p + 31) << 31);
        WRITE_MEM(Off, ColorMask);
        p += 32;
        Off++;
        xsize -= 32;
      }
      /* Last DWORD */
      if (xsize) {
        ColorMask = i = 0;
        AndMask = 0xFFFFFFFE << NumPixel_1;
        while (xsize) {
          U8 Index = *(p++);
          ColorMask |= Index << i++;
          xsize--;
        }
        Data = READ_MEM(Off);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    /* Handle transparent bitmap with palette */
    LCD_PIXELINDEX pixel;
    if (pTrans) {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, *(pTrans + pixel));
        }
        xsize--;
        x++;
        p++;
      }
    /* Handle transparent bitmap without palette */
    } else {
      while (xsize > 0) {
        pixel = *p;
        if (pixel != 0) {
          LCD_L0_SetPixelIndex(x + 0, y, pixel);
        }
        xsize--;
        x++;
        p++;
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP, not optimized
*/
#else

#if (LCD_BITSPERPIXEL == 8)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  _DrawBitLine8BPP(int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        LCD_L0_SetPixelIndex(x, y, *p);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          LCD_L0_SetPixelIndex(x, y, Pixel);
        }
      }
    }
    break;
  }
}
#endif
#endif /* (LCD_MAX_LOG_COLORS > 16) */

/*********************************************************************
*
*       Draw Bitmap 16 BPP, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 16
*/
#if (LCD_BITSPERPIXEL > 8)
#if (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 16) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
static void  _DrawBitLine16BPP(unsigned x, unsigned y, U16 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
      }
    } else {
      int Off = y * DWORDS_PER_LINE + (x >> 1);
      U32 Data;

      /* First DWORD */
      if (x & 1) {
        Data = READ_MEM(Off);
        Data &= 0xFFFF;
        Data |= *p++ << 16;
        WRITE_MEM(Off++, Data);
        xsize--;
      }
      /* Complete DWORDS */
      if (xsize >= 8) {
        do {
          Data = *(p)   | (*(p+1) << 16);
          WRITE_MEM(Off, Data);
          Data = *(p+2) | (*(p+3) << 16);
          WRITE_MEM(Off+1, Data);
          Data = *(p+4) | (*(p+5) << 16);
          WRITE_MEM(Off+2, Data);
          Data = *(p+6) | (*(p+7) << 16);
          WRITE_MEM(Off+3, Data);
          p += 8;
          Off += 4;
        } while ((xsize -= 8) >= 8);
      }        
      if (xsize >= 2) {
        do {
          Data = *p | (*(p+1) << 16);
          WRITE_MEM(Off++, Data);
          p += 2;
        } while ((xsize -= 2) >= 2);
      }        
      /* Last DWORD */
      if (xsize > 0) {
        Data = READ_MEM(Off);
        Data &= 0xFFFF0000;
        Data |= *p;
        WRITE_MEM(Off, Data);
      }
    }
  } else {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, pixel);
        }
      }
    }
  }
}

/*********************************************************************
*
*       Draw Bitmap 16 BPP, not optimized
*/
#else

#if (LCD_BITSPERPIXEL == 16)
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

static void  _DrawBitLine16BPP(unsigned x, unsigned y, U16 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
      }
    } else {
      for (;xsize > 0; xsize--, x++, p++) {
        LCD_L0_SetPixelIndex(x, y, *p);
      }
    }
  } else {
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          LCD_L0_SetPixelIndex(x, y, pixel);
        }
      }
    }
  }
}
#endif
#endif /* (LCD_BITSPERPIXEL > 8) */

/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*
* Purpose:
*   Sets the index of the given pixel. The upper layers
*   calling this routine make sure that the coordinates are in range, so
*   that no check on the parameters needs to be performed.
*/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Write into hardware ... Adapt to your system */
  {
    unsigned Off      = XY2OFF(xPhys, yPhys);
    U32 Data;
    unsigned PixelPos = xPhys & (PIXELS_PER_DWORD - 1);
    Data = READ_MEM(Off);
    #if   (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 1)
      Data &= ~(0x1        << (    PixelPos));
      Data |= PixelIndex   << (    PixelPos);
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 1)
      Data &= ~(0x80000000 >> (    PixelPos));
      Data |= PixelIndex   << (    (31 - PixelPos));
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 2)
      Data &= ~(0x3        << (2 * PixelPos));
      Data |= PixelIndex   << (2 * PixelPos);
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 2)
      Data &= ~(0xC0000000 >> (2 * PixelPos));
      Data |= PixelIndex   << (2 * (15 - PixelPos));
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 4)
      Data &= ~(0xF        << (4 * PixelPos));
      Data |= PixelIndex   << (4 * PixelPos);
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 4)
      Data &= ~(0xF0000000 >> (4 * PixelPos));
      Data |= PixelIndex   << (4 * (7 - PixelPos));
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 8)
      Data &= ~(0xFF       << (8 * PixelPos));
      Data |= PixelIndex   << (8 * PixelPos);
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 8)
      Data &= ~(0xFF000000 >> (8 * PixelPos));
      Data |= PixelIndex   << (8 * (3 - PixelPos));
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 16)
      Data &= ~(0xFFFF     << (16 * PixelPos));
      Data |= PixelIndex   << (16 * PixelPos);
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 16)
      Data &= ~(0xFFFF0000 >> (16 * PixelPos));
      Data |= PixelIndex   << (16 * (1 - PixelPos));
    #else
      #error Not supported yet!
    #endif
    WRITE_MEM(Off, Data);
  }
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
*
* Purpose:
*   Returns the index of the given pixel. The upper layers
*   calling this routine make sure that the coordinates are in range, so
*   that no check on the parameters needs to be performed.
*/
unsigned int LCD_L0_GetPixelIndex(int x, int y) {
  LCD_PIXELINDEX PixelIndex;
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Read from hardware ... Adapt to your system */
  {
    int Off = yPhys * DWORDS_PER_LINE + xPhys / PIXELS_PER_DWORD;
    U32 Data;
    int PixelPos = xPhys & (PIXELS_PER_DWORD - 1);
    Data = READ_MEM(Off);
    #if   (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 1)
      PixelIndex = (U8)            (Data >> (    PixelPos)) & 0x1;
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 1)
      PixelIndex = (U8)            (Data >> (    (31 - PixelPos))) & 0x1;
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 2)
      PixelIndex = (U8)            (Data >> (2 * PixelPos)) & 0x3;
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 2)
      PixelIndex = (U8)            (Data >> (2 * (15 - PixelPos))) & 0x3;
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 4)
      PixelIndex = (U8)            (Data >> (4 * PixelPos)) & 0xF;
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 4)
      PixelIndex = (U8)            (Data >> (4 * (7 - PixelPos))) & 0xF;
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 8)
      PixelIndex = (U8)            (Data >> (8 * PixelPos));
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 8)
      PixelIndex = (U8)            (Data >> (8 * (3 - PixelPos)));
    #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 16)
      PixelIndex = (LCD_PIXELINDEX)(Data >> (16 * PixelPos));
    #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 16)
      PixelIndex = (LCD_PIXELINDEX)(Data >> (16 * (1 - PixelPos)));
    #else
      #error Not supported yet!
    #endif
  }
  return PixelIndex;
}

/*********************************************************************
*
*       LCD_L0_XorPixel
*/
void LCD_L0_XorPixel(int x, int y) {
  LCD_PIXELINDEX PixelIndex = LCD_L0_GetPixelIndex(x, y);
  LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - PixelIndex - 1);
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 16
*/
#if (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 16) && \
    (!LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (!LCD_SWAP_XY)
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    int Off, RemPixels;
    U32 Data, ColorMask;
    Off = y * DWORDS_PER_LINE + x0 / PIXELS_PER_DWORD;
    RemPixels = x1 - x0 + 1;

    /* First DWORD */
    if (x0 & (PIXELS_PER_DWORD - 1)) {
      Data = READ_MEM(Off);
      Data &= 0xFFFF;
      Data |= (LCD_COLORINDEX << 16);
      WRITE_MEM(Off++, Data);
      RemPixels--;
    }
    /* Complete DWORDS */
    ColorMask = LCD_COLORINDEX * 0x00010001;
    if (RemPixels >= 16) {
      do {
        LCD_WRITE_MEM(Off,    ColorMask);
        LCD_WRITE_MEM(Off +1, ColorMask);
        LCD_WRITE_MEM(Off +2, ColorMask);
        LCD_WRITE_MEM(Off +3, ColorMask);
        LCD_WRITE_MEM(Off +4, ColorMask);
        LCD_WRITE_MEM(Off +5, ColorMask);
        LCD_WRITE_MEM(Off +6, ColorMask);
        LCD_WRITE_MEM(Off +7, ColorMask);
        Off += 8;
      } while ((RemPixels -= 16) >= 16);
    }
    if (RemPixels >= 2) {
      do {
        LCD_WRITE_MEM(Off++, ColorMask);
      } while ((RemPixels -= 2) >= 2);
    }        
    /* Last DWORD */
    if (RemPixels > 0) {
      Data = READ_MEM(Off);
      Data &= 0xFFFF0000;
      Data |= LCD_COLORINDEX & 0xFFFF;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8
*/
#elif (LCD_ENDIAN_BIG == 0) && \
      (LCD_BITSPERPIXEL == 8) && \
      (!LCD_MIRROR_X) && \
      (!LCD_MIRROR_Y) && \
      (!LCD_SWAP_XY)
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    int Off, NumPixel_0, NumPixel_1, RemPixels;
    U32 Data, ColorMask, AndMask;
    Off = y * DWORDS_PER_LINE + x0 / PIXELS_PER_DWORD;
    NumPixel_0 = x0 & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = x1 & (PIXELS_PER_DWORD - 1);
    RemPixels = x1 - x0 + 1;
    /* First DWORD */
    if (NumPixel_0) {
      AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
      if ((RemPixels < 3) && (NumPixel_1)) {
        AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
      }
      ColorMask = (LCD_COLORINDEX * 0x01010101) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
      RemPixels -= (4 - NumPixel_0);
      Off++;
    }
    /* Complete DWORDS */
    ColorMask = LCD_COLORINDEX * 0x01010101;
    while (RemPixels >= 4) {
      LCD_WRITE_MEM(Off, ColorMask);
      Off++;
      RemPixels -= 4;
    }
    /* Last DWORD */
    if (RemPixels > 0) {
      AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
      ColorMask = (LCD_COLORINDEX * 0x01010101) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 4
*/
#elif (LCD_ENDIAN_BIG == 0) && \
      (LCD_BITSPERPIXEL == 4) && \
      (!LCD_MIRROR_X) && \
      (!LCD_MIRROR_Y) && \
      (!LCD_SWAP_XY)
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    int Off, NumPixel_0, NumPixel_1, RemPixels;
    U32 Data, ColorMask, AndMask;
    Off = y * DWORDS_PER_LINE + x0 / PIXELS_PER_DWORD;
    NumPixel_0 = x0 & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = x1 & (PIXELS_PER_DWORD - 1);
    RemPixels = x1 - x0 + 1;
    /* First DWORD */
    if (NumPixel_0) {
      AndMask = ~(0xFFFFFFFF << (4 * NumPixel_0));
      if ((x0 & ~0x7) == (x1 & ~0x7)) {
        AndMask |= ~(0xFFFFFFFF >> (4 * (7 - NumPixel_1)));
      }
      ColorMask = (LCD_COLORINDEX * 0x11111111) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
      RemPixels -= (8 - NumPixel_0);
      Off++;
    }
    /* Complete DWORDS */
    ColorMask = LCD_COLORINDEX * 0x11111111;
    while (RemPixels >= 8) {
      LCD_WRITE_MEM(Off, ColorMask);
      Off++;
      RemPixels -= 8;
    }
    /* Last DWORD */
    if (RemPixels > 0) {
      AndMask = 0xFFFFFFF0 << (4 * NumPixel_1);
      ColorMask = (LCD_COLORINDEX * 0x11111111) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 2
*/
#elif (LCD_ENDIAN_BIG == 0) && \
      (LCD_BITSPERPIXEL == 2) && \
      (!LCD_MIRROR_X) && \
      (!LCD_MIRROR_Y) && \
      (!LCD_SWAP_XY)
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    int Off, NumPixel_0, NumPixel_1, RemPixels;
    U32 Data, ColorMask, AndMask;
    Off = y * DWORDS_PER_LINE + x0 / PIXELS_PER_DWORD;
    NumPixel_0 = x0 & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = x1 & (PIXELS_PER_DWORD - 1);
    RemPixels = x1 - x0 + 1;
    /* First DWORD */
    if (NumPixel_0) {
      AndMask = ~(0xFFFFFFFF << (2 * NumPixel_0));
      if ((x0 & ~0xf) == (x1 & ~0xf)) {
        AndMask |= ~(0xFFFFFFFF >> (2 * (15 - NumPixel_1)));
      }
      ColorMask = (LCD_COLORINDEX * 0x55555555) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
      RemPixels -= (16 - NumPixel_0);
      Off++;
    }
    /* Complete DWORDS */
    ColorMask = LCD_COLORINDEX * 0x55555555;
    while (RemPixels >= 16) {
      LCD_WRITE_MEM(Off, ColorMask);
      Off++;
      RemPixels -= 16;
    }
    /* Last DWORD */
    if (RemPixels > 0) {
      AndMask = 0xFFFFFFFC << (2 * NumPixel_1);
      ColorMask = (LCD_COLORINDEX * 0x55555555) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
      (LCD_BITSPERPIXEL == 1) && \
      (!LCD_MIRROR_X) && \
      (!LCD_MIRROR_Y) && \
      (!LCD_SWAP_XY)
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    int Off, NumPixel_0, NumPixel_1, RemPixels;
    U32 Data, ColorMask, AndMask;
    Off = y * DWORDS_PER_LINE + x0 / PIXELS_PER_DWORD;
    NumPixel_0 = x0 & (PIXELS_PER_DWORD - 1);
    NumPixel_1 = x1 & (PIXELS_PER_DWORD - 1);
    RemPixels = x1 - x0 + 1;
    /* First DWORD */
    if (NumPixel_0) {
      AndMask = ~(0xFFFFFFFF << NumPixel_0);
      if ((x0 & ~0x1f) == (x1 & ~0x1f)) {
        AndMask |= ~(0xFFFFFFFF >> (31 - NumPixel_1));
      }
      ColorMask = (LCD_COLORINDEX * 0xFFFFFFFF) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
      RemPixels -= (32 - NumPixel_0);
      Off++;
    }
    /* Complete DWORDS */
    ColorMask = LCD_COLORINDEX * 0xFFFFFFFF;
    while (RemPixels >= 32) {
      LCD_WRITE_MEM(Off, ColorMask);
      Off++;
      RemPixels -= 32;
    }
    /* Last DWORD */
    if (RemPixels > 0) {
      AndMask = 0xFFFFFFFE <<  NumPixel_1;
      ColorMask = (LCD_COLORINDEX * 0xFFFFFFFF) & ~AndMask;
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= ColorMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine, not optimized
*/
#else

#if !LCD_SWAP_XY
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    for (; x0 <= x1; x0++) {
      LCD_L0_SetPixelIndex(x0, y, LCD_COLORINDEX);
    }
  }
}
#endif

/*********************************************************************
*
*       LCD_L0_DrawVLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 8, LCD_MIRROR_X == 1, LCD_SWAP_XY == 1
*/
#if (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 8) && \
    (LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (LCD_SWAP_XY)
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      LCD_L0_XorPixel(x, y0);
      y0++;
    }
  } else {
    int x0_P, x1_P, y_P, Shift, RemPixels;
    unsigned Off;
    U32 Data, Color, OrMask = 0, AndMask = 0;
    x0_P  = (LCD_YSIZE - 1 - y1);
    x1_P  = (LCD_YSIZE - 1 - y0);
    y_P   = x;
    Off   = XY2OFF(x1_P, y_P);
    Color = LCD_COLORINDEX * 0x1010101;
    /* First pixels */
    RemPixels = x1_P - x0_P + 1;
    Shift = (3 - (x1_P & 0x3)) * 8;
    if (Shift) {
      AndMask = ~(0xffffffff >> Shift);
      OrMask  = Color >> Shift;
      Data = READ_MEM(Off);
      if (XY2OFF(x0_P, y_P) == Off) {
        /* Only one word is affected */
        Shift    = (x0_P & 0x3) * 8;
        OrMask  &= 0xffffffff << Shift;
        AndMask |= ~(0xffffffff << Shift);
        Data &= AndMask;
        Data |= OrMask;
        WRITE_MEM(Off, Data);
        return;
      }
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
      RemPixels -= 4 - (Shift >> 3);
      Off--;
    }
    /* Complete words */
    while (RemPixels >= 4) {
      LCD_WRITE_MEM(Off, Color);
      RemPixels -= 4;
      Off--;
    }
    /* Last pixels */
    if (RemPixels) {
      Shift   = (x0_P & 0x3) * 8;
      OrMask  = Color << Shift;
      AndMask = ~(0xffffffff << Shift);
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawVLine, optimized for LCD_ENDIAN_BIG == 0, LCD_BITSPERPIXEL == 2, LCD_MIRROR_X == 1, LCD_SWAP_XY == 1
*/
#elif (LCD_ENDIAN_BIG == 0) && \
    (LCD_BITSPERPIXEL == 2) && \
    (LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (LCD_SWAP_XY)
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      LCD_L0_XorPixel(x, y0);
      y0++;
    }
  } else {
    int x0_P, x1_P, y_P, Shift, RemPixels;
    unsigned Off;
    U32 Data, Color, OrMask = 0, AndMask = 0;
    x0_P  = (LCD_YSIZE - 1 - y1);
    x1_P  = (LCD_YSIZE - 1 - y0);
    y_P   = x;
    Off   = XY2OFF(x1_P, y_P);
    Color = LCD_COLORINDEX * 0x55555555;
    /* First pixels */
    RemPixels = x1_P - x0_P + 1;
    Shift = (15 - (x1_P & 0xf)) * 2;
    if (Shift) {
      AndMask = ~(0xffffffff >> Shift);
      OrMask  = Color >> Shift;
      Data = READ_MEM(Off);
      if (XY2OFF(x0_P, y_P) == Off) {
        /* Only one word is affected */
        Shift    = (x0_P & 0xf) * 2;
        OrMask  &= 0xffffffff << Shift;
        AndMask |= ~(0xffffffff << Shift);
        Data &= AndMask;
        Data |= OrMask;
        WRITE_MEM(Off, Data);
        return;
      }
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
      RemPixels -= 16 - (Shift >> 1);
      Off--;
    }
    /* Complete words */
    while (RemPixels >= 16) {
      LCD_WRITE_MEM(Off, Color);
      RemPixels -= 16;
      Off--;
    }
    /* Last pixels */
    if (RemPixels) {
      Shift   = (x0_P & 0xf) * 2;
      OrMask  = Color << Shift;
      AndMask = ~(0xffffffff << Shift);
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawVLine, optimized for LCD_ENDIAN_BIG == 1, LCD_BITSPERPIXEL == 2, LCD_MIRROR_X == 1, LCD_SWAP_XY == 1
*/
#elif (LCD_ENDIAN_BIG == 1) && \
    (LCD_BITSPERPIXEL == 2) && \
    (LCD_MIRROR_X) && \
    (!LCD_MIRROR_Y) && \
    (LCD_SWAP_XY)
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      LCD_L0_XorPixel(x, y0);
      y0++;
    }
  } else {
    int x0_P, x1_P, y_P, Shift, RemPixels;
    unsigned Off;
    U32 Data, Color, OrMask, AndMask;
    x0_P  = (LCD_YSIZE - 1 - y1);
    x1_P  = (LCD_YSIZE - 1 - y0);
    y_P   = x;
    Off   = XY2OFF(x1_P, y_P);
    Color = LCD_COLORINDEX * 0x55555555;
    /* First pixels */
    RemPixels = x1_P - x0_P + 1;
    Shift = (15 - (x1_P & 0xf)) * 2;
    if (Shift) {
      AndMask = ~(0xffffffff << Shift);
      OrMask  = Color << Shift;
      Data = READ_MEM(Off);
      if (XY2OFF(x0_P, y_P) == Off) {
        /* Only one word is affected */
        Shift = (x0_P & 0xf) * 2;
        OrMask &= 0xffffffff >> Shift;
        AndMask |= ~(0xffffffff >> Shift);
        Data &= AndMask;
        Data |= OrMask;
        WRITE_MEM(Off, Data);
        return;
      }
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
      RemPixels -= 16 - (Shift >> 1);
      Off--;
    }
    /* Complete words */
    while (RemPixels >= 16) {
      LCD_WRITE_MEM(Off, Color);
      RemPixels -= 16;
      Off--;
    }
    /* Last pixels */
    if (RemPixels) {
      Shift = (x0_P & 0xf) * 2;
      OrMask = Color >> Shift;
      AndMask = ~(0xffffffff >> Shift);
      Data = READ_MEM(Off);
      Data &= AndMask;
      Data |= OrMask;
      WRITE_MEM(Off, Data);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawVLine, not optimized
*/
#else

#if LCD_SWAP_XY
  #if LCD_ALLOW_NON_OPTIMIZED_MODE == 0
    #error Non-optimized mode. Please select an other mode, define LCD_ALLOW_NON_OPTIMIZED_MODE in LCDCOnf.h or contact info@micrium.com.
  #endif
#endif

void LCD_L0_DrawVLine(int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; y0 <= y1; y0++) {
      LCD_L0_XorPixel(x, y0);
    }
  } else {
    #if (!LCD_MIRROR_X) && (!LCD_MIRROR_Y) && (!LCD_SWAP_XY)
      U32 Mask, Color, Data;
      unsigned Off, PixelPos, NumPixels;
      Off       = XY2OFF(x, y0);
      PixelPos = (unsigned)x & (PIXELS_PER_DWORD - 1);
      /* Calculate mask, colorindex and offset */
      #if   (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 1)
        Mask  = ~(0x1          << (    PixelPos));
        Color = LCD_COLORINDEX << (    PixelPos);
      #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 1)
        Mask  = ~(0x80000000   >> (    PixelPos));
        Color = LCD_COLORINDEX << (    (31 - PixelPos));
      #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 2)
        Mask  = ~(0x3          << (2 * PixelPos));
        Color = LCD_COLORINDEX << (2 * PixelPos);
      #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 2)
        Mask  = ~(0xC0000000   >> (2 * PixelPos));
        Color = LCD_COLORINDEX << (2 * (15 - PixelPos));
      #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 4)
        Mask  = ~(0xF << (4 * PixelPos));
        Color = LCD_COLORINDEX << (4 * PixelPos);
      #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 4)
        Mask  = ~(0xF0000000   >> (4 * PixelPos));
        Color = LCD_COLORINDEX << (4 * (7 - PixelPos));
      #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 8)
        Mask  = ~(0xFF << (8 * PixelPos));
        Color = LCD_COLORINDEX << (8 * PixelPos);
      #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 8)
        Mask  = ~(0xFF000000   >> (8 * PixelPos));
        Color = LCD_COLORINDEX << (8 * (3 - PixelPos));
      #elif (LCD_ENDIAN_BIG == 0) && (LCD_BITSPERPIXEL == 16)
        Mask  = ~(0xFFFF       << (16 * PixelPos));
        Color = LCD_COLORINDEX << (16 * PixelPos);
      #elif (LCD_ENDIAN_BIG == 1) && (LCD_BITSPERPIXEL == 16)
        Mask  = ~(0xFFFF0000   >> (16 * PixelPos));
        Color = LCD_COLORINDEX << (16 * (1 - PixelPos));
      #else
        #error Not supported yet!
      #endif
      NumPixels = y1 - y0 + 1;           /* Really the number of pixels from now on */
      do {
        Data  = READ_MEM(Off);
        Data &= Mask;
        Data |= Color;
        WRITE_MEM(Off, Data);
        Off += DWORDS_PER_LINE;
      } while (--NumPixels);
    #else
      for (; y0 <= y1; y0++) {
        LCD_L0_SetPixelIndex(x, y0, LCD_COLORINDEX);
      }
    #endif
  }
}
#endif

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
  #ifdef LCD_FILL_RECT
     LCD_FILL_RECT(x0, y0, x1, y1, LCD_COLOR);
  #else
    #if !LCD_SWAP_XY
      for (; y0 <= y1; y0++) {
        LCD_L0_DrawHLine(x0,y0, x1);
      }
    #else
      for (; x0 <= x1; x0++) {
        LCD_L0_DrawVLine(x0,y0, y1);
      }
    #endif
  #endif
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  switch (BitsPerPixel) {
  case 1:
    #if (LCD_ENDIAN_BIG == 0)   && \
        ((LCD_BITSPERPIXEL == 2) || (LCD_BITSPERPIXEL == 8)) && \
        (LCD_SWAP_XY)
      xsize += Diff & 7;
      for (i = Diff; i < xsize; i++) {
        _DrawBitLine1BPP_Swap(x0 + i, y0, pData + (i >> 3), ysize, pTrans, BytesPerLine, i);
      }
    #else
      for (i = 0; i < ysize; i++) {
        _DrawBitLine1BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        pData += BytesPerLine;
      }
    #endif
    break;
  #if (LCD_MAX_LOG_COLORS > 2)
    case 2:
      for (i = 0; i < ysize; i++) {
        _DrawBitLine2BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        pData += BytesPerLine;
      }
      break;
  #endif
  #if (LCD_MAX_LOG_COLORS > 4)
    case 4:
      for (i = 0; i < ysize; i++) {
        _DrawBitLine4BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        pData += BytesPerLine;
      }
      break;
  #endif
  #if (LCD_MAX_LOG_COLORS > 16)
    case 8:
      #if (LCD_ENDIAN_BIG == 0)   && \
          (LCD_BITSPERPIXEL == 8) && \
          (LCD_MIRROR_X) && \
          (LCD_SWAP_XY)
        for (i = 0; i < xsize; i++) {
          _DrawBitLine8BPP_Swap(x0 + i, y0, pData + i, ysize, pTrans, BytesPerLine, i);
        }
      #else
        for (i = 0; i < ysize; i++) {
          _DrawBitLine8BPP(x0, i + y0, pData, xsize, pTrans);
          pData += BytesPerLine;
        }
      #endif
      break;
  #endif
  #if (LCD_BITSPERPIXEL > 8)
    case 16:
      for (i = 0; i < ysize; i++) {
        _DrawBitLine16BPP(x0, i + y0, (const U16 *)pData, xsize, pTrans);
        pData += BytesPerLine;
      }
      break;
  #endif
  }
}

/*********************************************************************
*
*       LCD_L0_SetOrg
*/
void LCD_L0_SetOrg(int x, int y) {
  LCD_SET_ORG(x, y);
}

/*********************************************************************
*
*       LCD_On / LCD_Off
*/
void LCD_On (void) {
#ifdef LCD_ON
  LCD_ON();
#endif
}

void LCD_Off (void) {
#ifdef LCD_OFF
  LCD_OFF();
#endif
}

/*********************************************************************
*
*       LCD_L0_Init
*
* Purpose:
*   Initialises the LCD-controller.
*/
int  LCD_L0_Init(void) {
  LCD_INIT_CONTROLLER();
  return 0;
}

/*********************************************************************
*
*       LCD_L0_SetLUTEntry
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
  LCD_SET_LUT_ENTRY(Pos, Color);
}

/*********************************************************************
*
*       LCD_L0_GetDevFunc
*/
void * LCD_L0_GetDevFunc(int Index) {
  GUI_USE_PARA(Index);
  return NULL;
}

#else

void LCDLin32_C(void);
void LCDLin32_C(void) { } /* avoid empty object files */

#endif /* (LCD_CONTROLLER undefined) */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
