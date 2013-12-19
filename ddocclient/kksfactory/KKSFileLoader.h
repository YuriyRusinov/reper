#ifndef KKSFILELOADER_H
#define KKSFILELOADER_H

#include "kksfactory_config.h"
#include <defines.h>

#include <QString>
#include <QObject>

class KKSResult;
class KKSDatabase;
class QWidget;

class _F_EXPORT KKSFileLoader : public QObject
{
    Q_OBJECT   
protected:
        friend class KKSSito;
        friend class KKSSitoWeb;

        KKSFileLoader();
        virtual ~KKSFileLoader();

        KKSDatabase * m_db;

    public:

        void setDb(KKSDatabase * db);
        KKSDatabase * getDb() const;

        int getDefaultBlockSize() const;
        //
        //работа с файлами
        //
        int     rGetFile  ( int idUrl, 
                            QString toUrl, 
                            int blockSize = _MAX_FILE_BLOCK,
                            QWidget * parent = NULL
                            ) const;

        int     rGetFile  ( QString fromUrl, 
                            QString toUrl, 
                            int blockSize = _MAX_FILE_BLOCK,
                            QWidget * parent = NULL
                            ) const;

        int     rGetGISFiles(bool bForRec, 
                             const QString & homeDir, 
                             qint64 idObj, 
                             int blockSize = _MAX_FILE_BLOCK, 
                             QWidget * parent = NULL) const;

        QString rGetAbsUrl( int idUrl ) const;

        int     rSetUrl(    int idUrl,
                            QString url
                            ) const;
        
        int     rWriteFile(int idUrl, 
                             QString fromUrl, 
                             bool safe, 
                             int blockSize = _MAX_FILE_BLOCK,
                             QWidget * parent = NULL) const;

        int     rDeleteFile( int idUrl ) const;
        
        int     rRemoveUrl( int idUrl ) const;
        
        int     ioRemoveGISFiles(qint64 idObj, bool withFiles) const;
        int     recRemoveGISFiles(qint64 idRecord, bool withFiles) const;

        qint64  rGetFileSize(int idUrl) const;

        QString raGetPath(  ) const;


    private:

        bool getResultBool(KKSResult * result) const;
        int getResultCode(KKSResult * result) const;
        QString getResultString(KKSResult * result) const;
        KKSResult * getResultDataset(KKSResult * result) const;

        void updateLastError(KKSResult * result) const;
        void clearLastError() const;
};
#endif
