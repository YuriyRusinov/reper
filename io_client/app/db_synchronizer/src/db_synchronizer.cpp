#include <QtGui>

#include "ui_db_synchronizer_form.h"
#include "db_synchronizer.h"
#include "siu_connect_form.h"

#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <kkssito.h>
#include <KKSLoader.h>
#include <KKSFileLoader.h>
#include <QFile>



DBSynchronizer::DBSynchronizer(QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::db_synchronizer_form)
{
    UI->setupUi (this);

	UI->deDate->setDate(QDate::currentDate());
	UI->pbStopTimer->setEnabled(false);
	
	int ok = kksSito->GUIConnect();
	if(ok != OK_CODE)
		return;

	db = kksSito->db();

    m_fileTypes = kksSito->loader()->loadFileTypes();

	ok = initCacheTable();//в том числе установили дату с которой надо загружать данные
	if(ok != OK_CODE)
		return;

	rootFileStore = "./";

	UI->teLog->setWordWrapMode(QTextOption::NoWrap);
	UI->teLogFile->setWordWrapMode(QTextOption::NoWrap);

	connected = false;
	//connectSIU();

	connect(UI->pbSetTimer, SIGNAL(clicked()), this, SLOT(setTimer()));
	connect(UI->pbStopTimer, SIGNAL(clicked()), this, SLOT(stopTimer()));
	connect(UI->pbQueryNow, SIGNAL(clicked()), this, SLOT(analyzeNow()));
	connect(UI->pbSIUConnect, SIGNAL(clicked()), this, SLOT(SIUConnect()));
	connect(UI->pbCancel, SIGNAL(clicked()), this, SLOT(close()));

}

int DBSynchronizer::initCacheTable()
{
	if(!db)
		return 0;

	KKSResult * res = db->execute("select f_is_table_exist('vto_siu_cache_table', 'public')");
	if(!res)
		return 0;

	int cnt = res->getCellAsInt(0, 0);
	delete res;
	
	if(cnt == 1){
		res = db->execute("select last_date from vto_siu_cache_table order by id limit 1");
		if(!res)
			return 0;
		
		lastDateSys = res->getCellAsDateTime(0, 0).date();

		delete res;
		
	    return 1;
	}

	int r = db->executeCmd("create table vto_siu_cache_table (id serial, last_date date);");
	if(r != OK_CODE)
		return 0;

	//lastDate = invalid;

	return 1;
}

void DBSynchronizer::SIUConnect()
{
	if(connected){
		QMessageBox::warning(this, tr("Warning"), tr("You are already successfully connected to SIU database!"), QMessageBox::Ok);
		return;
	}

	SIUConnectForm * f = new SIUConnectForm(this);
	int res = f->exec();
	if(res != QDialog::Accepted)
	{
	    return;
	}

    siuIP = f->getIP();
    siuPort = f->getPort();
    siuUser = f->getUser();
    siuPasswd = f->getPasswd();
    siuConstraint = f->getConstraint().toInt();
    siuSOP = f->getSOP();


	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Connecting to SIU database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	dbSIU.setTimeout(3000);
    bool ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
	if(ok){
		connected = true;
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull connecting to SIU database!\n")); 
		UI->teLog->moveCursor(QTextCursor::End);

		//connect(&dbSIU, 
		//	    SIGNAL(downloadFileFinished(QFile *)), 
		//		this, 
		//		SLOT(saveFileInDD(QFile *)));
		
		connect(&dbSIU, 
			    SIGNAL(downloadFileProgress(QString, QString, qint64, qint64)), 
			    this, 
				SLOT(downloadFileProgress(QString, QString, qint64, qint64)));
	}
	else{
		connected = false;
		UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while connecting to SIU database!\n")); 
		UI->teLog->moveCursor(QTextCursor::End);
	}

}


