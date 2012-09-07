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
		if(!etk.file_link.isEmpty()){
			prepareDownloadFile(etk.uid, i, etk.file_link, etk.file_name, files);
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
		    dbSIU.downloadFile(etk.uid, etk.file_link, files.at(0).second);
            if(files.count() > 1)
                dbSIU.downloadFile(etk.uid, etk.classificator_link, files.at(1).second);
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
		if(!gmo.file_link.isEmpty()){
			prepareDownloadFile(gmo.uid, i, gmo.file_link, gmo.file_name, files);
		}

		int ok = createIOFromGMO(gmo, files);
				
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from GMO in DynamicDocs database!\n") + 
									tr("GMO UID = %1 \n").arg(gmo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}

		if(files.count() > 0)
		    dbSIU.downloadFile(gmo.uid, gmo.file_link, files.last().second);
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
		if(!nvo.file_link.isEmpty()){
			prepareDownloadFile(nvo.uid, i, nvo.file_link, nvo.file_name, files);
		}

		int ok = createIOFromNVO(nvo, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from NVO in DynamicDocs database!\n") + 
									tr("NVO UID = %1\n").arg(nvo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}

		if(files.count() > 0)
			dbSIU.downloadFile(nvo.uid, nvo.file_link, files.last().second);
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
		if(!eoird.file_link.isEmpty()){
			prepareDownloadFile(eoird.uid, i, eoird.file_link, eoird.file_name, files);
		}

		int ok = createIOFromEOIRD(eoird, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from EOIRD in DynamicDocs database!\n") + 
									tr("EOIRD UID = %1\n").arg(eoird.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(eoird.uid, eoird.file_link, files.last().second);

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
		if(!efo.file_link.isEmpty()){
			prepareDownloadFile(efo.uid, i, efo.file_link, efo.file_name, files);
		}

		int ok = createIOFromEFO(efo, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from EFO in DynamicDocs database!\n") + 
									tr("EFO UID = %1\n").arg(efo.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(efo.uid, efo.file_link, files.last().second);

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
		if(!ugsh.file_link.isEmpty()){
			prepareDownloadFile(ugsh.uid, i, ugsh.file_link, ugsh.file_name, files);
		}

		int ok = createIOFromUGSH(ugsh, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from UGSH in DynamicDocs database!\n") + 
									tr("UGSH UID = %1\n").arg(ugsh.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(ugsh.uid, ugsh.file_link, files.last().second);

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
		if(!ttx.file_link.isEmpty()){
			prepareDownloadFile(ttx.uid, i, ttx.file_link, ttx.file_name, files);
		}

		int ok = createIOFromTTX(ttx, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from TTX in DynamicDocs database!\n") + 
									tr("TTX UID = %1\n").arg(ttx.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(ttx.uid, ttx.file_link, files.last().second);

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
		if(!pg.file_link.isEmpty()){
			prepareDownloadFile(pg.uid, i, pg.file_link, pg.file_name, files);
		}

		int ok = createIOFromPG(pg, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from PG in DynamicDocs database!\n") + 
									tr("PG UID = %1\n").arg(pg.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(pg.uid, pg.file_link, files.last().second);

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
		if(!por.file_link.isEmpty()){
			prepareDownloadFile(por.uid, i, por.file_link, por.file_name, files);
		}

		int ok = createIOFromPOR(por, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from POR in DynamicDocs database!\n") + 
									tr("POR UID = %1\n").arg(por.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(por.uid, por.file_link, files.last().second);

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
		if(!rbp.file_link.isEmpty()){
			prepareDownloadFile(rbp.uid, i, rbp.file_link, rbp.file_name, files);
		}

		int ok = createIOFromRBP(rbp, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from RBP in DynamicDocs database!\n") + 
									tr("RBP UID = %1\n").arg(rbp.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(rbp.uid, rbp.file_link, files.last().second);

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
		if(!req.file_link.isEmpty()){
			prepareDownloadFile(req.uid, i, req.file_link, req.file_name, files);
		}

		int ok = createIOFromREQ(req, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from REQ in DynamicDocs database!\n") + 
									tr("REQ UID = %1\n").arg(req.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(req.uid, req.file_link, files.last().second);

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
		if(!vto.file_link.isEmpty()){
			prepareDownloadFile(vto.uid, i, vto.file_link, vto.file_name, files);
		}

		int ok = createIOFromOPVTO(vto, files);
		if(ok != OK_CODE){
		    UI->teLog->setPlainText(UI->teLog->toPlainText() + 
				                    tr("An ERROR was occured while creating IO from OP VTO in DynamicDocs database!\n") + 
									tr("OP VTO UID = %1\n").arg(vto.uid));
			UI->teLog->moveCursor(QTextCursor::End);
			continue;
		}
		
		if(files.count() > 0)
			dbSIU.downloadFile(vto.uid, vto.file_link, files.last().second);

	}
	
	return OK_CODE;
}
