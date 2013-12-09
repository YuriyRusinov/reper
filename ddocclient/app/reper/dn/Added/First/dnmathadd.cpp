#include "dnmathadd.h"

DNMathAdd::DNMathAdd()
{
}
float DNMathAdd::GetMax(float *Mass,unsigned long long RMass)
{
 float ReturnValue;
 unsigned long long  i;

 ReturnValue=Mass[0];
 for(i=1;i<RMass;i++)
 {
  if(ReturnValue<=Mass[i])
   ReturnValue=Mass[i];
 }
 return ReturnValue;
}

float DNMathAdd::GetMin(float *Mass,unsigned long long RMass)
{
 float ReturnValue;
 unsigned long long  i;

 ReturnValue=Mass[0];
 for(i=1;i<RMass;i++)
 {
  if(ReturnValue>=Mass[i])
   ReturnValue=Mass[i];
 }
 return ReturnValue;
}

/****************************************************************************/
float DNMathAdd::CalcMidleValue(float *Mass,unsigned long long RMass)
{
 unsigned long long i;
 float MidleValue;

 MidleValue=0;
 for(i=0;i<RMass;i++)
 {
  MidleValue+=(float)Mass[i]/RMass;
 }
 return MidleValue;
}

float DNMathAdd::CalcSKO(float *Mass,unsigned long long RMass)
{
 unsigned long long i;
 float SKOValue,Midle;

 Midle=this->CalcMidleValue(Mass,RMass);
 SKOValue=0;

 for(i=0;i<RMass;i++)
 {
  SKOValue+=((Mass[i]-Midle)*(Mass[i]-Midle))/RMass;
 }
 SKOValue=sqrt(SKOValue);
 return SKOValue;
}

float DNMathAdd::CalcEntropOtn(float *Mass,quint64 KDiskr,unsigned long long RMass)
{
 float MinM,MaxM;
 float I0,Entrop,EntropOtn,Kof;

 qint64 Var;
 float fVar;
 QQueue <qint64> SMass;
 QMap <int,float> pi; //Вероятности появления точки

 unsigned long long i,Razm;

 EntropOtn=-1;
 MinM=this->GetMin(Mass,RMass);
 MaxM=this->GetMax(Mass,RMass);

 //Вычисление максимально возможной энтропии
 I0=log(/*MaxM-MinM+1*/(long double)KDiskr)/log((long double)2);
 //Дискритизация массива
 Kof=KDiskr/(MaxM-MinM);
 for(i=0;i<RMass;i++)
 {
  fVar=(Mass[i]-MinM)*Kof;
  Var=(/*int*/qint64)fVar;
  SMass<<Var;
 }

 qSort(SMass);

 /*Вычисление вероятностей*/
 for(i=0;i<RMass;i++)
  pi[SMass[i]]=0;

 for(i=0;i<RMass;i++)
 {
  pi[SMass[i]]+=1./RMass;
 }//for(i=1;i<RMass;i++)

 Entrop=0;
 Razm=pi.size();
 QMap <int,float>::iterator it=pi.begin();
 for(;it!=pi.end();it++)
 {
  Kof=it.value();
  Entrop+=Kof*(log(Kof)/log(2.))*(-1);
 }
 EntropOtn=Entrop/I0;
 return EntropOtn;
 //(log(P[i])/log(2.)
}

void DNMathAdd::CalcStaticParam(float *Mass,quint64 RMass)
{
 DNMathAdd::Moments Val;
 double u2,u3,u4;

 Val=this->CalcMN(Mass,RMass);

 u2=Val.M2-(Val.M1*Val.M1);
 u3=Val.M3-3*Val.M1*Val.M2+2*Val.M1*Val.M1;
 u4=Val.M4-4*Val.M1*Val.M3+6*Val.M1*Val.M1*Val.M2-3*Val.M1*Val.M1*Val.M1*Val.M1;

 this->Prop.Midle=(float)Val.M1;
 this->Prop.SKO=(float)sqrt((double)u2);
 this->Prop.KofAssim=(float)u3/(this->Prop.SKO*this->Prop.SKO*this->Prop.SKO);
 this->Prop.KofEksc=(float)(u4/(this->Prop.SKO*this->Prop.SKO*this->Prop.SKO*this->Prop.SKO))-3;
}

