#include "KKSFileLoader.h"

#include <QtDebug>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>
#include <QMessageBox>

//#include "kkssito.h"
//#include "kkserror.h"
#include "kksresult.h"
#include "kksdatabase.h"
#include "defines.h"


KKSFileLoader::KKSFileLoader( )
{
    m_db = NULL;
}

KKSFileLoader::~ KKSFileLoader( )
{
}

void KKSFileLoader::setDb(KKSDatabase * _db)
{
    m_db = _db;
}

KKSDatabase * KKSFileLoader::getDb() const
{
    return m_db;
}


/*!\brief Метод задает новый URL паспорту растра.

Т.е. после вызова считается, что растр, описываемый в данном паспорте, хранится по заданному пути. Должен задаваться путь ОТНОСИТЕЛЬНО пути к файловому хранилищу.

Вызывается функция БСФ rSetUrl(int4, int4, varchar).
  
  \param idUrl идентификатор паспорта растра
  \param url новый URL
  
  \return 1 при успехе
  \return -1, если произошла ошибка при выполнении запроса к БГД
*/
int	KKSFileLoader::rSetUrl(int idUrl, QString newUrl ) const
{
    QString sql = QString("select rSetUrl(%1, '%2');").arg(idUrl).arg(newUrl);

    KKSResult * res = m_db->execute(sql);
    
    return getResultCode(res);
}



/*!\brief Метод возвращает абсолютный путь к файлу растра.

  Вызывается функция БСФ rGetAbsUrl(int4, int4, bool).

  В случае ошибки или если указан неверный идентификатор паспорта растра возвращается QString::null.
  
  В отличие от метода rGetUrl(), возвращается путь с учетом расположения файлового хранилища (т.е. конкатенация значений, возвращаемых методами raGetPath() и rGetUrl()).
  
  Если метод rGetUrl() вернет QString::null, то описываемый метод также вернет QString::null. Если паспорта растра с заданным идентификатором нет, то возвращается QString::null.
  
  Прользователь, выполняющий данную операцию должен иметь право на чтение информации из данного файлового архива.

\param idRasterArchive идентификатор файлового хранилища
\param idPassport идентификатор паспорта растра.
\param copyFromExternal флаг, определяющий необходимость копирования растра из внешнего хранилища в данный файл. Важно отметить, что если растр хранится во внешнем хранилище, то он физически отсутствует по указанному пути. Этот путь используется как временное хранилище для него, чтобы с ним было возможно производить различные манипуляции. По умолчанию FALSE.
*/
QString KKSFileLoader::rGetAbsUrl( int idUrl ) const
{
    QString sql = QString("select rGetAbsUrl(%1);").arg(idUrl);

    KKSResult * res = m_db->execute(sql);
    
    return getResultString(res);
}

qint64 KKSFileLoader::rGetFileSize(int idUrl) const
{
    QString sql = QString("select rGetFileSize(%1);").arg(idUrl);

    KKSResult * res = m_db->execute(sql);
    
    qint64 fileSize = 0;
    if(!res || res->getCellAsInt64(0, 0) <= 0){
        if(res)
            delete res;

        return 0;
    }
    
    fileSize = res->getCellAsInt64(0, 0);
    delete res;

    return fileSize;

}

int KKSFileLoader::getDefaultBlockSize() const
{
    return _MAX_FILE_BLOCK;
}

