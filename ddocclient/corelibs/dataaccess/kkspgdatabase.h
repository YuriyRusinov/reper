
#ifndef KKSPGDATABASE_H
#define KKSPGDATABASE_H

#include <QString>

#include <libpq-fe.h>
#include <string>
//#include <libpq-fs.h>
#include "kksdatabase.h"
#include "config_dataaccess.h"
#include "pqnotify.h"


using namespace std;
class KKSPGResult;

/*!\ingroup SystemGroup
\brief Класс KKSPGDatabase
*/
class __DA_EXPORT KKSPGDatabase : public KKSDatabase
{
    public:
#ifdef USE_NOTIFICATION
        KKSPGDatabase(bool _tolisten = false);
#else
        KKSPGDatabase();
#endif

        KKSPGDatabase(PGconn * conn);

        virtual ~KKSPGDatabase();

        virtual bool connect( QString _ipServer = "",
                              QString _database = "", 
                              QString _user = "",
                              QString _password = "", 
                              QString _port = "5432",
                              bool reconnect = false ) const;

		


        virtual bool connect( bool reconnect ) const;

        void disconnect() const;
        bool connected() const;

#ifdef USE_NOTIFICATION
        void addListener( IKKSListener* listener, const char* notify_name );
        void  removeListener( IKKSListener* listener, const char* notify_name );
        void startListen();
        void stopListen();
        void checkNotifies();
#endif

        KKSResult *    execute( const char* query ) const;
        KKSResult *    execSQL( const char * sql, ...) const;
        KKSResult *    execParams(
                                    const char* command, 
                                    int nParams, 
                                    const int * paramTypes, 
                                    const char * const * paramValues, 
                                    const int * paramLengths, 
                                    const int * paramFormats, 
                                    int resultFormat
                                   ) const;

        int             sendQuery(const char* query) const;


        KKSResult * prepare(
                                const char* stmtName, 
                                const char * query,
                                int nParams, 
                                const int * paramTypes
                                ) const;

        KKSResult * execPrepared(
                                const char* stmtName, 
                                int nParams, 
                                const char * const * paramValues, 
                                const int * paramLengths, 
                                const int * paramFormats, 
                                int resultFormat
                                ) const;

        char       *    escapeAsciiString(const char * fromString) const;
        QString         escapeBinaryString(const unsigned char * fromString) const;

        bool begin() const;
        bool commit() const;
        bool rollback() const;
        void createSavePoint(const char * savePointName = NULL) const;
        void releaseSavePoint(const char * savePointName = NULL) const;
        void rollbackToSavePoint(const char * savePointName = NULL) const;

        KKSResult* declare(QString cursor_name, QString sql_string) const;
        KKSResult* fetch(QString cursor_name, int direction = 0, int count_str = 0) const;
        KKSResult* move(QString cursor_name, int direction = 0, int count_str = 0) const;
        KKSResult* close(QString cursor_name) const;

        PGconn * connection() const { return conn; }
        char * lastError() const;

        unsigned int importBlob(const char * fileName, 
                                const char * tableName, 
                                const char * blobColumnName, 
                                const char * keyColumnName, 
                                int id, 
                                bool needDeletePrevious = false);
        int exportBlob(const char * tableName, 
                       const char * blobColumnName, 
                       const char * keyColumnName, 
                       int id, 
                       const char * fileName);

        /* for rod web*/
        bool connect_s(      string _ipServer = "",
							         string _database = "", 
                                     string _user = "",
                                     string _password = "", 
                                     string _port = "5432",
                                      bool reconnect = false );

        /*for rod web  */

    protected:
        friend class KKSCoreApplication;
        mutable PGconn* conn;

#ifdef USE_NOTIFICATION
        mutable PGconn* notify_conn;
        list<KKSNotify*> notifies;
        bool to_listen;
#endif

        bool _exec( const char * sql, KKSPGResult ** res) const;
        int _sendQuery( const char * sql ) const;


        PGresult * _PQexecParams(const char *command,
			                     int nParams,
			                     const Oid *paramTypes,
			                     const char *const * paramValues,
			                     const int *paramLengths,
			                     const int *paramFormats,
			                     int resultFormat)const;

        PGresult * _PQexecPrepared(const char *stmtName,
			                       int nParams,
			                       const char *const * paramValues,
			                       const int *paramLengths,
			                       const int *paramFormats,
			                       int resultFormat) const;

        PGresult * _PQexec(const char *query) const;
};

#endif
