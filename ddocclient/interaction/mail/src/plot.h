/**********************************************************************/
/*               Заголовочный файл для Universal Platform             */
/**********************************************************************/

#ifndef PLOTH
#define PLOTH

/* Определяем среду для компиляции                                    */
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
/*#define ACNTFREE - без TESTACNT                                     */

/***************************** П   Л   О   Т **************************/
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

/***************************** П   Л   О   Т **************************/
/*========== С И М В О Л И Ч Е С К И Е   П О Д С Т А Н О В К И =======*/
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

/* Кодировки                                                          */
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

#define JUR_SYS_NAME  "jur_sys.jrn"/* имя файла журнала системных     */
                                   /* сообщений                       */
#define JUR_DAT_NAME  "jur_sys.jdt"/* имя файла журнала системных     */
                                   /* сообщений (данные)              */
#define JI_NAME         "jour_in.jrn"  /* имя файла ЖВС               */
#define JO_NAME         "jour_out.jrn" /* имя файла ЖИС               */
#define JSO_NAME        "jour_so.jrn"  /* имя файла ЖИЗСО             */
#define JFO_NAME        "jour_fo.jrn"  /* имя файла ЖИЗFTP            */
#define JSYSO_NAME      "jour_syo.jrn" /* имя файла ЖИЗSYS            */
#define JSYSI_NAME      "jour_syi.jrn" /* имя файла ЖВЗSYS            */

#define JI_EVENT_MDF    "ji_mdf_record"/* событие обновления ЖВС      */
#define JI_EVENT_ADD    "ji_add_record"/* событие обновления ЖВС      */
#define JI_EVENT_END    "ji_end_record"/* событие обновления ЖВС      */
#define JO_EVENT_MDF    "jo_mdf_record"/* событие обновления ЖИС      */
#define JO_EVENT_ADD    "jo_add_record"/* событие обновления ЖИС      */
#define JO_EVENT_END    "jo_end_record"/* событие обновления ЖИС      */
#define JSO_EVENT_MDF   "jso_mdf_record"/* событие обновления ЖИЗСО   */
#define JSO_EVENT_ADD   "jso_add_record"/* событие обновления ЖИЗСО   */
#define JSO_EVENT_END   "jso_end_record"/* событие обновления ЖИЗСО   */
#define JFO_EVENT_MDF   "jfo_mdf_record"/* событие обновления ЖИЗ FTP */
#define JFO_EVENT_ADD   "jfo_add_record"/* событие обновления ЖИЗ FTP */
#define JFO_EVENT_END   "jfo_del_record"/* событие обновления ЖИЗ FTP */
#define JSYSO_EVENT_MDF "jsyso_mdf_record"/* событие обновления ЖИЗSYS*/
#define JSYSO_EVENT_ADD "jsyso_add_record"/* событие обновления ЖИЗSYS*/
#define JSYSO_EVENT_END "jsyso_end_record"/* событие обновления ЖИЗSYS*/
#define JSYSI_EVENT_MDF "jsysi_mdf_record"/* событие обновления ЖВЗSYS*/
#define JSYSI_EVENT_ADD "jsysi_add_record"/* событие обновления ЖВЗSYS*/
#define JSYSI_EVENT_END "jsysi_end_record"/* событие обновления ЖВЗSYS*/

#define JSYS_ID         "SJRN"     /* Идентификатор ЖCС               */
#define JDAT_ID         "SDAT"     /* Идентификатор ЖCС (данные)      */
#define JI_ID           "IJRN"     /* Идентификатор ЖВС               */
#define JO_ID           "OJRN"     /* Идентификатор ЖИС               */
#define JSO_ID          "SOJR"     /* Идентификатор ЖИЗСО             */
#define JFO_ID          "FOJR"     /* Идентификатор ЖИЗСО FTP         */
#define JSYSO_ID        "SYOJ"     /* Идентификатор ЖИЗSYS            */
#define JSYSI_ID        "SYIJ"     /* Идентификатор ЖВЗSYS            */

#define PLOT_SUZIDN         "SUZ"/* Идентификатор SUZа                */
#define PLOTMSGSIZE         32736/* max размер сообщения в очереди    */
#define PLOTSUSIZE          1840 /* max расширения строки указания    */
#define PLOTINFSIZE         30000/* размер инф-ной части сообщения    */
#define PLOT_WAIT_REPLY     120000/* Идентификатор SUZа               */
#define PLOTPATHSIZE        256  /* max размер полного имени файла    */
#define PLOT_TI_NAME        128  /* мах размер имени ЭО               */
#define PLOTKEYSIZE         8    /* размер ключа временной памяти     */
#ifdef VMESA
#define PLOTINISTRSIZE      80   /* Длина строки словарей и profile's */
#else
#define PLOTINISTRSIZE      1024 /* Длина строки словарей и profile's */
#endif
#ifdef VMESA
  #define DELAYMAX          0    /* max время задержки потока         */
  #define DELAYMIN          0    /* min время задержки потока         */
  #define DELAYDOWN         0    /* дельта + для времени задержки     */
  #define DELAYUP           0    /* дельта - для времени задержки     */
#endif /*VMESA*/
#ifndef VMESA
  #define DELAYMAX          100  /* max время задержки потока         */
  #define DELAYMIN          20   /* min время задержки потока         */
  #define DELAYDOWN         5    /* дельта + для времени задержки     */
  #define DELAYUP           2    /* дельта - для времени задержки     */
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
#define PLOBL               128  /* длина области передачи            */
                                 /* параметров TCA                    */
#define SEGMN               8    /* ч-ло сегментов документа          */
#define STKNUMPARM          16   /* Число параметров функции,         */
                                 /* фиксируемое в стеке               */
#define PLOTTASKADD         4    /* max количество задач для обработки*/
#define PLOTMAXRECSIZE      8000 /* max размер записи в таблице       */
#define PLOTTABSTRSIZE      25500/* размер структуры таблицы          */
#define PLOTTABINFSIZE      30000/* размер инф-ной части сообщения    */
#define PLOTSIZEHESCHTABLE  4096 /* размер хеш-таблицы сжатия         */
#define PLOTOBJSYMBOLNAMELEN   9 /* длина символьного имени объекта   */

/* Варианты значения pcastop                                          */
#define PLOT_STOP_IGN       0    /* Работа разрешена                  */
#define PLOT_STOP_END       1    /* Признак завершения задачи         */
#define PLOT_STOP_REINIT    2    /* Признак переинициализации задачи  */

/* Типы сообщений                                                     */
#define MSG_STOR_TEMP       1    /* Расположено во временной памяти   */
#define MSG_STOR_NULL       2    /* Расположено не во временной памяти*/
#define MSG_OBMN_SYNC       4    /* Синхронное                        */
#define MSG_OBMN_ASYNC      8    /* Асинхронное                       */
#define MSG_OBMN_SYS        16   /* Системное                         */
#define MSG_TYPE_k          32   /* Квитанция о доведении             */
#define MSG_TYPE_K          64   /* Квитанция о постановке на обраб.  */
#define MSG_TYPE_O          128  /* Квитанция об обработке            */
#define MSG_TYPE_MSG        256  /* Сообщение                         */
#define MSG_ADR_LOCAL       512  /* АП == АО                          */
#define MSG_OBMN_INIT       1024 /* Системное                         */
#define MSG_OBMN_FWD        2048 /* Обернутое сообщение               */
#define MSG_OBMN_INIT_ASYNC 4096 /* Асинхронное cистемное             */

#define PLOT_FUNC           0    /* Префикс для идентификации в       */
                                 /* системе функций PLOT              */
#define B204_FUNC           1000 /* Префикс для идентификации в       */
                                 /* системе функций B204              */
#define GNET_FUNC           2000 /* Префикс для идентификации в       */
                                 /* системе функций GNET              */
#define INEX_FUNC           3000 /* Префикс для идентификации в       */
                                 /* системе функций INEX              */
#define PDKL_FUNC           4000 /* Префикс для идентификации в       */
                                 /* системе функций PDKL              */
#define USER_FUNC           5000 /* Префикс для идентификации в       */
                                 /* системе функций USER              */
#define MOCT_FUNC           6000 /* Префикс для идентификации в       */
                                 /* системе функций MOCT              */
#define BAUL_FUNC           7000 /* Префикс для идентификации в       */
                                 /* системе функций BAUL              */
#define WALL_FUNC           8000 /* Префикс для идентификации в       */
                                 /* системе функций WALL              */
#define CASE_FUNC           9000 /* Префикс для идентификации в       */
                                 /* системе функций CASE              */
#define GRAD_FUNC          10000 /* Префикс для идентификации в       */
                                 /* системе функций GRAD              */

typedef void* LINK;              /* дескриптор соед-я с др. модулем   */
typedef void* CURSOR;            /* дескриптор открытой таблицы       */

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
/*=========== О П Р Е Д Е Л Е Н И Е  О Ш И Б О К (начало) ============*/
/*====================================================================*/
/* Все ошибки, которые отсутствуют в данном файле,в документации к    */
/* операционной системе                                               */
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
#define PLOTERRJNOTREC    PLOTERR+1  /* Не найдена запись,            */
                                     /* соответствующая маске поиска  */
#define PLOTERRINVALPARFN PLOTERR+2  /* неправильные входные
                                        параметры для функции         */
#define PLOTERRJDAMAGE    PLOTERR+3  /* Нарушена структура журнала    */
#define PLOTERRTOOMANYTHR PLOTERR+4  /* превышено MAX допустимое
                                        число потоков в процессе      */
#define PLOTERRTOOMANYPRC PLOTERR+5  /* превышено MAX допустимое
                                        число процессов в системе     */
#define PLOTERRJEOF       PLOTERR+6  /* Достигнут конец журнала       */
#define PLOTERRNOACTUAL   PLOTERR+7  /* Данные не актуальны           */
#define PLOTERRNOPCT      PLOTERR+9  /* нет PCT                       */
#define PLOTERRNOPROC     PLOTERR+10 /* нет такого процесса в PCT     */
#define PLOTERRNOSTK      PLOTERR+11 /* нет массива стеков МПР про-
                                        цесса (не удалось создать)    */
#define PLOTERRLINEBROKEN PLOTERR+13 /* разрушена очередь             */
#define PLOTERRPTRLINK    PLOTERR+14 /* попытка повторного соед-я     */
                                     /* с другим модулем              */
#define PLOTERRNOLINE     PLOTERR+15 /* попытка записи в несуществу-
                                        ющую очередь                  */
#define PLOTERRLINENOMSG  PLOTERR+17 /* нет записей в очереди         */
#define PLOTERRNOLINK     PLOTERR+19 /* не удалось установить соед-е  */
                                     /* с указанным модулем           */
#define PLOTERRMAXLINK    PLOTERR+20 /* достигнуто максимально раз-   */
                                     /* решенное количество соед-й    */
#define PLOTERRSLVNUM     PLOTERR+21 /* ошибка инициализации SLVNUM   */
#define PLOTERRSLVADR     PLOTERR+22 /* ошибка инициализации SLVADR   */
#define PLOTERRSLV21Y     PLOTERR+23 /* ошибка инициализации SLV21Y   */
#define PLOTERRIUCVSET    PLOTERR+24 /* ошибка операции IUCV SET      */
#define PLOTERRIUCVCLR    PLOTERR+25 /* ошибка операции IUCV CLR      */
#define PLOTERRINVALADDR  PLOTERR+26 /* неправильный адрес в СУ       */
#define PLOTERRNOTNUM     PLOTERR+27 /* ошибка при поиске номера ВУ   */
                                     /* по адресу получателя          */
#define PLOTERRNOTADR     PLOTERR+29 /* нет такого ном. ВУ в SLVADR   */
#define PLOTERRFIELDNAME  PLOTERR+32 /* неизвестное имя поля в СУ     */
#define PLOTERRPROCNOWREG PLOTERR+34 /* задача уже зарегистрирована   */
#define PLOTERRPROCNOTREG PLOTERR+35 /* задача не зарегистрирована    */
#define PLOTERRNOWENQ     PLOTERR+36 /* повторное взятие потоком      */
                                     /* монопольного управления на    */
                                     /* один и тот же ресурс          */
#define PLOTERRNOTREC     PLOTERR+39 /* информационн. объект не наден */
#define PLOTERRNOACCREC   PLOTERR+40 /* нет доступа к инф. объекту    */
#define PLOTERRACCESS     PLOTERR+41 /* общая ошибка в методе доступа */
#define PLOTERRBADQUERY   PLOTERR+42 /* ошибочный запрос              */
#define PLOTERRBADRECLEN  PLOTERR+43 /* неверная длина данных         */
#define PLOTERRRECEXIST   PLOTERR+44 /* документ уже существует       */
#define PLOTERRDATAOOB    PLOTERR+45 /* Выход данных за пределы       */
                                     /* области                       */
#define PLOTERRKFSBADFLEN PLOTERR+46 /* неверная длина поля в KFSCODTB*/
#define PLOTERRKFSNOFIELD PLOTERR+47 /* неизвестен тип поля в KFSCODTB*/
#define PLOTERRIUCVCONECT PLOTERR+48 /* ошибка операции IUCV CONNECT  */
#define PLOTERRIUCVACCEPT PLOTERR+49 /* ошибка операции IUCV ACCEPT   */
#define PLOTERRIUCVSEND   PLOTERR+50 /* ошибка операции IUCV SEND     */
#define PLOTERRIUCVRECEIV PLOTERR+51 /* ошибка операции IUCV RECEIVE  */
#define PLOTERRIUCVREPLY  PLOTERR+52 /* ошибка операции IUCV REPLY    */
#define PLOTERRIUCVSEVER  PLOTERR+53 /* ошибка операции IUCV SEVER    */
#define PLOTERRIUCVDELAY  PLOTERR+54 /* ошибка при вызове функции     */
                                     /* delay из IUCV-потоков         */
#define PLOTERRNOTABLE    PLOTERR+55 /* отсутствует набор данных      */
#define PLOTERRRTEMPMEM   PLOTERR+57 /* ошибка чтения из временной    */
                                     /* памяти                        */
#define PLOTERRRTEMPEOF   PLOTERR+58 /* конец данных во временной     */
                                     /* памяти                        */
#define PLOTERRTMPFRMT    PLOTERR+61 /* Неправильный формат управля-  */
                                     /* ющего блока временной памяти  */
#define PLOTERRNOSU       PLOTERR+62 /* Данные не имеют строки ука-   */
                                     /* зания                         */
#define PLOTERRNOREZIM    PLOTERR+64 /* данный режим работы функции   */
                                     /* пока не поддерживается        */
#define PLOTERRTIMEOUT    PLOTERR+65 /* Время ожидания освобождения   */
                                     /* истекло                       */
#define PLOTERRPROCNOWAIT PLOTERR+66 /* Получатель сообщения не       */
                                     /* находится в режиме ожидания   */
#define PLOTERRNOTHREAD   PLOTERR+67 /* Нет потока-получателя данных  */
#define PLOTERRPRP        PLOTERR+68 /* Ошибка на этапе регистрации   */
#define PLOTERRNOPROFILE  PLOTERR+69 /* Ошибка работы с файлами       */
                                     /* инициализации                 */
#define PLOTERRINITLINE   PLOTERR+70 /* Ошибка инициализации очереди  */
#define PLOTERRNOACCESS   PLOTERR+71 /* У задачи нет доступа для ре-  */
                                     /* гистрации                     */
#define PLOTERRALRSTARTED PLOTERR+72 /* Система уже запущена          */
#define PLOTERRFORMATSLV  PLOTERR+73 /* Ошибка инициализации FORMAT   */
#define PLOTERRNOTYPE     PLOTERR+74 /* Тип док-та не определен в     */
                                     /* FORMAT.SLV                    */
#define PLOTERRMOCTRESET  PLOTERR+75 /* Задача MOCT была перезагружена*/
#define PLOTERRNODATAHDR  PLOTERR+85 /* Нет заголовка данных при      */
                                     /* передаче                      */
#define PLOTERRSIZESUEXT  PLOTERR+86 /* Ошибочный размер расширения   */
                                     /* строки указания               */
#define PLOTERRSIZEDATA   PLOTERR+87 /* Ошибочный размер данных на    */
                                     /* приеме                        */
#define PLOTERRNODATA     PLOTERR+88 /* Ошибка при приеме данных      */
#define PLOTERRMSGFORMAT  PLOTERR+89 /* Неверный формат сообщения     */
#define PLOTERRFRENAME    PLOTERR+90 /* Ошибка при переимен. файла    */
#define PLOTERRSTKOVRFLW  PLOTERR+91 /* Стек потока переполнен        */
#define PLOTERRFOPEN      PLOTERR+92 /* Ошибка при открытии файла     */
#define PLOTERRFCLOSE     PLOTERR+93 /* Ошибка при закрытии файла     */
#define PLOTERRFWRITE     PLOTERR+94 /* Ошибка при записи в файл      */
#define PLOTERRFREAD      PLOTERR+95 /* Ошибка при чтении из файла    */
#define PLOTERRFSEEK      PLOTERR+96 /* Ошибка при выполнении fseek   */
#define PLOTERRFDELETE    PLOTERR+97 /* Ошибка при удалении файла     */
#define PLOTERRJURNBLOCK  PLOTERR+98 /* Запись в журнал блокирована   */
#define PLOTERRTASKENV    PLOTERR+99 /* Отсутствует окружение задачи  */
#define PLOTERRPTRTABOPN  PLOTERR+100/* Ошибка открытия таблицы       */
#define PLOTERRMAXCURSOR  PLOTERR+101/* Кол-во курсоров- максимально  */
#define PLOTERRLOWMEM     PLOTERR+102/* Не достаточно памяти          */
#define PLOTERRNOACCMDF   PLOTERR+103/* Нет доступа для модификации   */
#define PLOTERRSENT       PLOTERR+104/* Нет доступа для рассылки      */
#define PLOTERRDINAMSEL   PLOTERR+105/* Ошибка при формировании динам.*/
                                     /* SELECTa */
#define PLOTERRNOSTRDS    PLOTERR+106/* Неправильная строка для динам.*/
                                     /* SELECTa */
#define PLOTERRREADYCAN   PLOTERR+107/* Ошибка при настройке обмена   */
#define PLOTERRRECGARD    PLOTERR+108/* Ошибка при чтении из GARDOV   */
#define PLOTERRNOSTRUCT   PLOTERR+110/* Структ. не занесена в KFSCODTB*/
#define PLOTERRNOENQ      PLOTERR+111/* Нет Enq                       */
#define PLOTERRNOTRABADR  PLOTERR+112/* Нет более готового канала     */
#define PLOTERRNOTMAINSNT PLOTERR+113/* Нет канала в SLVSNT           */
#define PLOTERRFTPPROPS   PLOTERR+120/* Неверные свойства ЭО          */
#define PLOTERRFTPREZIM   PLOTERR+121/* Необслуживаемый режим запроса */
#define PLOTERRFTPPARAM   PLOTERR+122/* Неверные вх. параметры        */
#define PLOTERRFTPSYSBL   PLOTERR+123/* Нарушена целостность сис. бл. */
#define PLOTERRFTPSYSMK   PLOTERR+124/* Ошибка в системной МК         */
#define PLOTERRFTPOTVET   PLOTERR+125/* Ошибка заполнения области отв.*/
#define PLOTERRFTPOPENND  PLOTERR+126/* Ошибка открытия НД            */
#define PLOTERRFTPSYSPR   PLOTERR+127/* Ошибка при работе НД SYSPRINT */
#define PLOTERRFTPNFOUND  PLOTERR+128/* ЭО недоступен или не найден   */
#define PLOTERRFTPTIOTUCB PLOTERR+129/* Ошибка при работе c TIOT/UCB  */
#define PLOTERRFTPCREATE  PLOTERR+130/* Ошибка при создании НД        */
#define PLOTERREOF        PLOTERR+131/* Достигнут конец файла         */
#define PLOTERRCODEUNKN   PLOTERR+132/* Неизвестная кодировка данных  */
#define PLOTERRFTPERRNAME PLOTERR+133/* Неверное имя ЭО               */
#define PLOTERRDRVNOREADY PLOTERR+134/* Устройство не готово          */
#define PLOTERRFCREATE    PLOTERR+135/* Ошибка при создании файла     */
#define PLOTERRCOMPRESS   PLOTERR+136/* Ошибка при сжатии данных      */
#define PLOTERRDECOMPRESS PLOTERR+137/* Ошибка при расжатии данных    */
#define PLOTERRNOFLDSRT   PLOTERR+138/* Неправильно задано поле для   */
                                     /* сортировки таблицы            */
#define PLOTERRSNCCADRNUM PLOTERR+139/* Нарушена последовательность   */
                                     /* кадров при синхронном обмене  */
#define PLOTERRNOUCHDATA  PLOTERR+140/* Не получены учетные данные ИО */
#define PLOTERRFSIZE      PLOTERR+141/* Ошибка при получении размера  */
                                     /* файла                         */
#define PLOTERRFTPNOSTART PLOTERR+142/* Ошибка при запуске процесса   */
#define PLOTERRBADAOAP    PLOTERR+143/* Не верно заданы АО или АП     */
#define PLOTERRNORIGHT    PLOTERR+144/* Полномочия не подтверждены    */
#define PLOTERRBADFMDCODE PLOTERR+145/* Нет такого кода операции      */
#define PLOTERRNOTRANG    PLOTERR+146/* Не загружена таблица рангов   */
#define PLOTERRBADRANG    PLOTERR+147/* Нет такого ранга              */
#define PLOTERRNOAO       PLOTERR+148/* Не задано имя модуля, заявив-
                                        шего операцию                 */
#define PLOTERRNOAP       PLOTERR+149/* Не задан адрес получателя     */
#define PLOTNEEDEXTRTEST  PLOTERR+150/* Полномочия подтверждены, тре-
                                        буется дополнительная проверка
                                        РД дискреционным методом      */
#define PLOTERRNOMODULE   PLOTERR+151/* Нет такого модуля             */
#define PLOTERRNOPATH     PLOTERR+152/* Не задан путь (устр-во)
                                        загрузки/выгрузки             */
#define PLOTERRNOPRTU     PLOTERR+153/* Не задано имя устр-ва печати  */
#define PLOTERRBADGRIF    PLOTERR+154/* Недопустимый гриф информации  */
#define PLOTERRNOACCSOD   PLOTERR+155/* Нет доступа (данный код гене-
                                        рируется при наличии прав на
                                        выполнение заявленной операции,
                                        но при отсутствии доступа к
                                        объекту доступа - адресату
                                        информации, каталогу загрузки
                                        (выгрузки) информации, устрой-
                                        ству печати и т.д.)           */
#define PLOTERRNONIPP     PLOTERR+156/* Не задан номер порта          */
#define PLOTERRNOMDLM     PLOTERR+157/* Не загружена матрица доступа  */
#define PLOTERRTLINENOWINIT PLOTERR+158/* Попытка повторной инициали- */
                                       /* зации очереди потока        */
#define PLOTERRINVALUSL   PLOTERR+159/* Неверные условия отбора       */
#define PLOTWRKPERMIT     PLOTERR+160/* Разрешение работы пользователя*/
#define PLOTERRPASSCOMPR  PLOTERR+161/* Предъявлен-й пароль не получен*/
#define PLOTERRPASSCHANGE PLOTERR+162/* Предъявленный пароль заменен  */
#define PLOTERRPASSWORD   PLOTERR+163/* Персональный пароль неверен   */
#define PLOTERRUSERABS    PLOTERR+164/* Нет такого пользователя       */
#define PLOTERRUSERBLOKED PLOTERR+165/* Пользователь заблокирован     */
#define PLOTERRMODULEABS  PLOTERR+166/* Попытка запуска несущ-го ЛМ   */
#define PLOTERRMODULEACSS PLOTERR+167/* Нет допуска к лог. модулю     */
#define PLOTERRADMINACSS  PLOTERR+168/* Нет допуска к функциям адм.СЗИ*/
#define PLOTERRPASSGENER  PLOTERR+169/* Ошибка при генерации паролей  */
#define PLOTWRKPERMITNEWP PLOTERR+170/* Разрешение работы + выдача ПП */
#define PLOTERRPROTPROCSS PLOTERR+171/* Ошибка при обр. макета запроса*/
#define PLOTERRGRIFCHNGACSS PLOTERR+172/* Нет полномочий на изм. грифа*/
#define PLOTERRTYPECHNGACSS PLOTERR+173/* Нет полномочий на изм. типа */
#define PLOTERRVIZASETACSS PLOTERR+174/* Нет полномочий на визирование*/
#define PLOTERRVIZARESETACSS PLOTERR+175/* Нет полномочий на снятие
                                                                 визы */
#define PLOTERRPROPRTACSS PLOTERR+176/* Нет полномочий на просмотр
                                        свойств информац-го объекта   */
#define PLOTERRVIZASET    PLOTERR+177/* Виза уже установлена          */
#define PLOTERRTRASTRELSH PLOTERR+178/* Доверительные отношения для
                                        данного модуля не установлены */
#define PLOTERRACSSADDR   PLOTERR+179/* Разрешенные адресаты для
                                        данного модуля не установлены */
#define PLOTERRMODACSSPATH PLOTERR+180/* Для каталога (устройства) не
                                             заданы допущенные модули */
#define PLOTERRMAKETABCRC PLOTERR+181/* Таблица контроля целостности
                                        не сформирована. Не найдена
                                        таблица контролир-х объектов  */
#define PLOTERRTESTCRC    PLOTERR+182/* Нарушение целостности ПО и ИО */
#define PLOTERRMDMCRC     PLOTERR+183/* Нарушена целостность МДМ      */
#define PLOTERRRANGMAKE   PLOTERR+184/* Ошибка при создании ранга     */
#define PLOTERRPORTNUMB   PLOTERR+185/* Указанный ip-порт не найден   */
#define PLOTERRPORTNOPARM PLOTERR+186/* Указанные параметры доступа по
                                        ip-порту не найдены           */
#define PLOTERRPORTDEFN   PLOTERR+187/* Указан-й ip-порт уже определен*/
#define PLOTERRPORTPRPARM PLOTERR+188/* Указанные параметры доступа по
                                        ip-порту уже установлены      */
#define PLOTERRIOBLOCKED  PLOTERR+189/* Информационный объект завизиро-
                                        ван. Заявленная операции над ИО
                                        запрещена                     */
#define PLOTPATHOLDER     PLOTERR+190/* Заданный том (каталог) не описан
                                        в матрице доступа логического
                                        модуля, но является родительским
                                        для одного из описанных       */
#define PLOTERRUNTGRIFCHG PLOTERR+191/* Для каталога (устройства) метка
                                        конфиденциальности уже
                                        установлена                   */
#define PLOTERRUNTGRIFSET PLOTERR+192/* Для заданного каталога (устрой-
                                        ства) не установлена метка кон-
                                        фиденциальности или установлен-
                                        ная метка не соответствует мет-
                                        ке конфиденциальности информа-
                                        ционного объекта              */
#define PLOTERRUSERLEVEL  PLOTERR+193/* Обнаружено несоответствие уров-
                                        ней полномочий пользователя, за-
                                        регистрированного в ОС и пользо-
                                        вателя, пытающегося зарегистри-
                                        роваться в АС                 */
#define PLOTERRIOXLINK    PLOTERR+194/* Полученный информационный объект
                                        является ссылкой              */
#define PLOTERRITEMGRIF   PLOTERR+195/* Метка конфиденциальности ЭО в
                                        СЗИ UP не соответствует метке
                                        конфиденциальности ЭО в ОС    */
#define PLOTERRPATHSETS   PLOTERR+196/* Заданный том (каталог) не описан
                                        в матрице доступа логического
                                        модуля                        */
#define PLOTMYTEMPPATH    PLOTERR+197/* Заданный каталог (том) является
                                        хранилищем временной памяти сво-
                                        его логического модуля        */
#define PLOTERRPATHOLDER  PLOTERR+198/* Заданный каталог (том) является
                                        родительским для каталога (тома)
                                        , уже имеющего метку конфиденци-
                                        альности                      */
#define PLOTERRPATHDOTHER PLOTERR+199/* Заданный каталог (том) является
                                        дочерним по отношению к каталогу
                                        (тому), уже имеющему метку кон-
                                        фиденциальности               */
#define PLOTERRPATHSTRTAB PLOTERR+201/* В заданном каталоге (томе) ука-
                                        зан модуль, который является
                                        подмножеством множества модулей,
                                        определенного в таблице для
                                        идентичного каталога (тома)   */
#define PLOTERRPATHTABSTR PLOTERR+202/* В таблице уже существует иден-
                                        тичный каталог (том), в котором
                                        указан модуль, являющийся под-
                                        множеством множества модулей,
                                        определенного для заданного ка-
                                        талога (тома)                 */
#define PLOTERRNOPRINTER  PLOTERR+203/* Не найдено доступное устройство
                                        печати  или гриф устройства не
                                        соответствует грифу информации*/
#define PLOTERRCASESUBJECTS PLOTERR+204 /* Некорректно задан список
                                           тем витрины                */
#define PLOTERRCASEXMLENTRY PLOTERR+205 /* Нарушена структура
                                           XML-описания витрины       */
#define PLOTERRCASEITEMINDEX PLOTERR+206/* Некорректно задан индекс
                                           элемента витрины           */
#define PLOTERRCASEMAXLEVELS PLOTERR+207/* Достигнут максимальный
                                           уровень вложенности
                                           элементов витрины          */
#define PLOTERRCASENOFOLDER PLOTERR+208 /* Нет такой папки на витрине */
#define PLOTERRCASENOITEM PLOTERR+209   /* Элемент витрины не найден  */
#define PLOTERRCASEITEMTYPE PLOTERR+210/* Неверен тип элемента витрины*/
#define PLOTERRCASEFOLDERENTRY PLOTERR+211/* Некорректно задано
                                             наименование темы (под-
                                             темы, региона или мест-
                                             ности) папки витрины     */
#define PLOTERRSLVOBJ     PLOTERR+212/* ошибка инициализации SLVOBJ   */
#define PLOTERRYNDEXFORMAT PLOTERR+213/* Неверен формат реестра витрин
                                         логического модуля           */
#define PLOTERRYNDEXSIZE  PLOTERR+214 /* Ошибочный размер реестра
                                         витрин логического модуля    */
#define PLOTERRYNDEXMAXSIZE  PLOTERR+215 /* Превышен максимально
                                            допустимый размер реестра
                                            витрин логического модуля */
#define PLOTERRYNDEXSTRUCT  PLOTERR+216 /* Нарушена структура реестра
                                           витрин логического модуля  */
#define PLOTERRCASENOTREG  PLOTERR+217 /* Витрина не зарегистрирована
                                          в реестре витрин логического
                                          модуля                      */
#define PLOTERRCASEREGEXIST  PLOTERR+218 /* Витрина уже зарегистриро-
                                            вана в реестре витрин
                                            логического модуля        */
#define PLOTERRRECNOTFOUND  PLOTERR+219 /* Нет такой записи в таблице */
#define PLOTERRCASEEMPTY    PLOTERR+220 /* Нет элементов на витрине   */
#define PLOTERRCASESELECTSTRSIZE  PLOTERR+221 /* Недопустимый размер
                                                 строки условий поис-
                                                 ка элементов витрины */
#define PLOTERRCASESELECTOPERATOR  PLOTERR+222 /* Недопустимый оператор
                                                  для параметра строки
                                                  условий поиска элемен-
                                                  тов витрины         */
#define PLOTERRCASESELECTPARMVALUE  PLOTERR+223 /* Недопустимое значение
                                                   для параметра строки
                                                   условий поиска
                                                   элементов витрины  */
#define PLOTERRCASESELECTTOOMANYPARM  PLOTERR+224 /* Параметр строки
                                                     условий поиска
                                                     элементов витрины
                                                     задан более одного
                                                     раза             */