/*!\brief Метод возвращает и сохраняет в файле локальной файловой системы заданный файл.

Вызываются функции БСФ rGetAbsUrl(int4, int4) и rGetRaster(varchar, int4, int8).

\param idUrl идентификатор паспорта файла
\param url путь и название файла, в котором должен быть сохранен растр
\param blockSize размер блока данных в байтах, получаемых за один раз при чтении из бинарного курсора. Будьте аккуратны. При очень больших размерах блока может возникнуть недостаток оперативной памяти. Если в качестве значения параметра указан 0 или отрицательное число, то используется значение по умолчанию 100000 байт. Не рекомендуется указывать как очень маленькое, так и очень большое значение (это влияет на быстродействие). Оптимальным является значение, используемое по умолчанию.
\param url абсолютный или относительный путь к файлу в котором необходимо сохранить растр


\return 1, при успехе
\return -1 при ошибке
*/
int KKSFileLoader::rGetFile(int idUrl, QString toUrl, int blockSize, QWidget * parent) const
{
    if(blockSize <= 0)
        blockSize = getDefaultBlockSize();
    
    QTime t;
    t.start();

    QFile file(toUrl);
    bool ok = file.open(QIODevice::WriteOnly);
    if(!ok){
        return -1;
    }

    QString tmp = rGetAbsUrl(idUrl);
    if(tmp.isEmpty())
        return ERROR_CODE;

    QByteArray rPath = tmp.toUtf8();
    const char * rasterPath = rPath.constData();


    char * command = new char[120 + strlen(rasterPath)];

    sprintf(command, "select rGetFileByUrl('%s', %d, $1::int8);", rasterPath, blockSize);
    qDebug () << __PRETTY_FUNCTION__ << command << blockSize;

    qint64 position = 0;
    int  paramTypes[1];
    int  paramLengths[1]; 
    int  paramFormats[1];
    char ** paramValues = new char * [1];
    paramValues[0] = new char[40];

    QProgressDialog *pImportD = NULL;
    QLabel *lImport = NULL;
    qint64 estimatedFileSize = 0;
    double k = 1.0;//используется для масштабирования шкалы прогрессбара при условии, что размер файла больше _MAX_INT32_

    if(parent){
        pImportD = new QProgressDialog (parent);
		lImport = new QLabel (QObject::tr("Download file, please wait..."), pImportD);
        pImportD->setLabel (lImport);
		QPushButton *pbCancelButton = new QPushButton (QObject::tr("Cancel download"), pImportD);
        pImportD->setCancelButton (pbCancelButton);
        QProgressBar *pBar = new QProgressBar (pImportD);
        pImportD->setBar (pBar);

        pBar->setMinimum (0);

        estimatedFileSize = rGetFileSize(idUrl);
        qint64 maxInt32 = (qint64)_MAX_INT32_;
        
        if(estimatedFileSize > maxInt32){
            k = (double)estimatedFileSize / maxInt32;
            pBar->setMaximum (maxInt32);
        }
        else
            pBar->setMaximum (estimatedFileSize);

		pImportD->setWindowTitle (QObject::tr("Downloading file"));
        pImportD->setWindowModality (Qt::WindowModal);
        QObject::connect (pbCancelButton, SIGNAL (clicked()), pImportD, SLOT (cancel()) );

        pImportD->setMinimumDuration (0);
        pImportD->show ();
    }

    do{
        sprintf(paramValues[0], "%lld", position);
        paramTypes[0]   = KKSResult::dtInt8;
        paramFormats[0] = 0; //text
        paramLengths[0]  = 0;

        KKSResult * res = m_db->execParams(command, 
                                          1, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          1);

        position += blockSize;

        if(pImportD){
            qint64 timeElapsed = (qint64)(t.elapsed());//в миллисекундах
            double V = ((double)estimatedFileSize * timeElapsed) / position;
            double avgSpeed = ((double)position * 1000) / (timeElapsed * 1024);//средняя скорость  в Kb/sec
            double E = (V - (double)timeElapsed) / 1000; //оталось времени дозавершения процесса копирования в секундах
            QTime tt(0, 0, 0);

			lImport->setText (QObject::tr("Downloading file, please wait... \n\n"
                                         "Time elapsed: %1\n"
                                         "Time estimated: %2\n"
                                         "Avg speed: %3 Kb/sec").arg(tt.addSecs((int)timeElapsed/1000).toString("hh:mm:ss"))
                                                                .arg(tt.addSecs((int)E).toString("hh:mm:ss"))
                                                                .arg((int)avgSpeed));

            pImportD->setValue (position/k);
            if (pImportD->wasCanceled())
            {
                //pImportD->setParent (0);
                //delete pImportD;
                updateLastError(res);
                if(res)
                    delete res;
                break;
            }
        }

        if (!res){
            updateLastError(NULL);

            file.close();
            delete[] paramValues[0];
            delete[] paramValues;
            delete[] command;
            return ERROR_CODE;
        }

        if(res->getRowCount() < 1){ 
            updateLastError(res);
            delete res;
            break;
        }
        if(res->isEmpty(0, 0)){
            updateLastError(res);
            delete res;
            break;
        }
        QByteArray bytea = res->getCellAsByteArray(0, 0);
        if(! bytea.isEmpty() ){
            qint64 r = file.write(bytea);
            if(r == -1){
                updateLastError(res);
                delete res;
                file.close();
                delete[] paramValues[0];
                delete[] paramValues;
                delete[] command;
                return ERROR_CODE;
            }
        }
        else{
            delete res;
            break;
        }

        delete res;
    }while(1);

    
    qWarning() << QString("file transferred from server in ") 
               << t.restart() 
               << QString("ms");

    file.close();
    delete[] command;
    delete[] paramValues[0];
    delete[] paramValues;
    
    clearLastError();

    if(pImportD){
        if(pImportD->wasCanceled()){
            int yes = QMessageBox::question(pImportD, 
				                            QObject::tr("Cancel download"), 
                                            QObject::tr("Download was cancelled by user. Delete downloaded part of the file?"), 
                                            QMessageBox::Yes | QMessageBox::No, 
                                            QMessageBox::Yes);
            if(yes == QMessageBox::Yes){
                file.remove();
            }
        }
        pImportD->hide ();
        pImportD->setParent (0);
        delete pImportD;

        //return ERROR_CODE;
    }

    return OK_CODE;
}