void DBSynchronizer::analyzeSIU()
{
    if(!connected)
		return;

    iTimeout = UI->sbTimeout->value() * 1000;//in seconds
    iReconnect = UI->sbReconnect->value();

    bool b1 = UI->pbSetTimer->isEnabled();
	bool b2 = UI->pbStopTimer->isEnabled();
	UI->pbQueryNow->setEnabled(false);
	UI->pbSetTimer->setEnabled(false);
	UI->pbStopTimer->setEnabled(false);
	UI->cbInterval->setEnabled(false);
	UI->sbTimer->setEnabled(false);

	timer.stop();
    
	UI->teLog->setPlainText(UI->teLog->toPlainText() + 
		                    tr("\n ========================\n") + 
					        QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + 
					        tr("\nStart querying SIU database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

    if(UI->lwIRTypes->item(0)->checkState() == Qt::Checked)
        getETK();
	if(UI->lwIRTypes->item(1)->checkState() == Qt::Checked)
        getGMO();
	if(UI->lwIRTypes->item(2)->checkState() == Qt::Checked)
        getNVO();
	if(UI->lwIRTypes->item(3)->checkState() == Qt::Checked)
        getEOIRD();
	if(UI->lwIRTypes->item(4)->checkState() == Qt::Checked)
        getEFO();

    if(UI->lwIRTypes->item(5)->checkState() == Qt::Checked)
        getUGSH();
	if(UI->lwIRTypes->item(6)->checkState() == Qt::Checked)
        getTTX();
    if(UI->lwIRTypes->item(7)->checkState() == Qt::Checked)
        getPG();
	if(UI->lwIRTypes->item(8)->checkState() == Qt::Checked)
        getPOR();
	if(UI->lwIRTypes->item(9)->checkState() == Qt::Checked)
        getRBP();
	if(UI->lwIRTypes->item(10)->checkState() == Qt::Checked)
        getREQ();
	if(UI->lwIRTypes->item(11)->checkState() == Qt::Checked)
        getOPVTO();

	UI->teLog->setPlainText(UI->teLog->toPlainText() + 
		                    tr("\n ++++++++++++++++++++++ \n") + 
					        QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + 
                            tr("\nEnd of querying SIU database.\n")); 
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = updateStartDate();
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
		                        tr("An ERROR was occured while updating start date filter in DynamicDocs database! \n")); 
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->pbQueryNow->setEnabled(true);
	UI->pbSetTimer->setEnabled(b1);
	UI->pbStopTimer->setEnabled(b2);
	UI->cbInterval->setEnabled(true);
	UI->sbTimer->setEnabled(true);

	timer.start();
}

int DBSynchronizer::createIO(int idCategory, 
			                 const QString & uid, 
			                 const QString & name, 
			                 int idMaclabel, 
							 const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;

	int idOrg = kksSito->loader()->getLocalOrgId();

	QString fName = name;
	if(name.startsWith("C:\\fakepath\\"))
		fName.remove("C:\\fakepath\\");	

	//!!!!!ДЛЯ НОВОЙ ВЕРСИИ ДОБАВИТЬ ЕЩЕ ОДИН ПАРАМЕТР В ФУНКЦИЮ! R_COLOR = NULL
    QString sql = QString ("select * from ioInsert (%1::varchar, %2, %3, %4, %5, %6, %7, %8, %9::varchar, %10, %11, %12, NULL, NULL, %13, %14, %15, NULL::varchar, NULL::uuid);")
	    .arg (QString("'") + fName + QString("'"))//name
		.arg (QString::number(idCategory))//id_io_category
        .arg (QString::number (1))// admin, ибо в БД-приемнике автора (пользователя) может не быть (как правило не будет)
		.arg (QString::number (1))//id_state
        .arg (QString ("NULL::varchar"))//table_name
        .arg (QString ("NULL::varchar")) //desc
        .arg (QString ("NULL::varchar"))//info
		.arg (QString::number (idMaclabel == 0 ? 1 : idMaclabel))//id_maclabel
		.arg (QString("'") + uid + QString("'"))//unique_id
        .arg (QString::number (1)) //id_sync_type
        .arg (QString::number (idOrg)) //id_owner_org
        .arg (QString ("TRUE")) //is_global
        .arg (QString ("NULL::int8")) //bgColor
        .arg (QString ("NULL::int8")) //fgColor
        .arg (QString::number(1));//id_io_type	
	
	KKSResult * res = db->execute(sql);
	if(!res){
        UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                tr("An ERROR was occured while inserting IO in DynamicDocs database!\n") + 
							    QString("uid = %1, name = %2 \n").arg(uid).arg(name) );
		UI->teLog->moveCursor(QTextCursor::End);

		return 0;
	}
    
	int idObject = res->getCellAsInt(0, 0);
	delete res;
	
	if(idObject < 0){
        UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                tr("An ERROR was occured while inserting IO in DynamicDocs database!\n") + 
							    QString("uid = %1, name = %2 \n").arg(uid).arg(name) );
		UI->teLog->moveCursor(QTextCursor::End);
	}
	
    createIOUrls(idObject, files);

	return idObject;
}

