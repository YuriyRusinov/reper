#ifndef DNBDSPECTR_H
#define DNBDSPECTR_H

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>
struct SpecGraph
{
 QList <float> KSJ;
 QList <float> Lam;
 QString ObjId;
};

class DNBDSpectr
{
public:
 struct NameAndNum
 {
  QString Name;
  QString Num;
  QString NameFieldNum;
 };
 struct NameAndClsIdFones
 {
  QString NameFone;
  QList <QString> ClsIdsFone;
 };
public:
 explicit DNBDSpectr(QString BDId, QString BDName, QString UserName,QString ComputerName, QString PassWord);
 QList <QString> ReadData(QString NameTable,QString NameCol,QString Condition="");

 QList<NameAndNum> ReadNamesSpectrs(QString NameClas);
 QList<NameAndNum> ReadNamesSpectrs(QString NameClas,int NumLandScape);
 QList<NameAndNum> ReadNamesSpectrs(QString NameClas,int Fenol,int NumLandScape);

 /*Получить название фонов для данного класса и данного объекта классификатора*/
 QList<NameAndClsIdFones> ReadFones(QString NameClas,QString NameObj);
 QList <int> ReadLandScape(QList<NameAndNum> ClsIdStruct);
 QList <int>  ReadLandScape(QList <NameAndNum> ClsIdStruct,int fenology);
 QString GetNameLandScape(int idLandScape);


 QList <SpecGraph> ReadSpectrsObj(QString NameObj);
 SpecGraph ReadSpectrsObj(quint64 ObjId);
 QList <SpecGraph> ReadSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId);
 QList <SpecGraph> ReadSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,int ObjFenology);
 QList <SpecGraph> ReadSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,int ObjFenology,int LandSpace);

 QList <SpecGraph> ReadPartSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,float LamStart,float LamFinish);
 int GetNumMeasureMent(SpecGraph Graph, float LamStart, float LamFinish);
 //Взять яркость объекта для определенной длинны волны (Lam задаётся в нанометрах) Возвращает список значений, если найдено несколько объектов в БД
 QList <float> GetBrightLamda(float Lam,QString ClsId,QList<QString> ClsBackgroId);
 QList <float> GetBrightLamda(float Lam,QString ClsId, QList<QString> ClsBackgroId,int ObjFenology);
 float GetBrightLamda(float Lam,quint64 ObjId);
 //Взять интегральную яркость объекта (Lam задаётся в нанометрах) Возвращает список значений, если найдено несколько объектов в БД
 QList <float> GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList<QString> ClsBackgroId);
 QList <float> GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList <QString> ClsBackgroId,int ObjFenology);
 QList <float> GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList <QString> ClsBackgroId,int ObjFenology,int LandSpace);

 //Функции получения имён и индексов
 //QList <QString> NameObjToClsId();
 QList <QString> NameObjToBackgroid(QString BackgroName);


 QSqlQuery sql;
 bool IsBDOpen;
 QString OpenBDError;
 QSqlDatabase *bd;

 QString ClsIdToSubClass(QString ClsId);
private:
 //Получить ClsId для данного класса и данного объекта классификатора
 QList <QString> GetClsId(QString NameClas,QString NameObj);
 QList <QString> GetClsId(QString NameObj);
 QList <QString> ClsIdToBackgroId(QList <QString> ClsId);

 //Получить BackgroId для данного класса и данного объекта классификатора, для которых есть спектральные графики
 QList<NameAndNum> GetClsBackgroId(QString NameClas,QString NameObj);
 //Получить все backgroid по наименованию фона
 QList <QString> NameFoneToBackgroid(QString NameFone);

 //Функция составляющая сложное условие для WHERE Возвращает строку: "NameField=DataList[0] Operator (And или or) NameField=DataList[1]..."
 QString GenCondition(QString NameField,QList <QString> DataList,QString Operator);
};

#endif // DNBDSPECTR_H