#define PLOTERRFIELDTYPEUNKNOWN  PLOTERR+225 /* Неизвестный тип поля  */
#define PLOTERRFORMATPROCESSING  PLOTERR+226 /* Обработчик указанного
                                                формата данных не
                                                определен             */
#define PLOTERROBJECTPATH  PLOTERR+227 /* Путь к объекту не задан или
                                          задан некорректно           */

#define PLOTERRSLVVNT     PLOTERR+228 /*  Ошибка инициализации SLVVNT */
#define PLOTERRFIELDNOTFOUND  PLOTERR+229 /* Нет такого поля в таблице*/
#define PLOTERRSELECT     PLOTERR+230 /* Нет записей (элементов),
                                         удовлетворяющих условиям
                                         поиска                       */
#define PLOTERRCASEITEMPARMID  PLOTERR+231 /* Неверно указан
                                              идентификатор параметра
                                              элемента витрины        */
#define PLOTERRNOCASEITEMPARMS  PLOTERR+232 /* Не задан список
                                               параметров (свойств)
                                               элемента витрины       */
#define PLOTERRCASEITEMPARMS  PLOTERR+233 /* Указанные параметры
                                             (свойства) не входят в
                                             состав структуры элемента
                                             витрины                  */
#define PLOTERRCASEITEMNAME  PLOTERR+234 /* Некорректно задано
                                            наименование элемента
                                            витрины                   */
#define PLOTERRCASEITEMOBJMOD  PLOTERR+235 /* В элементе витрины
                                              некорректно задано имя
                                              модуля-владельца объекта*/
#define PLOTERRCASEITEMOBJPATH  PLOTERR+236 /* В элементе витрины
                                               некорректно задан путь
                                               к объекту              */
#define PLOTERRCASEITEMOBJNAME  PLOTERR+237 /* В элементе витрины
                                               некорректно задано имя
                                               (ключ) объекта         */
#define PLOTERRCASEITEMOBJARHTYPE  PLOTERR+238 /* В элементе витрины
                                                  некорректно задан
                                                  тип поколений
                                                  объекта             */
#define PLOTERRCASEITEMOBJEXTN  PLOTERR+239 /* В элементе витрины
                                               некорректно задано
                                               расширение имени
                                               объекта                */
#define PLOTERRCASEITEMOBJEXTNTYPE  PLOTERR+240 /* В элементе витрины
                                                   некорректно задан
                                                   тип расширения имени
                                                   объекта            */
#define PLOTERRCASEITEMKEYWORDS  PLOTERR+241 /* В элементе витрины
                                                некорректно задан
                                                список ключевых слов  */
#define PLOTERRCHANNELGRIF       PLOTERR+242 /* Метка конфиденциаль-
                                                ности линии связи ниже
                                                метки конфиденциаль-
                                                ности передаваемой
                                                информации            */
#define PLOTERRDSNOTRUE   PLOTERR+243       /* Подпись не верна       */
#define PLOTERRDSCOMPR    PLOTERR+244       /* Ключ скомпрометирован  */
#define PLOTERRDSEXPIRE   PLOTERR+245       /* Ключ просрочен         */
#define PLOTERRDSNOZKEY   PLOTERR+246       /* Закрытый ключ не найден*/
#define PLOTERRDSNOSLV    PLOTERR+247       /* Таблица открытых ключей
                                               не загружена           */
#define PLOTERRDSERSIGN   PLOTERR+248       /* Не удалось подписать   */
                                            /* сообщение              */
#define PLOTERRDSNOOKEY   PLOTERR+249       /* Открытый ключ не найден*/
#define PLOTERRNOSRDO     PLOTERR+250       /* Не загружен словарь-
                                               справочник объектов АС */
#define PLOTERRNOLISTSLMS  PLOTERR+251      /* Не загружен связный
                                               список словарей-
                                               справочников штатных
                                               расписаний объектов АС */
#define PLOTERRNOSLMP  PLOTERR+252          /* Не загружен словарь-
                                               справочник пользователей
                                               АС                     */
#define PLOTERRNOOBJECTINSRDO  PLOTERR+253  /* Объект не описан в
                                               словаре-справочнике
                                               объектов АС            */
#define PLOTERRNOSLMS  PLOTERR+254          /* Не загружен словарь-
                                               справочник штатного
                                               расписания указанного
                                               объекта АС             */
#define PLOTERRNOOBJECTUSERS  PLOTERR+255   /* Пользователи объекта не
                                               описаны в словаре-
                                               справочнике пользователей
                                               АС                     */

/* Константы ошибок (признаков результата) задачи INEX                */
#define INEXERRBRWSEVER      INEXERR+1/* Броузер отменил запрос       */
#define INEXERRHTTPHEADEND   INEXERR+2/* Нет признака окончания заго-
                                         ловка HTTP-запроса           */
#define INEXERRMETHOD        INEXERR+3/* Указанный в запросе метод
                                         HTTP в настоящий момент не
                                         поддерживается               */
#define INEXERRNOHTTPVERSION INEXERR+4/* В заголовке запроса не ука-
                                         зана версия используемого
                                         протокола HTTP               */
#define INEXERRHTTPVERSION   INEXERR+5/* Версия протокола HTTP, в
                                         соответствии с которым оформ-
                                         лен запрос, в настоящий мо-
                                         мент не поддерживается       */
#define INEXERRNOPOSTLEN     INEXERR+6/* В запросе, использующем метод
                                         POST, нет параметра
                                         Content-Length, определяющего
                                         длину тела запроса           */
#define INEXERRBADPOSTLEN    INEXERR+7/* В запросе, использующем метод
                                         POST, неверно оформлен пара-
                                         метр Content-Length          */
#define INEXERRREQUESTTYPE   INEXERR+8/* Неверен тип запроса          */
#define INEXERRNOCGICLASS    INEXERR+9/* Не указан класс CGI-сценария */
#define INEXERRCGICLASS      INEXERR+10/* Неверен класс CGI-сценария  */
#define INEXERRNOCGIDLLPLACE INEXERR+11/* Не указано местоположение
                                          CGI-сценария класса DLL     */
#define INEXERRCGIDLLPLACE   INEXERR+12/* Неверно указано местоположе-
                                          ние CGI-сценария класса DLL */
#define INEXERRNOCGIDLLMODUL INEXERR+13/* Не указано логическое имя мо
                                          дуля, в БД которого хранится
                                          CGI-сценарий класса DLL     */
#define INEXERRCGIDLLMODUL   INEXERR+14/* Некорректно указано логи-
                                          ческое имя модуля, в БД ко-
                                          торого хранится CGI-сценарий
                                          класса DLL                  */
#define INEXERRNOCGIDLLNAME  INEXERR+15/* Не указано имя CGI-сценария
                                          класса DLL                  */
#define INEXERRCGIDLLNAME    INEXERR+16/* Некорректно указано имя
                                          CGI-сценария класса DLL     */
#define INEXERRNOCGIDLLFUNC  INEXERR+17/* Не указано имя функции,
                                          вызываемой из CGI-сценария
                                          класса DLL                  */
#define INEXERRCGIDLLFUNC    INEXERR+18/* Некорректно указано имя
                                          функции, вызываемой из
                                          CGI-сценария класса DLL     */
#define INEXERRNOCGITASKNAME INEXERR+19/* Не указано имя CGI-сценария
                                          класса TASK                 */
#define INEXERRCGITASKNAME   INEXERR+20/* Некорректно указано имя
                                          CGI-сценария класса TASK    */
#define INEXERRCGIDLLFUNCNF  INEXERR+21/* Не найдена точка входа в
                                          функцию CGI-сценария
                                          класса DLL                  */
#define INEXERRDBEREQUEST    INEXERR+22/* Неверно построен запрос DBE */
#define INEXERRDBENOOTYPE    INEXERR+23/* В запросе DBE не указан тип
                                          информационного объекта     */
#define INEXERRDBEOTYPE      INEXERR+24/* В запросе DBE неверно указан
                                          тип информационного объекта */
#define INEXERRDBENOMDNAME   INEXERR+25/* В запросе DBE не указан
                                          модуль-источник
                                          информационного объекта     */
#define INEXERRDBEMDNAME     INEXERR+26/* В запросе DBE некорректно
                                          указан модуль-источник
                                          информационного объекта     */
#define INEXERRDBENODBNAME   INEXERR+27/* В запросе DBE не указано
                                          имя БД                      */
#define INEXERRDBEDBNAME     INEXERR+28/* В запросе DBE некорректно
                                          указано имя БД              */
#define INEXERRDBENONDNAME   INEXERR+29/* В запросе DBE не указано
                                          имя НД                      */
#define INEXERRDBENDNAME     INEXERR+30/* В запросе DBE некорректно
                                          указано имя НД              */
#define INEXERRDBENOONAME    INEXERR+31/* В запросе DBE не указано
                                          имя информационного объекта */
#define INEXERRDBEONAME      INEXERR+32/* В запросе DBE некорректно
                                          указано имя информационного
                                          объекта                     */
#define INEXERRDBENOODATE    INEXERR+33/* В запросе DBE не указано
                                          значение параметра "DATE"
                                          (даты информационного
                                          объекта)                    */
#define INEXERRDBEODATE      INEXERR+34/* В запросе DBE некорректно
                                          указана дата информационного
                                          объекта                     */
#define INEXERRDBENOOSEGM    INEXERR+35/* В запросе DBE не указано
                                          значение параметра "SEGM"
                                          (сегмента информационного
                                          объекта)                    */
#define INEXERRDBEOSEGM      INEXERR+36/* В запросе DBE неверно задан
                                          сегмент информационного
                                          объекта                     */
#define INEXERRNOSERVICECLASS INEXERR+37/* Не указан класс сервиса    */
#define INEXERRSERVICECLASS  INEXERR+38/* Некорректно указан класс
                                          сервиса                     */

/* Константы ошибок (признаков результата) задачи MOCT                */
#define MOCTERRERRSYSTEM  MOCTERR+1 /* Системная ошибка при отправке  */
#define MOCTERRBREAKMSG   MOCTERR+2 /* Ошибка при работе с сообщением
                                       - снять с гарантийки           */
#define MOCTERRTRANSMIT   MOCTERR+3 /* Ошибка связи при отправке      */
#define MOCTERRSEVER      MOCTERR+4 /* Во время сеанса обмена произо-
                                       шла перезагрузка драйвера УПО  */
#define MOCTERRSTOPWORK   MOCTERR+5 /* Получен сигнал о завершении   */
#define MOCTERRNOCGIFUNC  MOCTERR+6 /* Не определена функция вызова
                                       сценария-обработчика для
                                       указанного формата данных      */

/* Константы ошибок (признаков результата) задачи USER                */
#define USERERRBREAK      USERERR+1 /* Операция прервана пользователем*/

/* Константы ошибок (признаков результата) задачи CASE                */
#define CASEERRSYNTAX     CASEERR+1 /* Некорректно произведен синтакси-
                                       ческий анализ строки условий
                                       поиска элементов витрины       */
/*====================================================================*/
/*=========== О П Р Е Д Е Л Е Н И Е  О Ш И Б О К (конец) =============*/
/*====================================================================*/

/*====================================================================*/
/*=================== Константы для FMD (начало) =====================*/
/*====================================================================*/
/* КОДЫ ОПЕРАЦИЙ                                                      */
/* ФУНКЦИИ ОБРАБОТКИ МАТРИЦЫ ДОСТУПА ЛОГИЧЕСКОГО МОДУЛЯ.              */
/* ОПРЕДЕЛЕНИЕ ШКАЛЫ РЕГИСТРИРУЕМЫХ СОБЫТИЙ                           */
/*                                                                    */
/* Коды операций функции обработки матрицы доступа                    */
/*                                                                    */
/* ДОСТУП К ВЫЧИСЛИТЕЛЬНОЙ УСТАНОВКЕ И ЛОГИЧЕСКОМУ МОДУЛЮ             */
/* без дополнительной проверки разграничения доступа                  */
#define PLOT_FMDACSVU 1  /* доступ ко всем ВУ                         */
#define PLOT_FMDACSLM 2  /* доступ ко всем ЛМ                         */
/* ОПЕРАЦИИ НАД ДОКУМЕНТАМИ БАЗ ДАННЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ             */
#define PLOT_FMDDMAKE 3  /* создание                                  */
#define PLOT_FMDDCHAT 4  /* изменение грифа                           */
#define PLOT_FMDDCHUT 5  /* изменение типа                            */
#define PLOT_FMDDSETV 6  /* визирование                               */
#define PLOT_FMDDRSTV 7  /* снятие визы                               */
#define PLOT_FMDDCRD  8  /* управление РД                             */
#define PLOT_FMDDREAD 9  /* чтение                                    */
#define PLOT_FMDDCHNG 10 /* изменение                                 */
#define PLOT_FMDDDELT 11 /* удаление                                  */
/* ОТПРАВКА ИНФОРМАЦИИ (ДОСТУП К КАНАЛАМ СВЯЗИ)                       */
#define PLOT_FMDSNDS  12 /* отправка сообщения                        */
#define PLOT_FMDSNDD  13 /* отправка документа                        */
#define PLOT_FMDSNDSA 14 /* отправка сообщения категории срочности "A"*/
#define PLOT_FMDSNDSB 15 /* отправка сообщения категории срочности "B"*/
#define PLOT_FMDSNDSC 16 /* отправка сообщения категории срочности "C"*/
#define PLOT_FMDSNDDA 17 /* отправка документа категории срочности "A"*/
#define PLOT_FMDSNDDB 18 /* отправка документа категории срочности "B"*/
#define PLOT_FMDSNDDC 19 /* отправка документа категории срочности "C"*/
/* ВЫГРУЗКА (ЗАГРУЗКА) ИНФОРМАЦИИ                                     */
#define PLOT_FMDUNLD  20 /* выгрузка                                  */
#define PLOT_FMDLOAD  21 /* загрузка                                  */
/* ПЕЧАТЬ ИНФОРМАЦИИ                                                  */
#define PLOT_FMDPRINT 22 /* печать информации                         */
/* УПРАВЛЕНИЕ ВЫЧИСЛИТЕЛЬНЫМ ПРОЦЕССОМ НА ЛОГИЧЕСКИХ МОДУЛЯХ          */
#define PLOT_FMDSTRT  23 /* запуск задач                              */
#define PLOT_FMDSTPT  24 /* завершение задач                          */
#define PLOT_FMDSYSJR 25 /* просмотр журналов ЛМ                      */
#define PLOT_FMDJRVU  26 /* просмотр журналов ВУ                      */
#define PLOT_FMDMEM   27 /* Операции над памятью ЛМ                   */
#define PLOT_FMDGARD  28 /* Управление гарантированным доведением     */
#define PLOT_FMDACIPP 29 /* доступ к ip-портам                        */
/* ОПЕРАЦИИ НАД ФАЙЛАМ (ТОМАМ, КАТАЛОГАМ) ОПЕРАЦИОННОЙ СИСТЕМЫ        */
#define PLOT_FMDFSVL  30 /* получение списка томов                    */
#define PLOT_FMDFSLL  31 /* чтение оглавлений томов                   */
#define PLOT_FMDFSLM  32 /* создание каталогов                        */
#define PLOT_FMDFSFL  33 /* чтение оглавлений каталогов               */
#define PLOT_FMDFSLN  34 /* переименование (без перемещения) каталогов*/
#define PLOT_FMDFSLD  35 /* удаление каталогов                        */
#define PLOT_FMDFSFM  36 /* создание файлов                           */
#define PLOT_FMDFSFR  37 /* чтение файлов                             */
#define PLOT_FMDFSFW  38 /* модификация файлов                        */
#define PLOT_FMDFSFN  39 /* переименование (без перемещения) файлов   */
#define PLOT_FMDFSFD  40 /* удаление файлов                           */
#define PLOT_FMDFSRUN 41 /* инициирование исполнимого модуля          */
#define PLOT_FMDFSGCH 42 /* изменение грифа файла (каталога)          */
#define PLOT_FMDFSECP 43 /* получение свойств элемента оглавления     */
/*                                                                    */
/* Максимально допустимое значение кода операции                      */
#define PLOT_FMDLAST  PLOT_FMDFSECP
/*                                                                    */
/* ШКАЛА РЕГИСТРИРУЕМЫХ СОБЫТИЙ                                       */
#define PLOT_FMDSUSOK 1  /* Идентификация пользователя - успех        */
#define PLOT_FMDSUSNO 2  /* Идентификация пользователя - отказ        */
#define PLOT_FMDSTPUS 3  /* Окончание работы пользователя             */
/* Регистрация операций над документами баз данных ЛМ                 */
#define PLOT_FMDCRTOK 4  /* Создание - успех                          */
#define PLOT_FMDCRTNO 5  /* Создание - отказ                          */
#define PLOT_FMDCHKOK 6  /* Изменение грифа - успех                   */
#define PLOT_FMDCHKNO 7  /* Изменение грифа - отказ                   */
#define PLOT_FMDCHTOK 8  /* Изменение типа - успех                    */
#define PLOT_FMDCHTNO 9  /* Изменение типа - отказ                    */
#define PLOT_FMDVISOK 10 /* Визирование - успех                       */
#define PLOT_FMDVISNO 11 /* Визирование - отказ                       */
#define PLOT_FMDCLVOK 12 /* Снятие визы - успех                       */
#define PLOT_FMDCLVNO 13 /* Снятие визы - отказ                       */
#define PLOT_FMDCRDOK 14 /* Управление РД - успех                     */
#define PLOT_FMDCRDNO 15 /* Управление РД - отказ                     */
#define PLOT_FMDREDOK 16 /* Чтение - успех                            */
#define PLOT_FMDREDNO 17 /* Чтение - отказ                            */
#define PLOT_FMDWRTOK 18 /* Модификация - успех                       */
#define PLOT_FMDWRTNO 19 /* Модификация - отказ                       */
#define PLOT_FMDDELOK 20 /* Удаление - успех                          */
#define PLOT_FMDDELNO 21 /* Удаление - отказ                          */
/* Регистрация отправки информации                                    */
#define PLOT_FMDSNDOK 22 /* Отправка информации - успех               */
#define PLOT_FMDSNDNO 23 /* Отправка информации - отказ               */
/* Регистрация доступа к внешним устройствам                          */
#define PLOT_FMDUNLOK 24 /* Выгрузка информации - успех               */
#define PLOT_FMDUNLNO 25 /* Выгрузка информации - отказ               */
#define PLOT_FMDLODOK 26 /* Загрузка информации - успех               */
#define PLOT_FMDLODNO 27 /* Загрузка информации - отказ               */
#define PLOT_FMDPRTOK 28 /* Печать информации - успех                 */
#define PLOT_FMDPRTNO 29 /* Печать информации - отказ                 */
/* Регистрация управления вычислительным процессом                    */
#define PLOT_FMDSTTOK 30 /* Запуск задач - успех                      */
#define PLOT_FMDSTTNO 31 /* Запуск задач - отказ                      */
#define PLOT_FMDSTPOK 32 /* Завершение задач - успех                  */
#define PLOT_FMDSTPNO 33 /* Завершение задач - отказ                  */
#define PLOT_FMDRJROK 34 /* Просмотр журналов ЛМ - успех              */
#define PLOT_FMDRJRNO 35 /* Просмотр журналов ЛМ - отказ              */
#define PLOT_FMDRJUOK 36 /* Просмотр журналов ВУ - успех              */
#define PLOT_FMDRJUNO 37 /* Просмотр журналов ВУ - отказ              */
#define PLOT_FMDMEMOK 38 /* Операции над памятью ЛМ - успех           */
#define PLOT_FMDMEMNO 39 /* Операции над памятью ЛМ - отказ           */
#define PLOT_FMDGRDOK 40 /* Управление гарант. доведением - успех     */
#define PLOT_FMDGRDNO 41 /* Управление гарант. доведением - отказ     */
#define PLOT_FMDIPPOK 42 /* Доступ к ip-портам - успех                */
#define PLOT_FMDIPPNO 43 /* Доступ к ip-портам - отказ                */
/* Регистрация операций над файлами (томами, каталлогами)             */
/* операционной системы                                               */
#define PLOT_FMDFVLOK 44 /* получение списка доступных томов - успех  */
#define PLOT_FMDFVLNO 45 /* получение списка доступных томов - отказ  */
#define PLOT_FMDFLLOK 46 /* чтение оглавлений томов - успех           */
#define PLOT_FMDFLLNO 47 /* чтение оглавлений томов - отказ           */
#define PLOT_FMDFLMOK 48 /* создание каталогов - успех                */
#define PLOT_FMDFLMNO 49 /* создание каталогов - отказ                */
#define PLOT_FMDFFLOK 50 /* чтение оглавлений каталогов - успех       */
#define PLOT_FMDFFLNO 51 /* чтение оглавлений каталогов - отказ       */
#define PLOT_FMDFLNOK 52 /* переим-е(без перемещ-я)каталогов - успех  */
#define PLOT_FMDFLNNO 53 /* переим-е(без перемещ-я)каталогов - отказ  */
#define PLOT_FMDFLDOK 54 /* удаление каталогов - успех                */
#define PLOT_FMDFLDNO 55 /* удаление каталогов - отказ                */
#define PLOT_FMDFFMOK 56 /* создание файлов - успех                   */
#define PLOT_FMDFFMNO 57 /* создание файлов - отказ                   */
#define PLOT_FMDFFROK 58 /* чтение файлов - успех                     */
#define PLOT_FMDFFRNO 59 /* чтение файлов - отказ                     */
#define PLOT_FMDFFWOK 60 /* модификация файлов - успех                */
#define PLOT_FMDFFWNO 61 /* модификация файлов - отказ                */
#define PLOT_FMDFFNOK 62 /* переим-е (без перемещ-я) файлов - успех   */
#define PLOT_FMDFFNNO 63 /* переим-е (без перемещ-я) файлов - отказ   */
#define PLOT_FMDFFDOK 64 /* удаление файлов - успех                   */
#define PLOT_FMDFFDNO 65 /* удаление файлов - отказ                   */
#define PLOT_FMDFRNOK 66 /* иницииров-е исполнимого модуля - успех    */
#define PLOT_FMDFRNNO 67 /* иницииров-е исполнимого модуля - отказ    */
#define PLOT_FMDFGCOK 68 /* изменение грифа файла (каталога) - успех  */
#define PLOT_FMDFGCNO 69 /* изменение грифа файла (каталога) - отказ  */
#define PLOT_FMDFEPOK 70 /* получение свойств эл-та оглавления - успех*/
#define PLOT_FMDFEPNO 71 /* получение свойств эл-та оглавления - отказ*/
/*====================================================================*/
/*=================== Константы для FMD (конец) ======================*/
/*====================================================================*/

/*====================================================================*/
/*=================== Константы для ПСЗИ (начало) ====================*/
/*====================================================================*/
/* КОДЫ ОПЕРАЦИЙ УПРАВЛЕНИЯ ПСЗИ                                      */
#define PLOT_SZIUSRSTART   101 /* Запрос на начало работы             */
#define PLOT_SZIUSREND     102 /* Сообщение об окончании работы       */
#define PLOT_SZIDATACHANG  103 /* Запрос на замену учетных данных
                                  пользователя, рассылаемый главной
                                  машиной объекта после успешной
                                  идентификации                       */
#define PLOT_SZIMESSAGE    104 /* Сообщение о НСД                     */
#define PLOT_SZIUSERADD    105 /* Добавление нового пользователя      */
#define PLOT_SZIUSERDEL    106 /* Удаление пользователя               */
#define PLOT_SZICHNGPASS   107 /* Смена паролей для всех пользователей*/
#define PLOT_SZICHNGPASSUS 108 /* Смена паролей отдельного польз-ля   */
#define PLOT_SZITELLPASS   109 /* Выдача пароля пользователю          */
#define PLOT_SZIUSERCHANGE 110 /* Корректир-ка учетных данных польз-ля*/
#define PLOT_SZIRANGRD     111 /* Чтение значения ранга               */
#define PLOT_SZIRANGWR     112 /* Корректировка значения ранга        */
#define PLOT_SZIRANGMK     113 /* Создание нового ранга               */
#define PLOT_SZIRANGDL     114 /* Удаление ранга                      */
#define PLOT_SZITRASTDRD   115 /* Чтение перечня доверяемых модулей
                                  для заданного ЛМ                    */
#define PLOT_SZITRASTDWR   116 /* Корректировка перечня доверяемых
                                  модулей для заданного ЛМ            */
#define PLOT_SZITRASTDDL   117 /* Удаление перечня доверяемых модулей
                                  для заданного ЛМ                    */
#define PLOT_SZITRASTGRD   118 /* Чтение перечня доверяющих модулей
                                  для заданного ЛМ                    */
#define PLOT_SZITRASTGWR   119 /* Корректировка перечня доверяющих
                                  модулей для заданного ЛМ            */
#define PLOT_SZITRASTGDL   120 /* Удаление перечня доверяющих модулей
                                  для заданного ЛМ                    */
#define PLOT_SZIADDRRD     121 /* Чтение перечня разрешенных адресатов*/
#define PLOT_SZIADDRWR     122 /* Корректировка перечня разрешенных
                                  адресатов                           */
#define PLOT_SZIADDRDL     123 /* Удаление перечня разрешенных
                                  адресатов                           */
#define PLOT_SZIPTHURD     124 /* Чтение перечня ЛМ, допущенных к ката-
                                  логу загрузки (выгрузки)            */
#define PLOT_SZIPTHUWR     125 /* Корректировка перечня ЛМ, допущенных
                                  к каталогу загрузки (выгрузки)      */
#define PLOT_SZIPTHUDL     126 /* Удаление доступа для каталога загруз-
                                  ки (выгрузки) вместе с перечнем
                                  допущенных ЛМ                       */
#define PLOT_SZIPRINTRD    130 /* Чтение перечня ЛМ, допущенных к
                                  заданному устройству                */
#define PLOT_SZIPRINTWR    131 /* Корректировка перечня ЛМ, допущенных
                                  к заданному устройству              */
#define PLOT_SZIPRINTDL    132 /* Удаление заданного устройства
                                  (вместе с перечнем допущенных ЛМ)   */
#define PLOT_SZIPROPIOR    137 /* Чтение учетных данных (свойств)
                                  информационных объектов             */
#define PLOT_SZIGETMDM     141 /* Запрос на матрицу доступа логического
                                  модуля                              */
#define PLOT_SZICHNGDATLM  142 /* Запрос на замену (добавление) данных
                                  о ЛМ                                */
#define PLOT_SZIMAKECRC    143 /* Сформировать эталонную таблицу
                                  целостности ПО и ИО                 */
#define PLOT_SZITESTCRC    144 /* Проверить целостность ПО и ИО */
#define PLOT_SZICLEARCRC   145 /* Очистить журнал контроля целостности
                                  ПО и ИО                             */
#define PLOT_SZIREGSYST    146 /* Управление подсистемой регистрации и
                                  учета                               */
#define PLOT_SZITESTSYST   147 /* Контрольная задачи программных
                                  средств защиты информации           */
#define PLOT_SZILMRANGEWR  148 /* Присвоение ранга ЛМ по умолчанию    */
#define PLOT_SZILMRANGEDL  149 /* Удаление ранга ЛМ по умолчанию      */
#define PLOT_SZIPORTIPADD  150 /* Добавление ip-порта в таблицу
                                  разрешенных ip-портов               */
#define PLOT_SZIPORTIPDEL  151 /* Удаление ip-порта из таблицы
                                  разрешенных ip-портов               */
#define PLOT_SZIPORTLMADD  152 /* Разрешить доступ ЛМ к ip-порту      */
#define PLOT_SZIPORTLMDEL  153 /* Исключить доступ ЛМ к ip-порту      */
#define PLOT_SZIREPORTPASS 154 /* Генерация отчета с паролями         */
                               /* пользователей                       */
#define PLOT_SZIMASSPASS   155 /* Генерация массива из N              */
                               /* L-значных паролей                   */
/*                                                                    */
/*====================================================================*/
/*=================== Константы для ПСЗИ (конец) =====================*/
/*====================================================================*/

/*====================================================================*/
/*=================== Константы для MDM (начало) =====================*/
/*====================================================================*/
/*                                                                    */
/* МАТРИЦА ДОСТУПА ЛОГИЧЕСКОГО МОДУЛЯ                                 */
/*                                                                    */
/* Шапка                                                              */
#define PLOT_MDMLNGHT 0   /* длина матрицы                            */
/*                           - резерв (8)                             */
#define PLOT_MDMCHKSM 12  /* контрольная сумма                        */
#define PLOT_MDMRGDEF 16  /* ранг по умолчанию                        */
/*                           - резерв (2)                             */
#define PLOT_MDMLZGM  20  /* длина заголовка                          */
#define PLOT_MDMADMIN 24  /* код модуля администратора ОБИ (2)        */
#define PLOT_MDMSCUF  26  /* код модуля администратора СКУФ (2)       */
#define PLOT_MDMUSER  28  /* смещение таблицы пользователей           */
#define PLOT_MDMRANG  32  /* смещение таблицы рангов                  */
#define PLOT_MDMTRSTD 36  /* смещение таблицы доверяемых модулей      */
#define PLOT_MDMTRSTG 40  /* смещение таблицы доверяющих модулей      */
#define PLOT_MDMADDRS 44  /* смещение таблицы разрешенных адресатов   */
#define PLOT_MDMRMVBL 48  /* смещение таблицы устройств и каталогов   */
/*                           для выгрузки и загрузки информации       */
#define PLOT_MDMDPRNT 52  /* смещение таблицы устройств печати        */
#define PLOT_MDMACIPP 56  /* смещение таблицы разрешенных ip-портов   */
#define PLOT_MDMRDIPP 60  /* смещение таблицы РД к ip-портам          */
#define PLOT_MDMSHREG 64  /* смещение шкалы регистрируемых событий    */
/* Значения                                                           */
#define PLOT_MDMZLZGM 68  /* длина заголовка                          */
/*                                                                    */
/* Заголовок таблицы (общий для всех таблиц, входящих в матрицу)      */
#define PLOT_MDMLZGT  0   /* длина заголовка                          */
#define PLOT_MDMLSTRT 4   /* длина строки                             */
#define PLOT_MDMNSTRT 8   /* количество строк                         */
#define PLOT_MDMLFLD1 12  /* длина первого поля строки                */
#define PLOT_MDMLFLD2 16  /* длина второго поля строки                */
/* Значения                                                           */
#define PLOT_MDMZLZGT 20  /* длина заголовка                          */
/*                                                                    */
/* Таблицы пользователей и разрешенных адресатов                      */
/* Значения                                                           */
#define PLOT_MDMZUAF1 2   /* длина кода объекта                       */
#define PLOT_MDMZUAF2 2   /* длина кода модуля                        */
#define PLOT_MDMZUSRM 600 /* максимальное количество пользователей    */
/*                                                                    */
/* Таблица рангов полномочий пользователей                            */
/* Значения                                                           */
#define PLOT_MDMZLNRG 2   /* длина номера ранга                       */
#define PLOT_MDMZLZRG 34  /* длина значения ранга (битовой шкалы)     */
/*                           (байт)                                   */
/* Таблицы доверяемых и доверяющих модулей                            */
/* Значения                                                           */
#define PLOT_MDMZLBCP 1   /* длина признака (операции с БД/           */
/*                           упр-е вычислительным процессом/          */
/*                           операции с файлами)                      */
#define PLOT_MDMZLTRM 75  /* длина перечня модулей (битовой шкалы)    */
/*                           (байт)                                   */
/* Таблица устройств и каталогов для выгрузки и загрузки информации   */
/* Таблица устройств печати                                           */
/* Значения                                                           */
#define PLOT_MDMZLIOP 1   /* длина признака (загрузка/выгузка) в      */
/*                           таблице устройств и каталогов для        */
/*                           выгрузки и загрузки информации           */
#define PLOT_MDMZLPUN 256 /* длина имени каталога (устройства) или    */
/*                           длина имени устройства печати            */
/*                                                                    */
/* Таблица ip-портов, разрешенных для использования                   */
/* Значения                                                           */
#define PLOT_MDMZACPN 2   /* длина номера ip-порта                    */
/*                                                                    */
/* Таблица РД к ip-портам                                             */
/* Значения                                                           */
#define PLOT_MDMZRDPM PLOT_MDMZUAF2 /* длина кода модуля              */
/*                                                                    */
/* Шкала регистрируемых событий                                       */
#define PLOT_MDMZLREG 9   /* длина битовой шкалы регистрируемых       */
/*                           событий (байт)                           */
/* Признаки обрабатываемых таблиц                                     */
#define PLOT_MDMPUSRT 1   /* таблица пользователей                    */
#define PLOT_MDMPRANG 2   /* таблица рангов                           */
#define PLOT_MDMPTRSD 3   /* таблица доверяемых модулей               */
#define PLOT_MDMPTRSG 4   /* таблица доверяющих модулей               */
#define PLOT_MDMPADDR 5   /* таблица разрешенных адресатов            */
#define PLOT_MDMPIO   6   /* таблица устройств и каталогов            */
/*                           для выгрузки и загрузки информации       */
#define PLOT_MDMPPRT  7   /* таблица устройств печати                 */
#define PLOT_MDMPAIPP 8   /* таблица разрешенных ip-портов            */
#define PLOT_MDMPDIPP 9   /* таблица РД к ip-портам                   */
#define PLOT_MDMPREG  10  /* таблица регистрируемых событий           */
/*====================================================================*/
/*=================== Константы для MDM (конец) ======================*/
/*====================================================================*/