int KKSFileLoader::rGetFile(QString fromUrl, QString toUrl, int blockSize, QWidget * parent) const
{
    Q_UNUSED(parent);

    if(blockSize <= 0)
        blockSize = getDefaultBlockSize();
    QTime t;
    t.start();

    QFile file(toUrl);
    bool ok = file.open(QIODevice::WriteOnly);
    if(!ok){
        return -1;
    }

    QByteArray rPath = fromUrl.toUtf8();
    const char * rasterPath = rPath.constData();


    char * command = new char[120 + strlen(rasterPath)];

    sprintf(command, "select rGetFileByUrl('%s', %d, $1::int8);", rasterPath, blockSize);

    qint64 position = 0;
    int  paramTypes[1];
    int  paramLengths[1]; 
    int  paramFormats[1];
    char ** paramValues = new char * [1];
    paramValues[0] = new char[40];

    do{
        sprintf(paramValues[0], "%lld", position);
        paramTypes[0]   = KKSResult::dtInt8;
        paramFormats[0] = 0; //text
        paramLengths[0]  = 0;

        KKSResult * res = m_db->execParams(command, 
                                          1, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          1);

        position += blockSize;
        if (!res){
            updateLastError(NULL);

            file.close();
            delete[] paramValues[0];
            delete[] paramValues;
            delete[] command;
            return ERROR_CODE;
        }

        if(res->getRowCount() < 1){ 
            updateLastError(res);
            delete res;
            break;
        }
        if(res->isEmpty(0, 0)){
            updateLastError(res);
            delete res;
            break;
        }
        QByteArray bytea = res->getCellAsByteArray(0, 0);
        if(! bytea.isEmpty() ){
            qint64 r = file.write(bytea);
            if(r == -1){
                updateLastError(res);
                delete res;
                file.close();
                delete[] paramValues[0];
                delete[] paramValues;
                delete[] command;
                return ERROR_CODE;
            }
        }
        else{
            delete res;
            break;
        }

        delete res;
    }while(1);

    qWarning() << QString("file transferred from server in ") 
               << t.restart() 
               << QString("ms");

    file.close();
    delete[] command;
    delete[] paramValues[0];
    delete[] paramValues;
    
    clearLastError();
    return OK_CODE;
}



