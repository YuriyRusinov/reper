#include "kkspgdatabase.h"
#include "kkspgresult.h"
#include <kks_port.h>
#include <libpq-fe.h>
#include <QVariant>
#include <QByteArray>
#include <QDebug>

KKSPGResult::KKSPGResult( PGresult * _res)
{
    res = _res;
    if(!res){
        rowCount = 0;
        columnCount = 0;
    }
    else{
        rowCount = PQntuples(res);
        columnCount = PQnfields(res);
        currentRow = 0;
    }
}

KKSPGResult::~ KKSPGResult( )
{
    PQclear(res);
}

unsigned char * KKSPGResult::getCellData( int row, int column ) const
{
    if ( PQgetisnull( res, row , column ) )
        return 0;

    return (unsigned char *)PQgetvalue(res, row, column);
}

int KKSPGResult::getCellLength( int row, int column ) const
{
    if ( PQgetisnull( res, row , column ) )
        return 0;

    return PQgetlength(res,row,column);
}

QVariant KKSPGResult::getCell( int row, int column ) const
{
    if ( PQgetisnull( res, row , column ) )
        return QVariant();

    bool bBinary = PQfformat( res, column ) == 1; // 1 - binary column, 0 - text

    if ( !bBinary )
    {
        //проверяем на массив. типы с рассматриваемыми ниже значениями являются массивами.
        //Их нужно представлять как QStringList
        int oid = (int)PQftype(res, column);
        if (   ( oid > 999 && oid < 1006 ) /* bool[], bytea[], char[], name[], int2[] */
            || ( oid == pgINT4ARRAYOID ) /*int4[]*/
            || ( oid == pgTEXTARRAYOID )/*text[]*/
            || ( oid > 1014 && oid < 1017 ) /*varchar[], int8[]*/
            || ( oid > 1020 && oid < 1023 ) /*float4[], float8[]*/
            || ( oid == pgTIMESTAMPARRAYOID )/*timestamp[]*/
           )
        {
            QString str = QString::fromUtf8(PQgetvalue(res, row, column));
            int length = str.length();
            str = str.left(length-1).right(length-2);
            return QVariant( str.split(",") );
        }
        if(oid == pgTIMESTAMPOID){
            const char * d = PQgetvalue(res, row, column);
            QDateTime dt = QDateTime::fromString(d, Qt::ISODate);
            //QString d = QString::fromUtf8(PQgetvalue( res, row, column ));
            //d = d.left(d.length()-2);
            //QDateTime dt = QDateTime::fromString(d, "yyyy-MM-dd hh:mm:ss.zzz");
            if(!dt.isValid())
                qWarning() << QString("Datetime is invalid in KKSPGResult! String = %1").arg(d);
            
            return QVariant(dt);
        }
        return QVariant(QString::fromUtf8(PQgetvalue( res, row, column )));
    }
    else
    {
        bool bVal;
        int iVal;
        qint64 i64Val; 
        float fVal;
        double dfVal;
        switch ( PQftype(res, column) )
        {
            case pgBOOLOID:
                bVal = *(bool *) PQgetvalue( res, row, column );
                return QVariant( bVal );
                break;
            case pgINT4OID:
                //memcpy(&iVal,PQgetvalue( res, row, column ),sizeof(iVal));
                iVal = *(int * ) PQgetvalue( res, row, column );
                XGIS_MSBWORD32(iVal);
                return QVariant( iVal );
                break;
            case pgINT8OID:
                i64Val = *( quint64 * ) PQgetvalue( res, row, column );
                XGIS_MSBPTR64(&i64Val)
                return QVariant( i64Val );
                break;
            case pgFLOAT4OID:
                fVal = *( float * ) PQgetvalue( res, row, column );
                XGIS_MSBPTR32(&fVal)
                return QVariant( fVal );
                break;
            case pgFLOAT8OID:
                dfVal = *( double * ) PQgetvalue( res, row, column );
                XGIS_MSBPTR64(&dfVal)
                return QVariant( dfVal );
                break;
            case pgBYTEAOID:
                return QVariant( QByteArray::fromRawData( PQgetvalue(res,row,column), PQgetlength(res,row,column) ) );
                break;
            case pgTEXTOID:
            default:
                return QVariant( QString::fromUtf8(PQgetvalue(res,row,column) ) );
                break;
        }
    }
    return QVariant();
}

QByteArray KKSPGResult::getCellAsByteArray (int row, int column) const
{
    if (!res || row < 0 || row >= rowCount || column < 0 || column >= columnCount)
        return 0;

    if (PQgetisnull (res, row , column) )
        return 0;

    //bool bBinary = PQfformat( res, column ) == 1; // 1 - binary column, 0 - text
    if ( PQfformat( res, column ) == 1 ){
        return QByteArray(PQgetvalue(res,row,column), PQgetlength(res,row,column));
    }

    unsigned char *v = (unsigned char *)PQgetvalue (res, row, column);
    size_t esc_size;
    char *wres = (char *)PQunescapeBytea (v, &esc_size);
    QByteArray V = QByteArray (wres, (int)esc_size);
    PQfreemem (wres);
    return V;
}




bool KKSPGResult::isEmpty( int row, int column ) const
{
    if(!res) 
        return TRUE;
    if(row < 0 || row >= rowCount) 
        return TRUE;
    if(column < 0 || column >= columnCount) 
        return TRUE;

    return PQgetisnull(res, row, column);
}

