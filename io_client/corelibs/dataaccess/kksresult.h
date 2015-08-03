#ifndef KKSRESULT_H
#define KKSRESULT_H

#include <QVariant>
#include <QStringList>
#include <QDateTime>

#include "config_dataaccess.h"

class __DA_EXPORT KKSResult
{
    public:
        KKSResult();
        virtual ~KKSResult();

        enum ResultStatus{
            EmptyQuery = 0,
            CommandOk = 1,
            TuplesOk = 2,
            CopyOut = 3,
            CopyIn = 4,
            BadResponse = 5,
            NonfatalError = 6,
            FatalError = 7,
            Unknown = 8
        };

        enum DataType{
            dtUnknown = -1,
            dtBool    = 0,
            dtChar    = 1,
            dtName    = 2,
            dtInt2    = 3,
            dtInt4    = 4,
            dtInt8    = 5,
            dtFloat4  = 6,
            dtFloat8  = 7,
            dtText    = 8,
            dtVarchar = 9,
            dtBytea   = 10,
            dtTimestamp = 11,

            dtBoolArray    = 12,
            dtCharArray    = 13,
            dtNameArray    = 14,
            dtInt2Array    = 15,
            dtInt4Array    = 16,
            dtInt8Array    = 17,
            dtFloat4Array  = 18,
            dtFloat8Array  = 19,
            dtTextArray    = 20,
            dtVarcharArray = 21,
            dtByteaArray   = 22,
            dtTimestampArray = 23,
            dtGeometry     = 24
        };

        virtual DataType getColumnDataType(int column) const = 0;
        virtual int getColumnNumber(const char * columnName) const = 0;
        virtual const char * getColumnName(int column) const = 0;
        QString getColumnNameAsString(int column) const;


        virtual unsigned char * getCellData( int row, int column ) const = 0; // возвращает результат запроса в виде char *
        virtual int getCellLength( int row, int column ) const = 0;
        
        virtual QVariant getCell(int row, int column) const = 0; // Возвращает результат sql-запроса в формате QVariant
        virtual QByteArray getCellAsByteArray (int row, int column) const = 0; // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea
        
        int getCellAsInt(int row, int column, bool * ok = 0) const; // Возвращает результат sql-запроса в виде целого числа
        qlonglong getCellAsInt64(int row, int column, bool * ok = 0) const; // Возвращает результат sql-запроса в виде целого числа
        bool getCellAsBool(int row, int column, bool * ok = 0) const; // Возвращает результат sql-запроса в булевом виде
        QString getCellAsString(int row, int column, bool * ok = 0) const; // строка
        double getCellAsDouble(int row, int column, bool * ok = 0) const; // число двойной точности
        QStringList getCellAsStringList(int row, int column, bool * ok = 0) const; // список строк
        QDateTime getCellAsDateTime(int row, int column, bool * ok = 0) const; //QDateTime


        virtual bool isEmpty(int row, int column) const = 0;
        virtual int getRowCount() const = 0;
        virtual int getColumnCount() const = 0;
        virtual ResultStatus resultStatus() const = 0;
        virtual QString errorMessage() const = 0;
        virtual QString errorCode() const = 0;

        virtual void next() = 0;
        virtual void prev() = 0;

        inline unsigned char * getCellData( int column ) const
        { 
            return getCellData( currentRow, column ); 
        }
        inline QVariant getCell(int column) const
        {
            return getCell(currentRow, column);
        }
        inline int getCellAsInt(int column, bool * ok = 0) const
        {
            return getCellAsInt(currentRow, column, ok);
        }
        inline qlonglong getCellAsInt64(int column, bool * ok = 0) const
        {
            return getCellAsInt64(currentRow, column, ok);
        }
        inline bool getCellAsBool(int column, bool * ok = 0) const
        {
            return getCellAsBool(currentRow, column, ok);
        }
        inline QString getCellAsString(int column, bool * ok = 0) const
        {
            return getCellAsString(currentRow, column, ok);
        }
        inline double getCellAsDouble(int column, bool * ok = 0) const
        {
            return getCellAsDouble(currentRow, column, ok);
        }
        inline QStringList getCellAsStringList(int column, bool * ok = 0) const
        {
            return getCellAsStringList(currentRow, column, ok);
        }
        inline QDateTime getCellAsDateTime(int column, bool * ok = 0) const
        {
            return getCellAsDateTime(currentRow, column, ok);
        }

        inline virtual bool isEmpty(int column) const
        {
            return isEmpty(currentRow, column);
        }

    protected:
        int currentRow;
};

#endif
