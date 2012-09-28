#ifndef DB_SYNCHRONIZER_H
#define DB_SYNCHRONIZER_H

#include <QDialog>
#include <QTimer>

class QDialogButtonBox;
class QFile;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;

class KKSDatabase;

#include <api_bdfz.h>
#include <KKSList.h>
#include <KKSFile.h>

namespace Ui
{
    class db_synchronizer_form;
};


//ЭТК
#define ID_CAT_ETK 551
//ЭФО
#define ID_CAT_EFO 540
//ЭОИД
#define ID_CAT_EOIRD 539
//краткосрочный прогноз
#define ID_CAT_GMO1 550
//среднесрочный прогноз
#define ID_CAT_GMO2 549
//долгосрочный прогноз
#define ID_CAT_GMO3 548
//текущее состояние глонасс
#define ID_CAT_NVO 541
//указания ГШ
#define ID_CAT_UGSH 552

//ТТХ самолетов ДЛРО
#define ID_CAT_TTX_DLRO 566
//ТТХ самолетов ИА ПВО
#define ID_CAT_TTX_IA_PVO 567
//ТТХ RLS
#define ID_CAT_TTX_RLS 568
//ТТХ ракет воздух-воздух
#define ID_CAT_TTX_RVV 569
//ТТХ ЗРК
#define ID_CAT_TTX_ZRK 570
//ТТХ средств ПВО
#define ID_CAT_TTX_PVO 571

//план-графики ИО ВТО
#define ID_CAT_PG 554
//перечни первоочередных объектов для мероприятий ИО ВТО
#define ID_CAT_POR 553
//районы боевого применения
#define ID_CAT_RBP 599
//заявки на ИО ВТО
#define ID_CAT_REQ 600

//перечни ОП для СВ
#define ID_CAT_SV 556
//перечни ОП для ВВС
#define ID_CAT_VVS 557
//перечни ОП для ВМФ
#define ID_CAT_VMF 558

class DBSynchronizer : public QDialog
{
    Q_OBJECT

public:
    DBSynchronizer(QWidget *parent=0, Qt::WFlags f=0);

private slots:
	void setTimer();
	void stopTimer();
	void analyzeNow();
	void analyzeSIU();

	void SIUConnect();

	//void saveFileInDD(QFile * f);
	void saveFileInDD(const QString & fileName);
    void downloadFileProgress(QString uid, QString filename, qint64 done, qint64 full);


private:

	//void connectToSIU();
	


    //IN DB_SYNCHRONIZER_SIU1.CPP
	void getETK();
	void getGMO();
	void getNVO();
	void getEOIRD();
	void getEFO();

	void getUGSH();
	void getTTX();
	void getPG();
	void getPOR();
	void getRBP();
	void getREQ();
	void getOPVTO();
    
	//IN DB_SYNCHRONIZER_DD2.CPP
	int createETKInDD(const QList<Result_ETK> & etkList);
	int createGMOInDD(const QList<Result_GMO> & gmoList);
	int createNVOInDD(const QList<Result_NVO> & nvoList);
	int createEOIRDInDD(const QList<Result_EOIRD> & eoirdList);
	int createEFOInDD(const QList<Result_EFO> & efoList);
	int createUGSHInDD(const QList<Result_UGSH> & ugshList);
    int createTTXInDD(const QList<Result_TTX> & ttxList);
	int createPGInDD(const QList<Result_PG> & pgList);
	int createPORInDD(const QList<Result_POR> & porList);
	int createRBPInDD(const QList<Result_RBP> & rbpList);
	int createREQInDD(const QList<Result_Request> & reqList);
	int createOPVTOInDD(const QList<Result_VTO> & vtoList);

	//IN DB_SYNCHRONIZER_DD1.CPP
	int createIOFromETK(const Result_ETK & etk, const QList<QPair<QString, QString> > & files);
	int createIOFromGMO(const Result_GMO & gmo, const QList<QPair<QString, QString> > & files);
	int createIOFromNVO(const Result_NVO & nvo, const QList<QPair<QString, QString> > & files);
	int createIOFromEOIRD(const Result_EOIRD & eoird, const QList<QPair<QString, QString> > & files);
	int createIOFromEFO(const Result_EFO & efo, const QList<QPair<QString, QString> > & files);
	int createIOFromUGSH(const Result_UGSH & ugsh, const QList<QPair<QString, QString> > & files);
	int createIOFromTTX(const Result_TTX & ttx, const QList<QPair<QString, QString> > & files);
	int createIOFromPG(const Result_PG & pg, const QList<QPair<QString, QString> > & files);
	int createIOFromPOR(const Result_POR & pg, const QList<QPair<QString, QString> > & files);
	int createIOFromRBP(const Result_RBP & rbp, const QList<QPair<QString, QString> > & files);
	int createIOFromREQ(const Result_Request & req, const QList<QPair<QString, QString> > & files);
	int createIOFromOPVTO(const Result_VTO & vto, const QList<QPair<QString, QString> > & files);

	int createIO(int idCategory, 
			     const QString & uid, 
			     const QString & name, 
			     int idMaclabel, 
				 const QList<QPair<QString, QString> > & files);

	bool findEqualIO(const QString & uid) const;

	int insertAttrs(int idObject, const QList<QPair<int, QString> > & attrs);
	
	int createFileInDD(int idUrl, const QString & localUrl);
	void createIOUrls(int idObject, const QList<QPair<QString, QString> > & files);
	int rInsertUrl(const QString & fileName, int fileType, const QString & fileExt);
	int ioInsertUrl(int idObject, int idUrl, const QString & fileName);


	int initCacheTable();
	QDate getFilterDate() const;

	int updateStartDate();
	void createPair(int idAttr, const QString & value, QList<QPair<int, QString> > & attrs);
    
	void prepareDownloadFile(const QString & uid, 
					         int index, 
					         const QString & fileLink, 
					         const QString & fileName,
					         QList<QPair<QString, QString> > & files);

private:
	ApiBdfz dbSIU;
	bool connected;
	Ui::db_synchronizer_form *UI;
	QTimer timer;
	KKSDatabase * db;
    
    int iTimeout;//таймаут для каджого метода опроса БД СИУ

	QList<QPair<int, QString> > idUrls_files;//ассоциации между idUrl, созданных в БД ДД, и файлами в файловой системе, которые находятся в процессе скачивания.

	QStringList downloadingFiles; //содержит перечень файлов, которые в настоящее время скачиваются из БД СИУ
	QDate lastDateSys;//дата, начиная с которой загружаем данные (считаем, что данные с датой ранее заданной уже загружены.)

	QString rootFileStore;//путь к локальному каталогу со скачиваемыми файлами

    KKSList<KKSFileType *> m_fileTypes;//зарегистрированные в БД ДД типы файлов

	//KKSLoader *
};

#endif
