#include "dnbdspectr.h"

DNBDSpectr::DNBDSpectr(QString BDId, QString BDName, QString UserName, QString ComputerName, QString PassWord)
{
 bd=new QSqlDatabase();
 bd=&QSqlDatabase::addDatabase(BDId);
 bd->setHostName(ComputerName); // или, например, "my1.server.ru"
 bd->setDatabaseName(BDName);
 bd->setUserName(UserName);
 bd->setPassword(PassWord);
 bd->setPort(5432);
 IsBDOpen=bd->open();
}

QList <QString> DNBDSpectr::ReadData(QString NameTable,QString NameCol,QString Condition)
{
 QList <QString> ReturnStr;
 ReturnStr.clear();
 QSqlQuery sql;
 QString StrSql;

 StrSql="SELECT "+NameCol+" "+"FROM "+NameTable;
 if(Condition!="")
 {
  StrSql=StrSql+" WHERE "+Condition;
//  QMessageBox msg;
//  msg.setText(StrSql);
//  msg.exec();
 }

 sql.exec(StrSql);

 if(sql.size()!=0)
 {
  while(sql.next())
  {
   QSqlRecord rec = sql.record();
   for(int i=0;i<rec.count();i++)
   {
    if(!sql.value(i).isNull())
    {
     QString str;
     str=sql.value(i).toString();
     ReturnStr<<str;
    }//if(!sql.value(i).isNull())
   }//for(int i=0;i<rec.count();i++)
  }//while(sql.next())
 }
 return ReturnStr;
}

