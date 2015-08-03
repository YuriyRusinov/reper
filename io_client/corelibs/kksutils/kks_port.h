#ifndef XGIS_BASE_H_INCLUDED
#define XGIS_BASE_H_INCLUDED

#include "configEndians.h"

/**
 * \file xgis_port.h
 *
 * Core portability definitions for XGIS.
 *
 */

/*---------------------------------------------------------------------
 *        types for 16 and 32 bits integers, etc...
 *--------------------------------------------------------------------*/
#if UINT_MAX == 65535
//typedef long            XGISInt32;
typedef unsigned long   XGISUInt32;
#else
//typedef int             XGISInt32;
typedef unsigned int    XGISUInt32;
#endif

//typedef short           GInt16;
typedef unsigned short  XGISUInt16;
typedef unsigned char   XGISByte;
//typedef int             GBool;

#define OK_CODE 1
#define ERROR_CODE 0
#define NULL_PTR NULL
#define NULL_STRING QString::null

/*---------------------------------------------------------------------
 *                         XGIS_LSB and XGIS_MSB
 * Only one of these 2 macros should be defined and specifies the byte 
 * ordering for the current platform.  
 * This should be defined in the Makefile, but if it is not then
 * the default is XGIS_LSB (Intel ordering, LSB first).
 *--------------------------------------------------------------------*/
#if defined(WORDS_BIGENDIAN) && !defined(XGIS_MSB) && !defined(XGIS_LSB)
#  define XGIS_MSB
#endif

#if ! ( defined(XGIS_LSB) || defined(XGIS_MSB) )
#define XGIS_LSB
#endif

#if defined(XGIS_LSB)
#  define XGIS_IS_LSB 1
#else
#  define XGIS_IS_LSB 0
#endif

/*---------------------------------------------------------------------
 *        Little endian <==> big endian byte swap macros.
 *--------------------------------------------------------------------*/

#define XGIS_SWAP16(x) \
        ((XGISUInt16)( \
            (((XGISUInt16)(x) & 0x00ffU) << 8) | \
            (((XGISUInt16)(x) & 0xff00U) >> 8) ))

#define XGIS_SWAP16PTR(x) \
{                                                                 \
    XGISByte       byTemp, *_pabyDataT = (XGISByte *) (x);        \
                                                                  \
    byTemp = _pabyDataT[0];                                       \
    _pabyDataT[0] = _pabyDataT[1];                                \
    _pabyDataT[1] = byTemp;                                       \
}                                                                    
                                                            
#define XGIS_SWAP32(x) \
        ((XGISUInt32)( \
            (((XGISUInt32)(x) & (XGISUInt32)0x000000ffUL) << 24) | \
            (((XGISUInt32)(x) & (XGISUInt32)0x0000ff00UL) <<  8) | \
            (((XGISUInt32)(x) & (XGISUInt32)0x00ff0000UL) >>  8) | \
            (((XGISUInt32)(x) & (XGISUInt32)0xff000000UL) >> 24) ))

#define XGIS_SWAP32PTR(x) \
{                                                                 \
    XGISByte       byTemp, *_pabyDataT = (XGISByte *) (x);        \
                                                                  \
    byTemp = _pabyDataT[0];                                       \
    _pabyDataT[0] = _pabyDataT[3];                                \
    _pabyDataT[3] = byTemp;                                       \
    byTemp = _pabyDataT[1];                                       \
    _pabyDataT[1] = _pabyDataT[2];                                \
    _pabyDataT[2] = byTemp;                                       \
}                                                                    
                                                            
#define XGIS_SWAP64PTR(x) \
{                                                                 \
    XGISByte       byTemp, *_pabyDataT = (XGISByte *) (x);        \
                                                                  \
    byTemp = _pabyDataT[0];                                       \
    _pabyDataT[0] = _pabyDataT[7];                                \
    _pabyDataT[7] = byTemp;                                       \
    byTemp = _pabyDataT[1];                                       \
    _pabyDataT[1] = _pabyDataT[6];                                \
    _pabyDataT[6] = byTemp;                                       \
    byTemp = _pabyDataT[2];                                       \
    _pabyDataT[2] = _pabyDataT[5];                                \
    _pabyDataT[5] = byTemp;                                       \
    byTemp = _pabyDataT[3];                                       \
    _pabyDataT[3] = _pabyDataT[4];                                \
    _pabyDataT[4] = byTemp;                                       \
}                                                                    
                                                            

/* Until we have a safe 64 bits integer data type defined, we'll replace
 * this version of the CPL_SWAP64() macro with a less efficient one.
 */
/*
#define CPL_SWAP64(x) \
        ((uint64)( \
            (uint64)(((uint64)(x) & (uint64)0x00000000000000ffULL) << 56) | \
            (uint64)(((uint64)(x) & (uint64)0x000000000000ff00ULL) << 40) | \
            (uint64)(((uint64)(x) & (uint64)0x0000000000ff0000ULL) << 24) | \
            (uint64)(((uint64)(x) & (uint64)0x00000000ff000000ULL) << 8) | \
            (uint64)(((uint64)(x) & (uint64)0x000000ff00000000ULL) >> 8) | \
            (uint64)(((uint64)(x) & (uint64)0x0000ff0000000000ULL) >> 24) | \
            (uint64)(((uint64)(x) & (uint64)0x00ff000000000000ULL) >> 40) | \
            (uint64)(((uint64)(x) & (uint64)0xff00000000000000ULL) >> 56) ))
*/

#define XGIS_SWAPDOUBLE(p) XGIS_SWAP64PTR(p)

#ifdef XGIS_MSB
#  define XGIS_MSBWORD16(x)      (x)
#  define XGIS_LSBWORD16(x)      XGIS_SWAP16(x)
#  define XGIS_MSBWORD32(x)      (x)
#  define XGIS_LSBWORD32(x)      XGIS_SWAP32(x)
#  define XGIS_MSBPTR16(x)       
#  define XGIS_LSBPTR16(x)       XGIS_SWAP16PTR(x)
#  define XGIS_MSBPTR32(x)       
#  define XGIS_LSBPTR32(x)       XGIS_SWAP32PTR(x)
#  define XGIS_MSBPTR64(x)       
#  define XGIS_LSBPTR64(x)       XGIS_SWAP64PTR(x)
#else
#  define XGIS_LSBWORD16(x)      (x)
#  define XGIS_MSBWORD16(x)      XGIS_SWAP16(x)
#  define XGIS_LSBWORD32(x)      (x)
#  define XGIS_MSBWORD32(x)      XGIS_SWAP32(x)
#  define XGIS_LSBPTR16(x)       
#  define XGIS_MSBPTR16(x)       XGIS_SWAP16PTR(x)
#  define XGIS_LSBPTR32(x)       
#  define XGIS_MSBPTR32(x)       XGIS_SWAP32PTR(x)
#  define XGIS_LSBPTR64(x)       
#  define XGIS_MSBPTR64(x)       XGIS_SWAP64PTR(x)
#endif

#ifdef __cplusplus
#  define XGIS_C_START           extern "C" {
#  define XGIS_C_END             }
#else
#  define XGIS_C_START
#  define XGIS_C_END
#endif


#endif /* ndef XGIS_BASE_H_INCLUDED */
