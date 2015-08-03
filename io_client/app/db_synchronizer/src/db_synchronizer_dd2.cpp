#include <QtGui>

#include "ui_db_synchronizer_form.h"
#include "db_synchronizer.h"

#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <kkssito.h>
#include <KKSLoader.h>





int DBSynchronizer::createETKInDD(const QList<Result_ETK> & etkList)
{
	for(int i=0; i< etkList.count(); i++){
		Result_ETK etk = etkList.at(i);
		bool bExist = findEqualIO(etk.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("ETK with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("ETK UID = %1\n").arg(etk.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(etk.file_link.count() > 0){
            for(int j=0; j<etk.file_link.count(); j++)
                prepareDownloadFile(etk.file_uid.at(j), i, etk.file_link.at(j), etk.file_name.at(j), files);
		}
        if(!etk.classificator_link.isEmpty()){
            prepareDownloadFile(etk.uid + "_RSC", i, etk.classificator_link, etk.classificator_name, files);
        }

		int ok = createIOFromETK(etk, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from ETK in DynamicDocs database!\n") + 
									tr("ETK UID = %1\n").arg(etk.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}

        if(files.count() > 0){
            if(etk.file_link.count() == 0){
                dbSIU.downloadFile(etk.uid, etk.classificator_link, files.at(0).second);
            }
            else{
                for(int j=0; j<etk.file_link.count(); j++){
                    dbSIU.downloadFile(etk.uid, etk.file_link.at(j), files.at(j).second);
                }
                if(files.count() > etk.file_link.count())
                    dbSIU.downloadFile(etk.uid, etk.classificator_link, files.at(files.count()-1).second);
            }
                
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
        
	}
	
	return OK_CODE;
}




int DBSynchronizer::createGMOInDD(const QList<Result_GMO> & gmoList)
{
	for(int i=0; i< gmoList.count(); i++){
		Result_GMO gmo = gmoList.at(i);
		bool bExist = findEqualIO(gmo.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("GMO with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("GMO UID = %1\n").arg(gmo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(gmo.file_link.count() > 0){
            for(int j=0; j<gmo.file_link.count(); j++)
                prepareDownloadFile(gmo.file_uid.at(j), i, gmo.file_link.at(j), gmo.file_name.at(j), files);
		}

		int ok = createIOFromGMO(gmo, files);
				
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from GMO in DynamicDocs database!\n") + 
									tr("GMO UID = %1 \n").arg(gmo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}

        if(files.count() > 0){
		    for(int j=0; j<gmo.file_link.count(); j++)
                dbSIU.downloadFile(gmo.uid, gmo.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
    }
	
	return OK_CODE;
}

int DBSynchronizer::createNVOInDD(const QList<Result_NVO> & nvoList)
{
	for(int i=0; i< nvoList.count(); i++){
		Result_NVO nvo = nvoList.at(i);
		bool bExist = findEqualIO(nvo.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("NVO with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("NVO UID = %1\n").arg(nvo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(nvo.file_link.count() > 0){
            for(int j=0; j<nvo.file_link.count(); j++)
                prepareDownloadFile(nvo.file_uid.at(j), i, nvo.file_link.at(j), nvo.file_name.at(j), files);
		}

		int ok = createIOFromNVO(nvo, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from NVO in DynamicDocs database!\n") + 
									tr("NVO UID = %1\n").arg(nvo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}

        if(files.count() > 0){
		    for(int j=0; j<nvo.file_link.count(); j++)
                dbSIU.downloadFile(nvo.uid, nvo.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }

	}
	
	return OK_CODE;
}

int DBSynchronizer::createEOIRDInDD(const QList<Result_EOIRD> & eoirdList)
{
	for(int i=0; i< eoirdList.count(); i++){
		Result_EOIRD eoird = eoirdList.at(i);
		bool bExist = findEqualIO(eoird.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("EOIRD with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("EOIRD UID = %1\n").arg(eoird.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(eoird.file_link.count() > 0){
            for(int j=0; j<eoird.file_link.count(); j++)
                prepareDownloadFile(eoird.file_uid.at(j), i, eoird.file_link.at(j), eoird.file_name.at(j), files);
		}

		int ok = createIOFromEOIRD(eoird, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from EOIRD in DynamicDocs database!\n") + 
									tr("EOIRD UID = %1\n").arg(eoird.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<eoird.file_link.count(); j++)
                dbSIU.downloadFile(eoird.uid, eoird.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }

	}
	
	return OK_CODE;
}

int DBSynchronizer::createEFOInDD(const QList<Result_EFO> & efoList)
{
	for(int i=0; i< efoList.count(); i++){
		Result_EFO efo = efoList.at(i);
		bool bExist = findEqualIO(efo.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("EFO with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("EFO UID = %1\n").arg(efo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(efo.file_link.count() > 0){
            for(int j=0; j<efo.file_link.count(); j++)
                prepareDownloadFile(efo.file_uid.at(j), i, efo.file_link.at(j), efo.file_name.at(j), files);
		}

		int ok = createIOFromEFO(efo, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from EFO in DynamicDocs database!\n") + 
									tr("EFO UID = %1\n").arg(efo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<efo.file_link.count(); j++)
                dbSIU.downloadFile(efo.uid, efo.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }

	}
	
	return OK_CODE;
}


int DBSynchronizer::createUGSHInDD(const QList<Result_UGSH> & ugshList)
{
	for(int i=0; i< ugshList.count(); i++){
		Result_UGSH ugsh = ugshList.at(i);
		bool bExist = findEqualIO(ugsh.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("UGSH with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("UGSH UID = %1\n").arg(ugsh.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(ugsh.file_link.count() > 0){
            for(int j=0; j<ugsh.file_link.count(); j++)
                prepareDownloadFile(ugsh.file_uid.at(j), i, ugsh.file_link.at(j), ugsh.file_name.at(j), files);
		}

		int ok = createIOFromUGSH(ugsh, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from UGSH in DynamicDocs database!\n") + 
									tr("UGSH UID = %1\n").arg(ugsh.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<ugsh.file_link.count(); j++)
                dbSIU.downloadFile(ugsh.uid, ugsh.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createTTXInDD(const QList<Result_TTX> & ttxList)
{
	for(int i=0; i< ttxList.count(); i++){
		Result_TTX ttx = ttxList.at(i);
		bool bExist = findEqualIO(ttx.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("TTX with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("TTX UID = %1\n").arg(ttx.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(ttx.file_link.count() > 0){
            for(int j=0; j<ttx.file_link.count(); j++)
                prepareDownloadFile(ttx.file_uid.at(j), i, ttx.file_link.at(j), ttx.file_name.at(j), files);
		}

		int ok = createIOFromTTX(ttx, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from TTX in DynamicDocs database!\n") + 
									tr("TTX UID = %1\n").arg(ttx.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<ttx.file_link.count(); j++)
                dbSIU.downloadFile(ttx.uid, ttx.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createPGInDD(const QList<Result_PG> & pgList)
{
	for(int i=0; i< pgList.count(); i++){
		Result_PG pg = pgList.at(i);
		bool bExist = findEqualIO(pg.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("PG with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("PG UID = %1\n").arg(pg.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(pg.file_link.count() > 0){
            for(int j=0; j<pg.file_link.count(); j++)
                prepareDownloadFile(pg.file_uid.at(j), i, pg.file_link.at(j), pg.file_name.at(j), files);
		}

		int ok = createIOFromPG(pg, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from PG in DynamicDocs database!\n") + 
									tr("PG UID = %1\n").arg(pg.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<pg.file_link.count(); j++)
                dbSIU.downloadFile(pg.uid, pg.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createPORInDD(const QList<Result_POR> & porList)
{
	for(int i=0; i< porList.count(); i++){
		Result_POR por = porList.at(i);
		bool bExist = findEqualIO(por.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("POR with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("POR UID = %1\n").arg(por.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(por.file_link.count() > 0){
            for(int j=0; j<por.file_link.count(); j++)
                prepareDownloadFile(por.file_uid.at(j), i, por.file_link.at(j), por.file_name.at(j), files);
		}

		int ok = createIOFromPOR(por, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from POR in DynamicDocs database!\n") + 
									tr("POR UID = %1\n").arg(por.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<por.file_link.count(); j++)
                dbSIU.downloadFile(por.uid, por.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createRBPInDD(const QList<Result_RBP> & rbpList)
{
	for(int i=0; i< rbpList.count(); i++){
		Result_RBP rbp = rbpList.at(i);
		bool bExist = findEqualIO(rbp.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("RBP with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("RBP UID = %1\n").arg(rbp.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(rbp.file_link.count() > 0){
            for(int j=0; j<rbp.file_link.count(); j++)
                prepareDownloadFile(rbp.file_uid.at(j), i, rbp.file_link.at(j), rbp.file_name.at(j), files);
		}

		int ok = createIOFromRBP(rbp, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from RBP in DynamicDocs database!\n") + 
									tr("RBP UID = %1\n").arg(rbp.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<rbp.file_link.count(); j++)
                dbSIU.downloadFile(rbp.uid, rbp.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createREQInDD(const QList<Result_Request> & reqList)
{
	for(int i=0; i< reqList.count(); i++){
		Result_Request req = reqList.at(i);
		bool bExist = findEqualIO(req.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("REQ with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("REQ UID = %1\n").arg(req.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(req.file_link.count() > 0){
            for(int j=0; j<req.file_link.count(); j++)
                prepareDownloadFile(req.file_uid.at(j), i, req.file_link.at(j), req.file_name.at(j), files);
		}

		int ok = createIOFromREQ(req, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from REQ in DynamicDocs database!\n") + 
									tr("REQ UID = %1\n").arg(req.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<req.file_link.count(); j++)
                dbSIU.downloadFile(req.uid, req.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}

int DBSynchronizer::createOPVTOInDD(const QList<Result_VTO> & vtoList)
{
	for(int i=0; i< vtoList.count(); i++){
		Result_VTO vto = vtoList.at(i);
		bool bExist = findEqualIO(vto.uid);
		if(bExist){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("VTO with equal UID was found in DynamicDocs database!\n") + 
									tr("Skipped!\n") +
									tr("VTO UID = %1\n").arg(vto.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		QList<QPair<QString, QString> > files;
        if(vto.file_link.count() > 0){
            for(int j=0; j<vto.file_link.count(); j++)
                prepareDownloadFile(vto.file_uid.at(j), i, vto.file_link.at(j), vto.file_name.at(j), files);
		}

		int ok = createIOFromOPVTO(vto, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from OP VTO in DynamicDocs database!\n") + 
									tr("OP VTO UID = %1\n").arg(vto.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
        if(files.count() > 0){
		    for(int j=0; j<vto.file_link.count(); j++)
                dbSIU.downloadFile(vto.uid, vto.file_link.at(j), files.at(j).second);
        }

        if(i > 0 && i%iReconnect == 0){

            QEventLoop eventLoop;
            connect(this,SIGNAL(allFilesDownloaded()),&eventLoop,SLOT(quit()));
		    eventLoop.exec();

            //QMessageBox::critical(this, tr(""), tr(""));

            bool ok = dbSIU.logout();
            if(ok != true){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось отсоединиться от БД СИУ!"));
                return ERROR_CODE;
            }

            ok = dbSIU.authorize(siuIP, siuPort, siuUser, siuPasswd, siuConstraint, siuSOP);
            if(!ok){
                QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось переподключиться к БД СИУ!"));
                return ERROR_CODE;
            }

        }
	}
	
	return OK_CODE;
}
