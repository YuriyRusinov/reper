#include <stdarg.h>
#include <QtDebug>

#include "kkspgdatabase.h"
#include "kkspgresult.h"
#include "pqnotify.h"

#define SEC_TO_SLEEP 10
#define _REPEAT_COUNT_ 5
#ifdef WIN32
    #define _SLEEP Sleep
    #define _SECONDS_TO_SLEEP_ 1000*SEC_TO_SLEEP
#else
    #include <unistd.h>
    #define _SLEEP sleep
    #define _SECONDS_TO_SLEEP_ SEC_TO_SLEEP
#endif

//------------------------------------------------------------------------------
//
// KKSPGDatabase::KKSPGDatabase
//
#ifdef USE_NOTIFICATION
KKSPGDatabase::KKSPGDatabase( bool _tolisten ) : notify_conn(NULL), to_listen(_tolisten)
#else
KKSPGDatabase::KKSPGDatabase() : KKSDatabase()
#endif
{
    conn = NULL;
}

KKSPGDatabase::KKSPGDatabase( PGconn * _conn ) : KKSDatabase()
{
    conn = _conn;

    ipServer = PQhost(conn);
    user = PQuser(conn);
    password = PQpass(conn);
    port = PQport(conn);
    database = PQdb(conn);
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::~KKSPGDatabase
//
KKSPGDatabase::~KKSPGDatabase()
{
#ifdef USE_NOTIFICATION
    for( list<KKSNotify*>::iterator ii = notifies.begin(); ii != notifies.end(); ii = notifies.erase(ii) )
        delete *ii;
    stopListen();
#endif
    disconnect();
}

//------------------------------------------------------------------------------
//
// PQdatabase::connect
//
bool KKSPGDatabase::connect( QString _ipServer, 
                             QString _database, 
                             QString _user, 
                             QString _password, 
                             QString _port,
                             bool reconnect) const
{
    if ( reconnect )
    {
        disconnect();
    }
    if ( connected() )
    {
        return true;
    }

    if ( ! _ipServer.isEmpty() ) ipServer = _ipServer;
    if ( ! _database.isEmpty() ) database = _database;
    if ( ! _user.isEmpty() ) user = _user;
    if ( ! _password.isEmpty() ) password = _password;
    if ( ! _port.isEmpty() ) port = _port;

    conn =  PQsetdbLogin( ipServer.toAscii().constData(), 
                          port.toAscii().constData(), 
                          NULL, 
                          NULL,
                          database.toAscii().constData(), 
                          user.toAscii().constData(),  
                          password.toAscii().constData());
    
    if( !connected() ){
        char * err = PQerrorMessage(conn);
        qCritical() << err;

        return connected();
    }

#ifdef USE_NOTIFICATION

    if (to_listen)
    {
        notify_conn = PQsetdbLogin( ipServer.toAscii().constData(), 
                                    port.toAscii().constData(), 
                                    NULL, 
                                    NULL,
                                    database.toAscii().constData(), 
                                    user.toAscii().constData(), 
                                    password.toAscii().constData() );

        switch( PQstatus( notify_conn ) )
        {
                case CONNECTION_OK:
                break;

                case CONNECTION_BAD:
                default:
                break;////////Oleg//////////////////
        }
    }
#endif
    
    if(!execute("set client_encoding to utf8")){
        qCritical("cannot set client_encoding to utf8");
        disconnect();
        return connected();
    }

    if(!execute("select f_set_all_schemas_visible()")){
        qCritical("cannot set search_path to public, dic, nub");
        disconnect();
        return connected();
    }
    

    if(!execute("select createTempTables()")){
        qCritical("cannot execute createTempTables()");
        disconnect();
        return connected();
    }
        
    if(idCurrentDl > 0){
        char * q = new char[100];
        snprintf(q, 99, "select setCurrentDl(%d)", idCurrentDl);
        if(!execute(q)){
            delete[] q;
            qCritical("cannot execute setCurrentDl()");
            disconnect();
            return connected();
        }
        delete[] q;
    }

    return connected();
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::disconnect
//
void KKSPGDatabase::disconnect() const
{
    idCurrentDl = -1;
    idCurrentUser = -1;

    if ( conn != NULL )
    {
        if(!execute("select f_clear_function();")){
            qCritical("cannot execute f_clear_function!");
        }

        PQfinish( conn );
        conn = NULL;
    }

#ifdef USE_NOTIFICATION
    if (!to_listen)
        return;

    if ( notify_conn != NULL )
    {
        PQfinish( notify_conn );
        notify_conn = NULL;
    }
#endif

    emit disconnected();
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::connected
//
bool KKSPGDatabase::connected() const
{
    bool s_conn;
#ifdef USE_NOTIFICATION

    bool  s_notify_conn;

    if (to_listen)
        s_notify_conn = (PQstatus( notify_conn ) == CONNECTION_OK);
#endif

    s_conn = (PQstatus( conn ) == CONNECTION_OK);

#ifdef USE_NOTIFICATION
    return ((to_listen) ? (s_conn && s_notify_conn) : s_conn);
#else
    return s_conn;
#endif
}

#ifdef USE_NOTIFICATION
//------------------------------------------------------------------------------
//
// KKSPGDatabase::addListener
//
void KKSPGDatabase::addListener( IKKSListener* listener, const char* notify_name )
{

    // Try to find it in the notifies
    for( list<KKSNotify*>::iterator ii = notifies.begin(); ii != notifies.end(); ii++ )
        if ( strcmp( (*ii)->name.c_str(), notify_name ) == 0 )
        {
            // We are listening it, so add only the listener
            bool isFound = false;
            for( list<IKKSListener*>::iterator jj = (*ii)->listeners.begin(); jj != (*ii)->listeners.end(); jj++ )
            {
                if ( (*jj) == listener )
                {
                    isFound = true;

                    break;
                }
            }
            if ( !isFound )
                (*ii)->listeners.push_back( listener );
            return;
        }

    // Start listen
    char * sql = new char[50 + strlen(notify_name)];
    PGresult* result = NULL;
    sprintf( sql,"LISTEN %s;", notify_name );

    //  if ( PQstatus( notify_conn ) == CONNECTION_BAD )
    //    throw PQexception( "Not Connected" );

    result = PQexec( notify_conn, sql );

    //  if ( PGresultStatus( result ) != PGRES_COMMAND_OK &&
    //       PGresultStatus( result ) != PGRES_TUPLES_OK )
    //    throw PQexception( PQerrorMessage( notify_conn ) );

    PQclear( result );

    // Add notify
    KKSNotify* notify = new KKSNotify();
    notify->name = notify_name;
    notify->listeners.push_back( listener );
    notifies.push_back( notify );

    delete[] sql;

    return;
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::removeListener
//
void KKSPGDatabase::removeListener( IKKSListener* listener, const char* notify_name )
{
    for( list<KKSNotify*>::iterator ii = notifies.begin(); ii != notifies.end(); ii++ ){
        if ( strcmp( (*ii)->name.c_str(), notify_name ) == 0 )
        {
            // Remove notify for the listener
            for( list<IKKSListener*>::iterator jj = (*ii)->listeners.begin(); jj != (*ii)->listeners.end(); jj++ )
                if ( (*jj) == listener )
                {
                    (*ii)->listeners.erase( jj );
                    break;
                }

            if ( (*ii)->listeners.size() > 0) 
                return;

            // If there are no listeners for notify,
            // Stop listen it
            char sql[50];
            PGresult* result = NULL;
            sprintf( sql,"UNLISTEN %s;", notify_name );

            //      if ( PQstatus( notify_conn ) == CONNECTION_BAD )
            //        throw PQexception( "Not Connected" );

            result = PQexec( notify_conn, sql );

            //      if ( PGresultStatus( result ) != PGRES_COMMAND_OK &&
            //           PGresultStatus( result ) != PGRES_TUPLES_OK )
            //        throw PQexception( PQerrorMessage( notify_conn ) );

            PQclear( result );

            // Remove from notifies
            delete *ii;
            notifies.erase( ii );
            if ( notifies.size() == 0) 
                stopListen();
            return;
        }
    }
    return;
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::startListen
//
void KKSPGDatabase::startListen()
{
    if(!notify_conn){
        notify_conn = PQsetdbLogin( ipServer.toAscii().constData(), 
                                    port.toAscii().constData(), 
                                    NULL, 
                                    NULL,
                                    database.toAscii().constData(), 
                                    user.toAscii().constData(), 
                                    password.toAscii().constData() );

        switch( PQstatus( notify_conn ) )
        {
                case CONNECTION_OK:
                break;

                case CONNECTION_BAD:
                default:
                break;
        }    }
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::stopListen
//
void KKSPGDatabase::stopListen()
{}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::checkNotifies
//
void KKSPGDatabase::checkNotifies()
{
    //  if ( PQstatus( notify_conn ) != CONNECTION_OK )
    //    throw PQexception( "Not Connected" );

    PGnotify* notify;
    PQconsumeInput( notify_conn );
    while ( ( notify = PQnotifies( notify_conn ) ) != NULL )
    {
        for( list<KKSNotify*>::iterator ii = notifies.begin(); ii != notifies.end(); ii++ ){
            KKSNotify * n = (*ii);
            if(!n)
                continue;

            if ( strcmp( n->name.c_str(), notify->relname ) == 0 )
            {
                for( list<IKKSListener*>::iterator jj = (*ii)->listeners.begin(); jj != (*ii)->listeners.end(); jj++ ){
                    IKKSListener * l = (*jj);
                    if(l)
                        l->notify( notify->relname, notify->extra );
                }
            }
        }
        PQfreemem( notify );
    }
}
#endif /* USE_NOTIFICATION */

//------------------------------------------------------------------------------
//
// KKSPGDatabase::execute
//
KKSResult* KKSPGDatabase::execute(const char* query ) const
{
    KKSPGResult* _res = NULL;

    if ( ! _exec(query, &_res) )
        return NULL;

    return (KKSResult*)_res;
}

int KKSPGDatabase::sendQuery(const char* query) const
{
    int res = 0;
    res = _sendQuery(query);
    if ( res != 1)
        return 0;

    return res;
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::execParams
//
KKSResult* KKSPGDatabase::execParams(const char* command, 
                                     int nParams, 
                                     const int * paramTypes, 
                                     const char * const * paramValues, 
                                     const int * paramLengths, 
                                     const int * paramFormats, 
                                     int resultFormat ) const
{
    KKSPGResult* _res = NULL;

    PGresult * res = NULL;
    int * internalParamTypes = NULL;

    if(nParams == 0){
        res = _PQexecParams( command,
                             0,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             resultFormat
                           );
    }
    else{
        internalParamTypes = new int[nParams];
        for (int i=0; i<nParams; i++)
            internalParamTypes[i] = KKSPGResult::toInternalType((KKSResult::DataType)paramTypes[i]);

        res = _PQexecParams( command,
                             nParams,
                             (const Oid *) internalParamTypes,
                             paramValues,
                             paramLengths,
                             paramFormats,
                             resultFormat
                           );
    }
    _res = new KKSPGResult(res);
    
    if(internalParamTypes) 
            delete[] internalParamTypes;
    
    KKSResult::ResultStatus status = (_res)->resultStatus();

    if ( status != KKSResult::CommandOk && status != KKSResult::TuplesOk )
    {
        fprintf(stderr, "PQdatabase: %s SQL: %s\n", _res->errorMessage().toLocal8Bit().constData(), command);
        qCritical() << _res->errorMessage() << " SQL: " << command;
        delete _res;
        return NULL;
    }

    return (KKSResult*)_res;
}

/*
 * under MCBC, где стоит Postgresql 7.4 функции PQprepare нет
 * поэтому метод закомментирован
 */

KKSResult * KKSPGDatabase::prepare(
                                const char* stmtName, 
                                const char * query,
                                int nParams, 
                                const int * paramTypes
                                ) const
{
    KKSPGResult* _res = NULL;

    PGresult * res = NULL;
    int * internalParamTypes = NULL;

    if(nParams == 0){
        res = PQprepare(  conn,
                          stmtName,
                          query,
                          0,
                          NULL
                       );
    }
    else{
        internalParamTypes = new int[nParams];
        for (int i=0; i<nParams; i++)
            internalParamTypes[i] = KKSPGResult::toInternalType((KKSResult::DataType)paramTypes[i]);

        res = PQprepare(  conn,
                              stmtName,
                             query,
                             nParams,
                             (const Oid *) internalParamTypes
                           );
    }
    _res = new KKSPGResult(res);
    
    if(internalParamTypes) 
            delete[] internalParamTypes;
    
    KKSResult::ResultStatus status = (_res)->resultStatus();

    if ( status != KKSResult::CommandOk && status != KKSResult::TuplesOk )
    {
        fprintf(stderr, "PQdatabase: %s SQL: %s\n", _res->errorMessage().toLocal8Bit().constData(), query);
        qCritical() << _res->errorMessage() << " SQL: " << query;
        delete _res;
        return NULL;
    }

    return (KKSResult*)_res;

}


KKSResult * KKSPGDatabase::execPrepared(
                                        const char* stmtName, 
                                        int nParams, 
                                        const char * const * paramValues, 
                                        const int * paramLengths, 
                                        const int * paramFormats, 
                                        int resultFormat
                                       ) const
{
    KKSPGResult* _res = NULL;

    PGresult * res = NULL;

    if(nParams == 0){
        res = _PQexecPrepared( stmtName,
                               0,
                               NULL,
                               NULL,
                               NULL,
                               resultFormat
                             );
    }
    else{

        res = _PQexecPrepared( stmtName,
                               nParams,
                               paramValues,
                               paramLengths,
                               paramFormats,
                               resultFormat
                             );
    }
    _res = new KKSPGResult(res);
        
    KKSResult::ResultStatus status = (_res)->resultStatus();

    if ( status != KKSResult::CommandOk && status != KKSResult::TuplesOk )
    {
        qCritical() << _res->errorMessage();
        delete _res;
        return NULL;
    }

    return (KKSResult*)_res;
}


//------------------------------------------------------------------------------
//
// KKSPGDatabase::begin
//
bool KKSPGDatabase::begin() const
{
    KKSResult* tempres;

    tempres = execute( "BEGIN");// ISOLATION LEVEL SERIALIZABLE" );

    if(tempres){
        delete tempres;
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::commit
//
bool KKSPGDatabase::commit() const
{
    KKSResult* tempres;

    tempres = execute( "COMMIT" );

    if(tempres){ 
        delete tempres;
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//
// KKSPGDatabase::rollback
//
bool KKSPGDatabase::rollback() const
{
    KKSResult* tempres;

    tempres = execute( "ROLLBACK" );

    if(tempres){
        delete tempres;
        return true;
    }

    return false;
}

void KKSPGDatabase::createSavePoint(const char * savePointName) const
{
    if(!savePointName){
        execute("SAVEPOINT new_kks_savepoint");
        return;
    }

    char * query  = new char[30 + strlen(savePointName)];
    strcpy(query, "SAVEPOINT ");
    strcat(query, savePointName);
    execute(query);
}

void KKSPGDatabase::releaseSavePoint(const char * savePointName) const
{
    if(!savePointName){
        execute("RELEASE SAVEPOINT new_kks_savepoint");
        return;
    }

    char * query  = new char[30 + strlen(savePointName)];
    strcpy(query, "RELEASE SAVEPOINT ");
    strcat(query, savePointName);
    execute(query);
}

void KKSPGDatabase::rollbackToSavePoint(const char * savePointName) const
{
    if(!savePointName){
        execute("ROLLBACK TO SAVEPOINT new_kks_savepoint");
        return;
    }

    char * query  = new char[30 + strlen(savePointName)];
    strcpy(query, "ROLLBACK TO SAVEPOINT ");
    strcat(query, savePointName);
    execute(query);
}


unsigned int KKSPGDatabase::importBlob(const char * fileName, 
                                       const char * tableName, 
                                       const char * blobColumnName, 
                                       const char * keyColumnName, 
                                       int id, 
                                       bool needDeletePrevious)
{
    begin();
    FILE * f = fopen(fileName, "r");
    if(!f)
    {
        rollback();
        return 0;
    }
    fclose(f);
    Oid oid = lo_import(conn, fileName);
    
    if(needDeletePrevious){
        KKSResult * res = execSQL("select lo_unlink(%s) from %s where %s = %d;", blobColumnName, tableName, keyColumnName, id);
        if(!res || res->resultStatus() != KKSResult::TuplesOk){
            if(res) delete res;
            rollback();
            return 0;
        }
    }
    
    KKSResult * res = execSQL("update %s set %s = %d where %s = %d", tableName, blobColumnName, (int)oid, keyColumnName, id);
    if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
        if(res) delete res;
        rollback();
        return 0;
    }
    
    commit();
    return oid;
}

int KKSPGDatabase::exportBlob(const char * tableName, 
                              const char * blobColumnName, 
                              const char * keyColumnName, 
                              int id, 
                              const char * fileName)
{
    begin();
    FILE * f = fopen(fileName, "w");
    if(!f)
    {
        rollback();
        return 0;
    }
    fclose(f);
    KKSResult * res = execSQL("select %s from %s where %s = %d", blobColumnName, tableName, keyColumnName, id);
    if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
        if(res) delete res;
        rollback();
        return 0;
    }
    int oid = res->getCellAsInt(0, 0);
    int result = lo_export(conn, oid, fileName);
    commit();
    return result;
}

KKSResult * KKSPGDatabase::execSQL( const char * query, ... ) const
{
    QString s;
    va_list ap;

    va_start(ap, query);
    s = s.vsprintf(query, ap);
    va_end(ap);
    
    return KKSDatabase::execute(s);
}

bool KKSPGDatabase::_exec( const char * sql, KKSPGResult ** _res ) const
{
    if ( ! _res )
        return FALSE;

    if ( *_res )
        delete *_res;

    *_res = new KKSPGResult(_PQexec( sql ));

    KKSResult::ResultStatus status = (*_res)->resultStatus();

    if ( status != KKSResult::CommandOk && status != KKSResult::TuplesOk )
    {
        //fprintf(stderr, "PQdatabase: %s SQL: %s\n", (*_res)->errorMessage().toLocal8Bit().constData(), sql);
        qCritical("PQdatabase: %s SQL: %s\n", (*_res)->errorMessage().toLocal8Bit().constData(), sql);
        delete *_res;
        return FALSE;
    }

    return TRUE;
}


int KKSPGDatabase::_sendQuery( const char * sql ) const
{
    int _res = PQsendQuery( conn, sql );
    return _res;

}

bool KKSPGDatabase::connect( bool reconnect ) const
{
    return connect(ipServer,database,user,password, port, reconnect);
}

char * KKSPGDatabase::escapeAsciiString(const char * fromString) const
{
    Q_UNUSED(fromString);

    return NULL;
}

QString KKSPGDatabase::escapeBinaryString (const unsigned char * fromString) const
{
    size_t esc_size;
    size_t fromSize = strlen((char *)fromString);
    unsigned char *esc = /*(char *)*/PQescapeBytea/*Conn*/ (/*conn,*/ fromString, fromSize, &esc_size);
    QString res ((char *)esc);
    PQfreemem (esc);

    return res;
}

KKSResult* KKSPGDatabase::declare(QString cursor_name, QString sql_string) const
{
    KKSResult* tempres;

    QString str = "";
    str = "DECLARE " + cursor_name + " SCROLL CURSOR FOR " + sql_string;

    tempres = execute( str.toAscii().constData() );

    return tempres;
}

KKSResult* KKSPGDatabase::fetch(QString cursor_name, int direction, int count_str) const
{
//FETCH [ direction { FROM | IN } ] cursorname
    KKSResult* tempres;

    QString str = "";

    switch(direction)
    {
        case 0:
            str = "fetch NEXT from " + cursor_name;
            break;
        case 1:
            str = "fetch PRIOR from " + cursor_name;
            break;
        case 2:
            str = "fetch FIRST from " + cursor_name;
            break;
        case 3:
            str = "fetch LAST from " + cursor_name;
            break;
        case 4:
            str = QString("fetch ABSOLUTE %1 from ").arg(count_str) + cursor_name;
            break;
        case 5:
            str = QString("fetch RELATIVE  %1 from ").arg(count_str) + cursor_name;
            break;
        case 6:
            str = QString("fetch %1 from ").arg(count_str) + cursor_name;
            break;
        case 7:
            str = "fetch ALL from " + cursor_name;
            break;
        case 8:
            str = "fetch FORWARD from " + cursor_name;
            break;
        case 9:
            str = QString("fetch FORWARD %1 from ").arg(count_str) + cursor_name;
            break;
        case 10:
            str = "fetch FORWARD ALL from " + cursor_name;
            break;
        case 11:
            str = "fetch BACKWARD from " + cursor_name;
            break;
        case 13:
            str = "fetch BACKWARD ALL from " + cursor_name;
            break;
        case 12:
            str = QString("fetch BACKWARD %1 from ").arg(count_str) + cursor_name;
            break;
        default:
            str = "";
    }

    tempres = execute( str.toAscii().constData() );

     return tempres;
}

KKSResult* KKSPGDatabase::close(QString cursor_name) const
{
//  CLOSE ?????? 

    KKSResult* tempres;

    QString str = "";
    str = "CLOSE " + cursor_name;

    tempres = execute( str.toAscii().constData() );
    commit();
    
    return tempres;
}

KKSResult* KKSPGDatabase::move(QString cursor_name, int direction, int count_str) const
{
    KKSResult* tempres;

    QString str = "";

    switch(direction)
    {
        case 0:
            str = "move NEXT from " + cursor_name;
            break;
        case 1:
            str = "move PRIOR from " + cursor_name;
            break;
        case 2:
            str = "move FIRST from " + cursor_name;
            break;
        case 3:
            str = "move LAST from " + cursor_name;
            break;
        case 4:
            str = QString("move ABSOLUTE %1 from ").arg(count_str) + cursor_name;
            break;
        case 5:
            str = QString("move RELATIVE  %1 from ").arg(count_str) + cursor_name;
            break;
        case 6:
            str = QString("move %1 from ").arg(count_str) + cursor_name;
            break;
        case 7:
            str = "move ALL from " + cursor_name;
            break;
        case 8:
            str = "move FORWARD from " + cursor_name;
            break;
        case 9:
            str = QString("move FORWARD %1 from ").arg(count_str) + cursor_name;
            break;
        case 10:
            str = "move FORWARD ALL from " + cursor_name;
            break;
        case 11:
            str = "move BACKWARD from " + cursor_name;
            break;
        case 13:
            str = "move BACKWARD ALL from " + cursor_name;
            break;
        case 12:
            str = QString("move BACKWARD %1 from ").arg(count_str) + cursor_name;
            break;
        default:
            str = "";
    }

    tempres = execute( str.toAscii().constData() );

    return tempres;
}

char * KKSPGDatabase::lastError()  const
{
    return PQerrorMessage(conn);
}

//------------------------------------------------------------------------------
//for rod web
// PQdatabase::connect_s
//
bool KKSPGDatabase::connect_s(	string _ipServer, 
								string _database, 
								string _user, 
								string _password, 
								string _port,
                                bool reconnect)
{


	return connect(QString(_ipServer.c_str()),
		QString(_database.c_str()),
		QString(_user.c_str()), 
		QString(_password.c_str()), 
		QString(_port.c_str()), 
		reconnect);


}

PGresult * KKSPGDatabase::_PQexecParams(const char *command,
	                                    int nParams,
	                                    const Oid *paramTypes,
	                                    const char *const * paramValues,
	                                    const int *paramLengths,
	                                    const int *paramFormats,
                                        int resultFormat) const
{
    PGresult * res;
    res = PQexecParams(conn, command, nParams, paramTypes, paramValues, paramLengths, paramFormats, resultFormat);
    if(res)
        return res;

    return res;
}

PGresult * KKSPGDatabase::_PQexecPrepared(const char *stmtName,
                                          int nParams,
                                          const char *const * paramValues,
                                          const int *paramLengths,
                                          const int *paramFormats,
                                          int resultFormat) const
{
    PGresult * res;
    res = PQexecPrepared(conn, stmtName, nParams, paramValues, paramLengths, paramFormats, resultFormat);
    return res;
}

PGresult * KKSPGDatabase::_PQexec(const char *query) const
{
    PGresult * res;
    res = PQexec(conn, query);
    int resStatus = PQresultStatus(res);
    if( resStatus == PGRES_FATAL_ERROR )
    {
    }
    else{
        return res;
    }

    //try to reconnect if disconnected
    int cStatus = PQstatus(conn);
    int i=0;
    if(cStatus != CONNECTION_OK){
        bool ok = false;
        do{
            ok = KKSPGDatabase::connect(true);
            if(ok){
                break;
            }
            _SLEEP(_SECONDS_TO_SLEEP_);
            i++;
        }while(i<_REPEAT_COUNT_);
        
        res = PQexec(conn, query);
    }

    return res;
}
