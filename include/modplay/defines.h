/*
 * Copyright (c) 2002, 2003, 2004, 2005, 2007 by Christian Nowak <chnowak@web.de>
 * Last update: 20th October, 2007
 */

#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
/*#define LITTLE_ENDIAN*/

#if   UCHAR_MAX == 0xff
typedef unsigned char u8;
typedef signed char s8;
#elif USHRT_MAX == 0xff
typedef unsigned short u8;
typedef signed short s8;
#elif UINT_MAX == 0xff
typedef unsigned int u8;
typedef signed int s8;
#elif ULONG_MAX == 0xff
typedef unsigned long u8;
typedef signed long s8;
#else
#error No 1 byte wide type found for u8 and s8
#endif

#if   UCHAR_MAX == 0xffff
typedef unsigned char u16;
typedef signed char s16;
#elif USHRT_MAX == 0xffff
typedef unsigned short u16;
typedef signed short s16;
#elif UINT_MAX == 0xffff
typedef unsigned int u16;
typedef signed int s16;
#elif ULONG_MAX == 0xffff
typedef unsigned long u16;
typedef signed long s16;
#else
#error No 2 byte wide type found for u16 and s16
#endif

#if   UCHAR_MAX == 0xffffffff
typedef unsigned char u32;
typedef signed char s32;
#elif USHRT_MAX == 0xffffffff
typedef unsigned short u32;
typedef signed short s32;
#elif UINT_MAX == 0xffffffff
typedef unsigned int u32;
typedef signed int s32;
#elif ULONG_MAX == 0xffffffff
typedef unsigned long u32;
typedef signed long s32;
#else
#error No 4 byte wide type found for u32 and s32
#endif
/*
#if  ULLONG_MAX == 0xffffffffffffffff
*/
typedef unsigned long long int u64;
typedef signed long long int s64;
/*
#else
#error No 8 byte wide type found for u64 and s64
#endif
*/
typedef union
  {
#ifdef LITTLE_ENDIAN
    struct
      {
        u8 low;
        u8 high;
      } abyte;
#else
    struct
      {
        u8 high;
        u8 low;
      } abyte;
#endif
    u16 aword;
  } union_word;


typedef union
  {
#ifdef LITTLE_ENDIAN
    struct
      {
        u16 low;
        u16 high;
      } aword;
#else
    struct
      {
        u16 high;
        u16 low;
      } aword;
#endif
    u32 adword;
  } union_dword;





#ifndef BOOL
#define BOOL  int
#endif
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#ifdef __cplusplus
  }
#endif

#endif