/*====================================================================*/
/*=================== Константы для NZR (начало) =====================*/
/*====================================================================*/
/* ОПРЕДЕЛЕНИЕ НОМЕРОВ ЗАЩИЩАЕМЫХ РЕСУРСОВ                            */
/*                                                                    */
/* ДОСТУП К ВЫЧИСЛИТЕЛЬНОЙ УСТАНОВКЕ И ЛОГИЧЕСКОМУ МОДУЛЮ             */
#define PLOT_NZRALLVU 1   /* доступ ко всем ВУ без доп. проверки РД   */
#define PLOT_NZRALLLM 2   /* доступ ко всем ЛМ без доп. проверки РД   */
/*                                                                    */
/* ДОСТУП К ДОКУМЕНТАМ БАЗ ДАННЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ                  */
/* Доступ к документам базы данных СВОЕГО ЛОГИЧЕСКОГО МОДУЛЯ          */
#define PLOT_NZRDMYCK 3   /* изменение грифа                          */
#define PLOT_NZRDMYCA 4   /* изменение типа                           */
#define PLOT_NZRDMYSV 5   /* визирование                              */
#define PLOT_NZRDMYRV 6   /* снятие визы                              */
/* Управление разграничением доступа к документам                     */
#define PLOT_NZRDMYC0 7   /* управление РД к несекретным документам   */
#define PLOT_NZRDMYC1 8   /* управление РД к документам ДСП           */
#define PLOT_NZRDMYC2 9   /* управление РД к секретным документам     */
#define PLOT_NZRDMYC3 10  /* управление РД к сов. секретным документам*/
/* Создание документов                                                */
#define PLOT_NZRDMYM0 11  /* создание несекретных документов          */
#define PLOT_NZRDMYM1 12  /* создание документов ДСП                  */
#define PLOT_NZRDMYM2 13  /* создание секретных документов            */
#define PLOT_NZRDMYM3 14  /* создание сов. секретных документов       */
/* Чтение документов                                                  */
#define PLOT_NZRDMYR0 15  /* чтение несекретных документов            */
#define PLOT_NZRDMYR1 16  /* чтение документов ДСП                    */
#define PLOT_NZRDMYR2 17  /* чтение секретных документов              */
#define PLOT_NZRDMYR3 18  /* чтение сов. секретных документов         */
/* Изменение документов                                               */
#define PLOT_NZRDMYW0 19  /* модификация несекретных документов       */
#define PLOT_NZRDMYW1 20  /* модификация документов ДСП               */
#define PLOT_NZRDMYW2 21  /* модификация секретных документов         */
#define PLOT_NZRDMYW3 22  /* модификация сов. секретных документов    */
/* Удаление документов                                                */
#define PLOT_NZRDMYD0 23  /* удаление несекретных документов          */
#define PLOT_NZRDMYD1 24  /* удаление документов ДСП                  */
#define PLOT_NZRDMYD2 25  /* удаление секретных документов            */
#define PLOT_NZRDMYD3 26  /* удаление сов. секретных документов       */
/* Выполнение операций управления РД, чтения, модификации и           */
/* удаления документов базы данных СВОЕГО ЛОГИЧЕСКОГО МОДУЛЯ без      */
/* доп. проверки РД дискреционным методом                             */
#define PLOT_NZRDMYFA 27
/*                                                                    */
/* Доступ к документам базы данных ДРУГОГО ЛОГИЧЕСКОГО МОДУЛЯ         */
#define PLOT_NZRDOTCK 28  /* изменение грифа                          */
#define PLOT_NZRDOTCA 29  /* изменение типа                           */
#define PLOT_NZRDOTSV 30  /* визирование                              */
#define PLOT_NZRDOTRV 31  /* снятие визы                              */
/* Управление разграничением доступа к документам                     */
#define PLOT_NZRDOTC0 32  /* управление РД к несекретным документам   */
#define PLOT_NZRDOTC1 33  /* управление РД к документам ДСП           */
#define PLOT_NZRDOTC2 34  /* управление РД к секретным документам     */
#define PLOT_NZRDOTC3 35  /* управление РД к сов. секретным документам*/
/* Создание документов                                                */
#define PLOT_NZRDOTM0 36  /* создание несекретных документов          */
#define PLOT_NZRDOTM1 37  /* создание документов ДСП                  */
#define PLOT_NZRDOTM2 38  /* создание секретных документов            */
#define PLOT_NZRDOTM3 39  /* создание сов. секретных документов       */
/* Чтение документов                                                  */
#define PLOT_NZRDOTR0 40  /* чтение несекретных документов            */
#define PLOT_NZRDOTR1 41  /* чтение документов ДСП                    */
#define PLOT_NZRDOTR2 42  /* чтение секретных документов              */
#define PLOT_NZRDOTR3 43  /* чтение сов. секретных документов         */
/* Изменение документов                                               */
#define PLOT_NZRDOTW0 44  /* модификация несекретных документов       */
#define PLOT_NZRDOTW1 45  /* модификация документов ДСП               */
#define PLOT_NZRDOTW2 46  /* модификация секретных документов         */
#define PLOT_NZRDOTW3 47  /* модификация сов. секретных документов    */
/* Удаление документов                                                */
#define PLOT_NZRDOTD0 48  /* удаление несекретных документов          */
#define PLOT_NZRDOTD1 49  /* удаление документов ДСП                  */
#define PLOT_NZRDOTD2 50  /* удаление секретных документов            */
#define PLOT_NZRDOTD3 51  /* удаление сов. секретных документов       */
/* Выполнение операций управления РД, чтения, модификации и           */
/* удаления документов базы данных ДРУГОГО ЛОГИЧЕСКОГО МОДУЛЯ без     */
/* доп. проверки РД дискреционным методом                             */
#define PLOT_NZRDOTFA 52
/*                                                                    */
/* Доступ к документам баз данных ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ СВОЕЙ ЛВС  */
#define PLOT_NZRDNTCK 53  /* изменение грифа                          */
#define PLOT_NZRDNTCA 54  /* изменение типа                           */
#define PLOT_NZRDNTSV 55  /* визирование                              */
#define PLOT_NZRDNTRV 56  /* снятие визы                              */
/* Управление разграничением доступа к документам                     */
#define PLOT_NZRDNTC0 57  /* управление РД к несекретным документам   */
#define PLOT_NZRDNTC1 58  /* управление РД к документам ДСП           */
#define PLOT_NZRDNTC2 59  /* управление РД к секретным документам     */
#define PLOT_NZRDNTC3 60  /* управление РД к сов. секретным документам*/
/* Создание документов                                                */
#define PLOT_NZRDNTM0 61  /* создание несекретных документов          */
#define PLOT_NZRDNTM1 62  /* создание документов ДСП                  */
#define PLOT_NZRDNTM2 63  /* создание секретных документов            */
#define PLOT_NZRDNTM3 64  /* создание сов. секретных документов       */
/* Чтение документов                                                  */
#define PLOT_NZRDNTR0 65  /* чтение несекретных документов            */
#define PLOT_NZRDNTR1 66  /* чтение документов ДСП                    */
#define PLOT_NZRDNTR2 67  /* чтение секретных документов              */
#define PLOT_NZRDNTR3 68  /* чтение сов. секретных документов         */
/* Изменение документов                                               */
#define PLOT_NZRDNTW0 69  /* модификация несекретных документов       */
#define PLOT_NZRDNTW1 70  /* модификация документов ДСП               */
#define PLOT_NZRDNTW2 71  /* модификация секретных документов         */
#define PLOT_NZRDNTW3 72  /* модификация сов. секретных документов    */
/* Удаление документов                                                */
#define PLOT_NZRDNTD0 73  /* удаление несекретных документов          */
#define PLOT_NZRDNTD1 74  /* удаление документов ДСП                  */
#define PLOT_NZRDNTD2 75  /* удаление секретных документов            */
#define PLOT_NZRDNTD3 76  /* удаление сов. секретных документов       */
/* Выполнение операций управления РД, чтения, модификации и           */
/* удаления документов баз данных ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ СВОЕЙ      */
/* ЛВС без доп. проверки РД дискреционным методом                     */
#define PLOT_NZRDNTFA 77
/*                                                                    */
/* Доступ к документам баз данных ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ ОБЪЕКТА    */
#define PLOT_NZRDOBCK 78  /* изменение грифа                          */
#define PLOT_NZRDOBCA 79  /* изменение типа                           */
#define PLOT_NZRDOBSV 80  /* визирование                              */
#define PLOT_NZRDOBRV 81  /* снятие визы                              */
/* Управление разграничением доступа к документам                     */
#define PLOT_NZRDOBC0 82  /* управление РД к несекретным документам   */
#define PLOT_NZRDOBC1 83  /* управление РД к документам ДСП           */
#define PLOT_NZRDOBC2 84  /* управление РД к секретным документам     */
#define PLOT_NZRDOBC3 85  /* управление РД к сов. секретным документам*/
/* Создание документов                                                */
#define PLOT_NZRDOBM0 86  /* создание несекретных документов          */
#define PLOT_NZRDOBM1 87  /* создание документов ДСП                  */
#define PLOT_NZRDOBM2 88  /* создание секретных документов            */
#define PLOT_NZRDOBM3 89  /* создание сов. секретных документов       */
/* Чтение документов                                                  */
#define PLOT_NZRDOBR0 90  /* чтение несекретных документов            */
#define PLOT_NZRDOBR1 91  /* чтение документов ДСП                    */
#define PLOT_NZRDOBR2 92  /* чтение секретных документов              */
#define PLOT_NZRDOBR3 93  /* чтение сов. секретных документов         */
/* Изменение документов                                               */
#define PLOT_NZRDOBW0 94  /* модификация несекретных документов       */
#define PLOT_NZRDOBW1 95  /* модификация документов ДСП               */
#define PLOT_NZRDOBW2 96  /* модификация секретных документов         */
#define PLOT_NZRDOBW3 97  /* модификация сов. секретных документов    */
/* Удаление документов                                                */
#define PLOT_NZRDOBD0 98  /* удаление несекретных документов          */
#define PLOT_NZRDOBD1 99  /* удаление документов ДСП                  */
#define PLOT_NZRDOBD2 100 /* удаление секретных документов            */
#define PLOT_NZRDOBD3 101 /* удаление сов. секретных документов       */
/* Выполнение операций управления РД, чтения, модификации и           */
/* удаления документов баз данных ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ ОБЪЕКТА    */
/* без доп. проверки РД дискреционным методом                         */
#define PLOT_NZRDOBFA 102
/*                                                                    */
/* ОТПРАВКА ИНФОРМАЦИИ (ДОСТУП К КАНАЛАМ СВЯЗИ)                       */
/* Отправка информации НА ДРУГИЕ ОБЪЕКТЫ                              */
#define PLOT_NZRSEXTS 103 /* сообщений на другие объекты              */
#define PLOT_NZRSEXTD 104 /* документов на другие объекты             */
#define PLOT_NZRSEXTR 105 /* инф-ции в любой адрес (на другие объекты)*/
#define PLOT_NZRSEXT0 106 /* несекретной информации на другие объекты */
#define PLOT_NZRSEXT1 107 /* информации ДСП на другие объекты         */
#define PLOT_NZRSEXT2 108 /* секретной информации на другие объекты   */
#define PLOT_NZRSEXT3 109 /* сов.секретной инф-ции на другие объекты  */
#define PLOT_NZRSEXTA 110 /* инф. кат. срочности "A" на др. объекты   */
#define PLOT_NZRSEXTB 111 /* инф. кат. срочности "B" на др. объекты   */
#define PLOT_NZRSEXTC 112 /* инф. кат. срочности "C" на др. объекты   */
/* Отправка информации АДРЕСАТАМ СВОЕГО ОБЪЕКТА                       */
#define PLOT_NZRSOBS  113 /* сообщений адресатам объекта              */
#define PLOT_NZRSOBD  114 /* документов адресатам объекта             */
#define PLOT_NZRSOBA  115 /* информации в любой адрес (внутри объекта)*/
#define PLOT_NZRSOB0  116 /* несекретной информации адресатам объекта */
#define PLOT_NZRSOB1  117 /* информации ДСП адресатам объекта         */
#define PLOT_NZRSOB2  118 /* секретной информации адресатам объекта   */
#define PLOT_NZRSOB3  119 /* сов.секретной инф-ции адресатам объекта  */
/* Отправка информации АДРЕСАТАМ СВОЕЙ ЛВС                            */
#define PLOT_NZRSNTS  120 /* сообщений адресатам своей ЛВС            */
#define PLOT_NZRSNTD  121 /* документов адресатам своей ЛВС           */
#define PLOT_NZRSNTA  122 /* инф-ции в любой адрес (внутри своей ЛВС) */
#define PLOT_NZRSNT0  123 /* несекретной инф-ции адресатам своей ЛВС  */
#define PLOT_NZRSNT1  124 /* информации ДСП адресатам своей ЛВС       */
#define PLOT_NZRSNT2  125 /* секретной информации адресатам своей ЛВС */
#define PLOT_NZRSNT3  126 /* сов.секретной инф-ции адресатам своей ЛВС*/
/*                                                                    */
/* ДОСТУП К ОТЧУЖДАЕМЫМ НОСИТЕЛЯМ                                     */
/* Выгрузка информации                                                */
#define PLOT_NZRUNLD0 127 /* выгрузка несекретной информации          */
#define PLOT_NZRUNLD1 128 /* выгрузка информации ДСП                  */
#define PLOT_NZRUNLD2 129 /* выгрузка секретной информации            */
#define PLOT_NZRUNLD3 130 /* выгрузка сов.секретной информации        */
/* Загрузка информации                                                */
#define PLOT_NZRLOAD0 131 /* загрузка несекретной информации          */
#define PLOT_NZRLOAD1 132 /* загрузка информации ДСП                  */
#define PLOT_NZRLOAD2 133 /* загрузка секретной информации            */
#define PLOT_NZRLOAD3 134 /* загрузка сов.секретной информации        */
/*                                                                    */
/* ДОСТУП К СРЕДСТВАМ ПЕЧАТИ                                          */
#define PLOT_NZRPRT0  135 /* печать несекретной информации            */
#define PLOT_NZRPRT1  136 /* печать информации ДСП                    */
#define PLOT_NZRPRT2  137 /* печать секретной информации              */
#define PLOT_NZRPRT3  138 /* печать сов. секретной информации         */
/*                                                                    */
/* УПРАВЛЕНИЕ ВЫЧИСЛИТЕЛЬНЫМ ПРОЦЕССОМ НА ЛОГИЧЕСКИХ МОДУЛЯХ          */
#define PLOT_NZRJRVU  139 /* просмотр журналов ВУ                     */
/* Управление вычислительным процессом НА СВОЕМ ЛОГИЧЕСКОМ МОДУЛЕ     */
#define PLOT_NZRCMTS  140 /* запуск задач                             */
#define PLOT_NZRCMTP  141 /* завершение задач                         */
#define PLOT_NZRCMSJR 142 /* просмотр журналов ЛМ                     */
#define PLOT_NZRCMMEM 143 /* операции над памятью ЛМ                  */
#define PLOT_NZRCMGRD 144 /* управление гарантированным доведением    */
#define PLOT_NZRCMIPP 145 /* доступ к ip-портам                       */
/* Выполнение операций запуска/завершения задач, доступ к ip-портам   */
/* НА СВОЕМ ЛОГИЧЕСКОМ МОДУЛЕ без доп. проверки РД                    */
/* дискреционным методом                                              */
#define PLOT_NZRCMFA  146
/*                                                                    */
/* Управление вычислительным процессом НА ДРУГОМ ЛОГИЧЕСКОМ МОДУЛЕ    */
#define PLOT_NZRCLTS  147 /* запуск задач                             */
#define PLOT_NZRCLTP  148 /* завершение задач                         */
#define PLOT_NZRCLSJR 149 /* просмотр журналов ЛМ                     */
#define PLOT_NZRCLMEM 150 /* операции над памятью ЛМ                  */
#define PLOT_NZRCLGRD 151 /* управление гарантированным доведением    */
#define PLOT_NZRCLIPP 152 /* доступ к ip-портам                       */
/* Выполнение операций запуска/завершения задач, доступ к ip-портам   */
/* НА ДРУГОМ ЛОГИЧЕСКОМ МОДУЛЕ без доп. проверки РД                   */
/* дискреционным методом                                              */
#define PLOT_NZRCLFA  153
/*                                                                    */
/* Управление вычислительным процессом                                */
/* НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ СВОЕЙ ЛВС                              */
#define PLOT_NZRCNTS  154 /* запуск задач                             */
#define PLOT_NZRCNTP  155 /* завершение задач                         */
#define PLOT_NZRCNSJR 156 /* просмотр журналов ЛМ                     */
#define PLOT_NZRCNMEM 157 /* операции над памятью ЛМ                  */
#define PLOT_NZRCNGRD 158 /* управление гарантированным доведением    */
#define PLOT_NZRCNIPP 159 /* доступ к ip-портам                       */
/* Выполнение операций запуска/завершения задач, доступ к ip-портам   */
/* НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ СВОЕЙ ЛВС без доп. проверки РД         */
/* дискреционным методом                                              */
#define PLOT_NZRCNFA  160
/*                                                                    */
/* Управление вычислительным процессом                                */
/* НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ ОБЪЕКТА                                */
#define PLOT_NZRCOTS  161 /* запуск задач                             */
#define PLOT_NZRCOTP  162 /* завершение задач                         */
#define PLOT_NZRCOSJR 163 /* просмотр журналов ЛМ                     */
#define PLOT_NZRCOMEM 164 /* операции над памятью ЛМ                  */
#define PLOT_NZRCOGRD 165 /* управление гарантированным доведением    */
#define PLOT_NZRCOIPP 166 /* доступ к ip-портам                       */
/* Выполнение операций запуска/завершения задач, доступ к ip-портам   */
/* НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ ОБЪЕКТА без доп. проверки РД           */
/* дискреционным методом                                              */
#define PLOT_NZRCOFA  167
/* ДОСТУП К ФАЙЛАМ (ТОМАМ, КАТАЛОГАМ) ОПЕРАЦИОННОЙ СИСТЕМЫ            */
/* Доступ к файлам (томам, каталогам) СВОЕГО ЛОГИЧЕСКОГО МОДУЛЯ       */
#define PLOT_NZRFMVL  168 /* получение списка доступных томов         */
#define PLOT_NZRFMLL  169 /* чтение оглавлений томов                  */
#define PLOT_NZRFMLM  170 /* создание каталогов                       */
#define PLOT_NZRFMFL  171 /* чтение оглавлений каталогов              */
#define PLOT_NZRFMLN  172 /* переименование(без перемещения) каталогов*/
#define PLOT_NZRFMLD  173 /* удаление каталогов                       */
#define PLOT_NZRFMFN  174 /* переименование (без перемещения) файлов  */
#define PLOT_NZRFMGC  175 /* изменение грифа файла (каталога)         */
#define PLOT_NZRFMRUN 176 /* инициирование исполнимого модуля         */
/*                                                                    */
#define PLOT_NZRFMFM0 177 /* создание несекретных файлов              */
#define PLOT_NZRFMFM1 178 /* создание файлов ДСП                      */
#define PLOT_NZRFMFM2 179 /* создание секретных файлов                */
#define PLOT_NZRFMFM3 180 /* создание сов. секретных файлов           */
/*                                                                    */
#define PLOT_NZRFMFR0 181 /* чтение несекретных файлов                */
#define PLOT_NZRFMFR1 182 /* чтение файлов ДСП                        */
#define PLOT_NZRFMFR2 183 /* чтение секретных файлов                  */
#define PLOT_NZRFMFR3 184 /* чтение сов. секретных файлов             */
/*                                                                    */
#define PLOT_NZRFMFW0 185 /* модификация несекретных файлов           */
#define PLOT_NZRFMFW1 186 /* модификация файлов ДСП                   */
#define PLOT_NZRFMFW2 187 /* модификация секретных файлов             */
#define PLOT_NZRFMFW3 188 /* модификация сов. секретных файлов        */
/*                                                                    */
#define PLOT_NZRFMFD0 189 /* удаление несекретных файлов              */
#define PLOT_NZRFMFD1 190 /* удаление файлов ДСП                      */
#define PLOT_NZRFMFD2 191 /* удаление секретных файлов                */
#define PLOT_NZRFMFD3 192 /* удаление сов. секретных файлов           */
/*                                                                    */
/* Выполнение операций чтения, модификации и удаления над файлами     */
/* (каталогами) НА СВОЕМ ЛОГИЧЕСКОМ МОДУЛЕ без доп.                   */
/* проверки РД дискреционным методом                                  */
#define PLOT_NZRFMFA  193
/*                                                                    */
/* Доступ к файлам (томам, каталогам) ДРУГОГО ЛОГИЧЕСКОГО МОДУЛЯ      */
#define PLOT_NZRFLVL  194 /* получение списка доступных томов         */
#define PLOT_NZRFLLL  195 /* чтение оглавлений томов                  */
#define PLOT_NZRFLLM  196 /* создание каталогов                       */
#define PLOT_NZRFLFL  197 /* чтение оглавлений каталогов              */
#define PLOT_NZRFLLN  198 /* переименование(без перемещения) каталогов*/
#define PLOT_NZRFLLD  199 /* удаление каталогов                       */
#define PLOT_NZRFLFN  200 /* переименование (без перемещения) файлов  */
#define PLOT_NZRFLGC  201 /* изменение грифа файла (каталога)         */
#define PLOT_NZRFLRUN 202 /* инициирование исполнимого модуля         */
/*                                                                    */
#define PLOT_NZRFLFM0 203 /* создание несекретных файлов              */
#define PLOT_NZRFLFM1 204 /* создание файлов ДСП                      */
#define PLOT_NZRFLFM2 205 /* создание секретных файлов                */
#define PLOT_NZRFLFM3 206 /* создание сов. секретных файлов           */
/*                                                                    */
#define PLOT_NZRFLFR0 207 /* чтение несекретных файлов                */
#define PLOT_NZRFLFR1 208 /* чтение файлов ДСП                        */
#define PLOT_NZRFLFR2 209 /* чтение секретных файлов                  */
#define PLOT_NZRFLFR3 210 /* чтение сов. секретных файлов             */
/*                                                                    */
#define PLOT_NZRFLFW0 211 /* модификация несекретных файлов           */
#define PLOT_NZRFLFW1 212 /* модификация файлов ДСП                   */
#define PLOT_NZRFLFW2 213 /* модификация секретных файлов             */
#define PLOT_NZRFLFW3 214 /* модификация сов. секретных файлов        */
/*                                                                    */
#define PLOT_NZRFLFD0 215 /* удаление несекретных файлов              */
#define PLOT_NZRFLFD1 216 /* удаление файлов ДСП                      */
#define PLOT_NZRFLFD2 217 /* удаление секретных файлов                */
#define PLOT_NZRFLFD3 218 /* удаление сов. секретных файлов           */
/*                                                                    */
/* Выполнение операций чтения, модификации и удаления над файлами     */
/* (каталогами) НА ДРУГОМ ЛОГИЧЕСКОМ МОДУЛЕ без доп.                  */
/* проверки РД дискреционным методом                                  */
#define PLOT_NZRFLFA  219
/*                                                                    */
/* Доступ к файлам (томам, каталогам)                                 */
/* ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ СВОЕЙ ЛВС                                 */
#define PLOT_NZRFNVL  220 /* получение списка доступных томов         */
#define PLOT_NZRFNLL  221 /* чтение оглавлений томов                  */
#define PLOT_NZRFNLM  222 /* создание каталогов                       */
#define PLOT_NZRFNFL  223 /* чтение оглавлений каталогов              */
#define PLOT_NZRFNLN  224 /* переименование(без перемещения) каталогов*/
#define PLOT_NZRFNLD  225 /* удаление каталогов                       */
#define PLOT_NZRFNFN  226 /* переименование (без перемещения) файлов  */
#define PLOT_NZRFNGC  227 /* изменение грифа файла (каталога)         */
#define PLOT_NZRFNRUN 228 /* инициирование исполнимого модуля         */
/*                                                                    */
#define PLOT_NZRFNFM0 229 /* создание несекретных файлов              */
#define PLOT_NZRFNFM1 230 /* создание файлов ДСП                      */
#define PLOT_NZRFNFM2 231 /* создание секретных файлов                */
#define PLOT_NZRFNFM3 232 /* создание сов. секретных файлов           */
/*                                                                    */
#define PLOT_NZRFNFR0 233 /* чтение несекретных файлов                */
#define PLOT_NZRFNFR1 234 /* чтение файлов ДСП                        */
#define PLOT_NZRFNFR2 235 /* чтение секретных файлов                  */
#define PLOT_NZRFNFR3 236 /* чтение сов. секретных файлов             */
/*                                                                    */
#define PLOT_NZRFNFW0 237 /* модификация несекретных файлов           */
#define PLOT_NZRFNFW1 238 /* модификация файлов ДСП                   */
#define PLOT_NZRFNFW2 239 /* модификация секретных файлов             */
#define PLOT_NZRFNFW3 240 /* модификация сов. секретных файлов        */
/*                                                                    */
#define PLOT_NZRFNFD0 241 /* удаление несекретных файлов              */
#define PLOT_NZRFNFD1 242 /* удаление файлов ДСП                      */
#define PLOT_NZRFNFD2 243 /* удаление секретных файлов                */
#define PLOT_NZRFNFD3 244 /* удаление сов. секретных файлов           */
/*                                                                    */
/* Выполнение операций чтения, модификации и удаления над файлами     */
/* (каталогами) НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ СВОЕЙ ЛВС без             */
/* доп. проверки РД дискреционным методом                             */
#define PLOT_NZRFNFA  245
/*                                                                    */
/* Доступ к файлам (томам, каталогам) ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЕЙ ОБЪЕКТА*/
#define PLOT_NZRFOVL  246 /* получение списка доступных томов         */
#define PLOT_NZRFOLL  247 /* чтение оглавлений томов                  */
#define PLOT_NZRFOLM  248 /* создание каталогов                       */
#define PLOT_NZRFOFL  249 /* чтение оглавлений каталогов              */
#define PLOT_NZRFOLN  250 /* переименование(без перемещения) каталогов*/
#define PLOT_NZRFOLD  251 /* удаление каталогов                       */
#define PLOT_NZRFOFN  252 /* переименование (без перемещения) файлов  */
#define PLOT_NZRFOGC  253 /* изменение грифа файла (каталога)         */
#define PLOT_NZRFORUN 254 /* инициирование исполнимого модуля         */
/*                                                                    */
#define PLOT_NZRFOFM0 255 /* создание несекретных файлов              */
#define PLOT_NZRFOFM1 256 /* создание файлов ДСП                      */
#define PLOT_NZRFOFM2 257 /* создание секретных файлов                */
#define PLOT_NZRFOFM3 258 /* создание сов. секретных файлов           */
/*                                                                    */
#define PLOT_NZRFOFR0 259 /* чтение несекретных файлов                */
#define PLOT_NZRFOFR1 260 /* чтение файлов ДСП                        */
#define PLOT_NZRFOFR2 261 /* чтение секретных файлов                  */
#define PLOT_NZRFOFR3 262 /* чтение сов. секретных файлов             */
/*                                                                    */
#define PLOT_NZRFOFW0 263 /* модификация несекретных файлов           */
#define PLOT_NZRFOFW1 264 /* модификация файлов ДСП                   */
#define PLOT_NZRFOFW2 265 /* модификация секретных файлов             */
#define PLOT_NZRFOFW3 266 /* модификация сов. секретных файлов        */
/*                                                                    */
#define PLOT_NZRFOFD0 267 /* удаление несекретных файлов              */
#define PLOT_NZRFOFD1 268 /* удаление файлов ДСП                      */
#define PLOT_NZRFOFD2 269 /* удаление секретных файлов                */
#define PLOT_NZRFOFD3 270 /* удаление сов. секретных файлов           */
/*                                                                    */
/* Выполнение операций чтения, модификации и удаления над файлами     */
/* (каталогами) НА ЛЮБЫХ ЛОГИЧЕСКИХ МОДУЛЯХ ОБЪЕКТА без               */
/* доп. проверки РД дискреционным методом                             */
#define PLOT_NZRFOFA  271
/*                                                                    */
#define PLOT_NZRMAXNM PLOT_NZRFOFA /* максимальный номер              */
/*====================================================================*/
/*=================== Константы для NZR (конец) ======================*/
/*====================================================================*/

/* Б/о грифа секретности                                              */
#define PLOT_GRIF_NA   '\0'
#define PLOT_GRIF_NS   '0'
#define PLOT_GRIF_DSP  '1'
#define PLOT_GRIF_S    '2'
#define PLOT_GRIF_SS   '3'

/* Б/о размещения информации                                          */
#define PLOT_DATAPLACE_MEMORY  '&'
#define PLOT_DATAPLACE_TEMP    'V'
#define PLOT_DATAPLACE_FILE    'F'

/*====================================================================*/
/*=========== О П Р Е Д Е Л Е Н И Е  Т И П О В  SUZа (начало)         */
/*====================================================================*/
#define SUZ_SENDMSG         'S'      /* отправить сообщение : в пе-   */
                                     /* ременной suz_data - адрес     */
                                     /* адресаты: трансп. задачи      */
/*====================================================================*/
/*=========== О П Р Е Д Е Л Е Н И Е  Т И П О В  SUZа (конец)          */
/*====================================================================*/

/*====================================================================*/
/*===== О П Р Е Д Е Л Е Н И Е  Т И П О В  С С Ы Л О К (начало) =======*/
/*====================================================================*/
#define PLOT_IO_NOT_XLINK       0    /* полученный документ является  */
                                     /* информационным объектом, а не */
                                     /* ссылкой                       */
/*====================================================================*/
/*====== О П Р Е Д Е Л Е Н И Е  Т И П О В  С С Ы Л О К (конец) =======*/
/*====================================================================*/

