/**********************************************************************/
/*               ������������ ���� ��� Universal Platform             */
/**********************************************************************/

#ifndef PLOTH
#define PLOTH

/* ���������� ����� ��� ����������                                    */
//define PLOT_LINUX 1
#define PLOT_UNIX 1
/*
#define WINNT
#define VMESA
#define PLOT_UNIX 0  // LINUX
#define PLOT_UNIX 1  // MSVS
#define PLOT_UNIX 2  // INTRAS
#define PLOT_UNIX 3  // LINUX390
#define PLOT_MAC_OSX
#define PLOT_OS2
*/
/*#define ACNTFREE - ��� TESTACNT                                     */

/***************************** �   �   �   � **************************/
/*====================== I N C L U D E  P A T H  =====================*/
/*--------------------------------------------------------------------*/

#ifdef WINNT
  #include <windows.h>
#endif /*WINNT*/

#ifdef VMESA
  #include <vmcmt.h>
#endif /*VMESA*/

#ifdef PLOT_UNIX
  #include <unistd.h>
  #include <sys/time.h>
  #include <pthread.h>
  #include <sys/types.h>
  #include <sys/ipc.h>
  #include <sys/sem.h>
  #include <sys/stat.h>
  #include <sys/param.h>
  #include <sys/resource.h>
  #include <syslog.h>
  #include <signal.h>
  #include <dirent.h>
  #include <ctype.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <sys/ioctl.h>
  #include <sys/wait.h>
#endif /*PLOT_UNIX*/

#ifdef PLOT_OS2
  #define INCL_BASE
  #include <os2.h>
  #include <types.h>
  #include <process.h>
  #include <string.h>
  #include <ctype.h>
#endif /*PLOT_OS2*/

#ifdef PLOT_MAC_OSX
  #include <unistd.h>
  #include <sys/times.h>
  #include <sys/time.h>
  #include <pthread.h>
  #include <sys/types.h>
  #include <sys/ipc.h>
  #include <semaphore.h>
  #include <sys/stat.h>
  #include <sys/param.h>
  #include <sys/resource.h>
  #include <syslog.h>
  #include <signal.h>
  #include <dirent.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif /*PLOT_MAC_OSX*/

#if !defined(_WIN32_WCE)
#include <errno.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

/***************************** �   �   �   � **************************/
/*========== � � � � � � � � � � � � �   � � � � � � � � � � � =======*/
#define PLOT_VERSION "5.0"

#ifdef WINNT
  #ifndef PLOTLIB
    #define PLOTLIB extern "C" __declspec(dllimport)
  #endif
  #define PLOT_CLK_TCK CLK_TCK
#endif /*WINNT*/
#ifdef VMESA
  typedef struct tm tm;
  #undef FALSE
  #undef TRUE
  typedef enum { FALSE, TRUE } bool;
  #define WINAPI
  #define PLOTLIB
  #define fparm (char *)&plist[1][0]
  #define PLOT_CLK_TCK 1000
#endif /*VMESA*/
#ifdef PLOT_UNIX
  typedef struct tm tm;
  #ifndef __cplusplus
    #undef FALSE
    #undef TRUE
    enum PLOT_BOOL { FALSE, TRUE };
    typedef enum PLOT_BOOL bool;
  #endif
  #define WINAPI
  #ifndef PLOTLIB
    #ifdef __cplusplus
      #define PLOTLIB extern
    //saa hack "C" __declspec(dllimport)
    #else
      #define PLOTLIB extern
    #endif
  #endif
  #define fparm argv[1]
  #define PLOT_CLK_TCK CLOCKS_PER_SEC
  #define Screen GetScreen()
#endif /*PLOT_UNIX*/
#ifdef PLOT_OS2
  typedef struct tm tm;
  #undef FALSE
  #undef TRUE
  typedef enum { FALSE, TRUE } bool;
  #define WINAPI
  #ifndef PLOTLIB
    #define PLOTLIB extern "C" __declspec(dllimport)
  #endif
  #define fparm argv[1]
  #define PLOT_CLK_TCK CLK_TCK
#endif /*PLOT_OS2*/

#ifdef PLOT_MAC_OSX
  typedef struct tm tm;
  #undef FALSE
  #undef TRUE
  typedef enum { FALSE, TRUE } bool;
  #define WINAPI
  #ifndef PLOTLIB
    #define PLOTLIB extern
  #endif
  #define fparm argv[1]
  #define PLOT_CLK_TCK 100
#endif /*PLOT_MAC_OSX*/

#ifndef min
  #define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
  #define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef MAKELONG
  #define MAKELONG(a, b) ((long) (((unsigned short) (a)) | ((unsigned long) ((unsigned short) (b))) << 16))
#endif
#ifndef MAKEWORD
  #define MAKEWORD(a, b) ((unsigned short) (((unsigned char) (a)) | ((unsigned short) ((unsigned char) (b))) << 8))
#endif

#define ZERO                0
#ifndef FALSE
  #define FALSE             (bool)0
#endif
#ifndef TRUE
  #define TRUE              (bool)1
#endif
#define u                   unsigned
#ifdef VMESA
  #define PLOT_INI_PLACEMENT "plot.ini.c"
  #define PLOT_ENV_PLACEMENT "plot.env.c"
  #define PLOT_LOG_PLACEMENT "plot.log.c"
#endif /*VMESA*/
#ifdef WINNT
#if !defined(_WIN32_WCE)
  #define PLOT_INI_PLACEMENT "c:\\plot\\plot.ini"
  #define PLOT_ENV_PLACEMENT "c:\\plot\\plot.env"
  #define PLOT_LOG_PLACEMENT "c:\\plot\\plot.log"
#else
  #define PLOT_INI_PLACEMENT "\\plot\\plot.ini"
  #define PLOT_ENV_PLACEMENT "\\plot\\plot.env"
  #define PLOT_LOG_PLACEMENT "\\plot\\plot.log"
#endif
#endif /*WINNT*/
#ifdef PLOT_UNIX
  #define PLOT_INI_PLACEMENT "/usr/plot/plot.ini"
  #define PLOT_ENV_PLACEMENT "/usr/plot/plot.env"
  #define PLOT_LOG_PLACEMENT "/usr/plot/plot.log"
  #define SHARED_TEMP_DIR    "/usr/plot/sharedtemp/"
  #define OBMN_SHARED_PID    "/usr/plot/sharedtemp/pid/"
#endif /*PLOT_UNIX*/
#ifdef PLOT_OS2
  #define PLOT_INI_PLACEMENT "c:\\plot\\plot.ini"
  #define PLOT_ENV_PLACEMENT "c:\\plot\\plot.env"
  #define PLOT_LOG_PLACEMENT "c:\\plot\\plot.log"
#endif /*PLOT_OS2*/
#ifdef PLOT_MAC_OSX
  #define PLOT_INI_PLACEMENT "/plot/plot.ini"
  #define PLOT_ENV_PLACEMENT "/plot/plot.env"
  #define PLOT_LOG_PLACEMENT "/plot/plot.log"
  #define SHARED_TEMP_DIR    "/plot/sharedtemp/"
  #define OBMN_SHARED_PID    "/plot/sharedtemp/pid/"
#endif /*PLOT_MAC_OSX*/

/* ���������                                                          */
#if defined(VMESA) | (defined(PLOT_UNIX) & (PLOT_UNIX == 3))
  #define PLOT_CODE_ANSI    1
  #define PLOT_CODE_DKOI    2
  #define PLOT_CODE_ASCII   3
  #define PLOT_CODE_KOI8    4
  #define PLOT_CODE_MACRUS  5
  #define PLOT_CODE_EBCDIC  6
#else
  #define PLOT_CODE_ANSI    129
  #define PLOT_CODE_DKOI    130
  #define PLOT_CODE_ASCII   131
  #define PLOT_CODE_KOI8    132
  #define PLOT_CODE_MACRUS  133
  #define PLOT_CODE_EBCDIC  134
#endif

#ifdef VMESA
  #define PLOT_SYS_CODE     PLOT_CODE_DKOI
#endif /*VMESA*/
#ifdef WINNT
  #define PLOT_SYS_CODE     PLOT_CODE_ANSI
#endif /*WINNT*/
#ifdef PLOT_UNIX
  #define PLOT_SYS_CODE     PLOT_CODE_KOI8
#endif /*PLOT_UNIX*/
#ifdef PLOT_OS2
  #define PLOT_SYS_CODE     PLOT_CODE_ASCII
#endif /*PLOT_OS2*/
#ifdef PLOT_MAC_OSX
  #define PLOT_SYS_CODE     PLOT_CODE_MACRUS
#endif /*PLOT_MAC_OSX*/

#define JUR_SYS_NAME  "jur_sys.jrn"/* ��� ����� ������� ���������     */
                                   /* ���������                       */
#define JUR_DAT_NAME  "jur_sys.jdt"/* ��� ����� ������� ���������     */
                                   /* ��������� (������)              */
#define JI_NAME         "jour_in.jrn"  /* ��� ����� ���               */
#define JO_NAME         "jour_out.jrn" /* ��� ����� ���               */
#define JSO_NAME        "jour_so.jrn"  /* ��� ����� �����             */
#define JFO_NAME        "jour_fo.jrn"  /* ��� ����� ���FTP            */
#define JSYSO_NAME      "jour_syo.jrn" /* ��� ����� ���SYS            */
#define JSYSI_NAME      "jour_syi.jrn" /* ��� ����� ���SYS            */

#define JI_EVENT_MDF    "ji_mdf_record"/* ������� ���������� ���      */
#define JI_EVENT_ADD    "ji_add_record"/* ������� ���������� ���      */
#define JI_EVENT_END    "ji_end_record"/* ������� ���������� ���      */
#define JO_EVENT_MDF    "jo_mdf_record"/* ������� ���������� ���      */
#define JO_EVENT_ADD    "jo_add_record"/* ������� ���������� ���      */
#define JO_EVENT_END    "jo_end_record"/* ������� ���������� ���      */
#define JSO_EVENT_MDF   "jso_mdf_record"/* ������� ���������� �����   */
#define JSO_EVENT_ADD   "jso_add_record"/* ������� ���������� �����   */
#define JSO_EVENT_END   "jso_end_record"/* ������� ���������� �����   */
#define JFO_EVENT_MDF   "jfo_mdf_record"/* ������� ���������� ��� FTP */
#define JFO_EVENT_ADD   "jfo_add_record"/* ������� ���������� ��� FTP */
#define JFO_EVENT_END   "jfo_del_record"/* ������� ���������� ��� FTP */
#define JSYSO_EVENT_MDF "jsyso_mdf_record"/* ������� ���������� ���SYS*/
#define JSYSO_EVENT_ADD "jsyso_add_record"/* ������� ���������� ���SYS*/
#define JSYSO_EVENT_END "jsyso_end_record"/* ������� ���������� ���SYS*/
#define JSYSI_EVENT_MDF "jsysi_mdf_record"/* ������� ���������� ���SYS*/
#define JSYSI_EVENT_ADD "jsysi_add_record"/* ������� ���������� ���SYS*/
#define JSYSI_EVENT_END "jsysi_end_record"/* ������� ���������� ���SYS*/

#define JSYS_ID         "SJRN"     /* ������������� �C�               */
#define JDAT_ID         "SDAT"     /* ������������� �C� (������)      */
#define JI_ID           "IJRN"     /* ������������� ���               */
#define JO_ID           "OJRN"     /* ������������� ���               */
#define JSO_ID          "SOJR"     /* ������������� �����             */
#define JFO_ID          "FOJR"     /* ������������� ����� FTP         */
#define JSYSO_ID        "SYOJ"     /* ������������� ���SYS            */
#define JSYSI_ID        "SYIJ"     /* ������������� ���SYS            */

#define PLOT_SUZIDN         "SUZ"/* ������������� SUZ�                */
#define PLOTMSGSIZE         32736/* max ������ ��������� � �������    */
#define PLOTSUSIZE          1840 /* max ���������� ������ ��������    */
#define PLOTINFSIZE         30000/* ������ ���-��� ����� ���������    */
#define PLOT_WAIT_REPLY     120000/* ������������� SUZ�               */
#define PLOTPATHSIZE        256  /* max ������ ������� ����� �����    */
#define PLOT_TI_NAME        128  /* ��� ������ ����� ��               */
#define PLOTKEYSIZE         8    /* ������ ����� ��������� ������     */
#ifdef VMESA
#define PLOTINISTRSIZE      80   /* ����� ������ �������� � profile's */
#else
#define PLOTINISTRSIZE      1024 /* ����� ������ �������� � profile's */
#endif
#ifdef VMESA
  #define DELAYMAX          0    /* max ����� �������� ������         */
  #define DELAYMIN          0    /* min ����� �������� ������         */
  #define DELAYDOWN         0    /* ������ + ��� ������� ��������     */
  #define DELAYUP           0    /* ������ - ��� ������� ��������     */
#endif /*VMESA*/
#ifndef VMESA
  #define DELAYMAX          100  /* max ����� �������� ������         */
  #define DELAYMIN          20   /* min ����� �������� ������         */
  #define DELAYDOWN         5    /* ������ + ��� ������� ��������     */
  #define DELAYUP           2    /* ������ - ��� ������� ��������     */
#endif /*VMESA*/
#ifdef PLOT_UNIX
  #define PLOTOBMNSIZE      0xFF00
#endif /*PLOT_UNIX*/
#ifdef PLOT_OS2
  #define PLOTOBMNSIZE      0xFF00
#endif /*PLOT_OS2*/
#ifdef PLOT_MAC_OSX
  #define PLOTOBMNSIZE      0xFF00
#endif /*PLOT_MAC_OSX*/
#define PLOBL               128  /* ����� ������� ��������            */
                                 /* ���������� TCA                    */
#define SEGMN               8    /* �-�� ��������� ���������          */
#define STKNUMPARM          16   /* ����� ���������� �������,         */
                                 /* ����������� � �����               */
#define PLOTTASKADD         4    /* max ���������� ����� ��� ���������*/
#define PLOTMAXRECSIZE      8000 /* max ������ ������ � �������       */
#define PLOTTABSTRSIZE      25500/* ������ ��������� �������          */
#define PLOTTABINFSIZE      30000/* ������ ���-��� ����� ���������    */
#define PLOTSIZEHESCHTABLE  4096 /* ������ ���-������� ������         */
#define PLOTOBJSYMBOLNAMELEN   9 /* ����� ����������� ����� �������   */

/* �������� �������� pcastop                                          */
#define PLOT_STOP_IGN       0    /* ������ ���������                  */
#define PLOT_STOP_END       1    /* ������� ���������� ������         */
#define PLOT_STOP_REINIT    2    /* ������� ����������������� ������  */

/* ���� ���������                                                     */
#define MSG_STOR_TEMP       1    /* ����������� �� ��������� ������   */
#define MSG_STOR_NULL       2    /* ����������� �� �� ��������� ������*/
#define MSG_OBMN_SYNC       4    /* ����������                        */
#define MSG_OBMN_ASYNC      8    /* �����������                       */
#define MSG_OBMN_SYS        16   /* ���������                         */
#define MSG_TYPE_k          32   /* ��������� � ���������             */
#define MSG_TYPE_K          64   /* ��������� � ���������� �� �����.  */
#define MSG_TYPE_O          128  /* ��������� �� ���������            */
#define MSG_TYPE_MSG        256  /* ���������                         */
#define MSG_ADR_LOCAL       512  /* �� == ��                          */
#define MSG_OBMN_INIT       1024 /* ���������                         */
#define MSG_OBMN_FWD        2048 /* ��������� ���������               */
#define MSG_OBMN_INIT_ASYNC 4096 /* ����������� c��������             */

#define PLOT_FUNC           0    /* ������� ��� ������������� �       */
                                 /* ������� ������� PLOT              */
#define B204_FUNC           1000 /* ������� ��� ������������� �       */
                                 /* ������� ������� B204              */
#define GNET_FUNC           2000 /* ������� ��� ������������� �       */
                                 /* ������� ������� GNET              */
#define INEX_FUNC           3000 /* ������� ��� ������������� �       */
                                 /* ������� ������� INEX              */
#define PDKL_FUNC           4000 /* ������� ��� ������������� �       */
                                 /* ������� ������� PDKL              */
#define USER_FUNC           5000 /* ������� ��� ������������� �       */
                                 /* ������� ������� USER              */
#define MOCT_FUNC           6000 /* ������� ��� ������������� �       */
                                 /* ������� ������� MOCT              */
#define BAUL_FUNC           7000 /* ������� ��� ������������� �       */
                                 /* ������� ������� BAUL              */
#define WALL_FUNC           8000 /* ������� ��� ������������� �       */
                                 /* ������� ������� WALL              */
#define CASE_FUNC           9000 /* ������� ��� ������������� �       */
                                 /* ������� ������� CASE              */
#define GRAD_FUNC          10000 /* ������� ��� ������������� �       */
                                 /* ������� ������� GRAD              */

typedef void* LINK;              /* ���������� ����-� � ��. �������   */
typedef void* CURSOR;            /* ���������� �������� �������       */

#define PLOT_FN_AVR     pstk->callstk[pstk->smpncall].avr
#define PLOT_FN_REZ     pstk->callstk[pstk->smpncall].rez
#define PLOT_FN_LASTREZ pstk->callstk[pstk->smpncall+1].rez
#define PLOT_FN_LASTAVR pstk->callstk[pstk->smpncall+1].avr

#define PLOT_RET(pstk, err_code, wrn_code)\
  PLOT_FN_AVR = err_code;\
  PLOT_FN_REZ = wrn_code;\
  pstk->smppavr = err_code;\
  pstk->smpprez = wrn_code;\
  pstk->smpncall--;\
  if (pstk->smppavr != PLOTNOERR){\
    return PLOTERROR;\
  }\
  if (pstk->smpprez != PLOTNOERR){\
    return PLOTWARNING;\
  }\
  else{\
    return PLOTNOERR;\
  }

#define PLOT_BEG(pstk, id)\
  if (pstk == NULL){\
    return PLOTERROR;\
  }\
  pstk->smpncall++;\
  if (pstk->smpncall >= 16){\
    pstk->smppavr = PLOTERRSTKOVRFLW;\
    return PLOTERROR;\
  }\
  memset(&pstk->callstk[pstk->smpncall], 0, sizeof(CALLSTK));\
  pstk->callstk[pstk->smpncall].idfn = id;\
  active_fix(pstk);\
  if (pstk->smpncall + 1 < 16){\
    memset(&pstk->callstk[pstk->smpncall + 1], 0, sizeof(CALLSTK));\
  }

/*====================================================================*/
/*=========== � � � � � � � � � � �  � � � � � � (������) ============*/
/*====================================================================*/
/* ��� ������, ������� ����������� � ������ �����,� ������������ �    */
/* ������������ �������                                               */
#define PLOTNOERR         0
#define PLOTERROR        -1
#define PLOTWARNING       1
#define PLOTERR           12000
#define GNETERR           PLOTERR+400
#define INEXERR           PLOTERR+600
#define PDKLERR           PLOTERR+800
#define USERERR           PLOTERR+1000
#define MOCTERR           PLOTERR+1200
#define WALLERR           PLOTERR+1400
#define B204ERR           PLOTERR+1600
#define GRADERR           PLOTERR+1800
#define CASEERR           PLOTERR+2000
#define PLOTERRJNOTREC    PLOTERR+1  /* �� ������� ������,            */
                                     /* ��������������� ����� ������  */
#define PLOTERRINVALPARFN PLOTERR+2  /* ������������ �������
                                        ��������� ��� �������         */
#define PLOTERRJDAMAGE    PLOTERR+3  /* �������� ��������� �������    */
#define PLOTERRTOOMANYTHR PLOTERR+4  /* ��������� MAX ����������
                                        ����� ������� � ��������      */
#define PLOTERRTOOMANYPRC PLOTERR+5  /* ��������� MAX ����������
                                        ����� ��������� � �������     */
#define PLOTERRJEOF       PLOTERR+6  /* ��������� ����� �������       */
#define PLOTERRNOACTUAL   PLOTERR+7  /* ������ �� ���������           */
#define PLOTERRNOPCT      PLOTERR+9  /* ��� PCT                       */
#define PLOTERRNOPROC     PLOTERR+10 /* ��� ������ �������� � PCT     */
#define PLOTERRNOSTK      PLOTERR+11 /* ��� ������� ������ ��� ���-
                                        ����� (�� ������� �������)    */
#define PLOTERRLINEBROKEN PLOTERR+13 /* ��������� �������             */
#define PLOTERRPTRLINK    PLOTERR+14 /* ������� ���������� ����-�     */
                                     /* � ������ �������              */
#define PLOTERRNOLINE     PLOTERR+15 /* ������� ������ � ����������-
                                        ���� �������                  */
#define PLOTERRLINENOMSG  PLOTERR+17 /* ��� ������� � �������         */
#define PLOTERRNOLINK     PLOTERR+19 /* �� ������� ���������� ����-�  */
                                     /* � ��������� �������           */
#define PLOTERRMAXLINK    PLOTERR+20 /* ���������� ����������� ���-   */
                                     /* �������� ���������� ����-�    */
#define PLOTERRSLVNUM     PLOTERR+21 /* ������ ������������� SLVNUM   */
#define PLOTERRSLVADR     PLOTERR+22 /* ������ ������������� SLVADR   */
#define PLOTERRSLV21Y     PLOTERR+23 /* ������ ������������� SLV21Y   */
#define PLOTERRIUCVSET    PLOTERR+24 /* ������ �������� IUCV SET      */
#define PLOTERRIUCVCLR    PLOTERR+25 /* ������ �������� IUCV CLR      */
#define PLOTERRINVALADDR  PLOTERR+26 /* ������������ ����� � ��       */
#define PLOTERRNOTNUM     PLOTERR+27 /* ������ ��� ������ ������ ��   */
                                     /* �� ������ ����������          */
#define PLOTERRNOTADR     PLOTERR+29 /* ��� ������ ���. �� � SLVADR   */
#define PLOTERRFIELDNAME  PLOTERR+32 /* ����������� ��� ���� � ��     */
#define PLOTERRPROCNOWREG PLOTERR+34 /* ������ ��� ����������������   */
#define PLOTERRPROCNOTREG PLOTERR+35 /* ������ �� ����������������    */
#define PLOTERRNOWENQ     PLOTERR+36 /* ��������� ������ �������      */
                                     /* ������������ ���������� ��    */
                                     /* ���� � ��� �� ������          */
#define PLOTERRNOTREC     PLOTERR+39 /* ������������. ������ �� ����� */
#define PLOTERRNOACCREC   PLOTERR+40 /* ��� ������� � ���. �������    */
#define PLOTERRACCESS     PLOTERR+41 /* ����� ������ � ������ ������� */
#define PLOTERRBADQUERY   PLOTERR+42 /* ��������� ������              */
#define PLOTERRBADRECLEN  PLOTERR+43 /* �������� ����� ������         */
#define PLOTERRRECEXIST   PLOTERR+44 /* �������� ��� ����������       */
#define PLOTERRDATAOOB    PLOTERR+45 /* ����� ������ �� �������       */
                                     /* �������                       */
#define PLOTERRKFSBADFLEN PLOTERR+46 /* �������� ����� ���� � KFSCODTB*/
#define PLOTERRKFSNOFIELD PLOTERR+47 /* ���������� ��� ���� � KFSCODTB*/
#define PLOTERRIUCVCONECT PLOTERR+48 /* ������ �������� IUCV CONNECT  */
#define PLOTERRIUCVACCEPT PLOTERR+49 /* ������ �������� IUCV ACCEPT   */
#define PLOTERRIUCVSEND   PLOTERR+50 /* ������ �������� IUCV SEND     */
#define PLOTERRIUCVRECEIV PLOTERR+51 /* ������ �������� IUCV RECEIVE  */
#define PLOTERRIUCVREPLY  PLOTERR+52 /* ������ �������� IUCV REPLY    */
#define PLOTERRIUCVSEVER  PLOTERR+53 /* ������ �������� IUCV SEVER    */
#define PLOTERRIUCVDELAY  PLOTERR+54 /* ������ ��� ������ �������     */
                                     /* delay �� IUCV-�������         */
#define PLOTERRNOTABLE    PLOTERR+55 /* ����������� ����� ������      */
#define PLOTERRRTEMPMEM   PLOTERR+57 /* ������ ������ �� ���������    */
                                     /* ������                        */
#define PLOTERRRTEMPEOF   PLOTERR+58 /* ����� ������ �� ���������     */
                                     /* ������                        */
#define PLOTERRTMPFRMT    PLOTERR+61 /* ������������ ������ �������-  */
                                     /* ����� ����� ��������� ������  */
#define PLOTERRNOSU       PLOTERR+62 /* ������ �� ����� ������ ���-   */
                                     /* �����                         */
#define PLOTERRNOREZIM    PLOTERR+64 /* ������ ����� ������ �������   */
                                     /* ���� �� ��������������        */
#define PLOTERRTIMEOUT    PLOTERR+65 /* ����� �������� ������������   */
                                     /* �������                       */
#define PLOTERRPROCNOWAIT PLOTERR+66 /* ���������� ��������� ��       */
                                     /* ��������� � ������ ��������   */
#define PLOTERRNOTHREAD   PLOTERR+67 /* ��� ������-���������� ������  */
#define PLOTERRPRP        PLOTERR+68 /* ������ �� ����� �����������   */
#define PLOTERRNOPROFILE  PLOTERR+69 /* ������ ������ � �������       */
                                     /* �������������                 */
#define PLOTERRINITLINE   PLOTERR+70 /* ������ ������������� �������  */
#define PLOTERRNOACCESS   PLOTERR+71 /* � ������ ��� ������� ��� ��-  */
                                     /* ���������                     */
#define PLOTERRALRSTARTED PLOTERR+72 /* ������� ��� ��������          */
#define PLOTERRFORMATSLV  PLOTERR+73 /* ������ ������������� FORMAT   */
#define PLOTERRNOTYPE     PLOTERR+74 /* ��� ���-�� �� ��������� �     */
                                     /* FORMAT.SLV                    */
#define PLOTERRMOCTRESET  PLOTERR+75 /* ������ MOCT ���� �������������*/
#define PLOTERRNODATAHDR  PLOTERR+85 /* ��� ��������� ������ ���      */
                                     /* ��������                      */
#define PLOTERRSIZESUEXT  PLOTERR+86 /* ��������� ������ ����������   */
                                     /* ������ ��������               */
#define PLOTERRSIZEDATA   PLOTERR+87 /* ��������� ������ ������ ��    */
                                     /* ������                        */
#define PLOTERRNODATA     PLOTERR+88 /* ������ ��� ������ ������      */
#define PLOTERRMSGFORMAT  PLOTERR+89 /* �������� ������ ���������     */
#define PLOTERRFRENAME    PLOTERR+90 /* ������ ��� ��������. �����    */
#define PLOTERRSTKOVRFLW  PLOTERR+91 /* ���� ������ ����������        */
#define PLOTERRFOPEN      PLOTERR+92 /* ������ ��� �������� �����     */
#define PLOTERRFCLOSE     PLOTERR+93 /* ������ ��� �������� �����     */
#define PLOTERRFWRITE     PLOTERR+94 /* ������ ��� ������ � ����      */
#define PLOTERRFREAD      PLOTERR+95 /* ������ ��� ������ �� �����    */
#define PLOTERRFSEEK      PLOTERR+96 /* ������ ��� ���������� fseek   */
#define PLOTERRFDELETE    PLOTERR+97 /* ������ ��� �������� �����     */
#define PLOTERRJURNBLOCK  PLOTERR+98 /* ������ � ������ �����������   */
#define PLOTERRTASKENV    PLOTERR+99 /* ����������� ��������� ������  */
#define PLOTERRPTRTABOPN  PLOTERR+100/* ������ �������� �������       */
#define PLOTERRMAXCURSOR  PLOTERR+101/* ���-�� ��������- �����������  */
#define PLOTERRLOWMEM     PLOTERR+102/* �� ���������� ������          */
#define PLOTERRNOACCMDF   PLOTERR+103/* ��� ������� ��� �����������   */
#define PLOTERRSENT       PLOTERR+104/* ��� ������� ��� ��������      */
#define PLOTERRDINAMSEL   PLOTERR+105/* ������ ��� ������������ �����.*/
                                     /* SELECTa */
#define PLOTERRNOSTRDS    PLOTERR+106/* ������������ ������ ��� �����.*/
                                     /* SELECTa */
#define PLOTERRREADYCAN   PLOTERR+107/* ������ ��� ��������� ������   */
#define PLOTERRRECGARD    PLOTERR+108/* ������ ��� ������ �� GARDOV   */
#define PLOTERRNOSTRUCT   PLOTERR+110/* ������. �� �������� � KFSCODTB*/
#define PLOTERRNOENQ      PLOTERR+111/* ��� Enq                       */
#define PLOTERRNOTRABADR  PLOTERR+112/* ��� ����� �������� ������     */
#define PLOTERRNOTMAINSNT PLOTERR+113/* ��� ������ � SLVSNT           */
#define PLOTERRFTPPROPS   PLOTERR+120/* �������� �������� ��          */
#define PLOTERRFTPREZIM   PLOTERR+121/* ��������������� ����� ������� */
#define PLOTERRFTPPARAM   PLOTERR+122/* �������� ��. ���������        */
#define PLOTERRFTPSYSBL   PLOTERR+123/* �������� ����������� ���. ��. */
#define PLOTERRFTPSYSMK   PLOTERR+124/* ������ � ��������� ��         */
#define PLOTERRFTPOTVET   PLOTERR+125/* ������ ���������� ������� ���.*/
#define PLOTERRFTPOPENND  PLOTERR+126/* ������ �������� ��            */
#define PLOTERRFTPSYSPR   PLOTERR+127/* ������ ��� ������ �� SYSPRINT */
#define PLOTERRFTPNFOUND  PLOTERR+128/* �� ���������� ��� �� ������   */
#define PLOTERRFTPTIOTUCB PLOTERR+129/* ������ ��� ������ c TIOT/UCB  */
#define PLOTERRFTPCREATE  PLOTERR+130/* ������ ��� �������� ��        */
#define PLOTERREOF        PLOTERR+131/* ��������� ����� �����         */
#define PLOTERRCODEUNKN   PLOTERR+132/* ����������� ��������� ������  */
#define PLOTERRFTPERRNAME PLOTERR+133/* �������� ��� ��               */
#define PLOTERRDRVNOREADY PLOTERR+134/* ���������� �� ������          */
#define PLOTERRFCREATE    PLOTERR+135/* ������ ��� �������� �����     */
#define PLOTERRCOMPRESS   PLOTERR+136/* ������ ��� ������ ������      */
#define PLOTERRDECOMPRESS PLOTERR+137/* ������ ��� �������� ������    */
#define PLOTERRNOFLDSRT   PLOTERR+138/* ����������� ������ ���� ���   */
                                     /* ���������� �������            */
#define PLOTERRSNCCADRNUM PLOTERR+139/* �������� ������������������   */
                                     /* ������ ��� ���������� ������  */
#define PLOTERRNOUCHDATA  PLOTERR+140/* �� �������� ������� ������ �� */
#define PLOTERRFSIZE      PLOTERR+141/* ������ ��� ��������� �������  */
                                     /* �����                         */
#define PLOTERRFTPNOSTART PLOTERR+142/* ������ ��� ������� ��������   */
#define PLOTERRBADAOAP    PLOTERR+143/* �� ����� ������ �� ��� ��     */
#define PLOTERRNORIGHT    PLOTERR+144/* ���������� �� ������������    */
#define PLOTERRBADFMDCODE PLOTERR+145/* ��� ������ ���� ��������      */
#define PLOTERRNOTRANG    PLOTERR+146/* �� ��������� ������� ������   */
#define PLOTERRBADRANG    PLOTERR+147/* ��� ������ �����              */
#define PLOTERRNOAO       PLOTERR+148/* �� ������ ��� ������, ������-
                                        ���� ��������                 */
#define PLOTERRNOAP       PLOTERR+149/* �� ����� ����� ����������     */
#define PLOTNEEDEXTRTEST  PLOTERR+150/* ���������� ������������, ���-
                                        ������ �������������� ��������
                                        �� ������������� �������      */
#define PLOTERRNOMODULE   PLOTERR+151/* ��� ������ ������             */
#define PLOTERRNOPATH     PLOTERR+152/* �� ����� ���� (����-��)
                                        ��������/��������             */
#define PLOTERRNOPRTU     PLOTERR+153/* �� ������ ��� ����-�� ������  */
#define PLOTERRBADGRIF    PLOTERR+154/* ������������ ���� ����������  */
#define PLOTERRNOACCSOD   PLOTERR+155/* ��� ������� (������ ��� ����-
                                        �������� ��� ������� ���� ��
                                        ���������� ���������� ��������,
                                        �� ��� ���������� ������� �
                                        ������� ������� - ��������
                                        ����������, �������� ��������
                                        (��������) ����������, ������-
                                        ���� ������ � �.�.)           */
#define PLOTERRNONIPP     PLOTERR+156/* �� ����� ����� �����          */
#define PLOTERRNOMDLM     PLOTERR+157/* �� ��������� ������� �������  */
#define PLOTERRTLINENOWINIT PLOTERR+158/* ������� ��������� ��������- */
                                       /* ����� ������� ������        */
#define PLOTERRINVALUSL   PLOTERR+159/* �������� ������� ������       */
#define PLOTWRKPERMIT     PLOTERR+160/* ���������� ������ ������������*/
#define PLOTERRPASSCOMPR  PLOTERR+161/* ����������-� ������ �� �������*/
#define PLOTERRPASSCHANGE PLOTERR+162/* ������������� ������ �������  */
#define PLOTERRPASSWORD   PLOTERR+163/* ������������ ������ �������   */
#define PLOTERRUSERABS    PLOTERR+164/* ��� ������ ������������       */
#define PLOTERRUSERBLOKED PLOTERR+165/* ������������ ������������     */
#define PLOTERRMODULEABS  PLOTERR+166/* ������� ������� �����-�� ��   */
#define PLOTERRMODULEACSS PLOTERR+167/* ��� ������� � ���. ������     */
#define PLOTERRADMINACSS  PLOTERR+168/* ��� ������� � �������� ���.���*/
#define PLOTERRPASSGENER  PLOTERR+169/* ������ ��� ��������� �������  */
#define PLOTWRKPERMITNEWP PLOTERR+170/* ���������� ������ + ������ �� */
#define PLOTERRPROTPROCSS PLOTERR+171/* ������ ��� ���. ������ �������*/
#define PLOTERRGRIFCHNGACSS PLOTERR+172/* ��� ���������� �� ���. �����*/
#define PLOTERRTYPECHNGACSS PLOTERR+173/* ��� ���������� �� ���. ���� */
#define PLOTERRVIZASETACSS PLOTERR+174/* ��� ���������� �� �����������*/
#define PLOTERRVIZARESETACSS PLOTERR+175/* ��� ���������� �� ������
                                                                 ���� */
#define PLOTERRPROPRTACSS PLOTERR+176/* ��� ���������� �� ��������
                                        ������� ��������-�� �������   */
#define PLOTERRVIZASET    PLOTERR+177/* ���� ��� �����������          */
#define PLOTERRTRASTRELSH PLOTERR+178/* ������������� ��������� ���
                                        ������� ������ �� ����������� */
#define PLOTERRACSSADDR   PLOTERR+179/* ����������� �������� ���
                                        ������� ������ �� ����������� */
#define PLOTERRMODACSSPATH PLOTERR+180/* ��� �������� (����������) ��
                                             ������ ���������� ������ */
#define PLOTERRMAKETABCRC PLOTERR+181/* ������� �������� �����������
                                        �� ������������. �� �������
                                        ������� ���������-� ��������  */
#define PLOTERRTESTCRC    PLOTERR+182/* ��������� ����������� �� � �� */
#define PLOTERRMDMCRC     PLOTERR+183/* �������� ����������� ���      */
#define PLOTERRRANGMAKE   PLOTERR+184/* ������ ��� �������� �����     */
#define PLOTERRPORTNUMB   PLOTERR+185/* ��������� ip-���� �� ������   */
#define PLOTERRPORTNOPARM PLOTERR+186/* ��������� ��������� ������� ��
                                        ip-����� �� �������           */
#define PLOTERRPORTDEFN   PLOTERR+187/* ������-� ip-���� ��� ���������*/
#define PLOTERRPORTPRPARM PLOTERR+188/* ��������� ��������� ������� ��
                                        ip-����� ��� �����������      */
#define PLOTERRIOBLOCKED  PLOTERR+189/* �������������� ������ ��������-
                                        ���. ���������� �������� ��� ��
                                        ���������                     */
#define PLOTPATHOLDER     PLOTERR+190/* �������� ��� (�������) �� ������
                                        � ������� ������� �����������
                                        ������, �� �������� ������������
                                        ��� ������ �� ���������       */
#define PLOTERRUNTGRIFCHG PLOTERR+191/* ��� �������� (����������) �����
                                        ������������������ ���
                                        �����������                   */
#define PLOTERRUNTGRIFSET PLOTERR+192/* ��� ��������� �������� (������-
                                        ����) �� ����������� ����� ���-
                                        ��������������� ��� ����������-
                                        ��� ����� �� ������������� ���-
                                        �� ������������������ �������-
                                        �������� �������              */