int DBSynchronizer::insertAttrs(int idObject, const QList<QPair<int, QString> > & attrs)
{
    if(!db)
		return 0;

	QString sql;

	for (int i=0; i< attrs.count(); i++)
    {
        QPair<int, QString> attr = attrs.at(i);

		QString attrSql = QString ("select * from ioInsertAttr (%1, %2, %3, NULL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (idObject)
			.arg (QString::number(attr.first)) //здесь в качестве ключа используется id_io_attribute
			.arg (attr.second.isEmpty() ? QString ("NULL") : QString("'")+attr.second+QString("'"));
        
		KKSResult * attrRes = db->execute (attrSql);

        int attr_ier = ERROR_CODE;
        if (attrRes)
        {
            attr_ier = attrRes->getCellAsInt (0, 0);
            delete attrRes;
        }
        if (attr_ier < 0){
            UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while inserting metadata values for IO in DynamicDocs database!\n") + 
									QString("idObject = %1, idAttribute = %2, value = %3 \n").arg(idObject).arg(attr.first).arg(attr.second) );
			UI->teLog->moveCursor(QTextCursor::End);
	
            return ERROR_CODE;
        }
    }
	
	return 1;
}

void DBSynchronizer::prepareDownloadFile(const QString & uid, 
								  int index, 
								  const QString & fileLink, 
								  const QString & fileName,
								  QList<QPair<QString, QString> > & files)
{
	QString toFile = uid + "___" + QString::number(index);
	//dbSIU.downloadFile(uid, fileLink, toFile);

	QPair<QString, QString> ioUID_file;
	QString fName = fileName;
	if(fileName.startsWith("C:\\fakepath\\"))
		fName.remove("C:\\fakepath\\");

	ioUID_file.first = fName;
	ioUID_file.second = toFile;
	files.append(ioUID_file);
}



void DBSynchronizer::createPair(int idAttr, const QString & value, QList<QPair<int, QString> > & attrs)
{
    QPair<int, QString> attr;
    attr.first = idAttr;
	attr.second = value;

	attrs.append(attr);

	return;
}




void DBSynchronizer::setTimer()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(analyzeSIU()));
	
	timer.stop();

	int k = 1;
	if(UI->cbInterval->currentIndex() == 0)
		k = 1;
	else if(UI->cbInterval->currentIndex() == 1)
		k = 60;
	else 
		k = 60 * 24;

	k = UI->sbTimer->value() * 1000 * 60 * k;
	if(k <= 0)
		return;

	timer.start(k);
	UI->pbSetTimer->setEnabled(false);
	UI->pbStopTimer->setEnabled(true);

	UI->teLog->setPlainText(UI->teLog->toPlainText() + 
		                    tr("Start timer from %1 %2 (%3 milliseconds)\n")
		                             .arg(UI->sbTimer->value())
									 .arg(UI->cbInterval->currentText())
									 .arg(k));
	UI->teLog->moveCursor(QTextCursor::End);


}