/*!\brief Метод осуществляет запись растра из локального файла в соответствующее файловое хранилище на сервере.

\param idRasterArchive идентификатор растрового хранилища, в которое осуществляется запись растра
\param idPassport идентификатор паспорта растра в БД. Паспорт должен быть создан заранее.
\param url Путь к файлу, в котором содержится растр. Путь должен быть доступен текущему процессу клиентского приложения
\param safe Если TRUE, то в случае существования файла, связанного с заданным паспортом растра, в файловом хранилище функция не выполняет никаких действий. В противном случае файл перезаписывается.
\param blockSize размер блока данных, который передается за один запрос к БД.

\return 1 при успехе
\return -1 при ошибке
*/
int KKSFileLoader::rWriteFile( int idUrl, 
                              QString fromUrl,
                              bool safe, 
                              int blockSize,
                              QWidget * parent) const
{
    if(blockSize <= 0)
        blockSize = getDefaultBlockSize();

    QTime t;
    t.start();

    QFile file(fromUrl);
    bool ok = file.open(QIODevice::ReadOnly);
    if (!ok){
        return ERROR_CODE;
    }

    if (file.atEnd())
    {
        file.close();
        qDebug () << __PRETTY_FUNCTION__ << "End of file";
        return ERROR_CODE;
    }

    char * command = new char[100];
    int nParams = 2;
    sprintf(command, "select rWriteFile(%d, $1, $2);", idUrl);
    qDebug () << __PRETTY_FUNCTION__ << command << blockSize;
    
    int mode = safe ? 2 : 0;

    int  * paramTypes = new int[nParams];
    int  * paramLengths = new int[nParams]; 
    int  * paramFormats = new int[nParams];
    char ** paramValues = new char * [nParams];
    paramValues[0] = new char[blockSize];

    QProgressDialog *pImportD = NULL;
    QLabel *lImport = NULL;
    double k = 1.0;//используется для масштабирования шкалы прогрессбара при условии, что размер файла больше _MAX_INT32_
    qint64 estimatedFileSize = 0;
    
    if(parent){
        pImportD = new QProgressDialog (parent);
        lImport = new QLabel (QObject::tr("Uploading file, please wait..."), pImportD);
        pImportD->setLabel (lImport);
        QPushButton *pbCancelButton = new QPushButton (QObject::tr("Cancel upload"), pImportD);
        pImportD->setCancelButton (pbCancelButton);
        QProgressBar *pBar = new QProgressBar (pImportD);
        pImportD->setBar (pBar);

        pBar->setMinimum (0);
        
        estimatedFileSize = file.size();
        qint64 maxInt32 = (qint64)_MAX_INT32_;
        
        if(estimatedFileSize > maxInt32){
            k = (double)estimatedFileSize / maxInt32;
            pBar->setMaximum (maxInt32);
        }
        else
            pBar->setMaximum (estimatedFileSize);

        
        
        

        pImportD->setWindowTitle (QObject::tr("Uploading file"));
        pImportD->setWindowModality (Qt::WindowModal);
        QObject::connect (pbCancelButton, SIGNAL (clicked()), pImportD, SLOT (cancel()) );

        pImportD->setMinimumDuration (0);
        pImportD->show ();
    }
    
    qint64 position = 0;


    while( ! file.atEnd() ){
        qint64 size = file.read(paramValues[0], blockSize);
        
        if(size < 1){
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            file.seek(0);
            file.close();
            qWarning() << QObject::tr("Time elapsed ")  << t.restart()  << QString("ms");
            return ERROR_CODE;
        }

        if(pImportD){
            position = position + size;
            qint64 timeElapsed = (qint64)(t.elapsed());//в миллисекундах
            
            double V = ((double)estimatedFileSize * timeElapsed) / position;
            double avgSpeed = ((double)position * 1000) / (timeElapsed * 1024);//средняя скорость  в Kb/sec

            double E = (V - (double)timeElapsed) / 1000; //оталось времени дозавершения процесса копирования в секундах
            
            QTime tt(0, 0, 0); 
            
            lImport->setText (QObject::tr("Uploading file, please wait... \n\n"
                                         "Time elapsed: %1\n"
                                         "Time estimated: %2\n"
                                         "Avg speed: %3 Kb/sec").arg(tt.addSecs((int)timeElapsed/1000).toString("hh:mm:ss"))
                                                                .arg(tt.addSecs((int)E).toString("hh:mm:ss"))
                                                                .arg((int)avgSpeed));

            pImportD->setValue (position/k);
            if (pImportD->wasCanceled())
            {
                break;
            }
        }

        paramTypes[0]   = KKSResult::dtBytea;
        paramFormats[0] = 1; //binary
        paramLengths[0]  = size;

        paramTypes[1]   = KKSResult::dtInt4;
        paramFormats[1] = 0; //non binary
        paramLengths[1]  = 0;
        if(mode == 0)
            paramValues[1] = "0";
        else if(mode == 1)
            paramValues[1] = "1";
        else 
            paramValues[1] = "2";

        KKSResult * res = m_db->execParams(command, 
                                          nParams, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          0);

        if(!res){
            updateLastError(NULL);
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            qWarning() << QObject::tr("Time elapsed ")  << t.restart()  << QString("ms");
            return ERROR_CODE;
        }
        
        int status = res->getCellAsInt(0, 0);
        if(status <= 0 ){
            updateLastError(res);
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            qWarning() << QObject::tr("Time elapsed ")  << t.restart()  << QString("ms");
            return ERROR_CODE;
        }
        mode = 1;
        delete res;
    }

    delete[] paramValues[0];
    delete[] paramTypes;
    delete[] paramLengths;
    delete[] paramFormats;
    delete[] paramValues;
    delete[] command;

    
    file.seek(0);
    file.close();

    qWarning() << QObject::tr("file transferred to server in ") 
               << t.restart() 
               << QString("ms");

    clearLastError();

    if(pImportD){
        if(pImportD->wasCanceled()){
            int yes = QMessageBox::question(pImportD, 
                                            QObject::tr("Cancel uploading"), 
                                            QObject::tr("Uploading was cancelled by user. Delete uploaded part of the file on server?"), 
                                            QMessageBox::Yes | QMessageBox::No, 
                                            QMessageBox::Yes);
            if(yes == QMessageBox::Yes){
                int ok = rRemoveUrl(idUrl);
                if(ok <= 0){
                    qCritical() << QObject::tr("Cannot delete uploaded part of file");
                    QMessageBox::critical(pImportD, QObject::tr("Error"), QObject::tr("Cannot delete uploaded part of file"));
                }
            }

            pImportD->hide ();
            pImportD->setParent (0);
            delete pImportD;
            
            return ERROR_CODE;

        }

        pImportD->hide ();
        pImportD->setParent (0);
        delete pImportD;
    }

    QString sql = QString("select rSetUploaded(%1, true)").arg(idUrl);
    KKSResult * res = m_db->execute(sql);
    if(!res || res->getRowCount() < 1){
        qWarning() << __PRETTY_FUNCTION__ << "Error! Cannot set file as uploaded! ID = " << idUrl;
    }
    if(res)
        delete res;

    return OK_CODE;
}