#define PLOTERRUSERLEVEL  PLOTERR+193/* ���������� �������������� ����-
                                        ��� ���������� ������������, ��-
                                        ����������������� � �� � ������-
                                        ������, ����������� ����������-
                                        �������� � ��                 */
#define PLOTERRIOXLINK    PLOTERR+194/* ���������� �������������� ������
                                        �������� �������              */
#define PLOTERRITEMGRIF   PLOTERR+195/* ����� ������������������ �� �
                                        ��� UP �� ������������� �����
                                        ������������������ �� � ��    */
#define PLOTERRPATHSETS   PLOTERR+196/* �������� ��� (�������) �� ������
                                        � ������� ������� �����������
                                        ������                        */
#define PLOTMYTEMPPATH    PLOTERR+197/* �������� ������� (���) ��������
                                        ���������� ��������� ������ ���-
                                        ��� ����������� ������        */
#define PLOTERRPATHOLDER  PLOTERR+198/* �������� ������� (���) ��������
                                        ������������ ��� �������� (����)
                                        , ��� �������� ����� ����������-
                                        ��������                      */
#define PLOTERRPATHDOTHER PLOTERR+199/* �������� ������� (���) ��������
                                        �������� �� ��������� � ��������
                                        (����), ��� �������� ����� ���-
                                        ���������������               */
#define PLOTERRPATHSTRTAB PLOTERR+201/* � �������� �������� (����) ���-
                                        ��� ������, ������� ��������
                                        ������������� ��������� �������,
                                        ������������� � ������� ���
                                        ����������� �������� (����)   */
#define PLOTERRPATHTABSTR PLOTERR+202/* � ������� ��� ���������� ����-
                                        ������ ������� (���), � �������
                                        ������ ������, ���������� ���-
                                        ���������� ��������� �������,
                                        ������������� ��� ��������� ��-
                                        ������ (����)                 */
#define PLOTERRNOPRINTER  PLOTERR+203/* �� ������� ��������� ����������
                                        ������  ��� ���� ���������� ��
                                        ������������� ����� ����������*/
#define PLOTERRCASESUBJECTS PLOTERR+204 /* ����������� ����� ������
                                           ��� �������                */
#define PLOTERRCASEXMLENTRY PLOTERR+205 /* �������� ���������
                                           XML-�������� �������       */
#define PLOTERRCASEITEMINDEX PLOTERR+206/* ����������� ����� ������
                                           �������� �������           */
#define PLOTERRCASEMAXLEVELS PLOTERR+207/* ��������� ������������
                                           ������� �����������
                                           ��������� �������          */
#define PLOTERRCASENOFOLDER PLOTERR+208 /* ��� ����� ����� �� ������� */
#define PLOTERRCASENOITEM PLOTERR+209   /* ������� ������� �� ������  */
#define PLOTERRCASEITEMTYPE PLOTERR+210/* ������� ��� �������� �������*/
#define PLOTERRCASEFOLDERENTRY PLOTERR+211/* ����������� ������
                                             ������������ ���� (���-
                                             ����, ������� ��� ����-
                                             �����) ����� �������     */
#define PLOTERRSLVOBJ     PLOTERR+212/* ������ ������������� SLVOBJ   */
#define PLOTERRYNDEXFORMAT PLOTERR+213/* ������� ������ ������� ������
                                         ����������� ������           */
#define PLOTERRYNDEXSIZE  PLOTERR+214 /* ��������� ������ �������
                                         ������ ����������� ������    */
#define PLOTERRYNDEXMAXSIZE  PLOTERR+215 /* �������� �����������
                                            ���������� ������ �������
                                            ������ ����������� ������ */
#define PLOTERRYNDEXSTRUCT  PLOTERR+216 /* �������� ��������� �������
                                           ������ ����������� ������  */
#define PLOTERRCASENOTREG  PLOTERR+217 /* ������� �� ����������������
                                          � ������� ������ �����������
                                          ������                      */
#define PLOTERRCASEREGEXIST  PLOTERR+218 /* ������� ��� ������������-
                                            ���� � ������� ������
                                            ����������� ������        */
#define PLOTERRRECNOTFOUND  PLOTERR+219 /* ��� ����� ������ � ������� */
#define PLOTERRCASEEMPTY    PLOTERR+220 /* ��� ��������� �� �������   */
#define PLOTERRCASESELECTSTRSIZE  PLOTERR+221 /* ������������ ������
                                                 ������ ������� ����-
                                                 �� ��������� ������� */
#define PLOTERRCASESELECTOPERATOR  PLOTERR+222 /* ������������ ��������
                                                  ��� ��������� ������
                                                  ������� ������ ������-
                                                  ��� �������         */
#define PLOTERRCASESELECTPARMVALUE  PLOTERR+223 /* ������������ ��������
                                                   ��� ��������� ������
                                                   ������� ������
                                                   ��������� �������  */
#define PLOTERRCASESELECTTOOMANYPARM  PLOTERR+224 /* �������� ������
                                                     ������� ������
                                                     ��������� �������
                                                     ����� ����� ������
                                                     ����             */
#define PLOTERRFIELDTYPEUNKNOWN  PLOTERR+225 /* ����������� ��� ����  */
#define PLOTERRFORMATPROCESSING  PLOTERR+226 /* ���������� ����������
                                                ������� ������ ��
                                                ���������             */
#define PLOTERROBJECTPATH  PLOTERR+227 /* ���� � ������� �� ����� ���
                                          ����� �����������           */

#define PLOTERRSLVVNT     PLOTERR+228 /*  ������ ������������� SLVVNT */
#define PLOTERRFIELDNOTFOUND  PLOTERR+229 /* ��� ������ ���� � �������*/
#define PLOTERRSELECT     PLOTERR+230 /* ��� ������� (���������),
                                         ��������������� ��������
                                         ������                       */
#define PLOTERRCASEITEMPARMID  PLOTERR+231 /* ������� ������
                                              ������������� ���������
                                              �������� �������        */
#define PLOTERRNOCASEITEMPARMS  PLOTERR+232 /* �� ����� ������
                                               ���������� (�������)
                                               �������� �������       */
#define PLOTERRCASEITEMPARMS  PLOTERR+233 /* ��������� ���������
                                             (��������) �� ������ �
                                             ������ ��������� ��������
                                             �������                  */
#define PLOTERRCASEITEMNAME  PLOTERR+234 /* ����������� ������
                                            ������������ ��������
                                            �������                   */
#define PLOTERRCASEITEMOBJMOD  PLOTERR+235 /* � �������� �������
                                              ����������� ������ ���
                                              ������-��������� �������*/
#define PLOTERRCASEITEMOBJPATH  PLOTERR+236 /* � �������� �������
                                               ����������� ����� ����
                                               � �������              */
#define PLOTERRCASEITEMOBJNAME  PLOTERR+237 /* � �������� �������
                                               ����������� ������ ���
                                               (����) �������         */
#define PLOTERRCASEITEMOBJARHTYPE  PLOTERR+238 /* � �������� �������
                                                  ����������� �����
                                                  ��� ���������
                                                  �������             */
#define PLOTERRCASEITEMOBJEXTN  PLOTERR+239 /* � �������� �������
                                               ����������� ������
                                               ���������� �����
                                               �������                */
#define PLOTERRCASEITEMOBJEXTNTYPE  PLOTERR+240 /* � �������� �������
                                                   ����������� �����
                                                   ��� ���������� �����
                                                   �������            */
#define PLOTERRCASEITEMKEYWORDS  PLOTERR+241 /* � �������� �������
                                                ����������� �����
                                                ������ �������� ����  */
#define PLOTERRCHANNELGRIF       PLOTERR+242 /* ����� �������������-
                                                ����� ����� ����� ����
                                                ����� �������������-
                                                ����� ������������
                                                ����������            */
#define PLOTERRDSNOTRUE   PLOTERR+243       /* ������� �� �����       */
#define PLOTERRDSCOMPR    PLOTERR+244       /* ���� ����������������  */
#define PLOTERRDSEXPIRE   PLOTERR+245       /* ���� ���������         */
#define PLOTERRDSNOZKEY   PLOTERR+246       /* �������� ���� �� ������*/
#define PLOTERRDSNOSLV    PLOTERR+247       /* ������� �������� ������
                                               �� ���������           */
#define PLOTERRDSERSIGN   PLOTERR+248       /* �� ������� ���������   */
                                            /* ���������              */
#define PLOTERRDSNOOKEY   PLOTERR+249       /* �������� ���� �� ������*/
#define PLOTERRNOSRDO     PLOTERR+250       /* �� �������� �������-
                                               ���������� �������� �� */
#define PLOTERRNOLISTSLMS  PLOTERR+251      /* �� �������� �������
                                               ������ ��������-
                                               ������������ �������
                                               ���������� �������� �� */
#define PLOTERRNOSLMP  PLOTERR+252          /* �� �������� �������-
                                               ���������� �������������
                                               ��                     */
#define PLOTERRNOOBJECTINSRDO  PLOTERR+253  /* ������ �� ������ �
                                               �������-�����������
                                               �������� ��            */
#define PLOTERRNOSLMS  PLOTERR+254          /* �� �������� �������-
                                               ���������� ��������
                                               ���������� ����������
                                               ������� ��             */
#define PLOTERRNOOBJECTUSERS  PLOTERR+255   /* ������������ ������� ��
                                               ������� � �������-
                                               ����������� �������������
                                               ��                     */

/* ��������� ������ (��������� ����������) ������ INEX                */
#define INEXERRBRWSEVER      INEXERR+1/* ������� ������� ������       */
#define INEXERRHTTPHEADEND   INEXERR+2/* ��� �������� ��������� ����-
                                         ����� HTTP-�������           */
#define INEXERRMETHOD        INEXERR+3/* ��������� � ������� �����
                                         HTTP � ��������� ������ ��
                                         ��������������               */
#define INEXERRNOHTTPVERSION INEXERR+4/* � ��������� ������� �� ���-
                                         ���� ������ �������������
                                         ��������� HTTP               */
#define INEXERRHTTPVERSION   INEXERR+5/* ������ ��������� HTTP, �
                                         ������������ � ������� �����-
                                         ��� ������, � ��������� ��-
                                         ���� �� ��������������       */
#define INEXERRNOPOSTLEN     INEXERR+6/* � �������, ������������ �����
                                         POST, ��� ���������
                                         Content-Length, �������������
                                         ����� ���� �������           */
#define INEXERRBADPOSTLEN    INEXERR+7/* � �������, ������������ �����
                                         POST, ������� �������� ����-
                                         ���� Content-Length          */
#define INEXERRREQUESTTYPE   INEXERR+8/* ������� ��� �������          */
#define INEXERRNOCGICLASS    INEXERR+9/* �� ������ ����� CGI-�������� */
#define INEXERRCGICLASS      INEXERR+10/* ������� ����� CGI-��������  */
#define INEXERRNOCGIDLLPLACE INEXERR+11/* �� ������� ��������������
                                          CGI-�������� ������ DLL     */
#define INEXERRCGIDLLPLACE   INEXERR+12/* ������� ������� �����������-
                                          ��� CGI-�������� ������ DLL */
#define INEXERRNOCGIDLLMODUL INEXERR+13/* �� ������� ���������� ��� ��
                                          ����, � �� �������� ��������
                                          CGI-�������� ������ DLL     */
#define INEXERRCGIDLLMODUL   INEXERR+14/* ����������� ������� ����-
                                          ������ ��� ������, � �� ��-
                                          ������ �������� CGI-��������
                                          ������ DLL                  */
#define INEXERRNOCGIDLLNAME  INEXERR+15/* �� ������� ��� CGI-��������
                                          ������ DLL                  */
#define INEXERRCGIDLLNAME    INEXERR+16/* ����������� ������� ���
                                          CGI-�������� ������ DLL     */
#define INEXERRNOCGIDLLFUNC  INEXERR+17/* �� ������� ��� �������,
                                          ���������� �� CGI-��������
                                          ������ DLL                  */
#define INEXERRCGIDLLFUNC    INEXERR+18/* ����������� ������� ���
                                          �������, ���������� ��
                                          CGI-�������� ������ DLL     */
#define INEXERRNOCGITASKNAME INEXERR+19/* �� ������� ��� CGI-��������
                                          ������ TASK                 */
#define INEXERRCGITASKNAME   INEXERR+20/* ����������� ������� ���
                                          CGI-�������� ������ TASK    */
#define INEXERRCGIDLLFUNCNF  INEXERR+21/* �� ������� ����� ����� �
                                          ������� CGI-��������
                                          ������ DLL                  */
#define INEXERRDBEREQUEST    INEXERR+22/* ������� �������� ������ DBE */
#define INEXERRDBENOOTYPE    INEXERR+23/* � ������� DBE �� ������ ���
                                          ��������������� �������     */
#define INEXERRDBEOTYPE      INEXERR+24/* � ������� DBE ������� ������
                                          ��� ��������������� ������� */
#define INEXERRDBENOMDNAME   INEXERR+25/* � ������� DBE �� ������
                                          ������-��������
                                          ��������������� �������     */
#define INEXERRDBEMDNAME     INEXERR+26/* � ������� DBE �����������
                                          ������ ������-��������
                                          ��������������� �������     */
#define INEXERRDBENODBNAME   INEXERR+27/* � ������� DBE �� �������
                                          ��� ��                      */
#define INEXERRDBEDBNAME     INEXERR+28/* � ������� DBE �����������
                                          ������� ��� ��              */
#define INEXERRDBENONDNAME   INEXERR+29/* � ������� DBE �� �������
                                          ��� ��                      */
#define INEXERRDBENDNAME     INEXERR+30/* � ������� DBE �����������
                                          ������� ��� ��              */
#define INEXERRDBENOONAME    INEXERR+31/* � ������� DBE �� �������
                                          ��� ��������������� ������� */
#define INEXERRDBEONAME      INEXERR+32/* � ������� DBE �����������
                                          ������� ��� ���������������
                                          �������                     */
#define INEXERRDBENOODATE    INEXERR+33/* � ������� DBE �� �������
                                          �������� ��������� "DATE"
                                          (���� ���������������
                                          �������)                    */
#define INEXERRDBEODATE      INEXERR+34/* � ������� DBE �����������
                                          ������� ���� ���������������
                                          �������                     */
#define INEXERRDBENOOSEGM    INEXERR+35/* � ������� DBE �� �������
                                          �������� ��������� "SEGM"
                                          (�������� ���������������
                                          �������)                    */
#define INEXERRDBEOSEGM      INEXERR+36/* � ������� DBE ������� �����
                                          ������� ���������������
                                          �������                     */
#define INEXERRNOSERVICECLASS INEXERR+37/* �� ������ ����� �������    */
#define INEXERRSERVICECLASS  INEXERR+38/* ����������� ������ �����
                                          �������                     */

/* ��������� ������ (��������� ����������) ������ MOCT                */
#define MOCTERRERRSYSTEM  MOCTERR+1 /* ��������� ������ ��� ��������  */
#define MOCTERRBREAKMSG   MOCTERR+2 /* ������ ��� ������ � ����������
                                       - ����� � ����������           */
#define MOCTERRTRANSMIT   MOCTERR+3 /* ������ ����� ��� ��������      */
#define MOCTERRSEVER      MOCTERR+4 /* �� ����� ������ ������ ������-
                                       ��� ������������ �������� ���  */
#define MOCTERRSTOPWORK   MOCTERR+5 /* ������� ������ � ����������   */
#define MOCTERRNOCGIFUNC  MOCTERR+6 /* �� ���������� ������� ������
                                       ��������-����������� ���
                                       ���������� ������� ������      */

/* ��������� ������ (��������� ����������) ������ USER                */
#define USERERRBREAK      USERERR+1 /* �������� �������� �������������*/

/* ��������� ������ (��������� ����������) ������ CASE                */
#define CASEERRSYNTAX     CASEERR+1 /* ����������� ���������� ��������-
                                       ������ ������ ������ �������
                                       ������ ��������� �������       */
/*====================================================================*/
/*=========== � � � � � � � � � � �  � � � � � � (�����) =============*/
/*====================================================================*/

/*====================================================================*/
/*=================== ��������� ��� FMD (������) =====================*/
/*====================================================================*/
/* ���� ��������                                                      */
/* ������� ��������� ������� ������� ����������� ������.              */
/* ����������� ����� �������������� �������                           */
/*                                                                    */
/* ���� �������� ������� ��������� ������� �������                    */
/*                                                                    */
/* ������ � �������������� ��������� � ����������� ������             */
/* ��� �������������� �������� ������������� �������                  */
#define PLOT_FMDACSVU 1  /* ������ �� ���� ��                         */
#define PLOT_FMDACSLM 2  /* ������ �� ���� ��                         */
/* �������� ��� ����������� ��� ������ ���������� �������             */
#define PLOT_FMDDMAKE 3  /* ��������                                  */
#define PLOT_FMDDCHAT 4  /* ��������� �����                           */
#define PLOT_FMDDCHUT 5  /* ��������� ����                            */
#define PLOT_FMDDSETV 6  /* �����������                               */
#define PLOT_FMDDRSTV 7  /* ������ ����                               */
#define PLOT_FMDDCRD  8  /* ���������� ��                             */
#define PLOT_FMDDREAD 9  /* ������                                    */
#define PLOT_FMDDCHNG 10 /* ���������                                 */
#define PLOT_FMDDDELT 11 /* ��������                                  */
/* �������� ���������� (������ � ������� �����)                       */
#define PLOT_FMDSNDS  12 /* �������� ���������                        */
#define PLOT_FMDSNDD  13 /* �������� ���������                        */
#define PLOT_FMDSNDSA 14 /* �������� ��������� ��������� ��������� "A"*/
#define PLOT_FMDSNDSB 15 /* �������� ��������� ��������� ��������� "B"*/
#define PLOT_FMDSNDSC 16 /* �������� ��������� ��������� ��������� "C"*/
#define PLOT_FMDSNDDA 17 /* �������� ��������� ��������� ��������� "A"*/
#define PLOT_FMDSNDDB 18 /* �������� ��������� ��������� ��������� "B"*/
#define PLOT_FMDSNDDC 19 /* �������� ��������� ��������� ��������� "C"*/
/* �������� (��������) ����������                                     */
#define PLOT_FMDUNLD  20 /* ��������                                  */
#define PLOT_FMDLOAD  21 /* ��������                                  */
/* ������ ����������                                                  */
#define PLOT_FMDPRINT 22 /* ������ ����������                         */
/* ���������� �������������� ��������� �� ���������� �������          */
#define PLOT_FMDSTRT  23 /* ������ �����                              */
#define PLOT_FMDSTPT  24 /* ���������� �����                          */
#define PLOT_FMDSYSJR 25 /* �������� �������� ��                      */
#define PLOT_FMDJRVU  26 /* �������� �������� ��                      */
#define PLOT_FMDMEM   27 /* �������� ��� ������� ��                   */
#define PLOT_FMDGARD  28 /* ���������� ��������������� ����������     */
#define PLOT_FMDACIPP 29 /* ������ � ip-������                        */
/* �������� ��� ������ (�����, ���������) ������������ �������        */
#define PLOT_FMDFSVL  30 /* ��������� ������ �����                    */
#define PLOT_FMDFSLL  31 /* ������ ���������� �����                   */
#define PLOT_FMDFSLM  32 /* �������� ���������                        */
#define PLOT_FMDFSFL  33 /* ������ ���������� ���������               */
#define PLOT_FMDFSLN  34 /* �������������� (��� �����������) ���������*/
#define PLOT_FMDFSLD  35 /* �������� ���������                        */
#define PLOT_FMDFSFM  36 /* �������� ������                           */
#define PLOT_FMDFSFR  37 /* ������ ������                             */
#define PLOT_FMDFSFW  38 /* ����������� ������                        */
#define PLOT_FMDFSFN  39 /* �������������� (��� �����������) ������   */
#define PLOT_FMDFSFD  40 /* �������� ������                           */
#define PLOT_FMDFSRUN 41 /* ������������� ����������� ������          */
#define PLOT_FMDFSGCH 42 /* ��������� ����� ����� (��������)          */
#define PLOT_FMDFSECP 43 /* ��������� ������� �������� ����������     */
/*                                                                    */
/* ����������� ���������� �������� ���� ��������                      */
#define PLOT_FMDLAST  PLOT_FMDFSECP
/*                                                                    */
/* ����� �������������� �������                                       */
#define PLOT_FMDSUSOK 1  /* ������������� ������������ - �����        */
#define PLOT_FMDSUSNO 2  /* ������������� ������������ - �����        */
#define PLOT_FMDSTPUS 3  /* ��������� ������ ������������             */
/* ����������� �������� ��� ����������� ��� ������ ��                 */
#define PLOT_FMDCRTOK 4  /* �������� - �����                          */
#define PLOT_FMDCRTNO 5  /* �������� - �����                          */
#define PLOT_FMDCHKOK 6  /* ��������� ����� - �����                   */
#define PLOT_FMDCHKNO 7  /* ��������� ����� - �����                   */
#define PLOT_FMDCHTOK 8  /* ��������� ���� - �����                    */
#define PLOT_FMDCHTNO 9  /* ��������� ���� - �����                    */
#define PLOT_FMDVISOK 10 /* ����������� - �����                       */
#define PLOT_FMDVISNO 11 /* ����������� - �����                       */
#define PLOT_FMDCLVOK 12 /* ������ ���� - �����                       */
#define PLOT_FMDCLVNO 13 /* ������ ���� - �����                       */
#define PLOT_FMDCRDOK 14 /* ���������� �� - �����                     */
#define PLOT_FMDCRDNO 15 /* ���������� �� - �����                     */
#define PLOT_FMDREDOK 16 /* ������ - �����                            */
#define PLOT_FMDREDNO 17 /* ������ - �����                            */
#define PLOT_FMDWRTOK 18 /* ����������� - �����                       */
#define PLOT_FMDWRTNO 19 /* ����������� - �����                       */
#define PLOT_FMDDELOK 20 /* �������� - �����                          */
#define PLOT_FMDDELNO 21 /* �������� - �����                          */
/* ����������� �������� ����������                                    */
#define PLOT_FMDSNDOK 22 /* �������� ���������� - �����               */
#define PLOT_FMDSNDNO 23 /* �������� ���������� - �����               */
/* ����������� ������� � ������� �����������                          */
#define PLOT_FMDUNLOK 24 /* �������� ���������� - �����               */
#define PLOT_FMDUNLNO 25 /* �������� ���������� - �����               */
#define PLOT_FMDLODOK 26 /* �������� ���������� - �����               */
#define PLOT_FMDLODNO 27 /* �������� ���������� - �����               */
#define PLOT_FMDPRTOK 28 /* ������ ���������� - �����                 */
#define PLOT_FMDPRTNO 29 /* ������ ���������� - �����                 */
/* ����������� ���������� �������������� ���������                    */
#define PLOT_FMDSTTOK 30 /* ������ ����� - �����                      */
#define PLOT_FMDSTTNO 31 /* ������ ����� - �����                      */
#define PLOT_FMDSTPOK 32 /* ���������� ����� - �����                  */
#define PLOT_FMDSTPNO 33 /* ���������� ����� - �����                  */
#define PLOT_FMDRJROK 34 /* �������� �������� �� - �����              */
#define PLOT_FMDRJRNO 35 /* �������� �������� �� - �����              */
#define PLOT_FMDRJUOK 36 /* �������� �������� �� - �����              */
#define PLOT_FMDRJUNO 37 /* �������� �������� �� - �����              */
#define PLOT_FMDMEMOK 38 /* �������� ��� ������� �� - �����           */
#define PLOT_FMDMEMNO 39 /* �������� ��� ������� �� - �����           */
#define PLOT_FMDGRDOK 40 /* ���������� ������. ���������� - �����     */
#define PLOT_FMDGRDNO 41 /* ���������� ������. ���������� - �����     */
#define PLOT_FMDIPPOK 42 /* ������ � ip-������ - �����                */
#define PLOT_FMDIPPNO 43 /* ������ � ip-������ - �����                */
/* ����������� �������� ��� ������� (������, �����������)             */
/* ������������ �������                                               */
#define PLOT_FMDFVLOK 44 /* ��������� ������ ��������� ����� - �����  */
#define PLOT_FMDFVLNO 45 /* ��������� ������ ��������� ����� - �����  */
#define PLOT_FMDFLLOK 46 /* ������ ���������� ����� - �����           */
#define PLOT_FMDFLLNO 47 /* ������ ���������� ����� - �����           */
#define PLOT_FMDFLMOK 48 /* �������� ��������� - �����                */
#define PLOT_FMDFLMNO 49 /* �������� ��������� - �����                */
#define PLOT_FMDFFLOK 50 /* ������ ���������� ��������� - �����       */
#define PLOT_FMDFFLNO 51 /* ������ ���������� ��������� - �����       */
#define PLOT_FMDFLNOK 52 /* ������-�(��� �������-�)��������� - �����  */
#define PLOT_FMDFLNNO 53 /* ������-�(��� �������-�)��������� - �����  */
#define PLOT_FMDFLDOK 54 /* �������� ��������� - �����                */
#define PLOT_FMDFLDNO 55 /* �������� ��������� - �����                */
#define PLOT_FMDFFMOK 56 /* �������� ������ - �����                   */
#define PLOT_FMDFFMNO 57 /* �������� ������ - �����                   */
#define PLOT_FMDFFROK 58 /* ������ ������ - �����                     */
#define PLOT_FMDFFRNO 59 /* ������ ������ - �����                     */
#define PLOT_FMDFFWOK 60 /* ����������� ������ - �����                */
#define PLOT_FMDFFWNO 61 /* ����������� ������ - �����                */
#define PLOT_FMDFFNOK 62 /* ������-� (��� �������-�) ������ - �����   */
#define PLOT_FMDFFNNO 63 /* ������-� (��� �������-�) ������ - �����   */
#define PLOT_FMDFFDOK 64 /* �������� ������ - �����                   */
#define PLOT_FMDFFDNO 65 /* �������� ������ - �����                   */
#define PLOT_FMDFRNOK 66 /* ���������-� ����������� ������ - �����    */
#define PLOT_FMDFRNNO 67 /* ���������-� ����������� ������ - �����    */
#define PLOT_FMDFGCOK 68 /* ��������� ����� ����� (��������) - �����  */
#define PLOT_FMDFGCNO 69 /* ��������� ����� ����� (��������) - �����  */
#define PLOT_FMDFEPOK 70 /* ��������� ������� ��-�� ���������� - �����*/
#define PLOT_FMDFEPNO 71 /* ��������� ������� ��-�� ���������� - �����*/
/*====================================================================*/
/*=================== ��������� ��� FMD (�����) ======================*/
/*====================================================================*/

/*====================================================================*/
/*=================== ��������� ��� ���� (������) ====================*/
/*====================================================================*/
/* ���� �������� ���������� ����                                      */
#define PLOT_SZIUSRSTART   101 /* ������ �� ������ ������             */
#define PLOT_SZIUSREND     102 /* ��������� �� ��������� ������       */
#define PLOT_SZIDATACHANG  103 /* ������ �� ������ ������� ������
                                  ������������, ����������� �������
                                  ������� ������� ����� ��������
                                  �������������                       */
#define PLOT_SZIMESSAGE    104 /* ��������� � ���                     */
#define PLOT_SZIUSERADD    105 /* ���������� ������ ������������      */
#define PLOT_SZIUSERDEL    106 /* �������� ������������               */
#define PLOT_SZICHNGPASS   107 /* ����� ������� ��� ���� �������������*/
#define PLOT_SZICHNGPASSUS 108 /* ����� ������� ���������� �����-��   */
#define PLOT_SZITELLPASS   109 /* ������ ������ ������������          */
#define PLOT_SZIUSERCHANGE 110 /* ���������-�� ������� ������ �����-��*/
#define PLOT_SZIRANGRD     111 /* ������ �������� �����               */
#define PLOT_SZIRANGWR     112 /* ������������� �������� �����        */
#define PLOT_SZIRANGMK     113 /* �������� ������ �����               */
#define PLOT_SZIRANGDL     114 /* �������� �����                      */
#define PLOT_SZITRASTDRD   115 /* ������ ������� ���������� �������
                                  ��� ��������� ��                    */
#define PLOT_SZITRASTDWR   116 /* ������������� ������� ����������
                                  ������� ��� ��������� ��            */
#define PLOT_SZITRASTDDL   117 /* �������� ������� ���������� �������
                                  ��� ��������� ��                    */
#define PLOT_SZITRASTGRD   118 /* ������ ������� ���������� �������
                                  ��� ��������� ��                    */
#define PLOT_SZITRASTGWR   119 /* ������������� ������� ����������
                                  ������� ��� ��������� ��            */
#define PLOT_SZITRASTGDL   120 /* �������� ������� ���������� �������
                                  ��� ��������� ��                    */
#define PLOT_SZIADDRRD     121 /* ������ ������� ����������� ���������*/
#define PLOT_SZIADDRWR     122 /* ������������� ������� �����������
                                  ���������                           */
#define PLOT_SZIADDRDL     123 /* �������� ������� �����������
                                  ���������                           */
#define PLOT_SZIPTHURD     124 /* ������ ������� ��, ���������� � ����-
                                  ���� �������� (��������)            */
#define PLOT_SZIPTHUWR     125 /* ������������� ������� ��, ����������
                                  � �������� �������� (��������)      */
#define PLOT_SZIPTHUDL     126 /* �������� ������� ��� �������� ������-
                                  �� (��������) ������ � ��������
                                  ���������� ��                       */
#define PLOT_SZIPRINTRD    130 /* ������ ������� ��, ���������� �
                                  ��������� ����������                */
#define PLOT_SZIPRINTWR    131 /* ������������� ������� ��, ����������
                                  � ��������� ����������              */
#define PLOT_SZIPRINTDL    132 /* �������� ��������� ����������
                                  (������ � �������� ���������� ��)   */
#define PLOT_SZIPROPIOR    137 /* ������ ������� ������ (�������)
                                  �������������� ��������             */
#define PLOT_SZIGETMDM     141 /* ������ �� ������� ������� �����������
                                  ������                              */
#define PLOT_SZICHNGDATLM  142 /* ������ �� ������ (����������) ������
                                  � ��                                */
#define PLOT_SZIMAKECRC    143 /* ������������ ��������� �������
                                  ����������� �� � ��                 */
#define PLOT_SZITESTCRC    144 /* ��������� ����������� �� � �� */
#define PLOT_SZICLEARCRC   145 /* �������� ������ �������� �����������
                                  �� � ��                             */
#define PLOT_SZIREGSYST    146 /* ���������� ����������� ����������� �
                                  �����                               */
#define PLOT_SZITESTSYST   147 /* ����������� ������ �����������
                                  ������� ������ ����������           */
#define PLOT_SZILMRANGEWR  148 /* ���������� ����� �� �� ���������    */
#define PLOT_SZILMRANGEDL  149 /* �������� ����� �� �� ���������      */
#define PLOT_SZIPORTIPADD  150 /* ���������� ip-����� � �������
                                  ����������� ip-������               */
#define PLOT_SZIPORTIPDEL  151 /* �������� ip-����� �� �������
                                  ����������� ip-������               */
#define PLOT_SZIPORTLMADD  152 /* ��������� ������ �� � ip-�����      */
#define PLOT_SZIPORTLMDEL  153 /* ��������� ������ �� � ip-�����      */
#define PLOT_SZIREPORTPASS 154 /* ��������� ������ � ��������         */
                               /* �������������                       */
#define PLOT_SZIMASSPASS   155 /* ��������� ������� �� N              */
                               /* L-������� �������                   */
/*                                                                    */
/*====================================================================*/
/*=================== ��������� ��� ���� (�����) =====================*/
/*====================================================================*/

/*====================================================================*/
/*=================== ��������� ��� MDM (������) =====================*/
/*====================================================================*/
/*                                                                    */
/* ������� ������� ����������� ������                                 */
/*                                                                    */
/* �����                                                              */
#define PLOT_MDMLNGHT 0   /* ����� �������                            */
/*                           - ������ (8)                             */
#define PLOT_MDMCHKSM 12  /* ����������� �����                        */
#define PLOT_MDMRGDEF 16  /* ���� �� ���������                        */
/*                           - ������ (2)                             */
#define PLOT_MDMLZGM  20  /* ����� ���������                          */
#define PLOT_MDMADMIN 24  /* ��� ������ �������������� ��� (2)        */
#define PLOT_MDMSCUF  26  /* ��� ������ �������������� ���� (2)       */
#define PLOT_MDMUSER  28  /* �������� ������� �������������           */
#define PLOT_MDMRANG  32  /* �������� ������� ������                  */
#define PLOT_MDMTRSTD 36  /* �������� ������� ���������� �������      */
#define PLOT_MDMTRSTG 40  /* �������� ������� ���������� �������      */
#define PLOT_MDMADDRS 44  /* �������� ������� ����������� ���������   */
#define PLOT_MDMRMVBL 48  /* �������� ������� ��������� � ���������   */
/*                           ��� �������� � �������� ����������       */
#define PLOT_MDMDPRNT 52  /* �������� ������� ��������� ������        */
#define PLOT_MDMACIPP 56  /* �������� ������� ����������� ip-������   */
#define PLOT_MDMRDIPP 60  /* �������� ������� �� � ip-������          */
#define PLOT_MDMSHREG 64  /* �������� ����� �������������� �������    */
/* ��������                                                           */
#define PLOT_MDMZLZGM 68  /* ����� ���������                          */
/*                                                                    */
/* ��������� ������� (����� ��� ���� ������, �������� � �������)      */
#define PLOT_MDMLZGT  0   /* ����� ���������                          */
#define PLOT_MDMLSTRT 4   /* ����� ������                             */
#define PLOT_MDMNSTRT 8   /* ���������� �����                         */
#define PLOT_MDMLFLD1 12  /* ����� ������� ���� ������                */
#define PLOT_MDMLFLD2 16  /* ����� ������� ���� ������                */
/* ��������                                                           */
#define PLOT_MDMZLZGT 20  /* ����� ���������                          */
/*                                                                    */
/* ������� ������������� � ����������� ���������                      */
/* ��������                                                           */
#define PLOT_MDMZUAF1 2   /* ����� ���� �������                       */
#define PLOT_MDMZUAF2 2   /* ����� ���� ������                        */
#define PLOT_MDMZUSRM 600 /* ������������ ���������� �������������    */
/*                                                                    */
/* ������� ������ ���������� �������������                            */
/* ��������                                                           */
#define PLOT_MDMZLNRG 2   /* ����� ������ �����                       */
#define PLOT_MDMZLZRG 34  /* ����� �������� ����� (������� �����)     */
/*                           (����)                                   */
/* ������� ���������� � ���������� �������                            */
/* ��������                                                           */
#define PLOT_MDMZLBCP 1   /* ����� �������� (�������� � ��/           */
/*                           ���-� �������������� ���������/          */
/*                           �������� � �������)                      */
#define PLOT_MDMZLTRM 75  /* ����� ������� ������� (������� �����)    */
/*                           (����)                                   */
/* ������� ��������� � ��������� ��� �������� � �������� ����������   */
/* ������� ��������� ������                                           */
/* ��������                                                           */
#define PLOT_MDMZLIOP 1   /* ����� �������� (��������/�������) �      */
/*                           ������� ��������� � ��������� ���        */
/*                           �������� � �������� ����������           */
#define PLOT_MDMZLPUN 256 /* ����� ����� �������� (����������) ���    */
/*                           ����� ����� ���������� ������            */
/*                                                                    */
/* ������� ip-������, ����������� ��� �������������                   */
/* ��������                                                           */
#define PLOT_MDMZACPN 2   /* ����� ������ ip-�����                    */
/*                                                                    */
/* ������� �� � ip-������                                             */
/* ��������                                                           */
#define PLOT_MDMZRDPM PLOT_MDMZUAF2 /* ����� ���� ������              */
/*                                                                    */
/* ����� �������������� �������                                       */
#define PLOT_MDMZLREG 9   /* ����� ������� ����� ��������������       */
/*                           ������� (����)                           */
/* �������� �������������� ������                                     */
#define PLOT_MDMPUSRT 1   /* ������� �������������                    */
#define PLOT_MDMPRANG 2   /* ������� ������                           */
#define PLOT_MDMPTRSD 3   /* ������� ���������� �������               */
#define PLOT_MDMPTRSG 4   /* ������� ���������� �������               */
#define PLOT_MDMPADDR 5   /* ������� ����������� ���������            */
#define PLOT_MDMPIO   6   /* ������� ��������� � ���������            */
/*                           ��� �������� � �������� ����������       */
#define PLOT_MDMPPRT  7   /* ������� ��������� ������                 */
#define PLOT_MDMPAIPP 8   /* ������� ����������� ip-������            */
#define PLOT_MDMPDIPP 9   /* ������� �� � ip-������                   */
#define PLOT_MDMPREG  10  /* ������� �������������� �������           */
/*====================================================================*/
/*=================== ��������� ��� MDM (�����) ======================*/
/*====================================================================*/