QList<DNBDSpectr::NameAndNum> DNBDSpectr::ReadNamesSpectrs(QString NameClas)
{
 QList <QString> clsid;

 QList <NameAndNum> ReturnList;
 NameAndNum Val;

 /*Получаем в таблице id классов соответствующе заданному наименованию*/
 clsid=ReadData("classvaltab","clsid","classval=\'"+NameClas+"\' AND classpropid=1");

 QString Condition=GenCondition("clsid",clsid,"OR");//="clsid=";
 clsid.clear();

 /*Проверяем есть ли для соответствующих id спектральные данные*/
 QList <QString> clsidSpec;
 clsidSpec=ReadData("generalobjectstab","clsid",Condition);

 /*Убираем повторки*/
 for(int i=0;i<clsidSpec.size();i++)
 {
  int j=i+1;
  while(j<clsidSpec.size())
  {
   if(clsidSpec[i]==clsidSpec[j])
    clsidSpec.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }

 /*Определяем имена объектов классификатора */
 QList <QString> NamesSpectrs;
 Condition='(';
 Condition+=GenCondition("clsid",clsidSpec,"OR");
 Condition+=") AND classpropid=4";
 NamesSpectrs=ReadData("classvaltab","classval, clsid",Condition);
// clsidSpec.clear();

 for(int i=0;i<NamesSpectrs.size()/2;i++)
 {
  Val.Name=NamesSpectrs[i*2];
  Val.NameFieldNum="clsid";
  Val.Num=NamesSpectrs[i*2+1];
  ReturnList<<Val;
 }//for(int i=0;i<NamesSpectrs.size();i++)

 return ReturnList;//ReturnList;
}
QList<DNBDSpectr::NameAndNum> DNBDSpectr::ReadNamesSpectrs(QString NameClas,int NumLandScape)
{
 QList <QString> clsid;

 QList <NameAndNum> ReturnList;
 NameAndNum Val;

 /*Получаем в таблице id классов соответствующе заданному наименованию*/
 clsid=ReadData("classvaltab","clsid","classval=\'"+NameClas+"\' AND classpropid=1");

 QString Condition=GenCondition("clsid",clsid,"OR");//="clsid=";
 clsid.clear();

 /*Проверяем есть ли для соответствующих id спектральные данные*/
 QList <QString> clsidSpec;
 QList <QString> clsidAndLandScape;
 QString proba="proba";
 clsidAndLandScape=ReadData("generalobjectstab","clsid, objlandscape",Condition);

// clsidSpec.clear();

 for(int i=0;i<clsidAndLandScape.size()/2;i++)
 {
  if(clsidAndLandScape[i*2+1].size()>NumLandScape && clsidAndLandScape[i*2+1].at(NumLandScape)=='1')
//  {
   clsidSpec<<clsidAndLandScape[i*2];
//  }
//  if(clsidAndLandScape[i*2+1].size()<=NumLandScape)
//  {
//      QMessageBox msg;
//      msg.setText(clsidAndLandScape[i*2+1]);
//      msg.exec();
//  }
 }



 /*Убираем повторки*/
 for(int i=0;i<clsidSpec.size();i++)
 {
  int j=i+1;
  while(j<clsidSpec.size())
  {
   if(clsidSpec[i]==clsidSpec[j])
    clsidSpec.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }

 /*Определяем имена объектов классификатора */
 QList <QString> NamesSpectrs;
 Condition='(';
 Condition+=GenCondition("clsid",clsidSpec,"OR");
 Condition+=") AND classpropid=4";
 NamesSpectrs=ReadData("classvaltab","classval, clsid",Condition);
// clsidSpec.clear();



 for(int i=0;i<NamesSpectrs.size()/2;i++)
 {
//     QMessageBox msg;
//     msg.setText(NamesSpectrs[i*3+2]+"\n"+QString().setNum(NumLandScape));
//     msg.exec();
//  if(NamesSpectrs[i*3+2].at(NumLandScape)=='1')
//  {

   Val.Name=NamesSpectrs[i*2];
   Val.NameFieldNum="clsid";
   Val.Num=NamesSpectrs[i*2+1];
   ReturnList<<Val;
//  }
 }//for(int i=0;i<NamesSpectrs.size();i++)

 return ReturnList;//ReturnList;
}
QList<DNBDSpectr::NameAndNum> DNBDSpectr::ReadNamesSpectrs(QString NameClas,int Fenol,int NumLandScape)
{
 QList <QString> clsid;

 QList <NameAndNum> ReturnList;
 NameAndNum Val;

 /*Получаем в таблице id классов соответствующе заданному наименованию*/
 clsid=ReadData("classvaltab","clsid","classval=\'"+NameClas+"\' AND classpropid=1");

 QString Condition=GenCondition("clsid",clsid,"OR");//="clsid=";
 clsid.clear();

 /*Проверяем есть ли для соответствующих id спектральные данные*/
 QList <QString> clsidSpec;
 QList <QString> clsidAndLandScape;
 QString proba="proba";
 clsidAndLandScape=ReadData("generalobjectstab","clsid, objlandscape, objfenology",Condition);

// clsidSpec.clear();

 for(int i=0;i<clsidAndLandScape.size()/3;i++)
 {
  if(clsidAndLandScape[i*3+1].size()>NumLandScape && clsidAndLandScape[i*3+1].at(NumLandScape)=='1' && clsidAndLandScape[i*3+2].toInt()==Fenol)
//  {
   clsidSpec<<clsidAndLandScape[i*3];
//  }
//  if(clsidAndLandScape[i*2+1].size()<=NumLandScape)
//  {
//      QMessageBox msg;
//      msg.setText(clsidAndLandScape[i*2+1]);
//      msg.exec();
//  }
 }



 /*Убираем повторки*/
 for(int i=0;i<clsidSpec.size();i++)
 {
  int j=i+1;
  while(j<clsidSpec.size())
  {
   if(clsidSpec[i]==clsidSpec[j])
    clsidSpec.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }

 /*Определяем имена объектов классификатора */
 QList <QString> NamesSpectrs;
 Condition='(';
 Condition+=GenCondition("clsid",clsidSpec,"OR");
 Condition+=") AND classpropid=4";
 NamesSpectrs=ReadData("classvaltab","classval, clsid",Condition);
// clsidSpec.clear();



 for(int i=0;i<NamesSpectrs.size()/2;i++)
 {
//     QMessageBox msg;
//     msg.setText(NamesSpectrs[i*3+2]+"\n"+QString().setNum(NumLandScape));
//     msg.exec();
//  if(NamesSpectrs[i*3+2].at(NumLandScape)=='1')
//  {

   Val.Name=NamesSpectrs[i*2];
   Val.NameFieldNum="clsid";
   Val.Num=NamesSpectrs[i*2+1];
   ReturnList<<Val;
//  }
 }//for(int i=0;i<NamesSpectrs.size();i++)

 return ReturnList;//ReturnList;
}

QList<DNBDSpectr::NameAndClsIdFones> DNBDSpectr::ReadFones(QString NameClas,QString NameObj)
{
 QList <NameAndNum> ClsBackgroId;
 QList <QString> NamesBackgroid;
 QList <QString> NameBackgroid1,NameBackgroid2;
 QList < QList <QString> >ClsIdFones;
 QList <QString> ClsIdFone;

 QList <NameAndClsIdFones> Results;
 NameAndClsIdFones Result;

 ClsBackgroId=GetClsBackgroId(NameClas,NameObj);

 for(int i=0;i<ClsBackgroId.size();i++)
 {
  if(ClsBackgroId[i].Name!="0")
  {
   ClsIdFone.clear();
   NameBackgroid1=ReadData("classvaltab","classval","clsid="+ClsBackgroId[i].Name+" AND classpropid=4");
   ClsIdFone<<ClsBackgroId[i].Name;

   for(int j=0;j<ClsBackgroId.size();j++)
   {
    if(ClsBackgroId[i].Num==ClsBackgroId[j].Num && i!=j)
    {
     NameBackgroid2=ReadData("classvaltab","classval","clsid="+ClsBackgroId[j].Name+" AND classpropid=4");
     NameBackgroid1[0]+=','+NameBackgroid2[0];
     ClsIdFone<<ClsBackgroId[j].Name;
    }//
   }//for(j=0;j<ClsBackgroId.size();j++)
   NamesBackgroid<<NameBackgroid1[0];
   ClsIdFones<<ClsIdFone;
  }//if(ClsBackgroId[i].Name!="0")
 }//for(int i=0;i<ClsBackgroId.size();i++)

// QString Condition='(';
// Condition+=GenCondition("clsid",clsid,"OR");
// Condition+=") AND classpropid=4";


// NameBackgroid=ReadData("classvaltab","classval",Condition);

 for(int i=0;i<ClsBackgroId.size();i++)
 {
  if(ClsBackgroId[i].Name=="0")
  {
   NamesBackgroid<<"Фон отсутствует";
   ClsIdFone.clear();
   ClsIdFone<<"0";
   ClsIdFones<<ClsIdFone;
  }
 }

 //Убирание повторок
 for(int i=0;i<NamesBackgroid.size();i++)
 {
  int j=i+1;
  while(j<NamesBackgroid.size())
  {
   if(NamesBackgroid[i]==NamesBackgroid[j])
   {
    NamesBackgroid.removeAt(j);
    ClsIdFones.removeAt(j);
   }
   else
    j++;
  }//while(j<clsidSpec.size())
 }
 for(int i=0;i<NamesBackgroid.size();i++)
 {
  Result.NameFone=NamesBackgroid[i];
  Result.ClsIdsFone.clear();
  for(int j=0;j<ClsIdFones[i].size();j++)
   Result.ClsIdsFone<<ClsIdFones[i][j];
  Results<<Result;
 }
 return Results;
}
QList <int>  DNBDSpectr::ReadLandScape(QList <DNBDSpectr::NameAndNum> ClsIdStruct)
{
 QString Condition;
 QList <QString> LandScape;
 QList <int> Result;
 QList <QString> ClsId;
 ClsId.clear();
 for(int i=0;i<ClsIdStruct.size();i++)
 {
  ClsId<<ClsIdStruct[i].Num;
 }
 Condition=GenCondition("clsid",ClsId,"OR");
 LandScape=ReadData("generalobjectstab","objlandscape",Condition);
 for(int i=0; i<LandScape.size();i++)
 {
  int j=0;
  while(j!=-1)
  {
   j=LandScape[i].indexOf('1',j+1);
//   QMessageBox msg;
//   msg.setText(LandScape[i]+"\n"+QString().setNum(j));
//   msg.exec();
   if(j!=-1)
    Result<<j;
  }
 }

 /*Убираем повторки*/
 for(int i=0;i<Result.size();i++)
 {
  int j=i+1;
  while(j<Result.size())
  {
   if(Result[i]==Result[j])
    Result.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }
 return Result;
}
QList <int>  DNBDSpectr::ReadLandScape(QList <DNBDSpectr::NameAndNum> ClsIdStruct,int fenology)
{
 QString Condition;
 QList <QString> LandScape;
 QList <int> Result;
 QList <QString> ClsId;
 ClsId.clear();
 for(int i=0;i<ClsIdStruct.size();i++)
 {
  ClsId<<ClsIdStruct[i].Num;
 }
 Condition=GenCondition("clsid",ClsId,"OR");
 LandScape=ReadData("generalobjectstab","objlandscape","( "+Condition+" ) AND objfenology="+QString().setNum(fenology));
 for(int i=0; i<LandScape.size();i++)
 {
  int j=0;
  while(j!=-1)
  {
   j=LandScape[i].indexOf('1',j+1);
//   QMessageBox msg;
//   msg.setText(LandScape[i]+"\n"+QString().setNum(j));
//   msg.exec();
   if(j!=-1)
    Result<<j;
  }
 }

 /*Убираем повторки*/
 for(int i=0;i<Result.size();i++)
 {
  int j=i+1;
  while(j<Result.size())
  {
   if(Result[i]==Result[j])
    Result.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }
 return Result;
}


QString DNBDSpectr::GetNameLandScape(int idLandScape)
{
 QString LandScapeName;

 if(idLandScape==1)
  LandScapeName="Влажные тропические леса";
 if(idLandScape==2)
  LandScapeName="Широколиственные леса";
 if(idLandScape==3)
  LandScapeName="Тайга";
 if(idLandScape==4)
  LandScapeName="Степь";
 if(idLandScape==5)
  LandScapeName="Лесостепь";
 if(idLandScape==6)
  LandScapeName="Саванна";
 if(idLandScape==7)
  LandScapeName="Пустыня";
 if(idLandScape==8)
  LandScapeName="Тундра";
 if(idLandScape==9)
  LandScapeName="Лесотундра";
 if(idLandScape==10)
  LandScapeName="Умеренные леса";
 if(idLandScape==11)
  LandScapeName="Примитивные пустыни";
 return LandScapeName;
}

QList <SpecGraph> DNBDSpectr::ReadSpectrsObj(QString NameObj)
{
 QList <SpecGraph> Graphics;
 SpecGraph Graph;

 //Получаем идентификатор класса для указанных объектов
 QList <QString> clsid;
 clsid=ReadData("classvaltab","clsid","classval=\'"+NameObj+"\'");


 //Получаем идентификатор объекта
 QList <QString> objid;
 QString Condition;

 if(clsid.size()>1)
 {
  Condition=GenCondition("clsid",clsid,"OR");
  objid=ReadData("generalobjectstab","objid",Condition);
 }
 else
  objid=ReadData("generalobjectstab","objid","clsid="+clsid[0]);

 clsid.clear();

 //Получаем номер объектов в таблице КСЯ и значения КСЯ
 QList <QString> rangeid;
 QList <QString> KSJStr;
 QList <QString> LamStr;

 for(int i=0;i<objid.size();i++)
 {
  rangeid=ReadData("rangerelationksjtab","rangeid","objid="+objid[i]);

  for(int j=0; j<rangeid.size();j++)
  {
   KSJStr=ReadData("rangeksjtab","rangeksj","rangeid="+rangeid[j]);
   LamStr=ReadData("rangeksjtab","rangename","rangeid="+rangeid[j]);
   Graph.KSJ<<KSJStr[0].toFloat();
   Graph.Lam<<LamStr[0].toFloat();
   KSJStr.clear();
   LamStr.clear();
  }//for(int j=0; j<rangeid.size();j++)
  Graph.ObjId=objid[i];
  Graphics<<Graph;
  Graph.KSJ.clear();
  Graph.Lam.clear();
  rangeid.clear();
 }//for(int i=0;i<objid.size();i++)
 objid.clear();

 return Graphics;
}
SpecGraph DNBDSpectr::ReadSpectrsObj(quint64 ObjId)
{
 SpecGraph Graph;

 //Получаем идентификатор класса для указанных объектов
// QList <QString> clsid;
// clsid=ReadData("classvaltab","clsid","classval=\'"+NameObj+"\'");


 //Получаем идентификатор объекта
 QList <QString> objid;
// QString Condition;

// if(clsid.size()>1)
// {
////  Condition=GenCondition("clsid",clsid,"OR");
//  Condition
//  objid=ReadData("generalobjectstab","objid",Condition);
// }
// else
//  objid=ReadData("generalobjectstab","objid","clsid="+clsid[0]);

// clsid.clear();
 //Получаем номер объектов в таблице КСЯ и значения КСЯ
 objid<<QString().setNum(ObjId);
 QList <QString> rangeid;
 QList <QString> KSJStr;
 QList <QString> LamStr;

// for(int i=0;i<objid.size();i++)
// {
  rangeid=ReadData("rangerelationksjtab","rangeid","objid="+objid[0]);

  for(int j=0; j<rangeid.size();j++)
  {
   KSJStr=ReadData("rangeksjtab","rangeksj","rangeid="+rangeid[j]);
   LamStr=ReadData("rangeksjtab","rangename","rangeid="+rangeid[j]);
   Graph.KSJ<<KSJStr[0].toFloat();
   Graph.Lam<<LamStr[0].toFloat();
   KSJStr.clear();
   LamStr.clear();
  }//for(int j=0; j<rangeid.size();j++)
  Graph.ObjId=objid[0];
//  Graphics<<Graph;
//  Graph.KSJ.clear();
//  Graph.Lam.clear();
  rangeid.clear();
// }//for(int i=0;i<objid.size();i++)
 objid.clear();

 return Graph;
}
QList <SpecGraph> DNBDSpectr::ReadSpectrsObj(QString ClsIdObj,QList <QString> ClsBackgroId)
{
 QList <SpecGraph> Graphics;
 SpecGraph Graph;

 QList <QString> objid;
 QList <QString> BackGroId;
 BackGroId=ClsIdToBackgroId(ClsBackgroId);

 QString Condition=GenCondition("Backgroid",BackGroId,"OR");
 objid=ReadData("generalobjectstab","objid","(clsid="+ClsIdObj+") AND ("+Condition+')');

 //Получаем номер объектов в таблице КСЯ и значения КСЯ
 QList <QString> rangeid;
 QList <QString> KSJStr;
 QList <QString> LamStr;

 for(int i=0;i<objid.size();i++)
 {
  rangeid=ReadData("rangerelationksjtab","rangeid","objid="+objid[i]);

  for(int j=0; j<rangeid.size();j++)
  {
   KSJStr=ReadData("rangeksjtab","rangeksj","rangeid="+rangeid[j]);
   LamStr=ReadData("rangeksjtab","rangename","rangeid="+rangeid[j]);
   Graph.KSJ<<KSJStr[0].toFloat();
   Graph.Lam<<LamStr[0].toFloat();
   KSJStr.clear();
   LamStr.clear();
  }//for(int j=0; j<rangeid.size();j++)
  Graph.ObjId=objid[i];
  Graphics<<Graph;
  Graph.KSJ.clear();
  Graph.Lam.clear();
  rangeid.clear();
 }//for(int i=0;i<objid.size();i++)
 objid.clear();

 return Graphics;
}
QList <SpecGraph> DNBDSpectr::ReadSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,int ObjFenology)
{
 QList <SpecGraph> Graphics;
 SpecGraph Graph;

 QList <QString> objid;
 QList <QString> BackGroId;
 BackGroId=ClsIdToBackgroId(ClsBackgroId);

 QString Condition=GenCondition("Backgroid",BackGroId,"OR");
 objid=ReadData("generalobjectstab","objid","(clsid="+ClsIdObj+") AND ("+Condition+')'+" AND objfenology="+QString().setNum(ObjFenology));

 //Получаем номер объектов в таблице КСЯ и значения КСЯ
 QList <QString> rangeid;
 QList <QString> KSJStr;
 QList <QString> LamStr;

 for(int i=0;i<objid.size();i++)
 {
  rangeid=ReadData("rangerelationksjtab","rangeid","objid="+objid[i]);

  for(int j=0; j<rangeid.size();j++)
  {
   KSJStr=ReadData("rangeksjtab","rangeksj","rangeid="+rangeid[j]);
   LamStr=ReadData("rangeksjtab","rangename","rangeid="+rangeid[j]);
   Graph.KSJ<<KSJStr[0].toFloat();
   Graph.Lam<<LamStr[0].toFloat();
   KSJStr.clear();
   LamStr.clear();
  }//for(int j=0; j<rangeid.size();j++)
  Graph.ObjId=objid[i];
  Graphics<<Graph;
  Graph.KSJ.clear();
  Graph.Lam.clear();
  rangeid.clear();
 }//for(int i=0;i<objid.size();i++)
 objid.clear();
 return Graphics;
}
QList <SpecGraph> DNBDSpectr::ReadSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,int ObjFenology,int LandSpace)
{
 QList <SpecGraph> Graphics;
 SpecGraph Graph;

 QList <QString> objid;
 QList <QString> ObjAndLand;
 QList <QString> BackGroId;
 BackGroId=ClsIdToBackgroId(ClsBackgroId);

 QString Condition=GenCondition("Backgroid",BackGroId,"OR");
 ObjAndLand=ReadData("generalobjectstab","objid, objlandscape",
                    "(clsid="+ClsIdObj+") AND ("
                    +Condition+')'
                    +" AND objfenology="
                    +QString().setNum(ObjFenology));

 objid.clear();

 for(int i=0;i<ObjAndLand.size()/2;i++)
 {
//  QMessageBox msg;
//  msg.setText(ObjAndLand[i*2]+"\n"+ObjAndLand[i*2+1]+"\n"+QString().setNum(LandSpace));
//  msg.exec();
  int j=0;
  if(ObjAndLand[i*2+1].size()>LandSpace && ObjAndLand[i*2+1].at(LandSpace)=='1')
  {
   objid<<ObjAndLand[i*2];
  }
//  while(j!=-1)
//  {
//   j=ObjAndLand[i*2+1].indexOf('1',j+1);
//  }
 }

// QMessageBox msg;
// msg.setText(QString().setNum(objid.size()));
// msg.exec();


 //Получаем номер объектов в таблице КСЯ и значения КСЯ
 QList <QString> rangeid;
 QList <QString> KSJStr;
 QList <QString> LamStr;

 for(int i=0;i<objid.size();i++)
 {
  rangeid=ReadData("rangerelationksjtab","rangeid","objid="+objid[i]);

  for(int j=0; j<rangeid.size();j++)
  {
   KSJStr=ReadData("rangeksjtab","rangeksj","rangeid="+rangeid[j]);
   LamStr=ReadData("rangeksjtab","rangename","rangeid="+rangeid[j]);
   Graph.KSJ<<KSJStr[0].toFloat();
   Graph.Lam<<LamStr[0].toFloat();
   KSJStr.clear();
   LamStr.clear();
  }//for(int j=0; j<rangeid.size();j++)
  Graph.ObjId=objid[i];
  Graphics<<Graph;
  Graph.KSJ.clear();
  Graph.Lam.clear();
  rangeid.clear();
 }//for(int i=0;i<objid.size();i++)
 objid.clear();
 return Graphics;
}


QList <SpecGraph> DNBDSpectr::ReadPartSpectrsObj(QString ClsIdObj,QList<QString> ClsBackgroId,float LamStart,float LamFinish)
{
 QList <SpecGraph> Graphs;
 QList <SpecGraph> PartGraphics;
 SpecGraph GraphElement;

 Graphs=ReadSpectrsObj(ClsIdObj,ClsBackgroId);

 for(int i=0;i<Graphs.size();i++)
 {
  float DLam1, DLam2;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam1=LamStart-Graphs[i].Lam[j];
   DLam2=LamFinish-Graphs[i].Lam[j];

//   if(Graphs[i].Lam[j]>=700)
//   {
//    QMessageBox msg;
//    msg.setText(QString().setNum(Graphs[i].Lam[j],'d',3));
//    msg.exec();
//   }
   if((DLam1*(LamStart-Graphs[i].Lam[j+1])<=0))
   {
    NCh1=j;
   }
   if((DLam2*(LamFinish-Graphs[i].Lam[j+1])<=0))
   {
    NCh2=j;
   }
  }//for(int j=0;j<Graphs[i].Lam.size();j++)
  GraphElement.Lam.clear();
  GraphElement.KSJ.clear();
  if(DLam1<DLam2)
  {
   for(int j=NCh1+1;j<=NCh2;j++)
   {
    GraphElement.Lam<<Graphs[i].Lam[j];
    GraphElement.KSJ<<Graphs[i].KSJ[j];
   }//for(int j=NCh1;j<NCh2;j++)
   PartGraphics<<GraphElement;
  }//if(DLam1<DLam2)
 }//for(int i=0;i<Graphs.size();i++)
 return PartGraphics;
}

int DNBDSpectr::GetNumMeasureMent(SpecGraph Graph,float LamStart,float LamFinish)
{
 float DLam1, DLam2;
 int NCh1=0,NCh2=0;
 for(int j=0;j<Graph.Lam.size()-1;j++)
 {
  DLam1=LamStart-Graph.Lam[j];
  DLam2=LamFinish-Graph.Lam[j];

  if((DLam1*(LamStart-Graph.Lam[j+1])<=0))
  {
   NCh1=j;
  }
  if((DLam2*(LamFinish-Graph.Lam[j+1])<=0))
  {
   NCh2=j;
  }
 }//for(int j=0;j<Graphs[i].Lam.size();j++)
 int Num=NCh2-NCh1;
 return Num;
}

QList <float> DNBDSpectr::GetBrightLamda(float Lam,QString ClsId, QList<QString> ClsBackgroId)
{
 QList <SpecGraph> Graphs;
 QList <float> Brights;
 Graphs=ReadSpectrsObj(ClsId,ClsBackgroId);



 for(int i=0;i<Graphs.size();i++)
 {
  float DLam;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam=Lam-Graphs[i].Lam[j];
   NCh1=j;
   if((DLam*(Lam-Graphs[i].Lam[j+1])<0)||DLam==0)
   {
    NCh2=j+1;
    break;
   }//if(DLam*(Lam-Graphs[i].Lam[j+1])<0)
  }//for(int j=0;j<Graphs[i].Lam.size();j++)
   if(NCh1<NCh2)
  {
   Brights<<(((Lam-Graphs[i].Lam[NCh1])/(Graphs[i].Lam[NCh2]-Graphs[i].Lam[NCh1]))*(Graphs[i].KSJ[NCh2]-Graphs[i].KSJ[NCh1]))+Graphs[i].KSJ[NCh1];
  }//if(NumCh1<NumCh2)
  else
   Brights<<-1.;
 }//for(int i=0;i<Graphs.size();i++)
 return Brights;
}
QList <float> DNBDSpectr::GetBrightLamda(float Lam,QString ClsId, QList<QString> ClsBackgroId,int ObjFenology)
{
 QList <SpecGraph> Graphs;
 QList <float> Brights;
 Graphs=ReadSpectrsObj(ClsId,ClsBackgroId,ObjFenology);



 for(int i=0;i<Graphs.size();i++)
 {
  float DLam;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam=Lam-Graphs[i].Lam[j];
   NCh1=j;
   if((DLam*(Lam-Graphs[i].Lam[j+1])<0)||DLam==0)
   {
    NCh2=j+1;
    break;
   }//if(DLam*(Lam-Graphs[i].Lam[j+1])<0)
  }//for(int j=0;j<Graphs[i].Lam.size();j++)
   if(NCh1<NCh2)
  {
   Brights<<(((Lam-Graphs[i].Lam[NCh1])/(Graphs[i].Lam[NCh2]-Graphs[i].Lam[NCh1]))*(Graphs[i].KSJ[NCh2]-Graphs[i].KSJ[NCh1]))+Graphs[i].KSJ[NCh1];
  }//if(NumCh1<NumCh2)
  else
   Brights<<-1.;
 }//for(int i=0;i<Graphs.size();i++)
 return Brights;
}
float DNBDSpectr::GetBrightLamda(float Lam,quint64 ObjId)
{
 SpecGraph Graph;
 float Bright;
 Graph=ReadSpectrsObj(ObjId);



// for(int i=0;i<Graphs.size();i++)
// {
  float DLam;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graph.Lam.size()-1;j++)
  {
   DLam=Lam-Graph.Lam[j];
   NCh1=j;
   if((DLam*(Lam-Graph.Lam[j+1])<0)||DLam==0)
   {
    NCh2=j+1;
    break;
   }//if(DLam*(Lam-Graphs[i].Lam[j+1])<0)
  }//for(int j=0;j<Graphs[i].Lam.size();j++)
   if(NCh1<NCh2)
  {
   Bright=(((Lam-Graph.Lam[NCh1])/(Graph.Lam[NCh2]-Graph.Lam[NCh1]))*(Graph.KSJ[NCh2]-Graph.KSJ[NCh1]))+Graph.KSJ[NCh1];
  }//if(NumCh1<NumCh2)
  else
   Bright=-1.;
