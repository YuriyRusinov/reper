#ifndef KKSFILELOADER_H
#define KKSFILELOADER_H

#include "kksfactory_config.h"

#include <QString>

class KKSResult;
class KKSDatabase;

class _F_EXPORT KKSFileLoader
{
    protected:
        friend class KKSSito;
        friend class KKSSitoWeb;

        KKSFileLoader();
        virtual ~KKSFileLoader();

        KKSDatabase * m_db;

    public:

        void setDb(KKSDatabase * db);
        KKSDatabase * getDb() const;

        //
        //работа с файлами
        //
        int     rGetFile  ( int idUrl, 
                            QString toUrl, 
                            int blockSize = 100000 
                            ) const;

        int     rGetFile  ( QString fromUrl, 
                            QString toUrl, 
                            int blockSize = 100000 
                            ) const;

        QString rGetAbsUrl( int idUrl ) const;

        int     rSetUrl(    int idUrl,
                            QString url
                            ) const;
        
        int     rWriteFile(int idUrl, 
                             QString fromUrl, 
                             bool safe, 
                             int blockSize = 100000 ) const;

        int     rDeleteFile( int idUrl ) const;
        
        int     rRemoveUrl( int idUrl ) const;

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