/*====================================================================*/
/*=================== ��������� ��� NZR (������) =====================*/
/*====================================================================*/
/* ����������� ������� ���������� ��������                            */
/*                                                                    */
/* ������ � �������������� ��������� � ����������� ������             */
#define PLOT_NZRALLVU 1   /* ������ �� ���� �� ��� ���. �������� ��   */
#define PLOT_NZRALLLM 2   /* ������ �� ���� �� ��� ���. �������� ��   */
/*                                                                    */
/* ������ � ���������� ��� ������ ���������� �������                  */
/* ������ � ���������� ���� ������ ������ ����������� ������          */
#define PLOT_NZRDMYCK 3   /* ��������� �����                          */
#define PLOT_NZRDMYCA 4   /* ��������� ����                           */
#define PLOT_NZRDMYSV 5   /* �����������                              */
#define PLOT_NZRDMYRV 6   /* ������ ����                              */
/* ���������� �������������� ������� � ����������                     */
#define PLOT_NZRDMYC0 7   /* ���������� �� � ����������� ����������   */
#define PLOT_NZRDMYC1 8   /* ���������� �� � ���������� ���           */
#define PLOT_NZRDMYC2 9   /* ���������� �� � ��������� ����������     */
#define PLOT_NZRDMYC3 10  /* ���������� �� � ���. ��������� ����������*/
/* �������� ����������                                                */
#define PLOT_NZRDMYM0 11  /* �������� ����������� ����������          */
#define PLOT_NZRDMYM1 12  /* �������� ���������� ���                  */
#define PLOT_NZRDMYM2 13  /* �������� ��������� ����������            */
#define PLOT_NZRDMYM3 14  /* �������� ���. ��������� ����������       */
/* ������ ����������                                                  */
#define PLOT_NZRDMYR0 15  /* ������ ����������� ����������            */
#define PLOT_NZRDMYR1 16  /* ������ ���������� ���                    */
#define PLOT_NZRDMYR2 17  /* ������ ��������� ����������              */
#define PLOT_NZRDMYR3 18  /* ������ ���. ��������� ����������         */
/* ��������� ����������                                               */
#define PLOT_NZRDMYW0 19  /* ����������� ����������� ����������       */
#define PLOT_NZRDMYW1 20  /* ����������� ���������� ���               */
#define PLOT_NZRDMYW2 21  /* ����������� ��������� ����������         */
#define PLOT_NZRDMYW3 22  /* ����������� ���. ��������� ����������    */
/* �������� ����������                                                */
#define PLOT_NZRDMYD0 23  /* �������� ����������� ����������          */
#define PLOT_NZRDMYD1 24  /* �������� ���������� ���                  */
#define PLOT_NZRDMYD2 25  /* �������� ��������� ����������            */
#define PLOT_NZRDMYD3 26  /* �������� ���. ��������� ����������       */
/* ���������� �������� ���������� ��, ������, ����������� �           */
/* �������� ���������� ���� ������ ������ ����������� ������ ���      */
/* ���. �������� �� ������������� �������                             */
#define PLOT_NZRDMYFA 27
/*                                                                    */
/* ������ � ���������� ���� ������ ������� ����������� ������         */
#define PLOT_NZRDOTCK 28  /* ��������� �����                          */
#define PLOT_NZRDOTCA 29  /* ��������� ����                           */
#define PLOT_NZRDOTSV 30  /* �����������                              */
#define PLOT_NZRDOTRV 31  /* ������ ����                              */
/* ���������� �������������� ������� � ����������                     */
#define PLOT_NZRDOTC0 32  /* ���������� �� � ����������� ����������   */
#define PLOT_NZRDOTC1 33  /* ���������� �� � ���������� ���           */
#define PLOT_NZRDOTC2 34  /* ���������� �� � ��������� ����������     */
#define PLOT_NZRDOTC3 35  /* ���������� �� � ���. ��������� ����������*/
/* �������� ����������                                                */
#define PLOT_NZRDOTM0 36  /* �������� ����������� ����������          */
#define PLOT_NZRDOTM1 37  /* �������� ���������� ���                  */
#define PLOT_NZRDOTM2 38  /* �������� ��������� ����������            */
#define PLOT_NZRDOTM3 39  /* �������� ���. ��������� ����������       */
/* ������ ����������                                                  */
#define PLOT_NZRDOTR0 40  /* ������ ����������� ����������            */
#define PLOT_NZRDOTR1 41  /* ������ ���������� ���                    */
#define PLOT_NZRDOTR2 42  /* ������ ��������� ����������              */
#define PLOT_NZRDOTR3 43  /* ������ ���. ��������� ����������         */
/* ��������� ����������                                               */
#define PLOT_NZRDOTW0 44  /* ����������� ����������� ����������       */
#define PLOT_NZRDOTW1 45  /* ����������� ���������� ���               */
#define PLOT_NZRDOTW2 46  /* ����������� ��������� ����������         */
#define PLOT_NZRDOTW3 47  /* ����������� ���. ��������� ����������    */
/* �������� ����������                                                */
#define PLOT_NZRDOTD0 48  /* �������� ����������� ����������          */
#define PLOT_NZRDOTD1 49  /* �������� ���������� ���                  */
#define PLOT_NZRDOTD2 50  /* �������� ��������� ����������            */
#define PLOT_NZRDOTD3 51  /* �������� ���. ��������� ����������       */
/* ���������� �������� ���������� ��, ������, ����������� �           */
/* �������� ���������� ���� ������ ������� ����������� ������ ���     */
/* ���. �������� �� ������������� �������                             */
#define PLOT_NZRDOTFA 52
/*                                                                    */
/* ������ � ���������� ��� ������ ����� ���������� ������� ����� ���  */
#define PLOT_NZRDNTCK 53  /* ��������� �����                          */
#define PLOT_NZRDNTCA 54  /* ��������� ����                           */
#define PLOT_NZRDNTSV 55  /* �����������                              */
#define PLOT_NZRDNTRV 56  /* ������ ����                              */
/* ���������� �������������� ������� � ����������                     */
#define PLOT_NZRDNTC0 57  /* ���������� �� � ����������� ����������   */
#define PLOT_NZRDNTC1 58  /* ���������� �� � ���������� ���           */
#define PLOT_NZRDNTC2 59  /* ���������� �� � ��������� ����������     */
#define PLOT_NZRDNTC3 60  /* ���������� �� � ���. ��������� ����������*/
/* �������� ����������                                                */
#define PLOT_NZRDNTM0 61  /* �������� ����������� ����������          */
#define PLOT_NZRDNTM1 62  /* �������� ���������� ���                  */
#define PLOT_NZRDNTM2 63  /* �������� ��������� ����������            */
#define PLOT_NZRDNTM3 64  /* �������� ���. ��������� ����������       */
/* ������ ����������                                                  */
#define PLOT_NZRDNTR0 65  /* ������ ����������� ����������            */
#define PLOT_NZRDNTR1 66  /* ������ ���������� ���                    */
#define PLOT_NZRDNTR2 67  /* ������ ��������� ����������              */
#define PLOT_NZRDNTR3 68  /* ������ ���. ��������� ����������         */
/* ��������� ����������                                               */
#define PLOT_NZRDNTW0 69  /* ����������� ����������� ����������       */
#define PLOT_NZRDNTW1 70  /* ����������� ���������� ���               */
#define PLOT_NZRDNTW2 71  /* ����������� ��������� ����������         */
#define PLOT_NZRDNTW3 72  /* ����������� ���. ��������� ����������    */
/* �������� ����������                                                */
#define PLOT_NZRDNTD0 73  /* �������� ����������� ����������          */
#define PLOT_NZRDNTD1 74  /* �������� ���������� ���                  */
#define PLOT_NZRDNTD2 75  /* �������� ��������� ����������            */
#define PLOT_NZRDNTD3 76  /* �������� ���. ��������� ����������       */
/* ���������� �������� ���������� ��, ������, ����������� �           */
/* �������� ���������� ��� ������ ����� ���������� ������� �����      */
/* ��� ��� ���. �������� �� ������������� �������                     */
#define PLOT_NZRDNTFA 77
/*                                                                    */
/* ������ � ���������� ��� ������ ����� ���������� ������� �������    */
#define PLOT_NZRDOBCK 78  /* ��������� �����                          */
#define PLOT_NZRDOBCA 79  /* ��������� ����                           */
#define PLOT_NZRDOBSV 80  /* �����������                              */
#define PLOT_NZRDOBRV 81  /* ������ ����                              */
/* ���������� �������������� ������� � ����������                     */
#define PLOT_NZRDOBC0 82  /* ���������� �� � ����������� ����������   */
#define PLOT_NZRDOBC1 83  /* ���������� �� � ���������� ���           */
#define PLOT_NZRDOBC2 84  /* ���������� �� � ��������� ����������     */
#define PLOT_NZRDOBC3 85  /* ���������� �� � ���. ��������� ����������*/
/* �������� ����������                                                */
#define PLOT_NZRDOBM0 86  /* �������� ����������� ����������          */
#define PLOT_NZRDOBM1 87  /* �������� ���������� ���                  */
#define PLOT_NZRDOBM2 88  /* �������� ��������� ����������            */
#define PLOT_NZRDOBM3 89  /* �������� ���. ��������� ����������       */
/* ������ ����������                                                  */
#define PLOT_NZRDOBR0 90  /* ������ ����������� ����������            */
#define PLOT_NZRDOBR1 91  /* ������ ���������� ���                    */
#define PLOT_NZRDOBR2 92  /* ������ ��������� ����������              */
#define PLOT_NZRDOBR3 93  /* ������ ���. ��������� ����������         */
/* ��������� ����������                                               */
#define PLOT_NZRDOBW0 94  /* ����������� ����������� ����������       */
#define PLOT_NZRDOBW1 95  /* ����������� ���������� ���               */
#define PLOT_NZRDOBW2 96  /* ����������� ��������� ����������         */
#define PLOT_NZRDOBW3 97  /* ����������� ���. ��������� ����������    */
/* �������� ����������                                                */
#define PLOT_NZRDOBD0 98  /* �������� ����������� ����������          */
#define PLOT_NZRDOBD1 99  /* �������� ���������� ���                  */
#define PLOT_NZRDOBD2 100 /* �������� ��������� ����������            */
#define PLOT_NZRDOBD3 101 /* �������� ���. ��������� ����������       */
/* ���������� �������� ���������� ��, ������, ����������� �           */
/* �������� ���������� ��� ������ ����� ���������� ������� �������    */
/* ��� ���. �������� �� ������������� �������                         */
#define PLOT_NZRDOBFA 102
/*                                                                    */
/* �������� ���������� (������ � ������� �����)                       */
/* �������� ���������� �� ������ �������                              */
#define PLOT_NZRSEXTS 103 /* ��������� �� ������ �������              */
#define PLOT_NZRSEXTD 104 /* ���������� �� ������ �������             */
#define PLOT_NZRSEXTR 105 /* ���-��� � ����� ����� (�� ������ �������)*/
#define PLOT_NZRSEXT0 106 /* ����������� ���������� �� ������ ������� */
#define PLOT_NZRSEXT1 107 /* ���������� ��� �� ������ �������         */
#define PLOT_NZRSEXT2 108 /* ��������� ���������� �� ������ �������   */
#define PLOT_NZRSEXT3 109 /* ���.��������� ���-��� �� ������ �������  */
#define PLOT_NZRSEXTA 110 /* ���. ���. ��������� "A" �� ��. �������   */
#define PLOT_NZRSEXTB 111 /* ���. ���. ��������� "B" �� ��. �������   */
#define PLOT_NZRSEXTC 112 /* ���. ���. ��������� "C" �� ��. �������   */
/* �������� ���������� ��������� ������ �������                       */
#define PLOT_NZRSOBS  113 /* ��������� ��������� �������              */
#define PLOT_NZRSOBD  114 /* ���������� ��������� �������             */
#define PLOT_NZRSOBA  115 /* ���������� � ����� ����� (������ �������)*/
#define PLOT_NZRSOB0  116 /* ����������� ���������� ��������� ������� */
#define PLOT_NZRSOB1  117 /* ���������� ��� ��������� �������         */
#define PLOT_NZRSOB2  118 /* ��������� ���������� ��������� �������   */
#define PLOT_NZRSOB3  119 /* ���.��������� ���-��� ��������� �������  */
/* �������� ���������� ��������� ����� ���                            */
#define PLOT_NZRSNTS  120 /* ��������� ��������� ����� ���            */
#define PLOT_NZRSNTD  121 /* ���������� ��������� ����� ���           */
#define PLOT_NZRSNTA  122 /* ���-��� � ����� ����� (������ ����� ���) */
#define PLOT_NZRSNT0  123 /* ����������� ���-��� ��������� ����� ���  */
#define PLOT_NZRSNT1  124 /* ���������� ��� ��������� ����� ���       */
#define PLOT_NZRSNT2  125 /* ��������� ���������� ��������� ����� ��� */
#define PLOT_NZRSNT3  126 /* ���.��������� ���-��� ��������� ����� ���*/
/*                                                                    */
/* ������ � ����������� ���������                                     */
/* �������� ����������                                                */
#define PLOT_NZRUNLD0 127 /* �������� ����������� ����������          */
#define PLOT_NZRUNLD1 128 /* �������� ���������� ���                  */
#define PLOT_NZRUNLD2 129 /* �������� ��������� ����������            */
#define PLOT_NZRUNLD3 130 /* �������� ���.��������� ����������        */
/* �������� ����������                                                */
#define PLOT_NZRLOAD0 131 /* �������� ����������� ����������          */
#define PLOT_NZRLOAD1 132 /* �������� ���������� ���                  */
#define PLOT_NZRLOAD2 133 /* �������� ��������� ����������            */
#define PLOT_NZRLOAD3 134 /* �������� ���.��������� ����������        */
/*                                                                    */
/* ������ � ��������� ������                                          */
#define PLOT_NZRPRT0  135 /* ������ ����������� ����������            */
#define PLOT_NZRPRT1  136 /* ������ ���������� ���                    */
#define PLOT_NZRPRT2  137 /* ������ ��������� ����������              */
#define PLOT_NZRPRT3  138 /* ������ ���. ��������� ����������         */
/*                                                                    */
/* ���������� �������������� ��������� �� ���������� �������          */
#define PLOT_NZRJRVU  139 /* �������� �������� ��                     */
/* ���������� �������������� ��������� �� ����� ���������� ������     */
#define PLOT_NZRCMTS  140 /* ������ �����                             */
#define PLOT_NZRCMTP  141 /* ���������� �����                         */
#define PLOT_NZRCMSJR 142 /* �������� �������� ��                     */
#define PLOT_NZRCMMEM 143 /* �������� ��� ������� ��                  */
#define PLOT_NZRCMGRD 144 /* ���������� ��������������� ����������    */
#define PLOT_NZRCMIPP 145 /* ������ � ip-������                       */
/* ���������� �������� �������/���������� �����, ������ � ip-������   */
/* �� ����� ���������� ������ ��� ���. �������� ��                    */
/* ������������� �������                                              */
#define PLOT_NZRCMFA  146
/*                                                                    */
/* ���������� �������������� ��������� �� ������ ���������� ������    */
#define PLOT_NZRCLTS  147 /* ������ �����                             */
#define PLOT_NZRCLTP  148 /* ���������� �����                         */
#define PLOT_NZRCLSJR 149 /* �������� �������� ��                     */
#define PLOT_NZRCLMEM 150 /* �������� ��� ������� ��                  */
#define PLOT_NZRCLGRD 151 /* ���������� ��������������� ����������    */
#define PLOT_NZRCLIPP 152 /* ������ � ip-������                       */
/* ���������� �������� �������/���������� �����, ������ � ip-������   */
/* �� ������ ���������� ������ ��� ���. �������� ��                   */
/* ������������� �������                                              */
#define PLOT_NZRCLFA  153
/*                                                                    */
/* ���������� �������������� ���������                                */
/* �� ����� ���������� ������� ����� ���                              */
#define PLOT_NZRCNTS  154 /* ������ �����                             */
#define PLOT_NZRCNTP  155 /* ���������� �����                         */
#define PLOT_NZRCNSJR 156 /* �������� �������� ��                     */
#define PLOT_NZRCNMEM 157 /* �������� ��� ������� ��                  */
#define PLOT_NZRCNGRD 158 /* ���������� ��������������� ����������    */
#define PLOT_NZRCNIPP 159 /* ������ � ip-������                       */
/* ���������� �������� �������/���������� �����, ������ � ip-������   */
/* �� ����� ���������� ������� ����� ��� ��� ���. �������� ��         */
/* ������������� �������                                              */
#define PLOT_NZRCNFA  160
/*                                                                    */
/* ���������� �������������� ���������                                */
/* �� ����� ���������� ������� �������                                */
#define PLOT_NZRCOTS  161 /* ������ �����                             */
#define PLOT_NZRCOTP  162 /* ���������� �����                         */
#define PLOT_NZRCOSJR 163 /* �������� �������� ��                     */
#define PLOT_NZRCOMEM 164 /* �������� ��� ������� ��                  */
#define PLOT_NZRCOGRD 165 /* ���������� ��������������� ����������    */
#define PLOT_NZRCOIPP 166 /* ������ � ip-������                       */
/* ���������� �������� �������/���������� �����, ������ � ip-������   */
/* �� ����� ���������� ������� ������� ��� ���. �������� ��           */
/* ������������� �������                                              */
#define PLOT_NZRCOFA  167
/* ������ � ������ (�����, ���������) ������������ �������            */
/* ������ � ������ (�����, ���������) ������ ����������� ������       */
#define PLOT_NZRFMVL  168 /* ��������� ������ ��������� �����         */
#define PLOT_NZRFMLL  169 /* ������ ���������� �����                  */
#define PLOT_NZRFMLM  170 /* �������� ���������                       */
#define PLOT_NZRFMFL  171 /* ������ ���������� ���������              */
#define PLOT_NZRFMLN  172 /* ��������������(��� �����������) ���������*/
#define PLOT_NZRFMLD  173 /* �������� ���������                       */
#define PLOT_NZRFMFN  174 /* �������������� (��� �����������) ������  */
#define PLOT_NZRFMGC  175 /* ��������� ����� ����� (��������)         */
#define PLOT_NZRFMRUN 176 /* ������������� ����������� ������         */
/*                                                                    */
#define PLOT_NZRFMFM0 177 /* �������� ����������� ������              */
#define PLOT_NZRFMFM1 178 /* �������� ������ ���                      */
#define PLOT_NZRFMFM2 179 /* �������� ��������� ������                */
#define PLOT_NZRFMFM3 180 /* �������� ���. ��������� ������           */
/*                                                                    */
#define PLOT_NZRFMFR0 181 /* ������ ����������� ������                */
#define PLOT_NZRFMFR1 182 /* ������ ������ ���                        */
#define PLOT_NZRFMFR2 183 /* ������ ��������� ������                  */
#define PLOT_NZRFMFR3 184 /* ������ ���. ��������� ������             */
/*                                                                    */
#define PLOT_NZRFMFW0 185 /* ����������� ����������� ������           */
#define PLOT_NZRFMFW1 186 /* ����������� ������ ���                   */
#define PLOT_NZRFMFW2 187 /* ����������� ��������� ������             */
#define PLOT_NZRFMFW3 188 /* ����������� ���. ��������� ������        */
/*                                                                    */
#define PLOT_NZRFMFD0 189 /* �������� ����������� ������              */
#define PLOT_NZRFMFD1 190 /* �������� ������ ���                      */
#define PLOT_NZRFMFD2 191 /* �������� ��������� ������                */
#define PLOT_NZRFMFD3 192 /* �������� ���. ��������� ������           */
/*                                                                    */
/* ���������� �������� ������, ����������� � �������� ��� �������     */
/* (����������) �� ����� ���������� ������ ��� ���.                   */
/* �������� �� ������������� �������                                  */
#define PLOT_NZRFMFA  193
/*                                                                    */
/* ������ � ������ (�����, ���������) ������� ����������� ������      */
#define PLOT_NZRFLVL  194 /* ��������� ������ ��������� �����         */
#define PLOT_NZRFLLL  195 /* ������ ���������� �����                  */
#define PLOT_NZRFLLM  196 /* �������� ���������                       */
#define PLOT_NZRFLFL  197 /* ������ ���������� ���������              */
#define PLOT_NZRFLLN  198 /* ��������������(��� �����������) ���������*/
#define PLOT_NZRFLLD  199 /* �������� ���������                       */
#define PLOT_NZRFLFN  200 /* �������������� (��� �����������) ������  */
#define PLOT_NZRFLGC  201 /* ��������� ����� ����� (��������)         */
#define PLOT_NZRFLRUN 202 /* ������������� ����������� ������         */
/*                                                                    */
#define PLOT_NZRFLFM0 203 /* �������� ����������� ������              */
#define PLOT_NZRFLFM1 204 /* �������� ������ ���                      */
#define PLOT_NZRFLFM2 205 /* �������� ��������� ������                */
#define PLOT_NZRFLFM3 206 /* �������� ���. ��������� ������           */
/*                                                                    */
#define PLOT_NZRFLFR0 207 /* ������ ����������� ������                */
#define PLOT_NZRFLFR1 208 /* ������ ������ ���                        */
#define PLOT_NZRFLFR2 209 /* ������ ��������� ������                  */
#define PLOT_NZRFLFR3 210 /* ������ ���. ��������� ������             */
/*                                                                    */
#define PLOT_NZRFLFW0 211 /* ����������� ����������� ������           */
#define PLOT_NZRFLFW1 212 /* ����������� ������ ���                   */
#define PLOT_NZRFLFW2 213 /* ����������� ��������� ������             */
#define PLOT_NZRFLFW3 214 /* ����������� ���. ��������� ������        */
/*                                                                    */
#define PLOT_NZRFLFD0 215 /* �������� ����������� ������              */
#define PLOT_NZRFLFD1 216 /* �������� ������ ���                      */
#define PLOT_NZRFLFD2 217 /* �������� ��������� ������                */
#define PLOT_NZRFLFD3 218 /* �������� ���. ��������� ������           */
/*                                                                    */
/* ���������� �������� ������, ����������� � �������� ��� �������     */
/* (����������) �� ������ ���������� ������ ��� ���.                  */
/* �������� �� ������������� �������                                  */
#define PLOT_NZRFLFA  219
/*                                                                    */
/* ������ � ������ (�����, ���������)                                 */
/* ����� ���������� ������� ����� ���                                 */
#define PLOT_NZRFNVL  220 /* ��������� ������ ��������� �����         */
#define PLOT_NZRFNLL  221 /* ������ ���������� �����                  */
#define PLOT_NZRFNLM  222 /* �������� ���������                       */
#define PLOT_NZRFNFL  223 /* ������ ���������� ���������              */
#define PLOT_NZRFNLN  224 /* ��������������(��� �����������) ���������*/
#define PLOT_NZRFNLD  225 /* �������� ���������                       */
#define PLOT_NZRFNFN  226 /* �������������� (��� �����������) ������  */
#define PLOT_NZRFNGC  227 /* ��������� ����� ����� (��������)         */
#define PLOT_NZRFNRUN 228 /* ������������� ����������� ������         */
/*                                                                    */
#define PLOT_NZRFNFM0 229 /* �������� ����������� ������              */
#define PLOT_NZRFNFM1 230 /* �������� ������ ���                      */
#define PLOT_NZRFNFM2 231 /* �������� ��������� ������                */
#define PLOT_NZRFNFM3 232 /* �������� ���. ��������� ������           */
/*                                                                    */
#define PLOT_NZRFNFR0 233 /* ������ ����������� ������                */
#define PLOT_NZRFNFR1 234 /* ������ ������ ���                        */
#define PLOT_NZRFNFR2 235 /* ������ ��������� ������                  */
#define PLOT_NZRFNFR3 236 /* ������ ���. ��������� ������             */
/*                                                                    */
#define PLOT_NZRFNFW0 237 /* ����������� ����������� ������           */
#define PLOT_NZRFNFW1 238 /* ����������� ������ ���                   */
#define PLOT_NZRFNFW2 239 /* ����������� ��������� ������             */
#define PLOT_NZRFNFW3 240 /* ����������� ���. ��������� ������        */
/*                                                                    */
#define PLOT_NZRFNFD0 241 /* �������� ����������� ������              */
#define PLOT_NZRFNFD1 242 /* �������� ������ ���                      */
#define PLOT_NZRFNFD2 243 /* �������� ��������� ������                */
#define PLOT_NZRFNFD3 244 /* �������� ���. ��������� ������           */
/*                                                                    */
/* ���������� �������� ������, ����������� � �������� ��� �������     */
/* (����������) �� ����� ���������� ������� ����� ��� ���             */
/* ���. �������� �� ������������� �������                             */
#define PLOT_NZRFNFA  245
/*                                                                    */
/* ������ � ������ (�����, ���������) ����� ���������� ������� �������*/
#define PLOT_NZRFOVL  246 /* ��������� ������ ��������� �����         */
#define PLOT_NZRFOLL  247 /* ������ ���������� �����                  */
#define PLOT_NZRFOLM  248 /* �������� ���������                       */
#define PLOT_NZRFOFL  249 /* ������ ���������� ���������              */
#define PLOT_NZRFOLN  250 /* ��������������(��� �����������) ���������*/
#define PLOT_NZRFOLD  251 /* �������� ���������                       */
#define PLOT_NZRFOFN  252 /* �������������� (��� �����������) ������  */
#define PLOT_NZRFOGC  253 /* ��������� ����� ����� (��������)         */
#define PLOT_NZRFORUN 254 /* ������������� ����������� ������         */
/*                                                                    */
#define PLOT_NZRFOFM0 255 /* �������� ����������� ������              */
#define PLOT_NZRFOFM1 256 /* �������� ������ ���                      */
#define PLOT_NZRFOFM2 257 /* �������� ��������� ������                */
#define PLOT_NZRFOFM3 258 /* �������� ���. ��������� ������           */
/*                                                                    */
#define PLOT_NZRFOFR0 259 /* ������ ����������� ������                */
#define PLOT_NZRFOFR1 260 /* ������ ������ ���                        */
#define PLOT_NZRFOFR2 261 /* ������ ��������� ������                  */
#define PLOT_NZRFOFR3 262 /* ������ ���. ��������� ������             */
/*                                                                    */
#define PLOT_NZRFOFW0 263 /* ����������� ����������� ������           */
#define PLOT_NZRFOFW1 264 /* ����������� ������ ���                   */
#define PLOT_NZRFOFW2 265 /* ����������� ��������� ������             */
#define PLOT_NZRFOFW3 266 /* ����������� ���. ��������� ������        */
/*                                                                    */
#define PLOT_NZRFOFD0 267 /* �������� ����������� ������              */
#define PLOT_NZRFOFD1 268 /* �������� ������ ���                      */
#define PLOT_NZRFOFD2 269 /* �������� ��������� ������                */
#define PLOT_NZRFOFD3 270 /* �������� ���. ��������� ������           */
/*                                                                    */
/* ���������� �������� ������, ����������� � �������� ��� �������     */
/* (����������) �� ����� ���������� ������� ������� ���               */
/* ���. �������� �� ������������� �������                             */
#define PLOT_NZRFOFA  271
/*                                                                    */
#define PLOT_NZRMAXNM PLOT_NZRFOFA /* ������������ �����              */
/*====================================================================*/
/*=================== ��������� ��� NZR (�����) ======================*/
/*====================================================================*/

/* �/� ����� �����������                                              */
#define PLOT_GRIF_NA   '\0'
#define PLOT_GRIF_NS   '0'
#define PLOT_GRIF_DSP  '1'
#define PLOT_GRIF_S    '2'
#define PLOT_GRIF_SS   '3'

/* �/� ���������� ����������                                          */
#define PLOT_DATAPLACE_MEMORY  '&'
#define PLOT_DATAPLACE_TEMP    'V'
#define PLOT_DATAPLACE_FILE    'F'

/*====================================================================*/
/*=========== � � � � � � � � � � �  � � � � �  SUZ� (������)         */
/*====================================================================*/
#define SUZ_SENDMSG         'S'      /* ��������� ��������� : � ��-   */
                                     /* �������� suz_data - �����     */
                                     /* ��������: ������. ������      */
/*====================================================================*/
/*=========== � � � � � � � � � � �  � � � � �  SUZ� (�����)          */
/*====================================================================*/

/*====================================================================*/
/*===== � � � � � � � � � � �  � � � � �  � � � � � � (������) =======*/
/*====================================================================*/
#define PLOT_IO_NOT_XLINK       0    /* ���������� �������� ��������  */
                                     /* �������������� ��������, � �� */
                                     /* �������                       */
/*====================================================================*/
/*====== � � � � � � � � � � �  � � � � �  � � � � � � (�����) =======*/
/*====================================================================*/

/*====================================================================*/
/*=========== � � � � � � � � � � �  � � � � � � � � (������) ========*/
/*====================================================================*/
#ifdef WINNT    /* ��� ������ ���������� ���������                    */
  namespace Plot
  {
#endif /*WINNT*/

/**********************************************************************
                     ����� ��� ���������� �����
***********************************************************************/
#ifndef UP_ENQ_SETSIZE
  #define UP_ENQ_SETSIZE 16
#endif
typedef struct enq_set{
  u int count;
  char *enq_array[UP_ENQ_SETSIZE];
} enq_set;
#define ENQ_CLR(enq, set) do{ \
  u int __i; \
  if (set == NULL) break; \
  for (__i = 0; __i < ((enq_set *)(set))->count; __i++){ \
    if (strcmp(((enq_set *)(set))->enq_array[__i], enq) == 0){ \
      free(((enq_set *)(set))->enq_array[__i]); \
      ((enq_set *)(set))->enq_array[__i] = NULL; \
      while (__i < ((enq_set *)(set))->count-1) { \
        ((enq_set *)(set))->enq_array[__i] = \
            ((enq_set *)(set))->enq_array[__i+1]; \
        ((enq_set *)(set))->enq_array[__i+1] = NULL; \
        __i++; \
      } \
      ((enq_set *)(set))->count--; \
      break; \
    } \
  } \
} \
while(0)
#define ENQ_SET(enq, set) do{ \
  if (set == NULL) break; \
  if (((enq_set *)(set))->count < UP_ENQ_SETSIZE){ \
    ((enq_set *)(set))->enq_array[((enq_set *)(set))->count++] = \
      (char *)malloc(strlen(enq)+1); \
    strcpy(((enq_set *)(set))->enq_array[((enq_set *)(set))->count-1], enq); \
  } \
} \
while(0)
#define ENQ_ZERO(set) do{ \
  u int __i; \
  if (set == NULL) break; \
  for (__i = 0; __i < ((enq_set *)(set))->count; __i++){ \
    if (((enq_set *)(set))->enq_array[__i] != NULL){ \
      free(((enq_set *)(set))->enq_array[__i]); \
      ((enq_set *)(set))->enq_array[__i] = NULL; \
    } \
  } \
  ((enq_set FAR *)(set))->count = 0; \
} \
while(0)
#define ENQ_ISSET(enq, set) EnqIsSet((char *)(enq), (enq_set *)(set))

/**********************************************************************
                         �������� ��������� ���������������
***********************************************************************/

/***    ������������� �������                                       ***/
typedef struct _IDT  {              /* ��������� : ����-���           */
  u short       year;               /* ���                            */
  u char        month;              /* �����                          */
  u char        day;                /* ����                           */
  u long        tik;                /* ���                            */
} IDT;
typedef IDT   *PIDT;                /* ��������� �� ��������� IDT     */

/***    ������������� ������� � ������������ ��������               ***/
#define         PLOT_SYS_STARTTASK   1
#define         PLOT_SYS_ENDTASK     2
#define         PLOT_SYS_TERMTASK    3
#define         PLOT_SYS_ENA_LINES   4
#define         PLOT_SYS_DIS_LINES   5
#define         PLOT_SYS_ALT_KANS    6
#define         PLOT_SYS_MESSAGE     7
#define         PLOT_SYS_DELGARDREC  8
#define         PLOT_SYS_SCR         9
#define         PLOT_SYS_SCRDATA     10
#define         PLOT_SYS_VNT         11
#define         PLOT_SYS_VNTDATA     12
#define         PLOT_SYS_R           13

typedef struct _ITD  {              /* ��� - ���������, ��������� :   */
  IDT           idt;                /* �� �������-�� ������� �        */
  u long        req;                /* ������ (����) ������������     */
                                    /* ��������                       */
  u char        tsk_name[4];        /* ��� ������-����������� ��������*/
} ITD;
typedef ITD   *PITD;                /* ��������� �� ��������� ITD     */

/**********************************************************************
          ����������� ��������� ���������� ���������
***********************************************************************/
typedef struct _SYS{
  ITD     timecode;
  u char  sysoname[80];
  u char  data_c[256];
  u long  data_l[16];
  u short data_s[32];
  u char  data_x[64];
  u char  rezerv[96];
} SYS;
typedef SYS     *PSYS;

typedef struct _SUZ {
  u char  suz_iden[4];  /* ������� SUZ� ("SUZ\x0")                    */
  u long  suz_size;     /* ������ ������� � ����� ���������           */
  u char  suz_izo[4];   /* ������������� ������ �����������           */
  u char  suz_bo;       /* ����-��������� �������                     */
  u char  msg_type;     /* ��� ��������� (SYNC, ASYNC, SYS)           */
  u char  suz_rez[2];   /* ������                                     */
  u long  suz_naprsv;   /* ����������� �����                          */
  u long  suz_nvu;      /* ����� �������������� ���������             */
  u char  suz_data[40]; /* ������ (�������������� ��������� �������)  */
} SUZ;
typedef SUZ       *PSUZ;

/**********************************************************************
                      �������� ������ ��������
***********************************************************************/
typedef struct _SU {
  u char su_0;
  u char su_rezim;
  u char su_code;
  u char su_key[8];
  u char su_11;
  u char su_kvs[3];
  u char su_15;
  u char su_sr;
  u char su_17;
  u char su_id[2];
  u char su_20;
  u char su_uno[4];
  u char su_25;
  u char su_hhmm[4];
  u char su_30;
  u char su_grif;
  u char su_32;
  u char su_33[2];
  u char su_35[4];
  u char su_unp[4];
  u char su_43[3];
  u char su_ddmmyy[6];
  u char su_52[6];
  u char su_58;
  u char su_59;
  u char su_kvit;
  u char su_61[5];
  u char su_prrass;
  u char su_67;
  u char su_prz;
  u char su_kfs[3];
  u char su_kln[8];
} SU;
typedef SU       *PSU;

/**********************************************************************
                      �������� ����������� �������
***********************************************************************/
typedef struct _QUERYID {
  u long       pid;
  u long       tid;
  IDT          date;
} QUERYID;
typedef QUERYID   *PQUERYID;

/**********************************************************************
                �������� ��������� ��������� �������
***********************************************************************/
#define PLINE_DISK      0           /* ������� �� �����               */
#define PLINE_MEMORY    1           /* ������� � ������               */
#define LINE_DISK_SIZE  0x10485760  /* ������� ������ �������� �������*/
typedef struct _LINE {
  char     linefname[PLOTPATHSIZE]; /* ��� ����� �������              */
  u long   count_message;  /* ���������� ��������� � �������          */
  u long   m_read;         /* �������� ������� ������                 */
  u long   m_write;        /* �������� ������� ������                 */
  u long   sl_line;        /* ������� ���������                       */
  u long   linegran;       /* ������ �������                          */
                           /* (����������� ��� ������������� �������) */
  u long   diskline;       /* ������ ������ � ����� �������           */
  char     marker_read;    /* ������ ������: 1 - ������, 0 - ����     */
  char     marker_addn;    /* ������ ����������: 1 - ������, 0 - ���� */
  char     lineplace;      /* �/� ���������� �������                  */
  char     bo_sline;       /* ����/��������� ��������� �������        */
/* ������ ���������, ������������ � �������� ������� (������)         */
  char     name_thread[8]; /* ��� ������(4) + ��� ������(4)           */
  char    *buffer_line;    /* ������ ������� � ������                 */
  char    *ZipBuffer;      /* ��������� ����� ��� ��������� ��������� */
  u long   number_line;    /* ����� �������                           */
  u long   tid;            /* ������������� ������                    */
/* ������ ���������, ������������ � �������� ������� (�����)          */
  char     disrezerv[12];  /* ������                                  */
} LINE;
typedef LINE   *PLINE;

/**********************************************************************
      �������� �������� ��������� � ���������  ��������� � �������
***********************************************************************/
typedef struct _MESS {
  u long        size_message; /* ������ ���������                     */
  u char        tsksend[4];   /* ��� ������, ���������� ���������     */
  u char        is_msg_clop;  /* ������� �������������� ����������    */
  char          reserv[3];    /* ������                               */
  u long        w_size;       /* ������ ���������, ����������� ���    */
                              /* ������������ �� ������� 4 ����       */
} MESS;
typedef MESS  *PMESS;

typedef struct _POMS {
  u long   mnextmsg;       /* ��������� �� ��������� ������           */
  u char   nextmsgp;       /* �/� ������������ ���������� ���������   */
  u char   pr_read;        /* ������� ������� ��������� �� �������    */
  u char   rezerv2[10];    /* ������                                  */
} POMS;
typedef POMS   *PPOMS;

