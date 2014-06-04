/***************************************************************************
                          Gost32.h  -  описание
                             -------------------
    начало               : Mon Jul 14 2003
    авторство            : (C) 2003 разработчик 
    почта                : 
 ***************************************************************************/

/***************************************************************************
                          Gost32.h  -  описание
                             -------------------
    начало               : Thu Jul 10 2003
    авторство            : (C) 2003 разработчик 
    почта                : 
 ***************************************************************************/

#ifndef _Gost28147H
#define _Gost28147H

#if !defined BYTE
    typedef unsigned char       BYTE;
#endif
#if !defined WORD
typedef unsigned short      WORD;
#endif
#if !defined DWORD
typedef unsigned long       DWORD;
#endif
#if !defined UINT
typedef unsigned int        UINT;
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)((UINT)(w) >> 8))
#endif
#ifndef LOWORD
#define LOWORD(w)           ((WORD)(w))
#endif
#ifndef HIWORD
#define HIWORD(w)           ((WORD)((DWORD)(w) >> 16))
#endif
#ifndef MAKELONG
#define MAKELONG(low, high) ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#endif
#ifndef MAKEWORD
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))
#endif




static const DWORD Synchro[2] = {0x01010104, 0x01010101};


#define KEY_SIZE  256
#define TABLE_SIZE  512
#define BLOCK_SIZE  64

void ECBEncrypt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void ECBDecrypt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void CFBEncrypt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void CFBDecrypt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void PrepareCBC(DWORD Key[8]);
void CBCCrypt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void ImitIt(DWORD &Datal, DWORD &Datar, DWORD Key[8]);
void InitAll();
void SetTable(DWORD tab[16]);

#endif
