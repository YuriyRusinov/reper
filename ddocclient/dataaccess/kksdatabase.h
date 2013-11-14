
#ifndef KKSDATABASE_H
#define KKSDATABASE_H

#include "config_dataaccess.h"
#include "kksresult.h"
#include <QObject>


/*!\ingroup SystemGroup
\brief Класс KKSDatabase
*/
class __DA_EXPORT KKSDatabase : QObject
{
    Q_OBJECT

    friend class KKSSito;
    friend class KKSLoader;
    
    public:
        KKSDatabase()
        {
            idCurrentDl = -1;
            idCurrentUser = -1;
        }

        virtual ~KKSDatabase(){}

        virtual bool connect( QString _ipServer = "",
                              QString _database = "", 
                              QString _user = "",
                              QString _password = "", 
                              QString _port = "5432",
                              bool reconnect = false ) const = 0;

        virtual bool connect( bool reconnect ) const = 0;

        virtual void disconnect() const = 0;
        virtual bool connected() const = 0;

        virtual KKSResult * execute( const char* query ) const = 0;
                KKSResult * execute( const QString & query) const;
                int         executeCmd( const QString & sql ) const;
        virtual KKSResult * execSQL( const char * sql, ...) const = 0;

        virtual KKSResult * execPrepared(
                                        const char* stmtName, 
                                        int nParams, 
                                        const char * const * paramValues, 
                                        const int * paramLengths, 
                                        const int * paramFormats, 
                                        int resultFormat
                                       ) const = 0;


        virtual KKSResult * execParams(
                                        const char* command, 
                                        int nParams, 
                                        const int * paramTypes, 
                                        const char * const * paramValues, 
                                        const int * paramLengths, 
                                        const int * paramFormats, 
                                        int resultFormat
                                       ) const = 0;
                KKSResult * execParams(
                                        const QString & command, 
                                        const QList<KKSResult::DataType>& paramTypes, 
                                        const QList<QVariant> & paramValues,
                                        const QList<int> & paramFormats, 
                                        int resultFormat
                                        ) const;

        virtual char *    escapeAsciiString(const char * fromString) const = 0;
        virtual QString   escapeBinaryString(const unsigned char * fromString) const = 0;


        virtual bool begin() const = 0;
        virtual bool commit() const = 0;
        virtual bool rollback() const = 0;

        virtual void createSavePoint(const char * savePointName = NULL) const = 0;
        virtual void releaseSavePoint(const char * savePointName = NULL) const = 0;
        virtual void rollbackToSavePoint(const char * savePointName = NULL) const = 0;

        virtual KKSResult* declare(QString cursor_name, QString sql_string) const = 0;
        virtual KKSResult* fetch(QString cursor_name, int direction, int count_str = 0) const = 0;
        virtual KKSResult* move(QString cursor_name, int direction, int count_str = 0) const = 0;
        virtual KKSResult* close(QString cursor_name) const = 0;


        virtual char * lastError() const = 0; 
        
        virtual unsigned int importBlob(const char * fileName, 
                                        const char * tableName, 
                                        const char * blobColumnName, 
                                        const char * keyColumnName,  
                                        int id, 
                                        bool needDeletePrevious = false) = 0;
        virtual int          exportBlob(const char * tableName, 
                                        const char * blobColumnName, 
                                        const char * keyColumnName, 
                                        int id, 
                                        const char * fileName) = 0;

        QString getHost() const {return ipServer;}
        QString getPort() const {return port;}
        QString getName() const {return database;}
        QString getUser() const {return user;}
        QString getPass() const {return password;}

        int getCurrentDl() const {return idCurrentDl;}
        int getCurrentUser() const {return idCurrentUser;}

    protected:
        mutable QString ipServer;
        mutable QString port;
        mutable QString database;
        mutable QString user;
        mutable QString password;

        void setCurrentDl(int id) {idCurrentDl = id;}
        void setCurrentUser(int id) {idCurrentUser = id;}

        mutable int idCurrentDl;//id of current DL
                        //will used if need to reconnect
        mutable int idCurrentUser;

    signals:
        void disconnected() const;
};

#endif