/**********************************************************************
    ��������� ��� ��������� �������� SLVNUM  SLVADR  GARDOV
***********************************************************************/
typedef struct _SLVS{
    void   *pslvnum;
    u long  LenNum;
    void   *pslvadr;
    u long  LenAdr;
    void   *pslvsnt;
    u long  LenSnt;
    u long  MaxSnt;
    void   *pgardov;
    u long  LenGarD;
    u long  MaxGarD;
} SLVS;
typedef SLVS   *PSLVS;       /* ��� ��������� �� ������ �������      */

/* ���������� �������                                                 */
typedef struct _UP_OBJ_COORD{
  char    descr[64];        /* �������� �������������� �������        */
  char    rezerv[64];
} UP_OBJ_COORD;
typedef UP_OBJ_COORD   *PUP_OBJ_COORD;

/* ���������� �������                                                 */
typedef struct _UP_OBJ_STAT{
  IDT     started;          /* ���� ������ � online                   */
  IDT     last_activity;    /* ���� ��������� ����������              */
  u long  msg_sent;         /* ������� �������                        */
  u long  msg_received;     /* �������� �������                       */
  char    rezerv[32];
} UP_OBJ_STAT;
typedef UP_OBJ_STAT   *PUP_OBJ_STAT;

/* ��� ������� (�������� type)                                        */
#define PLOT_OBJ_TYPE_UNK       0
#define PLOT_OBJ_TYPE_AIR       1
#define PLOT_OBJ_TYPE_SEE       2
#define PLOT_OBJ_TYPE_CAR       3
#define PLOT_OBJ_TYPE_BAG       4
#define PLOT_OBJ_TYPE_FIX       5
#define PLOT_OBJ_TYPE_GEN       6
#define PLOT_OBJ_TYPE_LEE       7

/* ���� �������� � ������� RDO (�������� TypeObj)                     */
#define  RDO_TYPE_FIX      1;      /* ������������                    */
#define  RDO_TYPE_CAR      2;      /* ���������                       */
#define  RDO_TYPE_SEE      3;      /* �������                         */
#define  RDO_TYPE_AIR      4;      /* ���������                       */
#define  RDO_TYPE_BAG      5;      /* ��������� (�������)             */
#define  RDO_TYPE_LEE      6;      /* ����������                      */
#define  RDO_TYPE_SPC      7;      /* �����������                     */
/* ���������� ��������� �����                                         */
#define  RDO_NUM_TYPE     = 7;
/* ���� ������� �������� � ������� RDO                                */
/*(�������� TypeObj ��� LevelObj = "")            �������:            */
#define  RDO_TYPE_FIX_GEN   RDO_NUM_TYPE + 1;  /* ������������        */
#define  RDO_TYPE_CAR_GEN   RDO_NUM_TYPE + 2;  /* ���������           */
#define  RDO_TYPE_SEE_GEN   RDO_NUM_TYPE + 3;  /* �������             */
#define  RDO_TYPE_AIR_GEN   RDO_NUM_TYPE + 4;  /* ���������           */
#define  RDO_TYPE_BAG_GEN   RDO_NUM_TYPE + 5;  /* ��������� (�������) */
#define  RDO_TYPE_LEE_GEN   RDO_NUM_TYPE + 6;  /* ����������          */
#define  RDO_TYPE_SPC_GEN   RDO_NUM_TYPE + 7;  /* �����������         */
/* ���������� ���� ���� ������������ �����                            */
#define  RDO_NUM_ALLTYPE  RDO_NUM_TYPE * 2;

/* ��������� ������� (�������� state)                                 */
#define PLOT_OBJ_STATE_AVAILABLE      1
#define PLOT_OBJ_STATE_NOTAVAILABLE   2
/* ������ ��������                                                    */
typedef struct _UP_OBJ_LIST{
  char            parent[2];      /* ��� ������� �������� ������      */
  char            code[2];        /* ��� �������                      */
  char            name[80];       /* ������������ �������             */
  char            descr[256];     /* �������� �������                 */
  char            type;           /* ��� �������                      */
  char            state;          /* ��������� �������                */
  char            reserv1[2];     /* ������                           */
  char            state_descr[80];/* �������� ��������� �������       */
  UP_OBJ_COORD    coord;          /* �������������� �������           */
  UP_OBJ_STAT     stat;           /* ���������� ������ �������        */
  char            reserv[152];    /* ������                           */
} UP_OBJ_LIST;
typedef UP_OBJ_LIST      *PUP_OBJ_LIST;

/* ��������� �������������� ��������                                  */
typedef struct _UP_OBJ_COMMUNICATION{
  char   code_from[2];   /* ��� ������� �����������                   */
  char   code_to[2];     /* ��� ������� ����������                    */
  char   task[4];        /* ���������                                 */
  char   addr[40];       /* �����                                     */
  char   reserv[16];     /* ������                                    */
} UP_OBJ_COMMUNICATION;
typedef UP_OBJ_COMMUNICATION      *PUP_OBJ_COMMUNICATION;

/* ��������� ��������� ������ �� ������ �� ������ ��������            */
typedef struct _UP_OBJ_REPLY_HDR{
  char   coding;         /* ���������                                 */
  char   reserv1[3];     /* ������                                    */
  long   size;           /* ������ �������������� �����               */
  long   str_list_size;  /* ������ ��������� ������ ��������          */
  long   str_list_cnt;   /* ���������� ����� � ������ ��������        */
  long   str_com_size;   /* ������ ��������� UP_OBJ_COMMUNICATION     */
  long   str_com_cnt;    /* ���������� ����� � ������ COMMUNICATION   */
  char   reserv[56];     /* ������                                    */
} UP_OBJ_REPLY_HDR;
typedef UP_OBJ_REPLY_HDR      *PUP_OBJ_REPLY_HDR;

/**********************************************************************
                �������� ������� ������� ������� (CWA)
***********************************************************************/
typedef struct _CWA{
  u short      cwa_moct;      /* ���� CSA ������                      */
  char         cwaidbas[5];   /* ���� ��� �������� ��������������     */
  u char       cwaidcnt;      /* ������� ��� ��������� ������������-  */
                              /* �� ���������� �������                */
  u long       cwapdklw;      /* ������� ������ ������ PDKL           */
  SLVS         cwasslvs;      /* ��������� ��������                   */
  UP_OBJ_LIST *obj_list;      /* ������ �������� �� (������ ��� ��)   */
  u long       obj_list_sz;   /* ���������� ��������� �������� ��     */
  UP_OBJ_COMMUNICATION *com_list; /* ������ ������ �������� �� (��)   */
  u long       com_list_sz;   /* ������ ������                        */
  char         rezerv[958];   /* ������                               */
} CWA;
typedef CWA   *PCWA;          /* ��� ��������� �� ������ �������      */

/**********************************************************************
                �������� ������� ���������� ��������� (PCA)
***********************************************************************/
typedef struct _PCA {
  u char        pcastart[8]; /* ������� ������ PCA                    */
  u long        pcaspid;     /* ������������� ��������                */
  void         *pcaadres;    /* ����� (����� �����) � �������� ��-��  */
  u long        pcapcsa;     /* PID ��������, ��� ������� CSA         */
  void         *pcaacsa;     /* ����� ����� CSA                       */
  u char        pcapdata[80];/* ������� ���������� ��������           */
  PLINE         pcaline;     /* ����� ��������� ����� �������         */
  IDT           pcaaprx;     /* ���. �������. ��������� ����-�������  */
  ITD           pcasdate;    /* ������������� ����. ������ � �������  */
  u short       pcantask;    /* ���� �-�� �������� ����. �� MAX_TASK  */
  u short       pcanlink;    /* ���� �-�� �������� ����. �� MAX_LINK  */
  void         *pcasmpr;     /* ����. �� ������ ������ ��� �������    */
  void         *pcacodtb;    /* ����� ������� ������ �������������    */
  u long        pcacodsz;    /* P����� ������� ������ �������������   */
  void         *pcakfstb;    /* ����� ���-��� ���. �������-�� �� ���  */
  u long        pcakfssz;    /* ����� ���-��� ���. �������-�� �� ���  */
  union {
    void       *adr[PLOBL/4];/* ���. ������� ��� �������� ����������  */
    double      dbl[PLOBL/8];/* ���. ������� ��� �������� ����������  */
    u long      lng[PLOBL/4];/* ���. ������� ��� �������� ����������  */
    u short     shr[PLOBL/2];/* ���. ������� ��� �������� ����������  */
    u char      chr[PLOBL  ];/* ���. ������� ��� �������� ����������  */
  } pcarab;
  u char        pcastop;     /* ������� ��������������� ���������     */
  u char        pcapcode;    /* ����� ��������� ������                */
  char          stop_clop;
  char          reserv1;     /* ������                                */
  PCWA          pcaacwa;     /* ����� ������� ������� �������         */
  void         *pcafrmtb;    /* ����� ����������� �������             */
                             /* ����� ����������                      */
  u long        pcafrmsz;    /* P����� ����������� �������            */
                             /* ����� ����������                      */
  void         *mystrpct;    /* ����� ���� ������ � PCT               */
  IDT           last_act;    /* ����� ���������� �����-� ������ � ����*/
  void         *pcaamdm;     /* ����� ����� ����                      */
  void         *pcaccsa;     /* ����� ����� CSA                       */
  void         *pthrline;    /* ��������� �� ����������� �������      */
                             /* �������� ������� ������               */
  char          pcafmdm;     /* ���� ������ ����                      */
  u char        pcalevel;    /* ������ �������� � �������             */
  char          reserv[186]; /* ������                                */
} PCA;
typedef PCA    *PPCA;

/**********************************************************************
                  �������� ������ ������� ��������� (PCT)
***********************************************************************/
typedef struct _PCT {
  u char        pctsproc[4];  /* ���������� ��� ��������-�� PROC_IDT  */
  u char        pctprgnm[80]; /* ������ ��� ��������� - �� PROGNAME   */
  u char        pctlabel[80]; /* ����� ��� ��������� - �� PROGLBL     */
  u char        pctpdata[80]; /* ������� ���������� ��������          */
  u long        pctspid;      /* ������������� ��������               */
  PPCA          pctapca;      /* ����� ������� ���������� ���������   */
  u char        reserv1[4];   /* ������                               */
  ITD           pctsdate;     /* ������������� ����. ������ � ������� */
  PLINE         pctsline;     /* ����� ��������� �������              */
  u char        pctcode;      /* ����� ��������� ������               */
  u char        pct_sys;      /* �������(y,n): - ���������� ��������  */
  u char        pctstop;      /* ������� ���������� ��������          */
  u char        pctlevel;     /* ������ �������� � �������            */
  u char        pctshide;     /* ��������� ������� 0/1                */
  u char        pctseclevel;  /* ������� (�� �����) �������           */
                              /* ������������������ ������            */
  u char        pctshleif;    /* ������� (^=0) �����������            */
                              /* ���������������� (�����)             */
  u char        pctonline;    /*@ ������� (^=0) ��������-��� ��������@*/
  u short       pctbuild;     /*@ ����� ������ ����������            @*/
  u char        pctmajor;     /*@ ����� ������ ����������            @*/
  u char        pctminor;     /*@ ����� ������ ����������            @*/
  u char        pctdesign[16];/*@ ������ ������������ ����������     @*/
  u long        pctnuminp;    /* ������� ���������� �������� ���������*/
  u long        pctinpread;   /* ������� ���������� �����������       */
                              /* �������� ���������                   */
  u long        pctnumout;    /* ������� ���������� ���������         */
                              /* ���������                            */
  u long        pcthighinpsz; /* ����� ���������� ����������� �       */
  u long        pctlowinpsz;  /* ������� (������� � ������� �������)  */
  u long        pcthighreadsz;/* ����� ���������� ����������� ��      */
  u long        pctlowreadsz; /* ������� (������� � ������� �������)  */
  u long        pcthighoutsz; /* ����� ��������� ����������           */
  u long        pctlowoutsz;  /* (������� � ������� �������)          */
/* ��������: �������� ���������, ���������� "@", ������������         */
/* ����������� ����������� ������ ������� PutTaskInfo()               */
  char          reserv[44];  /* RESERV                                */
} PCT;
typedef PCT      *PPCT;      /* ��� ��������� �� ������ ������� PCT   */
typedef PCT       MPCT[];    /* ��� ������ ����� - ��� ������� PCT    */
typedef MPCT     *PMPCT;     /* ��� ��������� �� ������� PCT          */

/**********************************************************************
                       �������� ������ ���
***********************************************************************/
typedef struct _TVK {
  SU        tvk_su;
  SYS       tvk_sys;
} TVK;
typedef TVK      *PTVK;
typedef TVK       MTVK[];
typedef MTVK     *PMTVK;

/**********************************************************************
                  �������� ������������� ������� (CSA)
***********************************************************************/
typedef struct _CSA {
  u char     csastart[8]; /* ������� ������ CSA: "<CSASCODE>_CSA"     */
  IDT        csaapprx;    /* ���. ����������� ��������� ����-�������  */
  ITD        csasdata;    /* �����. ����-����. � ���.��������(='00')  */
  u char     csaswork[80];/* ���-�� ����-����: �� ������ �� SYSWORK   */
  u char     csasname[80];/* ����. ��� �������: �� ������ �� SYSNAME  */
  u char     csascode[4]; /* ��� ������� - �� SYSCODE                 */
  u char     csaspref[4]; /* ������� ����� ����. ������ �� SYSPREF    */
  u char     csagname[80];/* ������ ��� ����������� - �� ORGNAME      */
  u char     csagcode[8]; /* ��� ����������� - �� ORGCODE             */
  u char     csaoname[80];/* ������ ��� ������� - �� OBJNAME          */
  u char     csaocode[2]; /* ��� ������� - �� OBJCODE                 */
  u char     csamname[80];/* ������ ��� ������ - �� MODNAME           */
  u char     csamcode[2]; /* ��� ������ - �� MODCODE                  */
  u char     csanet[4];   /* �������. ����+����� ������ � ����(2+2)   */
  u char     csaspath[80];/*                                          */
  u char     csasinit[80];/*                                          */
  u char     csampath[80];/*                                          */
  u char     csaminit[80];/*        ��������� ����������              */
  u char     csamline[80];/*                                          */
  u char     csamtemp[80];/*                                          */
  u char     csamdump[80];/*                                          */
  u char     csasslv[80]; /*                                          */
  u char     csamjurn[80];/*                                          */
  u short    csanproc;    /* ���� �-�� ��������� �������-�� MAX_PROC  */
  u short    csantask;    /* ���� �-�� ������� ��������-MAX_TASK      */
  u short    csanlink;    /* ���� �-�� �������� ����-� �� MAX_LINK    */
  u char     reserv1[2];  /* RESERV                                   */
  u long     csalnsz;     /* ������ �������� (����)                   */
  PPCT       csampct;     /* ����� ������� �������� ����. ���������   */
  void      *csacodtb;    /* ����� ������� ������ �������������       */
  u long     csacodsz;    /* P����� ������� ������ �������������      */
  void      *csakfstb;    /* ����� ����������� ������� �������������  */
                          /* �� ����                                  */
  u long     csakfssz;    /* P����� ����������� ������� ���������-��  */
                          /* �� ����                                  */
  PTVK       csaatvk;     /* ����� ������� ���������� ��������        */
  u long     csatvksz;    /* ������������ ���������� ����� � �������  */
  u char     csanname[80];/* ������ ��� ����                          */
  u char     csa_code;    /* ���������, � ������� ��������� �������   */
  u char     csalevel;    /* ������� ������������� �������            */
  char       reserv2[2];  /* RESERV                                   */
  PCWA       csaacwa;     /* ����� ������� ������� �������            */
  u long     csarproc;    /* ������� ���������� ����� � �������       */
  u char     csaidusr[2];
  u char     csavzv[10];
  u char     csafio[18];
  u char     csarange[2];
  void      *csafrmtb;    /* ����� ����������� ������� ����� ���-���  */
  u long     csafrmsz;    /* P����� ����������� ������� ����� �-���   */
  void      *csaexttb;    /* ����� ����������� ������� ���������� ��  */
  u long     csaextsz;    /* P����� ����������� ������� ���������� �� */
  u long     csajisz;     /* ������������ ���������� ����� � ���      */
  u long     csajosz;     /* ������������ ���������� ����� � ���      */
  u long     csajsosz;    /* ������������ ���������� ����� � �����    */
  char       csajsofl[8]; /* ����� ���������� �����:                  */
                          /* 0 - ����������� �������� ������          */
                          /* 1 - ����������� �������� ������          */
                          /* 2 - ����������� �������� ��������        */
  u long     csajfosz;    /* ������������ ���������� ����� � ���FTP   */
  u long     csajsysosz;  /* ������������ ���������� ����� � ���SYS   */
  u long     csajsysisz;  /* ������������ ���������� ����� � ���SYS   */
  void      *csaamdm;     /* ����� ����                               */
  char       csafmdm;     /* ���� ������ ����                         */
  u char     csaminuslvl; /* ����������� ������� �� - ������������    */
  u char     csamaxuslvl; /* ������������ ������� �� - ������������   */
  char       reserv3;
  char       main_obj[4]; /* �� ��������� ������� ������� (0 - ���)   */
  void      *adrslvokb;   /* ����� ������� �������� ������            */
  void      *adrslvzkb;   /* ����� ��������� �����                    */
  u long     okbNum;      /* ���������� ������� � ������� ������      */
                          /* ������                                   */
  void      *adrlmp;      /* ������� ������������� �������            */
  u long     numlmp;      /* ���������� ������� � �������             */
                          /* ������������� �������                    */
  void      *adrlms;      /* ������� �������� ���������� �����������  */
  u long     numlms;      /* ���������� ������� � �������             */
                          /* �������� ���������� �����������          */
  void      *adrrdo;      /* ������� - ���������� ���                 */
  u long     numrdo;      /* ���������� ������� � ������� -           */
                          /* ����������� ���                          */
  void      *csavnttb;    /* ����� ����������� ������� ���������      */
  u long     csavntsz;    /* P����� ����������� ������� ���������     */
  char       reserv[92];  /* RESERV                                   */
} CSA;
typedef CSA *PCSA;

/**********************************************************************
 ��������� �������� ������ � ��
***********************************************************************/
typedef struct _INFREC {
    u char Regim;
    u char Rezerv;
}INFREC;
typedef INFREC  *PINFREC; /* ����. �� ���������                       */

/**********************************************************************
                �������� ��������� ISTOK - �������
***********************************************************************/
typedef struct _STRTAB {
    u char codepol[8];
    u char namepol[80];
    u char tippol;
    u char dlnpol[2];
    u char indpol;
    u char cod[8];
}STRTAB;
typedef STRTAB  *PSTRTAB;
typedef STRTAB  MSTRTAB[];    /* ��� ������ ����� - ��� �������       */
typedef MSTRTAB *PMSTRTAB;    /* ��� ��������� �� �������             */

/**********************************************************************
                �������� ��������� �������
***********************************************************************/
typedef struct _CURSTR {
  u long      id_cur;       /* 4  : 4                                 */
  u char      bd_name[8];   /* 8  : 12                                */
  u char      nd_name[8];   /* 8  : 20                                */
  u char      tb_name[80];  /* 80 : 100                               */
  u char      kl_tab[8];    /* 8  : 108                               */
    void     *astrtab;      /* 4  : 112                               */
  u long      dl_struc;     /* 4  : 116                               */
    void     *adata;        /* 4  : 120                               */
  u long      dl_data;      /* 4  : 124                               */
    void     *arecord;      /* 4  : 128                               */
  u long      dln_zap;      /* 4  : 132                               */
  u long      dln_soob;     /* 4  : 136                               */
  u long      smeshen;      /* 4  : 140                               */
  u long      kol_zap;      /* 4  : 144                               */
  u char      cod_num;      /* 1  : 145                               */
  u char      namepol[80];  /* 80 : 225                               */
  u char      napravl;      /* 1  : 226                               */
  u char      pr_scr;       /* 1  : 227                               */
  u char      rezerv[28];   /* 29 : 256                               */
  u char      pr_perecod;
}CURSTR;
typedef CURSTR     *PCURSTR;   /* ����. �� ���������                  */
typedef CURSTR      MCURSTR[]; /* ������ ��������                     */
typedef MCURSTR    *PMCURSTR;  /* ��������� �� ���� ������ ��������   */

/**********************************************************************
                �������� ����������� ��������� ����������
***********************************************************************/
typedef struct _LINKSTR {
  QUERYID      queryido;    /* ������������� ����������� �������      */
  QUERYID      queryidp;    /* ������������� ���������� �������       */
  u char       link_bo;     /* �/� ���� ��������� (�����, HTML � �.�  */
  u char       linkreg;     /* ����� ������                           */
  u char       linksegm;    /* ������� ����� ��������� ���������      */
  u char       linkrazm;    /* �/� ���������� ����������              */
    void      *linkadr;     /* ����������� ����� ���-�� ����-�        */
    char       linkrzrv[2]; /* ������                                 */
  u char       link_xlink;  /* �/� ���� ������                        */
  u char       linklast;    /* �/� �������� ���������� �����          */
    long       linkprz;     /* ������� ���������� ����������          */
  u char       linkdbn[8];  /* ��� ��                                 */
  u char       linktabn[8]; /* ��� �������                            */
  u char       linkkeyf[8]; /* ���� ������ ����������� ������         */
  u char       linkkeyl[8]; /* ���� ��������� ������������ ������     */
  u long       linkgetr;    /* ���������� ����������� �������         */
  u long       linkputr;    /* ���������� ���������� �������          */
  u long       linkblkl;    /* ������������ ������ �����              */
  u long       linkblkn;    /* ����� ����� (�����)                    */
    void      *linknach;    /* ����� ������ ������� ������            */
  u long       linkobll;    /* ������������ ����� ������� ������      */
  u long       linkinfl;    /* ������� ����� ���-��� � ���. ������    */
 PCURSTR       pcurstr;     /* ��������� �� ��������� CURSTR          */
    IDT        linkidt;     /* ����� � ���� �������������� ���������  */
  u char       linkvxd[8];  /* �������� ����� ���-��                  */
} LINKSTR;
typedef LINKSTR   *PLINKSTR;   /* ��������� �� ��������� ���������    */

/**********************************************************************
                 �������� ������� �������� ����������
***********************************************************************/
typedef struct _LINKT {
  u char   modname[4];        /* ��� ������, � ������� �����������    */
                              /* ����������                           */
  PLINKSTR plinkstr;          /* ����� ���-��� ���-�� ����-�          */
  PCURSTR  pcurstr;           /* ����� ��������� CURSTR               */
  u char   linktrez[4];       /* ������                               */
} LINKT;
typedef LINKT   *PLINKT;      /* ��� ��������� �� ������ �������      */
typedef LINKT    MLINKT[];    /* ��� ������ ����� - ��� �������       */
typedef MLINKT  *PMLINKT;     /* ��� ��������� �� �������             */

/**********************************************************************
               �������� ����� ������������
***********************************************************************/
typedef struct _CALLSTK {
    long       avr;
    long       rez;
    long       idfn;
    long       rezerv;
    long       parm[STKNUMPARM];
} CALLSTK;
typedef CALLSTK    *PCALLSTK;  /* ����. �� ������ (���� ��� ������)   */
typedef CALLSTK     MCALLSTK[];/* ������ ����� �� ������ ��� �������  */
typedef MCALLSTK   *PMCALLSTK; /* ��������� �� ���� ������ �����      */

typedef struct _STKMPR {
    char               pr_stek[4];/* ������� ����� ("STEK")           */
    long               smppavr;
    long               smpprez;
  u long               smptid;   /* �����. ������('00'-���� ��� ���)  */
    void              *smphndl;  /* ���������� ������                 */
  PCSA                 smpacsa;
  u long               smpidcsa; /* PID ����., � ���. ��������� CSA   */
  PPCA                 smpapca;  /* ����� PCA                         */
  u long               smpidpca; /* PID ����., ���. ����������� PCA   */
    long               smpdelay; /* ������� ����� �������� ������     */
  PLINKT               smplinkt; /* ��������� �� ������� ��������     */
                                 /* ����������                        */
    long               smpbusy;  /* ��. ��������� ������ ����� (�� 0) */
  u long               smpncall; /**/
  enq_set             *enqset;   /**/
  u char               res1[2];  /* ������                            */
  char                 blockmode;/**/
  char                 smplevel; /**/
  void                *smpblock; /* ����� ���-�� �������������� ������*/
  u char               rezerv[192];/* ������                          */
  union {
    void       *adr[512/4];  /* ���. ������� ��� �������� ����������  */
    double      dbl[512/8];  /* ���. ������� ��� �������� ����������  */
    u long      lng[512/4];  /* ���. ������� ��� �������� ����������  */
    u short     shr[512/2];  /* ���. ������� ��� �������� ����������  */
    u char      chr[512];    /* ���. ������� ��� �������� ����������  */
  } smprobl;
  CALLSTK              callstk[16];/**/
} STKMPR;
typedef STKMPR     *PSTKMPR;   /* ����. �� ������ (���� ��� ������)   */
typedef STKMPR      MSTKMPR[]; /* ������ ����� �� ������ ��� �������  */
typedef MSTKMPR    *PMSTKMPR;  /* ��������� �� ���� ������ �����      */

/**********************************************************************
            �������� ������ (����������� ��� ����������)
***********************************************************************/
typedef struct _ADRESS {
  u short un;       /* �������� �����                                 */
  u short pn;       /* ������������ �����                             */
} ADRESS;
typedef ADRESS     *PADRESS;

/**********************************************************************
            �������� ������� �������� �� �� � ����� ��
***********************************************************************/
typedef struct _SLVNUM {
  union {
    ADRESS   a;
    u long   l;
  }          addr_p;           /* ����� ���������� (4�)     (����)    */
  u long     num_vu;           /* ����� ��  (�����) (4�)              */
  u long     trnsprt;          /* ��� ������������ ������             */
  char       pr_alt;           /* ������� ����������������            */
  char       rezerv[3];
} SLVNUM;                      /* ��� ������ �������                  */
typedef SLVNUM   *PSLVNUM;     /* ��������� �� ������                 */
typedef SLVNUM    MSLVNUM[];   /* ��� ������ ����� - ��� �������      */
typedef MSLVNUM  *PMSLVNUM;    /* ��� ��������� �� �������            */

/**********************************************************************
                           �������� SNT
***********************************************************************/
typedef struct _SLVSNT {
  union {
    ADRESS   a;
    u long   l;
  } addr_p;                    /* ����� ���������� (4�)     (����)    */
  u long     num_vu;           /* ����� ��  (�����)                   */
  u long     trnsprt;          /* ��� ������������ ������             */
  u char     sntmname[80];     /* ��� ���. ������                     */
  u char     sntidusr[2];
  u char     sntvzv[10];
  u char     sntfio[18];
  char       pr_alt;           /* ������� ���������������� ��         */
  u char     sntrange[2];      /* ����                                */
  u char     rezerv[3];        /* ������                              */
} SLVSNT;                      /* ��� ������ �������                  */
typedef SLVSNT   *PSLVSNT;     /* ��������� �� ������                 */
typedef SLVSNT    MSLVSNT[];   /* ��� ������ ����� - ��� �������      */
typedef MSLVSNT  *PMSLVSNT;    /* ��� ��������� �� �������            */

/**********************************************************************
             ������� �������� ������ �� � �����
***********************************************************************/
typedef struct _SLVADR {
  u long   num_vu;             /* ����� �� (����)                     */
  u long   trnsprt;            /* ��� ������������ ������             */
  char     adr_vu[40];         /* ������� ����� ��                    */
  char     pr_got;             /* ������� ���������� ������ �����     */
  char     pr_ping;            /* ������� ����, ��� ����� ���������   */
                               /* (������ ��� BAUL�)                  */
  char     rezerv[10];
} SLVADR;                      /* ��� ������ �������                  */
typedef SLVADR   *PSLVADR;     /* ��������� �� ������                 */
typedef SLVADR    MSLVADR[];   /* ��� ������ ����� - ��� �������      */
typedef MSLVADR  *PMSLVADR;    /* ��� ��������� �� �������            */

/**********************************************************************
                   ������� �������� �� �������
***********************************************************************/
typedef struct _SLV21Y {
  u char    kvs[3];            /* ��� ���� ���������                  */
  u char    nm_field[8];       /* ������������� ��� ���� ��� ����-�   */
  u char    usl;               /* ������� �����-� (����� - �� �����)  */
  u long    testsz;            /* ������� �. �� ������ ���� ��������  */
  u char    test[16];          /* � ��� ��������                      */
  u char    preproc;           /* ������� ��������������� ���������   */
  u char    pr_init;           /* ������� ������������� ������        */
  u char    task[4];           /* ��� ������� ������                  */
  u char    is_null;           /* ������� ������ ��� ���              */
  u char    rezerv[13];
} SLV21Y;
typedef SLV21Y   *PSLV21Y;
typedef SLV21Y    MSLV21Y[];   /* ��� ������ ����� - ��� �������      */
typedef MSLV21Y  *PMSLV21Y;    /* ��� ��������� �� �������            */

/**********************************************************************
          ������� ���������������� ��������� GARDOV
***********************************************************************/
typedef struct _GARDOV {
  u char     pr_nal;           /* ������� ������� ������              */
  u char     pr_read;          /* ������� ������                      */
  u char     rezerv1[2];       /* ������                              */
  u long     nextstr;          /* ��������� ������                    */
  u long     priorstr;         /* ���������� ������                   */
  union {
    ADRESS   a;
    u long   l;
  }          adr_p;            /* ����� ����������(4�) ����-�� �����  */
  SU         str_SU;           /* ��(���� �� ���������) ��� ������    */
                               /* ��������!!! ������ ���� SU - ���    */
                               /* ������, �� ��� ������ SU � ������   */
                               /* ���� su_kln ��������. ����� ���     */
                               /* ������������ ��� �������� ����� ��  */
                               /* ���������� ��� ������ �������       */
  IDT        added;            /* ����� ����������                    */
  IDT        changed;          /* ����� ��������� ���������           */
                               /*   priorstr == -1                    */
  u char     rezerv[16];       /* ������                              */
} GARDOV;                      /* ��� ������                          */
typedef GARDOV   *PGARDOV;     /* ��������� �� ������                 */
typedef GARDOV    MGARDOV[];   /* ��� ������ ����� - ��� �������      */
typedef MGARDOV  *PMGARDOV;    /* ��� ��������� �� �������            */

/**********************************************************************
             �������� ��������� ������� �������������
***********************************************************************/
typedef struct _CODTABN {
  u char        tabpref[8];   /* ������� ������� "������� "           */
  u long        tabnum;       /* ���������� ����� � ����-�������      */
  u char        tabname[52];  /* ��� �������                          */
} CODTABN ;
typedef CODTABN   *PCODTABN;

/**********************************************************************
                 �������� ������� �������������
***********************************************************************/
typedef struct _CODTAB {
  CODTABN       tabname;      /* ��������� �������                    */
  u char        tabbody[256]; /* ���� �������                         */
} CODTAB;
typedef CODTAB   *PCODTAB;   /* ��� ��������� �� �������              */


/**********************************************************************
                      �������� �������� ���������
***********************************************************************/
typedef struct _SEGMENT {
  u long       segmlen;    /* ������ ��������                         */
  u char       SEGMNum;    /* ����� ��������                          */
  u char       segm_bo;    /* �/� ��������                            */
  u char       segmrez[2]; /* ������                                  */
} SEGMENT;

/**********************************************************************
                      �������� ��������� ���������
***********************************************************************/
typedef struct _DOCUMENT {
  u char       docname[8];     /* ��� ��������� �� ��������������     */
  u char       docrezv[8];     /* ������                              */
  SEGMENT      docsegm[SEGMN]; /* ��������� ��������� ���������       */
} DOCUMENT;
typedef DOCUMENT   *PDOCUM;    /* ��������� �� ��������� ���������    */

/**********************************************************************
         �������� ����������� ������� ������������� �� ��� � ��
***********************************************************************/
typedef struct _CDTKFS {
  u char       cdtkfsnm[3];   /* ���                                  */
  u char       cdttipfd;      /* ��� ����                             */
  u char       cdtfdlen[8];   /* ����� ����                           */
  u char       cdtfdnam[8];   /* ��� ����                             */
} CDTKFS;
typedef CDTKFS   *PCDTKFS;    /* ��� ��������� �� ������ �������      */
typedef CDTKFS    MCDTKFS[];  /* ��� ������ ����� - ��� �������       */
typedef MCDTKFS  *PMCDTKFS;   /* ��� ��������� �� �������             */

/**********************************************************************
         �������� ����������� ������� ����� ����������
***********************************************************************/
typedef struct _FORMAT {
  u char       frmext[8];     /* ���������� ������ ��� ���� ���-��    */
  u char       frmdescr[80];  /* ��������                             */
  u char       frmview[256];  /* ���������� ������� "��������"        */
  u char       frmprint[256]; /* ���������� ������� "������"          */
  u char       frmres[3];     /* ������                               */
  u char       frmbocod;      /* �/� (��������������(y)/�� �����-��(n)*/
} FORMAT;
typedef FORMAT   *PFORMAT;
typedef FORMAT    MFORMAT[];
typedef MFORMAT  *PMFORMAT;

/**********************************************************************
     �������� ����������� ������� ����� ���������� ������ ��������
***********************************************************************/
typedef struct _KFSEXT {
  u char       kfs[3];        /* ���                                  */
  u char       rezerv1[1];    /* ������                               */
  u long       ext_size;      /* ������ ���������� ��                 */
  u char       rezerv[8];     /* ������                               */
} KFSEXT;
typedef KFSEXT   *PKFSEXT;    /* ��� ��������� �� ������ �������      */
typedef KFSEXT    MKFSEXT[];  /* ��� ������ ����� - ��� �������       */
typedef MKFSEXT  *PMKFSEXT;   /* ��� ��������� �� �������             */

/**********************************************************************
     �������� ����������� ������� ����� ���������� ������ ��������
***********************************************************************/
typedef struct _TASKSPIS {
  char         tskname[4];
  IDT          obr_dt;
} TASKSPIS;
typedef TASKSPIS   *PTASKSPIS;    /* ��� ��������� �� ������ �������  */

/**********************************************************************
           �������� ������������ �������� ��������� ������
***********************************************************************/
#define DATAFILE   1       /* ������ - � �����                        */
#define DATAMEMORY 2       /* ������ - � ������                       */
#define DATATEMP   3       /* ������ - �� ��������� ������            */
typedef struct _TMPHDR {
  SU           su;         /* ������ �������� ��� ������              */
  char         su_ext[PLOTSUSIZE];/* ���������� ������ ��������       */
  u long       su_size;    /* ������ ���������� ������ ��������       */
  u long       datasize;   /* ������ ������ �� ��������� ������       */
  long         ref_type;   /* ��� ������ �� ������:                   */
                           /* DATAFILE - ������ ����������� � �����   */
                           /*            fname                        */
                           /* DATAMEMORY - ������ ����������� � ��-   */
                           /*              ���� �������� � ��������-  */
                           /*              ������� pid �� ������ adr  */
  char         mustdel;    /* ������� ������������� �������� �����    */
                           /* ������ �� ������� �� ����               */
  char         is_clop_ext;/* ������� �������� EXT ��                 */
  char         is_clop_data;/* ������� �������� ������                */
  char         clop_key[PLOTKEYSIZE];/* ���� �������� ������          */
  u char       idDS;       /* ���� ��������� ������������� ���������
                           /* 0 - �����������                         */
                           /* 1 - ������������                        */
                           /* 2 - ������������                        */
                           /* 3 - ���� ����������������               */
                           /* 4 - ���� �������� ����������� �����     */
                           /* 5 - ���� ��������                       */
                           /* 6 - ������� �����������                 */
  u long       clop_wrk1;  /* ������� ���������� ��� ��������         */
  u long       clop_wrk2;  /* ������� ���������� ��� ��������         */
  char         is_own;     /* ������� �������������� �� �����         */
                           /* 0 - �� ����������� �� �����             */
                           /* 1 - ����������� �� �����                */
  char         rezerv2[2]; /* ������ 2                                */
  char         pr_fwd; /*  */
  char         keyfwd[8];  /* ���� �� FWD - ���������                 */
  TASKSPIS     mtaskspis[PLOTTASKADD]; /* ������ ������ �����         */
  u long       lentask;      /* ���������� ��������� � ����. mtaskspis*/
  u long       clop_sm;    /* ������� ���������� ��� ��������         */
  u long       clop_wrk3;  /* ������� ���������� ��� ��������/��������*/
  u long       clop_wrk4;  /* ������� ���������� ��� ��������/��������*/
  char         rezerv3[20];/* ������ 3                                */
  union {
    struct {
      long     pid;
      void    *adr;
      char     rezerv[PLOTPATHSIZE - sizeof(long) - sizeof(void *)];
    }          point;
    char       fname[PLOTPATHSIZE]; /* ��� �����                      */
  }            refer;      /* ������ �� ������                        */
} TMPHDR;
typedef TMPHDR   *PTMPHDR;/* ��� ��������� �� ������ �������          */