int KKSFileLoader::rRemoveUrl( int idUrl ) const
{
    QString sql =
            QString("select rRemoveUrl(%1);").arg (idUrl);

    KKSResult * res = m_db->execute (sql);

    return getResultCode(res);
}

int KKSFileLoader::rDeleteFile( int idUrl ) const
{
    QString sql =
            QString("select rDeleteFile(%1);").arg (idUrl);

    KKSResult * res = m_db->execute (sql);

    return getResultCode(res);
}


/*!\brief Метод возвращает путь к файловому хранилищу растров относительно сервера БГД.

  Вызывается функция БСФ raGetPath(int4)

Хранилище задается своим идентификатором.

Пользователь должен входить в глобальную группу RASelector, а также иметь право на чтение информации о данном хранилище.

\param idRasterArchive идентификатор файлового хранилища
*/
QString KKSFileLoader::raGetPath(  ) const
{
    QString sql = QString("select raGetPath();");

    KKSResult * res = m_db->execute(sql);
    
    return getResultString(res);
}


/*======================*/

bool KKSFileLoader::getResultBool(KKSResult * res) const
{
    if(!res || res->resultStatus() != KKSResult::TuplesOk){
        updateLastError(res);
        if(res)
            delete res;
        return FALSE;
    }
    
    clearLastError();
    
    bool ret = res->getCellAsBool(0, 0);
    delete res;

    return ret;
}