// }//for(int i=0;i<Graphs.size();i++)
 return Bright;
}


QList <float> DNBDSpectr::GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList <QString> ClsBackgroId)
{
 float Bright;
 QList <float> Brights;
 QList <SpecGraph> Graphs;
 Graphs=ReadSpectrsObj(ClsId,ClsBackgroId);

 for(int i=0;i<Graphs.size();i++)
 {

  float DLam1, DLam2;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam1=LamStart-Graphs[i].Lam[j];
   DLam2=LamFinish-Graphs[i].Lam[j];

   if((DLam1*(LamStart-Graphs[i].Lam[j+1])<=0))
   {
    NCh1=j;
   }
   if((DLam2*(LamFinish-Graphs[i].Lam[j+1])<=0))
   {
    NCh2=j;
   }
//   if(N1 && N2)
//    break;
  }//for(int j=0;j<Graphs[i].Lam.size();j++)

  if(DLam1<DLam2)
  {
   Bright=0;
   float DeltaL;
   for(int j=NCh1+1;j<=NCh2;j++)
   {
    if(NCh1!=0)
     DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j-1];
    else
    {
     if(NCh2<Graphs[i].Lam.size()-1 || j<NCh2)
      DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j+1];
    }
    Bright+=Graphs[i].KSJ[j]*DeltaL/(Graphs[i].Lam[NCh2]-Graphs[i].Lam[NCh1]);
   }//for(int j=NCh1;j<NCh2;j++)
   Brights<<Bright;
  }//if(DLam1<DLam2)
  else
   Brights<<-1.;