#define PRPNOERR       '0'/* ��� ������                               */
#define PRPERRMODEXIST '1'/* ������ ��������������� �� ������ ��      */
#define PRPERRMAXSNT   '2'/* ����������� SNT �����                    */
#define PRPERRCHANGE   '3'/* ������ ������������� SNT                 */
#define PRPERRLINK     '4'/* ��� �����                                */
typedef struct _PRP{
  u char prpkvs[3];       /* ��� ���� ���������                       */
  u char prpdate[8];      /* ����                                     */
  u char prptime[8];      /* �����                                    */
  u char prpuno[2];       /* ���                                      */
  u char prppno[2];       /* ���                                      */
  u char prpnvu[4];       /* ����� �������������� ���������           */
  u char prpmname[80];    /* ������������ ������                      */
  u char prpid[2];
  u char prprange[2];
  u char prppass[8];
  u char prpvzv[10];
  u char prpfio[18];
  u char prperror;        /* ��������� ���������� ��������            */
  u char prpprotv;        /* ������� �����. ������ ('n' - �� �����)   */
  u char prprez[107];     /* ������                                   */
} PRP;
typedef PRP     *PPRP;

/**********************************************************************
          �������� ��������� ���� - ����� (���������� ���)
***********************************************************************/
typedef struct _DTS {
  u char  dtsday[2];       /* ����                                     */
  u char  dtsmonth[2];     /* �����                                    */
  u char  dtsyear[4];      /* ���                                      */
  u char  dtshour[2];      /* ����                                     */
  u char  dtsmin[2];       /* ������                                   */
  u char  dtssec[2];       /* �������                                  */
  u char  dtshund[3];      /* �����������                              */
  u char  dtsresrv[3];     /* ������                                   */
} DTS;
typedef DTS      *PDTS;

/**********************************************************************
     �������� ��������� ���������� ������� ����������������
***********************************************************************/
typedef struct _JURNSYSZ {
  char      idnt[4];     /* ������� �������                           */
  u long    n_rec;       /* ���������� ������� � �������              */
  u long    sm_add;      /* �������� ����������                       */
  u long    sm_first;    /* �������� ������ ������                    */
  u char    code;        /* ��������� ��������� �������               */
  u char    rezerv1[3];  /* ������                                    */
  u long    reclen;      /* ����� ������ � �������                    */
  u char    owner[4];    /* �������� (��� ������ ��� ��� (��� JSS))   */
  u char    rezerv[4];   /* ������                                    */
} JURNSYSZ;
typedef JURNSYSZ   *PJURNSYSZ;

/**********************************************************************
     �������� ������ ���������� ������� ����������������
***********************************************************************/
typedef struct _JURNSYS {
  STKMPR   sysstk;       /* ����                                      */
  IDT      systime;      /* �����                                     */
  char     systsk[4];    /* ��� ������                                */
  char     sysinf[256];  /* ���������� � �������                      */
  u long   syssmesh;     /* �������� ������� ������                   */
  u long   syslen;       /* ����� ������� ������                      */
  char     sysprdel;     /* ������� �������� ������                   */
  u char   sysprview;    /* ������� ��������� ������                  */
  u char   code;         /* ��������� ������                          */
  u char   rezerv1;      /* ������                                    */
  u char   rezerv[232];  /* ������                                    */
} JURNSYS;
typedef JURNSYS   *PJURNSYS;  /* ��� ��������� �� ������              */

/**********************************************************************
             �������� ���������� ����� ����������
***********************************************************************/
typedef struct _UPINFO {
  u char     orgname[80]; /* ������ ��� �����������                   */
  u char     orgcode[8];  /* ��� �����������                          */
  u char     objname[80]; /* ������ ��� �������                       */
  u char     objcode[2];  /* ��� �������                              */
  u char     modname[80]; /* ������ ��� ����������� ������            */
  u char     modcode[2];  /* ��� ����������� ������                   */
  u char     nvu[4];      /* ����� �������������� ���������           */
  u char     netname[80]; /* ������ ��� ����                          */
  u char     userstat[10];/* ��������� ������������                   */
  u char     userfio[18]; /* ��� ������������                         */
  u char     logadr[4];   /* ���� ���������� �����                    */
  u char     userid[2];   /* ������������� ������������               */
  u char     userrang[2]; /* ���� ������������                        */
  u char     rezerv[12];  /* ������                                   */
} UPINFO;
typedef UPINFO   *PUPINFO;    /* ��� ��������� �� ������              */

/**********************************************************************
             �������� ����� ���������� ������
***********************************************************************/
typedef struct _TASKINFO {
  IDT        idtapprx;    /* ���. ����������� ��������� ����-�������  */
  u long     pid;         /* ������������� ��������                   */
  u long     tid;         /* �����. ������                            */
  void      *hndl;        /* �����. ������                            */
  u char     name[4];     /* ������������� ������                     */
  u char     workdir[80]; /* ������� ���������� ������                */
  u char     stop;        /* ������� ��������������� ���������        */
  u char     code;        /* ����� ��������� ������                   */
  u char     rezerv[22];  /* ������                                   */
} TASKINFO;
typedef TASKINFO   *PTASKINFO;/* ��� ��������� �� ������              */

typedef struct _DIRECTION {
  u char        dir_0[59];
  u char        dir_1[5];
  u char        dir_grif[10];
  u char        dir_2;
  u char        dir_2_1[5];
  u char        dir_2_2[13];
  u char        dir_otpr[12];
  u char        dir_2_3;
  u char        dir_2_4[54];
  u char        dir_2_5[13];
  u char        dir_dtvr[14];
  u char        dir_2_6;
  u char        dir_2_7[52];
  u char        dir_2_8[14];
  u char        dir_isx[8];
  u char        dir_2_9;
  u char        dir_3[8];
  u char        dir_sris[16];
  u char        dir_srdt[8];
  u char        dir_4;
  u char        dir_srtm[5];
  u char        dir_5;
  u char        dir_6[44];
  u char        dir_adrp[18];
  u char        dir_7[40];
  u char        dir_8;
  u char        dir_adr1[10];
  u char        dir_9[2];
  u char        dir_pnp1[2];
  u char        dir_10;
  u char        dir_11[2];
  u char        dir_12;
  u char        dir_adr2[10];
  u char        dir_13[2];
  u char        dir_pnp2[2];
  u char        dir_14;
  u char        dir_15[2];
  u char        dir_16;
  u char        dir_adr3[10];
  u char        dir_17[2];
  u char        dir_pnp3[2];
  u char        dir_18;
  u char        dir_19[2];
  u char        dir_20;
  u char        dir_adr4[10];
  u char        dir_21[2];
  u char        dir_pnp4[2];
  u char        dir_22;
  u char        dir_23[10];
  u char        dir_24;
  u char        dir_adr5[10];
  u char        dir_25[2];
  u char        dir_pnp5[2];
  u char        dir_26;
  u char        dir_27[2];
  u char        dir_28;
  u char        dir_adr6[10];
  u char        dir_29[2];
  u char        dir_pnp6[2];
  u char        dir_30;
  u char        dir_31[2];
  u char        dir_32;
  u char        dir_adr7[10];
  u char        dir_33[2];
  u char        dir_pnp7[2];
  u char        dir_34;
  u char        dir_35[2];
  u char        dir_36;
  u char        dir_adr8[10];
  u char        dir_37[2];
  u char        dir_pnp8[2];
  u char        dir_38;
  u char        dir_39[85];
  u char        dir_40;
  u char        dir_msg[4720];
} DIRECTION;
typedef DIRECTION   *PDIRECTION;

typedef struct _ANSWRDIR {
  u char        adir_0[59];
  u char        adir_1[5];
  u char        adir_grif[10];
  u char        adir_2;
  u char        adir_2_1[5];
  u char        adir_2_2[13];
  u char        adir_otpr[12];
  u char        adir_2_3;
  u char        adir_2_4[54];
  u char        adir_2_5[13];
  u char        adir_dtvr[14];
  u char        adir_2_6;
  u char        adir_2_7[52];
  u char        adir_2_8[14];
  u char        adir_isx[8];
  u char        adir_2_9;
  u char        adir_3[7];
  u char        adir_4[17];
  u char        adir_uchn[14];
  u char        adir_5;
  u char        adir_6[44];
  u char        adir_adrp[18];
  u char        adir_7[40];
  u char        adir_8;
  u char        adir_adr1[10];
  u char        adir_9[2];
  u char        adir_pnp1[2];
  u char        adir_10;
  u char        adir_11[2];
  u char        adir_12;
  u char        adir_adr2[10];
  u char        adir_13[2];
  u char        adir_pnp2[2];
  u char        adir_14;
  u char        adir_15[2];
  u char        adir_16;
  u char        adir_adr3[10];
  u char        adir_17[2];
  u char        adir_pnp3[2];
  u char        adir_18;
  u char        adir_19[2];
  u char        adir_20;
  u char        adir_adr4[10];
  u char        adir_21[2];
  u char        adir_pnp4[2];
  u char        adir_22;
  u char        adir_23[10];
  u char        adir_24;
  u char        adir_adr5[10];
  u char        adir_25[2];
  u char        adir_pnp5[2];
  u char        adir_26;
  u char        adir_27[2];
  u char        adir_28;
  u char        adir_adr6[10];
  u char        adir_29[2];
  u char        adir_pnp6[2];
  u char        adir_30;
  u char        adir_31[2];
  u char        adir_32;
  u char        adir_adr7[10];
  u char        adir_33[2];
  u char        adir_pnp7[2];
  u char        adir_34;
  u char        adir_35[2];
  u char        adir_36;
  u char        adir_adr8[10];
  u char        adir_37[2];
  u char        adir_pnp8[2];
  u char        adir_38;
  u char        adir_39[85];
  u char        adir_40;
  u char        adir_msg[4720];
} ANSWRDIR;
typedef ANSWRDIR    *PANSWRDIR;

typedef struct _ANSWSIGNAL {
  u char        asgl_0[59];
  u char        asgl_1[5];
  u char        asgl_grif[10];
  u char        asgl_2;
  u char        asgl_3[201];
  u char        asgl_4[11];
  u char        asgl_vvod[14];
  u char        asgl_5;
  u char        asgl_6[44];
  u char        asgl_adrp[18];
  u char        asgl_7[40];
  u char        asgl_8;
  u char        asgl_adr1[10];
  u char        asgl_9[2];
  u char        asgl_pnp1[2];
  u char        asgl_10;
  u char        asgl_11[2];
  u char        asgl_12;
  u char        asgl_adr2[10];
  u char        asgl_13[2];
  u char        asgl_pnp2[2];
  u char        asgl_14;
  u char        asgl_15[2];
  u char        asgl_16;
  u char        asgl_adr3[10];
  u char        asgl_17[2];
  u char        asgl_pnp3[2];
  u char        asgl_18;
  u char        asgl_19[2];
  u char        asgl_20;
  u char        asgl_adr4[10];
  u char        asgl_21[2];
  u char        asgl_pnp4[2];
  u char        asgl_22;
  u char        asgl_23[10];
  u char        asgl_24;
  u char        asgl_adr5[10];
  u char        asgl_25[2];
  u char        asgl_pnp5[2];
  u char        asgl_26;
  u char        asgl_27[2];
  u char        asgl_28;
  u char        asgl_adr6[10];
  u char        asgl_29[2];
  u char        asgl_pnp6[2];
  u char        asgl_30;
  u char        asgl_31[2];
  u char        asgl_32;
  u char        asgl_adr7[10];
  u char        asgl_33[2];
  u char        asgl_pnp7[2];
  u char        asgl_34;
  u char        asgl_35[2];
  u char        asgl_36;
  u char        asgl_adr8[10];
  u char        asgl_37[2];
  u char        asgl_pnp8[2];
  u char        asgl_38;
  u char        asgl_39[85];
  u char        asgl_40;
  u char        asgl_msg[4720];
} ANSWSIGNAL;
typedef ANSWSIGNAL  *PANSWSIGNAL;

typedef struct _UPSIGNAL {
  u char        sgl_0[5];
  u char        sgl_shr[6];
  u char        sgl_1;
  u char        sgl_2[47];
  u char        sgl_3[5];
  u char        sgl_grif[10];
  u char        sgl_4;
  u char        sgl_5[201];
  u char        sgl_6[10];
  u char        sgl_7;
  u char        sgl_vvod[14];
  u char        sgl_8;
  u char        sgl_9[102];
  u char        sgl_10;
  u char        sgl_adr1[10];
  u char        sgl_11[2];
  u char        sgl_pnp1[2];
  u char        sgl_12;
  u char        sgl_13[2];
  u char        sgl_14;
  u char        sgl_adr2[10];
  u char        sgl_15[2];
  u char        sgl_pnp2[2];
  u char        sgl_16;
  u char        sgl_17[2];
  u char        sgl_18;
  u char        sgl_adr3[10];
  u char        sgl_19[2];
  u char        sgl_pnp3[2];
  u char        sgl_20;
  u char        sgl_21[2];
  u char        sgl_22;
  u char        sgl_adr4[10];
  u char        sgl_23[2];
  u char        sgl_pnp4[2];
  u char        sgl_24;
  u char        sgl_25[10];
  u char        sgl_26;
  u char        sgl_adr5[10];
  u char        sgl_27[2];
  u char        sgl_pnp5[2];
  u char        sgl_28;
  u char        sgl_29[2];
  u char        sgl_30;
  u char        sgl_adr6[10];
  u char        sgl_31[2];
  u char        sgl_pnp6[2];
  u char        sgl_32;
  u char        sgl_33[2];
  u char        sgl_34;
  u char        sgl_adr7[10];
  u char        sgl_35[2];
  u char        sgl_pnp7[2];
  u char        sgl_36;
  u char        sgl_37[2];
  u char        sgl_38;
  u char        sgl_adr8[10];
  u char        sgl_39[2];
  u char        sgl_pnp8[2];
  u char        sgl_40;
  u char        sgl_41[85];
  u char        sgl_42;
  u char        sgl_msg[4720];
} UPSIGNAL;
typedef UPSIGNAL    *PUPSIGNAL;

typedef struct _TEMAMSG {
  u char        tema_0[15];
  u char        tema_1;
  u char        tema_zfio[28];
  u char        tema_2;
  u char        tema_3[19];
  u char        tema_4[5];
  u char        tema_grif[10];
  u char        tema_5;
  u char        tema_6[15];
  u char        tema_7;
  u char        tema_adi1[13];
  u char        tema_8[2];
  u char        tema_adi2[13];
  u char        tema_9;
  u char        tema_10[35];
  u char        tema_11[15];
  u char        tema_12;
  u char        tema_sbo1[3];
  u char        tema_13[2];
  u char        tema_sbo2[3];
  u char        tema_14[2];
  u char        tema_sbo3[3];
  u char        tema_15[2];
  u char        tema_sbo4[3];
  u char        tema_16[2];
  u char        tema_sbo5[3];
  u char        tema_17;
  u char        tema_18[40];
  u char        tema_19[15];
  u char        tema_20;
  u char        tema_tem1[3];
  u char        tema_21[2];
  u char        tema_tem2[3];
  u char        tema_22;
  u char        tema_23[55];
  u char        tema_24[15];
  u char        tema_25;
  u char        tema_date[8];
  u char        tema_26;
  u char        tema_27[55];
  u char        tema_28[15];
  u char        tema_29;
  u char        tema_msgh[55];
  u char        tema_30;
  u char        tema_31[8];
  u char        tema_32[15];
  u char        tema_33;
  u char        tema_shr1[8];
  u char        tema_34[2];
  u char        tema_shr2[8];
  u char        tema_35[2];
  u char        tema_shr3[8];
  u char        tema_36[2];
  u char        tema_shr4[8];
  u char        tema_37[2];
  u char        tema_shr5[8];
  u char        tema_38[2];
  u char        tema_shr6[8];
  u char        tema_39;
  u char        tema_40[5];
  u char        tema_41[15];
  u char        tema_42;
  u char        tema_msg[4784];
} TEMAMSG;
typedef TEMAMSG     *PTEMAMSG;

/**********************************************************************
                         �������� ��������
***********************************************************************/
/* ��������� ������ � �������                                         */
#define   J_RECORD_DEL     0xFF
#define   J_RECORD_ADD     0x00
#define   J_RECORD_MDF     0x01

/**********************************************************************
      �������� ������ ������� �������� ����������
***********************************************************************/
/* ���������� �������� ������� ��������                               */
#define   JI_ITEM_RECVDT         2
#define   JI_ITEM_ISPRDT         4
#define   JI_ITEM_OBRDT          8
#define   JI_ITEM_ISPDT         16
#define   JI_ITEM_MSGLEN        32
#define   JI_ITEM_MYKEY         64
#define   JI_ITEM_FARKEY       128
#define   JI_ITEM_TMPKEY       256
#define   JI_ITEM_LAOTPR       512
#define   JI_ITEM_FORMAT      1024
#define   JI_ITEM_KVS         2048
#define   JI_ITEM_GRIF        4096
#define   JI_ITEM_SR          8192
#define   JI_ITEM_TMPKEYFWD  16384
#define   JI_ITEM_LAOTPRFWD  32768
#define   JI_ITEM_DSSTATUS   65536
typedef struct _JOURNIN{
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          recv_dt;       /* ���� � ����� ��������� ���������     */
  IDT          ispr_dt;       /* ���� � ����� ��� ����������          */
  IDT          obr_dt;        /* ���� � ����� ��������� ���������     */
  IDT          isp_dt;        /* ���� ���������� ���������            */
  u long       msglen;        /* ������ ���������                     */
  u char       mykey[8];      /* �������� ����                        */
  u char       farkey[8];     /* ��������� ���� ���������             */
  u char       tmpkey[8];     /* ���� ��������� �� ��������� ������   */
  u char       la_otpr[4];    /* ���. ����� ����������� ���������     */
  u char       format;        /* ������                               */
  u char       kvs[3];        /* ��� ���������                        */
  u char       grif;          /* ����                                 */
  u char       sr;            /* ���������                            */
  u char       tmpkeyfwd[8];  /* ���� ��������� �� ��������� ������   */
  u char       la_otprfwd[4]; /* ���. ����� ����������� ���������     */
  u char       pr_fwd;        /* ������� ����. ���������              */
  u char       ds_status;     /* ������ �������� �������              */
                              /* 0 - �����������                      */
                              /* 1 - ������������                     */
                              /* 2 - ������������                     */
                              /* 3 - ���� ����������������            */
                              /* 4 - ���� �������� ����������� �����  */
                              /* 5 - ���� ��������                    */
                              /* 6 - ������� �����������              */
  u char       rezerv[20];    /* ������                               */
} JOURNIN;
typedef JOURNIN   *PJOURNIN;

/**********************************************************************
      �������� ������ ������� ��������� ����������
***********************************************************************/
/* ���������� �������� ������� ���������                              */
#define   JO_ITEM_SENDDT        2
#define   JO_ITEM_DOVDT         4
#define   JO_ITEM_OBRBDT        8
#define   JO_ITEM_OBRDT        16
#define   JO_ITEM_ISPRDT       32
#define   JO_ITEM_ISPDT        64
#define   JO_ITEM_MSGLEN      128
#define   JO_ITEM_MYKEY       256
#define   JO_ITEM_FARKEY      512
#define   JO_ITEM_TMPKEY     1024
#define   JO_ITEM_LAPOL      2048
#define   JO_ITEM_FORMAT     4096
#define   JO_ITEM_KVS        8192
#define   JO_ITEM_GRIF      16384
#define   JO_ITEM_SR        32768
#define   JO_ITEM_DSSTATUS  65536
typedef struct _JOURNOUT{
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          send_dt;       /* ���� � ����� �������� ���������      */
  IDT          dov_dt;        /* ���� � ����� ��������� ���������     */
  IDT          obrb_dt;       /* ���� � ����� ���������� �� ��������� */
  IDT          obr_dt;        /* ���� � ����� ��������� ���������     */
  IDT          ispr_dt;       /* ��������� ���� � ����� ����������    */
  IDT          isp_dt;        /* ���� � ����� ���������� ���������    */
  u long       msglen;        /* ������ ���������                     */
  u char       mykey[8];      /* ��������� ����                       */
  u char       farkey[8];     /* �������� ���� ���������              */
  u char       tmpkey[8];     /* ���� ��������� �� ��������� ������   */
  u char       la_pol[4];     /* ���. ����� ���������� ���������      */
  u char       format;        /* ������                               */
  u char       kvs[3];        /* ��� ���������                        */
  u char       grif;          /* ����                                 */
  u char       sr;            /* ���������                            */
  u char       tmpkeyfwd[8];  /* ���� ��������� FWD �� ��             */
  u char       pr_fwd;        /* ������� ����. ���������              */
  u char       ds_status;     /* ������ �������� �������              */
                              /* 0 - �����������                      */
                              /* 1 - ������������                     */
                              /* 2 - ������������                     */
                              /* 3 - ���� ����������������            */
                              /* 4 - ���� �������� ����������� �����  */
                              /* 5 - ���� ��������                    */
                              /* 6 - ������� �����������              */
  u char       rezerv[8];     /* ������                               */
} JOURNOUT;
typedef JOURNOUT   *PJOURNOUT;

/**********************************************************************
      �������� ������ ������� ��������� �������� ����������� ������
***********************************************************************/
/* ���������� ���� ������                                             */
#define   JSO_OBMN_DEX           2     /* ���������                   */
#define   JSO_OBMN_REX           4     /* ������                      */
#define   JSO_OBMN_TEX           8     /* �������                     */
/* ���������� ���� ��������                                           */
#define   JSO_QUERY_READ        16     /* ������                      */
#define   JSO_QUERY_DELETE      32     /* �������                     */
#define   JSO_QUERY_SAVE        64     /* ��������                    */
#define   JSO_QUERY_CREATE     128     /* �������                     */
#define   JSO_QUERY_NEXT       256     /* ���� ���������              */
#define   JSO_QUERY_PREV       512     /* ���� ����������             */
#define   JSO_QUERY_OPEN      1024     /* �������                     */
/* ���������� �������� ������� ��������� ��������                     */
#define   JSO_ITEM_SENDDT        2
#define   JSO_ITEM_MYKEY         4
#define   JSO_ITEM_QUERYCOD      8
#define   JSO_ITEM_SOURCELA     16
#define   JSO_ITEM_OBJKEY       32
#define   JSO_ITEM_OBJDT        64
#define   JSO_ITEM_OBRDT       128
#define   JSO_ITEM_FARKEY      256
#define   JSO_ITEM_REZCODE     512
#define   JSO_ITEM_OBJSZ      1024
#define   JSO_ITEM_OBJFRMT    2048
#define   JSO_ITEM_TMPKEY     4096
#define   JSO_ITEM_GRIF       8192
typedef struct _JSYNCOUT {
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          send_dt;       /* ���� �������� �������                */
  QUERYID      my_key;        /* ��������� ���� �������               */
  u long       querycod;      /* ��� ������� + ��� ������             */
  u char       sourcela[4];   /* ���������� ����� ���������� �������  */
  u char       objkey[8];     /* ���� ����������� ��                  */
  IDT          obj_dt;        /* ����, �� ���. ����� ������           */
  IDT          obr_dt;        /* ���� ��������� ������ �� ������      */
  QUERYID      far_key;       /* �������� ���� �������                */
  u long       rezcode;       /* ��������� ���������� �������         */
  u long       obj_sz;        /* ������ ���������� ���������� [OPT]   */
  u long       obj_frmt;      /* ������ ���������� ���������� [OPT]   */
  u char       tmpkey[8];     /* ���� ��������� ������        [OPT]   */
  u char       grif;          /* ����                                 */
  u char       rezerv[11];
} JSYNCOUT;
typedef JSYNCOUT   *PJSYNCOUT;

/**********************************************************************
      �������� ������ ������� ��������� �������� ���������� ������
***********************************************************************/
/* ���������� �������� �������                                        */
#define   JSYSO_ITEM_SENDDT      2
#define   JSYSO_ITEM_DOVDT       4
#define   JSYSO_ITEM_OBRBDT      8
#define   JSYSO_ITEM_OBRDT      16
#define   JSYSO_ITEM_QUERY      32
#define   JSYSO_ITEM_REQ        64
#define   JSYSO_ITEM_OBRRDT    128
#define   JSYSO_ITEM_MYKEY     256
#define   JSYSO_ITEM_FARKEY    512
#define   JSYSO_ITEM_LAPOL    1024
#define   JSYSO_ITEM_GRIF     2048
#define   JSYSO_ITEM_RESULT   4096
typedef struct _JSYSOUT{
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          send_dt;       /* ���� � ����� �������� �������        */
  IDT          dov_dt;        /* ���� � ����� ��������� �������       */
  IDT          obrb_dt;       /* ���� � ����� ���������� �� ��������� */
  IDT          obr_dt;        /* ���� � ����� ��������� �������       */
  SYS          query;         /* ���������� ������                    */
  u char       mykey[PLOTKEYSIZE]; /* ��������� ����                  */
  u char       farkey[PLOTKEYSIZE];/* �������� ����                   */
  u char       la_pol[4];     /* ���. ����� ����������                */
  long         result;        /* ��������� ����������                 */
  u char       grif;          /* ����                                 */
  u char       rezerv[15];
} JSYSOUT;
typedef JSYSOUT   *PJSYSOUT;

/**********************************************************************
      �������� ������ ������� �������� �������� ���������� ������
***********************************************************************/
/* ���������� �������� �������                                        */
#define   JSYSI_ITEM_RECVDT      2
#define   JSYSI_ITEM_OBRBDT      4
#define   JSYSI_ITEM_OBRDT       8
#define   JSYSI_ITEM_QUERY      16
#define   JSYSI_ITEM_REQ        32
#define   JSYSI_ITEM_OBRRDT     64
#define   JSYSI_ITEM_MYKEY     128
#define   JSYSI_ITEM_FARKEY    256
#define   JSYSI_ITEM_LAOTPR    512
#define   JSYSI_ITEM_GRIF     1024
#define   JSYSI_ITEM_RESULT   2048
typedef struct _JSYSIN{
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          recv_dt;       /* ���� � ����� ��������� �������       */
  IDT          obrb_dt;       /* ���� � ����� ���������� �� ��������� */
  IDT          obr_dt;        /* ���� � ����� ��������� �������       */
  SYS          query;         /* ���������� ������                    */
  u char       mykey[PLOTKEYSIZE]; /* �������� ����                   */
  u char       farkey[PLOTKEYSIZE];/* ��������� ����                  */
  u char       la_otpr[4];    /* ���. ����� �����������               */
  long         result;        /* ��������� ����������                 */
  u char       grif;          /* ����                                 */
  u char       rezerv[15];
} JSYSIN;
typedef JSYSIN   *PJSYSIN;

/* ������� ������ ��� ������                                          */
#define    MAX_SRITEM_SZ     64
#define    SRITEM_TYPE_DATE   1
#define    SRITEM_TYPE_LONG   2
#define    SRITEM_TYPE_SHORT  3
#define    SRITEM_TYPE_CHAR   4
#define    SRITEM_TYPE_HEX    5
#define    SRITEM_USL_BIG     1
#define    SRITEM_USL_SML     2
#define    SRITEM_USL_EQU     3
#define    SRITEM_USL_NOT     4
typedef struct _SRITEM {
  u long      smesh;
  u long      size;
  u long      type;
  u long      usl;
} SRITEM;
typedef SRITEM    *PSRITEM;
typedef SRITEM     MSRITEM[];
typedef MSRITEM   *PMSRITEM;

/**********************************************************************
    ������� ������� � �����
***********************************************************************/
typedef struct _STDTPREF {
  u long    stdldata;            /* ����� ������ �����                */
  u long    stdlpref;            /* ����� ��������                    */
  u long    stdlstr;             /* ����� ������ � �������            */
  union {
    ADRESS   a;
    u long   l;
  }         std_lm;              /* ����� ���������� (4�)             */
  u char    std_code;            /* ���������                         */
  u char    std_rez[15];         /* ������                            */
} STDTPREF;                      /* ��� ������                        */
typedef STDTPREF   *PSTDTPREF;   /* ��������� �� ������               */

/**********************************************************************
    ��������� PING
***********************************************************************/
typedef struct _PING {
  void    *pingadr;               /* ����� ������ � SLVADR' baul�      */
  u long   task_pid;              /* PID ������                        */
  char     rezerv[72];
} PING;                          /* ��� ������                        */
typedef PING   *PPING;           /* ��������� �� ������               */

/**********************************************************************
    ��������� �����������
***********************************************************************/
typedef struct _NTFSTR {
  char       kl_obj[3];     /* ����� �������  "DBO"- ��� ��           */
  char       dborezim;      /* ����� ������ � ��������� ������ DBO    */
  char       rezerw1[4];    /* REZERW                                 */
  u long     adr_otpr;      /* ������������� ������ ��������������    */
                            /* �������� ��� �������� ��������� ������ */
  char       kl_dok[128];   /* ���(�������������) �������             */
  IDT        ntfidt;        /* ����� � �������� ��� ��������          */
  char       rezerw2[12];   /* REZERW                                 */
  char       group_name[4]; /* ��� ������ �����                       */
  char       task_name[4];  /* ��� ������                             */
  char       data_format[8];/* ������ �����. ������ (������� �������) */
  char       rezerv3[72];   /* REZERW                                 */
} NTFSTR;                   /* ��� ������                             */
typedef NTFSTR  *PNTFSTR;   /* ��������� �� ������                    */

typedef struct _ZVD {
  u char        zvd_0[11];
  u char        zvd_dtip[4];
  u char        zvd_1;
  u char        zvd_2[10];
  u char        zvd_zv1;
  u char        zvdndsut[8];
  u char        zvd_zv2;
  u char        zvd_pr[44];
  u char        zvd_3[11];
  u char        zvd_dnm[24];
  u char        zvd_4;
  u char        zvd_5[44];
  u char        zvd_6[11];
  u char        zvd_tpr[19];
  u char        zvd_7;
  u char        zvd_8[49];
  u char        zvd_9[11];
  u char        zvd_kr1[4];
  u char        zvd_10;
  u char        zvd_kr2[16];
  u char        zvd_11[2];
  u char        zvd_kr3[7];
  u char        zvd_12;
  u char        zvd_13[38];
  u char        zvd_14[11];
  u char        zvd_dkey[8];
  u char        zvd_15;
  u char        zvd_nul[1260];
  u char        zvd_message[160];
} ZVD;
typedef ZVD      *PZVD;

/* �������� ��                                                        */
/* ���������� ���� ��                                                 */
#define   UP_FTP_I_DISK         0   /* ����                           */
#define   UP_FTP_I_VOL          1   /* ���                            */
#define   UP_FTP_I_LIB_ND       2   /* ������������ �����             */
#define   UP_FTP_I_SOLID_ND     3   /* ������ �����                   */
#define   UP_FTP_I_POSLED_ND    4   /* ���������������� �����         */
#define   UP_FTP_I_LIB_PART     5   /* ������ ����������              */
#define   UP_FTP_I_FILE         6   /* ����                           */
#define   UP_FTP_I_DIR          7   /* ����������                     */
#define   UP_FTP_I_PLOT         8   /* ��������� ������ UP            */

/* ���������� ������� ������� ��                                      */
#define   UP_FTP_RECF_FIX      2    /* �������������                  */
#define   UP_FTP_RECF_BLOCK    4    /* �������������                  */
#define   UP_FTP_RECF_UNKN     8    /* �����������                    */
#define   UP_FTP_RECF_VARY     16   /* ����������                     */
#define   UP_FTP_RECF_OVER     32   /* �����������                    */
typedef struct _FTP_I_PROP {
  char         type;        /* ��� ��                                */
  char         is_folder;   /* ���� �� �������� �������� (true/false)*/
  char         rec_format;  /* ������ ������ (���)                   */
  char         n_remain_bl; /* ���������� ������ �� 1-� ����� ���� ��*/
  u long       size;        /* ������� 4 ����� �������               */
  IDT          create_dt;   /* ���� ��������                         */
  IDT          modify_dt;   /* ���� ���������                        */
  IDT          read_dt;     /* ���� ������                           */
  u long       rec_size;    /* ������ ������ (���)                   */
  u long       blk_size;    /* ������ ����� (���)                    */
  u short      nrec_blk;    /* ���-�� ������� � ����� ��             */
  u short      nblk_trk;    /* ���-�� ������ �� �� ����� �����       */
  u long       sblockttr;   /* ����� ������� ����� ������� ����. ��  */
  u long       major_size;  /* ������� 4 ����� �������               */
  u short      key_ofs;     /* �������� ������� ����� ������� ��     */
  u short      key_len;     /* ����� ����� ������� ��                */
  u char       itemgrif;    /* ��������� (����) ��                    */
  char         reserv2[7];
} FTP_I_PROP;
typedef FTP_I_PROP   *PFTP_I_PROP;

/* ��������� ������� FTP                                              */
typedef struct _UP_FTP {
  QUERYID      queryido;    /* ������������� ����������� ������� (*)  */
  QUERYID      queryidp;    /* ������������� ���������� �������  (*)  */
  u char       is_recode;   /* ������� ������������� (true/false)     */
  u char       regim;       /* ����� ������                      (*)  */
    char       pr_wait;     /* ������� 0-�� �����, 1-����� ���������� */
  u char       place;       /* �/� ���������� ����������         (*)  */
    void      *own_adr;     /* ����������� ����� ���-�� ����-�   (*)  */
    char       reserv2[3];  /* ������                                 */
  u char       linklast;    /* �/� �������� ���������� �����          */
    long       rez_code;    /* ������� ���������� ����������     (*)  */
  u long       smesh;       /* ������� �������� � ��                  */
  u long       cb_req;      /* ������������� �����                    */
  u long       cb_rez;      /* �������������� �����                   */
  u long       item_nsize;  /* ������ ����� �� (��� rezim='L')        */

  u char       ftp_key[8];  /* ���� ������ ('G' � 'P' ��� SLONA)      */
  u long       block_first; /* ����� ����� ('G' � 'P' ��� ����)       */
  u long       block_last;  /* ����� ����� ('G' � 'P' ��� ����)       */
  u long       block_getn;  /* ���������� ����������� ������          */
  u long       block_putn;  /* ���������� ���������� ������           */

  u long       linkblkl;    /* ����� �����                       (*)  */
    long       linkblkn;    /* ����� ����� (�����)               (*)  */
    void      *linknach;    /* ����� ������ ������� ������       (*)  */
  u long       linkobll;    /* ������������ ����� ������� ������ (*)  */
  u long       linkinfl;    /* ������� ����� ���. � ���. ������  (*)  */
  u long       item_numb;   /* ���������� �� (��� rezim='L')          */

  FTP_I_PROP   itemprop;    /* �������� ��                            */
    char       name_old[256];/* ��� ��                                */
  u long       exitcode;
  char         reserv4[332];
} UP_FTP;
typedef UP_FTP   *PUP_FTP;

/* ��� ��                                                             */
typedef struct _FTP_I_NAME_EXT{
  char         disk;
  char         disk_sep;
  char         dir_sep;
  char         reserv1;
  char         dir[256];
  char         name[256];
  char         ext[256];
} FTP_I_NAME_EXT;
typedef FTP_I_NAME_EXT   *PFTP_I_NAME_EXT;

/* ��������/���                                                       */
typedef struct _FTP_ITEM{
  FTP_I_PROP   itemprop;    /* �������� ��                            */
  char         itemname[256];/* UNI ��� ��                            */
  char         modname[4];  /* ��                                     */
  QUERYID      open_id;
  char         rezerv[44];
} FTP_ITEM;
typedef FTP_ITEM   *PFTP_ITEM;

/**********************************************************************
      �������� ������ ������� ��������� �������� FTP
***********************************************************************/
/* ���������� ���� �������� FTP                                       */
#define   JFO_QUERY_OPEN         2     /* �������                     */
#define   JFO_QUERY_CLOSE        4     /* �������                     */
#define   JFO_QUERY_CREATE       8     /* �������                     */
#define   JFO_QUERY_RENAME      16     /* �������������               */
#define   JFO_QUERY_DELETE      32     /* �������                     */
#define   JFO_QUERY_READ        64     /* ������                      */
#define   JFO_QUERY_WRITE      128     /* ��������                    */
#define   JFO_QUERY_RUN        256     /* ���������                   */
#define   JFO_QUERY_GETFILE    512     /* �������� ����               */
#define   JFO_QUERY_PUTFILE   1024     /* �������� ����               */

