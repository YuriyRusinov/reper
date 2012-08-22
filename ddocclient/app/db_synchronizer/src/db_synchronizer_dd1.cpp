#include <QtGui>

#include "ui_db_synchronizer_form.h"
#include "db_synchronizer.h"

#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <kkssito.h>
#include <KKSLoader.h>





int DBSynchronizer::createIOFromETK(const Result_ETK & etk, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idObject = createIO(ID_CAT_ETK, etk.uid, etk.file_name, etk.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	createPair(1273, etk.nomenclature, attrs);
	createPair(1274, etk.creation_date.toString("dd.MM.yyyy"), attrs);
	createPair(1275, QString::number(etk.scale), attrs);
	createPair(1276, QString::number(99124), attrs);
	createPair(1277, etk.sheet_name, attrs);
	
	createPair(1286, QString::number(etk.lat_min), attrs);
	createPair(1287, QString::number(etk.lat_max), attrs);
	createPair(1288, QString::number(etk.lon_min), attrs);
	createPair(1289, QString::number(etk.lon_max), attrs);

	createPair(1290, etk.classificator_name, attrs);
	//createPair(1291, QString::number(), attrs);
	//createPair(1292, QString::number(), attrs);
	createPair(1293, etk.organization, attrs);
	createPair(1294, etk.uid, attrs);
	createPair(1295, etk.author, attrs);


	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromGMO(const Result_GMO & gmo, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat;
	if(gmo.forecast_type == "Краткосрочный")
		idCat = ID_CAT_GMO1;
	else if(gmo.forecast_type == "Среднесрочный")
		idCat = ID_CAT_GMO2;
	else if(gmo.forecast_type == "Долгосрочный")
		idCat = ID_CAT_GMO3;
	else
		return 0;

	int idObject = createIO(idCat, gmo.uid, gmo.file_name, gmo.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	//createPair(1265, gmo., attrs);
	//createPair(1266, gmo., attrs);
	//createPair(1278, gmo., attrs);
	createPair(1279, QString::number(gmo.lat_min), attrs);
	createPair(1280, QString::number(gmo.lon_min), attrs);
	createPair(1281, QString::number(gmo.lat_max), attrs);
	createPair(1282, QString::number(gmo.lon_min), attrs);
	
	createPair(1283, gmo.forecast_date.toString("dd.MM.yyyy"), attrs);
    //createPair(1291, gmo., attrs);
	//createPair(1292, gmo., attrs);
	createPair(1293, gmo.organization, attrs);
	createPair(1294, gmo.uid, attrs);
	createPair(1295, gmo.author, attrs);
	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromNVO(const Result_NVO & nvo, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idObject = createIO(ID_CAT_NVO, nvo.uid, nvo.file_name, nvo.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1272, nvo.date.toString("dd.MM.yyyy") + " 00:00:00", attrs);
	//createPair(1291, nvo., attrs);
	//createPair(1292, nvo., attrs);
    
	createPair(1293, nvo.organization, attrs);
	createPair(1294, nvo.uid, attrs);
	createPair(1295, nvo.author, attrs);

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromEOIRD(const Result_EOIRD & eoird, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idObject = createIO(ID_CAT_EOIRD, eoird.uid, eoird.file_name, eoird.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1257, eoird.type_obj, attrs);
	//createPair(1258, eoird., attrs);
	createPair(1259, QString::number(eoird.number_t), attrs);
	createPair(1260, QString::number(eoird.number_k), attrs);
	createPair(1261, eoird.name, attrs);
	//!!createPair(1263, eoird.country, attrs);
	//!!createPair(1264, eoird.nationality, attrs);
	createPair(1265,  QString::number(eoird.lat_on), attrs);
	createPair(1266,  QString::number(eoird.lon_on), attrs);
	//createPair(1267, eoird., attrs);
	//createPair(1268, eoird., attrs);
	createPair(1269, eoird.material_date.toString("dd.MM.yyyy"), attrs);
	createPair(1270, eoird.development_date.toString("dd.MM.yyyy"), attrs);
	createPair(1271, eoird.executer, attrs);

	//createPair(1291, eoird., attrs);
	//createPair(1292, eoird., attrs);
 
	createPair(1293, eoird.organization, attrs);
	createPair(1294, eoird.uid, attrs);
	createPair(1295, eoird.author, attrs);

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromEFO(const Result_EFO & efo, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idObject = createIO(ID_CAT_EFO, efo.uid, efo.file_name, efo.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	createPair(1257, efo.type_obj, attrs);
	//createPair(1258, efo., attrs);
	createPair(1259, QString::number(efo.number_t), attrs);
	createPair(1260, QString::number(efo.number_k), attrs);
	createPair(1261, efo.name, attrs);
	//!!createPair(1263, efo.country, attrs);
	//!!createPair(1264, efo.nationality, attrs);
	createPair(1265,  QString::number(efo.lat_on), attrs);
	createPair(1266,  QString::number(efo.lon_on), attrs);
	//createPair(1267, efo., attrs);
	//createPair(1268, efo., attrs);
	createPair(1269, efo.material_date.toString("dd.MM.yyyy"), attrs);
	createPair(1270, efo.development_date.toString("dd.MM.yyyy"), attrs);
	createPair(1271, efo.executer, attrs);

	//createPair(1291, efo., attrs);
	//createPair(1292, efo., attrs);
 
	createPair(1293, efo.organization, attrs);
	createPair(1294, efo.uid, attrs);
	createPair(1295, efo.author, attrs);

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromUGSH(const Result_UGSH & ugsh, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat = ID_CAT_UGSH;


	int idObject = createIO(idCat, ugsh.uid, ugsh.file_name, ugsh.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	//createPair(1296, ugsh., attrs);
	createPair(1297, ugsh.signature_date.toString("dd.MM.yyyy"), attrs);
    createPair(1298, ugsh.outgoing_number, attrs);
	createPair(1299, ugsh.record_number, attrs);
	createPair(1293, ugsh.organization, attrs);
	createPair(1294, ugsh.uid, attrs);
	createPair(1295, ugsh.author, attrs);
	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromTTX(const Result_TTX & ttx, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat;
	if(ttx.weapon_type == "Самолеты ДРЛО")
		idCat = ID_CAT_TTX_DLRO;
	else if(ttx.weapon_type == "ЗРК средней и большой дальности")
		idCat = ID_CAT_TTX_ZRK;
	else if(ttx.weapon_type == "Ракеты «воздух-воздух»")
		idCat = ID_CAT_TTX_RVV;
	else if(ttx.weapon_type == "РЛС обнаружения")
		idCat = ID_CAT_TTX_RLS;	
	else if(ttx.weapon_type == "Самолеты истребительной авиации")
		idCat = ID_CAT_TTX_IA_PVO;	
	else if(ttx.weapon_type == "Средства ПВО боевых кораблей ИГ")
		idCat = ID_CAT_TTX_PVO;	
	else
		return 0;

	int idObject = createIO(idCat, ttx.uid, ttx.file_name, ttx.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1310, ttx.date.toString("dd.MM.yyyy"), attrs);
	createPair(1293, ttx.organization, attrs);
	createPair(1294, ttx.uid, attrs);
	createPair(1295, ttx.author, attrs);
	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromPG(const Result_PG & pg, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat = ID_CAT_PG;

	int idObject = createIO(idCat, pg.uid, pg.file_name, pg.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	//createPair(1291, pg., attrs);
	//createPair(1292, pg., attrs);
	createPair(1293, pg.organization, attrs);
	createPair(1294, pg.uid, attrs);
	createPair(1295, pg.author, attrs);
	createPair(1297, pg.signature_date.toString("dd.MM.yyyy"), attrs);
	createPair(1323, pg.rbp_number, attrs);
	createPair(1334, pg.addressee, attrs);
	createPair(1335, pg.category, attrs);

	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromPOR(const Result_POR & por, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat = ID_CAT_POR;

	int idObject = createIO(idCat, por.uid, por.file_name, por.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1293, por.organization, attrs);
	createPair(1294, por.uid, attrs);
	createPair(1295, por.author, attrs);
	createPair(1297, por.signature_date.toString("dd.MM.yyyy"), attrs);
	createPair(1334, por.addressee, attrs);
	createPair(1335, por.category, attrs);

	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromRBP(const Result_RBP & rbp, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat = ID_CAT_RBP;

	int idObject = createIO(idCat, rbp.uid, rbp.file_name, rbp.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1293, rbp.organization, attrs);
	createPair(1294, rbp.uid, attrs);
	createPair(1295, rbp.author, attrs);
	createPair(1323, rbp.rbp_number, attrs);
	createPair(1279, QString::number(rbp.lat_min), attrs);
	createPair(1280, QString::number(rbp.lon_min), attrs);
	createPair(1281, QString::number(rbp.lat_max), attrs);
	createPair(1282, QString::number(rbp.lon_min), attrs);

	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromREQ(const Result_Request & req, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat = ID_CAT_REQ;

	int idObject = createIO(idCat, req.uid, req.file_name, req.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1293, req.organization, attrs);
	createPair(1294, req.uid, attrs);
	createPair(1295, req.author, attrs);
	createPair(1336, req.number, attrs);
	createPair(1337, req.date.toString("dd.MM.yyyy"), attrs);

	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}

int DBSynchronizer::createIOFromOPVTO(const Result_VTO & vto, const QList<QPair<QString, QString> > & files)
{
    if(!db)
		return 0;
  
	int idCat;
	if(vto.vs_type == "ВВС")
		idCat = ID_CAT_VVS;
	else if(vto.vs_type == "ВМФ")
		idCat = ID_CAT_VMF;
	else if(vto.vs_type == "СВ")
		idCat = ID_CAT_SV;
	else
		return 0;

	int idObject = createIO(idCat, vto.uid, vto.file_name, vto.constraint, files);
	if(idObject < 0)
		return 0;

	QList<QPair<int, QString> > attrs;

	
	createPair(1301, vto.vto_usage_form, attrs);
	createPair(1303, vto.variant_number, attrs);
	createPair(1304, vto.plan_number, attrs);
	createPair(1323, vto.rbp_number, attrs);
	createPair(1293, vto.organization, attrs);
	createPair(1294, vto.uid, attrs);
	createPair(1295, vto.author, attrs);
	

	int ok = insertAttrs(idObject, attrs);

	return 1;
}