/*====================================================================*/
/*=========== О П Р Е Д Е Л Е Н И Е  С Т Р У К Т У Р (начало) ========*/
/*====================================================================*/
#ifdef WINNT    /* Для работы визуальных компонент                    */
  namespace Plot
  {
#endif /*WINNT*/

/**********************************************************************
                     Набор для сохранения строк
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
                         ОПИСАНИЕ СИСТЕМНЫХ ИДЕНТИФИКАТОРОВ
***********************************************************************/

/***    идентификатор времени                                       ***/
typedef struct _IDT  {              /* Структура : Дата-тик           */
  u short       year;               /* Год                            */
  u char        month;              /* Месяц                          */
  u char        day;                /* День                           */
  u long        tik;                /* Тик                            */
} IDT;
typedef IDT   *PIDT;                /* Указатель на структуру IDT     */

/***    идентификатор времени и запрошенного действия               ***/
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

typedef struct _ITD  {              /* это - структура, состоящая :   */
  IDT           idt;                /* из идентиф-ра времени и        */
  u long        req;                /* Номера (кода) запрошенного     */
                                    /* действия                       */
  u char        tsk_name[4];        /* Код задачи-обработчика действия*/
} ITD;
typedef ITD   *PITD;                /* Указатель на структуру ITD     */

/**********************************************************************
          Управляющая структура системного сообщения
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
  u char  suz_iden[4];  /* Признак SUZа ("SUZ\x0")                    */
  u long  suz_size;     /* Размер запроса с телом сообщения           */
  u char  suz_izo[4];   /* Идентификатор задачи отправителя           */
  u char  suz_bo;       /* Байт-описатель запроса                     */
  u char  msg_type;     /* Тип сообщения (SYNC, ASYNC, SYS)           */
  u char  suz_rez[2];   /* Резерв                                     */
  u long  suz_naprsv;   /* Направление связи                          */
  u long  suz_nvu;      /* Номер вычислительной установки             */
  u char  suz_data[40]; /* Данные (дополнительные параметры запроса)  */
} SUZ;
typedef SUZ       *PSUZ;

/**********************************************************************
                      ОПИСАНИЕ СТРОКИ УКАЗАНИЯ
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
                      ОПИСАНИЕ УНИКАЛЬНОГО ЗАПРОСА
***********************************************************************/
typedef struct _QUERYID {
  u long       pid;
  u long       tid;
  IDT          date;
} QUERYID;
typedef QUERYID   *PQUERYID;

/**********************************************************************
                ОПИСАНИЕ СТРУКТУРЫ ЗАГОЛОВКА ОЧЕРЕДИ
***********************************************************************/
#define PLINE_DISK      0           /* Очередь на диске               */
#define PLINE_MEMORY    1           /* Очередь в памяти               */
#define LINE_DISK_SIZE  0x10485760  /* Граница сдвига дисковой очереди*/
typedef struct _LINE {
  char     linefname[PLOTPATHSIZE]; /* Имя файла очереди              */
  u long   count_message;  /* Количество сообщений в очереди          */
  u long   m_read;         /* Смещение маркера чтения                 */
  u long   m_write;        /* Смещение маркера записи                 */
  u long   sl_line;        /* Позиция шлагбаума                       */
  u long   linegran;       /* Размер очереди                          */
                           /* (установлено при инициализации очереди) */
  u long   diskline;       /* Размер данных в файле очереди           */
  char     marker_read;    /* Маркер чтения: 1 - память, 0 - диск     */
  char     marker_addn;    /* Маркер добавления: 1 - память, 0 - диск */
  char     lineplace;      /* Б/о размещения очереди                  */
  char     bo_sline;       /* Байт/описатель состояния очереди        */
/* Секция заголовка, используемая в очередях потоков (начало)         */
  char     name_thread[8]; /* Имя задачи(4) + имя потока(4)           */
  char    *buffer_line;    /* Начало очереди в памяти                 */
  char    *ZipBuffer;      /* Резервный буфер для обработки сообщения */
  u long   number_line;    /* Номер очереди                           */
  u long   tid;            /* Идентификатор потока                    */
/* Секция заголовка, используемая в очередях потоков (конец)          */
  char     disrezerv[12];  /* Резерв                                  */
} LINE;
typedef LINE   *PLINE;

/**********************************************************************
      ОПИСАНИЕ СТРУКТУР ЗАГОЛОВКА И КОНЦЕВИКА  СООБЩЕНИЯ В ОЧЕРЕДИ
***********************************************************************/
typedef struct _MESS {
  u long        size_message; /* Размер сообщения                     */
  u char        tsksend[4];   /* Имя задачи, записавшей сообщение     */
  u char        is_msg_clop;  /* Признак преобразования информации    */
  char          reserv[3];    /* Резерв                               */
  u long        w_size;       /* Размер сообщения, дополненого для    */
                              /* выравнивания на границу 4 байт       */
} MESS;
typedef MESS  *PMESS;

typedef struct _POMS {
  u long   mnextmsg;       /* Указатель на очередную запись           */
  u char   nextmsgp;       /* Б/о расположения следующего сообщения   */
  u char   pr_read;        /* Признак выборки сообщения из очереди    */
  u char   rezerv2[10];    /* Резерв                                  */
} POMS;
typedef POMS   *PPOMS;

/**********************************************************************
    СТРУКТУРА ДЛЯ ПОЛУЧЕНИЯ СЛОВАРЕЙ SLVNUM  SLVADR  GARDOV
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
typedef SLVS   *PSLVS;       /* это указатель на строку таблицы      */

/* Координаты объекта                                                 */
typedef struct _UP_OBJ_COORD{
  char    descr[64];        /* Описание местоположения объекта        */
  char    rezerv[64];
} UP_OBJ_COORD;
typedef UP_OBJ_COORD   *PUP_OBJ_COORD;

/* Статистика объекта                                                 */
typedef struct _UP_OBJ_STAT{
  IDT     started;          /* Дата выхода в online                   */
  IDT     last_activity;    /* Дата последней активности              */
  u long  msg_sent;         /* Послано пакетов                        */
  u long  msg_received;     /* Получено пакетов                       */
  char    rezerv[32];
} UP_OBJ_STAT;
typedef UP_OBJ_STAT   *PUP_OBJ_STAT;

/* Тип объекта (параметр type)                                        */
#define PLOT_OBJ_TYPE_UNK       0
#define PLOT_OBJ_TYPE_AIR       1
#define PLOT_OBJ_TYPE_SEE       2
#define PLOT_OBJ_TYPE_CAR       3
#define PLOT_OBJ_TYPE_BAG       4
#define PLOT_OBJ_TYPE_FIX       5
#define PLOT_OBJ_TYPE_GEN       6
#define PLOT_OBJ_TYPE_LEE       7

/* Типы объектов в словаре RDO (параметр TypeObj)                     */
#define  RDO_TYPE_FIX      1;      /* Стационарный                    */
#define  RDO_TYPE_CAR      2;      /* Подвижный                       */
#define  RDO_TYPE_SEE      3;      /* Морской                         */
#define  RDO_TYPE_AIR      4;      /* Воздушный                       */
#define  RDO_TYPE_BAG      5;      /* Мобильный (носимый)             */
#define  RDO_TYPE_LEE      6;      /* Защищенный                      */
#define  RDO_TYPE_SPC      7;      /* Космический                     */
/* Количество различных типов                                         */
#define  RDO_NUM_TYPE     = 7;
/* Типы главных объектов в словаре RDO                                */
/*(параметр TypeObj при LevelObj = "")            Главный:            */
#define  RDO_TYPE_FIX_GEN   RDO_NUM_TYPE + 1;  /* стационарный        */
#define  RDO_TYPE_CAR_GEN   RDO_NUM_TYPE + 2;  /* подвижный           */
#define  RDO_TYPE_SEE_GEN   RDO_NUM_TYPE + 3;  /* морской             */
#define  RDO_TYPE_AIR_GEN   RDO_NUM_TYPE + 4;  /* воздушный           */
#define  RDO_TYPE_BAG_GEN   RDO_NUM_TYPE + 5;  /* мобильный (носимый) */
#define  RDO_TYPE_LEE_GEN   RDO_NUM_TYPE + 6;  /* защищенный          */
#define  RDO_TYPE_SPC_GEN   RDO_NUM_TYPE + 7;  /* космический         */
/* Количество всех всех определенных типов                            */
#define  RDO_NUM_ALLTYPE  RDO_NUM_TYPE * 2;

/* Состояние объекта (параметр state)                                 */
#define PLOT_OBJ_STATE_AVAILABLE      1
#define PLOT_OBJ_STATE_NOTAVAILABLE   2
/* Список объектов                                                    */
typedef struct _UP_OBJ_LIST{
  char            parent[2];      /* Код объекта верхнего уровня      */
  char            code[2];        /* Код объекта                      */
  char            name[80];       /* Наименование объекта             */
  char            descr[256];     /* Описание объекта                 */
  char            type;           /* Тип объекта                      */
  char            state;          /* Состояние объекта                */
  char            reserv1[2];     /* Резерв                           */
  char            state_descr[80];/* Описание состояния объекта       */
  UP_OBJ_COORD    coord;          /* Местоположение объекта           */
  UP_OBJ_STAT     stat;           /* Статистика работы объекта        */
  char            reserv[152];    /* Резерв                           */
} UP_OBJ_LIST;
typedef UP_OBJ_LIST      *PUP_OBJ_LIST;

/* Структура взаимодействия объектов                                  */
typedef struct _UP_OBJ_COMMUNICATION{
  char   code_from[2];   /* Код объекта отправителя                   */
  char   code_to[2];     /* Код объекта получателя                    */
  char   task[4];        /* Транспорт                                 */
  char   addr[40];       /* Адрес                                     */
  char   reserv[16];     /* Резерв                                    */
} UP_OBJ_COMMUNICATION;
typedef UP_OBJ_COMMUNICATION      *PUP_OBJ_COMMUNICATION;

/* Структура заголовка ответа на запрос на список объектов            */
typedef struct _UP_OBJ_REPLY_HDR{
  char   coding;         /* Кодировка                                 */
  char   reserv1[3];     /* Резерв                                    */
  long   size;           /* Размер информационной части               */
  long   str_list_size;  /* Размер структуры списка объектов          */
  long   str_list_cnt;   /* Количество строк в списке объектов        */
  long   str_com_size;   /* Размер структуры UP_OBJ_COMMUNICATION     */
  long   str_com_cnt;    /* Количество строк в списке COMMUNICATION   */
  char   reserv[56];     /* Резерв                                    */
} UP_OBJ_REPLY_HDR;
typedef UP_OBJ_REPLY_HDR      *PUP_OBJ_REPLY_HDR;

/**********************************************************************
                ОПИСАНИЕ РАБОЧЕЙ ОБЛАСТИ СИСТЕМЫ (CWA)
***********************************************************************/
typedef struct _CWA{
  u short      cwa_moct;      /* База CSA ИСТОКа                      */
  char         cwaidbas[5];   /* База для счетчика идентификатора     */
  u char       cwaidcnt;      /* Счетчик для получения идентификато-  */
                              /* ра исходящего запроса                */
  u long       cwapdklw;      /* Признак старта задачи PDKL           */
  SLVS         cwasslvs;      /* Структура словарей                   */
  UP_OBJ_LIST *obj_list;      /* Список объектов АС (Только для ГМ)   */
  u long       obj_list_sz;   /* Количество доступных объектов АС     */
  UP_OBJ_COMMUNICATION *com_list; /* Список связей объектов АС (ГМ)   */
  u long       com_list_sz;   /* Размер списка                        */
  char         rezerv[958];   /* Резерв                               */
} CWA;
typedef CWA   *PCWA;          /* это указатель на строку таблицы      */

/**********************************************************************
                ОПИСАНИЕ ОБЛАСТИ УПРАВЛЕНИЯ ПРОЦЕССОМ (PCA)
***********************************************************************/
typedef struct _PCA {
  u char        pcastart[8]; /* признак начала PCA                    */
  u long        pcaspid;     /* Идентификатор процесса                */
  void         *pcaadres;    /* Адрес (точка входа) в головную пр-му  */
  u long        pcapcsa;     /* PID процесса, где ведется CSA         */
  void         *pcaacsa;     /* Адрес самой CSA                       */
  u char        pcapdata[80];/* Рабочая директория процесса           */
  PLINE         pcaline;     /* Адрес заголовка своей очереди         */
  IDT           pcaaprx;     /* Нач. погрешн. системных даты-времени  */
  ITD           pcasdate;    /* Идентификатор врем. начала и запроса  */
  u short       pcantask;    /* МАКС ч-ло подзадач проц. из MAX_TASK  */
  u short       pcanlink;    /* МАКС ч-ло открытых соед. из MAX_LINK  */
  void         *pcasmpr;     /* Указ. на массив стеков МПР потоков    */
  void         *pcacodtb;    /* Адрес массива таблиц перекодировки    */
  u long        pcacodsz;    /* Pазмер массива таблиц перекодировки   */
  void         *pcakfstb;    /* Адрес упр-щей таб. перекод-ки по КФС  */
  u long        pcakfssz;    /* Длина упр-щей таб. перекод-ки по КФС  */
  union {
    void       *adr[PLOBL/4];/* раб. область для передачи параметров  */
    double      dbl[PLOBL/8];/* раб. область для передачи параметров  */
    u long      lng[PLOBL/4];/* раб. область для передачи параметров  */
    u short     shr[PLOBL/2];/* раб. область для передачи параметров  */
    u char      chr[PLOBL  ];/* раб. область для передачи параметров  */
  } pcarab;
  u char        pcastop;     /* Признак принудительноко завешения     */
  u char        pcapcode;    /* Номер кодировки задачи                */
  char          stop_clop;
  char          reserv1;     /* Резерв                                */
  PCWA          pcaacwa;     /* Адрес рабочей области системы         */
  void         *pcafrmtb;    /* Адрес управляющей таблицы             */
                             /* типов документов                      */
  u long        pcafrmsz;    /* Pазмер управляющей таблицы            */
                             /* типов документов                      */
  void         *mystrpct;    /* Адрес моей строки в PCT               */
  IDT           last_act;    /* Время последнего обращ-я задачи к ядру*/
  void         *pcaamdm;     /* Адрес копии МДЛМ                      */
  void         *pcaccsa;     /* Адрес копии CSA                       */
  void         *pthrline;    /* Указатель на управляющую область      */
                             /* очередей потоков задачи               */
  char          pcafmdm;     /* Флаг замены МДЛМ                      */
  u char        pcalevel;    /* Статус процесса в системе             */
  char          reserv[186]; /* Резерв                                */
} PCA;
typedef PCA    *PPCA;

/**********************************************************************
                  ОПИСАНИЕ СЕКЦИИ ТАБЛИЦЫ ПРОЦЕССОВ (PCT)
***********************************************************************/
typedef struct _PCT {
  u char        pctsproc[4];  /* Логическое имя процесса-из PROC_IDT  */
  u char        pctprgnm[80]; /* Полное имя программы - из PROGNAME   */
  u char        pctlabel[80]; /* Ярлык имя программы - из PROGLBL     */
  u char        pctpdata[80]; /* Рабочая директория процесса          */
  u long        pctspid;      /* Идентификатор процесса               */
  PPCA          pctapca;      /* Адрес области управления процессом   */
  u char        reserv1[4];   /* Резерв                               */
  ITD           pctsdate;     /* Идентификатор врем. начала и запроса */
  PLINE         pctsline;     /* Адрес заголовка очереди              */
  u char        pctcode;      /* Номер кодировки задачи               */
  u char        pct_sys;      /* Признак(y,n): - системного процесса  */
  u char        pctstop;      /* Признак завершения процесса          */
  u char        pctlevel;     /* Статус процесса в системе            */
  u char        pctshide;     /* Запускать скрытым 0/1                */
  u char        pctseclevel;  /* Текущий (на сеанс) уровень           */
                              /* конфиденциальности задачи            */
  u char        pctshleif;    /* Признак (^=0) автономного            */
                              /* функционирования (шлейф)             */
  u char        pctonline;    /*@ Признак (^=0) работосп-сти процесса@*/
  u short       pctbuild;     /*@ Номер сборки приложения            @*/
  u char        pctmajor;     /*@ Номер версии приложения            @*/
  u char        pctminor;     /*@ Номер релиза приложения            @*/
  u char        pctdesign[16];/*@ Данные разработчика приложения     @*/
  u long        pctnuminp;    /* Счетчик количества входящих сообщений*/
  u long        pctinpread;   /* Счетчик количества прочитанных       */
                              /* входящих сообщений                   */
  u long        pctnumout;    /* Счетчик количества исходящих         */
                              /* сообщений                            */
  u long        pcthighinpsz; /* Объем информации поступившей в       */
  u long        pctlowinpsz;  /* очередь (старший и младший разряды)  */
  u long        pcthighreadsz;/* Объем информации прочитанной из      */
  u long        pctlowreadsz; /* очереди (старший и младший разряды)  */
  u long        pcthighoutsz; /* Объем исходящей информации           */
  u long        pctlowoutsz;  /* (старший и младший разряды)          */
/* внимание: элементы структуры, помеченные "@", определяются         */
/* приложением посредством вызова функции PutTaskInfo()               */
  char          reserv[44];  /* RESERV                                */
} PCT;
typedef PCT      *PPCT;      /* это указатель на строку таблицы PCT   */
typedef PCT       MPCT[];    /* зто массив строк - вся таблица PCT    */
typedef MPCT     *PMPCT;     /* это указатель на таблицу PCT          */

/**********************************************************************
                       Описание строки ТВК
***********************************************************************/
typedef struct _TVK {
  SU        tvk_su;
  SYS       tvk_sys;
} TVK;
typedef TVK      *PTVK;
typedef TVK       MTVK[];
typedef MTVK     *PMTVK;

/**********************************************************************
                  ОПИСАНИЕ ОБЩЕСИСТЕМНОЙ ОБЛАСТИ (CSA)
***********************************************************************/
typedef struct _CSA {
  u char     csastart[8]; /* признак начала CSA: "<CSASCODE>_CSA"     */
  IDT        csaapprx;    /* Нач. погрешность системных даты-времени  */
  ITD        csasdata;    /* Идент. даты-врем. и нач.действия(='00')  */
  u char     csaswork[80];/* пар-ры разр-чика: по данным из SYSWORK   */
  u char     csasname[80];/* полн. имя системы: по данным из SYSNAME  */
  u char     csascode[4]; /* код системы - из SYSCODE                 */
  u char     csaspref[4]; /* префикс имени сист. файлов из SYSPREF    */
  u char     csagname[80];/* полное имя организации - из ORGNAME      */
  u char     csagcode[8]; /* код организации - из ORGCODE             */
  u char     csaoname[80];/* полное имя объекта - из OBJNAME          */
  u char     csaocode[2]; /* код объекта - из OBJCODE                 */
  u char     csamname[80];/* полное имя модуля - из MODNAME           */
  u char     csamcode[2]; /* код модуля - из MODCODE                  */
  u char     csanet[4];   /* идентиф. сети+номер машины в сети(2+2)   */
  u char     csaspath[80];/*                                          */
  u char     csasinit[80];/*                                          */
  u char     csampath[80];/*                                          */
  u char     csaminit[80];/*        СИСТЕМНЫЕ ДИРЕКТОРИИ              */
  u char     csamline[80];/*                                          */
  u char     csamtemp[80];/*                                          */
  u char     csamdump[80];/*                                          */
  u char     csasslv[80]; /*                                          */
  u char     csamjurn[80];/*                                          */
  u short    csanproc;    /* МАКС ч-ло процессов системы-из MAX_PROC  */
  u short    csantask;    /* МАКС ч-ло потоков процесса-MAX_TASK      */
  u short    csanlink;    /* МАКС ч-ло открытых соед-й из MAX_LINK    */
  u char     reserv1[2];  /* RESERV                                   */
  u long     csalnsz;     /* Размер очередей (байт)                   */
  PPCT       csampct;     /* адрес массива описания сист. процессов   */
  void      *csacodtb;    /* Адрес массива таблиц перекодировки       */
  u long     csacodsz;    /* Pазмер массива таблиц перекодировки      */
  void      *csakfstb;    /* Адрес управляющей таблицы перекодировки  */
                          /* по КФСу                                  */
  u long     csakfssz;    /* Pазмер управляющей таблицы перекодир-ки  */
                          /* по КФСу                                  */
  PTVK       csaatvk;     /* Адрес таблицы временного контроля        */
  u long     csatvksz;    /* Максимальное количество строк в таблице  */
  u char     csanname[80];/* Полное имя сети                          */
  u char     csa_code;    /* Кодировка, в которой находятся таблицы   */
  u char     csalevel;    /* Уровень инициализации системы            */
  char       reserv2[2];  /* RESERV                                   */
  PCWA       csaacwa;     /* Адрес рабочей области системы            */
  u long     csarproc;    /* Текущее количество задач в системе       */
  u char     csaidusr[2];
  u char     csavzv[10];
  u char     csafio[18];
  u char     csarange[2];
  void      *csafrmtb;    /* Адрес управляющей таблицы типов док-тов  */
  u long     csafrmsz;    /* Pазмер управляющей таблицы типов д-тов   */
  void      *csaexttb;    /* Адрес управляющей таблицы расширений СУ  */
  u long     csaextsz;    /* Pазмер управляющей таблицы расширений СУ */
  u long     csajisz;     /* Максимальное количество строк в ЖВС      */
  u long     csajosz;     /* Максимальное количество строк в ЖИС      */
  u long     csajsosz;    /* Максимальное количество строк в ЖИЗСО    */
  char       csajsofl[8]; /* Флаги управления ЖИЗСО:                  */
                          /* 0 - Фиксировать операции чтения          */
                          /* 1 - Фиксировать операции записи          */
                          /* 2 - Фиксировать операции удаления        */
  u long     csajfosz;    /* Максимальное количество строк в ЖИЗFTP   */
  u long     csajsysosz;  /* Максимальное количество строк в ЖИЗSYS   */
  u long     csajsysisz;  /* Максимальное количество строк в ЖВЗSYS   */
  void      *csaamdm;     /* Адрес МДЛМ                               */
  char       csafmdm;     /* Флаг замены МДЛМ                         */
  u char     csaminuslvl; /* Минимальный уровень СД - пользователя    */
  u char     csamaxuslvl; /* Максимальный уровень СД - пользователя   */
  char       reserv3;
  char       main_obj[4]; /* ЛА головного объекта системы (0 - сам)   */
  void      *adrslvokb;   /* Адрес таблицы открытых ключей            */
  void      *adrslvzkb;   /* Адрес закрытого ключа                    */
  u long     okbNum;      /* Количество записей в таблице открых      */
                          /* ключей                                   */
  void      *adrlmp;      /* Словарь пользователей системы            */
  u long     numlmp;      /* Количество записей в словаре             */
                          /* пользователей системы                    */
  void      *adrlms;      /* Словарь штатного расписания организации  */
  u long     numlms;      /* Количество записей в словаре             */
                          /* штатного расписания организации          */
  void      *adrrdo;      /* Словарь - справочник КСА                 */
  u long     numrdo;      /* Количество записей в словаре -           */
                          /* справочнике КСА                          */
  void      *csavnttb;    /* Адрес управляющей таблицы сценариев      */
  u long     csavntsz;    /* Pазмер управляющей таблицы сценариев     */
  char       reserv[92];  /* RESERV                                   */
} CSA;
typedef CSA *PCSA;

/**********************************************************************
 Структура закладки записи в СИ
***********************************************************************/
typedef struct _INFREC {
    u char Regim;
    u char Rezerv;
}INFREC;
typedef INFREC  *PINFREC; /* указ. на структуру                       */

/**********************************************************************
                ОПИСАНИЕ СТРУКТУРЫ ISTOK - таблицы
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
typedef STRTAB  MSTRTAB[];    /* зто массив строк - вся таблица       */
typedef MSTRTAB *PMSTRTAB;    /* это указатель на таблицу             */

/**********************************************************************
                ОПИСАНИЕ СТРУКТУРЫ КУРСОРА
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
typedef CURSTR     *PCURSTR;   /* указ. на структуру                  */
typedef CURSTR      MCURSTR[]; /* массив структур                     */
typedef MCURSTR    *PMCURSTR;  /* указатель на этот массив структур   */

/**********************************************************************
                ОПИСАНИЕ УПРАВЛЯЮЩЕЙ СТРУКТУРЫ СОЕДИНЕНИЯ
***********************************************************************/
typedef struct _LINKSTR {
  QUERYID      queryido;    /* идентификатор отправителя запроса      */
  QUERYID      queryidp;    /* идентификатор получателя запроса       */
  u char       link_bo;     /* б/о типа иформации (текст, HTML и т.д  */
  u char       linkreg;     /* режим обмена                           */
  u char       linksegm;    /* битовая шкала сегментов документа      */
  u char       linkrazm;    /* б/о размещения информации              */
    void      *linkadr;     /* собственный адрес стр-ры соед-я        */
    char       linkrzrv[2]; /* резерв                                 */
  u char       link_xlink;  /* б/о типа ссылки                        */
  u char       linklast;    /* б/о признака последнего кадра          */
    long       linkprz;     /* признак результата выполнения          */
  u char       linkdbn[8];  /* имя БД                                 */
  u char       linktabn[8]; /* имя таблицы                            */
  u char       linkkeyf[8]; /* ключ первой запрошенной записи         */
  u char       linkkeyl[8]; /* ключ последней возвращенной записи     */
  u long       linkgetr;    /* количество запрошенных записей         */
  u long       linkputr;    /* количество полученных записей          */
  u long       linkblkl;    /* максимальный размер блока              */
  u long       linkblkn;    /* номер блока (кадра)                    */
    void      *linknach;    /* адрес начала области данных            */
  u long       linkobll;    /* максимальная длина области данных      */
  u long       linkinfl;    /* текущая длина инф-ции в обл. данных    */
 PCURSTR       pcurstr;     /* указатель на структуру CURSTR          */
    IDT        linkidt;     /* время и дата запрашиваемого документа  */
  u char       linkvxd[8];  /* входящий номер док-та                  */
} LINKSTR;
typedef LINKSTR   *PLINKSTR;   /* указатель на заголовок документа    */

/**********************************************************************
                 ОПИСАНИЕ ТАБЛИЦЫ ОТКРЫТЫХ СОЕДИНЕНИЙ
***********************************************************************/
typedef struct _LINKT {
  u char   modname[4];        /* имя модуля, с которым установлено    */
                              /* соединение                           */
  PLINKSTR plinkstr;          /* Адрес упр-щей стр-ры соед-я          */
  PCURSTR  pcurstr;           /* Адрес структуры CURSTR               */
  u char   linktrez[4];       /* резерв                               */
} LINKT;
typedef LINKT   *PLINKT;      /* это указатель на строку таблицы      */
typedef LINKT    MLINKT[];    /* зто массив строк - вся таблица       */
typedef MLINKT  *PMLINKT;     /* это указатель на таблицу             */

/**********************************************************************
               ОПИСАНИЕ СТЕКА МЕТАПРОГРАММ
***********************************************************************/
typedef struct _CALLSTK {
    long       avr;
    long       rez;
    long       idfn;
    long       rezerv;
    long       parm[STKNUMPARM];
} CALLSTK;
typedef CALLSTK    *PCALLSTK;  /* указ. на строку (стек МПР потока)   */
typedef CALLSTK     MCALLSTK[];/* массив строк из стеков МПР потоков  */
typedef MCALLSTK   *PMCALLSTK; /* указатель на этот массив строк      */

typedef struct _STKMPR {
    char               pr_stek[4];/* Признак стека ("STEK")           */
    long               smppavr;
    long               smpprez;
  u long               smptid;   /* Идент. потока('00'-если его нет)  */
    void              *smphndl;  /* дескриптор потока                 */
  PCSA                 smpacsa;
  u long               smpidcsa; /* PID проц., в кот. находится CSA   */
  PPCA                 smpapca;  /* Адрес PCA                         */
  u long               smpidpca; /* PID проц., кот. принадлежит PCA   */
    long               smpdelay; /* Текущее время задержки потока     */
  PLINKT               smplinkt; /* Указатель на таблицу открытых     */
                                 /* соединений                        */
    long               smpbusy;  /* Пр. занятости строки стека (не 0) */
  u long               smpncall; /**/
  enq_set             *enqset;   /**/
  u char               res1[2];  /* Резерв                            */
  char                 blockmode;/**/
  char                 smplevel; /**/
  void                *smpblock; /* Адрес стр-ры блокированного вызова*/
  u char               rezerv[192];/* Резерв                          */
  union {
    void       *adr[512/4];  /* раб. область для передачи параметров  */
    double      dbl[512/8];  /* раб. область для передачи параметров  */
    u long      lng[512/4];  /* раб. область для передачи параметров  */
    u short     shr[512/2];  /* раб. область для передачи параметров  */
    u char      chr[512];    /* раб. область для передачи параметров  */
  } smprobl;
  CALLSTK              callstk[16];/**/
} STKMPR;
typedef STKMPR     *PSTKMPR;   /* указ. на строку (стек МПР потока)   */
typedef STKMPR      MSTKMPR[]; /* массив строк из стеков МПР потоков  */
typedef MSTKMPR    *PMSTKMPR;  /* указатель на этот массив строк      */

/**********************************************************************
            ОПИСАНИЕ АДРЕСА (ОТПРАВИТЕЛЯ ИЛИ ПОЛУЧАТЕЛЯ)
***********************************************************************/
typedef struct _ADRESS {
  u short un;       /* Условный номер                                 */
  u short pn;       /* Персональный номер                             */
} ADRESS;
typedef ADRESS     *PADRESS;

/**********************************************************************
            ОПИСАНИЕ СЛОВАРЯ ПЕРЕВОДА АП СУ В НОМЕР ВУ
***********************************************************************/
typedef struct _SLVNUM {
  union {
    ADRESS   a;
    u long   l;
  }          addr_p;           /* Адрес получателя (4б)     (вход)    */
  u long     num_vu;           /* Номер ВУ  (выход) (4б)              */
  u long     trnsprt;          /* Имя транспортной задачи             */
  char       pr_alt;           /* Признак альтернативности            */
  char       rezerv[3];
} SLVNUM;                      /* это строка словаря                  */
typedef SLVNUM   *PSLVNUM;     /* указатель на строку                 */
typedef SLVNUM    MSLVNUM[];   /* зто массив строк - вся таблица      */
typedef MSLVNUM  *PMSLVNUM;    /* это указатель на таблицу            */

/**********************************************************************
                           ОПИСАНИЕ SNT
***********************************************************************/
typedef struct _SLVSNT {
  union {
    ADRESS   a;
    u long   l;
  } addr_p;                    /* Адрес получателя (4б)     (вход)    */
  u long     num_vu;           /* номер ВУ  (выход)                   */
  u long     trnsprt;          /* Имя транспортной задачи             */
  u char     sntmname[80];     /* Имя лог. модуля                     */
  u char     sntidusr[2];
  u char     sntvzv[10];
  u char     sntfio[18];
  char       pr_alt;           /* Признак альтернативности КС         */
  u char     sntrange[2];      /* Ранг                                */
  u char     rezerv[3];        /* Резерв                              */
} SLVSNT;                      /* это строка словаря                  */
typedef SLVSNT   *PSLVSNT;     /* указатель на строку                 */
typedef SLVSNT    MSLVSNT[];   /* зто массив строк - вся таблица      */
typedef MSLVSNT  *PMSLVSNT;    /* это указатель на таблицу            */

/**********************************************************************
             СЛОВАРЬ ПЕРЕВОДА НОМЕРА ВУ В АДРЕС
***********************************************************************/
typedef struct _SLVADR {
  u long   num_vu;             /* Номер ВУ (вход)                     */
  u long   trnsprt;            /* Имя транспортной задачи             */
  char     adr_vu[40];         /* Сетевой адрес ВУ                    */
  char     pr_got;             /* Признак готовности канала связи     */
  char     pr_ping;            /* Признак того, что линия пингуется   */
                               /* (только для BAULа)                  */
  char     rezerv[10];
} SLVADR;                      /* это строка словаря                  */
typedef SLVADR   *PSLVADR;     /* указатель на строку                 */
typedef SLVADR    MSLVADR[];   /* зто массив строк - вся таблица      */
typedef MSLVADR  *PMSLVADR;    /* это указатель на таблицу            */