/* ���������� �������� ������� ��������� �������� FTP                 */
#define   JFO_ITEM_SENDDT        2
#define   JFO_ITEM_MYKEY         4
#define   JFO_ITEM_QUERYCOD      8
#define   JFO_ITEM_SOURCELA     16
#define   JFO_ITEM_OBJKEY       32
#define   JFO_ITEM_NEWKEY       64
#define   JFO_ITEM_OBRDT       128
#define   JFO_ITEM_FARKEY      256
#define   JFO_ITEM_REZCODE     512
#define   JFO_ITEM_IS_READ    1024
#define   JFO_ITEM_IS_WRITE   2048
#define   JFO_ITEM_CLOSEDT    4096
#define   JFO_ITEM_GRIF       8192
typedef struct _JFTPOUT {
  u long       number;        /* ���������� ����� ������ � �������    */
  char         state;         /* ��������� ������                     */
  u char       code;          /* ��������� ������                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* ����� �������� ������                */
  IDT          modify_dt;     /* ����� ������������� ������           */
  IDT          send_dt;       /* ���� �������� �������                */
  QUERYID      my_key;        /* ��������� ���� �������               */
  u long       querycod;      /* ��� �������                          */
  u char       sourcela[4];   /* ���������� ����� ���������� �������  */
  u char       objkey[256];   /* ��� ��                               */
  u char       newkey[256];   /* ����� ��� �� (��������������)        */
  IDT          obr_dt;        /* ���� ��������� ������ �� ������      */
  QUERYID      far_key;       /* �������� ���� �������                */
  u long       rezcode;       /* ��������� ���������� �������         */
  char         is_read;       /* ������������� �� ������              */
  char         is_write;      /* ������������� �� ������              */
  u char       rezerv2[2];
  IDT          close_dt;      /* ���� �������� �� (��������)          */
  u char       grif;          /* ����                                 */
  u char       rezerv[31];
} JFTPOUT;
typedef JFTPOUT   *PJFTPOUT;

/***********************************************************************
     �������� ��������� ��� ������ ������������ �������, �����������
     ������� CMS � ���������� ������ � ��������� ����� (���������� ���
     ��������� �������� FTP ���������)
***********************************************************************/
typedef struct _OBMENFTP {
  char   *buffer;            /* �����, ���� ��������� �������� ������ */
  u long  bufflen;           /* ������ ������ (������)                */
  u long  minotvet;          /* ����������� ����������� ����� ������  */
  u long  cmdlen;            /* ����� ����������� ������� CMS         */
  char    cmscmd[74];        /* ���� ������� CMS                      */
  u char  reserv[6];         /* ������                                */
} OBMENFTP;
typedef OBMENFTP  *POBMENFTP;

/***********************************************************************
              �������� ��������� ������ ��� ��������
***********************************************************************/
typedef struct _MESSAGE_PARAMS{
  char   kvs[3];     /* ��� ���� ���������                            */
  char   rezim;      /* �����                                         */
  char   adrpol[4];  /* ����� ����������                              */
  long   reftype;    /* ��� �������� ������                           */
  char  *data;       /* ����� ������ (��� ���� � �����)               */
  long   datalen;    /* ����� ������                                  */
  char   pr_code;    /* ������� ���������                             */
  u char numcode;    /* ����� ���������                               */
  u char kat_sr;     /* ��������� ���������                           */
  u char datagrif;   /* ���� ������                                   */
  char  *format;     /* ������ ������                                 */
  char   msg_key[PLOTKEYSIZE]; /* ��������� ���� ���������            */

} MESSAGE_PARAMS;
typedef MESSAGE_PARAMS   *PMESSAGE_PARAMS;

/***********************************************************************
                 �������� ��������� ������ �� ������
***********************************************************************/
typedef struct _MESSAGE_PARSED{
  char      keytemp[PLOTKEYSIZE]; /* ���� ��������� ������            */
  char      adrotp[4];            /* ����� ����������                 */
  char      kvs[3];               /* ��� ���� ���������               */
  char      rezim;                /* �����                            */
  char      format[8];            /* ������ ������                    */
  u char    kat_sr;               /* ��������� ���������              */
  u char    datagrif;             /* ���� ������                      */
  char      reserv[6];            /* ������                           */
} MESSAGE_PARSED;
typedef MESSAGE_PARSED   *PMESSAGE_PARSED;

/**********************************************************************
           �������� ��������� ����������� ������ �����������
***********************************************************************/
typedef struct _SYNCOBMN {
  QUERYID      queryid;     /* ������������� �������                  */
  u char       syncrazm;    /* �/� ���������� ������                  */
  u char       syncinft;    /* �/� ������� ������                     */
  u char       reserv1;     /* ������                                 */
  u char       synclast;    /* ������� ���������� �����               */
    void      *syncadr;     /* ����������� ����� ���-�� ����-�        */
    long       syncprz;     /* ������� ���������� ����������          */
    void      *syncobl;     /* ����� ������ ������� ������            */
  u long       syncobll;    /* ������ ������� ������                  */
  u long       syncinfl;    /* ������� ������ ���-��� � ���. ������   */
  u long       syncblkl;    /* ������ ����� (�����)                   */
  u long       syncblkn;    /* ����� ����� (�����)                    */
  u char       syncvxod[8]; /* �������� ����� ������                  */
  u char       syncgrif;    /* ���� ����������                        */
  u char       reserv2[103];/* ������                                 */
} SYNCOBMN;
typedef SYNCOBMN  *PSYNCOBMN;

/**********************************************************************
             �������� ��������� �������� ���������� �������
                �������� ��������� � ���������� ������
***********************************************************************/
typedef struct _SYNCPARM {
  u char   address[4];  /* ����� ����������                           */
  u char   reserv1[4];  /* ������                                     */
  void    *pdata;       /* ��������� �� ������������ ������           */
  u long   datasize;    /* ������ ������������ ������                 */
  void    *preplay;     /* ��������� �� ������� ������ ��� ������     */
  u long   replsize;    /* ������ ������                              */
  u long   interval;    /* �������� �������� ������                   */
  u char   dataplace;   /* �/� �������������� ������                  */
  u char   datafrmt;    /* �/� ������� ������                         */
  u char   reserv2;     /* ������                                     */
  u char   rezim;       /* ����� �������� (������('Q') ��� �����('R') */
  char     tempkey[PLOTKEYSIZE]; /* ���� ������ �� ��������� ������   */
  u char   datakfs[3];  /* ��� ������������ ������                    */
  u char   datagrif;    /* ���� ����������                            */
  u char   reserv3[20]; /* ������                                     */
} SYNCPARM;
typedef SYNCPARM  *PSYNCPARM;

/**********************************************************************
           �������� ��������� �������� ���������� � �������
***********************************************************************/
#define   INFT_UP_SORT     2
#define   INFT_DN_SORT     4
#define   INFT_NO_SORT     8
typedef struct _INFTABLE {
  char         modul_name[5];
  char         ndata_name[9];
  char         table_name[82];
  u long       kol_pol;
  u long       kol_elem;
  u long       kolrec;
  u char       numspole;
  u char       naprsort;
  u char       separfield;
  u char       separrecord;
  char         ptable[9];
  char         rezerv1[3];
  u long       dln_zap;
  char         rezerv2;
  char         poles[255];
} INFTABLE;
typedef INFTABLE   *PINFTABLE;

/***********************************************************************
                            ������ ��������� IDT
***********************************************************************/
typedef struct _IDH {   /* ��������� : ���� - 16-� �����              */
  u short       year;   /* ���                                        */
  u char        month;  /* �����                                      */
  u char        day;    /* ����                                       */
  u char        hour;   /* ����                                       */
  u char        min;    /* ������                                     */
  u char        sec;    /* �������                                    */
  u char        dec;    /* ������� ���� �������                       */
} IDH;
typedef IDH   *PIDH;    /* ��������� �� ��������� IDH                 */

/***********************************************************************
       ��������� �������������� ����� ��������� ��� ��������� ����
***********************************************************************/
typedef struct _ICHP {
  u char    ichp_kvs[3];     /* ���                                   */
  u char    ichp_rezim;      /* ����� ������� ('R' - �����������)     */
  IDH       ichp_idh;        /* ����-����� �������                    */
  u char    ichp_aolm[4];    /* �� - ��                               */
  u char    ichp_aovu[4];    /* �� - ��                               */
  u char    ichp_aplm[4];    /* �� - ��                               */
  u char    ichp_apvu[4];    /* �� - ��                               */
  u char    ichp_id[2];      /* �������������                         */
  u char    ichp_range[2];   /* ����                                  */
  u char    ichp_spec[18];   /* ������������                          */
  u short   ichp_codopr;     /* ��� ��������                          */
  u short   ichp_codres;     /* ��� ����������                        */
  u char    ichp_klq[8];     /* ���� �������                          */
  IDH       ichp_idha;       /* ����-����� ������                     */
  u char    ichp_kla[8];     /* ���� ������                           */
  u char    ichp_idsd[2];    /* ������������� ��                      */
  u char    ichp_rangesd[2]; /* ���� ��                               */
  u char    ichp_specsd[18]; /* ������������ ��                       */
  u char    ichp_katsd[1];   /* ��������� ��                          */
  u char    ichp_rez[1];     /* ������                                */
  u char    ichp_inf[20];    /* ���. ����������                       */
  u char    ichp_rez3[17];   /* ������ 3                              */
  u char    ichp_verup[5];   /* ����� ������ UP                       */
  u char    ichp_ui[16];     /* �����: 160 �.                         */
} ICHP;
typedef ICHP      *PICHP;

/**********************************************************************
      ��������� �������������� ����� ��������� � �������� �������
***********************************************************************/
typedef struct _MDM {
  u long    mdmlenght;       /* ����� �������                         */
  u char    mdmrez1[8];      /* ������ 1                              */
  u long    mdmchksum;       /* ����������� �����                     */
  u char    mdmrangdef[2];   /* ���� �� ���������                     */
  u char    mdmrez2[2];      /* ������ 2                              */
  u long    mdmltitle;       /* ����� ���������                       */
  u char    mdmadmin[2];     /* ��� ������ �������������� ���         */
  u char    mdmaskuf[2];     /* ��� ������ �������������� ����        */
  u long    mdmdusert;       /* �������� ������� �������������        */
  u long    mdmdrangt;       /* �������� ������� ������               */
  u long    mdmdtrastdt;     /* �������� ������� ���������� �������   */
  u long    mdmdtrastgt;     /* �������� ������� ���������� �������   */
  u long    mdmdaddrst;      /* �������� ������� ����������� ���������*/
  u long    mdmdremvblt;     /* �������� ������� ��������� � ���������*/
                             /* ��� �������� � �������� ����������    */
  u long    mdmdprintt;      /* �������� ������� ��������� ������     */
  u long    mdmdacippt;      /* �������� ������� ����������� ip-������*/
  u long    mdmdrdippt;      /* �������� ������� �� � ip-������       */
  u long    mdmdregevn;      /* �������� ����� �������������� ������� */
} MDM;
typedef MDM       *PMDM;

/**********************************************************************
      ��������� ��������� ��� ������, �������� � ������� �������
***********************************************************************/
typedef struct _TMDM {
/* ��������� ������� (����� ��� ���� ������, �������� � �������)      */
  u long    tmdmlttltab;     /* ����� ���������                       */
  u long    tmdmlstrtab;     /* ����� ������                          */
  u long    tmdmnstrtab;     /* ���������� �����                      */
  u long    tmdmlfld1;       /* ����� ������� ���� ������             */
  u long    tmdmlfld2;       /* ����� ������� ���� ������             */
} TMDM;
typedef TMDM      *PTMDM;

/**********************************************************************
      ��������� ������� ����������, ������������ ����� ���������
      �������� ���������� �� ���������� ���������� ��������
***********************************************************************/
typedef struct _ACNT {
  u long    acntkod;         /* ��� ���������� ��������               */
  char     *acntashkl;       /* ��������� �� ������� �����            */
  u long    acntnbobj;       /* ����� ������������ ���� 1             */
  u long    acntnbnet;       /* ����� ������������ ���� 2             */
  u long    acntnboth;       /* ����� ������������ ���� 3             */
  u long    acntnbmy;        /* ����� ������������ ���� 4             */
  u long    acntnbok;        /* ����� ������������ ���� 5             */
  u long    acntnbno;        /* ����� ������������ ���� 6             */
  bool      acntgrifset;     /* ���� �� �����/�� ����� (TRUE/FALSE)   */
  u char    acntobjao[PLOT_MDMZUAF1]; /* ����� ��, ����������         */
  u char    acntmodao[PLOT_MDMZUAF2]; /* �������� ��� ��              */
  u char    acntobjap[PLOT_MDMZUAF1]; /* ����� ��, �� ���. �����������*/
  u char    acntmodap[PLOT_MDMZUAF2]; /* ���������� ��������          */
  u char    acntpath[PLOT_MDMZLPUN]; /* ��� �������� ��� ����������   */
  u char    acntgrif;        /* ���� ������� �������                  */
  u char    acnttip;         /* ��� �������� ��� ������� ������� 1    */
  u char    acntkc;          /* ��� �������� ��� ������� ������� 2    */
  u char    acntrez[17];     /* ������                                */
} ACNT;
typedef ACNT      *PACNT;

/**********************************************************************
                   �������� �������������� ���������
***********************************************************************/
typedef struct _VUSTATE {
  u char     modserv[80];      /* ���������� ����������� ������       */
  u char     modname[4];       /* ��� ����������� ������              */
  u char     reserv1[4];       /* ������                              */
  u char     num_vu[4];        /* ����� ��                            */
  u char     reserv2[4];       /* ������                              */
  u char     userfio[18];      /* ������� �.�. ������������           */
  u char     userrank[10];     /* ��������� ������������              */
  u char     netname[80];      /* ������������ ����                   */
  u char     rezerv3[52];      /* ������                              */
} VUSTATE;                     /* ��� ������ ��                       */
typedef VUSTATE   *PVUSTATE;   /* ��������� �� ������                 */
typedef VUSTATE    MVUSTATE[]; /* ��� ������ �����                    */
typedef MVUSTATE  *PMVUSTATE;  /* ��� ��������� �� ������ �����       */

/**********************************************************************
        ��������� ���������� ��� ������� ��������� ������� ��
***********************************************************************/
#define PLOT_NET_NO_OWNVU      0
#define PLOT_NET_YES_OWNVU     1
typedef struct _UPVIEWNET {
  void      *buffer;           /* ��������� �� �����, ���������� ���  */
                               /* ��������� ������� MVUSTATE          */
  u long     buffsize;         /* ������ ������                       */
  u long     strnum;           /* ���������� ����� �����������        */
                               /* ������� MVUSTATE                    */
  long       interval;         /* ����� �������� ������� �� �������   */
  char       tempkey[PLOTKEYSIZE]; /* ���� ��������� ������ �������-  */
                               /* ���� ������� MVUSTATE               */
  u char     ownvuflag;        /* ���� ������� ������ �������� �����  */
                               /* �� � ������� MVUSTATE               */
  u char     rplace;           /* �/� �������������� �����������      */
                               /* ������� MVUSTATE                    */
  u char     netid;            /* ������������� ���                   */
  u char     rezerv;
  u char     objcode[2];       /* ��� �������                         */
  u char     reserv[34];       /* ������                              */
} UPVIEWNET;
typedef UPVIEWNET *PUPVIEWNET; /* ��������� �� ���������              */

typedef struct _TEMAHEAD {
  char          tema_0[15];
  char          tema_1;
  char          tema_dfio[28];
  char          tema_2;
  char          tema_3[19];
  char          tema_4[5];
  char          tema_kat[10];
  char          tema_5;
  char          tema_6[15];
  char          tema_7;
  char          tema_ist1[13];
  char          tema_8[2];
  char          tema_ist2[13];
  char          tema_9;
  char          tema_10[35];
  char          tema_11[15];
  char          tema_12;
  char          tema_reg1[3];
  char          tema_13[2];
  char          tema_reg2[3];
  char          tema_14[2];
  char          tema_reg3[3];
  char          tema_15[2];
  char          tema_reg4[3];
  char          tema_16[2];
  char          tema_reg5[3];
  char          tema_17;
  char          tema_18[40];
  char          tema_19[15];
  char          tema_20;
  char          tema_tem1[3];
  char          tema_21[2];
  char          tema_tem2[3];
  char          tema_22;
  char          tema_23[55];
  char          tema_24[15];
  char          tema_25;
  char          tema_date[8];
  char          tema_26;
  char          tema_time[8];
  char          tema_27[47];
  char          tema_28[15];
  char          tema_29;
  char          tema_msgh[55];
  char          tema_30;
  char          tema_31[8];
  char          tema_32[15];
  char          tema_33;
  char          tema_kwd1[8];
  char          tema_34[2];
  char          tema_kwd2[8];
  char          tema_35[2];
  char          tema_kwd3[8];
  char          tema_36[2];
  char          tema_kwd4[8];
  char          tema_37[2];
  char          tema_kwd5[8];
  char          tema_38[2];
  char          tema_kwd6[8];
  char          tema_39;
  char          tema_40[5];
  char          tema_41[15];
  char          tema_42;
} TEMAHEAD;
typedef TEMAHEAD     *PTEMAHEAD;

typedef struct _TEMASTR {
  IDT           temadttm;     /* ���� � ����� �������                 */
  long          reftype;      /* ��� �������� ������                  */
  char         *data;         /* ����� ������ (��� ���� � �����, ���  */
                              /* ���� ��������� ������)               */
  u long        datalen;      /* ������ ������                        */
  char          address[4];   /* ����� ����������                     */
  char          reserv1[4];   /* ������                               */
  u char        format;       /* ������ ������                        */
  char          datakat;      /* ��������� ������ (1 - 4)             */
  char          msgzag[55];   /* ��������� ���������                  */
  char          ist1[13];     /* �������� 1                           */
  char          ist2[13];     /* �������� 2                           */
  char          tem[3];       /* ����                                 */
  char          subtem[3];    /* �������                              */
  char          reg1[3];      /* ������/��������� 1                   */
  char          reg2[3];      /* ������/��������� 2                   */
  char          reg3[3];      /* ������/��������� 3                   */
  char          reg4[3];      /* ������/��������� 4                   */
  char          reg5[3];      /* ������/��������� 5                   */
  char          keyword1[8];  /* �������� ����� 1                     */
  char          keyword2[8];  /* �������� ����� 2                     */
  char          keyword3[8];  /* �������� ����� 3                     */
  char          keyword4[8];  /* �������� ����� 4                     */
  char          keyword5[8];  /* �������� ����� 5                     */
  char          keyword6[8];  /* �������� ����� 6                     */
  char          tempkey[8];   /* ���� ��������� �� ��������� ������   */
  char          reserv2[12];  /* ������                               */
} TEMASTR;
typedef TEMASTR      *PTEMASTR;

typedef struct _SZI {
  IDH       szi_idh;        /* ����-����� �������                     */
  u short   szi_codopr;     /* ��� ��������                           */
  u short   szi_codres;     /* ��� ����������                         */
  u char    szi_aolm[4];    /* �� - ��                                */
  u char    szi_aovu[4];    /* �� - ��                                */
  u char    szi_aplm[4];    /* �� - ��                                */
  u char    szi_apvu[4];    /* �� - ��                                */
  u char    szi_id[2];      /* �������������                          */
  u char    szi_range[2];   /* ����                                   */
  u char    szi_spec[18];   /* ������������                           */
  u char    szi_idsd[2];    /* ������������� ��                       */
  u char    szi_rangesd[2]; /* ���� ��                                */
  u char    szi_specsd[256]; /* ������������ ��                       */
  u char    szi_katsd;      /* ��������� ��                           */
  u char    szi_inf[80];    /* ���. ����������                        */
  u char    szi_rezim;      /* ����� ������� ('R' - �����������)      */
  u char    szi_klq[PLOTKEYSIZE]; /* ���� �������                     */
} SZI;
typedef SZI      *PSZI;

typedef struct _SYS_USER_INFO {
  char      username[64];   /* ����� ������������ ��                  */
  char      userinfo[128];  /* �������� ������������ ��               */
  u char    usergrifmin;    /* ���� ������������ (' ' - �� �����)     */
  u char    usergrifmax;    /* ���� ������������ (' ' - �� �����)     */
  char      rezerv[2];
  char      sysgrif[256];   /* ��������� ����� �� ��� ������������    */
                            /* LINUX, MCBC - struct mac_user (mac.h)  */
} SYS_USER_INFO;
typedef SYS_USER_INFO      *PSYS_USER_INFO;

typedef struct _UP_MEMHDR{
  u long  how;
  u long  task;
  u long  tid;
  u long  func;
  char    rezerv[16];
} UP_MEMHDR;
typedef UP_MEMHDR   *PUP_MEMHDR;

typedef struct _UP_MEMFOOTER{
  char    rezerv[32];
} UP_MEMFOOTER;
typedef UP_MEMFOOTER   *PUP_MEMFOOTER;

/* ������� ��� �������� ���������                                     */
typedef struct _SLV21Y009 {
  u char    kvs[3];            /* ��� ���� ���������                  */
  u char    nm_field[8];       /* ������������� ��� ���� ��� ����-�   */
  u char    usl;               /* ������� �����-� (����� - �� �����)  */
  u long    testsz;            /* ������� �. �� ������ ���� ��������  */
  u char    test[16];          /* � ��� ��������                      */
  u char    preproc;           /* ������� ��������������� ���������   */
  u char    is_null;           /* ������� ������ ��� ���              */
  u char    to_send[128];      /* ������ ������� ��� �������� (',')   */
  u char    rezerv[94];
} SLV21Y009;
typedef SLV21Y009   *PSLV21Y009;

/**********************************************************************
           �������� ��������� ���������� �������� � ��������
***********************************************************************/
typedef struct _CASEOBMN {
  QUERYID      queryido;    /* ������������� ����������� �������      */
  QUERYID      queryidp;    /* ������������� ���������� �������       */
  u char       caseinft;    /* �/� ������� ������                     */
  u char       caserezim;   /* ����� ������                           */
  u char       casegrif;    /* ��������� (����) ����������            */
  u char       caserazm;    /* �/� ���������� ������                  */
    void      *caseadr;     /* ����������� ����� ���-�� ����-�        */
    long       caseprz;     /* ������� ���������� ����������          */
    void      *caseobl;     /* ����� ������ ������� ������            */
  u long       caseobll;    /* ������ ������� ������                  */
  u long       caseinfl;    /* ������� ������ ���-��� � ���. ������   */
  u long       caseblkl;    /* ������ ����� (�����)                   */
  u long       caseblkn;    /* ����� ����� (�����)                    */
  u char       caselast;    /* ������� ���������� �����               */
  u char       reserv1[63]; /* ������                                 */
} CASEOBMN;
typedef CASEOBMN   *PCASEOBMN;

/* ����������� ��������� ��� ������ � ��������� ������ UP             */
#define UP_CASE_MAXLEVELSNUM       16    /* ������������ ����� �������*/
                                         /* ����������� ��������� ��  */
                                         /* �������                   */
#define UP_CASE_SUBJECTLEN         256   /* ������������ �����        */
                                         /* ������������ ����(�������)*/
                                         /* ������� (�����)           */
#define UP_CASE_SUBJECTSLISTSIZE   25700 /* ������������ ������ ������*/
                                         /* ��� �������               */
#define UP_CASE_SUBJECTSEPARATOR   "\1"  /* ����������� ��� � ������  */
#define UP_CASE_XML_CODE           PLOT_CODE_ANSI /* ���������,       */
                                         /* � ������� ��������        */
                                         /* XML-�������� ������       */
                                         /* (������� ������ ��)       */
#define UP_CASE_YNDEX_KEY          "Yndex"  /* ���� ���������-������� */
                                         /* ������ ��                 */
/**********************************************************************
                 �������� ��������� ������� ������ UP
***********************************************************************/
typedef struct _UP_CASE {
    char       casename[256];    /* ��� �������                       */
    char       case_nd[8];       /* ��� ������ ������                 */
    char       reserv1[8];       /* ������                            */
    char       case_key[6];      /* ���� �������                      */
    char       reserv2[10];      /* ������                            */
  u char       m_keeper[4];      /* "������-���������" �������        */
  u char       reserv3[4];       /* ������                            */
  u char       m_creator[4];     /* "������-���������" �������        */
  u char       reserv4[4];       /* ������                            */
    IDT        case_create;      /* ���� �������� �������             */
    IDT        case_change;      /* ���� ������������� �������        */
  u char       case_fiousr[18];  /* ��� "������������-���������"      */
  u char       case_postusr[10]; /* ���������                         */
                                 /* "������������-���������"          */
  u char       case_idusr[2];    /* �������������                     */
                                 /* "������������-���������"          */
  u char       casegrif;         /* ��������� (����) �������          */
  u char       reserv5[161];     /* ������                            */
} UP_CASE;
typedef UP_CASE     *PUP_CASE;

/**********************************************************************
                  �������� ��������� ����������� �������
***********************************************************************/
typedef struct _SHOWCASE {
  PUP_CASE     pup_case;      /* ��������� �� UP_CASE �������         */
  char        *sub_buff;      /* ��������� �� ������ ��� �������      */
  u long       sub_size;      /* ������ ������ ��� �������            */
  u char       reserv[20];    /* ������                               */
} SHOWCASE;
typedef SHOWCASE    *PSHOWCASE;   /* ��������� �� ���������           */

/**********************************************************************
            �������� ������ �� ������ �� ������� ������ UP
***********************************************************************/
typedef struct _CASEXLINK {
  char         modname[4];   /* "������-���������" �������            */
  char         obj_path[512];/* ������ ���� � �������                 */
  char         obj_name[256];/* ��� (����) �������                    */
  IDT          obj_date;     /* ���� ������������� �������            */
  u short      obj_format;   /* ������ ������������� �������          */
  u short      obj_arh;      /* ��� ��������� �������                 */
  char         obj_extn[256];/* ���������� ����� �������              */
  u short      extn_type;    /* ��� ���������� ����� �������          */
  u short      extn_reserv;  /* ��������� ����                        */
} CASEXLINK;
typedef CASEXLINK   *PCASEXLINK;   /* ��������� �� ������             */
typedef CASEXLINK    MCASEXLINK[]; /* ������ ������                   */
typedef MCASEXLINK  *PMCASEXLINK;  /* ��������� �� ������ ������      */

/* ����������� ����� ��������� ������ ������ UP                       */
#define UP_CASE_FOLDER             0     /* �����                     */

#define UP_CASE_XLINK_DOCUMENT     10    /* ������ �� �������� �� UP  */
#define UP_CASE_XLINK_TABLE        11    /* ������ �� ������� �� UP   */
#define UP_CASE_XLINK_SHOWCASE     12    /* ������ �� ������� UP      */
#define UP_CASE_XLINK_SEGMDOC      13    /* ������ �� ����������������*/
                                         /* �������� �� UP            */

#define UP_CASE_XLINK_SCRIPT       50    /* �������� UP               */

#define UP_CASE_XLINK_FILE         100   /* ������ �� ����            */
#define UP_CASE_XLINK_EXEC         101   /* ������ �� �����������     */
                                         /* ������                    */

#define UP_CASE_XLINK_EXTERNAL     200   /* ������� ������ (http, ftp */
                                         /* � �.�.)                   */

/* ����������� ����� ��������� �������������� �������� (�������       */
/* ������� �������)                                                   */
#define IO_GENERATIONS_NO            0   /* ��� ��������� (�������)   */
#define IO_GENERATIONS_HOURLY        10  /* ��������                  */
#define IO_GENERATIONS_DAILY         20  /* ����������                */
#define IO_GENERATIONS_WEEKLY        30  /* �����������               */
#define IO_GENERATIONS_MONTHLY       40  /* ����������                */
#define IO_GENERATIONS_QUARTERLY     50  /* �������������             */
#define IO_GENERATIONS_YEARLY        60  /* ��������                  */
#define IO_GENERATIONS_MODIFICATION  100 /* �� ����� ���������        */

/**********************************************************************
             �������� ��������� �������� ������� ������ UP
***********************************************************************/
typedef struct _CASEITEM {
    char       item_index[UP_CASE_MAXLEVELSNUM][PLOTKEYSIZE];
    char       xlink_text[256]; /* ����� ������ (��� �����)           */
    CASEXLINK  xlink;           /* ������ �� ������                   */
    IDT        item_date;       /* ����-����� ��������� ��������      */
                                /* �� �������                         */
  u short      xlink_type;      /* ��� ������ (�������� �������)      */
  u char       xlink_kat;       /* ��������� (����) �������� �������  */
    char       comment[209];    /* ���� ���������� (�������� ����)    */
} CASEITEM;
typedef CASEITEM    *PCASEITEM;   /* ��������� �� ���������           */
typedef CASEITEM     MCASEITEM[]; /* ������ ��������                  */
typedef MCASEITEM   *PMCASEITEM;  /* ��������� �� ������ ��������     */

/**********************************************************************
        �������� ��������� �������� ������� ������ UP ���� "�����"
***********************************************************************/
typedef struct _CASEFOLDER {
  CASEITEM   folder_prop;  /* �������� �����, ��� �������� �������    */
  char       f_subject[UP_CASE_SUBJECTLEN];     /* ���� �����         */
  char       f_sub_subject[UP_CASE_SUBJECTLEN]; /* ������� �����      */
  char       f_region[UP_CASE_SUBJECTLEN];      /* "������" �����     */
  char       f_sub_region[UP_CASE_SUBJECTLEN];  /* "���������" �����  */
} CASEFOLDER;
typedef CASEFOLDER    *PCASEFOLDER;   /* ��������� �� ���������       */
typedef CASEFOLDER     MCASEFOLDER[]; /* ������ ��������              */
typedef MCASEFOLDER   *PMCASEFOLDER;  /* ��������� �� ������ �������� */

/**********************************************************************
  �������� ��������� ���������� ��� ������� ������� ������ � ���������
        ������ UP (������ �������, ������ ������� ������ �� � �.�.)
***********************************************************************/
#define UP_CASE_AS_STRUCTURE       0  /* ����������� �������������    */
                                      /* ����������� ������� (������� */
                                      /* ������ ��)                   */
#define UP_CASE_AS_XML             1  /* ������������� �����������    */
                                      /* ������� (������� ������ ��)  */
                                      /* � ������� XML                */
typedef struct _CASEWORK {
    void     *work_buff;     /* ��������� �� ����� ��� ������         */
  u long      buff_size;     /* ������ ������ ��� ������              */
  u long      real_read;     /* ������ ���������� ������              */
  u char      view_type;     /* ��� (������) ������������� ������     */
                             /* (�����������, XML � �.�.)             */
  u char      reserv1;       /* ������                                */
  u char      bo_place;      /* �/� ���������� ���������� ������      */
  u char      reserv2;       /* ������                                */
    long      interval;      /* �������� �������� ��������� ��������  */
  u char      data_grif;     /* ���� ���������� ������                */
  u char      reserv3[11];   /* ������                                */
} CASEWORK;
typedef CASEWORK    *PCASEWORK;   /* ��������� �� ���������           */

/**********************************************************************/
/* ��������� � ��������� ��� ���������� SELECT � �������� ��������� UP*/
/**********************************************************************/
#define UP_DINSELECTDELETE   2     /* ������� ������                  */
#define UP_DINSELECTTABLE    4     /* ������� ������                  */
typedef struct _DINSELECT {
  u char    count;             /* ����� �����                         */
  u char    namepol[80];       /* ��� ����                            */
  u char    usl[2];            /* ������� �����-� (>,>=,<,<=,==,!=)   */
  u char    rezerv[7];         /* ������                              */
  u short   dlpol;             /* ����� ����                          */
  u char    typepol;           /* ��� ����                            */
  u char    typecon;           /* ��� ���������                       */
} DINSELECT;
typedef DINSELECT   *PDINSELECT;

/***********************************************************************
                     ��������� IDS - ������ � �����
***********************************************************************/
typedef struct _IDS {   /* ��������� : ���� - 16-� �����              */
  u char       day[2];  /* ����                                       */
  u char       raz1;    /* .                                          */
  u char       month[2];/* �����                                      */
  u char       raz2;    /* .                                          */
  u char       year[4]; /* ���                                        */
  u char       raz3;    /*                                            */
  u char       hour[2]; /* ����                                       */
  u char       raz4;    /* :                                          */
  u char       min[2];  /* ������                                     */
  u char       raz5;    /* :                                          */
  u char       sec[2];  /* �������                                    */
} IDS;
typedef IDS   *PIDS;    /* ��������� �� ��������� IDS                 */

/**********************************************************************
        ��������� ���������� �� ��� �������� ��������� ������� �
                      ������������� ���������� UP
***********************************************************************/
typedef struct _ALBACASE {
  u long    upcase_smech;     /* �������� ��������� UP_CASE �� ������ */
                              /* ALBACASE                             */
  u long    tema_smech;       /* �������� ������ ��� ������� �� ������*/
                              /* ALBACASE                             */
  u long    tema_size;        /* ������ ������ ��� �������            */
  u long    items_smech;      /* �������� ������ ��������� ������� �� */
                              /* ������ ALBACASE                      */
  u long    items_size;       /* ������ ������ ��������� �������      */
  u char    reserv[44];       /* ������                               */
} ALBACASE;
typedef ALBACASE  *PALBACASE;    /* ��������� �� ��������� ALBACASE   */
typedef ALBACASE   MALBACASE[];  /* ��� ������ �����                  */
typedef MALBACASE  *PMALBACASE;  /* ��� ��������� �� ������ �����     */

/***********************************************************************
       ��������� ���������� ���������� (�������) �������� �������
***********************************************************************/
typedef struct _SC_ITEM_CHANGE {
  u long    if_item_name;     /* ������� ��������� ������������       */
                              /* �������� �������                     */
    char    item_name[256];   /* ����� �������� ������������ �������� */
                              /* �������                              */
  u long    if_obj_mod;       /* ������� ��������� ����� ������-
                              /* ��������� ��������������� �������    */
    char    obj_module[4];    /* ����� �������� ����� ������-���������*/
                              /* ��������������� �������              */
    char    reserv1[4];       /* ������                               */
  u long    if_obj_path;      /* ������� ��������� ���� �             */
                              /* ��������������� �������              */
    char    obj_path[512];    /* ����� �������� ���� � ���������������*/
                              /* �������                              */
  u long    if_obj_name;      /* ������� ��������� ����� (�����)      */
                              /* ��������������� �������              */
    char    obj_name[256];    /* ����� �������� ����� (�����)         */
                              /* ��������������� �������              */
  u long    if_obj_date;      /* ������� ��������� ���� ������������� */
                              /* ��������������� �������              */
    IDT     obj_date;         /* ����� �������� ���� �������������    */
                              /* ��������������� �������              */
  u long    if_obj_format;    /* ������� ��������� �������� �������   */
                              /* ������������� ��������������� �������*/
  u short   obj_format;       /* ����� �������� ������� ������������� */
                              /* ��������������� �������              */
    char    reserv2[2];       /* ������                               */
  u long    if_obj_arh;       /* ������� ��������� ���� ���������     */
                              /* ��������������� �������              */
  u short   obj_arh;          /* ����� �������� ���� ���������        */
                              /* ��������������� �������              */
    char    reserv3[2];       /* ������                               */
  u long    if_obj_extn;      /* ������� ��������� ���������� �����   */
                              /* ��������������� �������              */
    char    obj_extn[256];    /* ����� �������� ���������� �����      */
                              /* ��������������� �������              */
  u long    if_obj_extn_type; /* ������� ��������� ���� ����������    */
                              /* ����� ��������������� �������        */
  u short   obj_extn_type;    /* ����� �������� ���� ���������� ����� */
                              /* ��������������� �������              */
    char    reserv4[2];       /* ������                               */
  u long    if_xlink_type;    /* ������� ��������� ���� ������        */
                              /* �� �������������� ������             */
  u short   xlink_type;       /* ����� �������� ���� ������ ��        */
                              /* �������������� ������                */
    char    reserv5[2];       /* ������                               */
  u long    if_item_keywords; /* ������� ��������� ������� ��������   */
                              /* ���� �������� �������                */
    char    item_keywords[209]; /* ����� �������� �������� ����       */
                              /* �������� �������                     */
    char    reserv6[7];       /* ������                               */
  u long    if_item_subject;  /* ������� ��������� ������������ ����  */
                              /* (��� ��������� ���� "�����")         */
    char    item_subject[256]; /* ����� �������� ������������ ����    */
  u long    if_item_sub_subject; /* ������� ��������� ������������    */
                              /* ������� (��� ��������� ���� "�����") */
    char    item_sub_subject[256]; /* ����� �������� ������������     */
                              /* �������                              */
  u long    if_item_region;   /* ������� ��������� ������������       */
                              /* ������� (��� ��������� ���� "�����") */
    char    item_region[256]; /* ����� �������� ������������ �������  */
  u long    if_item_sub_region; /* ������� ��������� ������������     */
                              /* ��������� (��� ��������� ����        */
                              /* "�����")                             */
    char    item_sub_region[256]; /* ����� �������� ������������      */
                              /* ���������                            */
  u long    if_obj_reserv;    /* ������� ��������� �������� ����������*/
                              /* ���� ��� �������                     */
  u short   obj_reserv;       /* ����� �������� ���������� ���� ���   */
                              /* �������                              */
    char    reserv7[2];       /* ������                               */
  u char    reserv[452];      /* ����� ������                         */
} SC_ITEM_CHANGE;
typedef SC_ITEM_CHANGE  *PSC_ITEM_CHANGE; /* ��������� �� ���������   */
typedef SC_ITEM_CHANGE   MSC_ITEM_CHANGE[]; /* ��� ������ �����       */
typedef MSC_ITEM_CHANGE  *PMSC_ITEM_CHANGE; /* ��������� �� ������    */