float DNMathAdd::CalcEvklid(float *Mass1,float *Mass2, quint64 RMass)
{
 float ReturnV;
 ReturnV=0;
 for(quint64 i=0;i<RMass;i++)
 {
  ReturnV+=(Mass1[i]-Mass2[i])*(Mass1[i]-Mass2[i]);
 }//for(quint64 i=0;i<RMass;i++)
 ReturnV=sqrt(ReturnV);
 return ReturnV;
}

/*Функции работы с матрицами*/
void DNMathAdd::CalcCovMatr(int nMass /*размер массива*/,int nElem,double *Mass /*Массив*/,double *CovMatr /*Указатель на массив матрицы*/)
{
 /*
Массив Mass заполняется по принципу:
номер строки - номер элемента масива
ноер столбца - номер самого массива
Mass[nMass][nElem]
ковариационная матрица имеет размер nElem на nElem
*/
 int j_x=0,j_y=0,j_x2=0;
 double *sred,den;

 /*Вычисление средних для каждого массива*/
 sred=new double[nElem];

 for(j_x=0;j_x<nElem;j_x++)
 {
  sred[j_x]=0;
  for(j_y=0;j_y<nMass;j_y++)
  {
   sred[j_x]+=Mass[j_y+j_x*nMass]/nMass;
   den=Mass[j_y+j_x*nMass];
  }
  den=sred[j_x];
 }

/*Рассчёт элементов ковариационной матрицы*/
 for(j_x=0;j_x<nElem;j_x++)
 {
  for(j_y=0;j_y<nElem;j_y++)
  {
   CovMatr[j_x+j_y*nElem]=0;
   for(j_x2=0;j_x2<nMass;j_x2++)
   {
    CovMatr[j_x+j_y*nElem]+=((Mass[j_x2+j_x*nMass]-sred[j_x])*(Mass[j_x2+j_y*nMass]-sred[j_y])/nMass);
   }//for(j_x2=0;j_x2<nElem;j_x2=0)
  }//for(j_x=0;j_x<nMass;j_x++)
 }//for(j_y=0;j_y<nMass;j_y++)
 delete[] sred;
}

double DNMathAdd::CalcOpredMatr(int nMass /*размер массива*/,double *Matr /*Указатель на массив матрицы*/)
{
    double max,var,*a,Result;
    int i,j_x,j_y,NumString,znak;
    //ValueRangTeen Determinant;

    a=new double[nMass*nMass];
   // MatrString=new double[n];
    for(j_y=0;j_y<nMass;j_y++)
    {
     for(j_x=0;j_x<nMass;j_x++)
     {
      a[j_x+j_y*nMass]=Matr[j_x+j_y*nMass];
     }
    }

    znak=1;
    for(i=0;i<nMass;i++)
    {
   /*Ищем максимальный элемент в i-том столбце матрицы*/
     max=a[i+i*nMass];
     NumString=i;
     for(j_y=i;j_y<nMass;j_y++)
     {
      if(fabs(max)<fabs(a[i+j_y*nMass]))
      {
       max=a[i+j_y*nMass];
       NumString=j_y;
      }
     }
   /*Меняем строку, содержащую максимальный элемент матрицы в i-том столбце, с i-той строкой*/
     if(NumString!=i)
     {
      znak=znak*(-1);
      for(j_x=0;j_x<nMass;j_x++)
      {
       var=a[j_x+i*nMass];
       a[j_x+i*nMass]=a[j_x+NumString*nMass];
       a[j_x+NumString*nMass]=var;
      }
     }
     if(a[i+i*nMass]!=0)
     {
   /*Почленно прибавляем i-тую строку умноженную на коэффициент к последующим строкам*/
      for(j_y=i+1;j_y<nMass;j_y++)
      {
       var=(-1)*(a[i+j_y*nMass]/a[i+i*nMass]);
       for(j_x=0;j_x<nMass;j_x++)
       {
        a[j_x+(j_y*nMass)]=a[j_x+(j_y*nMass)]+var*a[j_x+i*nMass];
       }
      }

     }
    }

    Result=znak;
    //Determinant.TeenRange=0;
    for(i=0;i<nMass;i++)
    {
     Result=Result*a[i+(i*nMass)];
     /*
     if(Determinant.Result!=0)
     {
      while(fabs(Determinant.Result)<1.)
      {
       Determinant.Result*=10;
       Determinant.TeenRange--;
      }
      while(fabs(Determinant.Result)>1.)
      {
       Determinant.Result=Determinant.Result/10;
       Determinant.TeenRange++;
      }
     }
     */
    }

    delete[] a;
    return Result;
}

