#ifndef KKSPGRESULT_H
#define KKSPGRESULT_H

#include "kksresult.h"
#include <libpq-fe.h>
//#include "kkspgdatabase.h"

class KKSPGResult : public KKSResult
{
    public:
        KKSPGResult(PGresult * _res);
        ~KKSPGResult();

        KKSResult::DataType getColumnDataType(int column) const ;
        int getColumnNumber(const char * columnName) const;
		const char * getColumnName(int column) const;

        unsigned char * getCellData( int row, int column ) const;
        int getCellLength( int row, int column ) const;

        QVariant getCell(int row, int column) const;
        QByteArray getCellAsByteArray (int row, int column) const;
        
        bool isEmpty(int row, int column) const;
        int getRowCount() const {return rowCount;}
        int getColumnCount() const {return columnCount;}
        ResultStatus resultStatus() const;
        QString errorMessage() const;
        QString errorCode() const;

        static int toInternalType(KKSResult::DataType type);

        void next();
        void prev();
    private:
        PGresult * res;
        int rowCount;
        int columnCount;
        
        enum PGDataType{
            pgGEOMETRY = 0,

            pgBOOLOID    = 16,
            pgINT2OID    = 21,
            pgINT4OID    = 23,
            pgINT8OID    = 20,
            pgFLOAT4OID  = 700,
            pgFLOAT8OID  = 701,
            pgCHAROID    = 18,
            pgNAMEOID    = 19,
            pgTEXTOID    = 25,
            pgVARCHAROID = 1043,
            pgBYTEAOID   = 17,
            pgTIMESTAMPOID = 1114,

            pgBOOLARRAYOID    = 1000,
            pgINT2ARRAYOID    = 1005,
            pgINT4ARRAYOID    = 1007,
            pgINT8ARRAYOID    = 1016,
            pgFLOAT4ARRAYOID  = 1021,
            pgFLOAT8ARRAYOID  = 1022,

            pgCHARARRAYOID    = 1002,
            pgNAMEARRAYOID    = 1003,
            pgTEXTARRAYOID    = 1009,
            pgVARCHARARRAYOID = 1015,
            pgBYTEAARRAYOID   = 1001,
            pgTIMESTAMPARRAYOID = 1115
        };
};

#endif