/***********************************************************************
                     ���������, ����������� ��������
***********************************************************************/
typedef struct _SCR_PARAMS {
  char    adr[4];        /* ����� ������-����������� ��������         */
  char    rezerw1[4];    /* ������                                    */
  char    scrpath[PLOTPATHSIZE];   /* ������� �������� �              */
                                   /* ������-���������, ������:       */
                         /*  #AA$$/scr_doc  - DBO                     */
                         /*  &AA$$/C/script - FTP                     */
  DTS     scrstart;      /* ����� ������� �������� �� ����������      */
                         /*         {0} - ��� ��������                */
  u char  scrkey[PLOTKEYSIZE]; /*������������� ������������� ���������*/
  void    *data;         /* ����� ������ ��� ��������� (���� �����)   */
  u long  datalen;       /* ������ ������ ��� ��������� (���� �����)  */
  u char  loginf;        /* ����-��������� ���������                  */
                         /*         'P' - �������� ���������          */
                         /*   (SYSR � ����������)                     */
  u char  razmesh;       /* ����-��������� ���������� ���������       */
                         /*   (���� �����)                            */
                         /*  'V' - �������� �� ��������� ������,      */
                         /*  � ������ data - ���� ��������� ������    */
                         /*  '&' - �������� � ������ data             */
  char    rezerw2[2];    /* ������                                    */
  long    interval;      /* �������� �������� (���� �����)            */
                         /*          0 - ����������� ����� �������    */
  char    rezerw3[24];   /* ������                                    */
} SCR_PARAMS;
typedef SCR_PARAMS *PSCR_PARAMS;  /* ��������� �� ���������           */

/***********************************************************************
                     ���������, ����������� �������
***********************************************************************/
typedef struct _VNT_PARAMS{
  u char   vnt_id[4];        /* ������������� �������                 */
  u char   rezerw1[4];       /* ������ 1                              */
  u char   group_name[4];    /* ��� ������ �����                      */
  u char   task_name[4];     /* ��� ������                            */
  u char   data_format[8];   /* ������ �����. ������ (������� �������)*/
  u char   uscr_path[PLOTPATHSIZE]; /* ������� �������� ����������    */
  u char   rezerw2[96];      /* ������ 2                              */
  u long   rezerw3[16];      /* ������ 3                              */
  u short  rezerw4[32];      /* ������ 4                              */
} VNT_PARAMS;
typedef VNT_PARAMS *PVNT_PARAMS;  /* ��������� �� ���������           */
/***********************************************************************
       ���������� � ����������, ���������� � ����������� �������
***********************************************************************/
typedef struct _TASK_PARAMS{
  char   group_name[4];    /* ��� ������ �����                        */
  char   task_name[4];     /* ��� ������                              */
  char   data_format[8];   /* ������ �����. ������ (������� �������)  */
  char   rezerw[48];       /* ������                                  */
} TASK_PARAMS;
typedef TASK_PARAMS *PTASK_PARAMS;  /* ��������� �� ���������         */
/***********************************************************************
         ���������, ����������� ����������� ������� ���������
***********************************************************************/
typedef struct _SLVVNT {
  char   vnt_id[4];             /* ������������� �������              */
  char   vnt_react;             /* ���������� �� �������              */
  char   react_type;            /* ������������� ���� �������         */
  char   adr_isp[4];            /* ����� �����������                  */
  char   react_id[PLOTPATHSIZE];/* ������������� ������               */
  DTS    vnt_start;             /* ����� ������� �������              */
  char   rezerw1[40];           /* ������ 1                           */
  char   rezerw2[40];           /* ������ 2                           */
} SLVVNT;
typedef SLVVNT   *PSLVVNT;          /* ��������� �� ������            */
typedef SLVVNT    MSLVVNT[];        /* ��� ������ ����� - ��� ������� */
typedef MSLVVNT  *PMSLVVNT;         /* ��� ��������� �� �������       */

/***********************************************************************
         ��������� ������ ������� ��������������� ��������������
                         ������������ � ���������
***********************************************************************/
typedef struct _DIGSIGN {
  short   NumVersion;             /* ����� ������                     */
  char    reserv1[2];             /* ������                           */
  char    KodeObj[2];             /* ��� ������� �����������          */
  char    reserv2[2];             /* ������                           */
  char    KodeMod[2];             /* ��� ������ �����������           */
  char    reserv3[2];             /* ������                           */
  char    IdUser [2];             /* ������������� �����������        */
  char    reserv4[2];             /* ������                           */
  char    Signature_r[128];       /* ��� ����������, ��� � ���������, */
  char    Signature_s[128];       /* ��� �������� �������� �������    */
  char    Detail[32];             /* ���� ��� �������                 */
  char    reserv5[32];            /* ������                           */
} DIGSIGN;
typedef DIGSIGN    *PDIGSIGN;

/***********************************************************************
            ��������� ������ ������� ������ ������ �������
***********************************************************************/
typedef struct _SSLVZKBDS {
  char    NumVersion[2];          /* ����� ������                     */
  char    reserv1[2];             /* ������                           */
  char    KodeObj[2];             /* ��� �������                      */
  char    reserv2[2];             /* ������                           */
  char    KodeMod[2];             /* ��� ������                       */
  char    reserv3[2];             /* ������                           */
  char    IdUser[2];              /* ������������� ������������       */
  char    reserv4[2];             /* ������                           */
  char    PrivatKey[128];         /* �������� ����                    */
  char    OpenKey_X[128];         /* ��� ����������, ��� � ���������, */
  char    OpenKey_Y[128];         /* ��� �������� ��������� �����     */
  char    Detail[32];             /* ���� ��� �������                 */
  char    reserv5[32];            /* ������                           */
  char    Status;                 /* ������ �����                     */
                                  /*   a - ������������               */
                                  /*   c - ����������������           */
                                  /*   o - ���������                  */
  char    reserv6[3];             /* ������                           */
  char    ProductionTime[8];      /* ����� ������ �����               */
  char    LifeTime[8];            /* ������ �������, �� ��������      */
                                  /* ����� ����                       */
} SSLVZKBDS;
typedef SSLVZKBDS    *PSSLVZKBDS;

/***********************************************************************
          ��������� ������� � ������� ������ ������ ��������
***********************************************************************/
typedef struct _SSLVOKBDS {
  char    NumVersion[2];          /* ����� ������                     */
  char    reserv1[2];             /* ������                           */
  char    KodeObj[2];             /* ��� �������                      */
  char    reserv2[2];             /* ������                           */
  char    KodeMod[2];             /* ��� ������                       */
  char    reserv3[2];             /* ������                           */
  char    IdUser [2];             /* ������������� ������������       */
  char    reserv4[2];             /* ������                           */
  char    OpenKey_X[128];         /* ��� ����������, ��� � ���������, */
  char    OpenKey_Y[128];         /* ��� �������� ��������� �����     */
  char    Detail[32];             /* ���� ��� �������                 */
  char    reserv5[32];            /* ������                           */
  char    Status;                 /* ������ �����                     */
                                  /*   a - ������������               */
                                  /*   c - ����������������           */
                                  /*   o - ���������                  */
  char    reserv6[3];             /* ������                           */
  char    ProductionTime[8];      /* ����� ������ �����               */
  char    LifeTime[8];            /* ������ �������, �� ��������      */
                                  /* ����� ����                       */
} SSLVOKBDS;
typedef SSLVOKBDS    *PSSLVOKBDS;

/***********************************************************************
              ��������� ������ � ������� ������������� ��
***********************************************************************/
typedef struct _SLMP {
  char    UserNameTh[64];      /* ��� ������                          */
  char    UserNameSh[18];      /* ��� �����������                     */
  char    reserv1[2];          /* ������ 1                            */
  char    UserUniqueId[8];     /* ��� � �����������                   */
  char    UserIdUP[2];         /* ��� � UP                            */
  char    reserv2[2];          /* ������ 2                            */
  char    NameObjSh[32];       /* ������������ ��� �����������        */
  char    PostSh[32];          /* ����������� ������������ ���������  */
  char    EMail[64];           /* ����� ����������� �����             */
  char    OpenKey_X[128];      /* ��� ����������, ��� � ���������,    */
  char    OpenKey_Y[128];      /* ��� �������� ��������� �����        */
  char    Comment[32];         /* ����������                          */
  char    reserv[256];         /* ����� ������                        */
} SLMP;
typedef SLMP    *PSLMP;
typedef SLMP     MSLMP[];
typedef MSLMP   *PMSLMP;

/***********************************************************************
        ��������� ������ � ������� �������� ���������� �����������
***********************************************************************/
typedef struct _SLMS {
  char    PostTh[128];         /* ������ ������������ ���������       */
  char    PostSh[32];          /* ����������� ������������ ���������  */
  char    KodeMod[2];          /* ��� ����������� ������              */
  char    reserv1[2];          /* ������ 1                            */
  char    PostTabNum[10];      /* ��������� �����                     */
  char    reserv2[2];          /* ������ 2                            */
  char    EMail[64];           /* ����� ����������� �����             */
  char    Comment[24];         /* ����������                          */
  char    reserv[248];         /* ����� ������                        */
} SLMS;
typedef SLMS    *PSLMS;
typedef SLMS     MSLMS[];
typedef MSLMS   *PMSLMS;

/***********************************************************************
 ��������� �������� ������ ��� ������� �������� ���������� �����������
***********************************************************************/
typedef struct _ListSLMS {
  void      *NextElem;         /* ����� ���������� �������� �������   */
                               /* ������                              */
  PSLMS      AdrLMS;           /* ����� ������� �� ������� ���������� */
  u long     NumRec;           /* ����� ������� � �������             */
  char       reserv[4];        /* ������                              */
} ListSLMS;
typedef ListSLMS    *PListSLMS;
typedef ListSLMS     MListSLMS[];
typedef MListSLMS   *PMListSLMS;

/***********************************************************************
             ��������� ������ � ������� - ����������� ���
***********************************************************************/
typedef struct _SRDO {
  char    NameObjTh[128];      /* ��� ������� �����������             */
  char    NameObjSh[32];       /* ��� ������� �����������             */
  char    ObjId[2];            /* ���������� ������������� �������    */
  char    reserv1[2];          /* ������ 1                            */
  char    LevelObj[4];         /* ������� ������� � ��������          */
  u long  TypeLMS;             /* ��� ������� ���������               */
  char    EMail[64];           /* ����� ����������� �����             */
  u long  TypeObj;             /* ��� �������                         */
  long    Coordinates[4];      /* ���������� �������                  */
  char    reserv[256];         /* ����� ������                        */
} SRDO;
typedef SRDO    *PSRDO;
typedef SRDO     MSRDO[];
typedef MSRDO   *PMSRDO;

/***********************************************************************
         ��������� ���������� ������, ����������� �������������
***********************************************************************/
typedef struct _USTASKINFO {
  u short       build;       /* ����� ������ ����������               */
  u char        major;       /* ����� ������ ����������               */
  u char        minor;       /* ����� ������ ����������               */
  u char        design[16];  /* ������ ������������ ����������        */
  u char        online;      /* ������� (^=0) �����������������       */
                             /* ��������                              */
  char          reserv[43];  /* ������                                */
} USTASKINFO;
typedef USTASKINFO   *PUSTASKINFO;

/**********************************************************************
               ��������� �� ���������� ������� �������
***********************************************************************/
#define UP_VU_INFO_STATUS_ON   0
#define UP_VU_INFO_STATUS_OFF  1
typedef struct _UP_VU_INFO {
  u char       os[128];
  u char       user_name[128];
  u char       vu_name[128];
  u char       cpu[128];
  u char       ram[128];
  u char       video[128];
  u char       hdd_phys[16][64];
  u char       hdd_logical[32][128];
  u char       hdd_cd[4][128];
  u char       printer[128];
  u char       modem[128];
  u char       status;
  u char       rezerv[18943];
} UP_VU_INFO;
typedef UP_VU_INFO   *PUP_VU_INFO;
typedef UP_VU_INFO    MUP_VU_INFO[];
typedef MUP_VU_INFO  *PMUP_VU_INFO;

/*=========== � � � � � � � � � � �  � � � � � � � � (�����) =========*/

#ifdef WINNT    /* ��� ������ ���������� ���������                    */
  } /* namespace Plot */
  #if !defined(NO_IMPLICIT_NAMESPACE_USE)
  using namespace Plot;
  #endif
#endif /*WINNT*/

/*=========== � � � � � � � � � � �  � � � � � � � (������) ==========*/

/* ������� ���������� ���������� ������                               */
#define    Fabnd                 1
PLOTLIB void WINAPI abnd(PSTKMPR pstk,
                         char   *message,
                         void   *data,
                         u long  sz_data);

/* �p���p�������� count �������� �� ��p��� string � ����� ���� long   */
PLOTLIB long WINAPI atoln(char *string,
                          int   count);

/* ������� ������ � �������������                                     */
char *get_element(char *wrk,
                  char *str,
                  int   ind,
                  char *t);

/* ��������� �� ������� ������ count, �������������� �� ������ pmass  */
/* ������ rezult ���� char*                                           */
PLOTLIB long WINAPI coded(u char *rezult,
                          u char *pmass,
                          long    count);

/* ������� �������� ���������� ������� ��� ������������ ����������    */
#define    FCreateEnq            2
PLOTLIB long WINAPI CreateEnq(PSTKMPR pstk,
                              void   *resname,
                              long    nmsz);

/* ��������� �� ������� ������ count, �������������� �� ������ pmass  */
/* ������ rezult ���� char*                                           */
PLOTLIB long WINAPI decod(u char *rezult,
                          u char *pmass,
                          long    count);

/* ������� ������ ������������ ����������                             */
#define    Fdeq                  3
PLOTLIB long WINAPI deq(PSTKMPR pstk,
                        void   *pmass,
                        long    count);

/* ������� ��������� ����� ��������                                   */
#define    Fdumpproc             4
PLOTLIB long WINAPI dumpproc(PSTKMPR pstk);

/* ������� ������ ������������ ����������                             */
#define    Fenq                  5
PLOTLIB long WINAPI enq(PSTKMPR pstk,
                        void   *pmass,
                        long    count);

/* ������� ������������ ����� ������, ������������������ �-�� GetMem  */
#define    FFreeMem              6
PLOTLIB long WINAPI FreeMem(PSTKMPR pstk,
                            void   *addr_obl);

/* ������� ��������� ����/�������                                     */
PLOTLIB void WINAPI GetCurrentDateTime(IDT  apprx,
                                       PIDT pcur_dt);

/* ������� �������������� ������������ ����� ������                   */
#define    FGetMem               7
PLOTLIB void* WINAPI GetMem(PSTKMPR pstk,
                            long    how);

/* ���� ����� �������� keyword �� ���������� �����                    */
PLOTLIB long WINAPI GPFDecod(char *filename,
                             char *section,
                             char *keyword);

/* ���� ����� �������� keyword �� ���������� �����                    */
PLOTLIB long WINAPI GPFInt(char *filename,
                           char *section,
                           char *keyword);

/* ���� ��p������ �������� keyword �� ���������� �����                */
PLOTLIB long WINAPI GPFString(char   *filename,
                              char   *section,
                              char   *keyword,
                              u char *buffer,
                              char    pr_end);

/* ������� ��������� ���������� ��������� ����������� ��������        */
PLOTLIB long WINAPI GetRez();

/* ������� ��������� ���������� ���������� ��������� ��������         */
PLOTLIB void WINAPI SetRez(int error);

/* �p���p�������� ����� src ���� long � ������ dst ���� char*         */
PLOTLIB char* WINAPI ltoan(char *dst,
                           long  src);

/* ������� ����������� ������ ����� ����������                        */
#define    Fobmn                 8
PLOTLIB long WINAPI obmn(PSTKMPR pstk,
                         u long  istpid,
                         void   *istoff,
                         u long  prmpid,
                         void   *prmoff,
                         u long  how);

/* ������� ������������� ������                                       */
#define    Funregp               9
PLOTLIB long WINAPI unregp(PPCA ppca);

/* ������� ����������� ������                                         */
#define    Fregp                 10
PLOTLIB long WINAPI regp(char    *pname,
                         PPCA    *pppca,
                         PSTKMPR *ppstk);

/* ������� ����������� ������                                         */
#define    Fregt                 11
PLOTLIB long WINAPI regt(PPCA     ppca,
                         PSTKMPR *ppstk);

/* ������� ������� ��������                                           */
#define    Fstpr                 12
PLOTLIB long WINAPI stpr(PSTKMPR pstk,
                         char   *procname,
                         char   *cmndline,
                         char   *workdir,
                         u long *pid);

/* ������� ������� ������                                             */
#define    Fstthr                13
PLOTLIB long WINAPI stthr(PSTKMPR pstk,
                          void   *func,
                          void   *param,
                          u long *tid);

/* ������� ������� ���������� � ����������� ��������                  */
PLOTLIB void WINAPI trim(u char *string);

/* ������� ��������� ID �������� ������                               */
PLOTLIB long WINAPI GetCurTID();

/* ������� ������������� ������                                       */
#define    Funregt               14
PLOTLIB long WINAPI unregt(PSTKMPR pstk);

/* ������� ���������� ������ � �������                                */
#define    Fvivd                 15
PLOTLIB long WINAPI vivd(PSTKMPR pstk,
                         char    lname[4],
                         void   *msgplace,
                         u long  msgsize);

/* ������� ������ ������ �� �������                                   */
#define    Fvvod                 16
PLOTLIB long WINAPI vvod(PSTKMPR pstk,
                         char   *tsksend,
                         void   *msgplace,
                         u long *msgsize);

/* ������� ����������� � ��                                           */
#define    Flinkm                17
PLOTLIB long WINAPI linkm(PSTKMPR pstk,
                          LINK   *ph,
                          u char  modname[4],
                          long    interval);

/* ������� ������������� ������ �� ���� � ������ ��������             */
#define    Fdecodkfs             18
PLOTLIB long WINAPI decodkfs(PSTKMPR pstk,
                             u char  codfrom,
                             u char  codto,
                             u char *block,
                             u long  lblock);

/* ������� �������������                                              */
#define    Fdecoder              19
PLOTLIB long WINAPI decoder(PSTKMPR pstk,
                            u char  codfrom,
                            u char  codto,
                            u char *blok,
                            u long  lblok);

/* ������� ������������ �������                                       */
#define    Fpost_event           20
PLOTLIB long WINAPI post_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count);

/* ������� �������� �������                                           */
#define    Fwait_event           21
PLOTLIB long WINAPI wait_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count,
                               long    interval);

/* ������� ���������� �� ��                                           */
#define    Funlinkm              22
PLOTLIB long WINAPI unlinkm(PSTKMPR pstk,
                            LINK ph,
                            long interval);

/* ������� ����������������� �������������� ������ ���. � ��          */
#define    Fobms                 23
#define S_INF               128  /* �������������� �������            */
#define S_2                  64  /* ������� 2                         */
#define S_3                  32  /* ������� 3                         */
#define S_STRUCT             16  /* ������� �������� ���������        */
#define S_5                   8  /* ������� 5                         */
#define S_6                   4  /* ������� 6                         */
#define S_7                   2  /* ������� 7                         */
#define S_8                   1  /* ������� 8                         */
PLOTLIB long WINAPI obms(PSTKMPR pstk,
                         LINK    ph,
                         char    rezim,
                         char   *dbname,
                         char   *tblname,
                         char   *docname,
                         void   *data,
                         u char  snum,
                         u char *razmech,
                         u char *inftype,
                         IDT     docidt,
                         long   *datalen,
                         long    interval);

/* ������� ������������ ������� ����� ����� (� �����)                 */
PLOTLIB void WINAPI makepath(char *name,
                             char *path);

/* ������� ��������� ID �������� ��������                             */
PLOTLIB long WINAPI GetCurPID();

/* ������� ������������ ���������� ������                             */
#define    Fdelay                24
#ifdef VMESA
  #pragma map(delay, "DELAYTH ")
#endif /* VMESA */
PLOTLIB long WINAPI delay(PSTKMPR pstk,
                          int     how);

/* ������� ������������� ������                                       */
#define    Finitproc             25
PLOTLIB long WINAPI initproc(PSTKMPR pstk,
                             char   *procname,
                             IDT     time);

/* ������� ��������������� ���������� ������                          */
#define    Fdelthr               26
PLOTLIB long WINAPI delthr(PSTKMPR pstk,
                           u long  thrtid,
                           void   *thrhndl);

/* ������� ��������� ID �������                                       */
#define    FGetQueryID           27
PLOTLIB int WINAPI GetQueryID(PSTKMPR  pstk,
                              PQUERYID pqueryid);

/* ������� ���������� ������ ������ ��������                          */
#define    FGetSU                28
PLOTLIB long WINAPI GetSU(PSTKMPR pstk,
                          PSU     psu);

/* ������� ��������� ���� ��������� �� FORMAT.SLV �� ���������� ����� */
#define    FGetDocType           29
PLOTLIB long WINAPI GetDocType(PSTKMPR  pstk,
                               u char  *inftype,
                               u char  *extfile);


/* ������� ������ �������� � ��                                       */
#define    Fobmz                 30
PLOTLIB long WINAPI obmz(PSTKMPR pstk,
                         LINK    hlink,
                         char    rezim,
                         char   *dbname,
                         char   *tblname,
                         char   *recnameget,
                         long    recnumget,
                         u char *razmech,
                         void   *data,
                         long    datalen,
                         char   *recnameput,
                         long   *recnumput,
                         long   *plenput,
                         long    interval);

/* ������� ������ ����������� � ��                                    */
#define    Fobmd                 31
PLOTLIB long WINAPI obmd(PSTKMPR pstk,
                         LINK    hlink,
                         char    rezim,
                         char   *dbname,
                         char   *tblname,
                         char   *docname,
                         u char *razmech,
                         void   *data,
                         long    datalen,
                         long   *plenput,
                         long    interval);

/* ������� ��������� ������ �� ��������� ������                       */
#define    Fsavetemp             32
PLOTLIB long WINAPI savetemp(PSTKMPR pstk,
                             long    type,
                             SU      su,
                             char   *su_ext,
                             u long  su_size,
                             char   *data,
                             u long  datasize,
                             char   *key);

/* ������� ���������� ������ �� ��������� ������                      */
#define    Faddtemp              33
PLOTLIB long WINAPI addtemp(PSTKMPR pstk,
                            PSU     psu,
                            char   *su_ext,
                            u long  su_size,
                            char   *key,
                            char   *data,
                            u long  datasize);

/* ������� ������ ������ �� ��������� ������                          */
#define    Freadtemp             34
PLOTLIB long WINAPI readtemp(PSTKMPR pstk,
                             char   *key,
                             PSU     psu,
                             char   *su_ext,
                             u long *su_size,
                             char   *data,
                             u long  datasize,
                             u long  smesh,
                             u long *cb_read);

/* ������� �������� ������ �� ��������� ������                        */
#define    Fdeltemp              35
PLOTLIB long WINAPI deltemp(PSTKMPR pstk,
                            char   *key);

/* ������� ��������� ����� �� ��������� ������                        */
#define    Fftotemp              36
PLOTLIB long WINAPI ftotemp(PSTKMPR pstk,
                            SU      su,
                            char   *su_ext,
                            u long  su_size,
                            char   *fname,
                            char   *key);

/* ������� ��������� ������� ������ �� ��������� ������               */
#define    Fsizetemp             37
PLOTLIB long WINAPI sizetemp(PSTKMPR pstk,
                             char   *key,
                             u long *datasize);

/* ������� �������� ������� ������, ���������� �������                */
#define    Ftest_event           38
PLOTLIB long WINAPI test_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count);

/* ������� ��������� ����� ��� ������ ��������                        */
#define    FGetGardKey           39
PLOTLIB long WINAPI GetGardKey(PSTKMPR pstk,
                               char   *key);

/* ������� ��������� ���������� ����� �� ���� ��������� �� FORMAT.SLV */
#define    FGetFileExt           40
PLOTLIB long WINAPI GetFileExt(PSTKMPR  pstk,
                               u char  *inftype,
                               u char  *extfile);

/* ������� ��������� ���������� ������� �� �������������� �������     */
#define    FGetFormatInfo        41
PLOTLIB long WINAPI GetFormatInfo(PSTKMPR  pstk,
                                  u char  *inftype,
                                  FORMAT  *pformat);

/* ������� ��������� �/� ������������� �� ���������� ����� ���        */
/* �� ���� ��������� �� FORMAT.SLV                                    */
#define    FGetDocCodbo          42
PLOTLIB long WINAPI GetDocCodbo(PSTKMPR  pstk,
                                u char  *inftype,
                                u char  *extfile,
                                u char  *codbo);

/* ������� ������ ���������� ���������                                */
#define    FPlotMsgBox           43
#define PLOTMSGBOXERR            1   /* ��������� �� ������           */
#define PLOTMSGBOXWRN            2   /* ��������������                */
#define PLOTMSGBOXINF            3   /* ����������                    */
#define PLOTMSGBOXQUE            4   /* ������                        */
PLOTLIB bool WINAPI PlotMsgBox(PSTKMPR pstk,
                               char   *message,
                               char    type);

/* ������� ��������� ���������� ������ �� ��������� ������            */
#define    Fquerytemp            44
PLOTLIB long WINAPI querytemp(PSTKMPR pstk,
                              char   *key,
                              PTMPHDR ptmphdr);

/* ������� �������������� ��������� IDT � ���������� �������������    */
#define    FGetIDTStr            45
PLOTLIB long WINAPI GetIDTStr(PSTKMPR pstk,
                              PIDT    pidt,
                              PDTS    pdts);

/* ������� �������������� ����������� ������������� ����              */
/* � ��������� IDT                                                    */
#define    FGetStrIDT            46
PLOTLIB long WINAPI GetStrIDT(PSTKMPR pstk,
                              PIDT    pidt,
                              PDTS    pdts);

/* ������� ��������� ����������� ���                                  */
#define    FGetIncome            47
PLOTLIB long WINAPI GetIncome(PSTKMPR  pstk,
                              JOURNIN *buffer,
                              u long   firstrec,
                              u long   nreqrec,
                              u long  *nretrec);

/* ������� ��������� ����������� ���                                  */
#define    FGetOutgoing          48
PLOTLIB long WINAPI GetOutgoing(PSTKMPR   pstk,
                                JOURNOUT *buffer,
                                u long    firstrec,
                                u long    nreqrec,
                                u long   *nretrec);

/* ������� ��������� ����������� �����                                */
#define    FGetSOut              49
PLOTLIB long WINAPI GetSOut(PSTKMPR   pstk,
                            JSYNCOUT *buffer,
                            u long    firstrec,
                            u long    nreqrec,
                            u long   *nretrec);

/* ������� ��������� �������� ��������� ���                           */
#define    FGetNewIncome         50
PLOTLIB long WINAPI GetNewIncome(PSTKMPR  pstk,
                                 JOURNIN *buffer,
                                 u long   buffsize,
                                 u long  *nretrec,
                                 IDT     *lasttime);

/* ������� ��������� �������� ��������� ���                           */
#define    FGetNewOutgoing       51
PLOTLIB long WINAPI GetNewOutgoing(PSTKMPR   pstk,
                                   JOURNOUT *buffer,
                                   u long    buffsize,
                                   u long   *nretrec,
                                   IDT      *lasttime);

/* ������� ��������� �������� ��������� �����                         */
#define    FGetNewSOut           52
PLOTLIB long WINAPI GetNewSOut(PSTKMPR   pstk,
                               JSYNCOUT *buffer,
                               u long    buffsize,
                               u long   *nretrec,
                               IDT      *lasttime);

/* ������� ����������� ��������� �� ��������� ������ � ����           */
#define    Ftemptof              53
PLOTLIB long WINAPI temptof(PSTKMPR pstk,
                            char   *key,
                            PSU     psu,
                            char   *su_ext,
                            u long *su_size,
                            char   *fname);

/* ������� ��������� ���������� ���������� ��                         */
#define    FGetExtSU             54
PLOTLIB long WINAPI GetExtSU(PSTKMPR pstk,
                             char   *kfs,
                             PKFSEXT pkfsext);

/* ������� ��������� �������� ������ ��������� ����������� �������    */
#define    FGetPlotAvr           55
PLOTLIB long WINAPI GetPlotAvr(PSTKMPR pstk);

/* ������� ��������� �������� ���������� ��������� ����������� �������*/
#define    FGetPlotRez           56
PLOTLIB long WINAPI GetPlotRez(PSTKMPR pstk);

/* ������� ������ ��������� � ������ ��������� �������                */
#define    FSysJurn              57
PLOTLIB long WINAPI SysJurn(PSTKMPR pstk,
                            char   *message,
                            void   *data,
                            u long  sz_data);

/* ������� ��������� ������� ���������� �������                       */
#define    FGetUPInfo            58
PLOTLIB long WINAPI GetUPInfo(PSTKMPR pstk,
                              UPINFO *pupinfo);

/* ������� ��������� ������� ���������� ������                        */
#define    FGetTaskInfo          59
PLOTLIB long WINAPI GetTaskInfo(PSTKMPR   pstk,
                                TASKINFO *ptaskinfo);

/* ������� �������� �������                                           */
#define    FOpenTable            60
PLOTLIB long WINAPI OpenTable(PSTKMPR   pstk,
                              LINK      op,
                              CURSOR   *opcur,
                              char     *bd_name,
                              char     *nd_name,
                              char     *tb_name,
                              void     *tbstradr,
                              u long   *tbstrlen,
                              long      interval);
/* ������� �������� �������                                           */
#define    FCloseTable           61
PLOTLIB long WINAPI CloseTable(PSTKMPR   pstk,
                               LINK      op,
                               CURSOR   *opcur,
                               long      interval);

/* ������� ������������� �������                                      */
#define    FDecodTab             62
PLOTLIB long WINAPI DecodTab(PSTKMPR   pstk,
                             CURSOR    opcur,
                             u char    rej1,
                             u char    rej2);

/* ������� �������� ���������                                         */
#define    FWriteTable           63
PLOTLIB long WINAPI WriteTable(PSTKMPR   pstk,
                               LINK      op,
                               CURSOR    opcur,
                               void     *soobadr,
                               u long    sooblen,
                               char      priznak,
                               long      interval);

/* ������� �������� �������                                          */
#define    FDelTable             64
PLOTLIB long WINAPI DelTable(PSTKMPR  pstk,
                             LINK      op,
                             CURSOR   *opcur,
                             long      interval);

/* ������� ���������� �������                                         */
#define    FSelectSortTable      65
PLOTLIB long WINAPI SelectSortTable(PSTKMPR   pstk,
                                    LINK      op,
                                    CURSOR    opcur,
                                    void     *tbdatadr,
                                    u long   *tbdatlen,
                                    u char    getmode,
                                    char     *namepol,
                                    u char    napravl,
                                    long      interval);

#define    FGetCsaPoint          66
long GetCsaPoint(PSTKMPR  pstk,
                 void   **padrcsa,
                 u long  *ppidcsa);

#define    FSetCsaPoint          67
long SetCsaPoint(PSTKMPR pstk,
                 void   *adrcsa,
                 u long  pidcsa);

#define    FSetEnvParm           68
long SetEnvParm(PSTKMPR pstk,
                char   *name,
                void   *buffer,
                u long  bufflen);

#define    FGetEnvParm           69
long GetEnvParm(PSTKMPR pstk,
                char   *name,
                void   *buffer,
                u long *bufflen);

PLOTLIB void WINAPI active_fix(PSTKMPR pstk);

void BldTblCRC(u long  crc_poly,
               u long *table);

u long UpdateCRC(u long  crc,
                 u char  curbyte,
                 u long *table);

#define    FGetFileCRC           70
long GetFileCRC(PSTKMPR pstk,
                char   *fname,
                u long *pcrc);

#define    FGetSysCRC            71
long GetSysCRC(PSTKMPR pstk,
               u long *pcrc);

#define    FGetModCRC            72
long GetModCRC(PSTKMPR pstk,
               char   *modname,
               u long *pcrc);

long coded3(u char *rezult,
            u char *pmass,
            long    count);

bool Test(PSRITEM  ppoisk,
          void    *test,
          void    *rec);

bool TestAndReplace(PSRITEM  ppoisk,
                    PSRITEM  pzamena,
                    void    *test,
                    void    *data,
                    void    *rec);

#define    FAddIncome            73
long AddIncome(PSTKMPR pstk,
               SU      su,
               IDT    *pidt_isp);

#define    FAddOutgoing          74
long AddOutgoing(PSTKMPR pstk,
                 SU      su,
                 IDT    *pidt_isp);

#define    FAddSOut              75
long AddSOut(PSTKMPR pstk,
             SU      su,
             LINKSTR param);

#define    FSetIncome            76
long SetIncome(PSTKMPR pstk,
               u long  src_mask,
               u long  set_mask,
               JOURNIN src_rec,
               JOURNIN set_rec);

#define    FSetOutgoing          77
long SetOutgoing(PSTKMPR  pstk,
                 u long   src_mask,
                 u long   set_mask,
                 JOURNOUT src_rec,
                 JOURNOUT set_rec);

#define    FSetSOut              78
long SetSOut(PSTKMPR  pstk,
             u long   src_mask,
             u long   set_mask,
             JSYNCOUT src_rec,
             JSYNCOUT set_rec);

/* ������� �������� ��������� �� ������������ ��������� ������        */
#define    FSendDoc              80
PLOTLIB long WINAPI SendDoc(PSTKMPR  pstk,
                            char    *log_adr,
                            u char  *kl_dok,
                            u char   dataplace,
                            void    *data,
                            u long   sizedata,
                            char    *DATAddmmgg,
                            u char  *format,
                            u char   code);

/* ������� ������ � GARDOV                                            */
#define    FVcan                 81
PLOTLIB long WINAPI Vcan(PSTKMPR  pstk,
                         char    *slv_name,
                         u long   napr_sv,
                         PSU      psu,
                         u long   rezim, ...);

/* ������� ������������� ��������������� ���������                    */
#define    FInitKan              82
PLOTLIB long WINAPI InitKan(PSTKMPR pstk,
                            u long  trnsprt);

/* ������� ��������� �������� �� B204                                 */
#define    FGetSLV               83
long GetSLV(PSTKMPR pstk,
            PSLVS   pslvs);

/* ������� ������������ ��������                                      */
#define    FFreeSLV              84
long FreeSLV(PSTKMPR pstk,
             PSLVS   pslvs);

/* ������� ������ SLV � B204                                          */
#define    FSetSLV               85
long SetSLV(PSTKMPR pstk,
            PSLVS   pslvs);

/* ��������� ����������� �������� ������ �����������                  */
#define    FScan                 86
PLOTLIB long WINAPI Scan(PSTKMPR  pstk,
                         u long   napr_sv,
                         u long   num_vu,
                         u long   trnsprt,
                         char    *adr_vu,
                         PSU      psu,
                         u long   priz_rez);

/* ������� ����������� �������� ������ ��������� �� �������           */
#define    FPrFalse              87
PLOTLIB long WINAPI PrFalse(PSTKMPR  pstk,
                            u long   napr_sv,
                            PSU      psu);

/* ������� �������� �� ������������ �������                         */
#define    Fdirector             88
PLOTLIB long WINAPI director(PSTKMPR pstk,
                             SLVS   *pslvs,
                             CSA    *pcsa,
                             char   *ladr,
                             u long *napr,
                             u long *nvu,
                             bool   *may_be,
                             bool    full_search,
                             u char *addr,
                             u long *linename);

/* ������� ������������� �� ����������� ����������� ����������� �����-*/
/* �� �������������                                                   */
#define    FDinamDecoder         89
PLOTLIB long WINAPI DinamDecoder(void    *codtbl,
                                 u long   ctbsize,
                                 u char   codfrom,
                                 u char   codto,
                                 void    *data,
                                 u long   datasize,
                                 PCDTKFS  uprtbl,
                                 u long   utblstrn);

/* ������� ������������� ��� �����                                    */
#define    FDecoder1             90
PLOTLIB long WINAPI Decoder1(void    *codtab,
                             u long   tabsize,
                             u char   codfrom,
                             u char   codto,
                             u char  *blok,
                             u long   lblok);

/* ������� �����������                                                */
#define    FNotifyDoc            91
#define    NTF_OBJ_TYPE_DOC        1   /* �������� ����������� ���
                                          ���������� �� UP            */
#define    NTF_OBJ_TYPE_SHOWCASE   2   /* �������� ����������� ���
                                          �������� ������ UP          */
PLOTLIB long WINAPI NotifyDoc(PSTKMPR  pstk,
                              char    *log_adr_otp,
                              char    *log_adr_pol,
                              u char  *kl_dok,
                              PIDT     TimePlace,
                              u char   obj_type,
                              char     rezim);

/* ������� ������������ TEMPa                                         */
#define    FTestTemp             92
PLOTLIB long WINAPI TestTemp(PSTKMPR  pstk,
                             char    *keytemp);

/* ������� ��������� ����������� ��������                             */
#define    FGetJurn              93
PLOTLIB long WINAPI GetJurn(PSTKMPR  pstk,
                            char    *source,
                            u char   bo_source,
                            char    *powner,
                            char    *type,
                            void    *buffer,
                            u long   buffsize,
                            u long   firstrec,
                            u long   nreqrec,
                            u long  *nretrec);

/* ������� ���������� ������ ��������� ���                            */
#define    FGetZVD               94
PLOTLIB long WINAPI GetZVD(PSTKMPR pstk,
                           PZVD    pzvd);