void DBSynchronizer::stopTimer()
{
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(analyzeSIU()));
	timer.stop();

	UI->pbSetTimer->setEnabled(true);
	UI->pbStopTimer->setEnabled(false);

	UI->teLog->setPlainText(UI->teLog->toPlainText() + 
		                    tr("Stop timer\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}


void DBSynchronizer::analyzeNow()
{
	analyzeSIU();
}

//void DBSynchronizer::saveFileInDD(QFile * f)
void DBSynchronizer::saveFileInDD(const QString & fileName)
{
	//QString fileName = f->fileName();
	for(int i=0; i<idUrls_files.count(); i++)
	{
		if(idUrls_files.at(i).second == fileName){
			int ok = createFileInDD(idUrls_files.at(i).first, idUrls_files.at(i).second);
			if(ok < 0){
				UI->teLogFile->setPlainText(UI->teLog->toPlainText() + 
										tr("An ERROR was occured while uploading file in DynamicDocs database!\n") + 
										QString("idUrl = %1, fileUrl = %2 \n")
												.arg(idUrls_files.at(i).first)
												.arg(idUrls_files.at(i).second));
				UI->teLogFile->moveCursor(QTextCursor::End);
			
			}
			idUrls_files.removeAt(i);
			break;
		}
	}
}

void DBSynchronizer::downloadFileProgress(QString uid, QString filename, qint64 done, qint64 full)
{
	if(done > 0 && done < full){
		if(!downloadingFiles.contains(filename)){
		    UI->teLogFile->setPlainText(UI->teLogFile->toPlainText() + tr("Start downloading file %1 ...\n").arg(filename));
			UI->teLogFile->moveCursor(QTextCursor::End);
			downloadingFiles.append(filename);
		}
	}

	if(done == full){
	    saveFileInDD(filename);
		if(downloadingFiles.contains(filename)){
		    UI->teLogFile->setPlainText(UI->teLogFile->toPlainText() + tr("File %1 was successfully downloaded!\n").arg(filename));
			UI->teLogFile->moveCursor(QTextCursor::End);
			downloadingFiles.removeAll(filename);
            if(downloadingFiles.count() == 0){
                emit allFilesDownloaded();
                //qWarning() << "All files downloaded!";
                //QMessageBox::critical(this, tr("..."), tr("..."));
            }
            /*
			if(downloadingFiles.count() == 0){
			    //это означает, что все файлы, которые реально загружаись из БД СИУ, загружены на сервер БД ДД
				idUrls_files.clear();
				QMessageBox::information(this, 
					                     tr("File downloading"), 
										 tr("All of files downloaded from SIU database was written to DynamicDocs database!"), 
										 QMessageBox::Ok);
			}
			*/
		}
	}
}

int DBSynchronizer::updateStartDate()
{
    if(!db)
		return 0;

	int ok = db->executeCmd("insert into vto_siu_cache_table (last_date) values (current_date)");
	if(ok != OK_CODE)
		return 0;

	return 1;
}


int DBSynchronizer::createFileInDD(int idUrl, const QString & localUrl)
{
	int ok = kksSito->fileLoader()->rWriteFile(idUrl, localUrl, true, -1, this);

	return ok;
}


void DBSynchronizer::createIOUrls(int idObject, const QList<QPair<QString, QString> > & files)
{
    
	for(int i=0; i<files.count(); i++){
        int idFileType = 1;
        QString ext = "qq";
        QString fileName = files.at(i).first;

        ext = fileName.section(".", -1);
        if(!ext.isEmpty()){
            bool bFound = false;
            for(int j=0; j<m_fileTypes.count(); j++){
                KKSFileType * type = m_fileTypes.at(j);
                bFound = type->assotiated(ext.toLower());
                if(bFound){
                    idFileType = type->id();
                    break;
                }
                else{
                    bFound = type->assotiated(ext.toUpper());
                    if(bFound){
                        idFileType = type->id();
                        break;
                    }
                }
            }
        }
        else{
            ext = "qq";
        }
        
        
        int idUrl = rInsertUrl(fileName, idFileType, ext);
		if(idUrl < 0){
            UI->teLogFile->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while inserting file in DynamicDocs database!\n") + 
									QString("idObject = %1, fileName = %2, fileUrl = %3 \n")
									        .arg(idObject)
											.arg(files.at(i).first)
											.arg(files.at(i).second));
			UI->teLogFile->moveCursor(QTextCursor::End);
			continue;
		}

		int ok = ioInsertUrl(idObject, idUrl, files.at(i).first);
		if(ok < 0){
            UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while assigning file to IO in DynamicDocs database!\n") + 
									QString("idObject = %1, fileName = %2, fileUrl = %3 \n")
									        .arg(idObject)
											.arg(files.at(i).first)
											.arg(files.at(i).second));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
         
		QPair<int, QString> p;
		p.first = idUrl;
		p.second = files.at(i).second;
		idUrls_files.append(p);
	} 

}

int DBSynchronizer::rInsertUrl(const QString & fileName, int fileType, const QString & fileExt)
{
	QString sql = QString("select rInsertUrl('%1', 'not assigned', %2, '%3')")
                                     .arg(fileName)
                                     .arg(fileType)
                                     .arg(fileExt);
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;
    if(res->getRowCount() == 0){
        delete res;
        return ERROR_CODE;
    }

    int idUrl = res->getCellAsInt(0, 0);
        
    delete res;
        
    if(idUrl <= 0)
        return ERROR_CODE;
        
    return idUrl;
}

int DBSynchronizer::ioInsertUrl(int idObject, int idUrl, const QString & fileName)
{
	QString sql = QString("select ioInsertUrl(%1, %2, '%3')")
                          .arg(idObject)
                          .arg(idUrl)
                          .arg(fileName);

    KKSResult * res = db->execute(sql);
    int ok = res ? res->getCellAsInt(0,0) : ERROR_CODE;
    if(res)
        delete res;

    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

bool DBSynchronizer::findEqualIO(const QString & uid) const
{
    QString sql = QString("select count(av.id_io_object) \
                           from \
                               attrs_values av, attrs_categories ac \
                           where \
							   av.id_attr_category = ac.id \
							   and ac.id_io_attribute = 1294 and value = '%1'").arg(uid);
	
	KKSResult * res = db->execute(sql);
    int ok = res ? res->getCellAsInt(0,0) : 0;
    if(res)
        delete res;

    if(ok > 0)
		return true;

	return false;
}

QDate DBSynchronizer::getFilterDate() const
{
    QDate d;
	if(UI->rbDate->isChecked()){
		d = UI->deDate->date();

        UI->teLog->setPlainText(UI->teLog->toPlainText() + 
			                    tr("Using filter date. Start from %1\n").arg(d.toString("dd.MM.yyyy")) );
		UI->teLog->moveCursor(QTextCursor::End);

	}
	else if(UI->rbSysDate->isChecked()){
		d = lastDateSys;
		
		UI->teLog->setPlainText(UI->teLog->toPlainText() + 
			                    tr("Using sys filter date. Start from %1\n").arg(d.toString("dd.MM.yyyy")) );
		UI->teLog->moveCursor(QTextCursor::End);

	}
	else{
        UI->teLog->setPlainText(UI->teLog->toPlainText() + 
	                            tr("Filter date not used! Get all available IO.\n") );
		UI->teLog->moveCursor(QTextCursor::End);

		return d;
	}

	return d;
}