//  QMessageBox msg;
//  msg.setText(QString().setNum(LamStart,'d',3)+"\n"+QString().setNum(LamFinish,'d',3)+"\n"+QString().setNum(Brights[i],'d',3));
//  msg.exec();
 }//for(int i=0;i<Graphs.size();i++)
 return Brights;
}
QList <float> DNBDSpectr::GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList <QString> ClsBackgroId,int ObjFenology)
{
 float Bright;
 QList <float> Brights;
 QList <SpecGraph> Graphs;
 Graphs=ReadSpectrsObj(ClsId,ClsBackgroId,ObjFenology);

 for(int i=0;i<Graphs.size();i++)
 {
  float DLam1, DLam2;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam1=LamStart-Graphs[i].Lam[j];
   DLam2=LamFinish-Graphs[i].Lam[j];

   if((DLam1*(LamStart-Graphs[i].Lam[j+1])<=0))
   {
    NCh1=j;
   }
   if((DLam2*(LamFinish-Graphs[i].Lam[j+1])<=0))
   {
    NCh2=j;
   }
//   if(N1 && N2)
//    break;
  }//for(int j=0;j<Graphs[i].Lam.size();j++)



  if(DLam1<DLam2)
  {
   Bright=0;
   float DeltaL;
   for(int j=NCh1+1;j<=NCh2;j++)
   {
    if(NCh1!=0)
     DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j-1];
    else
    {
     if(NCh2<Graphs[i].Lam.size()-1 || j<NCh2)
      DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j+1];
    }
    Bright+=Graphs[i].KSJ[j]*DeltaL/(Graphs[i].Lam[NCh2]-Graphs[i].Lam[NCh1]);
   }//for(int j=NCh1;j<NCh2;j++)
   Brights<<Bright;
  }//if(DLam1<DLam2)
  else
   Brights<<-1.;
 }//for(int i=0;i<Graphs.size();i++)
 return Brights;
}
QList <float> DNBDSpectr::GetBrightIntegral(float LamStart,float LamFinish,QString ClsId,QList <QString> ClsBackgroId,int ObjFenology,int LandSpace)
{
 float Bright;
 QList <float> Brights;
 QList <SpecGraph> Graphs;
 Graphs=ReadSpectrsObj(ClsId,ClsBackgroId,ObjFenology,LandSpace);


 for(int i=0;i<Graphs.size();i++)
 {
  float DLam1, DLam2;
  int NCh1=0,NCh2=0;
  for(int j=0;j<Graphs[i].Lam.size()-1;j++)
  {
   DLam1=LamStart-Graphs[i].Lam[j];
   DLam2=LamFinish-Graphs[i].Lam[j];

   if((DLam1*(LamStart-Graphs[i].Lam[j+1])<=0))
   {
    NCh1=j;
   }
   if((DLam2*(LamFinish-Graphs[i].Lam[j+1])<=0))
   {
    NCh2=j;
   }
//   if(N1 && N2)
//    break;
  }//for(int j=0;j<Graphs[i].Lam.size();j++)



  if(DLam1<DLam2)
  {
   Bright=0;
   float DeltaL;
   for(int j=NCh1+1;j<=NCh2;j++)
   {
    if(NCh1!=0)
     DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j-1];
    else
    {
     if(NCh2<Graphs[i].Lam.size()-1 || j<NCh2)
      DeltaL=Graphs[i].Lam[j]-Graphs[i].Lam[j+1];
    }
    Bright+=Graphs[i].KSJ[j]*DeltaL/(Graphs[i].Lam[NCh2]-Graphs[i].Lam[NCh1]);
   }//for(int j=NCh1;j<NCh2;j++)
   Brights<<Bright;
  }//if(DLam1<DLam2)
  else
   Brights<<-1.;
 }//for(int i=0;i<Graphs.size();i++)
 return Brights;
}