/**********************************************************************
                   СЛОВАРЬ РАЗВОДКИ ПО ЗАДАЧАМ
***********************************************************************/
typedef struct _SLV21Y {
  u char    kvs[3];            /* код вида сообщения                  */
  u char    nm_field[8];       /* символическое имя поля или смещ-е   */
  u char    usl;               /* условие сравн-я (равно - не равно)  */
  u long    testsz;            /* сколько б. от начала поля сравнить  */
  u char    test[16];          /* с чем сравнить                      */
  u char    preproc;           /* признак препроцессорной обработки   */
  u char    pr_init;           /* признак инициализации задачи        */
  u char    task[4];           /* имя целевой задачи                  */
  u char    is_null;           /* Базовая строка или нет              */
  u char    rezerv[13];
} SLV21Y;
typedef SLV21Y   *PSLV21Y;
typedef SLV21Y    MSLV21Y[];   /* зто массив строк - вся таблица      */
typedef MSLV21Y  *PMSLV21Y;    /* это указатель на таблицу            */

/**********************************************************************
          ТАБЛИЦА ГАРАНТИРОВАННОГО ДОВЕДЕНИЯ GARDOV
***********************************************************************/
typedef struct _GARDOV {
  u char     pr_nal;           /* Признак наличия строки              */
  u char     pr_read;          /* Признак чтения                      */
  u char     rezerv1[2];       /* Резерв                              */
  u long     nextstr;          /* Следующая строка                    */
  u long     priorstr;         /* Предыдущая строка                   */
  union {
    ADRESS   a;
    u long   l;
  }          adr_p;            /* Адрес получателя(4б) напр-ие связи  */
  SU         str_SU;           /* СУ(одна из квитанций) или заявка    */
                               /* ВНИМАНИЕ!!! Теперь если SU - это    */
                               /* заявка, то при записи SU в ГАРДОВ   */
                               /* поле su_kln чистится. Далее оно     */
                               /* используется для хранения ключа ВП  */
                               /* получателя для режима докачки       */
  IDT        added;            /* Время добавления                    */
  IDT        changed;          /* Время изменения состояния           */
                               /*   priorstr == -1                    */
  u char     rezerv[16];       /* Резерв                              */
} GARDOV;                      /* это строка                          */
typedef GARDOV   *PGARDOV;     /* указатель на строку                 */
typedef GARDOV    MGARDOV[];   /* зто массив строк - вся таблица      */
typedef MGARDOV  *PMGARDOV;    /* это указатель на таблицу            */

/**********************************************************************
             ОПИСАНИЕ ЗАГОЛОВКА ТАБЛИЦЫ ПЕРЕКОДИРОВКИ
***********************************************************************/
typedef struct _CODTABN {
  u char        tabpref[8];   /* Префикс таблицы "ТАБЛИЦА "           */
  u long        tabnum;       /* Порядковый номер в файл-словаре      */
  u char        tabname[52];  /* Имя таблицы                          */
} CODTABN ;
typedef CODTABN   *PCODTABN;

/**********************************************************************
                 ОПИСАНИЕ ТАБЛИЦЫ ПЕРЕКОДИРОВКИ
***********************************************************************/
typedef struct _CODTAB {
  CODTABN       tabname;      /* Заголовок таблицы                    */
  u char        tabbody[256]; /* Тело таблицы                         */
} CODTAB;
typedef CODTAB   *PCODTAB;   /* это указатель на таблицу              */


/**********************************************************************
                      ОПИСАНИЕ СЕГМЕНТА ДОКУМЕНТА
***********************************************************************/
typedef struct _SEGMENT {
  u long       segmlen;    /* размер сегмента                         */
  u char       SEGMNum;    /* номер сегмента                          */
  u char       segm_bo;    /* б/о сегмента                            */
  u char       segmrez[2]; /* резерв                                  */
} SEGMENT;

/**********************************************************************
                      ОПИСАНИЕ ЗАГОЛОВКА ДОКУМЕНТА
***********************************************************************/
typedef struct _DOCUMENT {
  u char       docname[8];     /* имя документа по классификатору     */
  u char       docrezv[8];     /* резерв                              */
  SEGMENT      docsegm[SEGMN]; /* заголовки сегментов документа       */
} DOCUMENT;
typedef DOCUMENT   *PDOCUM;    /* указатель на заголовок документа    */

/**********************************************************************
         ОПИСАНИЕ УПРАВЛЯЮЩЕЙ ТАБЛИЦЫ ПЕРЕКОДИРОВКИ ПО КФС В СУ
***********************************************************************/
typedef struct _CDTKFS {
  u char       cdtkfsnm[3];   /* КФС                                  */
  u char       cdttipfd;      /* тип поля                             */
  u char       cdtfdlen[8];   /* длина поля                           */
  u char       cdtfdnam[8];   /* имя поля                             */
} CDTKFS;
typedef CDTKFS   *PCDTKFS;    /* это указатель на строку таблицы      */
typedef CDTKFS    MCDTKFS[];  /* зто массив строк - вся таблица       */
typedef MCDTKFS  *PMCDTKFS;   /* это указатель на таблицу             */

/**********************************************************************
         ОПИСАНИЕ УПРАВЛЯЮЩЕЙ ТАБЛИЦЫ ТИПОВ ДОКУМЕНТОВ
***********************************************************************/
typedef struct _FORMAT {
  u char       frmext[8];     /* расширение файлов для типа док-та    */
  u char       frmdescr[80];  /* описание                             */
  u char       frmview[256];  /* обработчик команды "просмотр"        */
  u char       frmprint[256]; /* обработчик команды "печать"          */
  u char       frmres[3];     /* резерв                               */
  u char       frmbocod;      /* б/о (перекодировать(y)/не перек-ть(n)*/
} FORMAT;
typedef FORMAT   *PFORMAT;
typedef FORMAT    MFORMAT[];
typedef MFORMAT  *PMFORMAT;

/**********************************************************************
     ОПИСАНИЕ УПРАВЛЯЮЩЕЙ ТАБЛИЦЫ ТИПОВ РАСШИРЕНИЙ СТРОКИ УКАЗАНИЯ
***********************************************************************/
typedef struct _KFSEXT {
  u char       kfs[3];        /* КФС                                  */
  u char       rezerv1[1];    /* резерв                               */
  u long       ext_size;      /* Размер расширения СУ                 */
  u char       rezerv[8];     /* резерв                               */
} KFSEXT;
typedef KFSEXT   *PKFSEXT;    /* это указатель на строку таблицы      */
typedef KFSEXT    MKFSEXT[];  /* зто массив строк - вся таблица       */
typedef MKFSEXT  *PMKFSEXT;   /* это указатель на таблицу             */

/**********************************************************************
     ОПИСАНИЕ УПРАВЛЯЮЩЕЙ ТАБЛИЦЫ ТИПОВ РАСШИРЕНИЙ СТРОКИ УКАЗАНИЯ
***********************************************************************/
typedef struct _TASKSPIS {
  char         tskname[4];
  IDT          obr_dt;
} TASKSPIS;
typedef TASKSPIS   *PTASKSPIS;    /* это указатель на строку таблицы  */

/**********************************************************************
           Описание управляющего сегмента временной памяти
***********************************************************************/
#define DATAFILE   1       /* Данные - в файле                        */
#define DATAMEMORY 2       /* Данные - в памяти                       */
#define DATATEMP   3       /* Данные - во временной памяти            */
typedef struct _TMPHDR {
  SU           su;         /* строка указания для данных              */
  char         su_ext[PLOTSUSIZE];/* расширение строки указания       */
  u long       su_size;    /* размер расширения строки указания       */
  u long       datasize;   /* размер данных во временной памяти       */
  long         ref_type;   /* вид ссылки на данные:                   */
                           /* DATAFILE - данные расположены в файле   */
                           /*            fname                        */
                           /* DATAMEMORY - данные расположены в па-   */
                           /*              мяти процесса с идентифи-  */
                           /*              катором pid по адресу adr  */
  char         mustdel;    /* Признак необходимости удаления файла    */
                           /* вместе со ссылкой на него               */
  char         is_clop_ext;/* Признак закрытия EXT СУ                 */
  char         is_clop_data;/* Признак закрытия данных                */
  char         clop_key[PLOTKEYSIZE];/* Ключ закрытия данных          */
  u char       idDS;       /* Байт описатель достоверности сообщения
                           /* 0 - неподписано                         */
                           /* 1 - удостоверено                        */
                           /* 2 - недостоверно                        */
                           /* 3 - ключ скомпрометирован               */
                           /* 4 - срок действия сертификата истек     */
                           /* 5 - ключ ненайден                       */
                           /* 6 - словарь необнаружен                 */
  u long       clop_wrk1;  /* рабочая переменная для закрытия         */
  u long       clop_wrk2;  /* рабочая переменная для закрытия         */
  char         is_own;     /* признак принадлежности ВП ПЛОТа         */
                           /* 0 - не принадлежит ВП ПЛОТа             */
                           /* 1 - принадлежит ВП ПЛОТа                */
  char         rezerv2[2]; /* резерв 2                                */
  char         pr_fwd; /*  */
  char         keyfwd[8];  /* ключ на FWD - сообщение                 */
  TASKSPIS     mtaskspis[PLOTTASKADD]; /* массив списка задач         */
  u long       lentask;      /* Количество элементов в масс. mtaskspis*/
  u long       clop_sm;    /* рабочая переменная для открытия         */
  u long       clop_wrk3;  /* рабочая переменная для закрытия/открытия*/
  u long       clop_wrk4;  /* рабочая переменная для закрытия/открытия*/
  char         rezerv3[20];/* резерв 3                                */
  union {
    struct {
      long     pid;
      void    *adr;
      char     rezerv[PLOTPATHSIZE - sizeof(long) - sizeof(void *)];
    }          point;
    char       fname[PLOTPATHSIZE]; /* имя файла                      */
  }            refer;      /* ссылка на данные                        */
} TMPHDR;
typedef TMPHDR   *PTMPHDR;/* это указатель на строку таблицы          */

#define PRPNOERR       '0'/* Нет ошибки                               */
#define PRPERRMODEXIST '1'/* Модуль зарегистрирован на другой ВУ      */
#define PRPERRMAXSNT   '2'/* Переполнена SNT шлюза                    */
#define PRPERRCHANGE   '3'/* Ошибка корректировки SNT                 */
#define PRPERRLINK     '4'/* Нет связи                                */
typedef struct _PRP{
  u char prpkvs[3];       /* Код вида сообщения                       */
  u char prpdate[8];      /* Дата                                     */
  u char prptime[8];      /* Время                                    */
  u char prpuno[2];       /* УНО                                      */
  u char prppno[2];       /* ПНО                                      */
  u char prpnvu[4];       /* Номер вычислительной установки           */
  u char prpmname[80];    /* Наименование модуля                      */
  u char prpid[2];
  u char prprange[2];
  u char prppass[8];
  u char prpvzv[10];
  u char prpfio[18];
  u char prperror;        /* Результат выполнения операции            */
  u char prpprotv;        /* Признак необх. ответа ('n' - не нужен)   */
  u char prprez[107];     /* Резерв                                   */
} PRP;
typedef PRP     *PPRP;

/**********************************************************************
          ОПИСАНИЕ СТРУКТУРЫ ДАТА - ВРЕМЯ (СИМВОЛЬНЫЙ ВИД)
***********************************************************************/
typedef struct _DTS {
  u char  dtsday[2];       /* День                                     */
  u char  dtsmonth[2];     /* Месяц                                    */
  u char  dtsyear[4];      /* Год                                      */
  u char  dtshour[2];      /* Часы                                     */
  u char  dtsmin[2];       /* Минуты                                   */
  u char  dtssec[2];       /* Секунды                                  */
  u char  dtshund[3];      /* Милисекунды                              */
  u char  dtsresrv[3];     /* Резерв                                   */
} DTS;
typedef DTS      *PDTS;

/**********************************************************************
     ОПИСАНИЕ ЗАГОЛОВКА СИСТЕМНОГО ЖУРНАЛА ДОКУМЕНТООБОРОТА
***********************************************************************/
typedef struct _JURNSYSZ {
  char      idnt[4];     /* Признак журнала                           */
  u long    n_rec;       /* Количество записей в журнале              */
  u long    sm_add;      /* Смещение добавления                       */
  u long    sm_first;    /* Смещение первой записи                    */
  u char    code;        /* Кодировка заголовка журнала               */
  u char    rezerv1[3];  /* Резерв                                    */
  u long    reclen;      /* Длина записи в журнале                    */
  u char    owner[4];    /* Владелец (имя модуля или НВУ (для JSS))   */
  u char    rezerv[4];   /* Резерв                                    */
} JURNSYSZ;
typedef JURNSYSZ   *PJURNSYSZ;

/**********************************************************************
     ОПИСАНИЕ СТРОКИ СИСТЕМНОГО ЖУРНАЛА ДОКУМЕНТООБОРОТА
***********************************************************************/
typedef struct _JURNSYS {
  STKMPR   sysstk;       /* Стек                                      */
  IDT      systime;      /* Время                                     */
  char     systsk[4];    /* Имя задачи                                */
  char     sysinf[256];  /* Информация о событии                      */
  u long   syssmesh;     /* Смещение области данных                   */
  u long   syslen;       /* Длина области данных                      */
  char     sysprdel;     /* Признак удаления строки                   */
  u char   sysprview;    /* Признак просмотра строки                  */
  u char   code;         /* Кодировка строки                          */
  u char   rezerv1;      /* Резерв                                    */
  u char   rezerv[232];  /* Резерв                                    */
} JURNSYS;
typedef JURNSYS   *PJURNSYS;  /* это указатель на строку              */

/**********************************************************************
             ОПИСАНИЕ СИСТЕМНОГО БЛОКА ИНФОРМАЦИИ
***********************************************************************/
typedef struct _UPINFO {
  u char     orgname[80]; /* полное имя организации                   */
  u char     orgcode[8];  /* код организации                          */
  u char     objname[80]; /* полное имя объекта                       */
  u char     objcode[2];  /* код объекта                              */
  u char     modname[80]; /* полное имя логического модуля            */
  u char     modcode[2];  /* код логического модуля                   */
  u char     nvu[4];      /* номер вычислительной установки           */
  u char     netname[80]; /* полное имя сети                          */
  u char     userstat[10];/* должность пользователя                   */
  u char     userfio[18]; /* ФИО пользователя                         */
  u char     logadr[4];   /* свой логический адрес                    */
  u char     userid[2];   /* идентификатор пользователя               */
  u char     userrang[2]; /* ранг пользователя                        */
  u char     rezerv[12];  /* резерв                                   */
} UPINFO;
typedef UPINFO   *PUPINFO;    /* это указатель на строку              */

/**********************************************************************
             ОПИСАНИЕ БЛОКА ИНФОРМАЦИИ ЗАДАЧИ
***********************************************************************/
typedef struct _TASKINFO {
  IDT        idtapprx;    /* Нач. погрешность системных даты-времени  */
  u long     pid;         /* Идентификатор процесса                   */
  u long     tid;         /* Идент. потока                            */
  void      *hndl;        /* Идент. потока                            */
  u char     name[4];     /* Идентификатор задачи                     */
  u char     workdir[80]; /* Рабочая директория задачи                */
  u char     stop;        /* Признак принудительноко завешения        */
  u char     code;        /* Номер кодировки задачи                   */
  u char     rezerv[22];  /* резерв                                   */
} TASKINFO;
typedef TASKINFO   *PTASKINFO;/* это указатель на строку              */

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
                         ОПИСАНИЕ ЖУРНАЛОВ
***********************************************************************/
/* Состояние записи в журнале                                         */
#define   J_RECORD_DEL     0xFF
#define   J_RECORD_ADD     0x00
#define   J_RECORD_MDF     0x01

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ВХОДЯЩЕЙ ИНФОРМАЦИИ
***********************************************************************/
/* Определяем элементы журнала входящих                               */
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
  u long       number;        /* Порядковый номер записи в журнале    */
  char         state;         /* Состояние записи                     */
  u char       code;          /* Кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* Время создания записи                */
  IDT          modify_dt;     /* Время корректировки записи           */
  IDT          recv_dt;       /* Дата и время получения сообщения     */
  IDT          ispr_dt;       /* Дата и время для исполнения          */
  IDT          obr_dt;        /* Дата и время обработки сообщения     */
  IDT          isp_dt;        /* Дата исполнения сообщения            */
  u long       msglen;        /* Размер сообщения                     */
  u char       mykey[8];      /* Входящий ключ                        */
  u char       farkey[8];     /* Исходящий ключ сообщения             */
  u char       tmpkey[8];     /* Ключ сообщения во временной памяти   */
  u char       la_otpr[4];    /* Лог. адрес отправителя сообщения     */
  u char       format;        /* Формат                               */
  u char       kvs[3];        /* КВС сообщения                        */
  u char       grif;          /* Гриф                                 */
  u char       sr;            /* Срочность                            */
  u char       tmpkeyfwd[8];  /* Ключ сообщения во временной памяти   */
  u char       la_otprfwd[4]; /* Лог. адрес отправителя сообщения     */
  u char       pr_fwd;        /* Признак разв. сообщения              */
  u char       ds_status;     /* Статус проверки подписи              */
                              /* 0 - неподписано                      */
                              /* 1 - удостоверено                     */
                              /* 2 - недостоверно                     */
                              /* 3 - ключ скомпрометирован            */
                              /* 4 - срок действия сертификата истек  */
                              /* 5 - ключ ненайден                    */
                              /* 6 - словарь необнаружен              */
  u char       rezerv[20];    /* Резерв                               */
} JOURNIN;
typedef JOURNIN   *PJOURNIN;

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ИСХОДЯЩЕЙ ИНФОРМАЦИИ
***********************************************************************/
/* Определяем элементы журнала исходящих                              */
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
  u long       number;        /* Порядковый номер записи в журнале    */
  char         state;         /* Состояние записи                     */
  u char       code;          /* Кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* Время создания записи                */
  IDT          modify_dt;     /* Время корректировки записи           */
  IDT          send_dt;       /* Дата и время отправки сообщения      */
  IDT          dov_dt;        /* Дата и время доведения сообщения     */
  IDT          obrb_dt;       /* Дата и время постановки на обработку */
  IDT          obr_dt;        /* Дата и время обработки сообщения     */
  IDT          ispr_dt;       /* Требуемая дата и время исполнения    */
  IDT          isp_dt;        /* Дата и время исполнения сообщения    */
  u long       msglen;        /* Размер сообщения                     */
  u char       mykey[8];      /* Исходящий ключ                       */
  u char       farkey[8];     /* Входящий ключ сообщения              */
  u char       tmpkey[8];     /* Ключ сообщения во временной памяти   */
  u char       la_pol[4];     /* Лог. адрес получателя сообщения      */
  u char       format;        /* Формат                               */
  u char       kvs[3];        /* КВС сообщения                        */
  u char       grif;          /* Гриф                                 */
  u char       sr;            /* Срочность                            */
  u char       tmpkeyfwd[8];  /* Ключ сообщения FWD во ВП             */
  u char       pr_fwd;        /* Признак разв. сообщения              */
  u char       ds_status;     /* Статус проверки подписи              */
                              /* 0 - неподписано                      */
                              /* 1 - удостоверено                     */
                              /* 2 - недостоверно                     */
                              /* 3 - ключ скомпрометирован            */
                              /* 4 - срок действия сертификата истек  */
                              /* 5 - ключ ненайден                    */
                              /* 6 - словарь необнаружен              */
  u char       rezerv[8];     /* Резерв                               */
} JOURNOUT;
typedef JOURNOUT   *PJOURNOUT;

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ИСХОДЯЩИХ ЗАПРОСОВ СИНХРОННОГО ОБМЕНА
***********************************************************************/
/* Определяем коды обмена                                             */
#define   JSO_OBMN_DEX           2     /* документы                   */
#define   JSO_OBMN_REX           4     /* записи                      */
#define   JSO_OBMN_TEX           8     /* таблицы                     */
/* Определяем коды запросов                                           */
#define   JSO_QUERY_READ        16     /* читать                      */
#define   JSO_QUERY_DELETE      32     /* удалить                     */
#define   JSO_QUERY_SAVE        64     /* записать                    */
#define   JSO_QUERY_CREATE     128     /* создать                     */
#define   JSO_QUERY_NEXT       256     /* дать следующую              */
#define   JSO_QUERY_PREV       512     /* дать предыдущую             */
#define   JSO_QUERY_OPEN      1024     /* открыть                     */
/* Определяем элементы журнала исходящих запросов                     */
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
  u long       number;        /* порядковый номер записи в журнале    */
  char         state;         /* состояние записи                     */
  u char       code;          /* кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* время создания записи                */
  IDT          modify_dt;     /* время корректировки записи           */
  IDT          send_dt;       /* дата отправки запроса                */
  QUERYID      my_key;        /* исходящий ключ запроса               */
  u long       querycod;      /* Код запроса + Код обмена             */
  u char       sourcela[4];   /* логический адрес получателя запроса  */
  u char       objkey[8];     /* ключ адресуемого ИО                  */
  IDT          obj_dt;        /* дата, на кот. выдан запрос           */
  IDT          obr_dt;        /* дата получения ответа на запрос      */
  QUERYID      far_key;       /* входящий ключ запроса                */
  u long       rezcode;       /* результат выполнения запроса         */
  u long       obj_sz;        /* размер полученной информации [OPT]   */
  u long       obj_frmt;      /* формат полученной информации [OPT]   */
  u char       tmpkey[8];     /* ключ временной памяти        [OPT]   */
  u char       grif;          /* Гриф                                 */
  u char       rezerv[11];
} JSYNCOUT;
typedef JSYNCOUT   *PJSYNCOUT;

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ИСХОДЯЩИХ ЗАПРОСОВ СИСТЕМНОГО ОБМЕНА
***********************************************************************/
/* Определяем элементы журнала                                        */
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
  u long       number;        /* порядковый номер записи в журнале    */
  char         state;         /* состояние записи                     */
  u char       code;          /* кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* время создания записи                */
  IDT          modify_dt;     /* время корректировки записи           */
  IDT          send_dt;       /* дата и время отправки запроса        */
  IDT          dov_dt;        /* дата и время доведения запроса       */
  IDT          obrb_dt;       /* дата и время постановки на обработку */
  IDT          obr_dt;        /* дата и время обработки запроса       */
  SYS          query;         /* Собственно запрос                    */
  u char       mykey[PLOTKEYSIZE]; /* исходящий ключ                  */
  u char       farkey[PLOTKEYSIZE];/* входящий ключ                   */
  u char       la_pol[4];     /* лог. адрес получателя                */
  long         result;        /* Результат выполнения                 */
  u char       grif;          /* Гриф                                 */
  u char       rezerv[15];
} JSYSOUT;
typedef JSYSOUT   *PJSYSOUT;

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ВХОДЯЩИХ ЗАПРОСОВ СИСТЕМНОГО ОБМЕНА
***********************************************************************/
/* Определяем элементы журнала                                        */
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
  u long       number;        /* порядковый номер записи в журнале    */
  char         state;         /* состояние записи                     */
  u char       code;          /* кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* время создания записи                */
  IDT          modify_dt;     /* время корректировки записи           */
  IDT          recv_dt;       /* дата и время получения запроса       */
  IDT          obrb_dt;       /* дата и время постановки на обработку */
  IDT          obr_dt;        /* дата и время обработки запроса       */
  SYS          query;         /* Собственно запрос                    */
  u char       mykey[PLOTKEYSIZE]; /* входящий ключ                   */
  u char       farkey[PLOTKEYSIZE];/* исходящий ключ                  */
  u char       la_otpr[4];    /* лог. адрес отправителя               */
  long         result;        /* Результат выполнения                 */
  u char       grif;          /* Гриф                                 */
  u char       rezerv[15];
} JSYSIN;
typedef JSYSIN   *PJSYSIN;

/* Элемент поиска или замены                                          */
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
    ПРЕФИКС ТАБЛИЦЫ В ФАЙЛЕ
***********************************************************************/
typedef struct _STDTPREF {
  u long    stdldata;            /* Длина данных файла                */
  u long    stdlpref;            /* Длина префикса                    */
  u long    stdlstr;             /* Длина строки в таблице            */
  union {
    ADRESS   a;
    u long   l;
  }         std_lm;              /* Адрес получателя (4б)             */
  u char    std_code;            /* Кодировка                         */
  u char    std_rez[15];         /* Резерв                            */
} STDTPREF;                      /* это строка                        */
typedef STDTPREF   *PSTDTPREF;   /* указатель на строку               */

/**********************************************************************
    СТРУКТУРА PING
***********************************************************************/
typedef struct _PING {
  void    *pingadr;               /* Адрес строки в SLVADR' baulа      */
  u long   task_pid;              /* PID задачи                        */
  char     rezerv[72];
} PING;                          /* это строка                        */
typedef PING   *PPING;           /* указатель на строку               */

/**********************************************************************
    СТРУКТУРА УВЕДОМЛЕНИЯ
***********************************************************************/
typedef struct _NTFSTR {
  char       kl_obj[3];     /* Класс объекта  "DBO"- для БД           */
  char       dborezim;      /* Режим работы с объектами класса DBO    */
  char       rezerw1[4];    /* REZERW                                 */
  u long     adr_otpr;      /* Идентификатор модуля производившего    */
                            /* действие над объектом заданного класса */
  char       kl_dok[128];   /* Имя(Идентификатор) объекта             */
  IDT        ntfidt;        /* время и операция над объектом          */
  char       rezerw2[12];   /* REZERW                                 */
  char       group_name[4]; /* имя группы задач                       */
  char       task_name[4];  /* имя задачи                             */
  char       data_format[8];/* формат польз. данных (признак наличия) */
  char       rezerv3[72];   /* REZERW                                 */
} NTFSTR;                   /* это строка                             */
typedef NTFSTR  *PNTFSTR;   /* указатель на строку                    */

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

/* Свойства ЭО                                                        */
/* Определяем типы ЭО                                                 */
#define   UP_FTP_I_DISK         0   /* Диск                           */
#define   UP_FTP_I_VOL          1   /* Том                            */
#define   UP_FTP_I_LIB_ND       2   /* Библиотечный набор             */
#define   UP_FTP_I_SOLID_ND     3   /* Прямой набор                   */
#define   UP_FTP_I_POSLED_ND    4   /* Последовательный набор         */
#define   UP_FTP_I_LIB_PART     5   /* Раздел библиотеки              */
#define   UP_FTP_I_FILE         6   /* Файл                           */
#define   UP_FTP_I_DIR          7   /* Директория                     */
#define   UP_FTP_I_PLOT         8   /* Хранилище данных UP            */

/* Определяем форматы записей ЭО                                      */
#define   UP_FTP_RECF_FIX      2    /* Фиксированная                  */
#define   UP_FTP_RECF_BLOCK    4    /* Блокированная                  */
#define   UP_FTP_RECF_UNKN     8    /* Неизвестная                    */
#define   UP_FTP_RECF_VARY     16   /* Переменная                     */
#define   UP_FTP_RECF_OVER     32   /* Переходящая                    */
typedef struct _FTP_I_PROP {
  char         type;        /* тип ЭО                                */
  char         is_folder;   /* Есть ли дочерние элементы (true/false)*/
  char         rec_format;  /* Формат записи (УРС)                   */
  char         n_remain_bl; /* Количество блоков на 1-м треке библ НД*/
  u long       size;        /* младшие 4 байта размера               */
  IDT          create_dt;   /* дата создания                         */
  IDT          modify_dt;   /* дата изменения                        */
  IDT          read_dt;     /* дата чтения                           */
  u long       rec_size;    /* Размер записи (УРС)                   */
  u long       blk_size;    /* Размер блока (УРС)                    */
  u short      nrec_blk;    /* Кол-во записей в блоке НД             */
  u short      nblk_trk;    /* Кол-во блоков НД на треке диска       */
  u long       sblockttr;   /* Адрес первого блока раздела библ. НД  */
  u long       major_size;  /* старшие 4 байта размера               */
  u short      key_ofs;     /* Смещение позиции ключа прямого НД     */
  u short      key_len;     /* Длина ключа прямого НД                */
  u char       itemgrif;    /* Категория (гриф) ЭО                    */
  char         reserv2[7];
} FTP_I_PROP;
typedef FTP_I_PROP   *PFTP_I_PROP;

/* Структура запроса FTP                                              */
typedef struct _UP_FTP {
  QUERYID      queryido;    /* идентификатор отправителя запроса (*)  */
  QUERYID      queryidp;    /* идентификатор получателя запроса  (*)  */
  u char       is_recode;   /* признак перекодировки (true/false)     */
  u char       regim;       /* режим обмена                      (*)  */
    char       pr_wait;     /* признак 0-не ждать, 1-ждать выполнения */
  u char       place;       /* б/о размещения информации         (*)  */
    void      *own_adr;     /* собственный адрес стр-ры соед-я   (*)  */
    char       reserv2[3];  /* резерв                                 */
  u char       linklast;    /* б/о признака последнего кадра          */
    long       rez_code;    /* признак результата выполнения     (*)  */
  u long       smesh;       /* рабочее смещение в ИО                  */
  u long       cb_req;      /* запрашиваемая длина                    */
  u long       cb_rez;      /* результирующая длина                   */
  u long       item_nsize;  /* размер имени ЭО (для rezim='L')        */

  u char       ftp_key[8];  /* Ключ записи ('G' и 'P' для SLONA)      */
  u long       block_first; /* Номер блока ('G' и 'P' для УРСа)       */
  u long       block_last;  /* Номер блока ('G' и 'P' для УРСа)       */
  u long       block_getn;  /* количество запрошенных блоков          */
  u long       block_putn;  /* количество полученных блоков           */

  u long       linkblkl;    /* длина кадра                       (*)  */
    long       linkblkn;    /* номер блока (кадра)               (*)  */
    void      *linknach;    /* адрес начала области данных       (*)  */
  u long       linkobll;    /* максимальная длина области данных (*)  */
  u long       linkinfl;    /* текущая длина инф. в обл. данных  (*)  */
  u long       item_numb;   /* количество ЭО (для rezim='L')          */

  FTP_I_PROP   itemprop;    /* свойства ЭО                            */
    char       name_old[256];/* Имя ЭО                                */
  u long       exitcode;
  char         reserv4[332];
} UP_FTP;
typedef UP_FTP   *PUP_FTP;

/* Имя ЭО                                                             */
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

/* Свойства/имя                                                       */
typedef struct _FTP_ITEM{
  FTP_I_PROP   itemprop;    /* свойства ЭО                            */
  char         itemname[256];/* UNI имя ЭО                            */
  char         modname[4];  /* ЛА                                     */
  QUERYID      open_id;
  char         rezerv[44];
} FTP_ITEM;
typedef FTP_ITEM   *PFTP_ITEM;

/**********************************************************************
      ОПИСАНИЕ СТРОКИ ЖУРНАЛА ИСХОДЯЩИХ ЗАПРОСОВ FTP
***********************************************************************/
/* Определяем коды запросов FTP                                       */
#define   JFO_QUERY_OPEN         2     /* открыть                     */
#define   JFO_QUERY_CLOSE        4     /* закрыть                     */
#define   JFO_QUERY_CREATE       8     /* создать                     */
#define   JFO_QUERY_RENAME      16     /* переименовать               */
#define   JFO_QUERY_DELETE      32     /* удалить                     */
#define   JFO_QUERY_READ        64     /* читать                      */
#define   JFO_QUERY_WRITE      128     /* записать                    */
#define   JFO_QUERY_RUN        256     /* запустить                   */
#define   JFO_QUERY_GETFILE    512     /* получить файл               */
#define   JFO_QUERY_PUTFILE   1024     /* записать файл               */