void KKSPGResult::next( )
{
    if(!res) return;
    if(currentRow >= rowCount-1) return;

    currentRow++;
}

void KKSPGResult::prev( )
{
    if(!res) 
        return;
    if(currentRow <= 0) 
        return;

    currentRow--;
}

KKSResult::ResultStatus KKSPGResult::resultStatus( ) const
{
    if(!res) 
        return KKSResult::Unknown;

    int status = PQresultStatus(res);
    switch(status){
        case PGRES_EMPTY_QUERY: 
            return KKSResult::EmptyQuery;
        case PGRES_COMMAND_OK: 
            return KKSResult::CommandOk;
        case PGRES_TUPLES_OK: 
            return KKSResult::TuplesOk;
        case PGRES_COPY_OUT: 
            return KKSResult::CopyOut;
        case PGRES_COPY_IN: 
            return KKSResult::CopyIn;
        case PGRES_BAD_RESPONSE: 
            return KKSResult::BadResponse;
        case PGRES_NONFATAL_ERROR: 
            return KKSResult::NonfatalError;
        case PGRES_FATAL_ERROR: 
            return KKSResult::FatalError;
        default: 
            return KKSResult::Unknown;
    }
    return KKSResult::Unknown;
}

QString KKSPGResult::errorMessage( ) const
{
    if(!res) 
        return QString();
    
    return QObject::tr(PQresultErrorMessage(res));
}

QString KKSPGResult::errorCode( ) const
{
    if(!res) 
        return QString();
    
    return QObject::tr(PQresultErrorField(res, PG_DIAG_SQLSTATE));
}

int KKSPGResult::getColumnNumber(const char * columnName) const
{
    int index = -1;

    index = PQfnumber(res, columnName);

    return index;
}

const char * KKSPGResult::getColumnName(int column) const
{
    return PQfname(res, column);
}

KKSResult::DataType KKSPGResult::getColumnDataType(int column) const
{
    if(!res) 
        return KKSResult::dtUnknown;

    switch ( PQftype(res, column) ){
        case pgBOOLOID: 
            return dtBool;
        case pgBOOLARRAYOID: 
            return dtBoolArray;
        case pgBYTEAARRAYOID: 
            return dtByteaArray;
        case pgBYTEAOID: 
            return dtBytea;
        case pgCHARARRAYOID: 
            return dtCharArray;
        case pgCHAROID: 
            return dtChar;
        case pgFLOAT4OID: 
            return dtFloat4;
        case pgFLOAT4ARRAYOID: 
            return dtFloat4Array;
        case pgFLOAT8ARRAYOID: 
            return dtFloat8Array;
        case pgFLOAT8OID: 
            return dtFloat8;
        case pgINT2ARRAYOID: 
            return dtInt2Array;
        case pgINT2OID: 
            return dtInt2;
        case pgINT4ARRAYOID: 
            return dtInt4Array;
        case pgINT4OID: 
            return dtInt4;
        case pgINT8ARRAYOID: 
            return dtInt8Array;
        case pgINT8OID: 
            return dtInt8;
        case pgNAMEARRAYOID: 
            return dtNameArray;
        case pgNAMEOID: 
            return dtName;
        case pgTEXTARRAYOID: 
            return dtTextArray;
        case pgTEXTOID: 
            return dtText;
        case pgVARCHARARRAYOID: 
            return dtVarcharArray;
        case pgVARCHAROID: 
            return dtVarchar;
        case pgTIMESTAMPOID:
            return dtTimestamp;
        default: 
            return dtUnknown;
    };
    
    return dtUnknown;
}

int KKSPGResult::toInternalType(KKSResult::DataType type)
{
    switch ( type ){
        case dtBool:         return pgBOOLOID;    
        case dtBoolArray:     return pgBOOLARRAYOID;    
        case dtByteaArray:     return pgBYTEAARRAYOID;
        case dtBytea:         return pgBYTEAOID;    
        case dtCharArray:     return pgCHARARRAYOID;
        case dtChar:         return pgCHAROID;        
        case dtFloat4:         return pgFLOAT4OID;
        case dtFloat4Array:     return pgFLOAT4ARRAYOID;
        case dtFloat8Array:     return pgFLOAT8ARRAYOID;
        case dtFloat8:         return pgFLOAT8OID;        
        case dtInt2Array:     return pgINT2ARRAYOID;
        case dtInt2:         return pgINT2OID;        
        case dtInt4Array:     return pgINT4ARRAYOID;
        case dtInt4:         return pgINT4OID;        
        case dtInt8Array:     return pgINT8ARRAYOID;
        case dtInt8:         return pgINT8OID;        
        case dtNameArray:     return pgNAMEARRAYOID;
        case dtName:         return pgNAMEOID;        
        case dtTextArray:     return pgTEXTARRAYOID;
        case dtText:         return pgTEXTOID;            
        case dtVarcharArray: return pgVARCHARARRAYOID;
        case dtVarchar:         return pgVARCHAROID;
        case dtGeometry:     return pgGEOMETRY;
        case dtTimestamp:    return pgTIMESTAMPOID;
        
        default:             return pgVARCHAROID;
    };
    
    return pgVARCHAROID;
}