QList <QString> DNBDSpectr::NameObjToBackgroid(QString BackgroName)
{
 QList <QString> clsid;
 clsid=ReadData("classvaltab","clsid","classval=\'Искусственные объекты\' AND classpropid=1");

 QString Condition='(';
 Condition+=GenCondition("clsid",clsid,"OR");
 Condition+=") AND classpropid=4 AND classval=\'"+BackgroName+"\'";
 clsid.clear();
 clsid=ReadData("classvaltab","classval",Condition);

 Condition=GenCondition("clsid",clsid,"OR");

 QList <QString> backgroid;
 backgroid=ReadData("backgroundtab","backgroid",Condition);
 clsid.clear();
 return backgroid;
}
QString DNBDSpectr::ClsIdToSubClass(QString ClsId)
{
 QString NameSubClass="";
 QList <QString> Res;
 Res=ReadData("classvaltab","classval","clsid="+ClsId+" AND classpropid=2");

 if(Res.size()>0)
  NameSubClass=Res[0];
 return NameSubClass;
}

//private функции
QList <QString> DNBDSpectr::GetClsId(QString NameClas,QString NameObj)
{
 QList <QString> clsid;

 /*Получаем в таблице id классов соответствующе заданному наименованию*/
 clsid=ReadData("classvaltab","clsid","classval=\'"+NameClas+"\' AND classpropid=1");

 QString Condition=GenCondition("clsid",clsid,"OR");//="clsid=";
 clsid.clear();

 /*Проверяем есть ли для соответствующих id спектральные данные*/
 QList <QString> clsidSpec;
 clsidSpec=ReadData("generalobjectstab","clsid",Condition);

 /*Убираем повторки*/
 for(int i=0;i<clsidSpec.size();i++)
 {
  int j=i+1;
  while(j<clsidSpec.size())
  {
   if(clsidSpec[i]==clsidSpec[j])
    clsidSpec.removeAt(j);
   else
    j++;
  }//while(j<clsidSpec.size())
 }

 /*Определяем имена объектов классификатора */
 QList <QString> ResultClsId;
 Condition='(';
 Condition+=GenCondition("clsid",clsidSpec,"OR");
 Condition+=") AND classpropid=4 AND classval=\'"+NameObj+"\'";
 ResultClsId=ReadData("classvaltab","clsid",Condition);
 clsidSpec.clear();

 return ResultClsId;
}

