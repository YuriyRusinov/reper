#include <QtGui>

#include "ui_db_synchronizer_form.h"
#include "db_synchronizer.h"

#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <kkssito.h>
#include <KKSLoader.h>





void DBSynchronizer::getETK()
{
	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying ETK...\n"));

	Param_ETK etk;
	QDate d = getFilterDate();
	if(d.isValid())
		etk.updated_from = QDateTime(d);
	//etk.scale = 100000;
	//etk.nomenclature = "0.K-37-060";
	//etk.date_to = QDate::currentDate();
	//etk.constraints = 9;//любой

	QList<Result_ETK> res = dbSIU.searchIO(etk);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("ETK was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 ETK in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting ETK in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createETKInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting ETK in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting ETK in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getGMO()
{
	if(ID_CAT_GMO1 == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying GMO...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_GMO gmo;
	QDate d = getFilterDate();
	if(d.isValid())
		gmo.updated_from = QDateTime(d);

	QList<Result_GMO> res = dbSIU.searchIO(gmo);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("GMO was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 GMO in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting GMO in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createGMOInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting GMO in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting GMO in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getNVO()
{
	if(ID_CAT_NVO == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying NVO...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_NVO nvo;
	QDate d = getFilterDate();
	if(d.isValid())
		nvo.updated_from = QDateTime(d);

	QList<Result_NVO> res = dbSIU.searchIO(nvo);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("NVO was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 NVO in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting NVO in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createNVOInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting NVO in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting NVO in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getEOIRD()
{
	if(ID_CAT_EOIRD == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying EOIRD...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_EOIRD eoird;
	QDate d = getFilterDate();
	if(d.isValid())
		eoird.updated_from = QDateTime(d);

	QList<Result_EOIRD> res = dbSIU.searchIO(eoird);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("EOIRD was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 EOIRD in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting EOIRD in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createEOIRDInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting EOIRD in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting EOIRD in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getEFO()
{
	if(ID_CAT_EFO == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying EFO...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_EFO efo;
	QDate d = getFilterDate();
	if(d.isValid())
		efo.updated_from = QDateTime(d);

	QList<Result_EFO> res = dbSIU.searchIO(efo);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("EFO was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 EFO in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting EFO in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createEFOInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting EFO in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting EFO in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getUGSH()
{
	if(ID_CAT_UGSH == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying UGSH...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_UGSH ugsh;
	QDate d = getFilterDate();
	if(d.isValid())
		ugsh.updated_from = QDateTime(d);

	QList<Result_UGSH> res = dbSIU.searchIO(ugsh);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("UGSH was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 UGSH in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting UGSH in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createUGSHInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting UGSH in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting UGSH in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getTTX()
{
	if(ID_CAT_TTX_DLRO == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying TTX...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_TTX ttx;
	QDate d = getFilterDate();
	if(d.isValid())
		ttx.updated_from = QDateTime(d);

	QList<Result_TTX> res = dbSIU.searchIO(ttx);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("TTX was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 TTX in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting TTX in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createTTXInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting TTX in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting TTX in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getPG()
{
	if(ID_CAT_PG == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying PG...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_PG pg;
	QDate d = getFilterDate();
	if(d.isValid())
		pg.updated_from = QDateTime(d);

	QList<Result_PG> res = dbSIU.searchIO(pg);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("PG was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 PG in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting PG in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createPGInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting PG in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting PG in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getPOR()
{
	if(ID_CAT_POR == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying POR...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_POR por;
	QDate d = getFilterDate();
	if(d.isValid())
		por.updated_from = QDateTime(d);

	QList<Result_POR> res = dbSIU.searchIO(por);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("POR was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 POR in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting POR in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createPORInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting POR in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting POR in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getRBP()
{
	if(ID_CAT_RBP == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying RBP...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_RBP rbp;
	QDate d = getFilterDate();
	if(d.isValid())
		rbp.updated_from = QDateTime(d);

	QList<Result_RBP> res = dbSIU.searchIO(rbp);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("RBP was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 RBP in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting RBP in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createRBPInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting RBP in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting RBP in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getREQ()
{
	if(ID_CAT_REQ == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying REQ...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_Request req;
	QDate d = getFilterDate();
	if(d.isValid())
		req.updated_from = QDateTime(d);

	QList<Result_Request> res = dbSIU.searchIO(req);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("REQ was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 REQ in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting REQ in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createREQInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting REQ in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting REQ in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}

void DBSynchronizer::getOPVTO()
{
	if(ID_CAT_SV == -1)
		return;

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Querying OP VTO...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	Param_VTO vto;
	QDate d = getFilterDate();
	if(d.isValid())
        vto.updated_from = QDateTime(d);

	QList<Result_VTO> res = dbSIU.searchIO(vto);
	int cnt = res.count();
	if(cnt == 0) {
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("OP VTO was not found in SUI database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}
	else{
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Found %1 OP VTO in SUI database!\n").arg(cnt));
		UI->teLog->moveCursor(QTextCursor::End);
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Start inserting OP VTO in DynamicDocs database...\n"));
	UI->teLog->moveCursor(QTextCursor::End);

	int ok = createOPVTOInDD(res);
	if(ok != OK_CODE){
	    UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("An ERROR was occured while inserting OP VTO in DynamicDocs database!\n"));
		UI->teLog->moveCursor(QTextCursor::End);
		return;
	}

	UI->teLog->setPlainText(UI->teLog->toPlainText() + tr("Successfull inserting OP VTO in DynamicDocs database!\n"));
	UI->teLog->moveCursor(QTextCursor::End);
}