void DNMathAdd::MatrInverse(double *a,int n)
{
 double max,var;
 double *a2;
 int i,j_x,j_y,NumString,znak;

 a2=new double[n*n];
 for(j_y=0;j_y<n;j_y++)
 {
  for(j_x=0;j_x<n;j_x++)
  {
   a2[j_x+n*j_y]=0;
   if(j_x==j_y)
   {
    a2[j_x+n*j_y]=1;
   }
  }
 }

 znak=1;
 for(i=0;i<n;i++)
 {
/*Ищем максимальный элемент в i-том столбце матрицы*/
  max=a[i+i*n];

  NumString=i;
  for(j_y=i;j_y<n;j_y++)
  {
   if(fabs(max)<fabs(a[i+j_y*n]))
   {
    max=a[i+j_y*n];
    NumString=j_y;
   }
  }
/*Меняем строку, содержащую максимальный элемент матрицы в i-том столбце, с i-той строкой*/
  if(NumString!=i)
  {
   znak=znak*(-1);
   for(j_x=0;j_x<n;j_x++)
   {
    var=a[j_x+i*n];
    a[j_x+i*n]=a[j_x+NumString*n];
    a[j_x+NumString*n]=var;

    var=a2[j_x+i*n];
    a2[j_x+i*n]=a2[j_x+NumString*n];
    a2[j_x+NumString*n]=var;
   }
  }
  if(a[i+i*n]!=0)
  {
/*Почленно прибавляем i-тую строку умноженную на коэффициент к последующим строкам*/
   var=a[i+i*n];
   for(j_x=0;j_x<n;j_x++)
   {
    a[j_x+i*n]=a[j_x+i*n]/var;
    a2[j_x+i*n]=a2[j_x+i*n]/var;
   }

   for(j_y=i+1;j_y<n;j_y++)
   {
    var=(-1)*a[i+j_y*n];
    for(j_x=0;j_x<n;j_x++)
    {
     a2[j_x+(j_y*n)]=a2[j_x+(j_y*n)]+var*a2[j_x+i*n];
     a[j_x+(j_y*n)]=a[j_x+(j_y*n)]+var*a[j_x+i*n];
    }
   }
  }
 }


 for(i=0;i<n-1;i++)
 {
  for(j_y=0;j_y<n-i-1;j_y++)
  {
   var=a[(n-i-1)+(j_y*n)];
   for(j_x=0;j_x<n;j_x++)
   {
    a2[j_x+(j_y*n)]=a2[j_x+(j_y*n)]-a2[j_x+(n-i-1)*n]*var;
    a[j_x+(j_y*n)]=a[j_x+(j_y*n)]-a[j_x+(n-i-1)*n]*a[(n-i-1)+(j_y*n)];
   }
  }
 }

 for(j_y=0;j_y<n;j_y++)
 {
  for(j_x=0;j_x<n;j_x++)
  {
   a[j_x+n*j_y]=a2[j_x+n*j_y];
  }
 }
 delete a2;
}

void DNMathAdd::MatrTranspon(double *Matr,int x,int y)
{
 double *var,d;
 var=new double[y*x];
 for(int jy=0;jy<y;jy++)
 {
  for(int jx=0;jx<x;jx++)
  {
   d=Matr[jx+jy*x];
   var[jy+jx*y]=Matr[jx+jy*x];
  }//for(int jx=0;jx<x;jx++)
 }//for(int jy=0;jy<y;jy++)
 for(int jy=0;jy<y;jy++)
 {
  for(int jx=0;jx<x;jx++)
  {
   Matr[jx+jy*x]=var[jx+jy*x];
  }//for(int jx=0;jx<x;jx++)
 }

// for(int i=0;i<x*y;i++)
// {
//  QMessageBox msg;
//  msg.setText("x="+QString().setNum(Matr[i],'d',4));
//  msg.exec();
// }

 delete var;
}