QList <QString> DNBDSpectr::GetClsId(QString NameObj)
{
 QString Condition;

 /*Определяем имена объектов классификатора */
 QList <QString> ResultClsId;
 Condition="classpropid=4 AND classval=\'"+NameObj+"\'";
 ResultClsId=ReadData("classvaltab","clsid",Condition);
 return ResultClsId;
}

QList <QString> DNBDSpectr::ClsIdToBackgroId(QList <QString> ClsId)
{
 QList <QList <QString> > DataBackgroId;
 QList <QString> Data;
 QList <QString> Result;


 for(int i=0;i<ClsId.size();i++)
 {
////     if(ClsIdObj=="2")
////     {
//      QMessageBox msg;
//      msg.setText(ClsId[i]);
//      msg.exec();
//  //   }
  Data=ReadData("backgroundtab","backgroid","clsid="+ClsId[i]);
  DataBackgroId<<Data;
//  if(ClsId[i]=="0")
//  {
//   Result<<"0";
//  }
 }//for(int i=0;i<ClsId.size();i++)

 if(DataBackgroId.size()>1)
 {
  bool *Sravn;
  Sravn=new bool[DataBackgroId.size()];

   for(int j=0;j<DataBackgroId[0].size();j++)
   {
    for(int n=0;n<DataBackgroId.size();n++)
     Sravn[n]=false;
    Sravn[0]=true;
    for(int i2=1;i2<DataBackgroId.size();i2++)
    {
     for(int j2=0;j2<DataBackgroId[i2].size();j2++)
     {
      if(DataBackgroId[i2][j2]==DataBackgroId[0][j])
      {
       Sravn[i2]=true;
      }
     }//for(int j2=0;j2<DataBackgroId[i].size();j2++)
    }//for(int i2=0;i2<DataBackgroId.size();i2++)
    int Kolv=0;
    for(int n=0;n<DataBackgroId.size();n++)
    {
     if(Sravn[n]==true)
      Kolv++;
    }
    if(Kolv==DataBackgroId.size())
     Result<<DataBackgroId[0][j];
   }//for(int j=0;j<DataBackgroId[i].size();j++)
    delete[] Sravn;
  }//if(DataBackgroId.size()>1)
  else
   Result=DataBackgroId[0];

 for(int i=0;i<ClsId.size();i++)
 {
  if(ClsId[i]=="0")
  {
   Result<<"0";
  }
 }//for(int i=0;i<ClsId.size();i++)

 return Result;
}

