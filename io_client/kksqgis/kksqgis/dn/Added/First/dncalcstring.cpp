#include "Added/First/dncalcstring.h"

DNCalcString::DNCalcString()
{
}

float DNCalcString::MainFunc(QString MainStr)
{
 CurPos=0;
 float n;
 n=expr(MainStr);
 return n;
}

int DNCalcString::FillNumChan(QString DataString)
{
 this->NumChan.clear();
 this->pos.clear();
 this->len.clear();
 int indexB,indexE,posStr;
 QString Data;

 posStr=0;
 do{
    indexB=DataString.indexOf('[',0);
    posStr+=indexB;
    if(indexB>=0)
     pos<<posStr;
    DataString.remove(0,indexB);
    indexE=DataString.indexOf(']',0);
    if(indexE>=0)
     len<<indexE+1;
    posStr+=indexE+1;

    Data=DataString.left(indexE+1);

    if(!Data.isEmpty())
    {
     Data.remove('[');
     Data.remove(']');
     if(Data.indexOf('-')<0)
     {
      this->NumChan<<Data.toInt()-1;
      this->ChB<<-5;
      this->ChE<<-5;
     }
     else
     {
      this->NumChan<<-5;
      QString Data2;
      Data2=Data.left(Data.indexOf('-'));
      this->ChB<<Data2.toInt()-1;
      Data.remove(0,Data.indexOf('-')+1);
      this->ChE<<Data.toInt()-1;
     }
    }

    DataString.remove(0,indexE+1);
   }while(indexB>=0);
 return pos.size();
}

QString DNCalcString::FillStringChan(QString DataString,float *Mass)
{
 int dLenStr=0,*ThisPos;

 ThisPos=new int[pos.size()];

 for(int i=0;i<pos.size();i++)
  ThisPos[i]=pos[i];

 for(int i=0;i<pos.size();i++)
 {
  dLenStr+=QString().setNum(Mass[i],'d',4).length()-len[i];
  DataString.replace(ThisPos[i],len[i],QString().setNum(Mass[i],'d',4));

  if(i<pos.size()-1)
   ThisPos[i+1]=ThisPos[i+1]+dLenStr;
  }
 delete[] ThisPos;
 return DataString;
}

float DNCalcString::number(QString Str)
{
 QString Data;
 int Num;
 bool IsCor=FALSE;
 float result = 0.0;
 float k = 10.0;
 int sign = 1;
 while (Str[CurPos]== ' ')
  CurPos++;
 if(Str[CurPos]=='-')
 {
  sign=-1;
  CurPos++;
 }
 while (true)
 {
  while (Str[CurPos]==' ')
   CurPos++;
  IsCor=FALSE;
  Data=Str[CurPos];
  Num=Data.toInt(&IsCor);
  if(IsCor)
  {
   result=result*10.0+Num;
   CurPos++;
  }
  else
  {
   break;
  }
 }

 if(Str[CurPos]=='.')
 {
  while(true)
  {
   CurPos++;
   IsCor=FALSE;
   Data=Str[CurPos];
   Num=Data.toInt(&IsCor);
   if(IsCor)
   {
    result+=(float)Num/k;
    k*=10.0;
   }//if(IsCor)
   else
   {
    break;
   }
  }//while(true)
 }//if(Str[CurPos]=='.')

 return sign*result;
}

float DNCalcString::factor(QString Str)
{
 float result=brackets(Str);
 float temp;
 QString Data;

 while (true)
 {
  while(Str[CurPos]== ' ')
   CurPos++;
  Data=Str[CurPos];
  switch(Str[CurPos].toAscii())
  {
   case '*':
    CurPos++;
    result *= brackets(Str);
   break;
   case '/':
    CurPos++;
    temp=brackets(Str);
    result/= temp;
   break;
   default:
    return result;
  }//switch (Str[CurPos])
 }//while (true)
}

float DNCalcString::expr(QString Str)
{
 float result=factor(Str);
 QString Data;
 while (true)
 {
  while(Str[CurPos]==' ')
   CurPos++;
  Data=Str[CurPos];
  switch(Str[CurPos].toAscii())
  {
   case '+':
    CurPos++;
    result+=factor(Str);
   break;
   case '-':
    CurPos++;
    result-=factor(Str);
   break;
   default:
    return result;
  }
 }
}

float DNCalcString::brackets(QString Str)
{
 float result=0;
 int sign=1;
 /*****************************************/
 //result=F_sqrt(Str);
 /*****************************************/

 while(Str[CurPos] == ' ')
  CurPos++;
 if (Str[CurPos]=='-')
 {
  sign = -1;
  CurPos++;
 }
 while (Str[CurPos]== ' ')
  CurPos++;
 if (Str[CurPos]=='(')
 {
  CurPos++;
  result = sign * expr(Str);
  CurPos++;
  return result;
 }
 if(Str[CurPos]=='s')
  return F_sqrt(Str);
// else
// {
  return sign * number(Str);
// }
}

float DNCalcString::F_sqrt(QString Str)
{
 float result;
 int sign=1;
 while(Str[CurPos] == ' ')
  CurPos++;
 if (Str[CurPos]=='-')
 {
  sign = -1;
  CurPos++;
 }
 while (Str[CurPos]== ' ')
  CurPos++;
 if (Str[CurPos]=='s')
 {
  CurPos+=5;
  result = /*sign * */expr(Str);
  CurPos++;
  result=sign*sqrt(result);
  return result;
 }
}