double* DNMathAdd::MatrMulti(double *Matr1,int x1,int y1,double *Matr2,int x2,int y2)
{
 double *MatrRes;
 MatrRes=new double[x2*y1];

 for(int i=0;i<x2*y1;i++)
  MatrRes[i]=0;

 double d1,d2,d3;
 for(int jy=0;jy<y1;jy++)
 {
  for(int jx=0;jx<x2;jx++)
  {
   d3=0;
   for(int i=0;i<x1;i++)
   {
    d1=Matr1[i+jy*x1];
    d2=Matr2[jx+i*x2];
    MatrRes[jx+jy*x2]+=Matr1[i+jy*x1]*Matr2[jx+i*x2];
    d3+=Matr1[i+jy*x1]*Matr2[jx+i*x2];
   }
   d3=MatrRes[jx+jy*x2];
  }//for(jx=0;jx<x1;jx++)
 }//for(int jy=0;jy<y2;jy++)
 return MatrRes;
}

double* DNMathAdd::MatrMulti(double *Matr,int x,int y,double a)
{
 double *Res;
 Res=new double[x*y];
 for(int i=0;i<x*y;i++)
  Res[i]=Matr[i]*a;
 return Res;
}

void DNMathAdd::MatrAdd(double *Matr1,double *Matr2,int x,int y)
{
 for(int i=0;i<x*y;i++)
  Matr1[i]+=Matr2[i];
}

/*Статистика*/
double DNMathAdd::CalcCorel(quint64 nMass,float *Mass1,float *Mass2)
{
 double MMass1=0,MMass2=0;
 double Chisl=0,KvRazn1=0,KvRazn2=0;
 double KKor;

 for(quint64 i=0;i<nMass;i++)
 {
  MMass1+=Mass1[i]/nMass;
  MMass2+=Mass2[i]/nMass;
 }//for(quint64 i=0;i<nMass;i++)

 for(quint64 i=0;i<nMass;i++)
 {
  Chisl+=(Mass1[i]-MMass1)*(Mass2[i]-MMass2);
  KvRazn1+=(Mass1[i]-MMass1)*(Mass1[i]-MMass1);
  KvRazn2+=(Mass2[i]-MMass2)*(Mass2[i]-MMass2);
 }//for(quint64 i=0;i<nMass;i++)
 KKor=Chisl/sqrt(KvRazn1*KvRazn2);
 return KKor;
}

float DNMathAdd::CalcSpecAngle(int nMass,float *Mass1,float *Mass2)
{
 float ModMass1,ModMass2;
 float CosAlfa,Alfa;

 ModMass1=0;
 ModMass2=0;

 for(int i=0;i<nMass;i++)
 {
  ModMass1+=Mass1[i]*Mass1[i];
  ModMass2+=Mass2[i]*Mass2[i];
 }//for(int i=0;i<nMass;i++)
 ModMass1=sqrt(ModMass1);
 ModMass2=sqrt(ModMass2);

 CosAlfa=0;
 for(int i=0;i<nMass;i++)
 {
  CosAlfa+=Mass1[i]*Mass2[i];
 }//for(int i=0;i<nMass;i++)
 CosAlfa=CosAlfa/(ModMass1*ModMass2);
 Alfa=acos(CosAlfa);
 return Alfa;
}

float DNMathAdd::CalcModulVect(int nMass,float *Mass)
{
 float ModulVect;
 ModulVect=0;
 for(int i=0;i<nMass;i++)
 {
  ModulVect+=Mass[i]*Mass[i];
 }//for(int i=0;i<nMass;i++)
 ModulVect=sqrt(ModulVect);
 return ModulVect;
}

/*****************************************************************************************/

DNMathAdd::Moments DNMathAdd::CalcMN(float *Mass,quint32 RMass)
{
 quint32 i;
 DNMathAdd::Moments MValues;

 MValues.M1=0.;
 MValues.M2=0.;
 MValues.M3=0.;
 MValues.M4=0.;

 for(i=0;i<RMass;i++)
 {
  MValues.M1+=(double)Mass[i]/*/RMass*/;
  MValues.M2+=(double)(Mass[i]*Mass[i])/*/RMass*/;
  MValues.M3+=(double)(Mass[i]*Mass[i]*Mass[i])/*/RMass*/;
  MValues.M4+=(double)(Mass[i]*Mass[i]*Mass[i]*Mass[i])/*/RMass*/;
 }

 MValues.M1=MValues.M1/RMass;
 MValues.M2=MValues.M2/RMass;
 MValues.M3=MValues.M3/RMass;
 MValues.M4=MValues.M4/RMass;

 return MValues;
}