int KKSFileLoader::getResultCode(KKSResult * res) const
{
    if(!res || 
        res->resultStatus() != KKSResult::TuplesOk || 
        res->getCellAsInt(0, 0) <= ERROR_CODE)
    {
        updateLastError(res);
        if(res)
            delete res;
        return ERROR_CODE;
    }
    
    clearLastError();
    
    int ret = res->getCellAsInt(0, 0);
    delete res;

    return ret;
}

QString KKSFileLoader::getResultString(KKSResult * res) const
{
    if(!res || res->resultStatus() != KKSResult::TuplesOk){
        updateLastError(res);
        if(res)
            delete res;
        return QString::null;
    }
    
    clearLastError();
    
    if(res->isEmpty(0, 0)){
        delete res;
        return QString::null;
    }
    
    QString str = res->getCellAsString(0, 0);

    delete res;

    return str;
}

KKSResult * KKSFileLoader::getResultDataset(KKSResult * res) const
{

    if(!res || (res->resultStatus() != KKSResult::TuplesOk && res->resultStatus() != KKSResult::CommandOk) ){
        updateLastError(res);
        if(res) 
            delete res;
        return NULL;
    }

    clearLastError();
    
    if( res->resultStatus() == KKSResult::TuplesOk && res->getRowCount() == 0){
        delete res;
        return NULL;
    }
    return res;
}

void KKSFileLoader::updateLastError(KKSResult * result) const
{
    Q_UNUSED(result);
/*
    KKSError * error = xGis->getLastError();
    if(!error)
        return;

    if(!result){
        error->setType(KKSError::etPGError);
        error->setCode("UNDEF");
        error->setName("XGIS Server function execution error");
        error->setMessage( QObject::tr(m_db->lastError()) );
        error->setGroup( QObject::tr("Undefined") );
        error->setData("");
        return;
    }
    
    //if(result->getCellAsInt(0, 0) != ERROR_CODE){}
    
    KKSResult * res = m_db->execute ("select * from getLastError()");

    if(!res)
    {
        error->setType(KKSError::etPGError);
        error->setCode("UNDEF");
        error->setName("XGIS Server function execution error");
        error->setMessage( QObject::tr(m_db->lastError()) );
        error->setGroup( QObject::tr("Undefined") );
        error->setData("");
        return;
    }
    if(res && res->resultStatus() != KKSResult::TuplesOk)
    {
        error->setType(KKSError::etPGError);
        error->setCode(res->errorCode());
        error->setName(res->errorCode());
        error->setMessage(res->errorMessage());
        error->setGroup(QObject::tr("Not defined"));
        error->setData("");
        return;
    }

    error->setType(KKSError::etPGError);
    error->setCode(res->getCellAsString(0, 0));
    error->setName(res->getCellAsString(0, 1));
    error->setMessage(res->getCellAsString(0, 2));
    error->setGroup(res->getCellAsString(0, 4));
    error->setData(res->getCellAsString(0, 5));

    delete res;
    */
}
void KKSFileLoader::clearLastError() const
{
    /*
    KKSError * error = kksSito->getLastError();
    if(!error)
        return;

    error->setType(KKSError::etNoError);
    error->setCode("00000");
    error->setName("SUCCESSFUL COMPLETION");
    error->setMessage("SUCCESSFUL COMPLETION");
    error->setGroup("SUCCESSFUL COMPLETION");
    error->setData("");
    */
}