/* Определяем элементы журнала исходящих запросов FTP                 */
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
  u long       number;        /* порядковый номер записи в журнале    */
  char         state;         /* состояние записи                     */
  u char       code;          /* кодировка строки                     */
  u char       rezerv1[2];
  IDT          create_dt;     /* время создания записи                */
  IDT          modify_dt;     /* время корректировки записи           */
  IDT          send_dt;       /* дата отправки запроса                */
  QUERYID      my_key;        /* исходящий ключ запроса               */
  u long       querycod;      /* Код запроса                          */
  u char       sourcela[4];   /* логический адрес получателя запроса  */
  u char       objkey[256];   /* Имя ЭО                               */
  u char       newkey[256];   /* Новое Имя ЭО (Переименование)        */
  IDT          obr_dt;        /* дата получения ответа на запрос      */
  QUERYID      far_key;       /* входящий ключ запроса                */
  u long       rezcode;       /* результат выполнения запроса         */
  char         is_read;       /* производилось ли чтение              */
  char         is_write;      /* производилась ли запись              */
  u char       rezerv2[2];
  IDT          close_dt;      /* дата закрытия ЭО (Открытие)          */
  u char       grif;          /* Гриф                                 */
  u char       rezerv[31];
} JFTPOUT;
typedef JFTPOUT   *PJFTPOUT;

/***********************************************************************
     Описание структуры для вызова ассемблерной вставки, выполняющей
     команду CMS и помещающей данные в указанный буфер (используем при
     обработке запросов FTP протокола)
***********************************************************************/
typedef struct _OBMENFTP {
  char   *buffer;            /* Адрес, куда ассемблер помещает данные */
  u long  bufflen;           /* Размер буфера (данных)                */
  u long  minotvet;          /* Минимальная разрешенная длина ответа  */
  u long  cmdlen;            /* Длина выполняемой команды CMS         */
  char    cmscmd[74];        /* Сама команда CMS                      */
  u char  reserv[6];         /* Резерв                                */
} OBMENFTP;
typedef OBMENFTP  *POBMENFTP;

/***********************************************************************
              ОПИСАНИЕ СТРУКТУРЫ ДАННЫХ ДЛЯ ОТПРАВКИ
***********************************************************************/
typedef struct _MESSAGE_PARAMS{
  char   kvs[3];     /* Код вида сообщения                            */
  char   rezim;      /* Режим                                         */
  char   adrpol[4];  /* Адрес получателя                              */
  long   reftype;    /* Тип хранения данных                           */
  char  *data;       /* Адрес данных (или путь к файлу)               */
  long   datalen;    /* Длина данных                                  */
  char   pr_code;    /* Признак кодировки                             */
  u char numcode;    /* Номер кодировки                               */
  u char kat_sr;     /* Категория срочности                           */
  u char datagrif;   /* Гриф данных                                   */
  char  *format;     /* Формат данных                                 */
  char   msg_key[PLOTKEYSIZE]; /* Исходящий ключ сообщения            */

} MESSAGE_PARAMS;
typedef MESSAGE_PARAMS   *PMESSAGE_PARAMS;

/***********************************************************************
                 ОПИСАНИЕ СТРУКТУРЫ ДАННЫХ НА ПРИЕМЕ
***********************************************************************/
typedef struct _MESSAGE_PARSED{
  char      keytemp[PLOTKEYSIZE]; /* Ключ временной памяти            */
  char      adrotp[4];            /* Адрес получателя                 */
  char      kvs[3];               /* Код вида сообшения               */
  char      rezim;                /* Режим                            */
  char      format[8];            /* Формат данных                    */
  u char    kat_sr;               /* Категория срочности              */
  u char    datagrif;             /* Гриф данных                      */
  char      reserv[6];            /* Резерв                           */
} MESSAGE_PARSED;
typedef MESSAGE_PARSED   *PMESSAGE_PARSED;

/**********************************************************************
           ОПИСАНИЕ СТРУКТУРЫ СИНХРОННОГО ОБМЕНА СООБЩЕНИЯМИ
***********************************************************************/
typedef struct _SYNCOBMN {
  QUERYID      queryid;     /* Идентификатор запроса                  */
  u char       syncrazm;    /* Б/о размещения данных                  */
  u char       syncinft;    /* Б/о формата данных                     */
  u char       reserv1;     /* Резерв                                 */
  u char       synclast;    /* Признак последнего кадра               */
    void      *syncadr;     /* Собственный адрес стр-ры соед-я        */
    long       syncprz;     /* Признак результата выполнения          */
    void      *syncobl;     /* Адрес начала области данных            */
  u long       syncobll;    /* Размер области данных                  */
  u long       syncinfl;    /* Текущий размер инф-ции в обл. данных   */
  u long       syncblkl;    /* Размер блока (кадра)                   */
  u long       syncblkn;    /* Номер блока (кадра)                    */
  u char       syncvxod[8]; /* Входящий номер ответа                  */
  u char       syncgrif;    /* Гриф информации                        */
  u char       reserv2[103];/* резерв                                 */
} SYNCOBMN;
typedef SYNCOBMN  *PSYNCOBMN;

/**********************************************************************
             ОПИСАНИЕ СТРУКТУРЫ ПЕРЕДАЧИ ПАРАМЕТРОВ ФУНКЦИИ
                ОТПРАВКИ СООБЩЕНИЯ В СИНХРОННОМ РЕЖИМЕ
***********************************************************************/
typedef struct _SYNCPARM {
  u char   address[4];  /* Адрес получателя                           */
  u char   reserv1[4];  /* Резерв                                     */
  void    *pdata;       /* Указатель на отправляемые данные           */
  u long   datasize;    /* Размер отправляемых данных                 */
  void    *preplay;     /* Указатель на область памяти для ответа     */
  u long   replsize;    /* Размер ответа                              */
  u long   interval;    /* Интервал ожидания ответа                   */
  u char   dataplace;   /* Б/о местоположения данных                  */
  u char   datafrmt;    /* Б/о формата данных                         */
  u char   reserv2;     /* Резерв                                     */
  u char   rezim;       /* Режим отправки (запрос('Q') или ответ('R') */
  char     tempkey[PLOTKEYSIZE]; /* Ключ ответа во временной памяти   */
  u char   datakfs[3];  /* КФС отправляемых данных                    */
  u char   datagrif;    /* Гриф информации                            */
  u char   reserv3[20]; /* Резерв                                     */
} SYNCPARM;
typedef SYNCPARM  *PSYNCPARM;

/**********************************************************************
           ОПИСАНИЕ СТРУКТУРЫ ПЕРЕДАЧИ ИНФОРМАЦИИ О ТАБЛИЦЕ
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
                            АНАЛОГ СТРУКТУРЫ IDT
***********************************************************************/
typedef struct _IDH {   /* Структура : Дата - 16-е время              */
  u short       year;   /* Год                                        */
  u char        month;  /* Месяц                                      */
  u char        day;    /* День                                       */
  u char        hour;   /* Часы                                       */
  u char        min;    /* Минуты                                     */
  u char        sec;    /* Секунды                                    */
  u char        dec;    /* Десятые доли секунды                       */
} IDH;
typedef IDH   *PIDH;    /* Указатель на структуру IDH                 */

/***********************************************************************
       СТРУКТУРА ИНФОРМАЦИОННОЙ ЧАСТИ ПРОТОКОЛА ИЛС КОМПОНЕНТ ПСЗИ
***********************************************************************/
typedef struct _ICHP {
  u char    ichp_kvs[3];     /* КВС                                   */
  u char    ichp_rezim;      /* Режим запроса ('R' - регистрация)     */
  IDH       ichp_idh;        /* Дата-Время запроса                    */
  u char    ichp_aolm[4];    /* АО - ЛМ                               */
  u char    ichp_aovu[4];    /* АО - ВУ                               */
  u char    ichp_aplm[4];    /* АП - ЛМ                               */
  u char    ichp_apvu[4];    /* АП - ВУ                               */
  u char    ichp_id[2];      /* Идентификатор                         */
  u char    ichp_range[2];   /* Ранг                                  */
  u char    ichp_spec[18];   /* Спецификация                          */
  u short   ichp_codopr;     /* Код операции                          */
  u short   ichp_codres;     /* Код результата                        */
  u char    ichp_klq[8];     /* Ключ запроса                          */
  IDH       ichp_idha;       /* Дата-Время ответа                     */
  u char    ichp_kla[8];     /* Ключ ответа                           */
  u char    ichp_idsd[2];    /* Идентификатор СД                      */
  u char    ichp_rangesd[2]; /* Ранг СД                               */
  u char    ichp_specsd[18]; /* Спецификация СД                       */
  u char    ichp_katsd[1];   /* Категория СД                          */
  u char    ichp_rez[1];     /* Резерв                                */
  u char    ichp_inf[20];    /* Доп. информация                       */
  u char    ichp_rez3[17];   /* Резерв 3                              */
  u char    ichp_verup[5];   /* Номер версии UP                       */
  u char    ichp_ui[16];     /* Итого: 160 б.                         */
} ICHP;
typedef ICHP      *PICHP;

/**********************************************************************
      СТРУКТУРА ИНФОРМАЦИОННОЙ ЧАСТИ СООБЩЕНИЯ С МАТРИЦЕЙ ДОСТУПА
***********************************************************************/
typedef struct _MDM {
  u long    mdmlenght;       /* длина матрицы                         */
  u char    mdmrez1[8];      /* резерв 1                              */
  u long    mdmchksum;       /* контрольная сумма                     */
  u char    mdmrangdef[2];   /* ранг по умолчанию                     */
  u char    mdmrez2[2];      /* резерв 2                              */
  u long    mdmltitle;       /* длина заголовка                       */
  u char    mdmadmin[2];     /* код модуля администратора СЗИ         */
  u char    mdmaskuf[2];     /* код модуля администратора СКУФ        */
  u long    mdmdusert;       /* смещение таблицы пользователей        */
  u long    mdmdrangt;       /* смещение таблицы рангов               */
  u long    mdmdtrastdt;     /* смещение таблицы доверяемых модулей   */
  u long    mdmdtrastgt;     /* смещение таблицы доверяющих модулей   */
  u long    mdmdaddrst;      /* смещение таблицы разрешенных адресатов*/
  u long    mdmdremvblt;     /* смещение таблицы устройств и каталогов*/
                             /* для выгрузки и загрузки информации    */
  u long    mdmdprintt;      /* смещение таблицы устройств печати     */
  u long    mdmdacippt;      /* смещение таблицы разрешенных ip-портов*/
  u long    mdmdrdippt;      /* смещение таблицы РД к ip-портам       */
  u long    mdmdregevn;      /* смещение шкалы регистрируемых событий */
} MDM;
typedef MDM       *PMDM;

/**********************************************************************
      СТРУКТУРА ЗАГОЛОВКА ДЛЯ ТАБЛИЦ, ВХОДЯЩИХ В МАТРИЦУ ДОСТУПА
***********************************************************************/
typedef struct _TMDM {
/* Заголовок таблицы (общий для всех таблиц, входящих в матрицу)      */
  u long    tmdmlttltab;     /* длина заголовка                       */
  u long    tmdmlstrtab;     /* длина строки                          */
  u long    tmdmnstrtab;     /* количество строк                      */
  u long    tmdmlfld1;       /* длина первого поля строки             */
  u long    tmdmlfld2;       /* длина второго поля строки             */
} TMDM;
typedef TMDM      *PTMDM;

/**********************************************************************
      СТРУКТУРА ОБЛАСТИ ПАРАМЕТРОВ, ПЕРЕДАВАЕМЫХ МЕЖДУ ФУНКЦИЯМИ
      ПРОВЕРКИ ПОЛНОМОЧИЙ НА ВЫПОЛНЕНИЕ ЗАЯВЛЕННОЙ ОПЕРАЦИИ
***********************************************************************/
typedef struct _ACNT {
  u long    acntkod;         /* код заявленной операции               */
  char     *acntashkl;       /* указатель на битовой шкалу            */
  u long    acntnbobj;       /* номер проверяемого бита 1             */
  u long    acntnbnet;       /* номер проверяемого бита 2             */
  u long    acntnboth;       /* номер проверяемого бита 3             */
  u long    acntnbmy;        /* номер проверяемого бита 4             */
  u long    acntnbok;        /* номер проверяемого бита 5             */
  u long    acntnbno;        /* номер проверяемого бита 6             */
  bool      acntgrifset;     /* гриф ОД задан/не задан (TRUE/FALSE)   */
  u char    acntobjao[PLOT_MDMZUAF1]; /* адрес ЛМ, заявившего         */
  u char    acntmodao[PLOT_MDMZUAF2]; /* операцию над ОД              */
  u char    acntobjap[PLOT_MDMZUAF1]; /* адрес ЛМ, на кот. выполняется*/
  u char    acntmodap[PLOT_MDMZUAF2]; /* заявленная операция          */
  u char    acntpath[PLOT_MDMZLPUN]; /* имя каталога или устройства   */
  u char    acntgrif;        /* гриф объекта доступа                  */
  u char    acnttip;         /* тип операции или объекта доступа 1    */
  u char    acntkc;          /* тип операции или объекта доступа 2    */
  u char    acntrez[17];     /* резерв                                */
} ACNT;
typedef ACNT      *PACNT;

/**********************************************************************
                   ОПИСАНИЕ ВЫЧИСЛИТЕЛЬНОЙ УСТАНОВКИ
***********************************************************************/
typedef struct _VUSTATE {
  u char     modserv[80];      /* Назначение логического модуля       */
  u char     modname[4];       /* Имя логического модуля              */
  u char     reserv1[4];       /* Резерв                              */
  u char     num_vu[4];        /* Номер ВУ                            */
  u char     reserv2[4];       /* Резерв                              */
  u char     userfio[18];      /* Фамилия И.О. пользователя           */
  u char     userrank[10];     /* Должность пользователя              */
  u char     netname[80];      /* Наименование сети                   */
  u char     rezerv3[52];      /* Резерв                              */
} VUSTATE;                     /* Это строка ВУ                       */
typedef VUSTATE   *PVUSTATE;   /* Указатель на строку                 */
typedef VUSTATE    MVUSTATE[]; /* Это массив строк                    */
typedef MVUSTATE  *PMVUSTATE;  /* Это указатель на массив строк       */

/**********************************************************************
        СТРУКТУРА ПАРАМЕТРОВ ДЛЯ ФУНКЦИЙ ПОЛУЧЕНИЯ СПИСКОВ ВУ
***********************************************************************/
#define PLOT_NET_NO_OWNVU      0
#define PLOT_NET_YES_OWNVU     1
typedef struct _UPVIEWNET {
  void      *buffer;           /* Указатель на буфер, выделенный для  */
                               /* получения массива MVUSTATE          */
  u long     buffsize;         /* Размер буфера                       */
  u long     strnum;           /* Количество строк полученного        */
                               /* массива MVUSTATE                    */
  long       interval;         /* Время ожидания ответов на запросы   */
  char       tempkey[PLOTKEYSIZE]; /* Ключ временной памяти получен-  */
                               /* ного массива MVUSTATE               */
  u char     ownvuflag;        /* Флаг наличия строки описания своей  */
                               /* ВУ в массиве MVUSTATE               */
  u char     rplace;           /* Б/о местоположения полученного      */
                               /* массива MVUSTATE                    */
  u char     netid;            /* Идентификатор ЛВС                   */
  u char     rezerv;
  u char     objcode[2];       /* Код объекта                         */
  u char     reserv[34];       /* Резерв                              */
} UPVIEWNET;
typedef UPVIEWNET *PUPVIEWNET; /* указатель на структуру              */

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
  IDT           temadttm;     /* Дата и время события                 */
  long          reftype;      /* Тип хранения данных                  */
  char         *data;         /* Адрес данных (или путь к файлу, или  */
                              /* ключ временной памяти)               */
  u long        datalen;      /* Размер данных                        */
  char          address[4];   /* Адрес получателя                     */
  char          reserv1[4];   /* Резерв                               */
  u char        format;       /* Формат данных                        */
  char          datakat;      /* Категория данных (1 - 4)             */
  char          msgzag[55];   /* Заголовок сообщения                  */
  char          ist1[13];     /* Источник 1                           */
  char          ist2[13];     /* Источник 2                           */
  char          tem[3];       /* Тема                                 */
  char          subtem[3];    /* Подтема                              */
  char          reg1[3];      /* Регион/подрегион 1                   */
  char          reg2[3];      /* Регион/подрегион 2                   */
  char          reg3[3];      /* Регион/подрегион 3                   */
  char          reg4[3];      /* Регион/подрегион 4                   */
  char          reg5[3];      /* Регион/подрегион 5                   */
  char          keyword1[8];  /* Ключевое слово 1                     */
  char          keyword2[8];  /* Ключевое слово 2                     */
  char          keyword3[8];  /* Ключевое слово 3                     */
  char          keyword4[8];  /* Ключевое слово 4                     */
  char          keyword5[8];  /* Ключевое слово 5                     */
  char          keyword6[8];  /* Ключевое слово 6                     */
  char          tempkey[8];   /* Ключ сообщения во временной памяти   */
  char          reserv2[12];  /* Резерв                               */
} TEMASTR;
typedef TEMASTR      *PTEMASTR;

typedef struct _SZI {
  IDH       szi_idh;        /* Дата-Время запроса                     */
  u short   szi_codopr;     /* Код операции                           */
  u short   szi_codres;     /* Код результата                         */
  u char    szi_aolm[4];    /* АО - ЛМ                                */
  u char    szi_aovu[4];    /* АО - ВУ                                */
  u char    szi_aplm[4];    /* АП - ЛМ                                */
  u char    szi_apvu[4];    /* АП - ВУ                                */
  u char    szi_id[2];      /* Идентификатор                          */
  u char    szi_range[2];   /* Ранг                                   */
  u char    szi_spec[18];   /* Спецификация                           */
  u char    szi_idsd[2];    /* Идентификатор СД                       */
  u char    szi_rangesd[2]; /* Ранг СД                                */
  u char    szi_specsd[256]; /* Спецификация СД                       */
  u char    szi_katsd;      /* Категория СД                           */
  u char    szi_inf[80];    /* Доп. информация                        */
  u char    szi_rezim;      /* Режим запроса ('R' - регистрация)      */
  u char    szi_klq[PLOTKEYSIZE]; /* Ключ запроса                     */
} SZI;
typedef SZI      *PSZI;