/* ������� ����� ������� ������                                       */
#define    FChangeChannelStatus  95
long ChangeChannelStatus(PSTKMPR  pstk,
                         u long   napr_sv,
                         u long   num_vu,
                         u long   trnsprt,
                         char    *adr_vu,
                         PSLVS    pslvs);

#define    FClearNaprSv          96
PLOTLIB long WINAPI ClearNaprSv(PSTKMPR  pstk,
                                u long   napr_sv,
                                u long   num_vu,
                                u long   trnsprt);

/* ������� ��������� ����������                                       */
#define    FFtpGetToc            97
PLOTLIB long WINAPI FtpGetToc(PSTKMPR  pstk,
                              char    *mname,
                              char    *uni_name,
                              char    *data,
                              u long   cb_data_sz,
                              u long  *cb_data,
                              bool    *is_data_oob,
                              long     interval);

/* ������� �������� ��                                                */
#define    FFtpOpenItem         98
PLOTLIB long WINAPI FtpOpenItem(PSTKMPR  pstk,
                                char    *mname,
                                char    *uni_name,
                                void   **pftphi,
                                long     interval);

/* ������� �������� ��                                                */
#define    FFtpCloseItem        99
PLOTLIB long WINAPI FtpCloseItem(PSTKMPR pstk, void *ftphi);

/* ������� �������� ��                                                */
#define    FFtpDelItem         100
PLOTLIB long WINAPI FtpDelItem(PSTKMPR  pstk,
                               char    *mname,
                               char    *uni_name,
                               long     interval);

/* ������� �������������� ��                                          */
#define    FFtpRenItem         101
PLOTLIB long WINAPI FtpRenItem(PSTKMPR  pstk,
                               char    *mname,
                               char    *uni_name,
                               char    *new_name,
                               long     interval);

/* ������� ������ ��                                                  */
#define    FFtpRead            102
PLOTLIB long WINAPI FtpRead(PSTKMPR  pstk,
                            void    *ftphi,
                            char    *data,
                            u long   data_sz,
                            u long   smesh,
                            u long   cb_required,
                            bool     need_recode,
                            u long  *cb_received,
                            bool    *is_data_oob,
                            long     interval);

/* ������� ��������� ������� ��                                       */
#define    FFtpGetItemProps    103
PLOTLIB long WINAPI FtpGetItemProps(PSTKMPR      pstk,
                                    char        *mname,
                                    char        *uni_name,
                                    PFTP_I_PROP  pprops,
                                    long         interval);

/* ������� �������� ��                                                */
#define    FFtpCreateItem      104
PLOTLIB long WINAPI FtpCreateItem(PSTKMPR     pstk,
                                  char       *mname,
                                  char       *uni_name,
                                  FTP_I_PROP  props,
                                  long        interval);

/* ������� ������ ����� ��                                            */
#define    FFtpReadBlock       105
PLOTLIB long WINAPI FtpReadBlock(PSTKMPR  pstk,
                                 void    *ftphi,
                                 char    *data,
                                 u long   data_sz,
                                 u long   block_first,
                                 u long   n_block_req,
                                 bool     need_recode,
                                 u long  *n_block_ret,
                                 u long  *cb_received,
                                 bool    *is_data_oob,
                                 long     interval);

/* ������� ������ ����� ��                                            */
#define    FFtpWriteBlock      106
#ifdef VMESA
  #pragma map(FtpWriteBlock, "FTPWRBL ")
#endif /* VMESA */
PLOTLIB long WINAPI FtpWriteBlock(PSTKMPR  pstk,
                                  void    *ftphi,
                                  char    *data,
                                  u long   data_sz,
                                  u long   n_block_first,
                                  u long   n_block_req,
                                  bool     need_recode,
                                  u long  *n_block_writen,
                                  long     interval);

/* ������� ������ ��                                                  */
#define    FFtpWrite           107
PLOTLIB long WINAPI FtpWrite(PSTKMPR  pstk,
                             void    *ftphi,
                             char    *data,
                             u long   data_sz,
                             u long   smesh,
                             bool     need_recode,
                             u long  *cb_writen,
                             long     interval);

/* ���������� ������ � ���FTP                                         */
#define    FAddFTPOut          108
long AddFTPOut(PSTKMPR pstk,
               SU      su,
               UP_FTP param,
               char *data);

/* �������������� ������ � ���FTP                                     */
#define    FSetFTPOut          109
long SetFTPOut(PSTKMPR  pstk,
             u long   src_mask,
             u long   set_mask,
             JFTPOUT  src_rec,
             JFTPOUT  set_rec);

/* ������� ��������� ������ �� ���FTP                                 */
#define    FGetFTPOut          110
PLOTLIB long WINAPI GetFTPOut(PSTKMPR   pstk,
                              JFTPOUT  *buffer,
                              u long    firstrec,
                              u long    nreqrec,
                              u long   *nretrec);

/* ������� ��������� ����������� ������ �� ���FTP                     */
#define    FGetNewFTPOut       111
PLOTLIB long WINAPI GetNewFTPOut(PSTKMPR   pstk,
                                 JFTPOUT  *buffer,
                                 u long    buffsize,
                                 u long   *nretrec,
                                 IDT      *lasttime);

PLOTLIB char * WINAPI GetStrError(long err_code);

#define    Fclop               112
PLOTLIB long WINAPI clop(PSTKMPR pstk,
                         void   *AddrBuff,
                         u long  LInf,
                         void   *AddrKl,
                         u long *AddrD,
                         u long *AddrP,
                         u long *AddrPd);

#define    Fclopf              113
PLOTLIB long WINAPI clopf(PSTKMPR pstk,
                          char   *NameFile,
                          void   *AddrKl);

#define    FCreateDisTable     114
PLOTLIB long WINAPI CreateDisTable(PSTKMPR   pstk,
                                   LINK      op,
                                   CURSOR   *opcur,
                                   char     *bd_name,
                                   char     *nd_name,
                                   char     *tb_name);

#define    FSendData           115
PLOTLIB long WINAPI SendData(PSTKMPR          pstk,
                             PMESSAGE_PARAMS  pparm);

#define    FParseData          116
PLOTLIB long WINAPI ParseData(PSTKMPR          pstk,
                              char            *message,
                              u long           sizedata,
                              PMESSAGE_PARSED  pparm);

/* ������� �������������� ��������� IDT � 16-������ �������������     */
#define    FGetIDTHex          117
PLOTLIB long WINAPI GetIDTHex(PSTKMPR pstk,
                              PIDT    pidt,
                              PIDH    pidh);

/* ������� �������� ��������� � ���������� ������                     */
#define    FSyncSendData       118
PLOTLIB long WINAPI SyncSendData(PSTKMPR  pstk, PSYNCPARM  pparm);

#ifndef VMESA
/* ������� ��������� ������� � �������������                          */
#define    FGetTable           119
PLOTLIB long WINAPI GetTable(PSTKMPR    pstk,
                             INFTABLE  *inftable,
                             void      *datazag,
                             u long    *sizezag,
                             char      *pr_zag,
                             void      *data,
                             u long    *sizedata,
                             char      *pr_temp,
                             long       interval);

/* ������� �������������� ������� � HTML ���                          */
/* (���������� ����� GetTable)                                        */
#define    FGetHTMLTable       120
PLOTLIB long WINAPI GetHTMLTable(PSTKMPR   pstk,
                                 INFTABLE *inftable,
                                 void     *zag,
                                 u long    sizezag,
                                 char      pr_zag,
                                 void     *data,
                                 u long   *sizedata,
                                 char      pr_temp);
#endif /*NOT VMESA*/

/* ������� ����������������� �������������� ������ ���. � ��          */
/* ��� ��������� �������� ��� � ������ ����������                     */
#define    FAsyncObms          123
long AsyncObms(PSTKMPR pstk,
               LINK    ph,
               char    rezim,
               char   *dbname,
               char   *tblname,
               char   *docname,
               void   *data,
               u char  snum,
               u char *razmech,
               u char *inftype,
               IDT     docidt,
               long   *datalen,
               long    interval,
               u char *realaddr,
               u char *userid,
               u char *userrang);

/* ������� ������ ����������                                          */
#define    FCompress           124
long Compress(PSTKMPR    pstk,
              u char    *BufIst,
              u char    *BufPrm,
              u long     LenIst,
              u long    *LenPrm);

/* ������� �������� ����������                                        */
#define    FDecompress         125
long Decompress(PSTKMPR    pstk,
              u char    *BufIst,
              u char    *BufPrm,
              u long     LenIst,
              u long    *LenPrm);

/* ������� �������������� ��������� IDH � ���������� �������������    */
#define    FGetIDHStr          126
PLOTLIB long WINAPI GetIDHStr(PSTKMPR pstk,
                              PIDH    pidh,
                              PDTS    pdts);

#define    FSendAcc            127
PLOTLIB long WINAPI SendAcc(PSTKMPR          pstk,
                            PMESSAGE_PARSED  pparm);

#define    Fwritetemp          128
PLOTLIB long WINAPI writetemp(PSTKMPR pstk,
                              char   *key,
                              u long  offset,
                              char   *data,
                              u long  datasize);

/* ������� ��������� SNT ������ ������                                */
#define    FGetOwnSNT          129
long GetOwnSNT(PSTKMPR    pstk,
               void     **psnt,
               u long    *sntsize,
               u long    *sntreal);

/* ������� ��������� ������ �� ������ ���                             */
#define    FUP_GetGatewaysList 130
PLOTLIB long WINAPI UP_GetGatewaysList(PSTKMPR     pstk,
                                       PUPVIEWNET  pparm);

/* ������� ��������� ������ �� ��������� ���                          */
#define    FUP_GetNetState     131
PLOTLIB long WINAPI UP_GetNetState(PSTKMPR     pstk,
                                   PUPVIEWNET  pparm);

/* ������� ��������� �������� ����� ��                                */
#define    FUP_GetOwnVUState   132
PLOTLIB long WINAPI UP_GetOwnVUState(PSTKMPR   pstk,
                                     PVUSTATE  pvustate);

#define    FGetMemoryCRC       133
long GetMemoryCRC(PSTKMPR pstk,
                  void   *addr,
                  u long  size,
                  u long *pcrc);

#define    FstprEx             134
PLOTLIB long WINAPI stprEx(PSTKMPR pstk,
                           char   *procname,
                           char   *cmndline,
                           char   *workdir,
                           u long *pid,
                           bool    is_hide);


#define    FFtpExistItem       135
PLOTLIB long WINAPI FtpExistItem(PSTKMPR pstk,
                                 char   *mname,
                                 char   *uni_name,
                                 long    interval);

#define    FEnqIsSet           136
int EnqIsSet(char *enq, enq_set *set);

#define    FDeleteEnq          137
PLOTLIB long WINAPI DeleteEnq(PSTKMPR pstk,
                              void   *resname,
                              long    nmsz);

#define    Fup_clock           138
u long up_clock();

/* ������� �������� ������������� ���������                           */
#define    FSendTemaMsg        140
PLOTLIB long WINAPI SendTemaMsg(PSTKMPR     pstk,
                                PTEMASTR    pparm);

#define    FFormTemaHead       141
long FormTemaHead(PSTKMPR     pstk,
                  PTEMAHEAD   phead);

#define    FFtpStartProc       142
/* ������� ������� ���������� ��������                                */
PLOTLIB long WINAPI FtpStartProc(PSTKMPR   pstk,
                                 char     *mname,
                                 char     *uni_name,
                                 char      pr_wait,
                                 u long   *exitcode,
                                 long      interval);

#define    FSYS_ctask          143
/* ������� ���������� �������                                         */
PLOTLIB long WINAPI SYS_ctask(PSTKMPR pstk,
                              char   *mname,
                              char   *taskname,
                              u char  req,
                              IDT    *time);

#define    FTestBit            144
/* ������� �������� �������� ���� � ���.����� �� ��� ������ (1,2,...) */
PLOTLIB long WINAPI TestBit(PSTKMPR pstk,
                            u long  NBit,
                            char   *PShkl,
                            u char *ZnBit);

#define    FTestTAdr           145
/* ������� �������� ���������� �� ���������� �������� ����� - ������  */
long TestTAdr(PSTKMPR pstk, PACNT pacnt);

#define    FTestIO             146
/* ������� �������� ���������� �� ���������� �������� ����� - ������  */
long TestIO(PSTKMPR pstk, PACNT pacnt);

#define    FTestDCFP           147
/* ������� �������� ���������� �� ���������� �������� ��� �����������-
   ���� ��������� ��, ���������� �������������� ���������, ������
   � �������� ������� � ip-������                                     */
long TestDCFP(PSTKMPR pstk, PACNT pacnt);

#define    FTestAcnt           148
/* ������� �������� ���������� ������������ �� ���������� ����������
   �������� ��� �������� �������                                      */
PLOTLIB long WINAPI TestAcnt(PSTKMPR pstk,
                             char   *modotpr,
                             u long  kod,
                             char   *range,
                             char   *modpol,
                             char   *grif,
                             char   *n_path,
                             u short numport,
                             char   *user_id,
                             char   *spec_sd);

/* ������� ������������� ������� ������                               */
#define    FInitTLine          149
PLOTLIB long WINAPI InitTLine(PSTKMPR   pstk,
                              char     *name_thread,
                              void    **handline,
                              u long    linesize,
                              char      lineplace);

/* ������� ������ ������� ������                                      */
#define    FThrVvod            150
PLOTLIB long WINAPI ThrVvod(PSTKMPR   pstk,
                            void     *disline,
                            void     *Buffer,
                            u long   *SizeBuffer);

/* ������� ������ ��������� � ������� ������                          */
#define    FThrVivd            151
PLOTLIB long WINAPI ThrVivd(PSTKMPR   pstk,
                            char     *name_thread,
                            void     *Buffer,
                            u long    SizeBuffer);

/* ���������� ������ � ���SYS                                         */
#define    FAddSysIn           152
long AddSysIn(PSTKMPR pstk,
              SU      su,
              SYS     param);

/* ���������� ������ � ���SYS                                         */
#define    FSetSysIn           153
long SetSysIn(PSTKMPR  pstk,
              u long   src_mask,
              u long   set_mask,
              JSYSIN   src_rec,
              JSYSIN   set_rec);

/* ��������� ����� ����� �� ���SYS                                    */
#define    FGetNewSysIn        154
PLOTLIB long WINAPI GetNewSysIn(PSTKMPR   pstk,
                                SRITEM   *filter,
                                u long    filter_sz,
                                JSYSIN    filter_rec,
                                JSYSIN   *buffer,
                                u long    buffsize,
                                u long   *nretrec,
                                IDT      *lasttime);

/* ������ �� �������� ������ GARD                                     */
#define    FSYS_cgard          155
PLOTLIB long WINAPI SYS_cgard(PSTKMPR pstk,
                              char   *mname,
                              SU      str,
                              u char  req,
                              IDT    *time);

#define    FRegSbSrd           156
/* ������� ���-��� ������� ������� ����������� ������������ ���-���   */
long RegSbSrd(PSTKMPR  pstk, PSZI  pjrnb);

#define    FFtpCancel          157
/* ������� ������ ����������� �������� FTP                            */
PLOTLIB long WINAPI FtpCancel(PSTKMPR pstk,
                              long    rezcode);

#define    FOpenCurTable       158
/* ������� �������� ������� �������                                   */
PLOTLIB long WINAPI OpenCurTable(PSTKMPR   pstk,
                                 LINK      op,
                                 CURSOR   *opcur,
                                 char     *bd_name,
                                 char     *nd_name,
                                 char     *tb_name);

#define    FReceiveStamp       159
/* ������� ��������� ����� ��������(����������) �������� � ��������   */
/* ����������                                                         */
PLOTLIB long WINAPI ReceiveStamp(PSTKMPR pstk,
                                 char   *PPath,
                                 char   *PStamp,
                                 char   *PAddrPoluch);
#ifdef WINNT
/* ������� ����������� �������� � ������ ����������� ���� �����
   ���������� ������ UP */
PLOTLIB void WINAPI NewVerCrc(void);
#endif /*WINNT*/

/* ��������� ���������� �����                                         */
char *UP_GetDir(char *dir, char *file);

/* ��������� ���������� ����� � ������� UP                            */
char *UP_UniGetDir(char *dir, char *uniname);

#define    FGetSysUserInfo     160
/* ������� ��������� ���������� � ������������ ��                     */
PLOTLIB long WINAPI GetSysUserInfo(PSTKMPR         pstk,
                                   PSYS_USER_INFO  pinfo);

#define    FGetSysFileInfo     161
/* ������� ��������� ���������� � ����� ��                            */
PLOTLIB long WINAPI GetSysFileInfo(PSTKMPR pstk,
                                   char *fname,
                                   u char *pfilegrif);

#define    FSetSysFileInfo     162
/* ������� ��������� ����� ����� ��                                   */
PLOTLIB long WINAPI SetSysFileInfo(PSTKMPR pstk,
                                   char *fname,
                                   u char filegrif);

#define    FDefineUserLevel    163
/* ����������� ������ �������� ������� - ������������                 */
long DefineUserLevel(PSTKMPR pstk,
                     char    priznak,
                     u char  shkl1,
                     u char  shkl2,
                     u char *retMinLevel,
                     u char *retMaxLevel);

#define    FSetTempGuard       165
/* ��������� �������� �������������� ������ � ����� �� �����          */
PLOTLIB long WINAPI SetTempGuard(PSTKMPR  pstk,
                                 char    *key,
                                 char     is_own);

#define    FGetXLinkType       167
/* ������� ��������� �/� ���� ������                                  */
PLOTLIB long WINAPI GetXLinkType(PSTKMPR pstk,
                                 LINK    plink,
                                 u char *pXLink);

/* ������� �������������� ���� �������� (����������)                  */
/* �� ������� �� � ������ UP                                          */
char *UP_DirConvert(char *up_path, char *os_path);

#define    FForwardMessage        168
PLOTLIB long WINAPI ForwardMessage(PSTKMPR pstk,
                                   char   *tempkey,
                                   char   *addr_pol,
                                   char   *tempkeyfwd);

#define    FUnwrapForwardMessage  169
PLOTLIB long WINAPI UnwrapForwardMessage(PSTKMPR  pstk,
                                         char    *tempkey,
                                         char    *tempkeyunfwd,
                                         SU      *sufwd);
#define    Fchquerytemp           170
long chquerytemp(PSTKMPR pstk,
                 char   *key,
                 PTMPHDR ptmphdr);

#define    FPRTInquiry            171
/* ������� �������� ������� �� ������ ����������                      */
PLOTLIB long WINAPI PRTInquiry(PSTKMPR pstk,
                               char   *key,
                               char   *addrprinter);

#define    FCreateShowcase        172
/* ������� �������� �������                                           */
PLOTLIB long WINAPI CreateShowcase(PSTKMPR   pstk,
                                   PUP_CASE  pcase,
                                   char     *subjects,
                                   long      interval);

#define    FDelShowcase           173
/* ������� �������� �������                                           */
PLOTLIB long WINAPI DelShowcase(PSTKMPR   pstk,
                                PUP_CASE  pcase,
                                long      interval);

#define    FOpenShowcase          174
/* ������� �������� �������                                           */
PLOTLIB long WINAPI OpenShowcase(PSTKMPR    pstk,
                                 PSHOWCASE *handler,
                                 char      *mod_name,
                                 char      *nd_name,
                                 char      *case_key,
                                 long       interval);

#define    FCloseShowcase         175
/* ������� �������� �������                                           */
PLOTLIB long WINAPI CloseShowcase(PSTKMPR    pstk,
                                  PSHOWCASE *handler,
                                  long       interval);

#define    FAddFolderOnShowcase   176
/* ������� ���������� �������� "�����" �� �������                     */
PLOTLIB long WINAPI AddFolderOnShowcase(PSTKMPR     pstk,
                                        PSHOWCASE   handler,
                                        PCASEITEM   pfolder,
                                        char       *subject,
                                        char       *sub_subject,
                                        char       *region,
                                        char       *sub_region,
                                        long        interval);

#define    FAddXLinkOnShowcase    177
/* ������� ���������� �������� "������" �� �������                    */
PLOTLIB long WINAPI AddXLinkOnShowcase(PSTKMPR     pstk,
                                       PSHOWCASE   handler,
                                       PCASEITEM   pxlink,
                                       long        interval);

PLOTLIB long WINAPI up_log(PPCA ppca, char *fmt, ...);

/* ������� �������� �������� � �������                                */
#define    FDelItemFromShowcase   178
PLOTLIB long WINAPI DelItemFromShowcase(PSTKMPR    pstk,
                                        PSHOWCASE  handler,
                                        PCASEITEM  pitem,
                                        long       interval);

/* ������� ��������� ��                                               */
#define    FFtpReadFile            179
PLOTLIB long WINAPI FtpReadFile(PSTKMPR  pstk,
                                char    *mname,
                                char    *uni_name,
                                char     bo_place,
                                char    *local_name,
                                bool     need_recode,
                                long     interval);

/* ������� ������ ��                                                  */
#define    FFtpWriteFile           180
#ifdef VMESA
  #pragma map(FtpWriteFile, "FTPWRFL ")
#endif /* VMESA */
PLOTLIB long WINAPI FtpWriteFile(PSTKMPR  pstk,
                                 char    *mname,
                                 char    *uni_name,
                                 char     bo_place,
                                 char    *local_name,
                                 bool     need_recode,
                                 long     interval);

#define    Fcopytemp               181
long copytemp(PSTKMPR pstk,
              SU      su,
              char   *su_ext,
              u long  su_size,
              char   *srckey,
              char   *key);

#define    Frecodetemp             182
long recodetemp(PSTKMPR  pstk,
                char    *keytemp,
                u char   code);

/* ������� ��������� ��������� SELECT � �������� ��������� UP         */
/* (��������� �������� � ������� ������� DINSELECT)                   */
#define    FDinSelectTable        183
PLOTLIB long WINAPI DinSelectTable(PSTKMPR   pstk,
                                   LINK      op,
                                   CURSOR    opcur,
                                   void     *tbdatadr,
                                   u long   *tbdatlen,
                                   char     *namepol,
                                   u char    napravl,
                                   u char   *dinselect,
                                   u long    lendinsel,
                                   u char   *data_t,
                                   long      action,
                                   char      pr_code,
                                   long      interval);

/* ������� ������������� ��������� DINSELECT �� ��������� ������ �    */
/* � ��������� ��������� UP                                           */
#define    FDecodSELECT           184
long DecodSELECT(PSTKMPR    pstk,
                 CURSOR     opcur,
                 void      *dinselect,
                 u long     lendinsel);

/* ������� ������������� �������, ����������� �� ��������� ������     */
/* �� ��������� ��������� UP � ��������� ������                       */
#define    FDecTEMPTab            185
long DecTEMPTab(PSTKMPR pstk,
                CURSOR  opcur,
                char   *keytemp);

/* ������� ��������� ��������� SELECT � �������� ��������� UP         */
/* (��������� �������� � ������ ���� char*)                           */
#define    FCharSelect            186
PLOTLIB long WINAPI CharSelect(PSTKMPR  pstk,
                               LINK     op,
                               CURSOR   opcur,
                               char    *str,
                               char    *BufferTab,
                               u long  *tbdatlen,
                               u char  *data_t,
                               u long   action,
                               char     pr_code,
                               long     interv);

/* ������� ������ �������                                             */
#define    FReadShowcase          187
PLOTLIB long WINAPI ReadShowcase(PSTKMPR    pstk,
                                 PSHOWCASE  handler,
                                 PCASEWORK  pparm);

/* ������� ��������� XML-�������� �������                             */
#define    FGetXMLShowcase        188
long GetXMLShowcase(PSTKMPR    pstk,
                    PSHOWCASE  handler,
                    char      *list_temp,
                    u long     list_size,
                    char      *xml_temp,
                    u long    *xml_size);

/* ������� "���������������" ��������� UP_CASE � XML-�������� ������� */
#define    FGetXMLFromUPCASE      189
long GetXMLFromUPCASE(PSTKMPR    pstk,
                      PSHOWCASE  handler,
                      char      *xml_segm);

/* ������� "���������������" ������� �������� ��� XML-��������        */
#define    FGetXMLIndex           190
long GetXMLIndex(PSTKMPR    pstk,
                 PCASEITEM  pitem,
                 char      *str_index);

/* ������� �������������� 16-������� ������������� ����-������� �     */
/* ��������� IDT                                                      */
#define    FGetHexIDT             191
PLOTLIB long WINAPI GetHexIDT(PSTKMPR pstk,
                              PIDH    pidh,
                              PIDT    pidt);

/* ������� ������ ������� ������ ����������� ������                   */
#define    FReadYndex             192
PLOTLIB long WINAPI ReadYndex(PSTKMPR    pstk,
                              char      *mod_name,
                              PCASEWORK  pparm);

/* ������� �������������� XML-������������� ������� ������ �� �       */
/* �����������                                                        */
#define    FGetStrYndex           193
long GetStrYndex(PSTKMPR    pstk,
                 char      *xml_temp,
                 u long     xml_size,
                 char      *str_temp,
                 u long    *str_size);

/* ������� ����������� ������� � ������� ������ ����������� ������    */
#define    FRegShowcase           194
PLOTLIB long WINAPI RegShowcase(PSTKMPR    pstk,
                                PSHOWCASE  handler,
                                char      *mod_name,
                                long       interval);

/* ������� ���������� ������� �� ������� ������ ����������� ������    */
#define    FUnregShowcase         195
PLOTLIB long WINAPI UnregShowcase(PSTKMPR    pstk,
                                  PSHOWCASE  handler,
                                  char      *mod_name,
                                  long       interval);

/* ������� ������� ��������� ������� �� �������� ��������             */
#define    FSelectFromShowcase    196
/* ����������� ���� ���������� ������ �������                         */
#define PARM_PARENT_ID            "PARENT_ID"
#define PARM_LEVELS_INTERVAL      "LEVELS_INTERVAL"
#define PARM_ELEMENT_NAME         "ELEMENT_NAME"
#define PARM_ELEMENT_DATE         "ELEMENT_DATE"
#define PARM_ELEMENT_KEYWORDS     "ELEMENT_KEYWORDS"
#define PARM_ELEMENT_SUBJECT      "ELEMENT_SUBJECT"
#define PARM_ELEMENT_SUB_SUBJECT  "ELEMENT_SUB_SUBJECT"
#define PARM_ELEMENT_REGION       "ELEMENT_REGION"
#define PARM_ELEMENT_SUB_REGION   "ELEMENT_SUB_REGION"
#define PARM_OBJECT_MODULE        "OBJECT_MODULE"
#define PARM_OBJECT_GENERATIONS   "OBJECT_GENERATIONS"
#define PARM_OBJECT_TYPE          "OBJECT_TYPE"
#define PARM_OBJECT_GRIF          "OBJECT_GRIF"
#define PARM_OBJECT_CHANGE        "OBJECT_CHANGE"
/* ����������� ��������� ����������                                   */
#define OPERATOR_EQUALITY         "="
#define OPERATOR_INEQUALITY       "<>"
#define OPERATOR_GREATER_THAN     ">"
#define OPERATOR_EQUAL_GREATER    "=>"
#define OPERATOR_LESS_THAN        "<"
#define OPERATOR_EQUAL_LESS       "<="
#define OPERATOR_BETWEEN          "BETWEEN"
#define OPERATOR_AND              "AND"
#define OPERATOR_NOT              "NOT"
#define OPERATOR_IN               "IN"
/* ����������� ��������� ����, �������� � �������������� �������      */
#define SERVICE_SHOWCASE_ROOT     "ROOT"
#define SERVICE_ALL_ELEMENTS      "ALL"
#define SERVICE_LIKE_SYMBOL       "*"
#define SERVICE_STRING_SYMBOL     "'"
#define SERVICE_IN_BEGIN_SYMBOL   "("
#define SERVICE_IN_END_SYMBOL     ")"
#define SERVICE_IN_LIST_SYMBOL    ","
#define SERVICE_LETTER_CASE       "[!]"
/* ����������� ����� ��������� �������������� ��������                */
/* ��� ��������� (�������)                                            */
#define TYPE_GENERATIONS_NO            "NO_GENERATIONS"
/* ��������                                                           */
#define TYPE_GENERATIONS_HOURLY        "HOURLY_GENERATIONS"
/* ����������                                                         */
#define TYPE_GENERATIONS_DAILY         "DAILY_GENERATIONS"
/* �����������                                                        */
#define TYPE_GENERATIONS_WEEKLY        "WEEKLY_GENERATIONS"
/* ����������                                                         */
#define TYPE_GENERATIONS_MONTHLY       "MONTHLY_GENERATIONS"
/* �������������                                                      */
#define TYPE_GENERATIONS_QUARTERLY     "QUARTERLY_GENERATIONS"
/* ��������                                                           */
#define TYPE_GENERATIONS_YEARLY        "YEARLY_GENERATIONS"
/* �� ����� ���������                                                 */
#define TYPE_GENERATIONS_MODIFICATION  "MODIFICATION_GENERATIONS"
/* ����������� ����� �������������� ��������                          */
/* �������� �� UP                                                     */
#define IO_TYPE_DOCUMENT     "IO_DOCUMENT"
/* ������� �� UP                                                      */
#define IO_TYPE_TABLE        "IO_TABLE"
/* ������� UP                                                         */
#define IO_TYPE_SHOWCASE     "IO_SHOWCASE"
/* ���������������� �������� �� UP                                    */
#define IO_TYPE_SEGMDOC      "IO_SEGMDOC"
/* �������� UP                                                        */
#define IO_TYPE_SCRIPT       "IO_SCRIPT"
/* ����                                                               */
#define IO_TYPE_FILE         "IO_FILE"
/* ����������� ������                                                 */
#define IO_TYPE_EXEC         "IO_EXEC"
/* ������� ������ (http, ftp � �.�.)                                  */
#define IO_TYPE_EXTERNAL     "IO_EXTERNAL"
/* �������� �������                                                   */
PLOTLIB long WINAPI SelectFromShowcase(PSTKMPR     pstk,
                                       PSHOWCASE   handler,
                                       PCASEWORK   pparm,
                                       char       *select_str);

/* ������� ��������� �������, ���������� � ������ �������             */
#define    FGetObjectFromXLink    197
PLOTLIB long WINAPI GetObjectFromXLink(PSTKMPR    pstk,
                                       PCASEWORK  pparm,
                                       PCASEITEM  pitem);

/* ������� ������ ��������� �� ������ �������                         */
#define    FGetXlinkDocum         198
#ifdef VMESA
  #pragma map(GetXlinkDocum, "GETXLDOC")
#endif /* VMESA */
long GetXlinkDocum(PSTKMPR     pstk,
                   PCASEWORK   pparm,
                   char       *doc_path,
                   u short     doc_type);

/* ������� ������ ����� �� ������ �������                             */
#define    FGetXlinkFile          199
#ifdef VMESA
  #pragma map(GetXlinkFile, "GETXLFIL")
#endif /* VMESA */
long GetXlinkFile(PSTKMPR     pstk,
                  PCASEWORK   pparm,
                  char       *file_path);

/* ������� ������ ������� �� ������ �������                           */
#define    FGetXlinkTable         200
#ifdef VMESA
  #pragma map(GetXlinkTable, "GETXLTAB")
#endif /* VMESA */
long GetXlinkTable(PSTKMPR     pstk,
                   PCASEWORK   pparm,
                   char       *table_path);

/* ������� ������ ������� �� ������ �������                           */
#define    FGetXlinkShowcase      201
#ifdef VMESA
  #pragma map(GetXlinkShowcase, "GETXLSC ")
#endif /* VMESA */
long GetXlinkShowcase(PSTKMPR     pstk,
                      PCASEWORK   pparm,
                      char       *showcase_path);

/* ������� ������ �������                                             */
#define    FReadTable             202
PLOTLIB long WINAPI ReadTable(PSTKMPR   pstk,
                              LINK      plink,
                              CURSOR    tab_cursor,
                              void     *tab_buff,
                              u long    buff_size,
                              u long   *tab_size,
                              u long    sort_field_num,
                              u char    sort_direction,
                              u char   *bo_place,
                              bool      pr_decod,
                              long      wait_interval);

/* ������� �������� ������� �� ���������� ��������                    */
#define    FSendScr               203
long  SendScr(PSTKMPR  pstk, PSCR_PARAMS  pscrparm);

/* ������� ��������� ���� � ����������� �������                       */
#define    FSendEvent             204
PLOTLIB long WINAPI SendEvent(PSTKMPR      pstk,
                              PVNT_PARAMS  pvntparm,
                              void        *data,
                              u long       datasize,
                              long         dataplace,
                              u char       datagrif,
                              u char       dataformat,
                              u char       datacode);

/* ���������� ������ � ���SYS                                         */
#define    FAddSysOut             205
long AddSysOut(PSTKMPR pstk,
               SU      su,
               SYS     param);

/* ���������� ������ � ���SYS                                         */
#define    FSetSysOut             206
long SetSysOut(PSTKMPR  pstk,
               u long   src_mask,
               u long   set_mask,
               JSYSOUT  src_rec,
               JSYSOUT  set_rec);

/* ������� ��������� �������� �������                                 */
#define    FChangeShowcaseItem    207
/* ����������� �������� ��������������� ������� (����������)          */
/* �������� �������                                                   */
#define CASE_ITEM_NAME_ID          1  /* ������������ ��������        */
#define CASE_ITEM_OBJ_MODULE_ID    2  /* "������-���������" �������   */
#define CASE_ITEM_OBJ_PATH_ID      3  /* ������ ���� � �������        */
#define CASE_ITEM_OBJ_NAME_ID      4  /* ��� (����) �������           */
#define CASE_ITEM_OBJ_DATE_ID      5  /* ���� ������������� �������   */
#define CASE_ITEM_OBJ_FORMAT_ID    6  /* ������ ������������� ������� */
#define CASE_ITEM_OBJ_ARH_ID       7  /* ��� ��������� �������        */
#define CASE_ITEM_OBJ_EXTN_ID      8  /* ���������� ����� �������     */
#define CASE_ITEM_EXTN_TYPE_ID     9  /* ��� ���������� ����� ������� */
#define CASE_ITEM_XLINK_TYPE_ID   10  /* ��� ������                   */
#define CASE_ITEM_KEYWORDS_ID     11  /* ������ �������� ����         */
#define CASE_ITEM_SUBJECT_ID      12  /* ����                         */
#define CASE_ITEM_SUB_SUBJECT_ID  13  /* �������                      */
#define CASE_ITEM_REGION_ID       14  /* ������                       */
#define CASE_ITEM_SUB_REGION_ID   15  /* ���������                    */
#define CASE_ITEM_OBJ_RESERV_ID   16  /* ��������� ���� ��� �������   */
/* �������� �������                                                   */
PLOTLIB long WINAPI ChangeShowcaseItem(PSTKMPR          pstk,
                                       PSHOWCASE        handler,
                                       void            *pitem_index,
                                       long             interval,
                                       PSC_ITEM_CHANGE  pparams);

/* ������� ���������� �������� UP �� ������ �������                   */
#define    FGetXlinkScript        208
#ifdef VMESA
  #pragma map(GetXlinkScript, "GETXLSCR")
#endif /* VMESA */
long GetXlinkScript(PSTKMPR     pstk,
                    PCASEWORK   pparm,
                    PCASEITEM   pitem);

#define    FzkbReadRecord         209
long zkbReadRecord(PSTKMPR     pstk,
                   PSSLVZKBDS  precord,
                   char       *KodeObj,
                   char       *IdUser);

#define    FzkbAddRecord          210
long zkbAddRecord(PSTKMPR    pstk,
                  SSLVZKBDS  record);

#define    FzkbRemoveRecord       211
long zkbRemoveRecord(PSTKMPR  pstk,
                     char    *KodeObj,
                     char    *IdUser);

#define    FokbReadRecord         212
long okbReadRecord(PSTKMPR     pstk,
                   PSSLVOKBDS  precord,
                   char       *KodeObj,
                   char       *IdUser);

#define    FokbAddRecord          213
long okbAddRecord(PSTKMPR    pstk,
                  SSLVOKBDS  record);

#define    FokbRemoveRecord       214
long okbRemoveRecord(PSTKMPR  pstk,
                     char    *KodeObj,
                     char    *IdUser);

/* ������� �������� ���������� ������                                 */
#define    FSetTaskInfo           215
PLOTLIB long WINAPI SetTaskInfo(PSTKMPR     pstk,
                                USTASKINFO *pustaskinfo);

/* Functions for compatibility with Windows CE                        */
#if defined(_WIN32_WCE)
int rename(char *oldname, char *newname);
int remove(char *filename);
#endif

#ifdef VMESA
/* ����� ��� ������ �������� (������ ��� "��������������" �������     */
/* VM ESA                                                             */
  int vmstpr(int name);

/* ������������ ������� ��� ������ REXX ��������� ���������� �������  */
/* CMS � ���������� ������ � ��������� �����                          */
  #pragma map(upcms,"UPCMSQUE")
  long upcms(u long pstruct);
#endif
/*=========== � � � � � � � � � � �  � � � � � � � (�����) ===========*/
#endif /*PLOTH*/