QList <DNBDSpectr::NameAndNum> DNBDSpectr::GetClsBackgroId(QString NameClas,QString NameObj)
{
 NameAndNum Var;
 QList <NameAndNum> Result;

 QList <QString> clsid;
 clsid=GetClsId(NameClas,NameObj);
 QString Condition;
 Condition=GenCondition("clsid",clsid,"OR");

 clsid.clear();
 QList <QString> backgroid;
 backgroid=ReadData("generalobjectstab","backgroid",Condition);
 for(int i=0;i<backgroid.size();i++)
 {
  if(backgroid[i]=="0")
  {
   Var.Name='0';
   Var.Num='0';
   Var.NameFieldNum="backgroid";
   Result<<Var;
   break;
  }
 }

 int n=0;
 while(n<backgroid.size())
 {
  if(backgroid[n]=="0")
   backgroid.removeAt(n);
  else
   n++;
 }//while(j<clsidSpec.size())


 Condition=GenCondition("backgroid",backgroid,"OR");
 QList <QString> ResultStr;
 ResultStr=ReadData("backgroundtab","clsid, backgroid",Condition);

 for(int i=0;i<ResultStr.size()/2;i++)
 {

  Var.Name=ResultStr[i*2];
  Var.Num=ResultStr[i*2+1];
  Var.NameFieldNum="backgroid";
  Result<<Var;
 }
 backgroid.clear();
 return Result;
}

QList <QString> DNBDSpectr::NameFoneToBackgroid(QString NameFone)
{
 QList <QString> ClsId;
 ClsId=GetClsId(NameFone);
 QString Condition=GenCondition("clsid",ClsId,"OR");
 QList <QString> Result;
 Result=ReadData("backgroundtab","backgroid",Condition);
 return Result;
}

QString DNBDSpectr::GenCondition(QString NameField,QList <QString> DataList,QString Operator)
{
 QString Condition=NameField+'=';
 for(int i=0;i<DataList.size();i++)
 {
  Condition+=DataList[i];
  if(i!=DataList.size()-1)
  {
   Condition+=' '+Operator+' '+NameField+'=';
  }//if(i!=clsid.size()-1)
 }//for(int i=0;i<clsid.size();i++)
 return Condition;
}