typedef struct _SYS_USER_INFO {
  char      username[64];   /* Логин пользователя ОС                  */
  char      userinfo[128];  /* Описание пользователя ОС               */
  u char    usergrifmin;    /* Гриф пользователя (' ' - не задан)     */
  u char    usergrifmax;    /* Гриф пользователя (' ' - не задан)     */
  char      rezerv[2];
  char      sysgrif[256];   /* Мандатная метка ОС для пользователя    */
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

/* Словарь для рассылки сообщений                                     */
typedef struct _SLV21Y009 {
  u char    kvs[3];            /* код вида сообщения                  */
  u char    nm_field[8];       /* символическое имя поля или смещ-е   */
  u char    usl;               /* условие сравн-я (равно - не равно)  */
  u long    testsz;            /* сколько б. от начала поля сравнить  */
  u char    test[16];          /* с чем сравнить                      */
  u char    preproc;           /* признак препроцессорной обработки   */
  u char    is_null;           /* Базовая строка или нет              */
  u char    to_send[128];      /* Список адресов для рассылки (',')   */
  u char    rezerv[94];
} SLV21Y009;
typedef SLV21Y009   *PSLV21Y009;

/**********************************************************************
           ОПИСАНИЕ СТРУКТУРЫ СИНХРОННЫХ ЗАПРОСОВ К ВИТРИНАМ
***********************************************************************/
typedef struct _CASEOBMN {
  QUERYID      queryido;    /* Идентификатор отправителя запроса      */
  QUERYID      queryidp;    /* Идентификатор получателя запроса       */
  u char       caseinft;    /* Б/о формата данных                     */
  u char       caserezim;   /* Режим обмена                           */
  u char       casegrif;    /* Категория (гриф) информации            */
  u char       caserazm;    /* Б/о размещения данных                  */
    void      *caseadr;     /* Собственный адрес стр-ры соед-я        */
    long       caseprz;     /* Признак результата выполнения          */
    void      *caseobl;     /* Адрес начала области данных            */
  u long       caseobll;    /* Размер области данных                  */
  u long       caseinfl;    /* Текущий размер инф-ции в обл. данных   */
  u long       caseblkl;    /* Размер блока (кадра)                   */
  u long       caseblkn;    /* Номер блока (кадра)                    */
  u char       caselast;    /* Признак последнего кадра               */
  u char       reserv1[63]; /* Резерв                                 */
} CASEOBMN;
typedef CASEOBMN   *PCASEOBMN;

/* Определение элементов для работы с витринами данных UP             */
#define UP_CASE_MAXLEVELSNUM       16    /* Максимальное число уровней*/
                                         /* вложенности элементов на  */
                                         /* витрине                   */
#define UP_CASE_SUBJECTLEN         256   /* Максимальная длина        */
                                         /* наименования темы(подтемы)*/
                                         /* витрины (папки)           */
#define UP_CASE_SUBJECTSLISTSIZE   25700 /* Максимальный размер списка*/
                                         /* тем витрины               */
#define UP_CASE_SUBJECTSEPARATOR   "\1"  /* Разделитель тем в списке  */
#define UP_CASE_XML_CODE           PLOT_CODE_ANSI /* Кодировка,       */
                                         /* в которой хранятся        */
                                         /* XML-описания витрин       */
                                         /* (реестры витрин ЛМ)       */
#define UP_CASE_YNDEX_KEY          "Yndex"  /* Ключ документа-реестра */
                                         /* витрин ЛМ                 */
/**********************************************************************
                 ОПИСАНИЕ СТРУКТУРЫ ВИТРИНЫ ДАННЫХ UP
***********************************************************************/
typedef struct _UP_CASE {
    char       casename[256];    /* Имя витрины                       */
    char       case_nd[8];       /* Имя набора данных                 */
    char       reserv1[8];       /* Резерв                            */
    char       case_key[6];      /* Ключ витрины                      */
    char       reserv2[10];      /* Резерв                            */
  u char       m_keeper[4];      /* "Модуль-хранитель" витрины        */
  u char       reserv3[4];       /* Резерв                            */
  u char       m_creator[4];     /* "Модуль-создатель" витрины        */
  u char       reserv4[4];       /* Резерв                            */
    IDT        case_create;      /* Дата создания витрины             */
    IDT        case_change;      /* Дата корректировки витрины        */
  u char       case_fiousr[18];  /* ФИО "пользователя-создателя"      */
  u char       case_postusr[10]; /* Должность                         */
                                 /* "пользователя-создателя"          */
  u char       case_idusr[2];    /* Идентификатор                     */
                                 /* "пользователя-создателя"          */
  u char       casegrif;         /* Категория (гриф) витрины          */
  u char       reserv5[161];     /* Резерв                            */
} UP_CASE;
typedef UP_CASE     *PUP_CASE;

/**********************************************************************
                  ОПИСАНИЕ СТРУКТУРЫ ДЕСКРИПТОРА ВИТРИНЫ
***********************************************************************/
typedef struct _SHOWCASE {
  PUP_CASE     pup_case;      /* Указатель на UP_CASE витрины         */
  char        *sub_buff;      /* Указатель на список тем витрины      */
  u long       sub_size;      /* Размер списка тем витрины            */
  u char       reserv[20];    /* Резерв                               */
} SHOWCASE;
typedef SHOWCASE    *PSHOWCASE;   /* Указатель на структуру           */

/**********************************************************************
            ОПИСАНИЕ ССЫЛКИ НА ОБЪЕКТ ИЗ ВИТРИНЫ ДАННЫХ UP
***********************************************************************/
typedef struct _CASEXLINK {
  char         modname[4];   /* "Модуль-хранитель" объекта            */
  char         obj_path[512];/* Полный путь к объекту                 */
  char         obj_name[256];/* Имя (ключ) объекта                    */
  IDT          obj_date;     /* Дата корректировки объекта            */
  u short      obj_format;   /* Формат представления объекта          */
  u short      obj_arh;      /* Тип поколений объекта                 */
  char         obj_extn[256];/* Расширение имени объекта              */
  u short      extn_type;    /* Тип расширения имени объекта          */
  u short      extn_reserv;  /* Резервное поле                        */
} CASEXLINK;
typedef CASEXLINK   *PCASEXLINK;   /* Указатель на ссылку             */
typedef CASEXLINK    MCASEXLINK[]; /* Массив ссылок                   */
typedef MCASEXLINK  *PMCASEXLINK;  /* Указатель на массив ссылок      */

/* Определение типов элементов витрин данных UP                       */
#define UP_CASE_FOLDER             0     /* Папка                     */

#define UP_CASE_XLINK_DOCUMENT     10    /* Ссылка на документ ХД UP  */
#define UP_CASE_XLINK_TABLE        11    /* Ссылка на таблицу ХД UP   */
#define UP_CASE_XLINK_SHOWCASE     12    /* Ссылка на витрину UP      */
#define UP_CASE_XLINK_SEGMDOC      13    /* Ссылка на сегментированный*/
                                         /* документ ХД UP            */

#define UP_CASE_XLINK_SCRIPT       50    /* Сценарий UP               */

#define UP_CASE_XLINK_FILE         100   /* Ссылка на файл            */
#define UP_CASE_XLINK_EXEC         101   /* Ссылка на исполняемый     */
                                         /* модуль                    */

#define UP_CASE_XLINK_EXTERNAL     200   /* Внешняя ссылка (http, ftp */
                                         /* и т.п.)                   */

/* Определение типов поколений информационных объектов (способы       */
/* ведения архивов)                                                   */
#define IO_GENERATIONS_NO            0   /* Нет поколений (архивов)   */
#define IO_GENERATIONS_HOURLY        10  /* Ежечасно                  */
#define IO_GENERATIONS_DAILY         20  /* Ежесуточно                */
#define IO_GENERATIONS_WEEKLY        30  /* Еженедельно               */
#define IO_GENERATIONS_MONTHLY       40  /* Ежемесячно                */
#define IO_GENERATIONS_QUARTERLY     50  /* Ежеквартально             */
#define IO_GENERATIONS_YEARLY        60  /* Ежегодно                  */
#define IO_GENERATIONS_MODIFICATION  100 /* По факту изменения        */

/**********************************************************************
             ОПИСАНИЕ СТРУКТУРЫ ЭЛЕМЕНТА ВИТРИНЫ ДАННЫХ UP
***********************************************************************/
typedef struct _CASEITEM {
    char       item_index[UP_CASE_MAXLEVELSNUM][PLOTKEYSIZE];
    char       xlink_text[256]; /* Текст ссылки (имя папки)           */
    CASEXLINK  xlink;           /* Ссылка на объект                   */
    IDT        item_date;       /* Дата-время помещения элемента      */
                                /* на витрину                         */
  u short      xlink_type;      /* Тип ссылки (элемента витрины)      */
  u char       xlink_kat;       /* Категория (гриф) элемента витрины  */
    char       comment[209];    /* Поле примечаний (ключевых слов)    */
} CASEITEM;
typedef CASEITEM    *PCASEITEM;   /* Указатель на структуру           */
typedef CASEITEM     MCASEITEM[]; /* Массив структур                  */
typedef MCASEITEM   *PMCASEITEM;  /* Указатель на массив структур     */

/**********************************************************************
        ОПИСАНИЕ СТРУКТУРЫ ЭЛЕМЕНТА ВИТРИНЫ ДАННЫХ UP ТИПА "ПАПКА"
***********************************************************************/
typedef struct _CASEFOLDER {
  CASEITEM   folder_prop;  /* Свойства папки, как элемента витрины    */
  char       f_subject[UP_CASE_SUBJECTLEN];     /* Тема папки         */
  char       f_sub_subject[UP_CASE_SUBJECTLEN]; /* Подтема папки      */
  char       f_region[UP_CASE_SUBJECTLEN];      /* "Регион" папки     */
  char       f_sub_region[UP_CASE_SUBJECTLEN];  /* "Местность" папки  */
} CASEFOLDER;
typedef CASEFOLDER    *PCASEFOLDER;   /* Указатель на структуру       */
typedef CASEFOLDER     MCASEFOLDER[]; /* Массив структур              */
typedef MCASEFOLDER   *PMCASEFOLDER;  /* Указатель на массив структур */

/**********************************************************************
  ОПИСАНИЕ СТРУКТУРЫ ПАРАМЕТРОВ ДЛЯ ВЫЗОВОВ ФУНКЦИЙ РАБОТЫ С ВИТРИНАМИ
        ДАННЫХ UP (ЧТЕНИЕ ВИТРИНЫ, ЧТЕНИЕ РЕЕСТРА ВИТРИН ЛМ И Т.П.)
***********************************************************************/
#define UP_CASE_AS_STRUCTURE       0  /* Структурное представление    */
                                      /* содержимого витрины (реестра */
                                      /* витрин ЛМ)                   */
#define UP_CASE_AS_XML             1  /* Представление содержимого    */
                                      /* витрины (реестра витрин ЛМ)  */
                                      /* в формате XML                */
typedef struct _CASEWORK {
    void     *work_buff;     /* Указатель на буфер для данных         */
  u long      buff_size;     /* Размер буфера для данных              */
  u long      real_read;     /* Размер полученных данных              */
  u char      view_type;     /* Вид (формат) представления данных     */
                             /* (структурное, XML и т.д.)             */
  u char      reserv1;       /* Резерв                                */
  u char      bo_place;      /* Б/о размещения полученных данных      */
  u char      reserv2;       /* Резерв                                */
    long      interval;      /* Интервал ожидания окончания операции  */
  u char      data_grif;     /* Гриф полученных данных                */
  u char      reserv3[11];   /* Резерв                                */
} CASEWORK;
typedef CASEWORK    *PCASEWORK;   /* Указатель на структуру           */

/**********************************************************************/
/* КОНСТАНТЫ И СТРУКТУРА ДЛЯ ВЫПОЛНЕНИЯ SELECT К ТАБЛИЦАМ ХРАНИЛИЩА UP*/
/**********************************************************************/
#define UP_DINSELECTDELETE   2     /* Выбрать записи                  */
#define UP_DINSELECTTABLE    4     /* Удалить записи                  */
typedef struct _DINSELECT {
  u char    count;             /* Число полей                         */
  u char    namepol[80];       /* Имя поля                            */
  u char    usl[2];            /* Условие сравн-я (>,>=,<,<=,==,!=)   */
  u char    rezerv[7];         /* Резерв                              */
  u short   dlpol;             /* Длина поля                          */
  u char    typepol;           /* Тип поля                            */
  u char    typecon;           /* Тип отношения                       */
} DINSELECT;
typedef DINSELECT   *PDINSELECT;

/***********************************************************************
                     СТРУКТУРА IDS - СТРОКА С ДАТОЙ
***********************************************************************/
typedef struct _IDS {   /* Структура : Дата - 16-е время              */
  u char       day[2];  /* День                                       */
  u char       raz1;    /* .                                          */
  u char       month[2];/* Месяц                                      */
  u char       raz2;    /* .                                          */
  u char       year[4]; /* Год                                        */
  u char       raz3;    /*                                            */
  u char       hour[2]; /* Часы                                       */
  u char       raz4;    /* :                                          */
  u char       min[2];  /* Минуты                                     */
  u char       raz5;    /* :                                          */
  u char       sec[2];  /* Секунды                                    */
} IDS;
typedef IDS   *PIDS;    /* Указатель на структуру IDS                 */

/**********************************************************************
        СТРУКТУРА РАСШИРЕНИЯ СУ ДЛЯ ПЕРЕДАЧИ ЭЛЕМЕНТОВ ВИТРИНЫ В
                      АНАЛИТИЧЕСКУЮ ПОДСИСТЕМУ UP
***********************************************************************/
typedef struct _ALBACASE {
  u long    upcase_smech;     /* Смещение структуры UP_CASE от начала */
                              /* ALBACASE                             */
  u long    tema_smech;       /* Смещение списка тем витрины от начала*/
                              /* ALBACASE                             */
  u long    tema_size;        /* Размер списка тем витрины            */
  u long    items_smech;      /* Смещение списка элементов витрины от */
                              /* начала ALBACASE                      */
  u long    items_size;       /* Размер списка элементов витрины      */
  u char    reserv[44];       /* Резерв                               */
} ALBACASE;
typedef ALBACASE  *PALBACASE;    /* Указатель на структуру ALBACASE   */
typedef ALBACASE   MALBACASE[];  /* Это массив строк                  */
typedef MALBACASE  *PMALBACASE;  /* Это указатель на массив строк     */

/***********************************************************************
       СТРУКТУРА ИЗМЕНЯЕМЫХ ПАРАМЕТРОВ (СВОЙСТВ) ЭЛЕМЕНТА ВИТРИНЫ
***********************************************************************/
typedef struct _SC_ITEM_CHANGE {
  u long    if_item_name;     /* Признак изменения наименования       */
                              /* элемента витрины                     */
    char    item_name[256];   /* Новое значение наименования элемента */
                              /* витрины                              */
  u long    if_obj_mod;       /* Признак изменения имени модуля-
                              /* хранителя информационного объекта    */
    char    obj_module[4];    /* Новое значение имени модуля-хранителя*/
                              /* информационного объекта              */
    char    reserv1[4];       /* Резерв                               */
  u long    if_obj_path;      /* Признак изменения пути к             */
                              /* информационному объекту              */
    char    obj_path[512];    /* Новое значение пути к информационному*/
                              /* объекту                              */
  u long    if_obj_name;      /* Признак изменения имени (ключа)      */
                              /* информационного объекта              */
    char    obj_name[256];    /* Новое значение имени (ключа)         */
                              /* информационного объекта              */
  u long    if_obj_date;      /* Признак изменения даты корректировки */
                              /* информационного объекта              */
    IDT     obj_date;         /* Новое значение даты корректировки    */
                              /* информационного объекта              */
  u long    if_obj_format;    /* Признак изменения значения формата   */
                              /* представления информационного объекта*/
  u short   obj_format;       /* Новое значение формата представления */
                              /* информационного объекта              */
    char    reserv2[2];       /* Резерв                               */
  u long    if_obj_arh;       /* Признак изменения типа поколений     */
                              /* информационного объекта              */
  u short   obj_arh;          /* Новое значение типа поколений        */
                              /* информационного объекта              */
    char    reserv3[2];       /* Резерв                               */
  u long    if_obj_extn;      /* Признак изменения расширения имени   */
                              /* информационного объекта              */
    char    obj_extn[256];    /* Новое значение расширения имени      */
                              /* информационного объекта              */
  u long    if_obj_extn_type; /* Признак изменения типа расширения    */
                              /* имени информационного объекта        */
  u short   obj_extn_type;    /* Новое значение типа расширения имени */
                              /* информационного объекта              */
    char    reserv4[2];       /* Резерв                               */
  u long    if_xlink_type;    /* Признак изменения типа ссылки        */
                              /* на информационный объект             */
  u short   xlink_type;       /* Новое значение типа ссылки на        */
                              /* информационный объект                */
    char    reserv5[2];       /* Резерв                               */
  u long    if_item_keywords; /* Признак изменения перечня ключевых   */
                              /* слов элемента витрины                */
    char    item_keywords[209]; /* Новый перечень ключевых слов       */
                              /* элемента витрины                     */
    char    reserv6[7];       /* Резерв                               */
  u long    if_item_subject;  /* Признак изменения наименования темы  */
                              /* (для элементов типа "папка")         */
    char    item_subject[256]; /* Новое значение наименования темы    */
  u long    if_item_sub_subject; /* Признак изменения наименования    */
                              /* подтемы (для элементов типа "папка") */
    char    item_sub_subject[256]; /* Новое значение наименования     */
                              /* подтемы                              */
  u long    if_item_region;   /* Признак изменения наименования       */
                              /* региона (для элементов типа "папка") */
    char    item_region[256]; /* Новое значение наименования подтемы  */
  u long    if_item_sub_region; /* Признак изменения наименования     */
                              /* местности (для элементов типа        */
                              /* "папка")                             */
    char    item_sub_region[256]; /* Новое значение наименования      */
                              /* местности                            */
  u long    if_obj_reserv;    /* Признак изменения значения резервного*/
                              /* поля для объекта                     */
  u short   obj_reserv;       /* Новое значение резервного поля для   */
                              /* объекта                              */
    char    reserv7[2];       /* Резерв                               */
  u char    reserv[452];      /* Общий резерв                         */
} SC_ITEM_CHANGE;
typedef SC_ITEM_CHANGE  *PSC_ITEM_CHANGE; /* Указатель на структуру   */
typedef SC_ITEM_CHANGE   MSC_ITEM_CHANGE[]; /* Это массив строк       */
typedef MSC_ITEM_CHANGE  *PMSC_ITEM_CHANGE; /* Указатель на массив    */

/***********************************************************************
                     СТРУКТУРА, ОПИСЫВАЮЩАЯ СЦЕНАРИЙ
***********************************************************************/
typedef struct _SCR_PARAMS {
  char    adr[4];        /* Адрес модуля-исполнителя сценария         */
  char    rezerw1[4];    /* Резерв                                    */
  char    scrpath[PLOTPATHSIZE];   /* Локатор сценария в              */
                                   /* модуле-хранилище, пример:       */
                         /*  #AA$$/scr_doc  - DBO                     */
                         /*  &AA$$/C/script - FTP                     */
  DTS     scrstart;      /* Время запуска сценария на исполнение      */
                         /*         {0} - без ожидания                */
  u char  scrkey[PLOTKEYSIZE]; /*Идентификатор отправленного сообщения*/
  void    *data;         /* Адрес буфера для протокола (синх вызов)   */
  u long  datalen;       /* Размер буфера для протокола (синх вызов)  */
  u char  loginf;        /* Байт-описатель протокола                  */
                         /*         'P' - протокол необходим          */
                         /*   (SYSR с протоколом)                     */
  u char  razmesh;       /* Байт-описатель размещения протокола       */
                         /*   (синх вызов)                            */
                         /*  'V' - протокол во временной памяти,      */
                         /*  в буфере data - ключ временной памяти    */
                         /*  '&' - протокол в буфере data             */
  char    rezerw2[2];    /* Резерв                                    */
  long    interval;      /* Интервал ожидания (синх вызов)            */
                         /*          0 - асинхронный вызов функции    */
  char    rezerw3[24];   /* Резерв                                    */
} SCR_PARAMS;
typedef SCR_PARAMS *PSCR_PARAMS;  /* Указатель на структуру           */

/***********************************************************************
                     СТРУКТУРА, ОПИСЫВАЮЩАЯ СОБЫТИЕ
***********************************************************************/
typedef struct _VNT_PARAMS{
  u char   vnt_id[4];        /* Идентификатор события                 */
  u char   rezerw1[4];       /* Резерв 1                              */
  u char   group_name[4];    /* Имя группы задач                      */
  u char   task_name[4];     /* Имя задачи                            */
  u char   data_format[8];   /* Формат польз. данных (признак наличия)*/
  u char   uscr_path[PLOTPATHSIZE]; /* Локатор сценария приложения    */
  u char   rezerw2[96];      /* Резерв 2                              */
  u long   rezerw3[16];      /* Резерв 3                              */
  u short  rezerw4[32];      /* Резерв 4                              */
} VNT_PARAMS;
typedef VNT_PARAMS *PVNT_PARAMS;  /* указатель на структуру           */
/***********************************************************************
       ИНФОРМАЦИЯ О ПРИЛОЖЕНИИ, ИЗВЕЩАЮЩЕМ О НАСТУПЛЕНИИ СОБЫТИЯ
***********************************************************************/
typedef struct _TASK_PARAMS{
  char   group_name[4];    /* Имя группы задач                        */
  char   task_name[4];     /* Имя задачи                              */
  char   data_format[8];   /* Формат польз. данных (признак наличия)  */
  char   rezerw[48];       /* Резерв                                  */
} TASK_PARAMS;
typedef TASK_PARAMS *PTASK_PARAMS;  /* указатель на структуру         */
/***********************************************************************
         СТРУКТУРА, ОПИСЫВАЮЩАЯ УПРАВЛЯЮЩИЙ СЛОВАРЬ СЦЕНАРИЕВ
***********************************************************************/
typedef struct _SLVVNT {
  char   vnt_id[4];             /* Идентификатор события              */
  char   vnt_react;             /* Разрешение на реакцию              */
  char   react_type;            /* Идентификатор типа реакции         */
  char   adr_isp[4];            /* Адрес исполнителя                  */
  char   react_id[PLOTPATHSIZE];/* Идентификатор реации               */
  DTS    vnt_start;             /* Время запуска реакции              */
  char   rezerw1[40];           /* Резерв 1                           */
  char   rezerw2[40];           /* Резерв 2                           */
} SLVVNT;
typedef SLVVNT   *PSLVVNT;          /* указатель на строку            */
typedef SLVVNT    MSLVVNT[];        /* это массив строк - вся таблица */
typedef MSLVVNT  *PMSLVVNT;         /* это указатель на таблицу       */

/***********************************************************************
         СТРУКТУРА ДАННЫХ СИСТЕМЫ ГАРАНТИРОВАННОЙ АУТЕНТИФИКАЦИИ
                         ПОЛЬЗОВАТЕЛЯ В СООБЩЕНИИ
***********************************************************************/
typedef struct _DIGSIGN {
  short   NumVersion;             /* Номер версии                     */
  char    reserv1[2];             /* Резерв                           */
  char    KodeObj[2];             /* Код объекта отправителя          */
  char    reserv2[2];             /* Резерв                           */
  char    KodeMod[2];             /* Код модуля отправителя           */
  char    reserv3[2];             /* Резерв                           */
  char    IdUser [2];             /* Идентификатор отправителя        */
  char    reserv4[2];             /* Резерв                           */
  char    Signature_r[128];       /* Две переменные, эта и следующая, */
  char    Signature_s[128];       /* Для хранения цифровой подписи    */
  char    Detail[32];             /* Поле для фамилии                 */
  char    reserv5[32];            /* Резерв                           */
} DIGSIGN;
typedef DIGSIGN    *PDIGSIGN;

/***********************************************************************
            СТРУКТУРА ЗАПИСИ СЛОВАРЯ КЛЮЧЕЙ СВОЕГО ОБЪЕКТА
***********************************************************************/
typedef struct _SSLVZKBDS {
  char    NumVersion[2];          /* Номер версии                     */
  char    reserv1[2];             /* Резерв                           */
  char    KodeObj[2];             /* Код объекта                      */
  char    reserv2[2];             /* Резерв                           */
  char    KodeMod[2];             /* Код модуля                       */
  char    reserv3[2];             /* Резерв                           */
  char    IdUser[2];              /* Идентификатор пользователя       */
  char    reserv4[2];             /* Резерв                           */
  char    PrivatKey[128];         /* Закрытый ключ                    */
  char    OpenKey_X[128];         /* Две переменные, эта и следующая, */
  char    OpenKey_Y[128];         /* для хранения открытого ключа     */
  char    Detail[32];             /* Поле для фамилии                 */
  char    reserv5[32];            /* Резерв                           */
  char    Status;                 /* Статус ключа                     */
                                  /*   a - действителен               */
                                  /*   c - скомпрометирован           */
                                  /*   o - просрочен                  */
  char    reserv6[3];             /* Резерв                           */
  char    ProductionTime[8];      /* Время выдачи ключа               */
  char    LifeTime[8];            /* Момент времени, до которого      */
                                  /* живет ключ                       */
} SSLVZKBDS;
typedef SSLVZKBDS    *PSSLVZKBDS;

/***********************************************************************
          СТРУКТУРА ЗАПИСЕЙ В СЛОВАРЕ КЛЮЧЕЙ ДРУГИХ ОБЪЕКТОВ
***********************************************************************/
typedef struct _SSLVOKBDS {
  char    NumVersion[2];          /* Номер версии                     */
  char    reserv1[2];             /* Резерв                           */
  char    KodeObj[2];             /* Код объекта                      */
  char    reserv2[2];             /* Резерв                           */
  char    KodeMod[2];             /* Код модуля                       */
  char    reserv3[2];             /* Резерв                           */
  char    IdUser [2];             /* Идентификатор пользователя       */
  char    reserv4[2];             /* Резерв                           */
  char    OpenKey_X[128];         /* Две переменные, эта и следующая, */
  char    OpenKey_Y[128];         /* Для хранения открытого ключа     */
  char    Detail[32];             /* Поле для фамилии                 */
  char    reserv5[32];            /* Резерв                           */
  char    Status;                 /* Статус ключа                     */
                                  /*   a - действителен               */
                                  /*   c - скомпрометирован           */
                                  /*   o - просрочен                  */
  char    reserv6[3];             /* Резерв                           */
  char    ProductionTime[8];      /* Время выдачи ключа               */
  char    LifeTime[8];            /* Момент времени, до которого      */
                                  /* живет ключ                       */
} SSLVOKBDS;
typedef SSLVOKBDS    *PSSLVOKBDS;

/***********************************************************************
              СТРУКТУРА ЗАПИСИ В СЛОВАРЕ ПОЛЬЗОВАТЕЛЕЙ АС
***********************************************************************/
typedef struct _SLMP {
  char    UserNameTh[64];      /* ФИО полное                          */
  char    UserNameSh[18];      /* ФИО сокращенное                     */
  char    reserv1[2];          /* Резерв 1                            */
  char    UserUniqueId[8];     /* УИП в организации                   */
  char    UserIdUP[2];         /* УИП в UP                            */
  char    reserv2[2];          /* Резерв 2                            */
  char    NameObjSh[32];       /* Наименование КСА сокращенное        */
  char    PostSh[32];          /* Сокращенное наименование должности  */
  char    EMail[64];           /* Адрес электронной почты             */
  char    OpenKey_X[128];      /* Две переменные, эта и следующая,    */
  char    OpenKey_Y[128];      /* для хранения открытого ключа        */
  char    Comment[32];         /* Примечание                          */
  char    reserv[256];         /* Общий резерв                        */
} SLMP;
typedef SLMP    *PSLMP;
typedef SLMP     MSLMP[];
typedef MSLMP   *PMSLMP;

/***********************************************************************
        СТРУКТУРА ЗАПИСИ В СЛОВАРЕ ШТАТНОГО РАСПИСАНИЯ ОРГАНИЗАЦИИ
***********************************************************************/
typedef struct _SLMS {
  char    PostTh[128];         /* Полное наименование должности       */
  char    PostSh[32];          /* Сокращенное наименование должности  */
  char    KodeMod[2];          /* Код логического модуля              */
  char    reserv1[2];          /* Резерв 1                            */
  char    PostTabNum[10];      /* Табельный номер                     */
  char    reserv2[2];          /* Резерв 2                            */
  char    EMail[64];           /* Адрес электронной почты             */
  char    Comment[24];         /* Примечание                          */
  char    reserv[248];         /* Общий резерв                        */
} SLMS;
typedef SLMS    *PSLMS;
typedef SLMS     MSLMS[];
typedef MSLMS   *PMSLMS;

/***********************************************************************
 СТРУКТУРА СВЯЗНОГО СПИСКА ДЛЯ СЛОВАРЯ ШТАТНОГО РАСПИСАНИЯ ОРГАНИЗАЦИИ
***********************************************************************/
typedef struct _ListSLMS {
  void      *NextElem;         /* Адрес следующего элемента данного   */
                               /* списка                              */
  PSLMS      AdrLMS;           /* Адрес таблицы со штатной структурой */
  u long     NumRec;           /* Число записей в таблице             */
  char       reserv[4];        /* Резерв                              */
} ListSLMS;
typedef ListSLMS    *PListSLMS;
typedef ListSLMS     MListSLMS[];
typedef MListSLMS   *PMListSLMS;

/***********************************************************************
             СТРУКТУРА ЗАПИСИ В СЛОВАРЕ - СПРАВОЧНИКЕ КСА
***********************************************************************/
typedef struct _SRDO {
  char    NameObjTh[128];      /* Имя объекта расширенное             */
  char    NameObjSh[32];       /* Имя объекта сокращенное             */
  char    ObjId[2];            /* Уникальный идентификатор объекта    */
  char    reserv1[2];          /* Резерв 1                            */
  char    LevelObj[4];         /* Уровень объекта в иерархии          */
  u long  TypeLMS;             /* Тип штатной структуры               */
  char    EMail[64];           /* Адрес электронной почты             */
  u long  TypeObj;             /* Тип объекта                         */
  long    Coordinates[4];      /* Координаты объекта                  */
  char    reserv[256];         /* Общий резерв                        */
} SRDO;
typedef SRDO    *PSRDO;
typedef SRDO     MSRDO[];
typedef MSRDO   *PMSRDO;

/***********************************************************************
         СТРУКТУРА ПАРАМЕТРОВ ЗАДАЧИ, УКАЗЫВАЕМЫХ РАЗРАБОТЧИКОМ
***********************************************************************/
typedef struct _USTASKINFO {
  u short       build;       /* Номер сборки приложения               */
  u char        major;       /* Номер версии приложения               */
  u char        minor;       /* Номер релиза приложения               */
  u char        design[16];  /* Данные разработчика приложения        */
  u char        online;      /* Признак (^=0) работоспособности       */
                             /* процесса                              */
  char          reserv[43];  /* Резерв                                */
} USTASKINFO;
typedef USTASKINFO   *PUSTASKINFO;

/**********************************************************************
               СТРУКТУРА СО СВОЙСТВАМИ РАБОЧЕЙ СТАНЦИИ
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

/*=========== О П Р Е Д Е Л Е Н И Е  С Т Р У К Т У Р (конец) =========*/

#ifdef WINNT    /* Для работы визуальных компонент                    */
  } /* namespace Plot */
  #if !defined(NO_IMPLICIT_NAMESPACE_USE)
  using namespace Plot;
  #endif
#endif /*WINNT*/

/*=========== О П Р Е Д Е Л Е Н И Е  Ф У Н К Ц И Й (начало) ==========*/

/* Функция аварийного завершения задачи                               */
#define    Fabnd                 1
PLOTLIB void WINAPI abnd(PSTKMPR pstk,
                         char   *message,
                         void   *data,
                         u long  sz_data);

/* Пpеобpазование count символов из стpоки string в число типа long   */
PLOTLIB long WINAPI atoln(char *string,
                          int   count);

/* Элемент строки с разделителями                                     */
char *get_element(char *wrk,
                  char *str,
                  int   ind,
                  char *t);

/* Формирует из массива длиной count, расположенного по адресу pmass  */
/* строку rezult типа char*                                           */
PLOTLIB long WINAPI coded(u char *rezult,
                          u char *pmass,
                          long    count);

/* Функция создания системного ресурса для монопольного управления    */
#define    FCreateEnq            2
PLOTLIB long WINAPI CreateEnq(PSTKMPR pstk,
                              void   *resname,
                              long    nmsz);

/* Формирует из массива длиной count, расположенного по адресу pmass  */
/* строку rezult типа char*                                           */
PLOTLIB long WINAPI decod(u char *rezult,
                          u char *pmass,
                          long    count);

/* Функция снятия монопольного управления                             */
#define    Fdeq                  3
PLOTLIB long WINAPI deq(PSTKMPR pstk,
                        void   *pmass,
                        long    count);

/* Функция получения дампа процесса                                   */
#define    Fdumpproc             4
PLOTLIB long WINAPI dumpproc(PSTKMPR pstk);

/* Функция взятия монопольного управления                             */
#define    Fenq                  5
PLOTLIB long WINAPI enq(PSTKMPR pstk,
                        void   *pmass,
                        long    count);

/* Функция освобождения блока памяти, зарезервированного ф-ей GetMem  */
#define    FFreeMem              6
PLOTLIB long WINAPI FreeMem(PSTKMPR pstk,
                            void   *addr_obl);

/* Функция получения даты/времени                                     */
PLOTLIB void WINAPI GetCurrentDateTime(IDT  apprx,
                                       PIDT pcur_dt);

/* Функция резервирования непрерывного блока памяти                   */
#define    FGetMem               7
PLOTLIB void* WINAPI GetMem(PSTKMPR pstk,
                            long    how);

/* Дать целое значение keyword из системного файла                    */
PLOTLIB long WINAPI GPFDecod(char *filename,
                             char *section,
                             char *keyword);

/* Дать целое значение keyword из системного файла                    */
PLOTLIB long WINAPI GPFInt(char *filename,
                           char *section,
                           char *keyword);

/* Дать стpоковое значение keyword из системного файла                */
PLOTLIB long WINAPI GPFString(char   *filename,
                              char   *section,
                              char   *keyword,
                              u char *buffer,
                              char    pr_end);

/* Функция получения результата последней выполненной операции        */
PLOTLIB long WINAPI GetRez();

/* Функция установки результата выполнения последней операции         */
PLOTLIB void WINAPI SetRez(int error);

/* Пpеобpазование числа src типа long в строку dst типа char*         */
PLOTLIB char* WINAPI ltoan(char *dst,
                           long  src);

/* Функция копирования памяти между процессами                        */
#define    Fobmn                 8
PLOTLIB long WINAPI obmn(PSTKMPR pstk,
                         u long  istpid,
                         void   *istoff,
                         u long  prmpid,
                         void   *prmoff,
                         u long  how);

/* Функция дерегистрации задачи                                       */
#define    Funregp               9
PLOTLIB long WINAPI unregp(PPCA ppca);

/* Функция регистрации задачи                                         */
#define    Fregp                 10
PLOTLIB long WINAPI regp(char    *pname,
                         PPCA    *pppca,
                         PSTKMPR *ppstk);

/* Функция регистрации потока                                         */
#define    Fregt                 11
PLOTLIB long WINAPI regt(PPCA     ppca,
                         PSTKMPR *ppstk);

/* Функция запуска процесса                                           */
#define    Fstpr                 12
PLOTLIB long WINAPI stpr(PSTKMPR pstk,
                         char   *procname,
                         char   *cmndline,
                         char   *workdir,
                         u long *pid);

/* Функция запуска потока                                             */
#define    Fstthr                13
PLOTLIB long WINAPI stthr(PSTKMPR pstk,
                          void   *func,
                          void   *param,
                          u long *tid);

/* Функция удалени лидирующих и завершающих пробелов                  */
PLOTLIB void WINAPI trim(u char *string);

/* Функция получения ID текущего потока                               */
PLOTLIB long WINAPI GetCurTID();

/* Функция дерегистрации потока                                       */
#define    Funregt               14
PLOTLIB long WINAPI unregt(PSTKMPR pstk);

/* Функция добавления записи в очередь                                */
#define    Fvivd                 15
PLOTLIB long WINAPI vivd(PSTKMPR pstk,
                         char    lname[4],
                         void   *msgplace,
                         u long  msgsize);

/* Функция чтения записи из очереди                                   */
#define    Fvvod                 16
PLOTLIB long WINAPI vvod(PSTKMPR pstk,
                         char   *tsksend,
                         void   *msgplace,
                         u long *msgsize);

/* Функция подключения к БД                                           */
#define    Flinkm                17
PLOTLIB long WINAPI linkm(PSTKMPR pstk,
                          LINK   *ph,
                          u char  modname[4],
                          long    interval);

/* Функция перекодировки данных по КФСу в строке указаний             */
#define    Fdecodkfs             18
PLOTLIB long WINAPI decodkfs(PSTKMPR pstk,
                             u char  codfrom,
                             u char  codto,
                             u char *block,
                             u long  lblock);

/* Функция перекодировки                                              */
#define    Fdecoder              19
PLOTLIB long WINAPI decoder(PSTKMPR pstk,
                            u char  codfrom,
                            u char  codto,
                            u char *blok,
                            u long  lblok);

/* Функция постирования события                                       */
#define    Fpost_event           20
PLOTLIB long WINAPI post_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count);

/* Функция ожидания события                                           */
#define    Fwait_event           21
PLOTLIB long WINAPI wait_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count,
                               long    interval);

/* Функция отключения от БД                                           */
#define    Funlinkm              22
PLOTLIB long WINAPI unlinkm(PSTKMPR pstk,
                            LINK ph,
                            long interval);

/* Функция сегментированного многопакетного обмена инф. с БД          */
#define    Fobms                 23
#define S_INF               128  /* информационный сегмент            */
#define S_2                  64  /* сегмент 2                         */
#define S_3                  32  /* сегмент 3                         */
#define S_STRUCT             16  /* сегмент описания структуры        */
#define S_5                   8  /* сегмент 5                         */
#define S_6                   4  /* сегмент 6                         */
#define S_7                   2  /* сегмент 7                         */
#define S_8                   1  /* сегмент 8                         */
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

/* Функция формирования полного имени файла (с путем)                 */
PLOTLIB void WINAPI makepath(char *name,
                             char *path);

/* Функция получения ID текущего процесса                             */
PLOTLIB long WINAPI GetCurPID();

/* Функция приостановки выполнения потока                             */
#define    Fdelay                24
#ifdef VMESA
  #pragma map(delay, "DELAYTH ")
#endif /* VMESA */
PLOTLIB long WINAPI delay(PSTKMPR pstk,
                          int     how);

/* Функция инициализации задачи                                       */
#define    Finitproc             25
PLOTLIB long WINAPI initproc(PSTKMPR pstk,
                             char   *procname,
                             IDT     time);

/* Функция принудительного завершения потока                          */
#define    Fdelthr               26
PLOTLIB long WINAPI delthr(PSTKMPR pstk,
                           u long  thrtid,
                           void   *thrhndl);

/* Функция получения ID запроса                                       */
#define    FGetQueryID           27
PLOTLIB int WINAPI GetQueryID(PSTKMPR  pstk,
                              PQUERYID pqueryid);

/* Функция заполнения макета строки указания                          */
#define    FGetSU                28
PLOTLIB long WINAPI GetSU(PSTKMPR pstk,
                          PSU     psu);

/* Функция получения типа документа из FORMAT.SLV по расширению файла */
#define    FGetDocType           29
PLOTLIB long WINAPI GetDocType(PSTKMPR  pstk,
                               u char  *inftype,
                               u char  *extfile);


/* Функция обмена записями с БД                                       */
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

/* Функция обмена документами с БД                                    */
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

/* Функция занесения данных во временную память                       */
#define    Fsavetemp             32
PLOTLIB long WINAPI savetemp(PSTKMPR pstk,
                             long    type,
                             SU      su,
                             char   *su_ext,
                             u long  su_size,
                             char   *data,
                             u long  datasize,
                             char   *key);

/* Функция добавления данных во временную память                      */
#define    Faddtemp              33
PLOTLIB long WINAPI addtemp(PSTKMPR pstk,
                            PSU     psu,
                            char   *su_ext,
                            u long  su_size,
                            char   *key,
                            char   *data,
                            u long  datasize);

/* Функция чтения данных из временной памяти                          */
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

/* Функция удаления данных из временной памяти                        */
#define    Fdeltemp              35
PLOTLIB long WINAPI deltemp(PSTKMPR pstk,
                            char   *key);

/* Функция занесения файла во временную память                        */
#define    Fftotemp              36
PLOTLIB long WINAPI ftotemp(PSTKMPR pstk,
                            SU      su,
                            char   *su_ext,
                            u long  su_size,
                            char   *fname,
                            char   *key);

/* Функция получения размера данных во временной памяти               */
#define    Fsizetemp             37
PLOTLIB long WINAPI sizetemp(PSTKMPR pstk,
                             char   *key,
                             u long *datasize);

/* Функция проверки наличия потока, ожидающего событие                */
#define    Ftest_event           38
PLOTLIB long WINAPI test_event(PSTKMPR pstk,
                               void   *pmass,
                               long    count);

/* Функция получения ключа для строки указания                        */
#define    FGetGardKey           39
PLOTLIB long WINAPI GetGardKey(PSTKMPR pstk,
                               char   *key);

/* Функция получения расширения файла по типу документа из FORMAT.SLV */
#define    FGetFileExt           40
PLOTLIB long WINAPI GetFileExt(PSTKMPR  pstk,
                               u char  *inftype,
                               u char  *extfile);

/* Функция получения параметров формата по идентификатору формата     */
#define    FGetFormatInfo        41
PLOTLIB long WINAPI GetFormatInfo(PSTKMPR  pstk,
                                  u char  *inftype,
                                  FORMAT  *pformat);

/* Функция получения б/о перекодировки по расширению файла или        */
/* по типу документа из FORMAT.SLV                                    */
#define    FGetDocCodbo          42
PLOTLIB long WINAPI GetDocCodbo(PSTKMPR  pstk,
                                u char  *inftype,
                                u char  *extfile,
                                u char  *codbo);

/* Функция выдачи системного сообщения                                */
#define    FPlotMsgBox           43
#define PLOTMSGBOXERR            1   /* Сообщение об ошибке           */
#define PLOTMSGBOXWRN            2   /* Предупреждение                */
#define PLOTMSGBOXINF            3   /* Информация                    */
#define PLOTMSGBOXQUE            4   /* Вопрос                        */
PLOTLIB bool WINAPI PlotMsgBox(PSTKMPR pstk,
                               char   *message,
                               char    type);

/* Функция получения параметров данных во временной памяти            */
#define    Fquerytemp            44
PLOTLIB long WINAPI querytemp(PSTKMPR pstk,
                              char   *key,
                              PTMPHDR ptmphdr);

/* Функция преобразования структуры IDT в символьное представление    */
#define    FGetIDTStr            45
PLOTLIB long WINAPI GetIDTStr(PSTKMPR pstk,
                              PIDT    pidt,
                              PDTS    pdts);

/* Функция преобразования символьного представления даты              */
/* в структуру IDT                                                    */
#define    FGetStrIDT            46
PLOTLIB long WINAPI GetStrIDT(PSTKMPR pstk,
                              PIDT    pidt,
                              PDTS    pdts);

/* Функция получения содержимого ЖВС                                  */
#define    FGetIncome            47
PLOTLIB long WINAPI GetIncome(PSTKMPR  pstk,
                              JOURNIN *buffer,
                              u long   firstrec,
                              u long   nreqrec,
                              u long  *nretrec);

/* Функция получения содержимого ЖИС                                  */
#define    FGetOutgoing          48
PLOTLIB long WINAPI GetOutgoing(PSTKMPR   pstk,
                                JOURNOUT *buffer,
                                u long    firstrec,
                                u long    nreqrec,
                                u long   *nretrec);

/* Функция получения содержимого ЖИЗСО                                */
#define    FGetSOut              49
PLOTLIB long WINAPI GetSOut(PSTKMPR   pstk,
                            JSYNCOUT *buffer,
                            u long    firstrec,
                            u long    nreqrec,
                            u long   *nretrec);

/* Функция получения сегмента изменений ЖВС                           */
#define    FGetNewIncome         50
PLOTLIB long WINAPI GetNewIncome(PSTKMPR  pstk,
                                 JOURNIN *buffer,
                                 u long   buffsize,
                                 u long  *nretrec,
                                 IDT     *lasttime);

/* Функция получения сегмента изменений ЖИС                           */
#define    FGetNewOutgoing       51
PLOTLIB long WINAPI GetNewOutgoing(PSTKMPR   pstk,
                                   JOURNOUT *buffer,
                                   u long    buffsize,
                                   u long   *nretrec,
                                   IDT      *lasttime);

/* Функция получения сегмента изменений ЖИЗСО                         */
#define    FGetNewSOut           52
PLOTLIB long WINAPI GetNewSOut(PSTKMPR   pstk,
                               JSYNCOUT *buffer,
                               u long    buffsize,
                               u long   *nretrec,
                               IDT      *lasttime);

/* Функция копирования сообщения из временной памяти в файл           */
#define    Ftemptof              53
PLOTLIB long WINAPI temptof(PSTKMPR pstk,
                            char   *key,
                            PSU     psu,
                            char   *su_ext,
                            u long *su_size,
                            char   *fname);

/* Функция получения параметров расширения СУ                         */
#define    FGetExtSU             54
PLOTLIB long WINAPI GetExtSU(PSTKMPR pstk,
                             char   *kfs,
                             PKFSEXT pkfsext);

/* Функция получения признака аварии последней выполненной функции    */
#define    FGetPlotAvr           55
PLOTLIB long WINAPI GetPlotAvr(PSTKMPR pstk);

/* Функция получения признака результата последней выполненной функции*/
#define    FGetPlotRez           56
PLOTLIB long WINAPI GetPlotRez(PSTKMPR pstk);

/* Функция записи сообщения в журнал системных событий                */
#define    FSysJurn              57
PLOTLIB long WINAPI SysJurn(PSTKMPR pstk,
                            char   *message,
                            void   *data,
                            u long  sz_data);

/* Функция получения текущих параметров системы                       */
#define    FGetUPInfo            58
PLOTLIB long WINAPI GetUPInfo(PSTKMPR pstk,
                              UPINFO *pupinfo);

/* Функция получения текущих параметров задачи                        */
#define    FGetTaskInfo          59
PLOTLIB long WINAPI GetTaskInfo(PSTKMPR   pstk,
                                TASKINFO *ptaskinfo);

/* Функция открытия таблицы                                           */
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
/* Функция закрытия таблицы                                           */
#define    FCloseTable           61
PLOTLIB long WINAPI CloseTable(PSTKMPR   pstk,
                               LINK      op,
                               CURSOR   *opcur,
                               long      interval);

/* Функция перекодировки таблицы                                      */
#define    FDecodTab             62
PLOTLIB long WINAPI DecodTab(PSTKMPR   pstk,
                             CURSOR    opcur,
                             u char    rej1,
                             u char    rej2);

/* Функция фиксации изменения                                         */
#define    FWriteTable           63
PLOTLIB long WINAPI WriteTable(PSTKMPR   pstk,
                               LINK      op,
                               CURSOR    opcur,
                               void     *soobadr,
                               u long    sooblen,
                               char      priznak,
                               long      interval);

/* Функция удаления таблицы                                          */
#define    FDelTable             64
PLOTLIB long WINAPI DelTable(PSTKMPR  pstk,
                             LINK      op,
                             CURSOR   *opcur,
                             long      interval);

/* Функция сортировки таблицы                                         */
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

/* Функция отправки документа по асинхронному протоколу обмена        */
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

/* Функция записи в GARDOV                                            */
#define    FVcan                 81
PLOTLIB long WINAPI Vcan(PSTKMPR  pstk,
                         char    *slv_name,
                         u long   napr_sv,
                         PSU      psu,
                         u long   rezim, ...);

/* Функция инициализации гарантированого доведения                    */
#define    FInitKan              82
PLOTLIB long WINAPI InitKan(PSTKMPR pstk,
                            u long  trnsprt);

/* Функция получения словарей из B204                                 */
#define    FGetSLV               83
long GetSLV(PSTKMPR pstk,
            PSLVS   pslvs);

/* Функция освобождения словарей                                      */
#define    FFreeSLV              84
long FreeSLV(PSTKMPR pstk,
             PSLVS   pslvs);

/* Функция записи SLV в B204                                          */
#define    FSetSLV               85
long SetSLV(PSTKMPR pstk,
            PSLVS   pslvs);

/* Обработка результатов отправки данных транспортом                  */
#define    FScan                 86
PLOTLIB long WINAPI Scan(PSTKMPR  pstk,
                         u long   napr_sv,
                         u long   num_vu,
                         u long   trnsprt,
                         char    *adr_vu,
                         PSU      psu,
                         u long   priz_rez);

/* Функция выставления признака чтения сообщения из очереди           */
#define    FPrFalse              87
PLOTLIB long WINAPI PrFalse(PSTKMPR  pstk,
                            u long   napr_sv,
                            PSU      psu);

/* Функция разводки по транспортным задачам                         */
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

/* Функция перекодировки по динамически формируемой управляющей табли-*/
/* це перекодировки                                                   */
#define    FDinamDecoder         89
PLOTLIB long WINAPI DinamDecoder(void    *codtbl,
                                 u long   ctbsize,
                                 u char   codfrom,
                                 u char   codto,
                                 void    *data,
                                 u long   datasize,
                                 PCDTKFS  uprtbl,
                                 u long   utblstrn);

/* Функция перекодировки без стека                                    */
#define    FDecoder1             90
PLOTLIB long WINAPI Decoder1(void    *codtab,
                             u long   tabsize,
                             u char   codfrom,
                             u char   codto,
                             u char  *blok,
                             u long   lblok);

/* Функция уведомления                                                */
#define    FNotifyDoc            91
#define    NTF_OBJ_TYPE_DOC        1   /* Операция произведена над
                                          документом ХД UP            */
#define    NTF_OBJ_TYPE_SHOWCASE   2   /* Операция произведена над
                                          витриной данных UP          */
PLOTLIB long WINAPI NotifyDoc(PSTKMPR  pstk,
                              char    *log_adr_otp,
                              char    *log_adr_pol,
                              u char  *kl_dok,
                              PIDT     TimePlace,
                              u char   obj_type,
                              char     rezim);

/* Функция тестирования TEMPa                                         */
#define    FTestTemp             92
PLOTLIB long WINAPI TestTemp(PSTKMPR  pstk,
                             char    *keytemp);

/* Функция получения содержимого журналов                             */
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

/* Функция заполнения макета сообщения ЗВД                            */
#define    FGetZVD               94
PLOTLIB long WINAPI GetZVD(PSTKMPR pstk,
                           PZVD    pzvd);

/* Функция смены статуса канала                                       */
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

/* Функция получения оглавления                                       */
#define    FFtpGetToc            97
PLOTLIB long WINAPI FtpGetToc(PSTKMPR  pstk,
                              char    *mname,
                              char    *uni_name,
                              char    *data,
                              u long   cb_data_sz,
                              u long  *cb_data,
                              bool    *is_data_oob,
                              long     interval);

/* Функция открытия ЭО                                                */
#define    FFtpOpenItem         98
PLOTLIB long WINAPI FtpOpenItem(PSTKMPR  pstk,
                                char    *mname,
                                char    *uni_name,
                                void   **pftphi,
                                long     interval);

/* Функция закрытия ЭО                                                */
#define    FFtpCloseItem        99
PLOTLIB long WINAPI FtpCloseItem(PSTKMPR pstk, void *ftphi);

/* Функция удаления ЭО                                                */
#define    FFtpDelItem         100
PLOTLIB long WINAPI FtpDelItem(PSTKMPR  pstk,
                               char    *mname,
                               char    *uni_name,
                               long     interval);

/* Функция переименования ЭО                                          */
#define    FFtpRenItem         101
PLOTLIB long WINAPI FtpRenItem(PSTKMPR  pstk,
                               char    *mname,
                               char    *uni_name,
                               char    *new_name,
                               long     interval);

/* Функция чтения ЭО                                                  */
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

/* Функция получения свойств ЭО                                       */
#define    FFtpGetItemProps    103
PLOTLIB long WINAPI FtpGetItemProps(PSTKMPR      pstk,
                                    char        *mname,
                                    char        *uni_name,
                                    PFTP_I_PROP  pprops,
                                    long         interval);

/* Функция создания ЭО                                                */
#define    FFtpCreateItem      104
PLOTLIB long WINAPI FtpCreateItem(PSTKMPR     pstk,
                                  char       *mname,
                                  char       *uni_name,
                                  FTP_I_PROP  props,
                                  long        interval);

/* Функция чтения блока ЭО                                            */
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

/* Функция записи блока ЭО                                            */
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

/* Функция записи ЭО                                                  */
#define    FFtpWrite           107
PLOTLIB long WINAPI FtpWrite(PSTKMPR  pstk,
                             void    *ftphi,
                             char    *data,
                             u long   data_sz,
                             u long   smesh,
                             bool     need_recode,
                             u long  *cb_writen,
                             long     interval);

/* добавление строки в ЖИЗFTP                                         */
#define    FAddFTPOut          108
long AddFTPOut(PSTKMPR pstk,
               SU      su,
               UP_FTP param,
               char *data);

/* редактирование строки в ЖИЗFTP                                     */
#define    FSetFTPOut          109
long SetFTPOut(PSTKMPR  pstk,
             u long   src_mask,
             u long   set_mask,
             JFTPOUT  src_rec,
             JFTPOUT  set_rec);

/* Функция получения данных из ЖИЗFTP                                 */
#define    FGetFTPOut          110
PLOTLIB long WINAPI GetFTPOut(PSTKMPR   pstk,
                              JFTPOUT  *buffer,
                              u long    firstrec,
                              u long    nreqrec,
                              u long   *nretrec);

/* Функция получения обновленных данных из ЖИЗFTP                     */
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

/* Функция преобразования структуры IDT в 16-ричное представление     */
#define    FGetIDTHex          117
PLOTLIB long WINAPI GetIDTHex(PSTKMPR pstk,
                              PIDT    pidt,
                              PIDH    pidh);

/* Функция отправки сообщения в синхронном режиме                     */
#define    FSyncSendData       118
PLOTLIB long WINAPI SyncSendData(PSTKMPR  pstk, PSYNCPARM  pparm);

#ifndef VMESA
/* Функция получения таблицы с разделителями                          */
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

/* Функция преобразования таблицы в HTML вид                          */
/* (вызывается после GetTable)                                        */
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

/* Функция сегментированного многопакетного обмена инф. с БД          */
/* при обработке запросов ЗВД и замены документов                     */
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

/* Функция сжатия информации                                          */
#define    FCompress           124
long Compress(PSTKMPR    pstk,
              u char    *BufIst,
              u char    *BufPrm,
              u long     LenIst,
              u long    *LenPrm);

/* Функция расжатия информации                                        */
#define    FDecompress         125
long Decompress(PSTKMPR    pstk,
              u char    *BufIst,
              u char    *BufPrm,
              u long     LenIst,
              u long    *LenPrm);

/* Функция преобразования структуры IDH в символьное представление    */
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

/* Функция получения SNT своего модуля                                */
#define    FGetOwnSNT          129
long GetOwnSNT(PSTKMPR    pstk,
               void     **psnt,
               u long    *sntsize,
               u long    *sntreal);

/* Функция получения списка ВУ шлюзов ЛВС                             */
#define    FUP_GetGatewaysList 130
PLOTLIB long WINAPI UP_GetGatewaysList(PSTKMPR     pstk,
                                       PUPVIEWNET  pparm);

/* Функция получения списка ВУ указанной ЛВС                          */
#define    FUP_GetNetState     131
PLOTLIB long WINAPI UP_GetNetState(PSTKMPR     pstk,
                                   PUPVIEWNET  pparm);

/* Функция получения описания своей ВУ                                */
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

/* Функция отправки тематического сообщения                           */
#define    FSendTemaMsg        140
PLOTLIB long WINAPI SendTemaMsg(PSTKMPR     pstk,
                                PTEMASTR    pparm);

#define    FFormTemaHead       141
long FormTemaHead(PSTKMPR     pstk,
                  PTEMAHEAD   phead);

#define    FFtpStartProc       142
/* Функция запуска удаленного процесса                                */
PLOTLIB long WINAPI FtpStartProc(PSTKMPR   pstk,
                                 char     *mname,
                                 char     *uni_name,
                                 char      pr_wait,
                                 u long   *exitcode,
                                 long      interval);

#define    FSYS_ctask          143
/* Функция управления задачей                                         */
PLOTLIB long WINAPI SYS_ctask(PSTKMPR pstk,
                              char   *mname,
                              char   *taskname,
                              u char  req,
                              IDT    *time);

#define    FTestBit            144
/* Функция проверки значения бита в бит.шкале по его номеру (1,2,...) */
PLOTLIB long WINAPI TestBit(PSTKMPR pstk,
                            u long  NBit,
                            char   *PShkl,
                            u char *ZnBit);

#define    FTestTAdr           145
/* Функция проверки полномочий на выполнение операций ввода - вывода  */
long TestTAdr(PSTKMPR pstk, PACNT pacnt);

#define    FTestIO             146
/* Функция проверки полномочий на выполнение операций ввода - вывода  */
long TestIO(PSTKMPR pstk, PACNT pacnt);

#define    FTestDCFP           147
/* Функция проверки полномочий на выполнение операций над информацион-
   ными объектами БД, управление вычислительным процессом, доступ
   к файловой системе и ip-портам                                     */
long TestDCFP(PSTKMPR pstk, PACNT pacnt);

#define    FTestAcnt           148
/* Функция проверки полномочий пользователя на выполнение заявленной
   операции над объектом доступа                                      */
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

/* Функция инициализации очереди потока                               */
#define    FInitTLine          149
PLOTLIB long WINAPI InitTLine(PSTKMPR   pstk,
                              char     *name_thread,
                              void    **handline,
                              u long    linesize,
                              char      lineplace);

/* Функция опроса очереди потока                                      */
#define    FThrVvod            150
PLOTLIB long WINAPI ThrVvod(PSTKMPR   pstk,
                            void     *disline,
                            void     *Buffer,
                            u long   *SizeBuffer);

/* Функция записи сообщения в очередь потока                          */
#define    FThrVivd            151
PLOTLIB long WINAPI ThrVivd(PSTKMPR   pstk,
                            char     *name_thread,
                            void     *Buffer,
                            u long    SizeBuffer);

/* Добавление строки в ЖВЗSYS                                         */
#define    FAddSysIn           152
long AddSysIn(PSTKMPR pstk,
              SU      su,
              SYS     param);

/* Обновление строки в ЖВЗSYS                                         */
#define    FSetSysIn           153
long SetSysIn(PSTKMPR  pstk,
              u long   src_mask,
              u long   set_mask,
              JSYSIN   src_rec,
              JSYSIN   set_rec);

/* Получение новых строк из ЖВЗSYS                                    */
#define    FGetNewSysIn        154
PLOTLIB long WINAPI GetNewSysIn(PSTKMPR   pstk,
                                SRITEM   *filter,
                                u long    filter_sz,
                                JSYSIN    filter_rec,
                                JSYSIN   *buffer,
                                u long    buffsize,
                                u long   *nretrec,
                                IDT      *lasttime);

/* Запрос на удаление строки GARD                                     */
#define    FSYS_cgard          155
PLOTLIB long WINAPI SYS_cgard(PSTKMPR pstk,
                              char   *mname,
                              SU      str,
                              u char  req,
                              IDT    *time);

#define    FRegSbSrd           156
/* Функция рег-ции событий системы обеспечения безопасности инф-ции   */
long RegSbSrd(PSTKMPR  pstk, PSZI  pjrnb);

#define    FFtpCancel          157
/* Функция отмены блокирующей операции FTP                            */
PLOTLIB long WINAPI FtpCancel(PSTKMPR pstk,
                              long    rezcode);

#define    FOpenCurTable       158
/* Функция открытия курсора таблицы                                   */
PLOTLIB long WINAPI OpenCurTable(PSTKMPR   pstk,
                                 LINK      op,
                                 CURSOR   *opcur,
                                 char     *bd_name,
                                 char     *nd_name,
                                 char     *tb_name);

#define    FReceiveStamp       159
/* Функция получения грифа каталога(устройства) загрузки и выгрузки   */
/* информации                                                         */
PLOTLIB long WINAPI ReceiveStamp(PSTKMPR pstk,
                                 char   *PPath,
                                 char   *PStamp,
                                 char   *PAddrPoluch);
#ifdef WINNT
/* Функция копирования словарей и снятия контрольных сумм после
   обновления версии UP */
PLOTLIB void WINAPI NewVerCrc(void);
#endif /*WINNT*/

/* Получение директории файла                                         */
char *UP_GetDir(char *dir, char *file);

/* Получение директории файла в формате UP                            */
char *UP_UniGetDir(char *dir, char *uniname);

#define    FGetSysUserInfo     160
/* Функция получения информации о пользователе ОС                     */
PLOTLIB long WINAPI GetSysUserInfo(PSTKMPR         pstk,
                                   PSYS_USER_INFO  pinfo);

#define    FGetSysFileInfo     161
/* Функция получения информации о файле ОС                            */
PLOTLIB long WINAPI GetSysFileInfo(PSTKMPR pstk,
                                   char *fname,
                                   u char *pfilegrif);

#define    FSetSysFileInfo     162
/* Функция установки грифа файла ОС                                   */
PLOTLIB long WINAPI SetSysFileInfo(PSTKMPR pstk,
                                   char *fname,
                                   u char filegrif);

#define    FDefineUserLevel    163
/* Определение уровня субъекта доступа - пользователя                 */
long DefineUserLevel(PSTKMPR pstk,
                     char    priznak,
                     u char  shkl1,
                     u char  shkl2,
                     u char *retMinLevel,
                     u char *retMaxLevel);

#define    FSetTempGuard       165
/* Установка признака принадлежности данных в ТЕМРе ВП ПЛОТа          */
PLOTLIB long WINAPI SetTempGuard(PSTKMPR  pstk,
                                 char    *key,
                                 char     is_own);

#define    FGetXLinkType       167
/* Функция получения б/о типа ссылки                                  */
PLOTLIB long WINAPI GetXLinkType(PSTKMPR pstk,
                                 LINK    plink,
                                 u char *pXLink);

/* Функция преобразования пути каталога (устройства)                  */
/* из формата ОС в формат UP                                          */
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
/* Функция отправки запроса на печать информации                      */
PLOTLIB long WINAPI PRTInquiry(PSTKMPR pstk,
                               char   *key,
                               char   *addrprinter);

#define    FCreateShowcase        172
/* Функция создания витрины                                           */
PLOTLIB long WINAPI CreateShowcase(PSTKMPR   pstk,
                                   PUP_CASE  pcase,
                                   char     *subjects,
                                   long      interval);

#define    FDelShowcase           173
/* Функция удаления витрины                                           */
PLOTLIB long WINAPI DelShowcase(PSTKMPR   pstk,
                                PUP_CASE  pcase,
                                long      interval);

#define    FOpenShowcase          174
/* Функция открытия витрины                                           */
PLOTLIB long WINAPI OpenShowcase(PSTKMPR    pstk,
                                 PSHOWCASE *handler,
                                 char      *mod_name,
                                 char      *nd_name,
                                 char      *case_key,
                                 long       interval);

#define    FCloseShowcase         175
/* Функция закрытия витрины                                           */
PLOTLIB long WINAPI CloseShowcase(PSTKMPR    pstk,
                                  PSHOWCASE *handler,
                                  long       interval);

#define    FAddFolderOnShowcase   176
/* Функция добавления элемента "папка" на витрину                     */
PLOTLIB long WINAPI AddFolderOnShowcase(PSTKMPR     pstk,
                                        PSHOWCASE   handler,
                                        PCASEITEM   pfolder,
                                        char       *subject,
                                        char       *sub_subject,
                                        char       *region,
                                        char       *sub_region,
                                        long        interval);

#define    FAddXLinkOnShowcase    177
/* Функция добавления элемента "ссылка" на витрину                    */
PLOTLIB long WINAPI AddXLinkOnShowcase(PSTKMPR     pstk,
                                       PSHOWCASE   handler,
                                       PCASEITEM   pxlink,
                                       long        interval);

PLOTLIB long WINAPI up_log(PPCA ppca, char *fmt, ...);

/* Функция удаления элемента с витрины                                */
#define    FDelItemFromShowcase   178
PLOTLIB long WINAPI DelItemFromShowcase(PSTKMPR    pstk,
                                        PSHOWCASE  handler,
                                        PCASEITEM  pitem,
                                        long       interval);

/* Функция получения ЭО                                               */
#define    FFtpReadFile            179
PLOTLIB long WINAPI FtpReadFile(PSTKMPR  pstk,
                                char    *mname,
                                char    *uni_name,
                                char     bo_place,
                                char    *local_name,
                                bool     need_recode,
                                long     interval);

/* Функция записи ЭО                                                  */
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

/* Функция выпонения оператора SELECT к таблицам хранилища UP         */
/* (параметры задаются в массиве структу DINSELECT)                   */
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

/* Функция перекодировки структуры DINSELECT из кодировки задачи в    */
/* в кодировку хранилища UP                                           */
#define    FDecodSELECT           184
long DecodSELECT(PSTKMPR    pstk,
                 CURSOR     opcur,
                 void      *dinselect,
                 u long     lendinsel);

/* Функция перекодировки таблицы, находящейся во временной памяти     */
/* из кодировки хранилища UP в кодировку задачи                       */
#define    FDecTEMPTab            185
long DecTEMPTab(PSTKMPR pstk,
                CURSOR  opcur,
                char   *keytemp);

/* Функция выпонения оператора SELECT к таблицам хранилища UP         */
/* (параметры задаются в строке типа char*)                           */
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

/* Функция чтения витрины                                             */
#define    FReadShowcase          187
PLOTLIB long WINAPI ReadShowcase(PSTKMPR    pstk,
                                 PSHOWCASE  handler,
                                 PCASEWORK  pparm);

/* Функция получения XML-описания витрины                             */
#define    FGetXMLShowcase        188
long GetXMLShowcase(PSTKMPR    pstk,
                    PSHOWCASE  handler,
                    char      *list_temp,
                    u long     list_size,
                    char      *xml_temp,
                    u long    *xml_size);

/* Функция "конвертирования" структуры UP_CASE в XML-описание витрины */
#define    FGetXMLFromUPCASE      189
long GetXMLFromUPCASE(PSTKMPR    pstk,
                      PSHOWCASE  handler,
                      char      *xml_segm);

/* Функция "конвертирования" индекса элемента для XML-описания        */
#define    FGetXMLIndex           190
long GetXMLIndex(PSTKMPR    pstk,
                 PCASEITEM  pitem,
                 char      *str_index);

/* Функция преобразования 16-ричного представления даты-времени в     */
/* структуру IDT                                                      */
#define    FGetHexIDT             191
PLOTLIB long WINAPI GetHexIDT(PSTKMPR pstk,
                              PIDH    pidh,
                              PIDT    pidt);

/* Функция чтения реестра витрин логического модуля                   */
#define    FReadYndex             192
PLOTLIB long WINAPI ReadYndex(PSTKMPR    pstk,
                              char      *mod_name,
                              PCASEWORK  pparm);

/* Функция преобразования XML-представления реестра витрин ЛМ в       */
/* структурное                                                        */
#define    FGetStrYndex           193
long GetStrYndex(PSTKMPR    pstk,
                 char      *xml_temp,
                 u long     xml_size,
                 char      *str_temp,
                 u long    *str_size);

/* Функция регистрации витрины в реестре витрин логического модуля    */
#define    FRegShowcase           194
PLOTLIB long WINAPI RegShowcase(PSTKMPR    pstk,
                                PSHOWCASE  handler,
                                char      *mod_name,
                                long       interval);

/* Функция исключения витрины из реестра витрин логического модуля    */
#define    FUnregShowcase         195
PLOTLIB long WINAPI UnregShowcase(PSTKMPR    pstk,
                                  PSHOWCASE  handler,
                                  char      *mod_name,
                                  long       interval);

/* Функция выборки элементов витрины по заданным условиям             */
#define    FSelectFromShowcase    196
/* Определения имен параметров строки запроса                         */
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
/* Определения возможных операторов                                   */
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
/* Определения служебных слов, символов и дополнительных условий      */
#define SERVICE_SHOWCASE_ROOT     "ROOT"
#define SERVICE_ALL_ELEMENTS      "ALL"
#define SERVICE_LIKE_SYMBOL       "*"
#define SERVICE_STRING_SYMBOL     "'"
#define SERVICE_IN_BEGIN_SYMBOL   "("
#define SERVICE_IN_END_SYMBOL     ")"
#define SERVICE_IN_LIST_SYMBOL    ","
#define SERVICE_LETTER_CASE       "[!]"
/* Определения типов поколений информационных объектов                */
/* Нет поколений (архивов)                                            */
#define TYPE_GENERATIONS_NO            "NO_GENERATIONS"
/* Ежечасно                                                           */
#define TYPE_GENERATIONS_HOURLY        "HOURLY_GENERATIONS"
/* Ежесуточно                                                         */
#define TYPE_GENERATIONS_DAILY         "DAILY_GENERATIONS"
/* Еженедельно                                                        */
#define TYPE_GENERATIONS_WEEKLY        "WEEKLY_GENERATIONS"
/* Ежемесячно                                                         */
#define TYPE_GENERATIONS_MONTHLY       "MONTHLY_GENERATIONS"
/* Ежеквартально                                                      */
#define TYPE_GENERATIONS_QUARTERLY     "QUARTERLY_GENERATIONS"
/* Ежегодно                                                           */
#define TYPE_GENERATIONS_YEARLY        "YEARLY_GENERATIONS"
/* По факту изменения                                                 */
#define TYPE_GENERATIONS_MODIFICATION  "MODIFICATION_GENERATIONS"
/* Определения типов информационных объектов                          */
/* Документ ХД UP                                                     */
#define IO_TYPE_DOCUMENT     "IO_DOCUMENT"
/* Таблица ХД UP                                                      */
#define IO_TYPE_TABLE        "IO_TABLE"
/* Витрину UP                                                         */
#define IO_TYPE_SHOWCASE     "IO_SHOWCASE"
/* Сегментированный документ ХД UP                                    */
#define IO_TYPE_SEGMDOC      "IO_SEGMDOC"
/* Сценарий UP                                                        */
#define IO_TYPE_SCRIPT       "IO_SCRIPT"
/* Файл                                                               */
#define IO_TYPE_FILE         "IO_FILE"
/* Исполняемый модуль                                                 */
#define IO_TYPE_EXEC         "IO_EXEC"
/* Внешняя ссылка (http, ftp и т.п.)                                  */
#define IO_TYPE_EXTERNAL     "IO_EXTERNAL"
/* Прототип функции                                                   */
PLOTLIB long WINAPI SelectFromShowcase(PSTKMPR     pstk,
                                       PSHOWCASE   handler,
                                       PCASEWORK   pparm,
                                       char       *select_str);

/* Функция получения объекта, указанного в ссылке витрины             */
#define    FGetObjectFromXLink    197
PLOTLIB long WINAPI GetObjectFromXLink(PSTKMPR    pstk,
                                       PCASEWORK  pparm,
                                       PCASEITEM  pitem);

/* Функция чтения документа по ссылке витрины                         */
#define    FGetXlinkDocum         198
#ifdef VMESA
  #pragma map(GetXlinkDocum, "GETXLDOC")
#endif /* VMESA */
long GetXlinkDocum(PSTKMPR     pstk,
                   PCASEWORK   pparm,
                   char       *doc_path,
                   u short     doc_type);

/* Функция чтения файла по ссылке витрины                             */
#define    FGetXlinkFile          199
#ifdef VMESA
  #pragma map(GetXlinkFile, "GETXLFIL")
#endif /* VMESA */
long GetXlinkFile(PSTKMPR     pstk,
                  PCASEWORK   pparm,
                  char       *file_path);

/* Функция чтения таблицы по ссылке витрины                           */
#define    FGetXlinkTable         200
#ifdef VMESA
  #pragma map(GetXlinkTable, "GETXLTAB")
#endif /* VMESA */
long GetXlinkTable(PSTKMPR     pstk,
                   PCASEWORK   pparm,
                   char       *table_path);

/* Функция чтения витрины по ссылке витрины                           */
#define    FGetXlinkShowcase      201
#ifdef VMESA
  #pragma map(GetXlinkShowcase, "GETXLSC ")
#endif /* VMESA */
long GetXlinkShowcase(PSTKMPR     pstk,
                      PCASEWORK   pparm,
                      char       *showcase_path);

/* Функция чтения таблицы                                             */
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

/* Функция отправки запроса на выполнение сценария                    */
#define    FSendScr               203
long  SendScr(PSTKMPR  pstk, PSCR_PARAMS  pscrparm);

/* Функция извещения ядра о наступлении события                       */
#define    FSendEvent             204
PLOTLIB long WINAPI SendEvent(PSTKMPR      pstk,
                              PVNT_PARAMS  pvntparm,
                              void        *data,
                              u long       datasize,
                              long         dataplace,
                              u char       datagrif,
                              u char       dataformat,
                              u char       datacode);

/* Добавление строки в ЖИЗSYS                                         */
#define    FAddSysOut             205
long AddSysOut(PSTKMPR pstk,
               SU      su,
               SYS     param);

/* Обновление строки в ЖИЗSYS                                         */
#define    FSetSysOut             206
long SetSysOut(PSTKMPR  pstk,
               u long   src_mask,
               u long   set_mask,
               JSYSOUT  src_rec,
               JSYSOUT  set_rec);

/* Функция изменения элемента витрины                                 */
#define    FChangeShowcaseItem    207
/* Определения констант идентификаторов свойств (параметров)          */
/* элемента витрины                                                   */
#define CASE_ITEM_NAME_ID          1  /* Наименование элемента        */
#define CASE_ITEM_OBJ_MODULE_ID    2  /* "Модуль-хранитель" объекта   */
#define CASE_ITEM_OBJ_PATH_ID      3  /* Полный путь к объекту        */
#define CASE_ITEM_OBJ_NAME_ID      4  /* Имя (ключ) объекта           */
#define CASE_ITEM_OBJ_DATE_ID      5  /* Дата корректировки объекта   */
#define CASE_ITEM_OBJ_FORMAT_ID    6  /* Формат представления объекта */
#define CASE_ITEM_OBJ_ARH_ID       7  /* Тип поколений объекта        */
#define CASE_ITEM_OBJ_EXTN_ID      8  /* Расширение имени объекта     */
#define CASE_ITEM_EXTN_TYPE_ID     9  /* Тип расширения имени объекта */
#define CASE_ITEM_XLINK_TYPE_ID   10  /* Тип ссылки                   */
#define CASE_ITEM_KEYWORDS_ID     11  /* Список ключевых слов         */
#define CASE_ITEM_SUBJECT_ID      12  /* Тема                         */
#define CASE_ITEM_SUB_SUBJECT_ID  13  /* Подтема                      */
#define CASE_ITEM_REGION_ID       14  /* Регион                       */
#define CASE_ITEM_SUB_REGION_ID   15  /* Местность                    */
#define CASE_ITEM_OBJ_RESERV_ID   16  /* Резервное поле для объекта   */
/* Прототип функции                                                   */
PLOTLIB long WINAPI ChangeShowcaseItem(PSTKMPR          pstk,
                                       PSHOWCASE        handler,
                                       void            *pitem_index,
                                       long             interval,
                                       PSC_ITEM_CHANGE  pparams);

/* Функция выполнения сценария UP по ссылке витрины                   */
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

/* Функция указания параметров задачи                                 */
#define    FSetTaskInfo           215
PLOTLIB long WINAPI SetTaskInfo(PSTKMPR     pstk,
                                USTASKINFO *pustaskinfo);

/* Functions for compatibility with Windows CE                        */
#if defined(_WIN32_WCE)
int rename(char *oldname, char *newname);
int remove(char *filename);
#endif

#ifdef VMESA
/* Поток для старта процесса (только для "мультизадачной" системы     */
/* VM ESA                                                             */
  int vmstpr(int name);

/* Ассемблерная вставка для вызова REXX процедуры выполнения команды  */
/* CMS с получением данных в указанный буфер                          */
  #pragma map(upcms,"UPCMSQUE")
  long upcms(u long pstruct);
#endif
/*=========== О П Р Е Д Е Л Е Н И Е  Ф У Н К Ц И Й (конец) ===========*/
#endif /*PLOTH*/

